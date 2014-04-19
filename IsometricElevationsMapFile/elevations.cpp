
#include "elevations.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string> // this should be already included in <sstream>

using namespace std;


using namespace elevationsNS;






unit createUnit(int x, int y, int team, unitData data)
{
	unit newUnit;
	newUnit.x = x;
	newUnit.y = y;
	newUnit.id = data.id;
	newUnit.team = team;
	newUnit.attack = data.baseAttack;
	newUnit.defence = data.baseDefence;
	newUnit.range = data.baseRange;
	newUnit.currentFrame = data.startFrame;
	newUnit.health = data.maxhealth;
	newUnit.currentDirection = 5; //facing southwest
	newUnit.currentAction = 0; //standing still
	newUnit.movement = data.moveperturn;
	newUnit.animationCount = 0;

	return newUnit;
}


tile createTile(int x, int y, int currentLandFrame, int currentResourceFrame, int currentUnitFrame)
{
	tile newTile;
	newTile.x = x;
	newTile.y = y;
	
	newTile.currentLandFrame = currentLandFrame;
	newTile.currentResourceFrame = currentResourceFrame;
	newTile.currentUnitFrame = currentUnitFrame;
	newTile.currentHealthBarFrame = 0;
	newTile.currentUnitCounterFrame = 0;
	newTile.currentSelectFrame = 0;

	return newTile;
}

columns createColumn()
{
	columns newColumn;
	return newColumn;
}

int calculateHealthBar(int currenthealth, int maxhealth)
{
	int returnvalue = 0;
	float healthbarstat = (float) currenthealth / (float) maxhealth;

	if(healthbarstat > 0 && healthbarstat < .05)
	{
		returnvalue = 13;
	}

	else if(healthbarstat >= .5 && healthbarstat < .1) {returnvalue = 12;}
	else if(healthbarstat >= .1 && healthbarstat < .2){returnvalue = 11;}
	else if(healthbarstat >= .2 && healthbarstat < .3){returnvalue = 10;}
	else if(healthbarstat >= .3 && healthbarstat < .4){returnvalue = 9;}
	else if(healthbarstat >= .4 && healthbarstat < .5){returnvalue = 8;}
	else if(healthbarstat >= .5 && healthbarstat < .6){returnvalue = 7;}
	else if(healthbarstat >= .6 && healthbarstat < .7){returnvalue = 6;}
	else if(healthbarstat >= .7 && healthbarstat < .8){returnvalue = 5;}
	else if(healthbarstat >= .8 && healthbarstat < .9){returnvalue = 4;}
	else if(healthbarstat >= .9 && healthbarstat < .95){returnvalue = 3;}
	else if(healthbarstat >= .95 && healthbarstat < 1){returnvalue = 2;}
	else if(healthbarstat == 1){returnvalue = 1;}

	return returnvalue; // TODO: CHANGE ORDER OF IF STATEMENTS TO START WITH 100% health
}


//=============================================================================
// Constructor
//=============================================================================
Elevations::Elevations()
{
    for(int row=0; row<MAP_SIZE; row++)     // clear maps
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            tileMap[row][col]=0;
            heightMap[row][col]=0;
            objectMap[row][col]=0;   // 0=empty, 1=Tree0, 2=Tree1
			selectMap[row][col]=0; 
			unitMap[row][col]=0;
        }
    }

	alignmentTimer = 0;
    fontCK      = new Text();   // sprite based font
    dxFont14    = new TextDX(); // DirectX fonts
    dxFont24    = new TextDX();
    dxFont48    = new TextDX();
    dxFont48B   = new TextDX(); // bold
    dxFont96    = new TextDX();
    dxFont24F   = new TextDX();
    spriteText  = false;        // true for sprite text, false for DirectX text
    red=0, green=0, blue=0;
    fallingY = FALLING_Y;
    angle = 0;
    pointSize = 6;
    chTimer = 0;
    ch = 0;
    st[1] = '\0';
}

//=============================================================================
// Destructor
//=============================================================================
Elevations::~Elevations()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================


