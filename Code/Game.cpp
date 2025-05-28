#include "Game.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <cmath>
const float PI = 3.1415926;
const float SQRT2 = sqrt(2);
const int BIG_SCORE = 100;
const int SMALL_SCORE = 200;
const int SPECIAL_COOLDOWN = 360;
const int SPECIAL_LAST_TIME = 60;
Game::Game(const std::string& config) {
	init(config);
	m_score = 0;
	m_currentFrame = 0;
	m_lastEnemySpawnTime = 0;
	m_paused = false;
	m_running = true;
}
float Game::getRad(float deg) {
	return deg / 180 * PI;
}
void Game::init(const std::string& path) {
	std::ifstream input;
	WindowConfig window;
	FontConfig font;
	std::string s,dummy;
	input.open(path);
	while (!input.eof()) {
		getline(input, s);
		std::istringstream line(s);
		switch (s[0]) {
		case'w':
		case 'W':
			line >> dummy >> window.width >> window.height >> window.frame >> window.isFullScreen;
			break;
		case 'f':
		case 'F':
			line >> dummy >> font.name >> font.size >> font.r >> font.g >> font.b;
			break;
		case 'p':
		case 'P':
			line >> dummy >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
			break;
		case'e':
		case 'E':
			line >> dummy >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX>> m_enemyConfig.L>> m_enemyConfig.SI;
			break;
		case 'b':
		case 'B':
			line >> dummy >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
			break;
		default:
			std::cout << "Wrong info in the config file! Program ends!" << std::endl;
			exit(-1);
		}
	}
	input.close();
	if (window.isFullScreen) {
		m_window.create(sf::VideoMode(window.width,window.height),  "Assignment 2",sf::Style::Fullscreen );
	}
	else {
		m_window.create(sf::VideoMode(window.width, window.height), "Assignment 2");
	}
	m_window.setFramerateLimit(window.frame);
	if (!m_font.loadFromFile(font.name)) {
		std::cout << "Font not found! Program ends!" << std::endl;
		exit(-1);
	}
	m_text = sf::Text("Score: ", m_font, font.size);
	m_text.setFillColor(sf::Color(font.r, font.g, font.b));
	m_text.setPosition(m_text.getLocalBounds().width / 2, m_text.getLocalBounds().height / 2);
	m_specialProgress = sf::Text("Special Weapon Ready!", m_font, font.size);
	m_specialProgress.setFillColor(sf::Color(0, 255, 0));
	//TODOm_specialProgress.setPosition({});
	spawnPlayer();
}
void Game::run() {
	while (m_running) {
		if (!m_paused) {
			m_entites.update();
			sEnemySpawner();
			sMovement();
			sCollision();
			sLifespan();
		}
		sUserInput();
		sRender();
		speicalWeaponHelper();
		//increment the current frame
		// may need to be moved when pause implemented.
		m_currentFrame++;
	}
}
void Game::setPaused(bool paused) {
	m_paused = paused;
}
void Game::spawnPlayer() {
	auto entity = m_entites.addEntity("player");
	float cx = m_window.getSize().x / 2.0f;
	float cy = m_window.getSize().y / 2.0f;
	entity->transform = std::make_shared<CTransform>(Vec2(cx, cy), Vec2(0, 0), 0);
	entity->shape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V,sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
	entity->input = std::make_shared<CInput>();
	entity->collision = std::make_shared<CCollision>(m_playerConfig.CR);
	entity->hitpoint = std::make_shared<CHitPoint>(1);
	m_player = entity;
}
void Game::spawnEnemy() {
	auto entity = m_entites.addEntity("enemy");
	float ratio = (0 + rand() % 101) / 100.0f;
	float lower = m_enemyConfig.SR;
	float xUpper = m_window.getSize().x - lower;
	float yUpper = m_window.getSize().y - lower;
	float px = lower + ratio * (xUpper - lower);
	ratio = (0 + rand() % 101) / 100.0f;
	float py = lower + ratio * (yUpper - lower);
	float playerX = m_player->transform->position.x;
	float playerY = m_player->transform->position.y;
	float playerR = m_player->collision->radius;
	Vec2 player(playerX, playerY);
	Vec2 enemy(px, py);
	while (enemy.distance(player) <= 2 * playerR) {
		ratio = (0 + rand() % 101) / 100.0f;
		px = lower + ratio * (xUpper - lower);
		ratio = (0 + rand() % 101) / 100.0f;
		py = lower + ratio * (yUpper - lower);
		enemy = Vec2(px, py);
	}
	float minSpeed = m_enemyConfig.SMIN;
	float maxSpeed = m_enemyConfig.SMAX;
	float speed = minSpeed + ratio * (maxSpeed - minSpeed);
	float rad = getRad(0 + rand() % 361);
	float vx = speed * cos(rad);
	float vy = speed * sin(rad);
	int isPositive = 0 + rand() % 2;
	if (!isPositive)
		vx = -vx;
	isPositive = 0 + rand() % 2;
	if (!isPositive)
		vy = -vy;
	entity->transform = std::make_shared<CTransform>(Vec2(px, py), Vec2(vx, vy), 0);
	int side = m_enemyConfig.VMIN + rand() % m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1;
	int r = 0 + rand() % 256;
	int g = 0 + rand() % 256;
	int b = 0 + rand() % 256;
	entity->shape = std::make_shared<CShape>(m_enemyConfig.SR, side,sf::Color(r, g, b), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);
	entity->collision = std::make_shared<CCollision>(m_enemyConfig.CR);
	entity->score = std::make_shared<CScore>(BIG_SCORE);
	entity->hitpoint = std::make_shared<CHitPoint>(side / 2);
	m_lastEnemySpawnTime = m_currentFrame;
}
void Game::spawnSmallEnemies(std::shared_ptr<Entity>e) {
	int quantity = e->shape->shape.getPointCount();
	float spread = 360.0 / quantity;
	for (int i = 0; i < quantity; i++) {
		auto entity = m_entites.addEntity("smallEnemy");
		Vec2 position(e->transform->position.x, e->transform->position.y);
		float vx = e->transform->velocity.x;
		float vy = e->transform->velocity.y;
		float speed = sqrt(vx * vx + vy * vy);
		float direction = getRad(i * spread);
		vx = speed * cos(direction);
		vy = speed * sin(direction);
		Vec2 velocity(vx, vy);
		entity->transform = std::make_shared<CTransform>(position, velocity, 0);
		float radius = e->shape->shape.getRadius();
		sf::Color fill = e->shape->shape.getFillColor();
		sf::Color outline = e->shape->shape.getOutlineColor();
		float thickness = e->shape->shape.getOutlineThickness() * 0.5;
		entity->shape = std::make_shared<CShape>(radius*0.5, quantity, fill, outline, thickness);
		entity->collision = std::make_shared<CCollision>(m_enemyConfig.CR * 0.5);
		entity->lifespan = std::make_shared<CLifeSpan>(m_enemyConfig.L);
		entity->score = std::make_shared<CScore>(SMALL_SCORE);
		entity->hitpoint = std::make_shared<CHitPoint>(1);
	}
}
void Game::spawnBullet(std::shared_ptr<Entity> player, const Vec2& target) {
	auto bullet = m_entites.addEntity("bullet");
	Vec2 start = Vec2(player->shape->shape.getPosition().x, player->shape->shape.getPosition().y);
	Vec2 direction = target - start;
	direction.normalize();
	direction *= m_bulletConfig.S;
	bullet->transform = std::make_shared<CTransform>(start, direction, 0);
	bullet->shape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V,sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB,1), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	bullet->collision = std::make_shared<CCollision>(m_bulletConfig.CR);
	bullet->lifespan = std::make_shared<CLifeSpan>(m_bulletConfig.L);
}
void Game::spawnScore() {
	std::string curr = "Score: ";
	curr += std::to_string(m_score);
	m_text.setString(curr);
	m_window.draw(m_text);
}
void Game::speicalWeaponHelper() {
	if (!isSpecialReady&& m_currentFrame - m_lastSpecial >= SPECIAL_COOLDOWN) {
		isSpecialReady = true;
	}
}
void Game::spawnSpecialWeapon(std::shared_ptr<Entity>e, const Vec2& target) {
	if (isSpecialReady) {
		isSpecialReady = false;
		m_lastSpecial = m_currentFrame;
		Vec2 origin(e->transform->position.x, e->transform->position.y);
		Vec2 direction = target - origin;
		direction.normalize();
		float radius = e->shape->shape.getRadius();
		float windowX = m_window.getSize().x;
		float windowY = m_window.getSize().y;
		while (origin.x >= 0 && origin.x <= windowX && origin.y >= 0 && origin.y <= windowY) {
			auto special = m_entites.addEntity("special");
			special->transform = std::make_shared<CTransform>(origin, Vec2(0, 0), 0);
			special->shape = std::make_shared<CShape>(radius, 30, sf::Color(255, 0, 0), sf::Color(255, 0, 0), 1);
			special->lifespan = std::make_shared<CLifeSpan>(SPECIAL_LAST_TIME);
			special->collision = std::make_shared<CCollision>(radius);
			origin.x += direction.x * radius;
			origin.y += direction.y * radius;
		}
	}
}
void Game::playerMovement() {
	float speed = m_playerConfig.S;
	if (m_player->input->up) {
		m_player->transform->velocity.y = -speed;
	}
	if (m_player->input->down) {
		m_player->transform->velocity.y = speed;
	}
	if (m_player->input->left) {
		m_player->transform->velocity.x = -speed;
	}
	if (m_player->input->right) {
		m_player->transform->velocity.x = speed;
	}
	if (!m_player->input->up && !m_player->input->down) {
		m_player->transform->velocity.y = 0;
	}
	if (m_player->input->up && m_player->input->down) {
		m_player->transform->velocity.y = 0;
	}
	if (!m_player->input->left && !m_player->input->right) {
		m_player->transform->velocity.x = 0;
	}
	if (m_player->input->left && m_player->input->right) {
		m_player->transform->velocity.x = 0;
	}
	if (m_player->input->left && m_player->input->up) {
		m_player->transform->velocity.x = -speed / SQRT2;
		m_player->transform->velocity.y = -speed / SQRT2;
	}
	if (m_player->input->left && m_player->input->down) {
		m_player->transform->velocity.x = -speed / SQRT2;
		m_player->transform->velocity.y = speed / SQRT2;
	}
	if (m_player->input->right && m_player->input->up) {
		m_player->transform->velocity.x = speed / SQRT2;
		m_player->transform->velocity.y = -speed / SQRT2;
	}
	if (m_player->input->right && m_player->input->down) {
		m_player->transform->velocity.x = speed / SQRT2;
		m_player->transform->velocity.y = speed / SQRT2;
	}
	float radius = m_player->shape->shape.getRadius();
	float windowX = m_window.getSize().x;
	float windowY = m_window.getSize().y;
	float vx = m_player->transform->velocity.x;
	float vy = m_player->transform->velocity.y;
	if (m_player->transform->position.x >= radius && vx < 0) {
		m_player->transform->position.x += m_player->transform->velocity.x;
	}
	if (m_player->transform->position.x <= windowX - radius && vx > 0) {
		m_player->transform->position.x += m_player->transform->velocity.x;
	}
	if (m_player->transform->position.y <= windowY - radius && vy > 0) {
		m_player->transform->position.y += m_player->transform->velocity.y;
	}
	if (m_player->transform->position.y >= radius && vy < 0) {
		m_player->transform->position.y += m_player->transform->velocity.y;
	}
}
void Game::enemyMovement(){
	float windowX = m_window.getSize().x;
	float windowY= m_window.getSize().y;
	EntityVector enemies = m_entites.getEntities("enemy");
	for (auto e : enemies) {
		e->transform->position.x += e->transform->velocity.x;
		e->transform->position.y += e->transform->velocity.y;
		float radius = e->shape->shape.getRadius();
		if (e->transform->position.x <= radius || e->transform->position.x >= windowX - radius)
			e->transform->velocity.x = -e->transform->velocity.x;
		if (e->transform->position.y <= radius || e->transform->position.y >= windowY - radius)
			e->transform->velocity.y = -e->transform->velocity.y;
	}
}
void Game::bulletMovement() {
	EntityVector bullets = m_entites.getEntities("bullet");
	for (auto b : bullets) {
		b->transform->position.x += b->transform->velocity.x;
		b->transform->position.y += b->transform->velocity.y;
	}
}
void Game::smallEnemyMovement() {
	float windowX = m_window.getSize().x;
	float windowY = m_window.getSize().y;
	EntityVector smallEnemies = m_entites.getEntities("smallEnemy");
	for (auto s : smallEnemies) {
		s->transform->position.x += s->transform->velocity.x;
		s->transform->position.y += s->transform->velocity.y;
		float radius = s->shape->shape.getRadius();
		if (s->transform->position.x <= radius || s->transform->position.x >= windowX - radius)
			s->transform->velocity.x = -s->transform->velocity.x;
		if (s->transform->position.y <= radius || s->transform->position.y >= windowY - radius)
			s->transform->velocity.y = -s->transform->velocity.y;
	}
}
void Game::sMovement() {
	playerMovement();
	enemyMovement();
	bulletMovement();
	smallEnemyMovement();
}
void Game::sLifespan() {
	for (auto e : m_entites.getEntities()) {
		if (e->lifespan != nullptr) {
			if (e->lifespan->remaining > 0) {
				e->lifespan->remaining--;
				sf::Color c = e->shape->shape.getFillColor();
				sf::Color d = e->shape->shape.getOutlineColor();
				c.a = 255.0 * e->lifespan->remaining / e->lifespan->total;
				d.a = c.a;
				e->shape->shape.setFillColor(c);
				e->shape->shape.setOutlineColor(d);
			}
			else {
				e->destroy();
			}
		}
	}
}
void Game::sCollision() {
	float playerCollisionRadius = m_player->collision->radius;
	Vec2 playerPosition(m_player->transform->position.x, m_player->transform->position.y);
	EntityVector enemies = m_entites.getEntities("enemy");
	EntityVector smallEnemies = m_entites.getEntities("smallEnemy");
	for (auto s : smallEnemies) {
		enemies.push_back(s);
	}
	for (auto e : enemies) {
		float enemyCollsionRadius = e->collision->radius;
		Vec2 ePosition(e->transform->position.x, e->transform->position.y);
		if (playerPosition.distance(ePosition) <= playerCollisionRadius + enemyCollsionRadius) {
			if(e->tag()=="enemy")
				spawnSmallEnemies(e);
			e->destroy();
			m_player->destroy();
			spawnPlayer();
		}
		for (auto b : m_entites.getEntities("bullet")) {
			float bulletCollisionRadius = b->collision->radius;
			Vec2 bPosition(b->transform->position.x, b->transform->position.y);
			if (ePosition.distance(bPosition) <= enemyCollsionRadius + bulletCollisionRadius) {
				b->destroy();
				e->hitpoint->hp--;
				if (e->hitpoint->hp < 1) {
					m_score += e->score->score * e->shape->shape.getPointCount();
					if (e->tag() == "enemy") {
						spawnSmallEnemies(e);
					}
					e->destroy();
				}
			}
		}
		for (auto s : m_entites.getEntities("special")) {
			float specialCollisionRadius = s->collision->radius;
			Vec2 sPosition(s->transform->position.x, s->transform->position.y);
			if (ePosition.distance(sPosition) <= enemyCollsionRadius + specialCollisionRadius) {
				e->hitpoint->hp -= 10;
				if (e->hitpoint->hp<1) {
					m_score += e->score->score * e->shape->shape.getPointCount();
					if (e->tag() == "enemy")
						spawnSmallEnemies(e);
					e->destroy();
				}
			}
		}
	}
}
void Game::sEnemySpawner(){
	int time = m_enemyConfig.SI;
	if (m_currentFrame - m_lastEnemySpawnTime >= time) {
		spawnEnemy();
	}
}
void Game::sRender() {
		m_window.clear();
		for (auto e : m_entites.getEntities()) {
			e->shape->shape.setPosition(e->transform->position.x, e->transform->position.y);
			e->transform->angle += 1.0f;
			e->shape->shape.setRotation(e->transform->angle);
			m_window.draw(e->shape->shape);
		}
		spawnScore();
		m_window.display();
}
void Game::sUserInput() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case sf::Keyboard::W:
				std::cout << "W key pressed" << std::endl;
				m_player->input->up = true;
				break;
			case sf::Keyboard::S:
				std::cout << "S key pressed" << std::endl;
				m_player->input->down = true;
				break;
			case sf::Keyboard::A:
				std::cout << "A key pressed" << std::endl;
				m_player->input->left = true;
				break;
			case sf::Keyboard::D:
				std::cout << "D key pressed" << std::endl;
				m_player->input->right = true;
				break;
			case sf::Keyboard::P:
				setPaused(!m_paused);
				if (m_paused) {
					std::cout << "Game paused!" << std::endl;
				}
				else {
					std::cout << "Game resumed!" << std::endl;
				}
				break;
			case sf::Keyboard::Escape:
				m_window.close();
				std::cout << "Game closed!" << std::endl;
				exit(0);
			default:break;
			}
		}
		if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code) {
			case sf::Keyboard::W:
				std::cout << "W key released" << std::endl;
				m_player->input->up = false;
				break;
			case sf::Keyboard::S:
				std::cout << "S key released" << std::endl;
				m_player->input->down = false;
				break;
			case sf::Keyboard::A:
				std::cout << "A key released" << std::endl;
				m_player->input->left = false;
				break;
			case sf::Keyboard::D:
				std::cout << "D key released" << std::endl;
				m_player->input->right = false;
				break;
			}
		
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				std::cout << "Left Mouse button cliced at: " << event.mouseButton.x << ',' << event.mouseButton.y << std::endl;
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}
			if (event.mouseButton.button == sf::Mouse::Right) {
				std::cout << "Right Mouse button cliced at: " << event.mouseButton.x << ',' << event.mouseButton.y << std::endl;
				spawnSpecialWeapon(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}
		}
	}
}