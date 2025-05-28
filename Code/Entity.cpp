#include "Entity.h"
Entity::Entity() {
	m_id = 0;
	m_alive = true;
	m_tag = "default";
}
Entity::Entity(size_t id, const std::string& tag) {
	m_id = id;
	m_alive = true;
	m_tag = tag;
}
void Entity::destroy() {
	m_alive = false;
}
bool Entity::isActive() const{
	return m_alive;
}
const std::string& Entity:: tag() const {
	return m_tag;
}
size_t Entity::id() const {
	return m_id;
}
