// Programming 2D Games
// Copyright (c) 2011,2012 by: 
// Charles Kelly
// entity.h v1.4
// Last modification: Feb-12-2013

#ifndef _ENTITY_H               // Prevent multiple definitions if this 
#define _ENTITY_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class Entity;

#include "image.h"
#include "input.h"
#include "game.h"

namespace entityNS
{

}

class Entity : public Image
{
    // Entity properties
  protected:

    bool    active;         // only active entities may collide
	
	int id;
	int team;
	int health;
	int maxhealth;
	int attack;
	int defence;

	int x; // row
	int y; // column
	int startFrame; // starting frame inside spritesheet
	int endFrame; // end frame inside spritesheet
	int currentFrame; // current visible frame

	int animations[16][16]; 

	// max animation framelength: 16
  /// animations:
	// 0: standing still
	// 1: standing active
	// 2: walking northeast
	// 3: walking southeast
	// 4: walking southwest
	// 5: walking northwest
	// 6: attacking northeast
	// 7: attacking southeast
	// 8: attacking southwest
	// 9: attacking northwest
	// 10: death animation
	// 11: defence/sleep animation
	// 12: special animation
	// 13: reserved
	// 14: reserved
	// 15: reserved
	// 16: reserved


  public:
    // Constructor
    Entity();
    // Destructor
    virtual ~Entity() {}

    ////////////////////////////////////////
    //           Get functions            //
    ////////////////////////////////////////
	virtual int getCurrentFrame() const     {return currentFrame;}
	virtual void setCurrentFrame(int a)     {currentFrame = a;}

	virtual int getEndFrame() const     {return endFrame;}
	virtual void setEndFrame(int a)     {endFrame = a;}

	virtual int getStartFrame() const     {return startFrame;}
	virtual void setStartFrame(int a)     {startFrame = a;}

	virtual int getDefence() const     {return defence;}
	virtual void setDefence(int a)     {defence = a;}

	virtual int getAttack() const     {return attack;}
	virtual void setAttack(int a)     {attack = a;}

	virtual int getTeam() const     {return team;}
	virtual void setTeam(int a)     {team = a;}

	virtual int getID() const     {return id;}
	virtual void setID(int a)     {id = a;}

    virtual int getX() const     {return x;}
	virtual void  setX(int a)         {x = a;}

	virtual int getY() const     {return y;}
	virtual void  setY(int a)         {y = a;}

    // Return health;
    virtual int getHealth()         const {return health;}
	virtual void setHealth(int a)	{health = a;}



    ////////////////////////////////////////
    //           Set functions            //
    ////////////////////////////////////////

    // Set active.
    virtual void  setActive(bool a)         {active = a;}


    ////////////////////////////////////////
    //         Other functions            //
    ////////////////////////////////////////



    virtual bool initialize(int id, int team, int health, int maxhealth, int attack, int defence, int x, int y, int startFrame, int endFrame);
    // Activate Entity.
    virtual void activate();

    // Empty ai function to allow Entity objects to be instantiated.
    virtual void ai(float frameTime, Entity &ent);




};

#endif
