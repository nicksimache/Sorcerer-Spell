
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <map>
#include <conio.h>

#include "Animation.h"

int main()
{
	//sf::Style:: - checks window styles

	// GameStage Variable:
	// GameStage = 0 - Title Screen
	// GameStage = 1 - Lobby
	// GameStage = 2 - Game

	sf::IpAddress ip = sf::IpAddress::getLocalAddress(); // maybe need to change later
	sf::TcpSocket socket;
	char connectionType;

	std::cout << "(s) for server, (c) for client: ";
	std::cin >> connectionType;



	if (connectionType == 's')
	{
		sf::TcpListener listener;
		listener.listen(2000); //port
		listener.accept(socket);
	}
	else
	{
		socket.connect(ip, 2000);
	}

	sf::RenderWindow window(sf::VideoMode(800, 800), "game", sf::Style::Default); //change window size

	sf::RectangleShape player(sf::Vector2f(100.0f, 100.0f)); // replace with sprite
	sf::RectangleShape player2(sf::Vector2f(100.0f, 100.0f));

	player.setOrigin(sf::Vector2f(50.0f, 50.0f)); //origin of the object
	player2.setOrigin(sf::Vector2f(50.0f, 50.0f)); //origin of the object

	sf::Texture playerTexture;
	sf::Texture player2Texture;
	playerTexture.loadFromFile("sprites/RedSheet.png");
	player2Texture.loadFromFile("sprites/BlueSheet.png");
	player.setTexture(&playerTexture); // setTexture needs a pointer - &
	player2.setTexture(&player2Texture);

	Animation animation(&playerTexture, sf::Vector2u(2, 2), 0.3f);
	Animation animation2(&player2Texture, sf::Vector2u(2, 2), 0.3f);


	float deltaTime = 0.0f;
	sf::Clock clock;

	sf::Vector2f prevPosition, p2Position; //positions of the dudes

	socket.setBlocking(false);

	bool update = false;

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (evnt.type == sf::Event::GainedFocus)
			{
				update = true;
			}
			else if (evnt.type == sf::Event::LostFocus)
			{
				update = false;
			}
		}

		prevPosition = player.getPosition();

		if (update)
		{
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

		}

		sf::Packet packet;

		if (prevPosition != player.getPosition())
		{
			packet << player.getPosition().x << player.getPosition().y;
			socket.send(packet);
		}

		socket.receive(packet);
		if (packet >> p2Position.x >> p2Position.y)
		{
			if (player2.getPosition().x < p2Position.x)
			{
				animation2.Update(0, deltaTime);
			}
			else if (player2.getPosition().x > p2Position.x)
			{
				animation2.Update(1, deltaTime);
			}
			else if (animation2.getCurrentImage().y == 0)
			{
				animation2.Update(0, deltaTime);
			}
			else if (animation2.getCurrentImage().y == 1)
			{
				animation2.Update(1, deltaTime);
			}
			player2.setPosition(p2Position);

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
		player2.setTextureRect(animation2.uvRect);

		
		window.draw(player);
		window.draw(player2);
		window.display();
		window.clear(sf::Color::White);
	}

	system("pause");
}