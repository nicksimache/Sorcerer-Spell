
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
#include <list>
#include <algorithm>

#include "Animation.h"
#include "Board.h"
#include "LetterTile.h"

int main()
{
	Board B(7, 7);
	int updateStage = 0;

	//sf::Style:: - checks window styles

	// GameStage Variable:
	// gameStage = 1 - player1 turn
	// gameStage = 2 - player2 turn
	// gameStage = 3 - fight

	int gameStage = 0;

	sf::IpAddress ip = sf::IpAddress::getLocalAddress(); // maybe need to change later
	sf::TcpSocket socket;
	char connectionType;
	char buffer[2000];

	std::cout << "(s) for server, (c) for client: ";
	std::cin >> connectionType;



	if (connectionType == 's')
	{
		std::cout << ip;
		sf::TcpListener listener;
		listener.listen(55001); //port
		listener.accept(socket);

		gameStage = 1;

	}
	else
	{
		std::cin >> ip;
		socket.connect(ip, 55001);

		gameStage = 2;
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
	std::ifstream opendictionary("Dictionary.txt");

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

	bool isEnglishWord = false;

	std::vector<std::vector<sf::RectangleShape>> letterArray(B.xDim, std::vector<sf::RectangleShape>(B.yDim, sf::RectangleShape()));
	for (int i = 0; i < B.xDim; i++)
	{
		for (int j = 0; j < B.yDim; j++)
		{
			letterArray[i][j] = sf::RectangleShape(sf::Vector2f(100.0f, 100.0f));
		}
	}

	std::string chosenWord = "";
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

	//string array of dictionary words
	std::vector<std::string> dictionary(1000000);

	if (opendictionary.is_open())
	{
		int i = 0;
		while (!opendictionary.eof())
		{
			std::string str;
			opendictionary >> str;
			dictionary[i] = str;
			i++;
		}
	}

	int ammo = 0;
	int currentWordPoints = 0;


	float deltaTime = 0.0f;
	sf::Clock clock;

	sf::Vector2f prevPosition, p2Position; //positions of the dudes

	socket.setBlocking(false);

	bool focused = false;

	int socketCounter = 0;

	while (window.isOpen())
	{
		sf::Packet packet;

		

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

		//bounds
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


		if (prevPosition != player.getPosition()) {
			packet << player.getPosition().x << player.getPosition().y;
			
		}
		else {
			packet << -1 << -1;
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


				if (gameStage == 1) // if it is your turn
				{

					// if you click on one of the letters
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (sf::Mouse::getPosition(window).x >= letterX && sf::Mouse::getPosition(window).x <= letterX + 100) && (sf::Mouse::getPosition(window).y >= letterY && sf::Mouse::getPosition(window).y <= letterY + 100))
					{

						std::cout << "click";

						if (!chosenArr[i][j]) {
							chosenWord += B.getTile(i, j).letter;
							std::cout << chosenWord << std::endl;
							currentWordPoints += B.getTile(i, j).point;
							chosenArr[i][j] = true;
							std::cout << " " << i << " " << j << std::endl;

						}


					}
					if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left)))
					{
						
						// if its valid, change turn, add points etc.
						if (isEnglishWord)
						{
							isEnglishWord = false;
							ammo += currentWordPoints;
							std::cout << "eish";
							gameStage++;
							if (gameStage == 3)
							{
								gameStage == 1;
							}
							updateStage = 1;
							packet << updateStage;
						}
						else {
							updateStage = 0;
							packet << updateStage;
						}
						
						chosenWord = "";
						currentWordPoints = 0;

						//if mouse released deselects everything
						for (int p = 0; p < B.xDim; p++)
						{
							for (int q = 0; q < B.yDim; q++)
							{
								if (chosenArr[p][q])
								{
									chosenArr[p][q] = false;
								}
							}
						}

					}

					//if its your turn, send the selected word along with the coordinates that are selected
					packet << 0;

					for (int p = 0; p < B.xDim; p++)
					{
						for (int q = 0; q < B.yDim; q++)
						{
							if (chosenArr[p][q])
							{
								packet << p << q;
							}
						}
					}
					

				}
				socketCounter++;
				if (socketCounter == 3) {
					socketCounter = 0;
					socket.send(packet);
				}
				
				socket.receive(packet);
				if (packet >> p2Position.x >> p2Position.y)
				{
					std::cout << p2Position.x << " " << p2Position.y << std::endl;
					if (p2Position.x != -1 && p2Position.y != -1) {
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
					

				}


				if(gameStage == 2)
				{

					if (packet >> updateStage)
					{
						if (updateStage == 1)
						{
							updateStage = 0;
							gameStage++;
							if (gameStage == 3)
							{
								gameStage = 1;
							}
						}
					}

					int test;
					if (packet >> test)
					{

						std::vector<sf::Vector2i>L;
						int x, y;
						while (packet >> x >> y)
						{
							sf::Vector2i chosenCords(x,y);
							L.push_back(chosenCords);
						}

						for (int p = 0; p < B.xDim; p++)
						{
							for (int q = 0; q < B.yDim; q++)
							{
								for (int l = 0; l < L.size(); l++) {
									if (L[l].x == p && L[l].y == q) {
										chosenArr[p][q] = true;
									}
									else {
										chosenArr[p][q] = false;
									}
								}
							}
						}

						
						
				
							
					}
					

				}
				

				
			}//board refresh
		}//board refresh

		std::transform(chosenWord.begin(), chosenWord.end(), chosenWord.begin(),
			[](unsigned char c) { return std::tolower(c); });

		//checks if the selected word is valid
		if (chosenWord.compare("") != 0) {
			for (int p = 0; p < dictionary.size(); p++)
			{
				if (chosenWord.compare(dictionary[p]) == 0)
				{
					std::cout << "test";
					isEnglishWord = true;
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
