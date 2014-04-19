// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// entity.cpp v1.5
// Last modification: Feb-13-2013

#include "entity.h"

//=============================================================================
// constructor
//=============================================================================
Entity::Entity() : Image()
{




    active = true;                  // the entity is active
    health = 0;
	id =0;
	x = 0;
	y = 0;
	startFrame = 0;
	endFrame = 0;




}

//=============================================================================
// Initialize the Entity.
// Pre: *gamePtr = pointer to Game object
//      width = width of Image in pixels  (0 = use full texture width)
//      height = height of Image in pixels (0 = use full texture height)
//      ncols = number of columns in texture (1 to n) (0 same as 1)
//      *textureM = pointer to TextureManager object
// Post: returns true if successful, false if failed
//=============================================================================
bool Entity::initialize(int aid, int ateam, int ahealth, int amaxhealth, int aattack, int adefence, int ax, int ay, int astartFrame, int aendFrame)
{
	id = aid;
	team = ateam;
	maxhealth = amaxhealth;
	attack = aattack;

	defence = adefence;
	x = ax;
	y = ay;
	startFrame = astartFrame;
	endFrame = aendFrame;
  
	return true;
}

//=============================================================================
// activate the entity
//=============================================================================
void Entity::activate()
{
    active = true;
}


//=============================================================================
// ai (artificial intelligence)
// typically called once per frame
// performs ai calculations, ent is passed for interaction
//=============================================================================
void Entity::ai(float frameTime, Entity &ent)
{}
