#ifndef COMPONENT_H
#define COMPONENT_H
#include "Vec2.h"
#include "SFML/Graphics.hpp"
class CTransform {
public:
	Vec2 position;
	Vec2 velocity;
	float angle;
	CTransform() {
		position = { 0,0 };
		velocity = { 0,0 };
		angle = 0;
	}
	CTransform(const Vec2& p, const Vec2& v, const double a) {
		position = p;
		velocity = v;
		angle = a;
	}
};
class CScore {
public:
	unsigned int score;
	CScore() {
		score = 0;
	}
	CScore(int s) {
		score = s;
	}
};
class CCollision {
public:
	float radius;
	CCollision() {
		radius = 0;
	}
	CCollision(float r) {
		radius = r;
	}
};
class CShape {
public:
	sf::CircleShape shape;

	CShape(float radius, int side, const sf::Color fill, const sf::Color outline,
		float thickness) {
		shape = sf::CircleShape(radius, side);
		shape.setFillColor(fill);
		shape.setOrigin(radius, radius);
		shape.setOutlineColor(outline);
		shape.setOutlineThickness(thickness);
	}
};
class CLifeSpan {
public:
	int remaining;
	int total;
	CLifeSpan() {
		remaining = 0;
		total = 0;
	}
	CLifeSpan(int t) {
		remaining = t;
		total = t;
	}
};
class CInput {
public:
	bool up;
	bool down;
	bool left;
	bool right;
	bool shoot;
	CInput() {
		up = false;
		down = false;
		left = false;
		right = false;
		shoot = false;
	}
};
class CHitPoint {
public:
	char hp;
	CHitPoint() {
		hp = 1;
	}
	CHitPoint(char a) {
		hp = a;
	}
};
#endif