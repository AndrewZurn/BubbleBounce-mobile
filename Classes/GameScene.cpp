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
static int TIME_INTERVAL = 5000;
static int LABEL_FONT_SIZE = 65;
static int BALL_COUNT_CEILING = 30;
static int ADD_MORE_BALLS_COUNT = 3;

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
  
  //add background
  CCSprite* background = CCSprite::create("background.png");
  background->setPosition(ccp(windowSize.width/2 + origin.x, windowSize.height/2 + origin.y));
  this->addChild(background, ZIndexBackground);
  
  //add initial balls
  _ballArray = *new std::vector<Ball*>();
  for (int i = 0; i < STARTING_BALLS / 2; i++) {
    createNewBalls();
  }
  this->setBallArray(_ballArray);
  
  //add score label
  char scoreText[10];
  sprintf(scoreText, "Score: %d", _score);
  _scoreLabel = CCLabelTTF::create(scoreText, "Action Man", LABEL_FONT_SIZE);
  _scoreLabel->setAnchorPoint(ccp(0,0));
  _scoreLabel->cocos2d::CCNode::setPosition(ccp(15, windowSize.height - LABEL_FONT_SIZE));
  this->addChild(_scoreLabel, ZIndexScoreLabel);
  
  //add progress bar
  _progressBar = CCProgressTimer::create( CCSprite::create("progress_bar.png"));
  if ( _progressBar != NULL ) {
    _progressBar->setType(kCCProgressTimerTypeBar);
    _progressBar->setMidpoint(ccp(0,0));
    _progressBar->setBarChangeRate(ccp(1, 0));
    _progressBar->setPercentage( ((float) _ballArray.size()/BALL_COUNT_CEILING) * 100);
    _progressBar->setPosition(ccp(325, 50));
    this->addChild(_progressBar, ZIndexProgressBar);
  }

  //setup game scheduling/handling
  this->_lastElapsedTime = getCurrentTime();
  this->setTouchEnabled(true);
  this->schedule(schedule_selector(GameScene::GameUpdate));
  
  return true;
}

void GameScene::GameUpdate() {
  if (!_gameOver) {
    if (_ballArray.size() >= BALL_COUNT_CEILING ) {
      _gameOver = true;
    }
    else if ( didTimeElapse() ) {
      for (int i = 0; i < ADD_MORE_BALLS_COUNT; i++) {
        createNewBalls();
        _progressBar->setPercentage( ((float) _ballArray.size()/BALL_COUNT_CEILING) * 100);
      }
    }
    //update all the balls positions (animate the balls)
    std::vector<Ball*>::iterator iterator;
    for(iterator = _ballArray.begin(); iterator != _ballArray.end(); iterator++) {
      Ball* ball = *iterator;
      ball->updateBallPositions(_ballArray);
      _progressBar->setPercentage( ((float) _ballArray.size()/BALL_COUNT_CEILING) * 100);
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
  char scoreText[10];
  sprintf(scoreText, "Score: %d", _score);
  _scoreLabel->setString(scoreText);
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
  
  if ( currentTime - lastElapsedTime > TIME_INTERVAL ) {
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
