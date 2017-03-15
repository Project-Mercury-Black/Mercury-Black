#include "ToolBox.h"

#define DEFAULT_TEXT_SIZE 15

ToolBox::ToolBox(GameEngine * engine){

	this->engine = engine;

	mode = PLATFORM;
	tool = PLACE;

	/* View */

	toolBoxView.setViewport(sf::FloatRect(0, 0, 0.2f, 1));
	toolBoxView.setSize(toolBoxView.getViewport().width * engine->window.getSize().x, toolBoxView.getViewport().height * engine->window.getSize().y);

	/* Text */

	modeText = sf::Text("", engine->textureManager.slideFont, DEFAULT_TEXT_SIZE);
	toolText = sf::Text("", engine->textureManager.slideFont, DEFAULT_TEXT_SIZE);
	morphText1 = sf::Text("", engine->textureManager.slideFont, DEFAULT_TEXT_SIZE);
	morphText2 = sf::Text("", engine->textureManager.slideFont, DEFAULT_TEXT_SIZE);
	morphText3 = sf::Text("", engine->textureManager.slideFont, DEFAULT_TEXT_SIZE);

	modeText.setFillColor(sf::Color::Black);
	toolText.setFillColor(sf::Color::Black);
	morphText1.setFillColor(sf::Color::Black);
	morphText2.setFillColor(sf::Color::Black);
	morphText3.setFillColor(sf::Color::Black);

	modeText.setPosition(toolBoxView.getCenter().x - toolBoxView.getSize().x / 2.0f + 20, toolBoxView.getCenter().y - toolBoxView.getSize().y / 2.0f + 20);
	toolText.setPosition(toolBoxView.getCenter().x - toolBoxView.getSize().x / 2.0f + 20, toolBoxView.getCenter().y - toolBoxView.getSize().y / 2.0f + 70);
	morphText1.setPosition(toolBoxView.getCenter().x - toolBoxView.getSize().x / 2.0f + 20, toolBoxView.getCenter().y - toolBoxView.getSize().y / 2.0f + 170);
	morphText2.setPosition(toolBoxView.getCenter().x - toolBoxView.getSize().x / 2.0f + 20, toolBoxView.getCenter().y - toolBoxView.getSize().y / 2.0f + 220);
	morphText3.setPosition(toolBoxView.getCenter().x - toolBoxView.getSize().x / 2.0f + 20, toolBoxView.getCenter().y - toolBoxView.getSize().y / 2.0f + 120);

	/* Tool Box */

	toolBox.setFillColor(sf::Color::White);
	toolBox.setSize(toolBoxView.getSize());
	toolBox.setPosition(toolBoxView.getCenter().x - toolBoxView.getSize().x / 2.0f, toolBoxView.getCenter().y - toolBoxView.getSize().y / 2);
	
	toolBarShade.setFillColor(sf::Color(200, 200, 200, 255));
	toolBarShade.setSize(sf::Vector2f(30, engine->window.getSize().y));
	toolBarShade.setPosition(toolBox.getGlobalBounds().left + toolBox.getLocalBounds().width - 30, toolBox.getGlobalBounds().top);

	/* Buttons */

	boxTool = IconButton(0, 0, engine->textureManager.getTexture("boxTool"), &doNothing);
	freeTool = IconButton(0, 0, engine->textureManager.getTexture("freeTool"), &doNothing);
	groundTool = IconButton(0, 0, engine->textureManager.getTexture("groundTool"), &doNothing);
	eventTool = IconButton(0, 0, engine->textureManager.getTexture("eventTool"), &doNothing);
	objectTool = IconButton(0, 0, engine->textureManager.getTexture("objectTool"), &doNothing);

	boxTool.sprite.setPosition(toolBox.getPosition().x + toolBox.getLocalBounds().width - 30, toolBox.getPosition().y);
	freeTool.sprite.setPosition(toolBox.getPosition().x + toolBox.getLocalBounds().width - 30, toolBox.getPosition().y + 30);
	groundTool.sprite.setPosition(toolBox.getPosition().x + toolBox.getLocalBounds().width - 30, toolBox.getPosition().y + 60);
	eventTool.sprite.setPosition(toolBox.getPosition().x + toolBox.getLocalBounds().width - 30, toolBox.getPosition().y + 120);
	objectTool.sprite.setPosition(toolBox.getPosition().x + toolBox.getLocalBounds().width - 30, toolBox.getPosition().y + 180);

	//guiHandler.buttons.resize(5);
	//guiHandler.buttons = { &boxTool, &freeTool, &groundTool, &eventTool, &objectTool };

}

