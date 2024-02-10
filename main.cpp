
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Animation.h"

int main()
{
	//sf::Style:: - checks window styles

	// GameStage Variable:
	// GameStage = 0 - Title Screen
	// GameStage = 1 - Lobby
	// GameStage = 2 - Game

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "game", sf::Style::Default); //change window size
	sf::RectangleShape player(sf::Vector2f(100.0f, 100.0f)); // replace with sprite
	player.setOrigin(sf::Vector2f(50.0f, 50.0f)); //origin of the object
	sf::Texture playerTexture;
	playerTexture.loadFromFile("sprites/RedSheet.png");
	player.setTexture(&playerTexture); // setTexture needs a pointer - &

	Animation animation(&playerTexture, sf::Vector2u(2, 2), 0.3f);

	float deltaTime = 0.0f;
	sf::Clock clock;

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		// 0.1 / sqrt(2) = 0.0707
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) // LEFT
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			{
				player.move(-0.0707f, -0.0707f);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			{
				player.move(-0.0707f, 0.0707f);
			}
			else
			{
				player.move(-0.1f, 0.0f);
			}
			animation.Update(1, deltaTime);


		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) // RIGHT
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			{
				player.move(0.0707f, -0.0707f);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			{
				player.move(0.0707f, 0.0707f);
			}
			else
			{
				player.move(0.1f, 0.0f);
			}
			animation.Update(0, deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) // UP
		{
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				player.move(0.0f, -0.1f);
			}
			if (animation.getCurrentImage().y == 0) //Sprite facing right
			{
				animation.Update(0, deltaTime);
			}
			else
			{
				animation.Update(1, deltaTime);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) // DOWN
		{
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				player.move(0.0f, 0.1f);
			}
			if (animation.getCurrentImage().y == 0) //Sprite facing right
			{
				animation.Update(0, deltaTime);
			}
			else
			{
				animation.Update(1, deltaTime);
			}
		}

		/* implement magic here
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			//sf::Mouse::getPosition(window); need this so that a button press is relative to the window and not the screen
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			player.setPosition((float)mousePos.x, (float)mousePos.y);

		}
		*/

		player.setTextureRect(animation.uvRect);

		window.clear(sf::Color::White);
		window.draw(player);
		window.display();
	}

	return 0;
}