/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "../src/game_level.h"
#include "../src/game_object.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_1,
    GAME_2,
    GAME_3,
    GAME_MENU,
    GAME_WIN,
    GAME_LOST
};

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(40.0f, 70.0f);
const glm::vec2 ZAPPER_SIZE(60.0f, 250.0f);
const glm::vec2 COIN_SIZE(25.0f, 25.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;	
    bool                    Keys[1024];
    unsigned int            Width, Height;
    // std::vector<GameLevel>  Levels;
    unsigned int            Level;
    
    std::vector <GameObject> Coins;
    std::vector <GameObject> Zappers;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void DoCollisions();
    void Draw2(SpriteRenderer &renderer);
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void CoinSetup1();
    void ZappersSetup1();
    void CoinSetup2();
    void ZappersSetup2();
    void CoinSetup3();
    void ZappersSetup3();
};

#endif