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
  
  Ball* ballSprite = NULL;
  std::vector<Ball*> ballArray;
  for (int i = 0; i < 10; i++) {
    ballSprite = Ball::createBall(ballArray, windowSize.width, windowSize.height);
    ballArray.push_back(ballSprite);
    this->addChild(ballSprite, ZIndexBalls);
  }
  this->setBallArray(ballArray);
  
  this->addChild(background, ZIndexBackground);
  
  this->setTouchEnabled(true);
  
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
    if (ball->boundingBox().containsPoint(this->convertTouchToNodeSpace(touch))) {
      std::cout << ball->getX() << ":" << ball->getY() << " ball color: " << ball->getBallColor() << std::endl;
      ball->changeBallImage();
    }
  }
}

///////////////////////////////
// ---- PRIVATE METHODS ---- //
//////////////////////////////