void Elevations::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    std::string str;

    // map textures
    if (!textures.initialize(graphics,TEXTURES_CUSTOM))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures"));

    // object textures
    if (!textures2.initialize(graphics,TEXTURES2_CUSTOM))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures2"));

	//muh_textures
	 if (!textures3.initialize(graphics,TEXTURES3_CUSTOM))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures3"));
	
	 //muh_textures
	 if (!textures4.initialize(graphics,TEXTURES4_CUSTOM))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures4"));
	
	 //muh_textures
	 if (!textures5.initialize(graphics,TEXTURES5_CUSTOM))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures5"));

	 //muh_textures
	 if (!textures6.initialize(graphics,TEXTURES6_CUSTOM))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures6"));


	     // init text
    if (!fontCK->initialize(graphics,FONT_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing CKfont"));

    // initialize DirectX fonts
    // 14 point Arial
    if(dxFont14->initialize(graphics, 14, false, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 24 point Arial
    if(dxFont24->initialize(graphics, 24, false, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 48 point Arial
    if(dxFont48->initialize(graphics, 48, false, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 48 point Arial Bold
    if(dxFont48B->initialize(graphics, 48, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 96 point Arial
    if(dxFont96->initialize(graphics, 96, false, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    // 24 point Fixed Pitch Courier New
    if(dxFont24F->initialize(graphics, 24, true, false, "Courier New") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));


    // load map data
    try{
        std::ifstream mapData("map1.txt");
        if(!mapData)                            // if file error
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error loading map data"));
        std::getline(mapData,str);              // skip 1st comment line
        for(int row=0; row<MAP_SIZE; row++)
            for(int col=0; col<MAP_SIZE; col++)
                mapData >> tileMap[row][col];   // read Tile map
        std::getline(mapData,str);
        std::getline(mapData,str);              // skip 2nd comment line
        for(int row=0; row<MAP_SIZE; row++)
            for(int col=0; col<MAP_SIZE; col++)
                mapData >> heightMap[row][col]; // read Height map
        std::getline(mapData,str);
        std::getline(mapData,str);              // skip 3rd comment line
        for(int row=0; row<MAP_SIZE; row++)
            for(int col=0; col<MAP_SIZE; col++)
                mapData >> objectMap[row][col]; // read Object map
		std::getline(mapData,str);
        std::getline(mapData,str);              // skip 4th comment line
        for(int row=0; row<MAP_SIZE; row++)
            for(int col=0; col<MAP_SIZE; col++)
                mapData >> selectMap[row][col]; // read Selection map
		std::getline(mapData,str);

    }
    catch(...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error loading map data"));
    }


	for(int row=0; row<MAP_SIZE; row++)
	{
		worldMap.row.push_back(createColumn());
		for(int col=0; col<MAP_SIZE; col++)
		{
			worldMap.row[row].column.push_back(createTile(row, col, tileMap[row][col], objectMap[row][col], unitMap[row][col])); // create tiles from mapData
		}
	}
		

	//tile createTile(int x, int y, int currentLandFrame, int currentResourceFrame, int currentUnitFrame, int currentUnitCounterFrame)

    // map tile image
    mapTile.initialize(graphics,TEXTURE_SIZE,TEXTURE_SIZE,TEXTURE_COLS,&textures);
    mapTile.setFrames(0, 0);
    mapTile.setCurrentFrame(0);

    // objectResource image
    objectResource.initialize(graphics,TEXTURE2_SIZE,TEXTURE2_SIZE,TEXTURE2_COLS,&textures2);
    objectResource.setFrames(0, 0);
    objectResource.setCurrentFrame(0);

	// select image
    selection.initialize(graphics,TEXTURE3_SIZE,TEXTURE3_SIZE,TEXTURE3_COLS,&textures3);
    selection.setFrames(0, 0);
    selection.setCurrentFrame(0);

	//unit image
    unit.initialize(graphics,TEXTURE4_SIZE,TEXTURE4_SIZE,TEXTURE4_COLS,&textures4);
    unit.setFrames(0, 0);
    unit.setCurrentFrame(0);

	//healthbar image
    healthBar.initialize(graphics,TEXTURE5_SIZE,TEXTURE5_SIZE,TEXTURE5_COLS,&textures5);
    healthBar.setFrames(0, 0);
    healthBar.setCurrentFrame(0);

	//unitCounter image
    unitCounter.initialize(graphics,TEXTURE6_SIZE,TEXTURE6_SIZE,TEXTURE6_COLS,&textures6);
    unitCounter.setFrames(0, 0);
    unitCounter.setCurrentFrame(0);

	playerViewY = 0;
	playerViewX = 0;
	changeTile = 0;
	lastSelectCol = 0;
	lastSelectRow = 0;
	selected = 0;
	timer = 0;
	turn = 0;
	animationSlowTicker = 0;

	unitData bear; 
	bear.maxhealth = 10;
	bear.startFrame = 1;
	bear.endFrame = 8;
	bear.baseAttack = 5;
	bear.baseDefence = 1;
	bear.baseRange = 0;
	bear.id = 0;
	bear.moveperturn = 2;
	bear.unitName = "Bear";

	bear.animations[0][0] = 0;
	bear.animations[0][1] = 0;
	bear.animations[0][2] = 0;
	bear.animations[0][3] = 0;
	bear.animations[0][4] = 0;
	bear.animations[0][5] = 0;
	bear.animations[0][6] = 0;
	bear.animations[0][7] = 0;
	bear.animations[0][8] = 0;
	bear.animations[0][9] = 0;
	bear.animations[0][10] = 0;
	bear.animations[0][11] = 0;
	bear.animations[0][12] = 0;
	bear.animations[0][13] = 0;
	bear.animations[0][14] = 0;
	bear.animations[0][15] = 0;

	bear.animations[1][0] = 0;
	bear.animations[1][1] = 0;
	bear.animations[1][2] = 0;
	bear.animations[1][3] = 0;
	bear.animations[1][4] = 0;
	bear.animations[1][5] = 0;
	bear.animations[1][6] = 0;
	bear.animations[1][7] = 0;
	bear.animations[1][8] = 0;
	bear.animations[1][9] = 0;
	bear.animations[1][10] = 0;
	bear.animations[1][11] = 0;
	bear.animations[1][12] = 0;
	bear.animations[1][13] = 0;
	bear.animations[1][14] = 0;
	bear.animations[1][15] = 0;

	bear.animations[2][0] = 4;
	bear.animations[2][1] = 5;
	bear.animations[2][2] = 4;
	bear.animations[2][3] = 5;
	bear.animations[2][4] = 4;
	bear.animations[2][5] = 5;
	bear.animations[2][6] = 4;
	bear.animations[2][7] = 5;
	bear.animations[2][8] = 4;
	bear.animations[2][9] = 5;
	bear.animations[2][10] = 4;
	bear.animations[2][11] = 5;
	bear.animations[2][12] = 4;
	bear.animations[2][13] = 5;
	bear.animations[2][14] = 4;
	bear.animations[2][15] = 5;

	bear.animations[3][0] = 2;
	bear.animations[3][1] = 3;
	bear.animations[3][2] = 2;
	bear.animations[3][3] = 3;
	bear.animations[3][4] = 2;
	bear.animations[3][5] = 3;
	bear.animations[3][6] = 2;
	bear.animations[3][7] = 3;
	bear.animations[3][8] = 2;
	bear.animations[3][9] = 3;
	bear.animations[3][10] = 2;
	bear.animations[3][11] = 3;
	bear.animations[3][12] = 2;
	bear.animations[3][13] = 3;
	bear.animations[3][14] = 2;
	bear.animations[3][15] = 3;

	bear.animations[4][0] = 0;
	bear.animations[4][1] = 1;
	bear.animations[4][2] = 0;
	bear.animations[4][3] = 1;
	bear.animations[4][4] = 0;
	bear.animations[4][5] = 1;
	bear.animations[4][6] = 0;
	bear.animations[4][7] = 1;
	bear.animations[4][8] = 0;
	bear.animations[4][9] = 1;
	bear.animations[4][10] = 0;
	bear.animations[4][11] = 1;
	bear.animations[4][12] = 0;
	bear.animations[4][13] = 1;
	bear.animations[4][14] = 0;
	bear.animations[4][15] = 1;

	bear.animations[5][0] = 6;
	bear.animations[5][1] = 7;
	bear.animations[5][2] = 6;
	bear.animations[5][3] = 7;
	bear.animations[5][4] = 6;
	bear.animations[5][5] = 7;
	bear.animations[5][6] = 6;
	bear.animations[5][7] = 7;
	bear.animations[5][8] = 6;
	bear.animations[5][9] = 7;
	bear.animations[5][10] = 6;
	bear.animations[5][11] = 7;
	bear.animations[5][12] = 6;
	bear.animations[5][13] = 7;
	bear.animations[5][14] = 6;
	bear.animations[5][15] = 7;


	unitData lion;
	lion.maxhealth = 10;
	lion.startFrame = 14;
	lion.endFrame = 22;
	lion.baseAttack = 5;
	lion.baseDefence = 1;
	lion.baseRange = 0;
	lion.id = 1;
	lion.moveperturn = 3;
	lion.unitName = "Lion";

	lion.animations[0][0] = 0;
	lion.animations[0][1] = 0;
	lion.animations[0][2] = 0;
	lion.animations[0][3] = 0;
	lion.animations[0][4] = 0;
	lion.animations[0][5] = 0;
	lion.animations[0][6] = 0;
	lion.animations[0][7] = 0;
	lion.animations[0][8] = 0;
	lion.animations[0][9] = 0;
	lion.animations[0][10] = 0;
	lion.animations[0][11] = 0;
	lion.animations[0][12] = 0;
	lion.animations[0][13] = 0;
	lion.animations[0][14] = 0;
	lion.animations[0][15] = 0;

	lion.animations[1][0] = 0;
	lion.animations[1][1] = 0;
	lion.animations[1][2] = 0;
	lion.animations[1][3] = 0;
	lion.animations[1][4] = 0;
	lion.animations[1][5] = 0;
	lion.animations[1][6] = 0;
	lion.animations[1][7] = 0;
	lion.animations[1][8] = 0;
	lion.animations[1][9] = 0;
	lion.animations[1][10] = 0;
	lion.animations[1][11] = 0;
	lion.animations[1][12] = 0;
	lion.animations[1][13] = 0;
	lion.animations[1][14] = 0;
	lion.animations[1][15] = 0;

	lion.animations[2][0] = 6;
	lion.animations[2][1] = 7;
	lion.animations[2][2] = 6;
	lion.animations[2][3] = 7;
	lion.animations[2][4] = 6;
	lion.animations[2][5] = 7;
	lion.animations[2][6] = 6;
	lion.animations[2][7] = 7;
	lion.animations[2][8] = 6;
	lion.animations[2][9] = 7;
	lion.animations[2][10] = 6;
	lion.animations[2][11] = 7;
	lion.animations[2][12] = 6;
	lion.animations[2][13] = 7;
	lion.animations[2][14] = 6;
	lion.animations[2][15] = 7;

	lion.animations[3][0] = 2;
	lion.animations[3][1] = 3;
	lion.animations[3][2] = 2;
	lion.animations[3][3] = 3;
	lion.animations[3][4] = 2;
	lion.animations[3][5] = 3;
	lion.animations[3][6] = 2;
	lion.animations[3][7] = 3;
	lion.animations[3][8] = 2;
	lion.animations[3][9] = 3;
	lion.animations[3][10] = 2;
	lion.animations[3][11] = 3;
	lion.animations[3][12] = 2;
	lion.animations[3][13] = 3;
	lion.animations[3][14] = 2;
	lion.animations[3][15] = 3;

	lion.animations[4][0] = 0;
	lion.animations[4][1] = 1;
	lion.animations[4][2] = 0;
	lion.animations[4][3] = 1;
	lion.animations[4][4] = 0;
	lion.animations[4][5] = 1;
	lion.animations[4][6] = 0;
	lion.animations[4][7] = 1;
	lion.animations[4][8] = 0;
	lion.animations[4][9] = 1;
	lion.animations[4][10] = 0;
	lion.animations[4][11] = 1;
	lion.animations[4][12] = 0;
	lion.animations[4][13] = 1;
	lion.animations[4][14] = 0;
	lion.animations[4][15] = 1;

	lion.animations[5][0] = 4;
	lion.animations[5][1] = 5;
	lion.animations[5][2] = 4;
	lion.animations[5][3] = 5;
	lion.animations[5][4] = 4;
	lion.animations[5][5] = 5;
	lion.animations[5][6] = 4;
	lion.animations[5][7] = 5;
	lion.animations[5][8] = 4;
	lion.animations[5][9] = 5;
	lion.animations[5][10] = 4;
	lion.animations[5][11] = 5;
	lion.animations[5][12] = 4;
	lion.animations[5][13] = 5;
	lion.animations[5][14] = 4;
	lion.animations[5][15] = 5;

			//int animations[16][16]; 
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
	unitLibrary.library.push_back(bear);
	unitLibrary.library.push_back(lion);


	debugMessage = false;

	someUnits.units.push_back(createUnit(5, 5, 1, unitLibrary.library[0]));
	someUnits.units.push_back(createUnit(10, 5, 1, unitLibrary.library[0]));
	someUnits.units.push_back(createUnit(10, 10, 0, unitLibrary.library[1]));
	someUnits.units.push_back(createUnit(10, 10, 0, unitLibrary.library[1]));
	someUnits.units.push_back(createUnit(10, 10, 0, unitLibrary.library[1]));

	turnMessage = false;
	selectMessage = false;
	attacking = false;
	combatty = false;

	yourTeam = 0;
}

bool checkSpace(int xx, int yy, map worldMap)
{
	if(worldMap.row[xx].column[yy].currentUnitFrame == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

unitGroup combat(unitGroup someUnits, int selected, int targetX, int targetY)
{
	int target = -1;
	for(int x = 0; x < someUnits.units.size(); x++)
	{
		if(someUnits.units[x].x == targetX && someUnits.units[x].y == targetY)
		{
			target = x;
		}
	}
	///if(target != -1)
	//{
		someUnits.units[target].health -= someUnits.units[selected].attack;
		someUnits.units[selected].health -= (someUnits.units[target].defence);
	//}

	return someUnits;
}

//=============================================================================
// Update all game items
//=============================================================================
void Elevations::update()
{
	
   // fallingY += frameTime * FALLING_SPEED;
   // if(fallingY > GAME_HEIGHT)
   //     fallingY = FALLING_Y;
   // angle += frameTime * 360;

	animationSlowTicker -= 1;
	if(animationSlowTicker < 0)
	{
		animationSlowTicker = 10;
	}

	if(timer > 0)
	{
		timer = timer - 1;
		if(timer == 0)
		{
			for(int x = 0; x < someUnits.units.size(); x++)
			{
				if(someUnits.units[x].health <= 0)
				{
					someUnits.units.erase(someUnits.units.begin()+x);
					attacking = false;
					if(selected > x)
					{
						//debugMessage = true;
						selected--;
					}
					else if(selected == x)
					{
						selected = -1;
					}
				}
			}

			combatty = false;
			if(turnMessage == true)
			{
				//do endturn calculations
				for(int x = 0; x < someUnits.units.size(); x++)
				{
					someUnits.units[x].movement = unitLibrary.library[someUnits.units[x].id].moveperturn; 
				}

				turnMessage = false;
				debugMessage = false;
				// end of endturn calculations
			}
		}
	}

	for(int row=0; row<MAP_SIZE; row++)
	{
		for(int col=0; col<MAP_SIZE; col++)
		{
			worldMap.row[row].column[col].currentUnitFrame = 0;
			worldMap.row[row].column[col].currentSelectFrame = 0;
			worldMap.row[row].column[col].currentHealthBarFrame = 0;
			worldMap.row[row].column[col].currentUnitCounterFrame = 0;
		}
	}

	if(selected == -1)
	{
		worldMap.row[lastSelectRow].column[lastSelectCol].currentSelectFrame = 1;
	}
	else if(selected > -1)
	{
		worldMap.row[lastSelectRow].column[lastSelectCol].currentSelectFrame = someUnits.units[selected].currentDirection;
	}

	

	for(int x = 0; x < someUnits.units.size(); x++)
	{
		int unitTotal = 1;
		int unitInactive = 0;
		for(int y = 0; y < someUnits.units.size(); y++)
		{
			if(y != x && someUnits.units[y].x == someUnits.units[x].x && someUnits.units[y].y == someUnits.units[x].y)
			{
				unitTotal++;
			}
			if(someUnits.units[y].x == someUnits.units[x].x && someUnits.units[y].y == someUnits.units[x].y && someUnits.units[y].movement <= 0)
			{
				unitInactive++;
			}
		}
		worldMap.row[someUnits.units[x].x].column[someUnits.units[x].y].currentUnitCounterFrame = unitTotal + (unitInactive * 11); //set unitcounter

		//healthbar calculation
		worldMap.row[someUnits.units[x].x].column[someUnits.units[x].y].currentHealthBarFrame = calculateHealthBar(someUnits.units[x].health, unitLibrary.library[someUnits.units[x].id].maxhealth);
	}
	

	for(int x = 0; x < someUnits.units.size(); x++)
	{
		if(animationSlowTicker == 0)
		{
			someUnits.units[x].animationCount += 1;
		}
		if(someUnits.units[x].animationCount >= 15)
		{
			someUnits.units[x].animationCount = 0;
			someUnits.units[x].currentAction = 0;
		}

		someUnits.units[x].currentFrame = unitLibrary.library[someUnits.units[x].id].animations[someUnits.units[x].currentAction][someUnits.units[x].animationCount]
		+ unitLibrary.library[someUnits.units[x].id].startFrame; // make current frame of unit correspond to current animation

		worldMap.row[someUnits.units[x].x].column[someUnits.units[x].y].currentUnitFrame = someUnits.units[x].currentFrame; // place unit frame onto tilemap
	}

    mapTile.update(frameTime);

	if(selected == -1 || selected == -2)
	{
		if (input->isKeyDown(UP_KEY))       // if up arrow
		{
			playerViewY += 1;
		}
		if (input->isKeyDown(DOWN_KEY))     // if down arrow
		{
			playerViewY -= 1;
		}
		if (input->isKeyDown(LEFT_KEY))     // if left
		{
			playerViewX += 1;
		}
		if (input->isKeyDown(RIGHT_KEY))    // if right
		{
			playerViewX -= 1;
		}

	}
	else if(timer == 0 && someUnits.units[selected].movement > 0 && someUnits.units[selected].team == yourTeam)
	{
		timer = 25;
		if (input->isKeyDown(UP_KEY))       // if up arrow
		{
			if(checkSpace(lastSelectRow-1, lastSelectCol, worldMap) == false)
			{
				someUnits.units[selected].currentDirection = 2;
				someUnits.units[selected].x -= 1;
				someUnits.units[selected].movement--;
				someUnits.units[selected].currentAction = 2;
				lastSelectRow-=1;
			}
			else if(attacking == true) //unit is there and attacking == true
			{
				//do combat
				someUnits = combat(someUnits, selected, lastSelectRow-1, lastSelectCol);
				someUnits.units[selected].movement--;
				combatty = true;
			}
		}
		else if (input->isKeyDown(DOWN_KEY))     // if down arrow
		{
			if(checkSpace(lastSelectRow+1, lastSelectCol, worldMap) == false)
			{
				someUnits.units[selected].currentDirection = 4;
				someUnits.units[selected].x += 1;
				someUnits.units[selected].movement--;
				someUnits.units[selected].currentAction = 4;
				lastSelectRow+=1;
			}
			else if(attacking == true) //unit is there and attacking == true
			{
				//do combat
				someUnits = combat(someUnits, selected, lastSelectRow+1, lastSelectCol);
				someUnits.units[selected].movement--;
				combatty = true;
			}
		}
		else if (input->isKeyDown(LEFT_KEY))     // if left
		{
			if(checkSpace(lastSelectRow, lastSelectCol-1, worldMap) == false)
			{
				someUnits.units[selected].currentDirection = 5;
				someUnits.units[selected].y -= 1;
				someUnits.units[selected].movement--;
				someUnits.units[selected].currentAction = 5;
				lastSelectCol-=1;
			}
			else if(attacking == true) //unit is there and attacking == true
			{
				//do combat
				someUnits = combat(someUnits, selected, lastSelectRow, lastSelectCol-1);
				someUnits.units[selected].movement--;
				combatty = true;
			}
		}
		else if (input->isKeyDown(RIGHT_KEY))    // if right
		{
			if(checkSpace(lastSelectRow, lastSelectCol+1, worldMap) == false)
			{
				someUnits.units[selected].currentDirection = 3;
				someUnits.units[selected].y += 1;
				someUnits.units[selected].movement--;
				someUnits.units[selected].currentAction = 3;
				lastSelectCol+=1;
			}
			else if(attacking == true) //unit is there and attacking == true
			{
				//do combat
				someUnits = combat(someUnits, selected, lastSelectRow, lastSelectCol+1);
				someUnits.units[selected].movement--;
				combatty = true;
			}
		}

		else if (input->isKeyDown(ALT_KEY))
		{
			//attack mode on/off
			if(attacking == true)
			{
				attacking = false;
			}
			else
			{
				attacking = true;
			}
			timer = 25;
		}
	}


	if (input->isKeyDown(ENTER_KEY)&& timer == 0)
	{
		turn+=1;
		turnMessage=true;
		timer = 250;
	}



	if (input->getMouseLButton() || input->getMouseRButton())
	{
		int mx = input->getMouseX();
		int my = input->getMouseY();

		cout << "MOUSEINFO:" << endl;
		cout << mx << endl;
		cout << my << endl;
		bool Loop = true;


		for(int row=0; row<MAP_SIZE && Loop; row++)
		{
			for(int col=0; col<MAP_SIZE && Loop; col++)
			{
				if(mx <= (SCREEN_X - ((row+1)*TEXTURE_SIZE/2) + ((col+2)*TEXTURE_SIZE/2) ) + playerViewX + 16
				   && mx >= (SCREEN_X - ((row+1)*TEXTURE_SIZE/2) + ((col+2)*TEXTURE_SIZE/2) ) + playerViewX -16
				   && my <= ( SCREEN_Y + ((row+1)*TEXTURE_SIZE/4) + ((col+2)*TEXTURE_SIZE/4) - heightMap[row][col] * HEIGHT_CHANGE)+ playerViewY +16
				   && my >= ( SCREEN_Y + ((row+1)*TEXTURE_SIZE/4) + ((col+2)*TEXTURE_SIZE/4) - heightMap[row][col] * HEIGHT_CHANGE)+ playerViewY -16)
				{
					if(input->getMouseLButton())
					{
						selected = -1;
						worldMap.row[lastSelectRow].column[lastSelectCol].currentSelectFrame = 0;
						worldMap.row[row].column[col].currentSelectFrame = 1;
						lastSelectCol = col;
						lastSelectRow = row;
						
						//someUnits.units.size()
						for(int xy = 0; xy < someUnits.units.size(); xy++)
						{
							if(someUnits.units[xy].x == lastSelectRow && someUnits.units[xy].y == lastSelectCol)
							{
								selected = xy;
								//turnMessage = true;
							}
							
						}
						selectMessage = true;
					}
					else
					{
						bool Loop2 = true;
						for(int row=0; row<MAP_SIZE && Loop2; row++)
						{
							for(int col=0; col<MAP_SIZE && Loop2; col++)
							{
								if(worldMap.row[lastSelectRow].column[lastSelectCol].currentSelectFrame != 0)
								{
									worldMap.row[lastSelectRow].column[lastSelectCol].currentSelectFrame = 0;
									selected = -2;
								}
							}
						}
						selectMessage=false;
					}

					Loop = false;
				}
			}
		}
	}
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Elevations::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Elevations::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void Elevations::render()
{
    graphics->spriteBegin();

    // Draw map in Isometric Diamond
    for(int row=0; row<MAP_SIZE; row++)
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            mapTile.setCurrentFrame(worldMap.row[row].column[col].currentLandFrame);
            mapTile.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ) + playerViewX);
            mapTile.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) -
                                  heightMap[row][col] * HEIGHT_CHANGE)+ playerViewY);
            mapTile.draw();
        }
    }


	
    // Draw Objects

	for(int row=0; row<MAP_SIZE; row++)
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            objectResource.setCurrentFrame(worldMap.row[row].column[col].currentResourceFrame);
            objectResource.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ) + playerViewX);
            objectResource.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) -
                                  heightMap[row][col] * HEIGHT_CHANGE)+ playerViewY);
            objectResource.draw();
        }
    }


	// Draw Selection

	for(int row=0; row<MAP_SIZE; row++)
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            selection.setCurrentFrame(worldMap.row[row].column[col].currentSelectFrame);
            selection.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ) + playerViewX);
            selection.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) - 32)+ playerViewY);
            selection.draw();
        }
    }

		// Draw units

	for(int row=0; row<MAP_SIZE; row++)
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            unit.setCurrentFrame(worldMap.row[row].column[col].currentUnitFrame);
            unit.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ) + playerViewX);
            unit.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) - heightMap[row][col] * (HEIGHT_CHANGE))+ playerViewY);
            unit.draw();
        }
    }

	// Draw unitCounter
	for(int row=0; row<MAP_SIZE; row++)
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            unitCounter.setCurrentFrame(worldMap.row[row].column[col].currentUnitCounterFrame);
            unitCounter.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ) + playerViewX);
            unitCounter.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) + 3 * (HEIGHT_CHANGE)) + playerViewY);
            unitCounter.draw();
        }
    }

	// Draw healthBar
	for(int row=0; row<MAP_SIZE; row++)
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            healthBar.setCurrentFrame(worldMap.row[row].column[col].currentHealthBarFrame);
            healthBar.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ) + playerViewX);
            healthBar.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) - heightMap[row][col] * (HEIGHT_CHANGE)) + playerViewY);
            healthBar.draw();
        }
    }


	        // falling solid text
        fontCK->setProportional(true);
        fontCK->setFontColor(graphicsNS::GRAY); // solid
		string turny = static_cast<ostringstream*>( &(ostringstream() << turn) )->str();
        fontCK->print(" Turn: "+turny,0,0);
		if(turnMessage == true)
		{
			fontCK->print("ENDING TURN...",0,64);
		}

		if(selectMessage == true)
		{
			string terrain = "";
			if(worldMap.row[lastSelectRow].column[lastSelectCol].currentLandFrame == 2)
			{
				terrain = "Plains";
			}

			string alititude = static_cast<ostringstream*>( &(ostringstream() << heightMap[lastSelectRow][lastSelectCol]) )->str();

			fontCK->print("Terrain: "+terrain,0,128);
			fontCK->print("Altitude: "+alititude,0,192);\

			if(selected != -1)
			{
				string unitName;
				unitName = unitLibrary.library[someUnits.units[selected].id].unitName;

				string movementstring = static_cast<ostringstream*>( &(ostringstream() << someUnits.units[selected].movement) )->str();
				string healthstring = static_cast<ostringstream*>( &(ostringstream() << someUnits.units[selected].health) )->str();
				
				string teamString;

				if(someUnits.units[selected].team == yourTeam)
				{
					teamString = "FRIENDLY";
					fontCK->setFontColor(graphicsNS::GREEN); // solid
					fontCK->print("Team: "+teamString, 0, 448);
					fontCK->setFontColor(graphicsNS::GRAY); // solid
				}
				else //TODO: make conditions for war/peace neutral/friendly/hostile
				{
					fontCK->setFontColor(graphicsNS::RED); // solid
					teamString = "HOSTILE";
					fontCK->print("Team: "+teamString, 0, 448);
					fontCK->setFontColor(graphicsNS::GRAY); // solid
				}

				fontCK->print("Unit: "+unitName,0,256);
				fontCK->print("Movement: "+movementstring,0,320);
				fontCK->print("Health: "+healthstring, 0, 384);

				if(attacking == true && combatty == false)
				{
					fontCK->print("ATTACK?", 0, 512);
				}
				else if(combatty == true)
				{
					fontCK->setFontColor(graphicsNS::RED); // solid
					fontCK->print("ATTACKING",0,512);
				}
			}
		}

		if(debugMessage == true)
		{
			fontCK->print("BEEP BEEP", 100, 400);
		}

		//string sizenum = static_cast<ostringstream*>( &(ostringstream() << someUnits.units.size()) )->str();
		//fontCK->print("SIZE: "+sizenum, 200, 400);
		//fontCK->print(""+timer,0,12);
	 //bear.draw();
    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Elevations::releaseAll()
{
    textures.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Elevations::resetAll()
{
    textures.onResetDevice();

    Game::resetAll();
    return;
}
