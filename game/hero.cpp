#include "hero.h"

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool Hero::checkPlatform(SDL_Rect a, Tile *tiles[]) { //Detects if batman is on a platform
	//The sides of the rectangles
	int centerA, centerB;
	int topB;
	int bottomA;

	//Calculate the sides of rect A
	centerA = a.x + a.w/2;
	bottomA = a.y + a.h;

	for (int i = 0; i < TOTAL_TILES; i++) {
		if (tiles[i]->getType() == 2) {
			centerB = tiles[i]->getBox().x + tiles[i]->getBox().w/2;
			topB = tiles[i]->getBox().y;

			if (topB == bottomA && abs(centerA-centerB) <= 40)
			{
				return true;
			}
		}
	}
	return false;
}

Hero::Hero()
{
	//Initialize the position
	mPosX = SCREEN_WIDTH / 2;
	mPosY = SCREEN_HEIGHT-HERO_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	run_time = 0;
	lives = 3;
	hurt_time = 0;
	crashed = false;
	on_platform = true;
	jump_timer = 0;
	collider.w = HERO_WIDTH;
	collider.h = HERO_HEIGHT;

	current_clip = &gHeroClips[rstand];

	//Set sprite clips
	gHeroClips[rstand].x = 181*2;
	gHeroClips[rstand].y = 0 * 2;
	gHeroClips[rstand].w = 58 * 2;
	gHeroClips[rstand].h = 100 * 2;

	gHeroClips[rrun1].x = 100 * 2;
	gHeroClips[rrun1].y = 0 * 2;
	gHeroClips[rrun1].w = 80 * 2;
	gHeroClips[rrun1].h = 100 * 2;

	gHeroClips[rrun2].x = 0 * 2;
	gHeroClips[rrun2].y = 0 * 2;
	gHeroClips[rrun2].w = 99 * 2;
	gHeroClips[rrun2].h = 100 * 2;

	gHeroClips[rjump].x = 240 * 2;
	gHeroClips[rjump].y = 0 * 2;
	gHeroClips[rjump].w = 60 * 2;
	gHeroClips[rjump].h = 100 * 2;

	gHeroClips[lstand].x = 61 * 2;
	gHeroClips[lstand].y = 101 * 2;
	gHeroClips[lstand].w = 58 * 2;
	gHeroClips[lstand].h = 100 * 2;

	gHeroClips[lrun1].x = 120 * 2;
	gHeroClips[lrun1].y = 101 * 2;
	gHeroClips[lrun1].w = 80 * 2;
	gHeroClips[lrun1].h = 100 * 2;

	gHeroClips[lrun2].x = 201 * 2;
	gHeroClips[lrun2].y = 101 * 2;
	gHeroClips[lrun2].w = 99 * 2;
	gHeroClips[lrun2].h = 100 * 2;

	gHeroClips[ljump].x = 0 * 2;
	gHeroClips[ljump].y = 101 * 2;
	gHeroClips[ljump].w = 60 * 2;
	gHeroClips[ljump].h = 100 * 2;
}

void Hero::move(float timeStep, Tile* tiles[])
{
	run_time += timeStep;

	//Move the hero left or right
	mPosX += mVelX * timeStep;
	collider.x = int(mPosX);

	//If the hero went too far to the left or right
	if ((mPosX < 0)) {
		mPosX = 0;
	}
	else if ((collider.x + HERO_WIDTH > LEVEL_WIDTH)) {
		mPosX = LEVEL_WIDTH - HERO_WIDTH;
	}
	
	//jumping mechanics below
	if (jump_timer != 0)
	{
		if (jump_timer > 2) jump_timer = 0; //if jump_timer max is reached, reset timer
		else jump_timer = +timeStep; //else increment
	}
	//platform interaction
	if (jump_timer == 0 && checkPlatform(collider, tiles)) { //If we're on a platform
		on_platform = true;
		if (current_clip == &gHeroClips[rjump] && mVelX == 0) current_clip = &gHeroClips[rstand]; //show different sprites on landing
		else if (current_clip == &gHeroClips[ljump] && mVelX == 0) current_clip = &gHeroClips[lstand];
		else if (current_clip == &gHeroClips[rjump] && mVelX > 0) current_clip = &gHeroClips[rrun1];
		else if (current_clip == &gHeroClips[ljump] && mVelX < 0) current_clip = &gHeroClips[lrun1];
	}
	else {
		on_platform = false; //If we're not on platform and just started jumping...
	}
	if (!on_platform) {
		mPosY += mVelY * timeStep; //Move the hero up or down and accelerate
		mVelY += 750 * timeStep;
		if (mVelX > 0) current_clip = &gHeroClips[rjump]; //show sprite clips depending on orientation
		else if (mVelX < 0) current_clip = &gHeroClips[ljump];
		else if (current_clip == &gHeroClips[rstand]) current_clip = &gHeroClips[rjump];
		else if (current_clip == &gHeroClips[lstand]) current_clip = &gHeroClips[ljump];
	}

	collider.y = int(mPosY);

	//affects running animation
	if (run_time > .35 && current_clip == &gHeroClips[rrun1]) {
		current_clip = &gHeroClips[rrun2];
		run_time = 0;
	}else if (run_time > .35 && current_clip == &gHeroClips[rrun2]) {
		current_clip = &gHeroClips[rrun1];
		run_time = 0;
	}
	if (run_time > .35 && current_clip == &gHeroClips[lrun1]) {
		current_clip = &gHeroClips[lrun2];
		run_time = 0;
	}
	else if (run_time > .35 && current_clip == &gHeroClips[lrun2]) {
		current_clip = &gHeroClips[lrun1];
		run_time = 0;
	}
}
void Hero::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			if (on_platform && jump_timer == 0) {
				on_platform = false;
				mVelY = -HERO_VEL;
				jump_timer =+ 0.01;
			}
			break;
		case SDLK_LEFT:
			mVelX -= HERO_VEL;
			if (on_platform) current_clip = &gHeroClips[lrun1];
			break;
		case SDLK_RIGHT:
			mVelX += HERO_VEL;
			if (on_platform) current_clip = &gHeroClips[rrun1];
			break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			mVelX += HERO_VEL; 
			if (current_clip == &gHeroClips[lrun1] || current_clip == &gHeroClips[lrun2]) current_clip = &gHeroClips[lstand];
			break;
		case SDLK_RIGHT:
			mVelX -= HERO_VEL;
			if (current_clip == &gHeroClips[rrun1] || current_clip == &gHeroClips[rrun2]) current_clip = &gHeroClips[rstand];
			break;
		}
	}
}

SDL_Rect Hero::get_collider()
{
	return collider;
}
void Hero::set_crashed(bool hi)
{
	crashed = hi;
}
bool Hero::get_crashed()
{
	return crashed;
}
int Hero::get_lives()
{
	return lives;
}
void Hero::set_lives()
{
	lives = lives - 1;
}
int Hero::get_hurt_time()
{
	return hurt_time;
}
void Hero::set_hurt_time(int a)
{
	hurt_time = a;

}
bool Hero::get_on_platform()
{
	return on_platform;
}
void Hero::set_on_platform(bool a)
{
	on_platform = a;
}
float Hero::get_jump_timer()
{
	return jump_timer;
}
void Hero::set_jump_timer(float a)
{
	jump_timer = a;
}

void Hero::setCamera(SDL_Rect& camera)
{
	//Center the camera over the dot
	camera.x = (collider.x + HERO_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (collider.y + HERO_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}