void ToolBox::highlightButtons(sf::Vector2i position) {

	engine->window.setView(toolBoxView);
	//guiHandler.isSelected(engine->window.mapPixelToCoords(position));
	boxTool.isSelected(engine->window.mapPixelToCoords(position));
	freeTool.isSelected(engine->window.mapPixelToCoords(position));
	groundTool.isSelected(engine->window.mapPixelToCoords(position));
	eventTool.isSelected(engine->window.mapPixelToCoords(position));
	objectTool.isSelected(engine->window.mapPixelToCoords(position));

}

void ToolBox::clickButtons(sf::Vector2i position) {

	engine->window.setView(toolBoxView);

	if (boxTool.isSelected(engine->window.mapPixelToCoords(position))) {
		mode = PLATFORM;
		tool = BOX_PLACE;
		modeText.setString("Mode: Platform");
		toolText.setString("Tool: Box Place");
	}

	else if (freeTool.isSelected(engine->window.mapPixelToCoords(position))) {
		mode = PLATFORM;
		tool = PLACE;
		modeText.setString("Mode: Platform");
		toolText.setString("Tool: Free Place");
	}

	else if (groundTool.isSelected(engine->window.mapPixelToCoords(position))) {
		mode = PLATFORM;
		tool = GROUND_PLACE;
		modeText.setString("Mode: Platform");
		toolText.setString("Tool: Ground Place");
	}

	else if (objectTool.isSelected(engine->window.mapPixelToCoords(position))) {
		mode = OBJECT;
		tool = PLACE;
		modeText.setString("Mode: Object");
		toolText.setString("Tool: Place");
	}

	else if (eventTool.isSelected(engine->window.mapPixelToCoords(position))) {
		mode = EVENT;
		tool = PLACE;
		modeText.setString("Mode: Event");
		toolText.setString("Tool: Place");
	}

}

void ToolBox::update() {

}

void ToolBox::render() {

	engine->window.setView(toolBoxView);

	engine->window.draw(toolBox);
	engine->window.draw(modeText);
	engine->window.draw(toolText);
	engine->window.draw(morphText1);
	engine->window.draw(morphText2);
	engine->window.draw(morphText3);
	engine->window.draw(toolBarShade);
	//guiHandler.draw(&engine->window);
	engine->window.draw(boxTool.sprite);
	engine->window.draw(freeTool.sprite);
	engine->window.draw(groundTool.sprite);
	engine->window.draw(objectTool.sprite);
	engine->window.draw(eventTool.sprite);

}

void ToolBox::nextMode() {

	if (mode == PLATFORM) {
		mode = OBJECT;
		tool = PLACE;
		modeText.setString("Mode: Object");
		morphText1.setString("Points: N\\A");
	}
	else if (mode == OBJECT) {
		mode = EVENT;
		modeText.setString("Mode: Event");
		morphText1.setString("Points: N\\A");
	}
	else {
		mode = PLATFORM;
		modeText.setString("Mode: Platform");
		morphText1.setString("Points: N\\A");
	}

}

void ToolBox::nextTool() {

	if (mode == PLATFORM) {

		if (tool == PLACE) {
			tool = BOX_PLACE;
			toolText.setString("Tool: Box");
		}
		if (tool == BOX_PLACE) {
			tool = GROUND_PLACE;
			toolText.setString("Tool: Ground");
		}
		else {
			tool = PLACE;
			toolText.setString("Tool: Place");
		}

	}

	else if (mode == OBJECT) {

	}

	else if (mode == EVENT) {

	}

}

Mode ToolBox::getMode() {
	return mode;
}

Tool ToolBox::getTool() {
	return tool;
}