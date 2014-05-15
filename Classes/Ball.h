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
  const char* _ballColor; //the color of the ball ("orange", "red", etc.)
  int _state;
  bool _canCollide;
  
  //need list of balls for collisionArray
  std::vector<Ball*> _collisionArray;
  
  //game attributes
  int _displayWidth;
  int _displayHeight;
  int _progressBarHeight;
  int _topScreenAdjust;
  int _speed;
  
  //methods used within Ball to update various fields
  void detectCollisions(std::vector<Ball*> ballList);
  float distanceTo(Ball *thisBall, Ball *otherBall);
  void calculateNewVelocities(Ball* thisBall, Ball* otherBall);
  
  static void setNonOverlapRandomPoint(Ball* thisBall, std::vector<Ball*> otherBalls);
  static void setBallPositionToOnScreen(Ball* thisBall);
  
  static int getRandomVelocity();
  static const char* getRandomBallImage();
  static const char* getSelectedBallColor(const char* ball);
  static const char* getBallColorFromFileName(const char* ball);
  
  static int topScreenAdjust();
  
public:
  //constructors
  static Ball* createBall(std::vector<Ball*> otherBalls, int ballId, const char* original_color);
  
  //external methods to update ball fields
  void updateBallPositions(std::vector<Ball*> ballList);
  int changeBallImage();
  
  //comparison methods
  bool compareColor(Ball* otherBall);
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
  int getProgressBarHeight() { return _progressBarHeight; }
  const char* getSelectedBallImage() { return _ballSelected; }
  const char* getOriginalBallImage() { return _ball; }
  int getBallId() { return _ballId; }
  int getTopScreenAdjust() { return _topScreenAdjust; }
  bool getCanCollide() { return _canCollide; }
	
  //default setters
  void setX(int x) { _x = x; }
  void setXVelocity(int xVelocity) { _xVelocity = xVelocity; }
  void setY(int y) { _y = y; }
  void setYVelocity(int yVelocity) { _yVelocity = yVelocity; }
  void setDisplayHeight(int displayHeight) { _displayHeight = displayHeight; }
  void setDisplayWidth(int displayWidth) { _displayWidth = displayWidth; }
  void setProgressBarHeight(int progressBarHeight) { _progressBarHeight = progressBarHeight; }
  void setBallColor(const char* ballColor) { _ballColor = ballColor; }
  void setBall(const char* ball) { _ball = ball; };
  void setBallSelected(const char* ballSelected) { _ballSelected = ballSelected; }
  void setRadius(int radius) { _radius = radius; }
  void setState(int state) { _state = state; }
  void setCollisionArray(std::vector<Ball*> collisionArray) { _collisionArray = collisionArray; }
  void setBallId(int ballId) { _ballId = ballId; }
  void setTopScreenAdjust(int topScreenAdjust) { _topScreenAdjust = topScreenAdjust; }
  void setCanCollide(bool canCollide) { _canCollide = canCollide; }
};

#endif 