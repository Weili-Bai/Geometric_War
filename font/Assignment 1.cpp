#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
bool isRX(const sf::Shape* shape, int windowW) {
    int x = shape->getPosition().x;
    float w = shape->getLocalBounds().width;
    bool leftHit = x <= 0;
    bool rightHit = x +  w >= windowW;
    return leftHit ||rightHit;
}
bool isRY(const sf::Shape* shape, int windowH) {
    int y = shape->getPosition().y;
    float h = shape->getLocalBounds().height;
    bool upHit = y <= 0;
    bool downHit = y + h >= windowH;
    return upHit || downHit;
}
bool isCX(const sf::Shape* shape, int windowW) {
    float x = shape->getPosition().x;
    float w = shape->getLocalBounds().width;
    bool leftHit = x <= 0;
    bool rightHit = x + w >= windowW;
    return leftHit || rightHit;
}
bool isCY(const sf::Shape* shape, int windowH) {
    float y = shape->getPosition().y;
    float h = shape->getLocalBounds().height;
    bool upHit = y <= 0;
    bool downHit = y + h >= windowH;
    return upHit || downHit;
}
bool isCCollision(const sf::Shape* shape, int windowW, int windowH,float radius) {
    int x = shape->getPosition().x;
    int y = shape->getPosition().y;
    bool leftHit = x <= 0;
    bool upHit = y <= 0;
    bool rightHit = x + 2 * radius >= windowW;
    bool downHit = y + 2 * radius >= windowH;
    return leftHit||upHit||rightHit||downHit;
}
std::vector<int>getWindowFont(std::vector<int>& fontSize, std::ifstream& input,std::string&fontName) {
    input.open("config_Assignment1.txt");
    std::vector<int>result;
    std::string s;
    std::string temp;
    int r, g, b, size, width, height;

    while (!input.eof()) {
        getline(input, s);
        std::istringstream line(s);
        line >> s;
        switch (s[0]) {
        case'w':
        case'W':
            line >> width >> height;
            break;
        case'f':
        case'F':
            line >> temp >> size >> r >> g >> b;
            fontName += temp;
            break;
        }
    }
    input.close();
    result.push_back(width);
    result.push_back(height);
    fontSize[0] = size;
    fontSize[1] = r;
    fontSize[2] = g;
    fontSize[3] = b;
    return result;
}
void differShape(std::ifstream& input,std::vector<std::string>&circle,std::vector<std::string>&rectangle) {
    input.open("config_Assignment1.txt");
    std::string line;
    while (!input.eof()) {
        getline(input, line);
        switch (line[0]) {
        case 'C':
        case 'c':
            circle.push_back(line);
            break;
        case 'R':
        case 'r':
            rectangle.push_back(line);
        default:
            break;
        }
    }
    input.close();
}
std::vector<std::pair<sf::Shape*, sf::Text*>>makeRegtangle(const std::vector<std::string>&regtangle,std::vector<std::pair<float,float>>&regtangleSpeed,const sf::Font& f) {
    std::vector<std::pair<sf::Shape*,sf::Text*>>result;
    std::string dummy, name;
    int x, y, r, g, b;
    float speedX, speedY, width, height;
    for (auto i = regtangle.begin(); i != regtangle.end(); i++) {
        std::istringstream s(*i);
        s >> dummy >>name>> x >> y >> speedX >> speedY >> r >> g >> b >> width >> height;
        std::pair<float, float>p(speedX, speedY);
        regtangleSpeed.push_back(p);
        sf::Shape* newReg = new sf::RectangleShape(sf::Vector2f{ width,height });
        newReg->setFillColor(sf::Color(r, g, b));
        newReg->setPosition(x, y);
        sf::Text* text = new sf::Text;
        text->setFont(f);
        text->setString(name);
        float originX = text->getLocalBounds().width;
        float originY = text->getLocalBounds().height;
        text->setOrigin(originX / 2, originY);
        text->setPosition(x + 0.5 * width, y + 0.5 * height);
        std::pair<sf::Shape*, sf::Text*>q(newReg, text);
        result.push_back(q);
    }
    return result;
}
std::vector<std::pair<sf::Shape*, sf::Text*>>makeCircle(const std::vector<std::string>& circle,std::vector<std::pair<float,float>>&circleSpeed, const sf::Font& f) {
    std::vector<std::pair<sf::Shape*, sf::Text*>>result;
    std::string dummy, name;
    float x, y, speedX, speedY,radius;
    int r, g, b;
    for (auto i = circle.begin(); i != circle.end(); i++) {
        std::istringstream line(*i);
        line >> dummy >>name>> x >> y >> speedX >> speedY >> r >> g >> b >> radius;
        std::pair<float, float>p(speedX, speedY);
        circleSpeed.push_back(p);
        sf::Text* text = new sf::Text;
        text->setString(name);
        text->setFont(f);
        float originX = text->getLocalBounds().width;
        float originY = text->getLocalBounds().height;
        text->setOrigin(originX / 2, originY);
        text->setPosition(x +  radius, y +  radius);
        sf::Shape* newCircle = new sf::CircleShape(radius);
        newCircle->setFillColor(sf::Color(r, g, b));
        newCircle->setPosition(x, y);
        std::pair<sf::Shape*, sf::Text*>q(newCircle, text);
        result.push_back(q);
    }
    return result;
}
int main()
{
    sf::Shape* shape;
    std::ifstream input;
    std::vector<std::string> circle, regtangle;
    std::vector<std::pair<float, float>>rSpeed, cSpeed;
    differShape(input, circle, regtangle);
    
    
    shape = new sf::CircleShape(50);
    int r = 200;
    int g = 146;
    int b = 54;
    
    std::vector<int>list;
    std::vector<int>fontInfo(4);
    std::string fontName="font/";
    sf::Font myFont;
    list = getWindowFont(fontInfo, input, fontName);
    if (!myFont.loadFromFile(fontName)) {
        std::cout << "Font loading failed! Program Terminates!" << std::endl;
        exit(1);
    }
    
    sf::RenderWindow window(sf::VideoMode(list[0], list[1]), "Initial Work");
    shape->setFillColor(sf::Color(r,g,b));
   // window.setFramerateLimit(60);
    float xSpeed = 1.5f/10;
    float ySpeed = 3.0f/10;
    shape->setPosition(590.0f, 310.0f);
    sf::Text text("Cute Wife", myFont, fontInfo[0]);
    text.setPosition(640.0f-9*4.5f,360.0f-9.0f);
    text.setFillColor(sf::Color(fontInfo[1], fontInfo[2], fontInfo[3]));
    std::vector<std::pair<sf::Shape*, sf::Text*>>regtangleShape = makeRegtangle(regtangle, rSpeed, myFont);
    std::vector<std::pair<sf::Shape*, sf::Text*>>circleShape = makeCircle(circle, cSpeed,myFont);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {   
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (shape->getPosition().x <= 0 || shape->getPosition().x >= 1180) {
            xSpeed = 0 - xSpeed;
        }
        if (shape->getPosition().y <= 0 || shape->getPosition().y >= 620) {
            ySpeed = 0 - ySpeed;
        }
        shape->setPosition(shape->getPosition().x - xSpeed, shape->getPosition().y + ySpeed);
        text.setPosition(text.getPosition().x - xSpeed, text.getPosition().y + ySpeed);
        window.clear();
        window.draw(*shape);
        for (int i = 0; i < regtangleShape.size(); i++) {
            sf::Shape* reg = regtangleShape[i].first;
            sf::Text* text = regtangleShape[i].second;
            window.draw(*reg);
            window.draw(*text);
            float speedX = rSpeed[i].first;
            float speedY= rSpeed[i].second;
            float shapeX = reg->getPosition().x;
            float shapeY = reg->getPosition().y;
            float textX = text->getPosition().x;
            float textY = text->getPosition().y;
            reg->setPosition(shapeX + speedX, shapeY + speedY);
            text->setPosition(textX + speedX, textY + speedY);
            if (isRX(reg, list[0])) {
                rSpeed[i].first = -rSpeed[i].first;
            }
            if (isRY(reg, list[1])) {
                rSpeed[i].second = -rSpeed[i].second;
            }
        }
        for (int i = 0; i < circleShape.size(); i++) {
            sf::Shape* circle = circleShape[i].first;
            sf::Text* text = circleShape[i].second;
            window.draw(*circle);
            window.draw(*text);
            float speedX = cSpeed[i].first;
            float speedY = cSpeed[i].second;
            float shapeX = circle->getPosition().x;
            float shapeY = circle->getPosition().y;
            float textX = text->getPosition().x;
            float textY = text->getPosition().y;
            circle->setPosition(shapeX + speedX, shapeY + speedY);
            text->setPosition(textX + speedX, textY + speedY);
            if (isCX(circle, list[0])) {
                cSpeed[i].first = -cSpeed[i].first;
            }
            if (isCY(circle, list[1])) {
                cSpeed[i].second = -cSpeed[i].second;
            }
        }
        window.draw(text);
        window.display();
    }
    return 0;
}