/*
This file contains main class of the game - game class
Game class does control every process in the game, flow of the game,  main loop of the game
*/
#include <SFML/Graphics.hpp>
#include <string>
#include <Pac-Man.h>
#include <Ghosts.h>
#include <TileSet.h>
using namespace sf;
std::vector<std::vector<char>> charMap=
{
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},//1
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},//2
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},//3
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},//4
    {'E','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','E'},//5
    {'E','W','B','B','B','B','B','B','B','B','W','B','B','B','B','B','B','B','B','W','E'},//6
    {'E','W','G','W','W','B','W','W','W','B','W','B','W','W','W','B','W','W','G','W','E'},//7
    {'E','W','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','W','E'},//8
    {'E','W','B','W','W','B','W','B','W','W','W','W','W','B','W','B','W','W','B','W','E'},//9
    {'E','W','B','B','B','B','W','B','B','B','W','B','B','B','W','B','B','B','B','W','E'},//10
    {'E','W','W','W','W','B','W','W','W','B','W','B','W','W','W','B','W','W','W','W','E'},//11
    {'E','E','E','E','W','B','W','E','E','E','E','E','E','E','W','B','W','E','E','E','E'},//12
    {'W','W','W','W','W','B','W','E','W','W','E','W','W','E','W','B','W','W','W','W','W'},//13
    {'E','E','E','E','E','B','W','E','W','E','E','E','W','E','E','B','E','E','E','E','E'},//14
    {'W','W','W','W','W','B','W','E','W','W','W','W','W','E','W','B','W','W','W','W','W'},//15
    {'E','E','E','E','W','B','W','E','E','E','E','E','E','E','W','B','W','E','E','E','E'},//16
    {'E','W','W','W','W','B','W','E','W','W','W','W','W','E','W','B','W','W','W','W','E'},//17
    {'E','W','B','B','B','B','B','B','B','B','W','B','B','B','B','B','B','B','B','W','E'},//18
    {'E','W','B','W','W','B','W','W','W','B','W','B','W','W','W','B','W','W','B','W','E'},//19
    {'E','W','G','B','W','B','B','B','B','B','B','B','B','B','B','B','W','B','G','W','E'},//20
    {'E','W','W','B','W','B','W','B','W','W','W','W','W','B','W','B','W','B','W','W','E'},//21
    {'E','W','B','B','B','B','W','B','B','B','W','B','B','B','W','B','B','B','B','W','E'},//22
    {'E','W','B','W','W','W','W','W','W','B','W','B','W','W','W','W','W','W','B','W','E'},//23
    {'E','W','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','B','W','E'},//24
    {'E','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','E'},//25
    {'E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E','E'},//26
};
class Game
{
public:
    enum State
    {
        MENU,GAME,WON,LOST
    };
    Game(int x,int y,std::string &Name);
    void run();
private:
    void update(Time deltaTime);
    void handlePlayerInput(Keyboard::Key key,bool isPressed);
    void render();
    void menuRender();
    void wonRender();
    void lostRender();
    void gameRender();
    void proccessEvents();
public:
    RenderWindow window;
private:
    State gameState=State::MENU;
    Font font;
    Text text;
    Text text2;
    Clock clock;
    PacMan pacman;
    Ghosts ghosts;
    TileSet tileset;
    Texture pacmanLifeTexture;
    Texture logoTexture;
    Sprite pacmanLife;
    Sprite logo;
};
Game::Game(int x,int y,std::string &Name) :window(VideoMode(x,y),Name),pacman(&window,Vector2f(250+12.5,325+12.5)),tileset(&window,charMap),ghosts(&window)
{
    pacmanLifeTexture.loadFromFile("Moving2.png");
    logoTexture.loadFromFile("Logo.png");
    logo.setTexture(logoTexture);
    logo.setScale(0.25,0.25);
    logo.setPosition(115,150);
    pacmanLife.setTexture(pacmanLifeTexture);
    pacmanLife.setScale(0.25,0.25);
    font.loadFromFile("sansation.ttf");
    text.setFont(font);
    text2.setFont(font);
    text.setCharacterSize(28);
    text2.setCharacterSize(28);
}

