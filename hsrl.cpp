#include <SFML/Graphics.hpp>
#include <iostream>

sf::Sprite getSprite(int, const sf::Sprite[16][16]);
void writeChar(int, int, int, const sf::Sprite[16][16], sf::RenderWindow&);
void writeRect(int, int, int, const sf::Sprite[16][16], sf::RenderWindow&);
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
  
  // Creates spritemap so each image bit gets its own sprite
  sf::Sprite spritemap[16][16]; 
  for (int i = 0; i != 16; ++i){
    for (int j = 0; j != 16; ++j){
      spritemap[i][j].SetImage(Image);
      spritemap[i][j].SetSubRect(sf::IntRect(i*8, j*8, (i+1)*8, (j+1)*8));
    }
  }

  sf::Sprite Sprite;
  Sprite.SetImage(Image);
  Sprite.SetPosition(0, 0);

  // writeRect testing parameters
  int curX = 0;
  int curY = 0;
  int width = 16;

  // Start game loop
  while (App.IsOpened())
  {
    sf::Event Event;
    while (App.GetEvent(Event))
    {
      // Close window
      if (Event.Type == sf::Event::Closed)
        App.Close();

      // Testing (currently for writeRect)
      if (App.GetInput().IsKeyDown(sf::Key::Left))  curX--;
      if (App.GetInput().IsKeyDown(sf::Key::Right)) curX++;
      if (App.GetInput().IsKeyDown(sf::Key::Up))    curY--;
      if (App.GetInput().IsKeyDown(sf::Key::Down))  curY++;
      if (App.GetInput().IsKeyDown(sf::Key::N))     width--;
      if (App.GetInput().IsKeyDown(sf::Key::M))     width++;
    }

    // Clear the screen
    App.Clear(sf::Color(255, 255, 255));


    /*
    cursorX = 3;
    cursorY = 0;
    for (int i = 0;i != 256;++i){
      writeChar(i, 3, 16, spritemap, App);
    }*/

    writeRect(curX, width, curY, spritemap, App);

    App.Display();
  }
  return EXIT_SUCCESS;
}

sf::Sprite getSprite(int c, const sf::Sprite map[16][16]){
  return map[c%16][c/16];
}

void writeChar(int c, int start, int length, const sf::Sprite map[16][16], sf::RenderWindow& window){
  sf::Sprite s;
  s = getSprite(c, map);
  if (cursorX == length + start){
    cursorX = start;
    cursorY++;
  }
  s.SetPosition(cursorX*8, cursorY*8);
  window.Draw(s);
  cursorX++;
}

void writeRect(int xi, int xl, int yi, const sf::Sprite map[16][16], sf::RenderWindow& window){
  cursorX = xi;
  cursorY = yi;
  for (int i = 0;i != 256;++i){
    writeChar(i, xi, xl, map, window);
  }
}
