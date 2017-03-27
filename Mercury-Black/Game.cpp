#include "Game.h"

#include "EventHandler.h"
#include "System.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "Editor.h"
#include <map>

Game Game::game;

void Game::init() {

	world.textureManager = &engine->textureManager;

	metaballHandler.init(engine->window.getSize(), false);

	createPlayer(&world, 0, 0);
	createTest(&world, 2000, -1500);
	createHeart(&world, 900, 500);
	createWisp(&world, 500, 500, &metaballHandler);
	//createCeilingPlant(&world, 3000, 1000);

	/*Sound insertion code TEMPORARY*/

	sf::Vector2f size(800, 800);
	sf::Vector2f size2(500, 500);
	sf::RectangleShape * rectangle = new sf::RectangleShape(size);
	sf::RectangleShape * rectangle2 = new sf::RectangleShape(size2);
	rectangle->setFillColor(sf::Color::Blue);
	rectangle2->setFillColor(sf::Color::Green);
	eventMap.insertSound(rectangle, &world, "Music/frogs.ogg", 20.0, true);
	eventMap.insertSound(rectangle2, &world, "Music/drank.ogg", 25.0, true);
	rectangle->setPosition(1000, 1500);
	rectangle2->setPosition(1000, 1500);

	/* End of sound code*/

	objectMap = ObjectMap(&engine->textureManager);
	objectMap.load();
	platformMap.load();
	hitboxMap.load();

	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(3);

	waterHandler.load();

}

void Game::clean() {

	metaballHandler.clean();
	eventMap.clean();
	cleanWorld(&world);
	platformMap.clean();
	objectMap.clean();
	waterHandler.clean();
	hitboxMap.clean();

}

void Game::handleEvent() {

	sf::Event event;

	if (engine->window.pollEvent(event)) {

		switch (event.type) {

		case sf::Event::Closed:

			engine->quit();

			break;

		case sf::Event::KeyPressed:

			if (event.key.code == sf::Keyboard::Escape)
				engine->pushState(PauseMenu::instance(engine));

			if (event.key.code == sf::Keyboard::Tab)
				engine->changeState(Editor::instance(engine));

			if (event.key.code == sf::Keyboard::W)
				world.input[PLAYER].jump = true;
			if (event.key.code == sf::Keyboard::S)
				world.input[PLAYER].down = true;
			if (event.key.code == sf::Keyboard::A)
				world.input[PLAYER].left = true;
			if (event.key.code == sf::Keyboard::D)
				world.input[PLAYER].right = true;

			if (event.key.code == sf::Keyboard::L)
				drawPlatforms = !drawPlatforms;

			if (event.key.code == sf::Keyboard::J)
				world.input[PLAYER].attack = true;
			if (event.key.code == sf::Keyboard::K) {
				world.input[PLAYER].special = true;
			}

			if (event.key.code == sf::Keyboard::LShift)
				metaballHandler.sunburst(sf::Vector2f(world.position[0].x, world.position[0].y), 20);

			if (event.key.code == sf::Keyboard::R)
				world.health[0].current = 0;

			break;

		case sf::Event::KeyReleased:

			if (event.key.code == sf::Keyboard::W)
				world.input[PLAYER].jump = false;
			if (event.key.code == sf::Keyboard::S)
				world.input[PLAYER].down = false;
			if (event.key.code == sf::Keyboard::A)
				world.input[PLAYER].left = false;
			if (event.key.code == sf::Keyboard::D)
				world.input[PLAYER].right = false;

			if (event.key.code == sf::Keyboard::J)
				world.input[PLAYER].attack = false;
			if (event.key.code == sf::Keyboard::K) {
				world.input[PLAYER].special = false;
			}

			break;

		}

	}

}

void Game::update(const float dt) {

	if (world.health[0].current <= 0) {
		game.clean();
		game.init();
	}

	aiSystem(&world, dt);
	animationSystem(&world, dt);
	inputSystem(&world);
	gravitySystem(&world);
	shapeCollSystem(&world, &platformMap);
	movementSystem(&world);
	damageSystem(&world, dt, &hitboxMap);

	for (eventMap.eit = eventMap.events.begin(); eventMap.eit != eventMap.events.end(); eventMap.eit++)	
		if (eventMap.eit->second->isTriggered()) 
			eventMap.eit->second->trigger();

	waterHandler.update();
	waterHandler.updateWaves(dt);

	//listener.setPosition(world.position[0].x, world.position[0].y, 0);
	sf::Listener::setPosition(world.position[0].x, 0, world.position[0].y);

	view.setSize(sf::Vector2f(engine->window.getDefaultView().getSize().x * 3.5f, engine->window.getDefaultView().getSize().y * 3.5f));
	view.setCenter(sf::Vector2f(world.position[PLAYER].x, world.position[PLAYER].y - view.getSize().y / 4));
	engine->window.setView(view);

	metaballHandler.update(dt);

	/* Hitbox Temp */
	//rect.setSize(sf::Vector2f(world.sprite[0].sprite.getLocalBounds().width, world.sprite[0].sprite.getLocalBounds().height));
	//rect.setPosition(sf::Vector2f(world.sprite[0].sprite.getGlobalBounds().left, world.sprite[0].sprite.getGlobalBounds().top));

}

void Game::render(const float dt) {

	objectMap.drawSuperBackground(&engine->window);

	objectMap.drawBackground(&engine->window);

	renderSystem(&world, &engine->window);

	metaballHandler.draw(&engine->window, &view);

	waterHandler.draw(&engine->window);

	engine->window.draw(rect);

	objectMap.drawForeground(&engine->window);

	for (eventMap.eit = eventMap.events.begin(); eventMap.eit != eventMap.events.end(); eventMap.eit++)
	{
		engine->window.draw(*eventMap.eit->second->eventArea);
	}

	if (drawPlatforms)
		for (pit = platformMap.map.begin(); pit != platformMap.map.end(); pit++)
			engine->window.draw(*(pit->second));

}