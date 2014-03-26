//
//  GameScene.h
//  BouncingBallsCPP
//
//  Created by Andrew Zurn on 3/19/14.
//
//

#ifndef BouncingBallsCPP_GameScene_h
#define BouncingBallsCPP_GameScene_h

#include "cocos2d.h"
#include "Ball.h"

class GameScene : public cocos2d::CCLayer
{
private:
  Ball* _selectedBall;
  
  std::vector<Ball*> _ballArray;
  //methods needed for game functionality
  
public:
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  virtual bool init();
  
  // there's no 'id' in cpp, so we recommend returning the class instance pointer
  static cocos2d::CCScene* scene();
  
  // implement the "static node()" method manually
  CREATE_FUNC(GameScene);
  
  //touch handles
//  virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *event);
//  virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *event);
  virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *event);
  void handleBallTouch(cocos2d::CCTouch* touch);
  
  Ball* getSelectedBall() { return _selectedBall; }
  
  std::vector<Ball*> getBallArray() { return _ballArray; }
  void setBallArray(std::vector<Ball*> ballArray) { _ballArray = ballArray; }
  void setSelectedBall(Ball* ball) { _selectedBall = ball; }
};

#endif
