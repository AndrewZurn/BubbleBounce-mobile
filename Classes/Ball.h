#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"

class Ball : public cocos2d::CCSprite {
	private:
		cocos2d::CCSprite* ball;
		cocos2d::CCSprite* ballSelected;
		
		int x;
		int y;
		int xVelocity;
		int yVelocity;
		int radius; //can I get this by finding the size of CCSprite* ball?
		const* char color;
		
		//need list of balls for collisionArray
		//Ball[] collisionArray;
		
		int displayHeight;
		int displayWidth;
		int speed;
		
		Ball() { } //private default constructor
		
		void setRandomCoordinates(Ball[] otherBalls, int displayWidth,
				int displayHeight);
		float distanceTo(Ball thisBall, Ball otherBall);
		void calculateNewVelocities(Ball thisBall, Ball otherBall);
		
		
	public:
		Ball(CCSprite* ball, CCSprite* ballSelected, int x, int y,
				int radius, const* char color, int displayWidth, int displayHeight);
		
		Ball(CCSprite* ball, int displayWidth, int displayHeight);
		
		void updateBallPositions(Ball[] ballList);
		boolean compateColor(Ball otherBall);
		
		//default getters
		int getX() { return x; }
		int getY() { return y; }
		int getXVelocity() { return xVelocity; }
		int getYVelocity() { return yVelocity; }
		int getRadius() { return radius; }
		int getColor() { return color; }
	
		//default setters
		void setYVelocity();
		void setXVelocity();
		void setY();
		void setX( int x );
}

#endif 