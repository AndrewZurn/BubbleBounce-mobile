#include "Ball.h"

Ball::Ball(CCSprite* ball, CCSprite* ballSelected, int x, int y,
           int radius, std::string color, int displayWidth, int displayHeight) {
	
}

Ball::Ball(CCSprite* ball, int displayWidth, int displayHeight) {
	//TODO
}

//void Ball::updateBallPositions(Ball[] ballList) {
	//TODO
//}

bool Ball::compareColor(Ball otherBall) {
	//TODO
  return true;
}

void Ball::calculateNewVelocities(Ball thisBall, Ball otherBall) {
	//TODO
}

float Ball::distanceTo(Ball thisBall, Ball otherBall) {
	//TODO
  return 0.0;
}

//void Ball::setRandomCoordinates(Ball[] otherBalls,
//			int displayWidth, int displayHeight) {
	//TODO
//}

void Ball::setX(int x) { x = x; }
void Ball::setXVelocity(int xVelocity) { xVelocity = xVelocity; }
void Ball::setY(int y) { y = y; }
void Ball::setYVelocity(int yVelocity) { yVelocity = yVelocity; }
