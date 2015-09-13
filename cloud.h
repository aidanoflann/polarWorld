#ifndef GUARD_cloud
#define GUARD_cloud

#include "gameObject.h"

class Cloud : public gameObject
{
public:
	void init();
	//constructor
	Cloud(double, double);
	virtual void tick(double, double);
};

#endif