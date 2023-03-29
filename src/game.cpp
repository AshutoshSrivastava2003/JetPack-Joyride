#include "../src/game.h"
#include "../src/resource_manager.h"
#include "../src/sprite_renderer.h"
#include "../src/game_object.h"
#include "../src/text_renderer.h"
#include <bits/stdc++.h>
#include <vector>

#define SCR_Width 800.0f
#define SCR_Height 600.0f

// Game-related State data
SpriteRenderer *Renderer;
GameObject *Player;
TextRenderer  *Text;

int flag = 0;
int flag2=0;
int coin_count = 0;
double dist_travelled=0.0f;
const double level1_dist=2700.0f;
const double level2_dist=2650.0f;
const double level3_dist=2700.0f;
bool level_change=false;


glm::vec2 pos1=glm::vec2(0.0f, 0.0f);
glm::vec2 pos2=glm::vec2(SCR_Width, 0.0f);

std::vector<GameObject> Coins;
std::vector<GameObject> Zappers;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    
}

Game::~Game()
{
    // delete Renderer;
    // delete Player;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("../src/sprite.vs", "../src/sprite.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
    // load textures
    ResourceManager::LoadTexture("../textures/background.jpg", false, "background1");
    ResourceManager::LoadTexture("../textures/background2.png", true, "background2");
    ResourceManager::LoadTexture("../textures/background3.png", true, "background3");
    ResourceManager::LoadTexture("../textures/background4.jpg", false, "win_background");
    ResourceManager::LoadTexture("../textures/background5.png", true, "background5");
    ResourceManager::LoadTexture("../textures/Barry-nobg.png", true, "paddle");
    ResourceManager::LoadTexture("../textures/gold_coin.png", true, "golden");
    ResourceManager::LoadTexture("../textures/zapper-nobg.png", true, "zapper");
    
    // load levels
    // GameLevel one;
    // one.Load("../levels/one.lvl", this->Width, this->Height / 2);
    // GameLevel two; two.Load("../levels/two.lvl", this->Width, this->Height / 2);
    // GameLevel three; three.Load("../levels/three.lvl", this->Width, this->Height / 2);
    // GameLevel four; four.Load("../levels/four.lvl", this->Width, this->Height / 2);
    // this->Levels.push_back(one);
    // this->Levels.push_back(two);
    // this->Levels.push_back(three);
    // this->Levels.push_back(four);
    this->Level = 1;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f - 300.0f, this->Height / 2.0f - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../src/fonts/OCRAEXT.TTF", 24);
}

