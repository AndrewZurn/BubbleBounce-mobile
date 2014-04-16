//
//  FirstScene.cpp
//  BouncingBallsCPP
//
//  Created by Andrew Zurn on 3/19/14.
//
//

#include "FirstScene.h"
#include "GameScene.h"
#include "AppDelegate.h"
#include "Constants.h"
#include "SimpleAudioEngine.h"

#include "NativeUtils.h"

USING_NS_CC;

static int STARTING_BALLS = 10;

//////////////////////////////////////////////////////////////////////////////////////////
// Adds the first scene layer to this scene.
//////////////////////////////////////////////////////////////////////////////////////////
CCScene* FirstScene::scene()
{
  FirstScene *backgroundLayer = FirstScene::create();
  
  CCScene *scene = CCScene::create();
  scene->addChild(backgroundLayer);
  
  return scene;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Creates the background layer, adds the initial balls to the game,
// adds the buttons to the screen, and adds the timing scheduler.
//////////////////////////////////////////////////////////////////////////////////////////
bool FirstScene::init() {
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
  
  //add start game button
  CCMenuItemImage* gameLogo = CCMenuItemImage::create("bouncinballslogo.png", "bouncinballslogo.png", this, NULL);
  CCMenuItemImage* playButton = CCMenuItemImage::create("play_button.png", "play_button_pressed.png",
                                                        this, menu_selector(FirstScene::playButtonCallback));
  CCMenuItemImage* statsButton = CCMenuItemImage::create("stats_button.png", "stats_button_pressed.png",
                                                         this, menu_selector(FirstScene::statsButtonCallback));
  CCMenu* buttonMenu = CCMenu::create(gameLogo, playButton, statsButton, NULL);
  buttonMenu->alignItemsVerticallyWithPadding(15);
  buttonMenu->setPosition(ccp(windowSize.width/2, windowSize.height/1.75));
  this->addChild(buttonMenu, ZIndexButtonMenu);
  
  this->setTouchEnabled(true);
  this->schedule(schedule_selector(FirstScene::GameUpdate));
  CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bubble_pop.mp3");
  
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the positions of all the balls in the game.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::GameUpdate() {
  //update all the balls positions (animate the balls)
  std::vector<Ball*>::iterator iterator;
  for(iterator = _ballArray.begin(); iterator != _ballArray.end(); iterator++) {
    Ball* ball = (Ball*) *iterator;
    ball->updateBallPositions(_ballArray);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Looks for when the user's fingers leaves the screen.  Will make a
//  call to the handleBallTouch in the event of the user pressing the screen.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *event) {
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

void FirstScene::handleBallTouch(cocos2d::CCTouch *touch) {
  std::vector<Ball*>::iterator i;
  Ball* ball;
  for( i = _ballArray.begin(); i != _ballArray.end(); i++) {
    ball = (Ball *) (*i);
    
    //the tapped ball was the already selected ball
    if (ball->getState() == BallSelected) {
      continue; //should continue to the end
    }
    
    if ( ball->boundingBox().containsPoint(this->convertTouchToNodeSpace(touch)) ) {
      this->removeChild(ball);
      
      _ballArray.erase(i);
      
      CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("bubble_pop.mp3");
      break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Create the first ball, giving it the next ballId, then
// create the second ball, and add both of them to the ball array.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::createNewBalls() {
  Ball* firstBall = Ball::createBall(_ballArray, 0, "");
  Ball* secondBall = Ball::createBall(_ballArray, 0, firstBall->getOriginalBallImage());
  
  _ballArray.push_back(firstBall);
  _ballArray.push_back(secondBall);
  
  this->addChild(firstBall, ZIndexBalls);
  this->addChild(secondBall, ZIndexBalls);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Called when the 'PLAY!' button is pressed, cleans up this scene,
// and then starts GameScene to start the actual game.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::playButtonCallback(CCObject* pSender) {
  this->cleanup();
  
  CCScene* gameScene = GameScene::scene();
  CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, gameScene));
  CCDirector::sharedDirector()->retain();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Called when the 'STATS!' button is pressed, cleans up this scene,
// and then starts RankingsScene to to view the stats.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::statsButtonCallback(CCObject* pSender) {
  NativeUtils::showLeaderboards();
}