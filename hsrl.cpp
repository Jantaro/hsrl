#include <SFML/Graphics.hpp>
using sf::Sprite;
#include <iostream>
#include <fstream>
#include <vector>
using std::vector;

void drawMap(vector<vector<int> >, const vector<vector<Sprite> >, sf::RenderWindow&);
vector<vector<int> > readMap();

int main()
{
  // Create the main rendering window
  sf::RenderWindow App(sf::VideoMode(512, 512, 32), "HSRL");

  sf::Image Image;
  Image.SetSmooth(false);
  Image.LoadFromFile("tileset.png");
  
  vector<vector<Sprite> > spriteMap(16, vector<Sprite>(16));
  // Creates spritemap so each image bit gets its own sprite
  for (int y = 0; y != 16; ++y){
    for (int x = 0; x != 16; ++x){
      spriteMap[y][x].SetImage(Image);
      spriteMap[y][x].SetSubRect(sf::IntRect(x*8, y*8, (x+1)*8, (y+1)*8));
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

    vector<vector<int> > charMap = readMap(); /* 
    {
      {0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x79,0x71,0x70,0x71,0x70,0x71,0x70,0x71,0x70,0x71,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x7A,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x7A,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x7A,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x7A,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70},
      {0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70}
    }; */

    drawMap(charMap, spriteMap, App);

    App.Display();
  }
  return EXIT_SUCCESS;
}

void drawMap(vector<vector<int> > v, const vector<vector<Sprite> > map, sf::RenderWindow& window){
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

vector<vector<int> > readMap(){
  std::ifstream f("map.txt");
  vector<vector<int> > charMap;
  if (f){
    for (int y=0; y !=16; ++y){
      vector<int> row;
      for (int x=0; x !=16; ++x){
        row.push_back(f.get());
      }
      f.seekg(1, std::ios_base::cur); // Seeks past each expected newline.
      charMap.push_back(row);
    }
  }
  return charMap;
}
