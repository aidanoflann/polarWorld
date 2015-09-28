#include "player.h"
#include <iostream>


Player::Player() : gameObject()
{
	//default position is at (0,0) (in both xy and rtheta)
	r = 200;
	rVel = 0;
	rVelDirection = +1;
	theta = 270;
	thetaVel = 0.15;
	thetaVelDirection = 0;
	collisionRadius = 16;
	state = Midair;
	numKills = 0;
	shootingRight = 1; //1 is right, 0 is left
	runningRight = 1;
	justShot = false;
	timeSinceShot = 0;
}

void Player::init()
{
}

void Player::tick(double d_time)
{
	theta = theta + thetaVelDirection*thetaVel*d_time;
	if (state == Midair)
	{
		rVel = rVel + g*d_time;
		r = r + rVelDirection*rVel*d_time;
	}
	
	updateAnimationFrame(d_time, 100, 5);
	
	if (justShot)
	{
		if ( timeSinceShot > 300 )
		{
			timeSinceShot = 0;
			justShot = false;
		}
		else
			timeSinceShot += d_time;
	}
}

bool Player::collidingWithPlanet(double planetCollisionRadius)
{
	return ( state == Midair && planetCollisionRadius + collisionRadius > r )? 1:0;
}

void Player::collideWithPlanet(double planetCollisionRadius)
{
	state = Grounded;
	rVel = 0;
	rVelDirection = 0;
	r = collisionRadius + planetCollisionRadius;
}

void Player::jump()
{
	if (state == Grounded)
	{
		state = Midair;
		rVel = 0.5;
		rVelDirection = +1;
	}
}

int Player::getNumKills(){return numKills;}
void Player::addKill(){ numKills++; }
bool Player::getShootingRight() {return shootingRight;}
void Player::setShootingRight(bool shootingRightToSet) { shootingRight = shootingRightToSet;}
bool Player::getRunningRight() {return runningRight;}
void Player::setRunningRight(bool runningRightToSet) { runningRight = runningRightToSet;}
bool Player::ifMidair() { return (state == Midair); }
bool Player::ifGrounded() { return (state == Grounded); }
bool Player::ifRunning() { return (state == Running); }
bool Player::ifJustShot() { return justShot; }
void Player::shoot(int dir)
{
	shootingRight = (dir == +1)? true: false;
	justShot = true;
	timeSinceShot = 0;
}