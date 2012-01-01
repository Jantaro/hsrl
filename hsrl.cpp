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



typedef vector<vector<int> > Map;
typedef vector<vector<Sprite> > SpriteMap;
typedef pair<int, int> Coords;

Map readMap();
void drawMap(const Map&, const SpriteMap&, RenderWindow&);
void drawObject(int, const Coords&, const SpriteMap&, RenderWindow&);
bool impossible(const Coords&, const Map&);
void drawMessage(string, const SpriteMap&, RenderWindow&);
vector<int> range(int, int);
template <class T> inline string to_string(const T&);

int main()
{
  ////
  //  Initialization of variables
  ////
  
  // create the main rendering window
  RenderWindow App(sf::VideoMode(512, 512, 32), "HSRL", sf::Style::Close);

  App.SetFramerateLimit(30); // hardcoded framerate

  sf::Image Image;
  Image.SetSmooth(false);
  Image.LoadFromFile("tileset.png");
  
  SpriteMap tileset(16, vector<Sprite>(16));
  // creates spritemap so each image bit gets its own sprite
  for (int y = 0; y != 16; ++y){
    for (int x = 0; x != 16; ++x){
      tileset[y][x].SetImage(Image);
      tileset[y][x].SetSubRect(sf::IntRect(x*8, y*8, (x+1)*8, (y+1)*8));
    }
  }

  Coords playerPos(0,0); // hardcoded initial player position
  string framerateText;

  Map charMap = readMap();



  ////
  //  Main game loop
  ////

  // start game loop
  while (App.IsOpened())
  {
    sf::Event Event;
    while (App.GetEvent(Event))
    {
      // close window
      if (Event.Type == sf::Event::Closed)
        App.Close();

      Coords oldPos = playerPos;
      // try to move the player
      if (App.GetInput().IsKeyDown(sf::Key::Left))  playerPos.first--;
      if (App.GetInput().IsKeyDown(sf::Key::Right)) playerPos.first++;
      if (App.GetInput().IsKeyDown(sf::Key::Up))    playerPos.second--;
      if (App.GetInput().IsKeyDown(sf::Key::Down))  playerPos.second++;
      // ceset position if the destination was impossible
      if (impossible(playerPos, charMap)) playerPos = oldPos;
    }

    // clear the screen
    App.Clear(sf::Color(255, 255, 255));

    drawMap(charMap, tileset, App);
    drawObject('@', playerPos, tileset, App);

    framerateText = to_string(1/App.GetFrameTime());
    drawMessage(framerateText, tileset, App);

    App.Display();
  }
  return EXIT_SUCCESS;
}



////
//  Functions
////

Map readMap(){
  std::ifstream f("map.txt");
  Map charMap;
  if (f){
    for (int y=0; y !=16; ++y){ // hardcoded size 16x16
      vector<int> row;
      for (int x=0; x !=16; ++x){
        row.push_back(f.get());
      }
      f.seekg(1, std::ios_base::cur); // seeks past each expected newline
      charMap.push_back(row);
    }
  }
  return charMap;
}

void drawMap(const Map& v, const SpriteMap& map, RenderWindow& window){
  int cursorX = 0;
  int cursorY = 0;
  int c;
  Sprite s;
  for (int i = 0;i != 256;++i){
    c = v[i/16][i%16]; // vector[row][element] or v[y][x]
    s = map[c/16][c%16];
    if (cursorX == 16){
      cursorX = 0;
      cursorY++;
    }
    s.SetPosition(cursorX*8, cursorY*8);
    window.Draw(s);
    cursorX++; 
  }
}

void drawObject(int c, const Coords& pos, const SpriteMap& map, RenderWindow& window){
  Sprite s;
  s = map[c/16][c%16];
  s.SetPosition(pos.first*8, pos.second*8);
  window.Draw(s);
}

bool impossible(const Coords& pos, const Map& v){
  int lowerXBound = 0; // hardcoded size
  int upperXBound = 15;
  int lowerYBound = 0;
  int upperYBound = 15;
  int playerX = pos.first;
  int playerY = pos.second;
  // bounds must be checked before v is indexed with invalid values
  if (playerX < lowerXBound || playerX > upperXBound ||
      playerY < lowerYBound || playerY > upperYBound)
    return true;
  int c = v[playerY][playerX]; // segfaults if player is somehow outside map
  if (c == 'w') return true; // 'w' character for wall
  else return false;
}

void drawMessage(string m, const SpriteMap& map, RenderWindow& window){
  Sprite s;
  int messageX = 16; // hardcoded position
  int messageY = 0;
  for (string::iterator p=m.begin(); p!=(m.end()+1);++p){
    s = map[(*p)/16][(*p)%16];
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
