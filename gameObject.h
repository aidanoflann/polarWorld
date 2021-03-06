#ifndef GUARD_gameObject
#define GUARD_gameObject

#include <string>

class gameObject
{
protected:
	
	enum State
	{
		Midair,
		Grounded,
		Exploding,
	};
	State state;
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
	std::string enemyType; //sky or ground
	int animationFrame; //cycle through animations
	double frameTime;
	double explodingDuration;
	

public:
	//tick function - performs all physics etc
	gameObject();
	virtual void tick(double d_time);
	void init();
	double getR() const;
	double getTheta() const;
	double getCollisionRadius() const;
	void setCollisionRadius(double);
	void setR(double);
	void setTheta(double);
	void setRVelDirection(double);
	void setThetaVelDirection(double);
	double getThetaVelDirection();
	virtual bool collidingWithPlanet(double);
	virtual void collideWithPlanet(double);
	virtual void startExploding();
	bool collidingWithGameObject(double, double, double);
	bool getMarkedForDeletion();
	void MarkForDeletion();
	std::string getEnemyType();
	int getAnimationFrame();
	void updateAnimationFrame(double, double, int);
	bool isNotExploding();
};

#endif