#ifndef GUARD_groundEnemy
#define GUARD_groundEnemy

#include "gameObject.h"

class GroundEnemy : public gameObject
{
	//state attribute, i.e. whether groundEnemy is midair or not (maybe invulnerable etc)

	double timeExploding;
	
public:
	void init();
	//constructor
	GroundEnemy( double, double, double, double );
	virtual void tick(double) override;
	bool collidingWithPlanet(double) override;
	void collideWithPlanet(double) override;
};

#endif