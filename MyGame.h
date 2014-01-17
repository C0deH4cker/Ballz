//
//  MyGame.h
//  Ballz
//
//  Created by C0deH4cker on 12/8/13.
//  Copyright (c) 2013 C0deH4cker. All rights reserved.
//

#pragma once

#include <vector>
#include "Game.h"
#include "Ball.h"
#include "Vector2.h"
#include "Content.h"
#include "Texture2D.h"
#include "Rectangle.h"


using namespace sge;

class MyGame : public Game {
public:
	MyGame();
	~MyGame();
	
	void initialize();
	void update(double deltaTime);
	void draw(double deltaTime);
	
	/* Delegate methods */
	void mouseDown(int button, int mods);
	void mouseMoved(const Vector2& pos);
	void scrolled(const Vector2& offset);
	void keyDown(int key, int scancode, int mods);
	
private:
	Content content;
	Texture2D* cursorImg;
	Texture2D* ballImg;
	Rectangle cursorRect;
	std::vector<Ball*> balls;
};

