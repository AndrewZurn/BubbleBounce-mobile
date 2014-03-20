#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"

class Ball : public cocos2d::CCSprite {
  
  
private:
  //file names for the ball images
  const char* _ball;
  const char* _ballSelected;
  
  //ball attributes
  int _x;
  int _y;
  int _xVelocity;
  int _yVelocity;
  int _radius;
  const char* _ballColor;
  
  //need list of balls for collisionArray
  std::vector<Ball> _collisionArray;
  
  //game attributes
  int _displayHeight;
  int _displayWidth;
  int _speed;
  
  //game functionalities
  static cocos2d::CCPoint getRandomPoint();
  static int getRandomVelocity();
  static const char* getRandomBallColor();
  static const char* getSelectedBallColor(const char* ball);
  static const char* getBallColorFromFileName(const char* ball);
  
  
public:
  static Ball* createBall(int displayWidth, int displayHeight);
  
  void updateBallPositions(std::vector<Ball> ballList);
  void detectCollisions(std::vector<Ball> ballList);
  float distanceTo(Ball thisBall, Ball otherBall);
  void calculateNewVelocities(Ball thisBall, Ball otherBall);
  
  bool compareColor(Ball otherBall);
  
  //default getters
  int getX() { return _x; }
  int getY() { return _y; }
  int getXVelocity() { return _xVelocity; }
  int getYVelocity() { return _yVelocity; }
  int getRadius() { return _radius; }
  const char* getBallColor() { return _ballColor; }
  std::vector<Ball> getCollisionArray() { return _collisionArray; }
  int getDisplayWidth() { return _displayWidth; }
  int getDisplayHeight() { return _displayHeight; }
	
  //default setters
  void setX(int x) { _x = x; }
  void setXVelocity(int xVelocity) { _xVelocity = xVelocity; }
  void setY(int y) { _y = y; }
  void setYVelocity(int yVelocity) { _yVelocity = yVelocity; }
  void setDisplayHeight(int displayHeight) { _displayHeight = displayHeight; }
  void setDisplayWidth(int displayWidth) { _displayWidth = displayWidth; }
  void setBallColor(const char* ballColor) { _ballColor = ballColor; }
  void setBall(const char* ball) { _ball = ball; };
  void setBallSelected(const char* ballSelected) { _ballSelected = ballSelected; }
  void setRadius(int radius) { _radius = radius; }
};

#endif 