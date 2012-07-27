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
using std::make_pair;
#include <algorithm>
using std::binary_search;
#include <sstream>
#include <string>
using std::string;
#include <cstdlib>
//}}}

//{{{ Declarations
typedef vector<vector<int> > Map;
typedef vector<vector<Sprite> > SpriteMap;
typedef pair<int, int> Coords;
typedef vector<string> MsgStack;

struct Creature{
  Creature(string, int, int, Coords);
  void move(); //(TODO) maybe make virtual

  string name;
  int symbol;
  int hp;
  Coords pos;
};

struct Item{
  Item(string, int, int, Coords);

  string name;
  int symbol;
  int mass;
  Coords pos;
};

struct GameState{
  GameState();
  bool possible(const Coords&);
  void drawMap();
  void drawObject(int, const Coords&);
  void drawMessage(string, const Coords&);
  void drawMsgStack();
  void drawCreatures();
  void drawItems();

  RenderWindow window;
  SpriteMap tileset;
  Map charMap;
  MsgStack msgStack;
  vector<Creature> creatures;
  vector<Item> items;
};
//}}}

//void drawMessage(string, const SpriteMap&, RenderWindow&);

vector<int> range(int, int);
template <class T> inline string to_string(const T&);

Creature player("player", '@', 5, make_pair(0, 0));

GameState game;

//{{{ Main
int main()
{
  //{{{ Initializations

  Creature imp("imp", 'i', 2, make_pair(9, 12));
  game.creatures.push_back(imp);

  Item food("food", '%', 1, make_pair(11,13));
  game.items.push_back(food);

  //Coords playerPos(0,0); // hardcoded (TODO) initial player position
  //Coords impPos(9,12); // hardcoded (TODO)
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
    newPos = player.pos;
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
          case sf::Key::Numpad1 : {newPos.first--; newPos.second++; break;}
          case sf::Key::Numpad2 : {newPos.second++;                 break;}
          case sf::Key::Numpad3 : {newPos.first++; newPos.second++; break;}
          case sf::Key::Numpad4 : {newPos.first--;                  break;}
          case sf::Key::Numpad6 : {newPos.first++;                  break;}
          case sf::Key::Numpad7 : {newPos.first--; newPos.second--; break;}
          case sf::Key::Numpad8 : {newPos.second--;                 break;}
          case sf::Key::Numpad9 : {newPos.first++; newPos.second--; break;}
          default : break;
        }
        if (action == false){
          if (game.possible(newPos) && (newPos != player.pos)){
            player.pos = newPos;
            action = true; // player has made a movement
            ticks++;
          }
        }
      }
    }
    //}}}

    if (action == true){ // simulation can take place
      action = false;
      for (int i = 0; i != (int)game.creatures.size(); i++){
        game.creatures[i].move();
      }
    }

    // clear the screen
    game.window.Clear(sf::Color(128, 128, 128));

    game.drawMap();
    game.drawItems();
    //game.drawObject('i', imp.pos); // imp
    game.drawCreatures();
    game.drawObject('@', player.pos); // player

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
Creature::Creature(string cname, int csymbol, int chp, Coords cpos): name(cname), symbol(csymbol), hp(chp), pos(cpos){}

void Creature::move(){
  Coords newPos = pos;
  // makes creature "follow" player
  if (pos.first - player.pos.first > 2) newPos.first--;
  if (pos.first - player.pos.first < -2) newPos.first++;
  if (pos.second - player.pos.second > 2) newPos.second--;
  if (pos.second - player.pos.second < -2) newPos.second++;
           
  if (game.possible(newPos)) pos = newPos;
}

Item::Item(string cname, int csymbol, int cmass, Coords cpos): name(cname), symbol(csymbol), mass(cmass), pos(cpos){}

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

void GameState::drawCreatures(){
  for (int i = 0; i != (int)creatures.size(); i++){
    drawObject(creatures[i].symbol, creatures[i].pos);
  }
}

void GameState::drawItems(){
  for (int i = 0; i != (int)items.size(); i++){
    drawObject(items[i].symbol, items[i].pos);
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

template <class T> inline string to_string(const T& t){
  std::stringstream ss;
  ss << t;
  return ss.str();
}

//}}}
