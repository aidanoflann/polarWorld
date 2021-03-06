#ifndef GUARD_cloud
#define GUARD_cloud

#include "gameObject.h"

class Cloud : public gameObject
{
private:
	double startCDTimer;
	double spawnCoolDown;
	double timeNow;
	double thetaOffset;
	double thetaOffsetMax;
	
public:
	void init();
	//constructor
	Cloud(double, double);
	virtual void tick(double, double);
	bool doSpawn(double, int);
};

#endif