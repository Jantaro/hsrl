#include <SFML/Graphics.hpp>
#include <iostream>

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

    // Display contents
    // Should probably make some functions to make this neat
    for (int i = 0; i != 16; ++i){
      Sprite.SetSubRect(sf::IntRect(i*8, 16, (i+1)*8, 24));
      Sprite.SetPosition(i*8, 16);
      App.Draw(Sprite);
    }

    // Shitty brute-force AWESOMENESS
    Sprite.SetSubRect(sf::IntRect(64, 16, 72, 24));
    Sprite.SetPosition(0, 0);
    App.Draw(Sprite);
    Sprite.SetSubRect(sf::IntRect(120, 16, 128, 24));
    Sprite.SetPosition(8, 0);
    App.Draw(Sprite);
    Sprite.SetSubRect(sf::IntRect(104, 16, 112, 24));
    Sprite.SetPosition(16, 0);
    App.Draw(Sprite);
    Sprite.SetSubRect(sf::IntRect(40, 16, 48, 24));
    Sprite.SetPosition(24, 0);
    App.Draw(Sprite);
    Sprite.SetSubRect(sf::IntRect(24, 24, 32, 32));
    Sprite.SetPosition(32, 0);
    App.Draw(Sprite);
    Sprite.SetSubRect(sf::IntRect(32, 24, 40, 32));
    Sprite.SetPosition(40, 0);
    App.Draw(Sprite);
    Sprite.SetSubRect(sf::IntRect(40, 24, 48, 32));
    Sprite.SetPosition(48, 0);
    App.Draw(Sprite);
    Sprite.SetSubRect(sf::IntRect(24, 16, 32, 24));
    Sprite.SetPosition(56, 0);
    App.Draw(Sprite);
    Sprite.SetSubRect(sf::IntRect(88, 16, 96, 24));
    Sprite.SetPosition(64, 0);
    App.Draw(Sprite);

    App.Display();
  }
  return EXIT_SUCCESS;
}
