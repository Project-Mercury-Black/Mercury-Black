#ifndef AISCRIPT_H
#define AISCRIPT_H

#include "Entity.h"
#include <SFML/Graphics.hpp>

void scriptPlayer(World *world, float dt);
void scriptPlant(World *world, int entityID, float dt);

void scriptFollow(World * world, int entityID, sf::Vector2f position);


#endif