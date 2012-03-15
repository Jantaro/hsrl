//{{{ Includes
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
//}}}

//{{{ Declarations
typedef vector<vector<int> > Map;
typedef vector<vector<Sprite> > SpriteMap;
typedef pair<int, int> Coords;
typedef vector<string> MsgStack;

struct GameState {
  GameState();
  void drawMap();
  void drawObject(int, const Coords&);
  bool possible(const Coords&);
  void drawMessage(string, const Coords&);
  void drawMsgStack();

  RenderWindow window;
  SpriteMap tileset;
  Map charMap;
  MsgStack msgStack;
};
//}}}

//void drawMessage(string, const SpriteMap&, RenderWindow&);

vector<int> range(int, int);
template <class T> inline string to_string(const T&);

//{{{ Main
int main()
{
  //{{{ Initializations
  GameState game;
//  game.window.SetFramerateLimit(30); // hardcoded (TODO) framerate

  Coords playerPos(0,0); // hardcoded (TODO) initial player position
  Coords impPos(9,12); // hardcoded (TODO)
  Coords fpsPos(16,0); // hardcoded (TODO)
  Coords msgPos(0,16); // hardcoded (TODO)
  Coords newPos;
  bool action = false; // whether the player has made an action, allowing the simulation to run in response
  int ticks = 0;
  Coords ticksPos(16,2);
  string framerateText;
  string msgText = "this is a test, this is a test";
  game.msgStack.push_back("1 foo");
  game.msgStack.push_back("2 bar");
  game.msgStack.push_back("3 pop");
  game.msgStack.push_back("4 fiz");
  game.msgStack.push_back("5 fuk");
  //}}}

  //{{{ Game loop
  while (game.window.IsOpened()){
    newPos = playerPos;
    //{{{ Handle Events
    sf::Event Event;
    while (game.window.GetEvent(Event)){
      // close window
      if (Event.Type == sf::Event::Closed) game.window.Close();
      // handle keypresses
      if (Event.Type == sf::Event::KeyPressed){
        switch (Event.Key.Code) {
          case sf::Key::Escape : {game.window.Close(); break;}
          case sf::Key::Left   : {newPos.first--;      break;}
          case sf::Key::Right  : {newPos.first++;      break;}
          case sf::Key::Up     : {newPos.second--;     break;}
          case sf::Key::Down   : {newPos.second++;     break;}
          default : break;
        }
        if (action == false){
          if (game.possible(newPos) && (newPos != playerPos)){
            playerPos = newPos;
            action = true; // player has made a movement
            ticks++;
          }
        }
      }
    }
    //}}}

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
               
      if (game.possible(newImpPos)) impPos = newImpPos;
    }

    // clear the screen
    game.window.Clear(sf::Color(128, 128, 128));

    game.drawMap();
    game.drawObject('i', impPos); // imp
    game.drawObject('@', playerPos); // player

    framerateText = to_string(1/game.window.GetFrameTime());
    game.drawMessage(framerateText, fpsPos);
    game.drawMessage(to_string(ticks), ticksPos); 
 
    //game.drawMessage(msgText, msgPos);
    game.drawMsgStack();

    game.window.Display();
  }
  //}}}
  return EXIT_SUCCESS;
}
//}}}

//{{{ Functions
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
  for (string::iterator p=m.begin(); p!=m.end();++p){
    s = tileset[(*p)/16][(*p)%16];
    s.SetPosition((messageX+(std::distance(m.begin(),p)))*8, messageY*8);
    window.Draw(s);
  }
}

void GameState::drawMsgStack(){
  Coords msgStackPos(0,16);
  int msgSize = 4; // limits msgStack to display only the last four messages; hardcoded (TODO)
  for (MsgStack::size_type i = msgStack.size() - msgSize; i < msgStack.size(); i++){
    drawMessage(msgStack[i], msgStackPos);
    msgStackPos.second++;
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

template <class T> inline string to_string (const T& t){
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
//}}}
