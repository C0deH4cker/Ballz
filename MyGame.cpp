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
#include "Game.h"
#include "Ball.h"
#include "Color.h"


using namespace sge;

MyGame::MyGame()
: cursorRect(-500, -500, 0, 0) {
	srand((unsigned)time(NULL));
	window->setTitle("Ballz");
}

MyGame::~MyGame() {
	delete cursorImg;
	delete ballImg;
	
	std::vector<Ball*>::iterator itr;
	for(itr = balls.begin(); itr != balls.end(); itr++) {
		delete *itr;
	}
}

void MyGame::initialize() {
	window->setBackground(Color::CornflowerBlue);
	window->hideCursor(true);
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
	
	std::vector<Ball*>::iterator itr;
	
	for(itr = balls.begin(); itr != balls.end(); itr++) {
		(*itr)->velocity += dv/2;
		(*itr)->update(deltaTime);
		(*itr)->velocity += dv/2;
	}
	
	for(itr = balls.begin(); itr != balls.end(); itr++) {
		std::vector<Ball*>::iterator inner;
		for(inner = itr + 1; inner != balls.end(); inner++) {
			if((*itr)->checkCollision(*inner)) {
				(*itr)->handleCollision(*inner);
			}
		}
	}
}

void MyGame::draw(double deltaTime) {
//	printf("FPS: %.2f\n", 1.0 / deltaTime);
	std::vector<Ball*>::iterator itr;
	for(itr = balls.begin(); itr != balls.end(); itr++) {
		(*itr)->draw();
	}
	
	cursorImg->draw(cursorRect);
}

#pragma mark -
#pragma mark Delegate methods
#pragma mark -

void MyGame::mouseDown(int button, int mods) {
	if(button == GLFW_MOUSE_BUTTON_LEFT) {
		Ball* ball = new Ball(mouse, (float)(rand() % 50 + 30), ballImg, 0.0f);
		ball->velocity = {(rand() % 200) / 2.0f - 100.0f, (rand() % 200) / 2.0f - 100.0f};
		balls.push_back(ball);
	}
	else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
		std::vector<Ball*>::iterator itr;
		for(itr = balls.begin(); itr != balls.end();) {
			if((*itr)->contains(mouse))
				itr = balls.erase(itr);
			else
				itr++;
		}
	}
}

void MyGame::mouseMoved(const Vector2& pos) {
	cursorRect = {pos.x - 4, pos.y - 5, cursorImg->width / 10.0f, cursorImg->height / 10.0f};
}

void MyGame::scrolled(const Vector2 &offset) {
	std::vector<Ball*>::iterator itr;
	for(itr = balls.begin(); itr != balls.end(); itr++) {
		(*itr)->velocity += offset;
	}
}

void MyGame::keyDown(int key, int scancode, int mods) {
	/* Super */
	this->Game::keyDown(key, scancode, mods);
}

