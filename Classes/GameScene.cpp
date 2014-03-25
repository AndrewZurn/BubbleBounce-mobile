//
//  GameScene.cpp
//  BouncingBallsCPP
//
//  Created by Andrew Zurn on 3/19/14.
//
//
#include "GameScene.h"
#include "Constants.h"
#include "Ball.h"

USING_NS_CC;

static int STARTING_BALLS = 10;

CCScene* GameScene::scene()
{
  // 'scene' is an autorelease object
  CCScene *scene = CCScene::create();
  
  GameScene *backgroundLayer = GameScene::create();
  
  // add layers as a child to scene
  scene->addChild(backgroundLayer);
  
  // return the scene
  return scene;
}

bool GameScene::init() {
  if ( !CCLayer::init() ) {
    return false;
  }
  
  CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
  CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
  
  CCSprite* background = CCSprite::create("background.png");
  background->setPosition(ccp(windowSize.width/2 + origin.x, windowSize.height/2 + origin.y));
  
  Ball* firstBall = NULL;
  Ball* secondBall = NULL;
  std::vector<Ball*> ballArray;
  for (int i = 0; i < STARTING_BALLS / 2; i++) {
    firstBall = Ball::createBall(ballArray, "");
    ballArray.push_back(firstBall);
    this->addChild(firstBall, ZIndexBalls);
    
    secondBall = Ball::createBall(ballArray, firstBall->getOriginalBallImage());
    ballArray.push_back(secondBall);
    this->addChild(secondBall, ZIndexBalls);
  }
  this->setBallArray(ballArray);
  
  this->addChild(background, ZIndexBackground);
  
  this->setTouchEnabled(true);
  this->retain();
  
  return true;
}

void GameScene::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *event) {
  //not interested in?
}

void GameScene::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *event) {
  //not interested in?
}

void GameScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *event) {
  CCSetIterator i;
  CCTouch* touch;
  CCPoint tap;
  
  //for every touch
  for (i = pTouches->begin(); i != pTouches->end(); i++) {
    touch = (CCTouch *) (*i);
    
    if (touch) { //if touch was found
      handleBallTouch(touch);
    }
  }
  
}

void GameScene::handleBallTouch(cocos2d::CCTouch *touch) {
  std::vector<Ball*>::iterator i;
  std::vector<Ball*> ballArray = this->getBallArray();
  Ball* ball;
  
  //for each ball in game
  for( i = ballArray.begin(); i != ballArray.end(); i++) {
    ball = (Ball *) (*i);
    if (ball == this->getSelectedBall()) break;
    if (ball->boundingBox().containsPoint(this->convertTouchToNodeSpace(touch))) {
      
      //if no ball was previously touched
      if ( this->getSelectedBall() == NULL ) {
        ball->changeBallImage();
        this->setSelectedBall(ball);
        break;
      }
      else { //a ball was previously touched
        //if the colors match
        if ( ball->compareColor(this->getSelectedBall())) {
          this->removeChild(ball);
          this->removeChild(this->getSelectedBall());
          this->setSelectedBall(NULL);
          break;
        }
        else{ //the colors don't match
          this->getSelectedBall()->changeBallImage();
          this->setSelectedBall(NULL);
          break;
        }
      }
    }
  }
  this->retain();
}

///////////////////////////////
// ---- PRIVATE METHODS ---- //
//////////////////////////////