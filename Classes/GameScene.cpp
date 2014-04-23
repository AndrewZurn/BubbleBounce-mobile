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
#include "LossScene.h"

#include <sys/time.h>
#include "SimpleAudioEngine.h"

USING_NS_CC;

int nextBallId = 0;
static int STARTING_BALLS = 12;
int time_interval = 3750;
static int LABEL_FONT_SIZE = 65;
static int BALL_COUNT_CEILING = 26;
static int PROGRESS_OFFSET_Y = 50;
static int PROGRESS_OFFSET_X = 325;
int addMoreBallsCount = 4;

//////////////////////////////////////////////////////////////////////////////////////////
// Creates the game layer, and adds it to this scene.
//////////////////////////////////////////////////////////////////////////////////////////
CCScene* GameScene::scene()
{
  GameScene *backgroundLayer = GameScene::create();
  
  CCScene *scene = CCScene::create();
  scene->addChild(backgroundLayer);
  
  return scene;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Creates the context of the game.  Adds background,
//  the score text and the progress bar onto the game.
//  Also setups the game scheduling and other handlers.
//////////////////////////////////////////////////////////////////////////////////////////
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
  
  //add score label
  char scoreText[10];
  sprintf(scoreText, "Score: %d", _score);
  _scoreLabel = CCLabelTTF::create(scoreText, "Action Man", LABEL_FONT_SIZE);
  _scoreLabel->setAnchorPoint(ccp(0,0));
  _scoreLabel->cocos2d::CCNode::setPosition(ccp(15, windowSize.height - LABEL_FONT_SIZE*1.2));
  this->addChild(_scoreLabel, ZIndexScoreLabel);
  
  //add progress bar
  _progressBar = CCProgressTimer::create( CCSprite::create("progress_bar.png"));
  if ( _progressBar != NULL ) {
    _progressBar->setType(kCCProgressTimerTypeBar);
    _progressBar->setMidpoint(ccp(0,0));
    _progressBar->setBarChangeRate(ccp(1, 0));
    _progressBar->setPercentage( ((float) _ballArray.size()/BALL_COUNT_CEILING) * 100);
    _progressBar->setPosition(ccp(PROGRESS_OFFSET_X, PROGRESS_OFFSET_Y));
    this->addChild(_progressBar, ZIndexProgressBar);
  }
  
  //add go image
  _goTextImage = CCMenuItemImage::create("text_go.png", "text_go.png", this, NULL);
  _goTextImage->setPosition(ccp(windowSize.width/2, windowSize.height/2));
  this->addChild(_goTextImage, ZIndexGoImage);
  
  //add initial balls
  _ballArray = *new std::vector<Ball*>();
  for (int i = 0; i < STARTING_BALLS / 2; i++) {
    createNewBalls();
  }
  
  //setup game scheduling/handling/other attributes
  this->_gameOver = false;
  this->_lastElapsedTime = getCurrentTime();
  CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bubble_pop.mp3");
  
  this->setTouchEnabled(true);
  this->schedule(schedule_selector(GameScene::GameUpdate));
  
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Keeps track of moving the balls, tracking the time to add new balls,
// and watching the status of the game (if balls < ADD_MORE_BALLS_CEILING)
// the game will be over.
//////////////////////////////////////////////////////////////////////////////////////////
void GameScene::GameUpdate() {
  if (_gameOver == false) {
    
    //if too many balls on the screen, game over
    if (_ballArray.size() >= BALL_COUNT_CEILING ) {
      _gameOver = true;
    }
    else if ( didTimeElapse() ) {
      
      //add new balls to the screen
      for (int i = 0; i < addMoreBallsCount / 2; i++) {
        createNewBalls();
        _progressBar->setPercentage(((float) _ballArray.size()/BALL_COUNT_CEILING) * 100);
      }
      increaseAddBallCount();
      time_interval = time_interval - 50;
      
      //remove the GO! image from the screen
      if ( _goTextImage != NULL ) {
        CCAction* fadeOut = CCFadeOut::create(0.25);
        _goTextImage->runAction(fadeOut);
        _goTextImage = NULL;
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
      CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("bubble_pop.mp3");
    }
    _ballArray.clear();
    
    CCScene* lossScene = LossScene::scene(_score);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, lossScene));
    CCDirector::sharedDirector()->retain();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Looks for when the user's fingers leaves the screen.  Will make a
//  call to the handleBallTouch in the event of the user pressing the screen.
//////////////////////////////////////////////////////////////////////////////////////////
void GameScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *event) {
  CCSetIterator i;
  CCTouch* touch;
  
  //for every touch
  for (i = pTouches->begin(); i != pTouches->end(); i++) {
    touch = (CCTouch *) (*i);
    if (touch) { //if touch was found
      handleBallTouch(touch);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Handles the touching of a ball.
//  If the ball was already the previously selected ball then do nothing.
//  Else if it wasn't touched, and the ball colors are equal then remove them.
//    Else if they don't compare then deselect them and remove any holding of
//    the selected ball information.
//  Then update the score of the screen.
//////////////////////////////////////////////////////////////////////////////////////////
void GameScene::handleBallTouch(cocos2d::CCTouch *touch) {
  
  std::vector<Ball*>::iterator i;
  Ball* ball;
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
          _score++;
          CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("bubble_pop.mp3");
          
          this->removeChild(ball);
          this->removeChild(this->getSelectedBall());
          _ballArray.erase(i);
          std::vector<Ball*>::iterator j;  //find and erase the other ball
          for (j = _ballArray.begin(); j != _ballArray.end(); j++) {
            Ball* otherBall = (Ball*) (*j);
            if ( otherBall->getBallId() == this->getSelectedBall()->getBallId() ) {
              _ballArray.erase(j);
              break;
            }
          }
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
  char scoreText[10];
  sprintf(scoreText, "Score: %d", _score);
  _scoreLabel->setString(scoreText);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Create the first ball, giving it the next ballId, then
// create the second ball, and add both of them to the ball array.
//////////////////////////////////////////////////////////////////////////////////////////
void GameScene::createNewBalls() {
  Ball* firstBall = Ball::createBall(_ballArray, nextBallId++, "");
  firstBall->setProgressBarHeight(_progressBar->getSprite()->getTexture()->getPixelsHigh() + PROGRESS_OFFSET_Y/2);
  Ball* secondBall = Ball::createBall(_ballArray, nextBallId++, firstBall->getOriginalBallImage());
  secondBall->setProgressBarHeight(_progressBar->getSprite()->getTexture()->getPixelsHigh() + PROGRESS_OFFSET_Y/2);
  
  _ballArray.push_back(firstBall);
  _ballArray.push_back(secondBall);
  
  this->addChild(firstBall, ZIndexBalls);
  this->addChild(secondBall, ZIndexBalls);
}

void GameScene::increaseAddBallCount() {
  double random = ((double) rand() / (RAND_MAX)); //number between 0 and 1
  
  if ( random <= 0.33 ) {
    addMoreBallsCount--;
  }
  else if ( random <= 0.66 ) {
    addMoreBallsCount++;
  }
  else if ( random <= 1 ) {
    addMoreBallsCount = addMoreBallsCount + 2;
  }
  else {
    addMoreBallsCount++;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Checks the current time against the last elapsed time,
// if it is greater than the TIME_INTERVAL then add new balls to the game.
//////////////////////////////////////////////////////////////////////////////////////////
bool GameScene::didTimeElapse() {
  long currentTime = getCurrentTime();
  long lastElapsedTime = _lastElapsedTime;
  
  if ( currentTime - lastElapsedTime > time_interval ) {
    _lastElapsedTime = getCurrentTime();
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Private method to return the current time of the in milliseconds.
//////////////////////////////////////////////////////////////////////////////////////////
long GameScene::getCurrentTime() {
  timeval time;
  gettimeofday(&time, NULL);
  return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}
