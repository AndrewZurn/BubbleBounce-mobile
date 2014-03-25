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
  GameScene *backgroundLayer = GameScene::create();
  
  CCScene *scene = CCScene::create();
  scene->addChild(backgroundLayer);

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
  
  this->setTouchEnabled(true);
  this->retain();
  
  Ball* firstBall = NULL;
  Ball* secondBall = NULL;
  std::vector<Ball*> ballArray;
  for (int i = 0; i < STARTING_BALLS / 2; i++) {
    firstBall = Ball::createBall(ballArray, "");
    firstBall->retain();
    ballArray.push_back(firstBall);
    this->addChild(firstBall, ZIndexBalls);
    
    secondBall = Ball::createBall(ballArray, firstBall->getOriginalBallImage());
    secondBall->retain();
    ballArray.push_back(secondBall);
    this->addChild(secondBall, ZIndexBalls);
  }
  this->setBallArray(ballArray);
  
  this->addChild(background, ZIndexBackground);
  
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

//TODO: Need to remove the balls from ballArray when they are removed from screen.
void GameScene::handleBallTouch(cocos2d::CCTouch *touch) {
  std::vector<Ball*>::iterator i;
  std::vector<Ball*> ballArray = this->getBallArray();
  Ball* ball;
  
  //for each ball in game
  for( i = ballArray.begin(); i != ballArray.end(); i++) {
    ball = (Ball *) (*i);
    
    if (ball == this->getSelectedBall()) break;
    
    if (ball->boundingBox().containsPoint(this->convertTouchToNodeSpace(touch))) {
      std::cout << "Touched Color: " << ball->getOriginalBallImage() << std::endl;
      //if no ball was previously touched
      if ( this->getSelectedBall() == NULL ) {
        ball->changeBallImage();
        this->setSelectedBall(ball);
        break;
      }
      else { //a ball was previously touched
        //if the colors match
        std::cout << "Selected Color: " << this->getSelectedBall()->getOriginalBallImage() << std::endl;
        if ( ball->compareColor(this->getSelectedBall())) {
          this->removeChild(ball);
          ball->release();
          this->removeChild(this->getSelectedBall());
          this->getSelectedBall()->release();
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
  ball = NULL;
  this->retain();
}

///////////////////////////////
// ---- PRIVATE METHODS ---- //
//////////////////////////////