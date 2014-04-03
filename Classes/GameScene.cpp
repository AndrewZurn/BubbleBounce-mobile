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
#include <sys/time.h>

USING_NS_CC;

static int STARTING_BALLS = 12;

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
  this->addChild(background, ZIndexBackground);
  
  _ballArray = *new std::vector<Ball*>();
  for (int i = 0; i < STARTING_BALLS / 2; i++) {
    createNewBalls();
  }
  this->setBallArray(_ballArray);
  
  this->_lastElapsedTime = getCurrentTime();
  this->setTouchEnabled(true);
  this->schedule(schedule_selector(GameScene::GameUpdate));
  
  return true;
}

void GameScene::GameUpdate() {
  if (!_gameOver) {
    if (_ballArray.size() > 36 ) {
      _gameOver = true;
    }
    else if ( didTimeElapse() ) {
      for (int i = 0; i < 5; i++) {
        createNewBalls();
      }
    }
    //update all the balls positions (animate the balls)
    std::vector<Ball*>::iterator iterator;
    for(iterator = _ballArray.begin(); iterator != _ballArray.end(); iterator++) {
      Ball* ball = *iterator;
      ball->updateBallPositions(_ballArray);
    }
  }
  else { //game over
    std::vector<Ball*>::iterator iterator;
    for(iterator = _ballArray.begin(); iterator != _ballArray.end(); iterator++) {
      Ball* ball = *iterator;
      this->removeChild(ball);
    }
    _ballArray.clear();
    //go to you lose screen
  }
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
  Ball* ball;
  
  //for each ball in game
  for( i = _ballArray.begin(); i != _ballArray.end(); i++) {
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
          
          _ballArray.erase(i);
          _ballArray.erase(std::find(_ballArray.begin(), _ballArray.end(), _selectedBall));
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

void GameScene::createNewBalls() {
  Ball* firstBall = Ball::createBall(_ballArray, "");
  _ballArray.push_back(firstBall);
  this->addChild(firstBall, ZIndexBalls);
  
  Ball* secondBall = Ball::createBall(_ballArray, firstBall->getOriginalBallImage());
  _ballArray.push_back(secondBall);
  this->addChild(secondBall, ZIndexBalls);
}

bool GameScene::didTimeElapse() {
  long currentTime = getCurrentTime();
  long lastElapsedTime = _lastElapsedTime;
  
  if ( currentTime - lastElapsedTime > 5000 ) {
    _lastElapsedTime = getCurrentTime();
    return true;
  }
  return false;
}

long GameScene::getCurrentTime() {
  timeval time;
  gettimeofday(&time, NULL);
  return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}
