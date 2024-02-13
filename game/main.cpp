
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <map>
#include <conio.h>
#include <fstream>
#include <cctype>
#include <vector>

#include "Animation.h"
#include "Board.h"
#include "LetterTile.h"

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

	if (connectionType == 'c') // client character spawns on right
	{
		player.setPosition(1760, 540);
		player2.setPosition(220, 540);
	}
	else //server character spawns on left
	{
		player.setPosition(220, 540);
		player2.setPosition(1760, 540);
	}
	
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

	// Letter Array

	Board B(7, 7);
	std::vector<std::vector<sf::RectangleShape>> letterArray(B.xDim, std::vector<sf::RectangleShape>(B.yDim, sf::RectangleShape()));
	for (int i = 0; i < B.xDim; i++)
	{
		for (int j = 0; j < B.yDim; j++)
		{
			letterArray[i][j] = sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
		}
	}

	std::string chosenWord;
	std::vector<std::vector<bool>> chosenArr(B.xDim, std::vector<bool>(B.yDim, false));
	//gameLoop

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
			if (evnt.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
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
					player.move(-0.35f, -0.35f);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				{
					player.move(-0.35f, 0.35f);
				}
				else
				{
					player.move(-0.5f, 0.0f);
				}
				animation.Update(1, deltaTime);


			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) // RIGHT
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				{
					player.move(0.35f, -0.35f);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				{
					player.move(0.35f, 0.35f);
				}
				else
				{
					player.move(0.5f, 0.0f);
				}
				animation.Update(0, deltaTime);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) // UP
			{
				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				{
					player.move(0.0f, -0.5f);
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
					player.move(0.0f, 0.5f);
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

		if (connectionType == 'c') //player on right
		{
			if (player.getPosition().x > 1796)
			{
				player.setPosition(1796, player.getPosition().y);
			}
			if (player.getPosition().x < 1740)
			{
				player.setPosition(1740, player.getPosition().y);
			}
			if (player.getPosition().y < 235)
			{
				player.setPosition(player.getPosition().x, 235);
			}
			if (player.getPosition().y > 900)
			{
				player.setPosition(player.getPosition().x, 900);
			}
		
		}
		else //player on left
		{
			if (player.getPosition().x < 166)
			{
				player.setPosition(166, player.getPosition().y);
			}
			if (player.getPosition().x > 230)
			{
				player.setPosition(230, player.getPosition().y);
			}
			if (player.getPosition().y < 235)
			{
				player.setPosition(player.getPosition().x, 235);
			}
			if (player.getPosition().y > 900)
			{
				player.setPosition(player.getPosition().x, 900);
			}
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
		
		for (int i = 0; i < B.xDim; i++)
		{
			for (int j = 0; j < B.yDim; j++)
			{
				int letterX = (1920 - 760) / 2 + i * 110;
				int letterY = (1080 - 760) / 2 + j * 110;

				sf::Texture letterTexture;
				std::string str = "sprites/letters/";
				str += B.getTile(i, j).letter;
				str += ".png";
									
				letterTexture.loadFromFile(str);
				letterArray[i][j].setTexture(&letterTexture);
				letterArray[i][j].setPosition(letterX, letterY);
				window.draw(letterArray[i][j]);

				if (chosenArr[i][j])
				{
					sf::Texture letterTexture;
					std::string str = "sprites/letters/";
					str += B.getTile(i, j).letter;
					str += "selected.png";

					letterTexture.loadFromFile(str);
					letterArray[i][j].setTexture(&letterTexture);
					letterArray[i][j].setPosition(letterX, letterY);
					window.draw(letterArray[i][j]);
				}


				
				// if you click on one of the letters
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (sf::Mouse::getPosition(window).x >= letterX && sf::Mouse::getPosition(window).x <= letterX + 100) && (sf::Mouse::getPosition(window).y >= letterY && sf::Mouse::getPosition(window).y <= letterY + 100))
				{
					if (!chosenArr[i][j]) {
						chosenWord += B.getTile(i, j).letter;
						//std::cout << B.getTile(i, j).isLetterChosen << "\n";
						chosenArr[i][j] = true;
						//std::cout << B.getTile(i, j).isLetterChosen << "\n\n\n";

					}
					

				}
				if(!(sf::Mouse::isButtonPressed(sf::Mouse::Left)))
				{
					for (int i = 0; i < B.xDim; i++)
					{
						for (int j = 0; j < B.yDim; j++)
						{
							if (chosenArr[i][j])
							{
								chosenArr[i][j] = false;
							}
						}
					}
					chosenWord = "";

				}

				
			}
		}
		
		/*
		for (int i = 0; i < B.xDim; i++)
		{
			for (int j = 0; j < B.yDim; j++)
			{
				sf::Texture letterTexture;
				std::string str = "sprites/letters/";
				str += B.getTile(i, j).letter;
				str += ".png";

				letterTexture.loadFromFile(str);
				letterArray[i][j].setTexture(&letterTexture);
				letterArray[i][j].setPosition((1920 - 350) / 2 + i * 50, (1080 - 350) / 2 + j * 50);
				window.draw(letterArray[i][j]);
			}
		}
		*/

		
		player.setTextureRect(animation.uvRect);
		player2.setTextureRect(animation2.uvRect);

		
		window.draw(player);
		window.draw(player2);
		window.display();
		window.clear();
	}

	system("pause");
}