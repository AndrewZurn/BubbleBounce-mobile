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

class GameScene : public cocos2d::CCLayer
{
private:
  
  
public:
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  virtual bool init();
  
  // there's no 'id' in cpp, so we recommend returning the class instance pointer
  static cocos2d::CCScene* scene();
  
  // a selector callback
  void menuCloseCallback(CCObject* pSender);
  
  std::string getRandomBallColor();
  
  // implement the "static node()" method manually
  CREATE_FUNC(GameScene);
};

#endif