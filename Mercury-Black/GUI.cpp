#include "GUI.h"

bool GUI::isSelected(int mouseX, int mouseY) {

	if (text.getGlobalBounds().contains(sf::Vector2f(mouseX, mouseY))) {

		return true;

	}

	return false;
}