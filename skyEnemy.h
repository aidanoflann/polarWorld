#ifndef GUARD_skyEnemy
#define GUARD_skyEnemy

#include "gameObject.h"

class SkyEnemy : public gameObject
{
public:
	void init();
	//constructor
	SkyEnemy( double, double, double );
	virtual void tick(double) override;
};

#endif