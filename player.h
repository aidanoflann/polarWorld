#ifndef GUARD_player_h
#define GUARD_player_h

#include "gameObject.h"
#include "bullet.h"

class Player : public gameObject
{
private:
	//state attribute, i.e. whether player is midair or not (maybe invulnerable etc)
	enum State
	{
		Midair,
		Grounded,
	};
	State state;
	//rage, determines bullet speed as game progresses
	double rage;
	
public:
	//initialise default variables
	void init();
	//apply input, physics etc
	void tick(double);
	//check if colliding with the planet
	bool collidingWithPlanet(double);
	void collideWithPlanet(double);
	void jump();
	Player();
};

#endif