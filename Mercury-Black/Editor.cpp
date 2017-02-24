#include "Editor.h"

#include <iostream>
#include <fstream>
#include <string>

#include "MainMenu.h"
#include "Game.h"

Editor Editor::editor;

void Editor::init() {
	
	selector = Selector();

	showLines = true;
	mode = POINT;
	tool = PLACE;

	modeText = sf::Text("Mode: Point", engine->textureManager.font, 60);
	modeText.setFillColor(sf::Color::Black);

	toolText = sf::Text("Tool: Place", engine->textureManager.font, 60);
	toolText.setFillColor(sf::Color::Black);

	textureText = sf::Text("", engine->textureManager.font, 60);
	textureText.setFillColor(sf::Color::Black);

	collisionMap.load();
	collisionMap.updateVerticies();

	objectMap = ObjectMap(&engine->textureManager);
	objectMap.load();
	textureText.setString(objectMap.object.textureName);

	zoom = 2;
	view.setSize(sf::Vector2f(engine->window.getDefaultView().getSize().x * 2, engine->window.getDefaultView().getSize().y * 2));

}

void Editor::clean() {

	collisionMap.clean();
	objectMap.clean();
	platformPoints.clean();
	platformMap.clean();

}

void Editor::handleEvent() {

	sf::Event event;

	engine->window.pollEvent(event);

	switch (event.type) {

	case sf::Event::Closed:
		
		engine->quit();
		
		break;

	case sf::Event::MouseWheelScrolled:

		if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
			zoom += event.mouseWheelScroll.delta;
		}

		if (zoom < 1)
			zoom = 1;

		view.setSize(sf::Vector2f(engine->window.getDefaultView().getSize().x * zoom, engine->window.getDefaultView().getSize().y * zoom));

		break;

	case sf::Event::MouseButtonPressed:

		if (event.mouseButton.button == sf::Mouse::Left) {
			
			if(mode == POINT)
				collisionMap.insert(engine->window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
			if (mode == OBJECT)
				objectMap.insert(engine->window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
			if (mode == PLATFORM)
				if (tool == BOX) 
					corner1 = engine->window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				else
					platformPoints.insert(engine->window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
		}

		if (event.mouseButton.button == sf::Mouse::Right) {

			if (mode == POINT) {
				collisionMap.selected = collisionMap.findClosest(engine->window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));

				if (collisionMap.selected != collisionMap.map.end()) {
					selector.rect.setSize(sf::Vector2f(15, 15));
					selector.rect.setOrigin(selector.rect.getSize() * 0.5f);
					selector.rect.setPosition(collisionMap.selected->second->position);
					selector.rect.setOutlineColor(sf::Color::Blue);
				}
			}

			if (mode == OBJECT) {
				
				objectMap.selected = objectMap.findClosest(engine->window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));

				if (objectMap.selected != objectMap.map.end()) {
					selector.rect.setSize(sf::Vector2f(objectMap.selected->second->sprite.getLocalBounds().width, objectMap.selected->second->sprite.getLocalBounds().height));
					selector.rect.setOrigin(selector.rect.getSize() * 0.5f);
					selector.rect.setPosition(objectMap.selected->second->position.x, objectMap.selected->second->position.y);
					selector.rect.setOutlineColor(sf::Color::Blue);
				}

				else
					selector.rect.setOutlineColor(sf::Color::Transparent);

			}

		}

		break;

	case sf::Event::MouseButtonReleased:

		if (event.mouseButton.button == sf::Mouse::Left) {

			if (mode == PLATFORM) {
				if (tool == BOX) {
					corner2 = engine->window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					platformMap.insertBox(corner1, corner2);
				}
			}

		}

		break;

	case sf::Event::KeyPressed:
		
		if (event.key.code == sf::Keyboard::Escape)
			engine->changeState(MainMenu::instance(engine));

		if (event.key.code == sf::Keyboard::Tab)
			engine->changeState(Game::instance(engine));

		if (event.key.code == sf::Keyboard::Delete) {
			
			if (mode == POINT)
				collisionMap.remove();
			if (mode == OBJECT)
				objectMap.remove();
			if (mode == PLATFORM)
				platformPoints.remove();

			selector.rect.setOutlineColor(sf::Color::Transparent);

		}

		if (event.key.code == sf::Keyboard::Return) {

			if (mode == PLATFORM) {
				platformMap.insert(&platformPoints.lines);
				platformPoints.clean();
			}

		}

		if (event.key.code == sf::Keyboard::A)
			doLeft = true;
		if (event.key.code == sf::Keyboard::D)
			doRight = true;
		if (event.key.code == sf::Keyboard::W)
			doUp = true;
		if (event.key.code == sf::Keyboard::S)
			doDown = true;

		if (event.key.code == sf::Keyboard::L)
			showLines = !showLines;

		if (event.key.code == sf::Keyboard::J) {
			collisionMap.save();
			objectMap.save();
		}
		if (event.key.code == sf::Keyboard::K) {
			collisionMap.load();
			objectMap.load();
		}

		if (event.key.code == sf::Keyboard::M)
			rotateMode();
		if (event.key.code == sf::Keyboard::N)
			rotateTool();

		if (mode == OBJECT)
			if (event.key.code == sf::Keyboard::T) {
				objectMap.changeObject();
				textureText.setString(objectMap.object.textureName);
			}

		break;

	case sf::Event::KeyReleased:
		if (event.key.code == sf::Keyboard::A)
			doLeft = false;
		if (event.key.code == sf::Keyboard::D)
			doRight = false;
		if (event.key.code == sf::Keyboard::W)
			doUp = false;
		if (event.key.code == sf::Keyboard::S)
			doDown = false;
		break;
	}

}

