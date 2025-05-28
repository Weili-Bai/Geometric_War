#include "EntityManager.h"
EntityManager::EntityManager() {
	m_totalEntities = 0;
}
std::shared_ptr<Entity>EntityManager::addEntity(const std::string& tag) {
	auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities, tag));
	m_toAdd.push_back(e);
	m_totalEntities++;
	return e;
}
void EntityManager::removeDead(EntityVector& vec) {
	auto f = [](std::shared_ptr<Entity> e) {return !e->isActive(); };
	auto newEnd = remove_if(vec.begin(), vec.end(), f);
	vec.erase(newEnd, vec.end());
}
const EntityVector& EntityManager::getEntities() {
	return m_vector;
}
const EntityVector& EntityManager::getEntities(const std::string&tag) {
	return m_map[tag];
}
void EntityManager::update() {
	for (auto e : m_toAdd) {
		m_vector.push_back(e);
		m_map[e->tag()].push_back(e);
	}
	m_toAdd.clear();
	removeDead(m_vector);
	for (auto i = m_map.begin(); i != m_map.end(); i++) {
		removeDead(i->second);
	}
}