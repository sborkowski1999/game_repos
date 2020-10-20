#include <SDL.h>
#include "Tiles.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

class Hero
{
public:
	//The dimensions of the hero
	static const int HERO_WIDTH = 100;
	static const int HERO_HEIGHT = 200;
	static const int BRIEF_INV = 800;
	static const int ACCEL = 750;

	//Maximum axis velocity of the hero
	static const int HERO_VEL = 400;

	enum sprites{rstand,rrun1,rrun2,rjump,lstand,lrun1,lrun2,ljump};
	static const int SPRITES = 8;
	SDL_Rect gHeroClips[SPRITES];
	SDL_Rect* current_clip;

	float run_time;

	//Initializes the variables
	Hero();

	//Takes key presses and adjusts the xwing's velocity
	void handleEvent(SDL_Event& e);

	//Moves the hero
	void move(float timeStep, Tile *tiles[]);

	void setCamera(SDL_Rect& camera);
	bool checkPlatform(SDL_Rect, Tile *tiles[]);

	//Shows the xwing on the screen
	void render(SDL_Rect& camera);

	SDL_Rect get_collider();
	void set_crashed(bool);
	bool get_crashed();
	int get_lives();
	void set_lives();
	int get_hurt_time();
	void set_hurt_time(int);
	void set_on_platform(bool);
	bool get_on_platform();
	float get_jump_timer();
	void set_jump_timer(float);

private:
	bool on_platform;
	bool crashed;
	int lives;
	int hurt_time;
	float jump_timer;

	float mPosX, mPosY;
	float mVelX, mVelY;

	SDL_Rect collider;
};