// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// butterfly.h v1.0

#ifndef _BEAR_H            // Prevent multiple definitions if this 
#define _BEAR_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace bearNS
{

}

// inherits from Entity class
class Bear : public Entity
{
private:


public:
    // constructor
    Bear();

    // inherited member functions
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    void update(float frameTime);
    void repair();

    // new functions

    // move butterfly out of collision
    //void toOldX() { spriteData.x = oldX; }
    //void toOldY() { spriteData.y = oldY; }
};
#endif

