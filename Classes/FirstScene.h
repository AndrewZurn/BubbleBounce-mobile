//
//  FirstScene.h
//  BouncingBallsCPP
//
//  Created by Andrew Zurn on 3/19/14.
//
//

#ifndef BouncingBallsCPP_FirstScene_h
#define BouncingBallsCPP_FirstScene_h

#include "cocos2d.h"
#include "Ball.h"

class FirstScene : public cocos2d::CCLayer {
private:
    std::vector<Ball*> _ballArray;

    void GameUpdate();
    void createNewBalls();
    void popBall(Ball* ball, std::vector<Ball*>::iterator indexOfBall);
    void ballPopExplosion(Ball* ball);
    cocos2d::ccColor4F setExplosionColor(Ball* ball);
    void playButtonCallback(CCObject* pSender);
    void statsButtonCallback(CCObject* pSender);
    void helpButtonCallback(CCObject* pSender);
    const char* getRandomPopSound();

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    //touch handlers
    virtual void ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* event);
    void handleBallTouch(cocos2d::CCTouch* touch);

    // implement the "static node()" method manually
    CREATE_FUNC(FirstScene);

    std::vector<Ball*> getBallArray()
    {
        return _ballArray;
    }
    void setBallArray(std::vector<Ball*> ballArray)
    {
        _ballArray = ballArray;
    }
};

#endif
