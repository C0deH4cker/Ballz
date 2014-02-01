//
//  MyGame.cpp
//  Ballz
//
//  Created by C0deH4cker on 12/8/13.
//  Copyright (c) 2013 C0deH4cker. All rights reserved.
//

#include "MyGame.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include "Game.h"
#include "Ball.h"
#include "Color.h"


using namespace sge;

MyGame::MyGame()
: cursorRect(-500.0f, -500.0f, 0.0f, 0.0f), rng((unsigned)time(NULL)) {
	window->setTitle("Ballz");
	window->setBackground(Color::CornflowerBlue);
	window->hideCursor(true);
}

MyGame::~MyGame() {
	delete cursorImg;
	delete ballImg;
	
	for(auto it = balls.begin(); it != balls.end(); it++)
		delete *it;
	
	balls.clear();
}

void MyGame::initialize() {
	cursorImg = content.load<Texture2D>("cursor.png");
	ballImg = content.load<Texture2D>("ball.png");
}

void MyGame::update(double deltaTime) {
	Vector2 dv;
	float amount = 2.0f;
	
	if(glfwGetKey(glwindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
		dv.x -= amount;
	}
	if(glfwGetKey(glwindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		dv.x += amount;
	}
	if(glfwGetKey(glwindow, GLFW_KEY_UP) == GLFW_PRESS) {
		dv.y -= amount;
	}
	if(glfwGetKey(glwindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
		dv.y += amount;
	}
	
	for(auto it = balls.begin(); it != balls.end(); it++) {
		(*it)->velocity += dv / 2.0f;
		(*it)->update(deltaTime);
		(*it)->velocity += dv / 2.0f;
	}
	
	for(auto it = balls.begin(); it != balls.end(); it++) {
		for(auto inner = it + 1; inner != balls.end(); inner++) {
			if((*it)->checkCollision(*inner)) {
				(*it)->handleCollision(*inner);
			}
		}
	}
}

void MyGame::draw(double deltaTime) {
//	printf("FPS: %.2f\n", 1.0 / deltaTime);
	for(auto it = balls.begin(); it != balls.end(); it++)
		(*it)->draw();
	
	cursorImg->draw(cursorRect);
}

#pragma mark -
#pragma mark Delegate methods
#pragma mark -

void MyGame::mouseDown(int button, int mods) {
	if(button == GLFW_MOUSE_BUTTON_LEFT) {
		std::uniform_real_distribution<float> sizeDist(10.0f, 80.0f);
		Ball* ball = new Ball(mouse, sizeDist(rng), ballImg, 0.0f);
		
		std::uniform_real_distribution<float> velDist(-100.0f, 100.0f);
		ball->velocity = {velDist(rng), velDist(rng)};
		balls.push_back(ball);
	}
	else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
		auto it = balls.begin();
		while(it != balls.end()) {
			if((*it)->contains(mouse))
				it = balls.erase(it);
			else
				++it;
		}
	}
}

void MyGame::mouseMoved(const Vector2& pos) {
	cursorRect = {pos.x - 4, pos.y - 5, cursorImg->width / 10.0f, cursorImg->height / 10.0f};
}

void MyGame::scrolled(const Vector2 &offset) {
	for(auto it = balls.begin(); it != balls.end(); it++)
		(*it)->velocity += offset;
}

void MyGame::keyDown(int key, int scancode, int mods) {
	if(key == GLFW_KEY_SPACE) {
		for(auto it = balls.begin(); it != balls.end(); ++it) {
			(*it)->velocity = {0.0f, 0.0f};
		}
	}
	
	/* Super */
	this->Game::keyDown(key, scancode, mods);
}

