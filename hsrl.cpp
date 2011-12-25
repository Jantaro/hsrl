#include <SFML/Graphics.hpp>
using sf::Sprite;
#include <iostream>
#include <fstream>
#include <vector>
using std::vector;

Sprite getSprite(int, const vector<vector<Sprite> >);
void writeChar(int, const vector<vector<Sprite> >, sf::RenderWindow&);
//void writeMap(int[16][16], const Sprite[16][16], sf::RenderWindow&);
//vector<vector<int> > readMap();
int cursorX;
int cursorY;

int main()
{
  // Create the main rendering window
  sf::RenderWindow App(sf::VideoMode(512, 512, 32), "HSRL");

  sf::Image Image;
  Image.SetSmooth(false);
  if (!Image.LoadFromFile("tileset.png"))
  {
    // Error...
  }
  
  vector<vector<Sprite> > spriteMap(16, vector<Sprite>(16));
  // Creates spritemap so each image bit gets its own sprite
  for (int i = 0; i != 16; ++i){
    for (int j = 0; j != 16; ++j){
      spriteMap[i][j].SetImage(Image);
      spriteMap[i][j].SetSubRect(sf::IntRect(i*8, j*8, (i+1)*8, (j+1)*8));
    }
  }

  // Start game loop
  while (App.IsOpened())
  {
    sf::Event Event;
    while (App.GetEvent(Event))
    {
      // Close window
      if (Event.Type == sf::Event::Closed)
        App.Close();

      // Move the sprite
      // Pretty useless at this point 
/*    if (App.GetInput().IsKeyDown(sf::Key::Left))  sprite.Move(-8, 0);
      if (App.GetInput().IsKeyDown(sf::Key::Right)) sprite.Move( 8, 0);
      if (App.GetInput().IsKeyDown(sf::Key::Up))    sprite.Move(0, -8);
      if (App.GetInput().IsKeyDown(sf::Key::Down))  sprite.Move(0,  8);
*/  }

    // Clear the screen
    App.Clear(sf::Color(255, 255, 255));

    vector<vector<int> > charMap =
    {
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,124,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,124,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,124,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46},
      {46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46}
    };

    cursorX = 0;
    cursorY = 0;
    for (int i = 0;i != 256;++i){
      writeChar(i, spriteMap, App);
    }

    App.Display();
  }
  return EXIT_SUCCESS;
}

Sprite getSprite(int c, const vector<vector<Sprite> > map){
  return map[c%16][c/16];
}

void writeChar(int c, const vector<vector<Sprite> > map, sf::RenderWindow& window){
  Sprite s;
  s = getSprite(c, map);
  if (cursorX == 16){
    cursorX = 0;
    cursorY++;
  }
  s.SetPosition(cursorX*8, cursorY*8);
  window.Draw(s);
  cursorX++;
}
/*
vector<vector<int> > readMap(){
  std::ifstream f("map.txt");
  vector<vector<int> > charMap;
  if (f){
    vector<int> v;
    for (int i=0; i !=16; ++i){
      for (int j=0; j !=16; ++j){
        charMap[j][i] = f.get();
      }
      f.seekg(1,ios::cur);
    }
  }
  return charMap;
}
*/
