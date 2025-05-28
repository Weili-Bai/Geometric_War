#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include "Entity.h"
#include <vector>
#include <unordered_map>
typedef std::vector<std::shared_ptr<Entity>> EntityVector;
typedef std::unordered_map<std::string, EntityVector> EntityMap;
class EntityManager
{
public:
	EntityManager();
	std::shared_ptr<Entity>addEntity(const std::string&);
	const EntityVector& getEntities();
	const EntityVector& getEntities(const std::string&);
	void update();
private:
	EntityVector m_vector;
	EntityVector m_toAdd;
	EntityMap m_map;
	size_t m_totalEntities;
	void removeDead(EntityVector&);
};
#endif