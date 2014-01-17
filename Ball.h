//
//  Ball.h
//  Ballz
//
//  Created by C0deH4cker on 12/7/13.
//  Copyright (c) 2013 C0deH4cker. All rights reserved.
//

#pragma once

#include <vector>
#include <OpenGL/gl.h>
#include "Vector2.h"
#include "Color.h"
#include "Texture2D.h"


using namespace sge;

class Ball {
public:
	Vector2 center;
	Vector2 velocity;
	float radius;
	Color* color;
	Texture2D* texture;
	bool hasTexture;
	float gravity;
	
	Ball(Vector2 start, float radius, Color* color, float gravity=0.0f);
	Ball(Vector2 start, float radius, Texture2D* image, float gravity=0.0f);
	~Ball();
	
	void update(float deltaTime);
	void draw();
	void redraw();
	
	float area();
	bool contains(const Vector2& pt);
	bool checkCollision(Ball* other);
	Vector2 collisionPoint(Ball* other);
	void handleCollision(Ball* other);
	
private:
	GLuint vertexBuffer;
	std::vector<Vector2> vertices;
	static const float Restitution;
};

