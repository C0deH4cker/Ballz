//
//  Ball.cpp
//  Ballz
//
//  Created by C0deH4cker on 12/7/13.
//  Copyright (c) 2013 C0deH4cker. All rights reserved.
//

#include "Ball.h"
#include <OpenGL/gl.h>
#include <math.h>
#include <helpers.h>
#include <Vector2.h>
#include <Color.h>
#include <Game.h>
#include <Window.h>


using namespace sge;

const float Ball::restitution = 1.0f;

Ball::Ball(Vector2 start, float radius, Color* color, float gravity)
: center(start), radius(radius), color(color), texture(NULL), gravity(gravity), hasTexture(false) {
	redraw();
	vertices.reserve(102);
	glGenBuffers(1, &vertexBuffer);
}

Ball::Ball(Vector2 start, float radius, Texture2D* image, float gravity)
: center(start), radius(radius), color(NULL), texture(image), gravity(gravity), hasTexture(true) {
	
}

Ball::~Ball() {
	if(!hasTexture) {
		delete color;
		glDeleteBuffers(1, &vertexBuffer);
	}
}

void Ball::update(float deltaTime) {
	velocity.y += 0.5f * gravity;
	center += velocity * deltaTime;
	
	Window* window = Game::instance()->window;
	
	unsigned w = window->getWidth();
	unsigned h = window->getHeight();
	
	if(center.x + radius >= w) {
		center.x = w - radius;
		velocity.x *= -restitution;
	}
	else if(center.x - radius <= 0) {
		center.x = radius;
		velocity.x *= -restitution;
	}
	
	if(center.y + radius >= h) {
		center.y = h - radius;
		velocity.y *= -restitution;
	}
	else if(center.y - radius <= 0) {
		center.y = radius;
		velocity.y *= -restitution;
	}
	
	velocity.y += 0.5f * gravity;
}

void Ball::draw() {
	if(hasTexture) {
		texture->draw({center.x - radius, center.y - radius, radius * 2, radius * 2});
	}
	else {
		// Position renderer at ball's center
		glLoadIdentity();
		glTranslatef(center.x, center.y, 0.0f);
		
		color->apply();
		
		// Bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);
		glVertexPointer(2, GL_FLOAT, sizeof(Vector2), 0);
		
		// Draw the vertices
		glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)vertices.size());
	}
}

void Ball::redraw() {
	if(hasTexture)
		return;
	
	vertices.clear();
	
	// Center of the circle
	vertices.push_back({0.0f, 0.0f});
	
	// Generate a circle
	GLfloat angle;
	int numSegments = 100;
	for(int i = 0; i <= numSegments; i++) {
		angle = i * 2.0f * M_PI / numSegments;
		vertices.push_back({cosf(angle) * radius, sinf(angle) * radius});
	}
	
	// Store the data in the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);
	glVertexPointer(2, GL_FLOAT, sizeof(Vector2), 0);
}

float Ball::area() {
	return M_PI * radius*radius;
}

bool Ball::contains(const Vector2& pt) {
	return (pt - center).sqrmagnitude() <= radius*radius;
}

bool Ball::checkCollision(Ball* other) {
	float r = radius + other->radius;
	return center.sqrdistance(other->center) <= r*r;
}

Vector2 Ball::collisionPoint(Ball *other) {
	float cx = (center.x * other->radius + other->center.x * radius) /
			   (radius + other->radius);
	
	float cy = (center.y * other->radius + other->center.y * radius) /
			   (radius + other->radius);
	
	return {cx, cy};
}

void Ball::handleCollision(Ball* other) {
	float m1 = area();
	float m2 = other->area();
	
	float m21 = m2 / m1;
	Vector2 p21 = other->center - center;
	Vector2 v21 = other->velocity - velocity;
	
	Vector2 vcm = (m1 * velocity + m2 * other->velocity) / (m1 + m2);
	
	// Return if balls are not approaching
	if((v21.x * p21.x + v21.y * p21.y) >= 0)
		return;
	
	// Update velocities
	float a = p21.y / p21.x;
	float dvx2 = -2 * (v21.x + a * v21.y) / ((1 + a*a) * (1 + m21));
	other->velocity.x += dvx2;
	other->velocity.y += a * dvx2;
	velocity.x -= m21 * dvx2;
	velocity.y -= a * m21 * dvx2;
	
	// Velocity correction for inelastic collisions
	velocity = (velocity - vcm) * restitution + vcm;
	other->velocity = (other->velocity - vcm) * restitution + vcm;
}

