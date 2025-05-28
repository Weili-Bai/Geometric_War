#ifndef GAME_H
#define GAME_H
#include "Entity.h"
#include "EntityManager.h"
struct WindowConfig {
	int width, height, frame;
	bool isFullScreen;
};
struct FontConfig {
	std::string name;
	int size, r, g, b;
};
struct PlayerConfig {
	int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
	float S;
};
struct EnemyConfig {
	int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
	float SMIN,SMAX;
};
struct BulletConfig {
	int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
	float S;
};
class Game
{
public:
	Game(const std::string&);
	void run();
private:
	sf::RenderWindow m_window; // the window we will draw to
		EntityManager m_entites;
		sf::Font m_font;
		sf::Text m_text;
		sf::Text m_specialProgress;
		PlayerConfig m_playerConfig;
		EnemyConfig m_enemyConfig;
		BulletConfig m_bulletConfig;
		int m_score = 0;
		int m_currentFrame = 0;
		int m_lastEnemySpawnTime = 0;
		bool m_paused = false;
		bool m_running = true;;
		std::shared_ptr<Entity>m_player;

		void init(const std::string& config);// initialize the GameState with a config
		void setPaused(bool paused); // pause the game

		void sMovement(); //System: Entity position / movement update
		void sUserInput(); //System: User Input
		void sLifespan(); //System:Lifespan
		void sRender(); //System: Render / Drawing
		void sEnemySpawner(); //System : spawns Enemies
		void sCollision(); //System: Collisions

		void spawnPlayer();
		void spawnEnemy();
		void spawnSmallEnemies(std::shared_ptr<Entity>);
		void spawnBullet(std::shared_ptr<Entity>, const Vec2&);
		void spawnSpecialWeapon(std::shared_ptr<Entity>, const Vec2&);
		// My helper functions:
		bool isSpecialReady = true;
		int m_lastSpecial = 0;
		void speicalWeaponHelper();
		float getRad(float);
		void playerMovement();
		void enemyMovement();
		void bulletMovement();
		void spawnScore();
		void smallEnemyMovement();
};
#endif