#include "AIScript.h"
#include <cmath>

void scriptTest(World * world, int entityID, float dt) {

	Velocity * v;
	Sprite * s;
	ScriptParameters * sp;

	v = &(world->velocity[entityID]);
	s = &(world->sprite[entityID]);
	sp = &(world->scriptParameters[entityID]);

	scriptFollow(world, entityID, world->position[0].x, world->position[0].y);
	scriptAttack(world, entityID, world->position[0].x, world->position[0].y);

	if (v->x != 0)
		s->animationManager.changeAnimation("sheathedRun");
	else {
		if (sp->currentState == ATTACK_STATE)
			s->animationManager.changeAnimation("idleAttack");
		else
			s->animationManager.changeAnimation("idleUnsheathed");
	}

	if (v->x < 0)
		s->sprite.setTextureRect(sf::IntRect(s->sprite.getLocalBounds().width, 0, -s->sprite.getLocalBounds().width, s->sprite.getLocalBounds().height));
	if (v->x > 0)
		s->sprite.setTextureRect(sf::IntRect(0, 0, s->sprite.getLocalBounds().width, s->sprite.getLocalBounds().height));

	s->sprite.setTexture(*s->animationManager.getCurrentTexture());
	s->sprite.setOrigin(sf::Vector2f(s->sprite.getLocalBounds().width / 2, s->sprite.getLocalBounds().height));

	/* Allow Animation Changes If Current Animation Has Ended */
	if (s->animationManager.updateAnimation(dt) == 1)
		sp->currentState = NO_STATE;

}

void scriptFollow(World * world, int entityID, float x, float y) {

	if (std::fabs(world->position[entityID].x - x) <=
		world->scriptParameters[entityID].followDistMax &&

		std::fabs(world->position[entityID].x - x) >=
		world->scriptParameters[entityID].followDistMin){
			
			if (world->position[entityID].x - x < 0) {
				world->input[entityID].right = true;
			}
			
			else {
				world->input[entityID].left = true;
			}
	}

	else {
		world->input[entityID].left = false;
		world->input[entityID].right = false;
	}
	
}

void scriptAttack(World* world, int entityID, float x, float y) {

	if (std::fabs(world->position[entityID].x - x) <=
		world->scriptParameters[entityID].attackRangeMax &&

		std::fabs(world->position[entityID].x - x) >=
		world->scriptParameters[entityID].attackRangeMin) {
			
		world->scriptParameters[entityID].currentState = ATTACK_STATE;
			//world->input[entityID].attack = true;
	}

	else {
		//world->input[entityID].attack = false;
	}
}

void scriptRetreat(World * world, int entityID, float x, float y) {

	if (std::fabs(world->position[entityID].x - x) <=
		world->scriptParameters[entityID].followDistMin) {

		if (world->position[entityID].x - x < 0) {
			world->input[entityID].left = true;
		}

		else {
			world->input[entityID].right = true;
		}
	}

	else {
		world->input[entityID].left = false;
		world->input[entityID].right = false;
	}
}

void scriptPlayer(World *world, float dt) {

	Input * i;
	Sprite * s;
	Velocity * v;
	ScriptParameters * sp;
	
	s = &(world->sprite[0]);
	v = &(world->velocity[0]);
	i = &(world->input[0]);
	sp = &(world->scriptParameters[0]);
	
	/* On Ground */
	if (v->onGround) {
		/* Moving */
		if (v->x != 0) {
			/* Jump */
			if (i->jump) {
				s->animationManager.changeAnimation("jump");
				sp->currentState = JUMP_STATE;
			}
			/* Attack */
			//else if (i->attack) {
				//TODO
			//}
			/* No Input */
			else {
				s->animationManager.changeAnimation("sheathedRun");
			}
		}
		/* Not Moving*/
		else {
			/* Jump */
			if (i->jump) {
				s->animationManager.changeAnimation("jump");
				sp->currentState = JUMP_STATE;
			}
			/* Attack */
			else if (i->attack) {
				s->animationManager.changeAnimation("idleAttack");
				sp->currentState = ATTACK_STATE;
			}
			/* No Input */
			else {
				if (sp->currentState == NO_STATE) {
					s->animationManager.changeAnimation("idleUnsheathed");
				}
			}
		}
	}
	/* In Air */
	else {
		/* Moving */
		if (v->x != 0) {
			/* Attack */
			//if (i->attack) {
				//TODO
			//}
			/* No Input */
			//else {
				if (sp->currentState == NO_STATE) {
					s->animationManager.changeAnimation("inAir");
				}
			//}
		}
		/* Not Moving  (Might Be Able To Remove)*/
		else {
			/* Attack */
			//if (i->attack) {
				//TODO
			//}
			/* No Input */
			//else {
				if (sp->currentState == NO_STATE) {
					s->animationManager.changeAnimation("inAir");
				}
			//}
		}
	}
	
	if (v->x < 0)
		s->sprite.setTextureRect(sf::IntRect(s->sprite.getLocalBounds().width, 0, -s->sprite.getLocalBounds().width, s->sprite.getLocalBounds().height));
	if (v->x > 0)
		s->sprite.setTextureRect(sf::IntRect(0, 0, s->sprite.getLocalBounds().width, s->sprite.getLocalBounds().height));

	s->sprite.setTexture(*s->animationManager.getCurrentTexture());
	s->sprite.setOrigin(sf::Vector2f(s->sprite.getLocalBounds().width / 2, s->sprite.getLocalBounds().height));

	/* Allow Animation Changes If Current Animation Has Ended */
	if (s->animationManager.updateAnimation(dt) == 1)
		sp->currentState = NO_STATE;

}

void scriptPlant(World * world, int entityID, float dt) {
	
	Sprite *s;
	ScriptParameters *sP;

	s = &(world->sprite[entityID]);
	sP = &(world->scriptParameters[entityID]);
	
	if (sP->currentState == NO_STATE) {
		scriptAttack(world, entityID, world->position[0].x, world->position[0].y);
		
		if (sP->currentState == ATTACK_STATE)
			s->animationManager.changeAnimation("tripleAttack");
		else {
			if (sP->currentState == NO_STATE)
				s->animationManager.changeAnimation("idle");
		}
	}
	
	else if (sP->currentState == NOT_SPAWNED_STATE) {
		scriptSpawn(world, entityID);
		if (sP->currentState == SPAWN_STATE)
			s->animationManager.changeAnimation("spawn");
		else
			s->animationManager.changeAnimation("notSpawn");
	}

	s->sprite.setTexture(*s->animationManager.getCurrentTexture());
	s->sprite.setOrigin(sf::Vector2f(s->sprite.getLocalBounds().width / 2, s->sprite.getLocalBounds().height));
	
	if (s->animationManager.updateAnimation(dt) == 1)
		sP->currentState = NO_STATE;
}

void scriptSpawn(World *world, int entityID) {
	ScriptParameters * sP;

	sP = &world->scriptParameters[entityID];

	if (sP->spawnDistance >= abs(world->position[entityID].x - world->position[0].x) &&
		sP->spawnDistance >= abs(world->position[entityID].y - world->position[0].y)) {
		sP->currentState = SPAWN_STATE;
	}

	return;
}