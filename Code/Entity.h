#ifndef ENTITY_H
#define ENTITY_H
#include "Component.h"
#include <string>
#include <memory>
#include <vector>
class Entity
{
public:
	std::shared_ptr<CTransform>transform;
	std::shared_ptr<CCollision>collision;
	std::shared_ptr<CScore>score;
	std::shared_ptr<CShape>shape;
	std::shared_ptr<CLifeSpan>lifespan;
	std::shared_ptr<CInput>input;
	std::shared_ptr<CHitPoint>hitpoint;
	void destroy();
	bool isActive() const;
	const std::string& tag() const;
	size_t id() const;
private:
	friend class EntityManager;
	size_t m_id;
	std::string m_tag;
	bool m_alive;
	Entity();
	Entity(size_t, const std::string&);
};
#endif