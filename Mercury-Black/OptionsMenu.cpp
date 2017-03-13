#include "OptionsMenu.h"
#include <iostream>

void OptionsMenu::init(){
	
	hText = sf::Color::Black;
	uText = sf::Color(100, 100, 100, 255);

	title = sf::Text("Options", engine->textureManager.font, OPTION_TITLE_SIZE);
	title.setFillColor(sf::Color::Black);
	title.setOutlineThickness(5);
	title.setOutlineColor(uText);
	title.setOrigin(title.getGlobalBounds().width / 2, title.getGlobalBounds().height / 2);

	musicToggle = Button("Music... Not yet used.", 0, 0, uText, OPTION_FONT_SIZE, &engine->textureManager.font, &doNothing);
	sfxToggle = Button("SFX ", 0, 0, uText, OPTION_FONT_SIZE, &engine->textureManager.font, &doNothing);
	changeBinding = Button("", 0, 0, uText, OPTION_FONT_SIZE, &engine->textureManager.font, &doNothing);
	
	buttons.resize(3);
	buttons = { &musicToggle, &sfxToggle, &changeBinding};

	view = engine->window.getDefaultView();
	engine->window.setView(view);

	title.setPosition(view.getCenter().x, view.getCenter().y - (view.getSize().y / 5));

	musicToggle.text.setPosition(view.getCenter().x - (view.getSize().x / 2) * 1 / 3, title.getPosition().y + (view.getSize().y / 2) * 2 / 3);
	sfxToggle.text.setPosition(view.getCenter().x, title.getPosition().y + (view.getSize().y / 2) * 2 / 3);
	changeBinding.text.setPosition(view.getCenter().x + (view.getSize().x / 2) * 1 / 3, title.getPosition().y + (view.getSize().y / 2) * 2 / 3);
	
}

void OptionsMenu::clean(){

}

void OptionsMenu::handleEvent(){

	sf::Event event;

	if (engine->window.pollEvent(event)) {

		switch (event.type) {

		case sf::Event::MouseMoved:

			for (size_t i = 0; i < buttons.size(); i++) {

				if (buttons[i]->isSelected(engine->window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y))))
					buttons[i]->text.setFillColor(hText);
				else
					buttons[i]->text.setFillColor(uText);

			}
			break;

		case sf::Event::MouseButtonPressed:

			if (event.mouseButton.button == sf::Mouse::Left) {

				for (size_t i = 0; i < buttons.size(); i++) {

					if (buttons[i]->isSelected(engine->window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))))
						buttons[i]->interact(engine);

				}

			}
			break;

		case sf::Event::Closed:
			engine->quit();
			break;

		}

	}
}

void OptionsMenu::update(const float dt){

}

void OptionsMenu::render(const float dt){
	
	for (size_t i = 0; i < buttons.size(); i++)
		engine->window.draw(buttons[i]->text);
	engine->window.draw(title);
}
