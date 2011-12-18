#include <SFML/Graphics.hpp>
#include <iostream>

sf::Sprite getSprite(int, const sf::Sprite[16][16]);
sf::Sprite spritemap[16][16];
void writeChar(int);
int cursorX,cursorY;
sf::RenderWindow App;

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
  for (int i = 0; i != 16; ++i){
    for (int j = 0; j != 16; ++j){
      spritemap[i][j].SetImage(Image);
      spritemap[i][j].SetSubRect(sf::IntRect(i*8, j*8, (i+1)*8, (j+1)*8));
    }
  }

  sf::Sprite Sprite;
  Sprite.SetImage(Image);
  Sprite.SetPosition(0, 0);

  // Start game loop
  while (App.IsOpened())
  {
    // Process events
    sf::Event Event;
    while (App.GetEvent(Event))
    {
      // Close window
      if (Event.Type == sf::Event::Closed)
        App.Close();

      // Move the sprite
      // Pretty useless at this point
      if (App.GetInput().IsKeyDown(sf::Key::Left))  Sprite.Move(-8, 0);
      if (App.GetInput().IsKeyDown(sf::Key::Right)) Sprite.Move( 8, 0);
      if (App.GetInput().IsKeyDown(sf::Key::Up))    Sprite.Move(0, -8);
      if (App.GetInput().IsKeyDown(sf::Key::Down))  Sprite.Move(0,  8);
    }

    // Clear the screen
    App.Clear(sf::Color(255, 255, 255));


    cursorX = 0;
    cursorY = 4;
    for (int i = 0;i != 256;++i){
      Sprite = getSprite(i, spritemap);
      if (cursorX == 16){
        cursorX = 0;
        cursorY++;}
      Sprite.SetPosition(cursorX*8, cursorY*8);
      App.Draw(Sprite);
      cursorX++;
    }

    App.Display();
  }
  return EXIT_SUCCESS;
}

sf::Sprite getSprite(int c, const sf::Sprite map[16][16]){
  return map[c%16][c/16];
}
/*
void writeChar(int c){
  sf::Sprite s;
  s = getSprite(c);
  if (cursorX == 16){
    cursorX = 0;
    cursorY++;}
  s.SetPosition(cursorX*8, cursorY*8);
  App.Draw(s);
  cursorX++;
} */
