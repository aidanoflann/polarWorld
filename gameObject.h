#ifndef GUARD_gameObject
#define GUARD_gameObject

class gameObject
{
protected:
	//coordinates in polar space
	double r;
	double theta;
	//velocity in polar space
	double rVel;
	double thetaVel;
	//direction of rVel and thetaVel (-1, 0 or 1)
	double rVelDirection;
	double thetaVelDirection;
	//collision radius - all collisions will be circle-circle
	double collisionRadius;
	//gravity. same for all objects
	double g;
	//attribute for removing dead objects at the end of the while loop
	bool markedForDeletion;

public:
	//tick function - performs all physics etc
	gameObject();
	virtual void tick(double d_time);
	void init();
	double getR() const;
	double getTheta() const;
	double getCollisionRadius() const;
	void setR(double);
	void setTheta(double);
	void setRVelDirection(double);
	void setThetaVelDirection(double);
	virtual bool collidingWithPlanet(double);
	virtual void collideWithPlanet(double);
	bool collidingWithGameObject(double, double, double);
	bool getMarkedForDeletion();
	void MarkForDeletion();
};

#endif