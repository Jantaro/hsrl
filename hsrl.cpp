#include <SFML/Graphics.hpp>
using sf::Sprite;
using sf::RenderWindow;
#include <iostream>
#include <fstream>
#include <vector>
using std::vector;
#include <utility>
using std::pair;
#include <algorithm>
using std::binary_search;
#include <sstream>

using std::string;
#include <cstdlib>



typedef vector<vector<int> > Map;
typedef vector<vector<Sprite> > SpriteMap;
typedef pair<int, int> Coords;

struct GameState {
  GameState();
  void drawMap();
  void drawObject(int, const Coords&);
  bool possible(const Coords&);
  void drawMessage(string, const Coords&);

  RenderWindow window;
  SpriteMap tileset;
  Map charMap;
};


//void drawMessage(string, const SpriteMap&, RenderWindow&);

vector<int> range(int, int);
template <class T> inline string to_string(const T&);

int main()
{
  ////
  //  Initialization of variables
  ////
  
  GameState game;
//  game.window.SetFramerateLimit(30); // hardcoded (TODO) framerate


  Coords playerPos(0,0); // hardcoded (TODO) initial player position
  Coords impPos(9,12); // hardcoded (TODO)
  Coords fpsPos(16,0); // hardcoded (TODO)
  Coords msgPos(0,16); // hardcoded (TODO)
  bool action = false; // whether the player has made an action, allowing the simulation to run in response
  string framerateText;
  string msgText = "this is a test, this is a test";



  ////
  //  Main game loop
  ////

  // start game loop
  while (game.window.IsOpened())
  {
    sf::Event Event;
    while (game.window.GetEvent(Event))
    {
      // close window
      if (Event.Type == sf::Event::Closed)
        game.window.Close();

      Coords newPos = playerPos;
      // try to move the player
      if (action == false){ // player hasn't sent multiple keypresses
        if (game.window.GetInput().IsKeyDown(sf::Key::Left))  newPos.first--;
        if (game.window.GetInput().IsKeyDown(sf::Key::Right)) newPos.first++;
        if (game.window.GetInput().IsKeyDown(sf::Key::Up))    newPos.second--;
        if (game.window.GetInput().IsKeyDown(sf::Key::Down))  newPos.second++;
        // reset position if the destination was impossible
        if (game.possible(newPos) && (newPos != playerPos)){
          playerPos = newPos;
          action = true; // player has made a movement
        }
      }
    }

    if (action == true){ // simulation can take place
      action = false;

      // imp AI
      Coords newImpPos = impPos;
      /*
      int randDir = rand() % 4; // get a random direction
      if (randDir == 0) newImpPos.first++;
      if (randDir == 1) newImpPos.second--;
      if (randDir == 2) newImpPos.first--;
      if (randDir == 3) newImpPos.second++;*/
      
      // makes imp follow player
      if (impPos.first - playerPos.first > 2) newImpPos.first--;
      if (impPos.first - playerPos.first < -2) newImpPos.first++;
      if (impPos.second - playerPos.second > 2) newImpPos.second--;
      if (impPos.second - playerPos.second < -2) newImpPos.second++;
               
      // this does not work for some reason
      // TODO: get clean syntax for this
      /*
      switch (randDir){
        case 0: newImpPos.first++;
        case 1: newImpPos.second--;
        case 2: newImpPos.first--;
        case 3: newImpPos.second++;
        default: std::cout << "some sort of problem" << std::endl;
      }
      */
      if (game.possible(newImpPos)) impPos = newImpPos;
    }

    // clear the screen
    game.window.Clear(sf::Color(255, 255, 255));

    game.drawMap();
    game.drawObject('i', impPos); // imp
    game.drawObject('@', playerPos); // player

    framerateText = to_string(1/game.window.GetFrameTime());
    game.drawMessage(framerateText, fpsPos);
 
    game.drawMessage(msgText, msgPos);

    game.window.Display();
  }
  return EXIT_SUCCESS;
}



////
//  Functions
////


void GameState::drawMap(){
  int cursorX = 0;
  int cursorY = 0;
  int c;
  Sprite s;
  for (int i = 0;i != 256;++i){
    c = charMap[i/16][i%16]; // vector[row][element] or v[y][x]
    s = tileset[c/16][c%16];
    if (cursorX == 16){
      cursorX = 0;
      cursorY++;
    }
    s.SetPosition(cursorX*8, cursorY*8);
    window.Draw(s);
    cursorX++; 
  }
}

void GameState::drawObject(int c, const Coords& pos){
  Sprite s;
  s = tileset[c/16][c%16];
  s.SetPosition(pos.first*8, pos.second*8);
  window.Draw(s);
}

bool GameState::possible(const Coords& pos){
  int lowerXBound = 0; // hardcoded (TODO) size
  int upperXBound = 15;
  int lowerYBound = 0;
  int upperYBound = 15;
  int playerX = pos.first;
  int playerY = pos.second;
  // bounds must be checked before v is indexed with invalid values
  if (playerX < lowerXBound || playerX > upperXBound ||
      playerY < lowerYBound || playerY > upperYBound)
    return false;
  int c = charMap[playerY][playerX]; // segfaults if object is somehow outside map
  if (c == 'w') return false; // 'w' character for wall
  else return true;
}

void GameState::drawMessage(string m, const Coords& pos){
  Sprite s;
  int messageX = pos.first;
  int messageY = pos.second;
  for (string::iterator p=m.begin(); p!=(m.end()+1);++p){
    s = tileset[(*p)/16][(*p)%16];
    s.SetPosition((messageX+(std::distance(m.begin(),p)))*8, messageY*8);
    window.Draw(s);
  }
}

vector<int> range(int start, int end){
  vector<int> v;
  if (start < end){
    for (int i=start;i!=(end+1);++i){
      v.push_back(i);
    }
  }
  else if (start > end){
    for (int i=start;i!=(end-1);--i){
      v.push_back(i);
    }
  }
  else v.push_back(start); // just use the value if they are equal
  return v;
}

template <class T>
inline string to_string (const T& t)
{
std::stringstream ss;
ss << t;
return ss.str();
}

GameState::GameState(): window(sf::VideoMode(512, 512, 32), "HSRL", sf::Style::Close), tileset(16, vector<Sprite>(16)) {  // hardcoded (TODO) size

  window.SetFramerateLimit(30);

  // construct tileset
  static sf::Image Image;
  Image.SetSmooth(false);
  Image.LoadFromFile("tileset.png");
  // creates spritemap so each image bit gets its own sprite
  for (int y = 0; y != 16; ++y){
    for (int x = 0; x != 16; ++x){
      tileset[y][x].SetImage(Image);
      tileset[y][x].SetSubRect(sf::IntRect(x*8, y*8, (x+1)*8, (y+1)*8));
    }
  }

  // construct map
  std::ifstream f("map.txt"); // hardcoded (TODO) file
  if (f){
    for (int y=0; y !=16; ++y){ // hardcoded (TODO) size 16x16
      vector<int> row;
      for (int x=0; x !=16; ++x){
        row.push_back(f.get());
      }
      f.seekg(1, std::ios_base::cur); // seeks past each expected newline
      charMap.push_back(row);
    }
  }
}