void Editor::update(const float dt) {

	if (doLeft == true)
		viewVelX = -15.0f;
	else if (doRight == true)
		viewVelX = 15.0f;
	else
		viewVelX = 0.0f;

	if (doUp == true)
		viewVelY = -15.0f;
	else if (doDown == true)
		viewVelY = 15.0f;
	else
		viewVelY = 0.0f;

	engine->window.setView(view);

	modeText.setPosition(view.getCenter().x - view.getSize().x / 2 + 50, view.getCenter().y - view.getSize().y / 2 + 50);
	toolText.setPosition(view.getCenter().x - view.getSize().x / 2 + 50, view.getCenter().y - view.getSize().y / 2 + 200);
	textureText.setPosition(view.getCenter().x - view.getSize().x / 2 + 50, view.getCenter().y - view.getSize().y / 2 + 350);

	view.move(viewVelX, viewVelY);

}

void Editor::render(const float dt) {

	objectMap.draw(&engine->window);

	if (showLines)
		engine->window.draw(collisionMap.lines);

	platformPoints.draw(&engine->window);
	platformMap.draw(&engine->window);

	engine->window.draw(selector.rect);

	engine->window.draw(modeText);
	engine->window.draw(toolText);
	engine->window.draw(textureText);
}

void Editor::rotateMode() {
	
	if (mode == POINT) {
		mode = OBJECT;
		modeText.setString("Mode: Object");
	}
	else if (mode == OBJECT) {
		mode = PLATFORM;
		modeText.setString("Mode: Platform");
	}
	else if (mode == PLATFORM) {
		mode = POINT;
		modeText.setString("Mode: Point");
	}
	else {
		mode = POINT;
		modeText.setString("Mode: Point");
	}

}

void Editor::rotateTool() {

	if (tool == PLACE) {
		tool = DELETE;
		toolText.setString("Tool: Delete");
	}
	else if (tool == DELETE) {
		tool = MOVE;
		toolText.setString("Tool: Move");
	}
	else if (tool == MOVE) {
		tool = BOX;
		toolText.setString("Tool: Box");
	}
	else {
		tool = PLACE;
		toolText.setString("Tool: Place");
	}

}

Selector::Selector() {

	rect.setOutlineColor(sf::Color::Transparent);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(5);

}