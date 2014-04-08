//
//  LossScene.h
//  BouncingBallsCPP
//
//  Created by Andrew Zurn on 3/19/14.
//
//

#ifndef BouncingBallsCPP_LossScene_h
#define BouncingBallsCPP_LossScene_h

#include "cocos2d.h"

class LossScene : public cocos2d::CCLayer
{
private:
  
  void playButtonCallback(CCObject* pSender);
  void statsButtonCallback(CCObject* pSender);
  
public:
  
  // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  virtual bool init();
  
  // there's no 'id' in cpp, so we recommend returning the class instance pointer
  static cocos2d::CCScene* scene();
  
  //touch handlers
//  virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *event);
  
  // implement the "static node()" method manually
  CREATE_FUNC(LossScene);
  
};
#endif
