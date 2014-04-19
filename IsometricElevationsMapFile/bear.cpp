// Programming 2D Games
// Copyright (c) 2011,2012 by: 
// Charles Kelly
// butterfly.cpp v2.0 X and Y scrolling


#include "bear.h"
using namespace bearNS;

//=============================================================================
// default constructor
//=============================================================================
Bear::Bear() : Entity()
{



}

//=============================================================================
// Initialize the Butterfly.
// Post: returns true if successful, false if failed
//=============================================================================
bool Bear::initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM)
{
	return true;
   // return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Bear::update(float frameTime)
{

    Entity::update(frameTime);



}

//=============================================================================
// Draw Image using color as filter. Default color is WHITE.
//=============================================================================
//void Butterfly::draw(COLOR_ARGB color)
//{
    //int flyingFrame = getCurrentFrame();
    //setCurrentFrame(butterflyNS::FLYING_BODY);
    //Image::draw(color);
    //setCurrentFrame(flyingFrame);
//    Image::draw(color);
//}