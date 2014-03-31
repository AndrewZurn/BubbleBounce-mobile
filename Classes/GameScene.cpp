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
std::vector<Ball*> thisBallArray;

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
  thisBallArray = *new std::vector<Ball*>();
  for (int i = 0; i < STARTING_BALLS / 2; i++) {
    firstBall = Ball::createBall(thisBallArray, "");
    firstBall->retain();
    thisBallArray.push_back(firstBall);
    this->addChild(firstBall, ZIndexBalls);
    
    secondBall = Ball::createBall(thisBallArray, firstBall->getOriginalBallImage());
    secondBall->retain();
    thisBallArray.push_back(secondBall);
    this->addChild(secondBall, ZIndexBalls);
  }
  
  this->setBallArray(thisBallArray);
  this->addChild(background, ZIndexBackground);
  
  return true;
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
  Ball* ball;
  
  //for each ball in game
  for( i = thisBallArray.begin(); i != thisBallArray.end(); i++) {
    ball = (Ball *) (*i);
    
    //the tapped ball was the already selected ball
    if (ball->getState() == BallSelected) {
      continue; //should continue to the end
    }

    if ( ball->boundingBox().containsPoint(this->convertTouchToNodeSpace(touch)) ) {

      //if no ball was previously touched
      if ( this->getSelectedBall() == NULL ) {
        ball->changeBallImage();
        this->setSelectedBall(ball);
        break;
      }
      else { //a ball was previously touched
        if ( ball->compareColor(this->getSelectedBall()) ) {
          this->removeChild(ball);
          this->removeChild(this->getSelectedBall());
          
          thisBallArray.erase(i);
          thisBallArray.erase(std::find(thisBallArray.begin(), thisBallArray.end(), _selectedBall));
          
          ball->release();
          this->getSelectedBall()->release();
          this->setSelectedBall(NULL);
          
          _score++;
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
  std::cout << "Score: " << _score << std::endl;
}