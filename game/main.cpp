
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <map>
#include <conio.h>
#include <fstream>
#include <cctype>

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

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "game", sf::Style::Default); //change window size

	sf::RectangleShape player(sf::Vector2f(50.0f, 50.0f));
	sf::RectangleShape player2(sf::Vector2f(50.0f, 50.0f));

	player.setOrigin(sf::Vector2f(50.0f, 50.0f)); //origin of the object
	player2.setOrigin(sf::Vector2f(50.0f, 50.0f)); //origin of the object

	player.setPosition(960, 300);
	player2.setPosition(960, 300);

	sf::Texture playerTexture;
	sf::Texture player2Texture;
	playerTexture.loadFromFile("sprites/RedSheet.png");
	player2Texture.loadFromFile("sprites/BlueSheet.png");
	player.setTexture(&playerTexture); // setTexture needs a pointer - &
	player2.setTexture(&player2Texture);

	Animation animation(&playerTexture, sf::Vector2u(2, 2), 0.3f);
	Animation animation2(&player2Texture, sf::Vector2u(2, 2), 0.3f);


	//tilemap


	std::ifstream openfile("Map1.txt");

	sf::Texture brickTexture;
	sf::Sprite brickTiles;
	brickTexture.loadFromFile("sprites/Brick.png");
	brickTiles.setTexture(brickTexture);

	sf::Texture floorTexture;
	sf::Sprite floorTiles;
	floorTexture.loadFromFile("sprites/Floor.png");
	floorTiles.setTexture(floorTexture);

	int map[100][100];
	sf::Vector2i loadCounter = sf::Vector2i(0, 0);

	if (openfile.is_open())
	{
		while (!openfile.eof())
		{
			std::string str;
			openfile >> str;
			char x = str[0];
			if (!isdigit(x))
			{
				map[loadCounter.x][loadCounter.y] = -1;
			}
			else
			{
				map[loadCounter.x][loadCounter.y] = (x - '0');
			}

			if (openfile.peek() == '\n')
			{
				loadCounter.x = 0;
				loadCounter.y++;
			}
			else
			{
				loadCounter.x++;
			}

		}

		loadCounter.y++;
	}



	float deltaTime = 0.0f;
	sf::Clock clock;

	sf::Vector2f prevPosition, p2Position; //positions of the dudes

	socket.setBlocking(false);

	bool focused = false;

	int socketCounter = 0;

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
				focused = true;
			}
			else if (evnt.type == sf::Event::LostFocus)
			{
				focused = false;
			}
		}

		prevPosition = player.getPosition();

		if (focused)
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

		if (player.getPosition().x < 352)
		{
			player.setPosition(352, player.getPosition().y);
		}
		if (player.getPosition().x > 1600)
		{
			player.setPosition(1600, player.getPosition().y);
		}
		if (player.getPosition().y < 96)
		{
			player.setPosition(player.getPosition().x, 96);
		}
		if (player.getPosition().y > 512)
		{
			player.setPosition(player.getPosition().x, 512);
		}

		sf::Packet packet;

		if (prevPosition != player.getPosition())
		{
			packet << player.getPosition().x << player.getPosition().y;
			socketCounter++;
			if (socketCounter == 3) {
				socketCounter = 0;
				socket.send(packet);
			}
	
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

		for (int i = 0; i < loadCounter.x; i++) {
			for (int j = 0; j < loadCounter.y; j++) {
				if (map[i][j] == 1)
				{
					floorTiles.setPosition(i * 32, j * 32);
					floorTiles.setTexture(floorTexture);
					window.draw(floorTiles);
				}
				else if (map[i][j] == 2)
				{
					brickTiles.setPosition(i * 32, j * 32);
					brickTiles.setTexture(brickTexture);
					window.draw(brickTiles);
				}
			}
		}


		player.setTextureRect(animation.uvRect);
		player2.setTextureRect(animation2.uvRect);

		
		window.draw(player);
		window.draw(player2);
		window.display();
		window.clear();
	}

	system("pause");
}