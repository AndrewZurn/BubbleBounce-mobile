#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"

class Ball : public cocos2d::CCSprite {
  
  
private:
  int _ballId;
  
  //file names for the ball images
  const char* _ball;
  const char* _ballSelected;
  
  //ball attributes
  int _x;
  int _y;
  int _xVelocity;
  int _yVelocity;
  int _radius; //can I get this by finding the size of CCSprite* ball?
  const char* _ballColor;
  int _state;
  
  //need list of balls for collisionArray
  std::vector<Ball*> _collisionArray;
  
  //game attributes
  int _displayWidth;
  int _displayHeight;
  int _speed;
  
  static void setNonOverlapRandomPoint(Ball* thisBall, std::vector<Ball*> otherBalls);
  static void setBallPositionToOnScreen(Ball* thisBall);
  static int getRandomVelocity();
  static const char* getRandomBallColor();
  static const char* getSelectedBallColor(const char* ball);
  static const char* getBallColorFromFileName(const char* ball);
  
  
public:
  static Ball* createBall(std::vector<Ball*> otherBalls, int displayWidth, int displayHeight);
  
  void updateBallPositions(std::vector<Ball*> ballList);
  void detectCollisions(std::vector<Ball*> ballList);
  float distanceTo(Ball *thisBall, Ball *otherBall);
  void calculateNewVelocities(Ball* thisBall, Ball* otherBall);
  
  int changeBallImage();
  
  bool compareColor(Ball otherBall);
  bool operator==(const Ball &ball) const {
    return _x == ball._x && _y == ball._y && _ballColor == ball._ballColor;
  }
  
  //default getters
  int getX() { return _x; }
  int getY() { return _y; }
  int getXVelocity() { return _xVelocity; }
  int getYVelocity() { return _yVelocity; }
  int getRadius() { return _radius; }
  int getState() { return _state; }
  const char* getBallColor() { return _ballColor; }
  std::vector<Ball*> getCollisionArray() { return _collisionArray; }
  int getDisplayWidth() { return _displayWidth; }
  int getDisplayHeight() { return _displayHeight; }
  const char* getSelectedBallImage() { return _ballSelected; }
  const char* getOriginalBallImage() { return _ball; }
	
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
  void setState(int state) { _state = state; }
};

#endif 