void Game::run()
{
    Clock clock;
    Time timeLastUpdate = Time::Zero;
    Time TimePerFrame = seconds(1.f/60.f);
    while(window.isOpen())
    {
        proccessEvents();
        timeLastUpdate+=clock.restart();
        while(timeLastUpdate>TimePerFrame)
        {
            if(gameState==State::GAME&&Tile::time>0)
                --Tile::time;
            timeLastUpdate-=TimePerFrame;
            update(TimePerFrame);
            render();
            if(pacman.state==PacMan::DEAD)
            {
                if(pacman.clock.getElapsedTime()<seconds(4))
                {
                    sleep(seconds(0.25));
                    pacman.setVelocity(Vector2f(0,0));
                    pacman.draw();
                    clock.restart();
                }
                else if(pacman.lives==0&&gameState!=State::WON)
                {
                    gameState=State::LOST;
                }
                else
                {
                    pacman.setState(PacMan::ALIVE);
                    pacman.setPosition(pacman.origPos);
                }
            }
        }
    }
}
void Game::update(Time deltaTime)
{
    if(Tile::ballCount==0)
    {
        gameState=State::WON;
    }
    pacman.update(deltaTime);
    if(pacman.state==PacMan::ALIVE)
    {
        ghosts.update(deltaTime);
    }
}
void Game::handlePlayerInput(Keyboard::Key key,bool isPressed)
{

}
void Game::proccessEvents()
{
    Event event;
    while(window.pollEvent(event))
    {
        if(event.type==Event::Closed)
            window.close();
        if(event.type==Event::KeyPressed&&event.key.code==Keyboard::Space)
        {
            gameState=State::GAME;
            Tile::time=180*60;
        }
        pacman.processEvents(event);
    }
}
void Game::render()
{
    window.clear();
    if(gameState==State::MENU)
    {
        menuRender();
    }
    if(gameState==State::GAME)
    {
        gameRender();
    }
    if(gameState==State::WON)
    {
        wonRender();
    }
    if(gameState==State::LOST)
    {
        lostRender();
    }
    window.draw(text);
    window.display();
}
void Game::gameRender()
{
    text.setPosition(0,0);
        text.setString("\n\t\t\t\Score:\n\t\t\t"+std::to_string(Tile::score));
        text2.setString("\n\t\t\t\t\t\t\t\t\t\t\t\t\tTime\n\t\t\t\t\t\t\t\t\t\t\t\t\t"+std::to_string(Tile::time/60));
        tileset.render();
        if(pacman.state==PacMan::ALIVE)
        {
            ghosts.checkCollision(&pacman);
            tileset.checkCollision(&pacman);
            for(int i=0;i<ghosts.size();++i)
            {
                tileset.checkCollision(ghosts.getGhost(i));
                tileset.proccessGhosts(ghosts.getGhost(i));
            }
            ghosts.checkCollision();
            ghosts.draw();
        }
        pacman.draw();
        for(int i=1;i<=pacman.lives;++i)
        {
            pacmanLife.setPosition(Vector2f(i*25,625));
            window.draw(pacmanLife);
        }
        window.draw(text2);
}
void Game::lostRender()
{
    text.setPosition(135,325);
    text.setString("\t You have lost!");
}
void Game::wonRender()
{
    text.setPosition(135,225);
    text.setString("\t\t   You won!\n\n\tYour Score: "+std::to_string(Tile::score)+"\n\n\t\tTime Left: "+std::to_string(Tile::time/60));
    if(pacman.lives!=0)
    {
        --pacman.lives;
        Tile::score+=600;
        sleep(seconds(1));
    }
    else if(Tile::time>0)
    {
        Tile::score+=10;
        Tile::time-=10;
    }
    for(int i=1;i<=pacman.lives;++i)
    {
        pacmanLife.setPosition(Vector2f(i*25,625));
        window.draw(pacmanLife);
    }
}
void Game::menuRender()
{
    text.setPosition(125,325);
    text.setString("Press  'Space' to start");
    window.draw(logo);
}
int main()
{
    std::string Name="PacMan";
    Game game(525,650,Name);
    game.run();
}
