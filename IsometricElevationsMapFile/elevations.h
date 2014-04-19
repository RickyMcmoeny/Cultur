
#ifndef _ELEVATIONS_H           // Prevent multiple definitions if this 
#define _ELEVATIONS_H           // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <fstream>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "text.h"
#include "textDX.h"
#include "entity.h"
#include <vector>

using namespace std; 

	struct unitData //axeman data, spearman data, etc.
	{
		int maxhealth;
		int startFrame; // starting frame inside spritesheet
		int endFrame; // end frame inside spritesheet
		int animations[16][16]; 
		int baseAttack;
		int baseDefence;
		int baseRange;
		int id; // id that corresponds to this unitdata
		int moveperturn;

		string unitName;

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
	};

	struct dataBase
	{
		vector<unitData> library;
	};


	struct unit
	{
			
		int id; // what kind of unit it is (ie: spearman, axeman, swordsman)
		int team; // which team
		int health; // hit points
		int attack; // baseAttack + modifiers
		int defence; // baseDefence + modifiers
		int range; //0: melee, 1: throwing distance (1 tile), 2: two tiles, 3: 3 tiles ect.
		int x; // row
		int y; // column
		int movement; //how many tiles it can move per turn
		int currentFrame; // current visible frame
		int animationCount;
		int currentAction; 
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
		int currentDestinationX; //set destination x
		int currentDestinationY; //set destination y
		int currentDirection; // current direction the unit is facing (ie northwest)
						  // 0: north
						  // 1: northeast
						  // 2: east
						  // 3: southeast
						  // 4: south
						  // 5: southwest
						  // 6: west
					      // 7: northwest
	};

	struct unitGroup
	{
		bool canMove;
		vector<unit> units;
	};

	struct teamUnits
	{
		bool hasMoved;
		vector<unit> singleUnits;
		vector<unitGroup> groups;
	};

	struct team
	{
		teamUnits forces;
		int teamID;
	};


	struct improvementDescription
	{
		int resourceRequired; //0 = none, 1=sheep, 2=iron...ect...
		int terrainRequired; //0 = none, 1=plains...ect
	};



	struct improvement
	{
		int controller; // team ID
		int ownership; //private v.s. public
		int value; //0 = shithole, 100 = flourishing
		bool sacked; // false: all is good, true = burn't
		int upkeep; // how much money it costs to keep it running
	};

	struct resource
	{
		int resourceID; //sheep, frogs, iron, etc.
		int resourceAmount;
		int resourceAccessibility;
	};

	struct city
	{


	};

	struct population
	{
		vector<int> religionID; // id of religions
		vector<int> cultureID; // id of cultures
		vector<int> religionInfluence; // influence of religion
		vector<int> cultureInfluence; // influence of culture
		
		int crime; // amount of crime (0 = law and order, 1000 = "public" ownership
		int happiness; //how happy everyone is (0 = frothing at the mouth, 1000 = extacy
		int stability; //stability of the population (0 = apocalyptic chaos, 1000 = complete order)
		int prosperity; //economiic prosperity (0 = somalian, 1000 = golden age)

		int populationSize; //amount of people
		float populationGrowth; //growth of population (-100% - +infinte%)

		int settlementLevel; //solitary shack, outpost, cottages, hamlet, town, large town, urban, dense urban, metropolice
	};

	struct tile
	{
		int x; //row
		int y; //column

		int alititude; //below sea level, at sea level, above sea level, medium alititude, high altitude, extreme altitude, space
		int hills; // crater, valley, canyon, flat, rough, rolling hills, hilly, large hills, mountainous, jaggedly mountaineous, mordor
		int vegetation; //desolate, weeds, shrubs, grass, plains, trees, thin forest, forest, thick forest, jungle, amazonian
		int water;	// 0 = moon, 1 = desert, 2 = arid, 3 = streams, 4 = river, 5 = lakes, 6 = soggy as fuck, 7 = knee-deep, 8 = oceanic
		int pollution; // 0 = fresh, 1000 = not in my backyard

		int arability; //how well the land can be farmed

		population people;
		vector<improvement> improvements; //mines, trading posts, wells, aquaducts, power plants, etc.
		vector<resource> resources;

		vector<int> teamVisibility; //if id of team is in here this tile is immediately visible to that team
		vector<int> teamMapped; //if id of team is in here this tile is known to that team

		int currentLandFrame;
		int currentHillFrame;
		int currentResourceFrame;
		int currentSettlementFrame;
		int currentSelectFrame;
		int currentUnitFrame;
		int currentUnitCounterFrame;
		int currentHealthBarFrame;
	
	};

	struct columns
	{
		vector<tile> column;
	};

	
	struct map
	{

		vector<columns> row;
	};


