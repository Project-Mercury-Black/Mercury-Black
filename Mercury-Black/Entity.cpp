#include "Entity.h"

void cleanWorld(World * world) {

	for (int entityID = 0; entityID < MAX_ENTITIES; entityID++) {

		world->mask[entityID] = EMPTY;

	}

}

int createEntity(World * world){

	int entityID;
	
	for (entityID = 0; entityID < MAX_ENTITIES; entityID++){

		if (world->mask[entityID] == EMPTY)
			return entityID;

	}

	printf("No More Entity Space, Overriding last entity in array...\n");

	return MAX_ENTITIES;

}

void destroyEntity(World * world, int entityID) {

	printf("Destroying Entity: %s With ID: %d\n", world->name[entityID].name.c_str(), entityID);

	world->mask[entityID] = EMPTY;

}

int createPlayer(World * world, float x, float y) {

	int entityID = createEntity(world);

	world->mask[entityID] = NAME | INPUT | POSITION | VELOCITY | SPRITE | COLLISION | GRAVITY;

	world->name[entityID].name = "Player";

	world->position[entityID].x = x;
	world->position[entityID].y = y;

	world->velocity[entityID].x = 0.0f;
	world->velocity[entityID].y = 0.0f;
	world->velocity[entityID].speed = 10.0f;
	world->velocity[entityID].canJump = false;
	world->velocity[entityID].onGround = false;

	world->gravity[entityID].weight = 1.0f;

	Animation * runAnimation = new Animation(0.125f);
	runAnimation->addFrame(world->textureManager->getTexture("player_run_1"));
	runAnimation->addFrame(world->textureManager->getTexture("player_run_2"));
	runAnimation->addFrame(world->textureManager->getTexture("player_run_3"));
	runAnimation->addFrame(world->textureManager->getTexture("player_run_4"));
	runAnimation->addFrame(world->textureManager->getTexture("player_run_5"));
	runAnimation->addFrame(world->textureManager->getTexture("player_run_6"));

	world->sprite[entityID].animationManager.addAnimation(runAnimation, "player_run");
	
	world->sprite[entityID].sprite.setOrigin(sf::Vector2f(world->sprite[entityID].sprite.getLocalBounds().width / 2, world->sprite[entityID].sprite.getLocalBounds().height));

	return entityID;

}