void Game::Update(float dt)
{
    // update objects
    // Player->Move(dt, this->Width);
    // check for collisions
    this->DoCollisions();

}
bool CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                      two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                      two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}
void Game::DoCollisions()
{
    for (GameObject &coin : Coins)
    {
        if (!coin.Destroyed)
        {
            if (CheckCollision(*Player, coin))
            {
                if (!coin.IsSolid)
                {
                    coin.Destroyed = true;
                    coin_count++;
                }

                // this->State=GAME_LOST;
            }
        }
    }
    for (GameObject &zapper : Zappers)
    {
        if (CheckCollision(*Player, zapper))
        {
            // std::cout<<"Collided";
            this->State = GAME_LOST;
        }
    }
}
void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE )
    {
        float velocity = PLAYER_VELOCITY * dt;
        if (Player->Position.y <= this->Height - Player->Size.y)
        {
            Player->Position.y += velocity / 5;
        }
        // move playerboard
        if (this->Keys[GLFW_KEY_SPACE])
        {
            if (Player->Position.y >= 0.0f)
                Player->Position.y -= velocity;
        }
        // if (this->Keys[GLFW_KEY_S])
        // {
        //     if (Player->Position.y >= 0.0f)
        //         Player->Position.y -= 1.5*velocity;
        // }
        // if (this->Keys[GLFW_KEY_S])
        // {
        //     if (Player->Position.y <= this->Height - Player->Size.y)
        //         Player->Position.y += velocity;
        // }
    }
}
void Game::Draw2(SpriteRenderer &renderer)
{
    for (GameObject &coins : this->Coins)
        if (!coins.Destroyed)
            coins.Draw(renderer);
    for (GameObject &zappers : this->Zappers)
        if (!zappers.Destroyed)
            zappers.Draw(renderer);
}
void Game::CoinSetup1()
{
    int num_coins = 50;

    double coinscrd[num_coins][2];
    double x = this->Width / 2.0f - PLAYER_SIZE.x / 2.0f - 350.0f;
    for (int i = 0; i < num_coins; i += 5)
    {
        // GameObject * coin;
        // coin.Destroyed=false;
        // glm::vec2 pos=glm::vec2(rand()%(this->Width),rand()%(this->Height));
        // coin = GameObject::GameObject(pos,(20.0f, 20.0f),ResourceManager::GetTexture("paddle"));
        // Coins.push_back(coin);
        int y = rand() % (this->Height);
        coinscrd[i][1] = y;
        coinscrd[i + 1][1] = y;
        coinscrd[i + 2][1] = y;
        coinscrd[i + 3][1] = y;
        coinscrd[i + 4][1] = y;

        coinscrd[i][0] = x;
        coinscrd[i + 1][0] = x + 25;
        coinscrd[i + 2][0] = x + 50;
        coinscrd[i + 3][0] = x + 75;
        coinscrd[i + 4][0] = x + 100;

        x += 250;
    }
    for (int i = 0; i < num_coins; i++)
    {
        glm::vec2 pos = glm::vec2(coinscrd[i][0], coinscrd[i][1]);
        GameObject coin = GameObject(pos, COIN_SIZE, ResourceManager::GetTexture("golden"));
        Coins.push_back(coin);
    }
}
void Game::ZappersSetup1()
{
    int num_zappers = 10;
    double zappercrd[num_zappers][2];
    double x = this->Width / 2.0f - PLAYER_SIZE.x / 2.0f;
    double s=1;
    for (int i = 0; i < num_zappers; i++)
    {
        s*=-1;
        zappercrd[i][0] = x;
        if(s==1){zappercrd[i][1]=0.0f;}
        else{zappercrd[i][1] = this->Height-ZAPPER_SIZE.y ;}
        x += 250.0f;
        if(i==4)x+=100.0f;
    }
    for (int i = 0; i < num_zappers; i++)
    {
        glm::vec2 pos = glm::vec2(zappercrd[i][0], zappercrd[i][1]);
        GameObject zapper = GameObject(pos, ZAPPER_SIZE, ResourceManager::GetTexture("zapper"));
        Zappers.push_back(zapper);
    }
}
void Game::CoinSetup2()
{
    int num_coins = 50;

    double coinscrd[num_coins][2];
    double x = this->Width / 2.0f - PLAYER_SIZE.x / 2.0f - 350.0f ;
    for (int i = 0; i < num_coins; i += 5)
    {
        // GameObject * coin;
        // coin.Destroyed=false;
        // glm::vec2 pos=glm::vec2(rand()%(this->Width),rand()%(this->Height));
        // coin = GameObject::GameObject(pos,(20.0f, 20.0f),ResourceManager::GetTexture("paddle"));
        // Coins.push_back(coin);
        int y = rand() % (this->Height);
        coinscrd[i][1] = y;
        coinscrd[i + 1][1] = y;
        coinscrd[i + 2][1] = y;
        coinscrd[i + 3][1] = y;
        coinscrd[i + 4][1] = y;

        coinscrd[i][0] = x;
        coinscrd[i + 1][0] = x + 25;
        coinscrd[i + 2][0] = x + 50;
        coinscrd[i + 3][0] = x + 75;
        coinscrd[i + 4][0] = x + 100;

        x += 250;
    }
    for (int i = 0; i < num_coins; i++)
    {
        glm::vec2 pos = glm::vec2(coinscrd[i][0], coinscrd[i][1]);
        GameObject coin = GameObject(pos, COIN_SIZE, ResourceManager::GetTexture("golden"));
        Coins.push_back(coin);
    }
}
void Game::ZappersSetup2()
{
    int num_zappers = 10;
    double zappercrd[num_zappers][2];
    double x = this->Width / 2.0f - PLAYER_SIZE.x / 2.0f;
    double s=1;
    for (int i = 0; i < num_zappers; i++)
    {
        s*=-1;
        zappercrd[i][0] = x;
        if(s==1){zappercrd[i][1]=0.0f;}
        else{zappercrd[i][1] = this->Height-ZAPPER_SIZE.y ;}
        x += 250.0f;
        if(i==4)x+=100.0f;
    }
    for (int i = 0; i < num_zappers; i++)
    {
        glm::vec2 pos = glm::vec2(zappercrd[i][0], zappercrd[i][1]);
        GameObject zapper = GameObject(pos, ZAPPER_SIZE, ResourceManager::GetTexture("zapper"));
        Zappers[i]=zapper;
    }
}
void Game::CoinSetup3()
{
    int num_coins = 50;

    double coinscrd[num_coins][2];
    double x = this->Width / 2.0f - PLAYER_SIZE.x / 2.0f - 350.0f;
    for (int i = 0; i < num_coins; i += 5)
    {
        // GameObject * coin;
        // coin.Destroyed=false;
        // glm::vec2 pos=glm::vec2(rand()%(this->Width),rand()%(this->Height));
        // coin = GameObject::GameObject(pos,(20.0f, 20.0f),ResourceManager::GetTexture("paddle"));
        // Coins.push_back(coin);
        int y = rand() % (this->Height);
        coinscrd[i][1] = y;
        coinscrd[i + 1][1] = y;
        coinscrd[i + 2][1] = y;
        coinscrd[i + 3][1] = y;
        coinscrd[i + 4][1] = y;

        coinscrd[i][0] = x;
        coinscrd[i + 1][0] = x + 25;
        coinscrd[i + 2][0] = x + 50;
        coinscrd[i + 3][0] = x + 75;
        coinscrd[i + 4][0] = x + 100;

        x += 250;
    }
    for (int i = 0; i < num_coins; i++)
    {
        glm::vec2 pos = glm::vec2(coinscrd[i][0], coinscrd[i][1]);
        GameObject coin = GameObject(pos, COIN_SIZE, ResourceManager::GetTexture("golden"));
        Coins.push_back(coin);
    }
}
void Game::ZappersSetup3()
{
    int num_zappers = 10;
    double zappercrd[num_zappers][2];
    double x = this->Width / 2.0f - PLAYER_SIZE.x / 2.0f;
    double s=1;
    for (int i = 0; i < num_zappers; i++)
    {
        s*=-1;
        zappercrd[i][0] = x;
        if(s==1){zappercrd[i][1]=0.0f;}
        else{zappercrd[i][1] = this->Height-ZAPPER_SIZE.y ;}
        x += 250.0f;
        if(i==4 || i==6)x+=100.0f;
    }
    for (int i = 0; i < num_zappers; i++)
    {
        glm::vec2 pos = glm::vec2(zappercrd[i][0], zappercrd[i][1]);
        GameObject zapper = GameObject(pos, ZAPPER_SIZE, ResourceManager::GetTexture("zapper"));
        Zappers[i]=zapper;
    }
}
void Game::Render()
{
    // TextRenderer();
    
    if (flag == 0)
    {
        // CoinSetup1();
        // ZappersSetup1();
        level_change=true;
        flag = 1;
        // this->Level=1;
    }

    

    if (this->State == GAME_ACTIVE && this->Level==1)
    {
        if(level_change){
            CoinSetup1();
            ZappersSetup1();
            level_change=false;
        }
        
        Texture2D myTexture1 = ResourceManager::GetTexture("background1");
        Renderer->DrawSprite(myTexture1, pos1, glm::vec2(this->Width, this->Height), 0.0f);
        Texture2D myTexture2 = ResourceManager::GetTexture("background2");
        Renderer->DrawSprite(myTexture2, pos2, glm::vec2(this->Width, this->Height), 0.0f);

        pos1.x-=0.50f;
        pos2.x-=0.50f;
        // if(pos2.x=this->width && pos2.x>0){
        //     pos1.x=this->Width;
        // }
        
        // if(!flag2 && pos2.x<=0.0f){
        //     pos1.x+=this->Width;
        //     flag2=1;
        // }
        std::stringstream ss; ss << coin_count;
        std::stringstream ss2; ss2 << round(dist_travelled);
        // std::string coins="Coins";
        Text->RenderText("Coins "+ ss.str(),5.0f, 5.0f,1.0f,glm::vec3(0.0f,1.0f,0.0f));
        Text->RenderText("Distance "+ ss2.str(),5.0f, 25.0f,1.0f,glm::vec3(0.0f,1.0f,0.0f));
        Text->RenderText("Level 1",this->Width -150.0f, 5.0f,1.2f,glm::vec3(0.0f,0.0f,0.0f));
        // Text->RenderText("ashutosh",5.0f, 5.0f,1.0f,glm::vec3(1.0f,1.0f,0.0f));


        // draw background
         // draw level
        // this->Levels[this->Level].Draw(*Renderer);
        // Draw2(*Renderer);
        // draw player
        Player->Draw(*Renderer);
        for (int i = 0; i < Coins.size(); i++)
        {
            Coins[i].Position.x -= 2.0f;
            if (!Coins[i].Destroyed){
                Coins[i].Draw(*Renderer);
            }
        }
        Zappers[4].Position.y-=0.2f;
        Zappers[4].Size.y+=0.2f;
        for (int i = 0; i < Zappers.size(); i++)
        {
            Zappers[i].Position.x -= 2.0f;
            Zappers[i].Draw(*Renderer);
        }
        DoCollisions();
        dist_travelled+=2.0f;
        if(dist_travelled>level1_dist){
            // std::cout<<"Level 1 passed\n";
            this->Level++;
            level_change=1;
            dist_travelled=0;
        }
        // if (!level_change && Zappers[Zappers.size()-1].Position.x<=this->Width / 2.0f - PLAYER_SIZE.x / 2.0f - 325.0f){
        //     std::cout<<"Level passed";
        //     this->Level++;
        //     level_change=1;
        // }
        // std::cout<<coin_count;
        // glm::vec2 pos=glm::vec2();
    }
    if (this->State == GAME_ACTIVE && this->Level==2)
    {
        if(level_change){
            CoinSetup2();
            ZappersSetup2();
            level_change=false;
            pos1=glm::vec2(0.0f,0.0f);
            pos2=glm::vec2(this->Width,0.0f);
        }
        // std::stringstream ss; ss << coin_count;
        // std::string coins="Coins";
        // Text->RenderText(coins,5.0f, 5.0f,1.0f,glm::vec3(0.0f,1.0f,0.0f));
        // Text->RenderText("ashutosh",5.0f, 5.0f,10.0f,glm::vec3(1.0f,1.0f,0.0f));


        // draw background
        
        Texture2D myTexture = ResourceManager::GetTexture("background2");
        Renderer->DrawSprite(myTexture, pos1, glm::vec2(this->Width, this->Height), 0.0f);
        Texture2D myTexture1 = ResourceManager::GetTexture("background1");
        Renderer->DrawSprite(myTexture1, pos2, glm::vec2(this->Width, this->Height), 0.0f);
        pos1.x-=0.50f;
        pos2.x-=0.50f;
        // draw level
        // this->Levels[this->Level].Draw(*Renderer);
        // Draw2(*Renderer);
        // draw player
        std::stringstream ss; ss << coin_count;
        std::stringstream ss2; ss2 << round(level1_dist+dist_travelled);
    
        // std::string coins="Coins";
        Text->RenderText("Coins "+ ss.str(),5.0f, 5.0f,1.0f,glm::vec3(0.0f,1.0f,0.0f));
        Text->RenderText("Distance "+ ss2.str(),5.0f, 25.0f,1.0f,glm::vec3(0.0f,1.0f,0.0f));
        Text->RenderText("Level 2",this->Width -150.0f, 5.0f,1.2f,glm::vec3(0.0f,0.0f,0.0f));
        Player->Draw(*Renderer);
        for (int i = 0; i < Coins.size(); i++)
        {
            Coins[i].Position.x -= 2.5f;
            if (!Coins[i].Destroyed){
                Coins[i].Draw(*Renderer);
            }
        }
        Zappers[4].Position.y-=0.2f;
        Zappers[4].Size.y+=0.2f;
        Zappers[5].Position.y+=0.3f;
        Zappers[5].Size.y+=0.2f;
        for (int i = 0; i < Zappers.size(); i++)
        {
            Zappers[i].Position.x -= 2.5f;
            Zappers[i].Draw(*Renderer);
        }
        DoCollisions();
        dist_travelled+=2.5f;
        if(dist_travelled>level2_dist){
            // std::cout<<"Level 2 passed\n";
            this->Level++;
            level_change=1;
            dist_travelled=0;
        }
        // if (!level_change && Zappers[Zappers.size()-1].Position.x<=this->Width / 2.0f - PLAYER_SIZE.x / 2.0f - 325.0f){
        //     std::cout<<"Level passed";
        //     this->Level++;
        //     level_change=1;
        // }
        // std::cout<<coin_count;
        // glm::vec2 pos=glm::vec2();
    }
    if (this->State == GAME_ACTIVE && this->Level==3)
    {
        if(level_change){
            CoinSetup3();
            ZappersSetup3();
            level_change=false;
            pos1=glm::vec2(0.0f,0.0f);
            pos2=glm::vec2(this->Width,0.0f);
        }
        // std::stringstream ss; ss << coin_count;
        // std::string coins="Coins";
        // Text->RenderText(coins,5.0f, 5.0f,1.0f,glm::vec3(0.0f,1.0f,0.0f));
        // Text->RenderText("ashutosh",5.0f, 5.0f,10.0f,glm::vec3(1.0f,1.0f,0.0f));


        // draw background
        Texture2D myTexture = ResourceManager::GetTexture("background3");
        Renderer->DrawSprite(myTexture, pos1, glm::vec2(this->Width, this->Height), 0.0f);
        Texture2D myTexture2 = ResourceManager::GetTexture("background5");
        Renderer->DrawSprite(myTexture2, pos2, glm::vec2(this->Width, this->Height), 0.0f);
        pos1.x-=0.50f;
        pos2.x-=0.50f;
        
        // draw level
        // this->Levels[this->Level].Draw(*Renderer);
        // Draw2(*Renderer);
        // draw player
        std::stringstream ss; ss << coin_count;
        std::stringstream ss2; ss2 << round(level1_dist+level2_dist+dist_travelled);
        // std::string coins="Coins";
        Text->RenderText("Coins "+ ss.str(),5.0f, 5.0f,1.0f,glm::vec3(0.0f,1.0f,0.0f));
        Text->RenderText("Distance "+ ss2.str(),5.0f, 25.0f,1.0f,glm::vec3(0.0f,1.0f,0.0f));
        Text->RenderText("Level 3",this->Width -150.0f, 5.0f,1.2f,glm::vec3(0.0f,0.0f,0.0f));
        Player->Draw(*Renderer);
        for (int i = 0; i < Coins.size(); i++)
        {
            Coins[i].Position.x -= 3.0f;
            if (!Coins[i].Destroyed){
                Coins[i].Draw(*Renderer);
            }
        }
        Zappers[4].Position.y-=0.2f;
        Zappers[4].Size.y+=0.2f;
        Zappers[5].Position.y+=0.3f;
        Zappers[5].Size.y+=0.2f;
        Zappers[7].Position.x-=0.2f;
        // Zappers[7].Size.x+=0.1f;
        for (int i = 0; i < Zappers.size(); i++)
        {
            Zappers[i].Position.x -= 3.0f;
            Zappers[i].Draw(*Renderer);
        }
        DoCollisions();
        dist_travelled+=3.0f;
        if(dist_travelled>level3_dist){
            // std::cout<<"Level 3 passed\n";
            this->State=GAME_WIN;
            // this->Level++;
            level_change=1;
            // dist_travelled=0;
        }
        // if (!level_change && Zappers[Zappers.size()-1].Position.x<=this->Width / 2.0f - PLAYER_SIZE.x / 2.0f - 325.0f){
        //     std::cout<<"Level passed";
        //     this->Level++;
        //     level_change=1;
        // }
        // std::cout<<coin_count;
        // glm::vec2 pos=glm::vec2();
        // std::cout<<"You won";
    }
    if (this->State==GAME_WIN){
        Player->Draw(*Renderer);
        std::stringstream ss; ss << coin_count;
        std::stringstream ss2; ss2 << round(level1_dist+level2_dist+dist_travelled);
        Text->RenderText("YOU WIN!!!",this->Width/2.0f -175.0f, this->Height/2.0f-50.0f,2.0f,glm::vec3(0.0f,1.0f,0.0f));
        Text->RenderText("Coins "+ ss.str(),this->Width/2.0f-100.0f, this->Height/2.0f-50.0f+ 75.0f,1.0f,glm::vec3(1.0f,1.0f,0.0f));
        Text->RenderText("Distance "+ ss2.str(),this->Width/2.0f-130.0f, this->Height/2.0f-50.0f+100.0f,1.0f,glm::vec3(0.0f,1.0f,0.0f));
        // std::cout << "YOU WIN!!!" << std::endl;
    }

    else if (this->State == GAME_LOST)
    {
        Player->Draw(*Renderer);
        Text->RenderText("GAME OVER!!!",this->Width/2.0f -175.0f, this->Height/2.0f-75.0f,2.0f,glm::vec3(1.0f,0.0f,0.0f));
        Text->RenderText("YOU LOSE :(",this->Width/2.0f -175.0f, this->Height/2.0f+25.0f,2.0f,glm::vec3(1.0f,0.0f,0.0f));
        // std::cout << "GAME OVER" << std::endl;    
    }
}