namespace elevationsNS
{
    // Map textures
    const int TEXTURE_COLS = 4;
    const int TEXTURE_SIZE = 64;
    const int MAP_SIZE = 16;
    const int SCREEN_X = GAME_WIDTH/2-TEXTURE_SIZE/2;
    const int SCREEN_Y = GAME_HEIGHT/8;

	const int BUF_SIZE = 20;
	const float FALLING_SPEED = 100.0f;
	const float FALLING_Y = -100.0f;
	const float POINT_SIZE_SPEED = 200.0f;
	const float MAX_POINT_SIZE = 480.0f;
	const float CH_TIME = 0.1f;

    // Object textures
    const int TEXTURE2_COLS = 6;
    const int TEXTURE2_SIZE = 64;

	//Selection Textures
	const int TEXTURE3_COLS = 6;
    const int TEXTURE3_SIZE = 64;

	const int TEXTURE4_COLS = 8;
    const int TEXTURE4_SIZE = 64;

	const int TEXTURE5_COLS = 13;
    const int TEXTURE5_SIZE = 64;

	const int TEXTURE6_COLS = 11;
    const int TEXTURE6_SIZE = 64;
}

// Elevations is the class we create, it inherits from the Game class
class Elevations : public Game
{

private:
    // game items
    TextureManager textures;    // textures
    TextureManager textures2;   // object texture
	TextureManager textures3;   // selection textures
	TextureManager textures4;   // unit textures
	TextureManager textures5;   // unit textures
	TextureManager textures6;   // unit textures
    Image   mapTile;
    Image   objectResource;
	Image   selection;
	Image	unit;
	Image   healthBar;
	Image	unitCounter;
    // Maps
    int tileMap[elevationsNS::MAP_SIZE][elevationsNS::MAP_SIZE];
    int heightMap[elevationsNS::MAP_SIZE][elevationsNS::MAP_SIZE];
    int objectMap[elevationsNS::MAP_SIZE][elevationsNS::MAP_SIZE];    // 
	int selectMap[elevationsNS::MAP_SIZE][elevationsNS::MAP_SIZE];    //
	int unitMap[elevationsNS::MAP_SIZE][elevationsNS::MAP_SIZE];    //
	int healthbarMap[elevationsNS::MAP_SIZE][elevationsNS::MAP_SIZE];    //
	int unitCounterMap[elevationsNS::MAP_SIZE][elevationsNS::MAP_SIZE];    //


	int playerViewY;
	int playerViewX;
	int changeTile;
	int lastSelectCol;
	int lastSelectRow;
	int selected;
	int turn;
	int timer;
	int yourTeam;

	int animationSlowTicker;

	map worldMap;
	unitGroup someUnits;
	dataBase unitLibrary;

	bool turnMessage;
	bool selectMessage;
	bool debugMessage;
	bool attacking;
	bool combatty;

	 // game items    
    Text    *fontCK;                    // sprite based font
    TextDX  *dxFont14;                  // DirectX fonts
    TextDX  *dxFont24;
    TextDX  *dxFont48;
    TextDX  *dxFont48B;                 // bold
    TextDX  *dxFont96;
    TextDX  *dxFont24F;                 // 24 point fixed pitch

    // game variables
    double  alignmentTimer;             // used to change text alignment display
    bool    spriteText;                 // true for sprite text, false for DirectX text
    byte red, green, blue;
   // char buffer[BUF_SIZE];
    VECTOR2 position;                   // used to position items on screen
    float fallingY;
    float angle;
    float pointSize;                    // for changing point size
    float chTimer;
    UCHAR ch;
    char st[2];

public:
	// Constructor
    Elevations();
    // Destructor
    virtual ~Elevations();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
};

#endif
