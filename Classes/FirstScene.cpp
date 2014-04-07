//
//  FirstScene.cpp
//  BouncingBallsCPP
//
//  Created by Andrew Zurn on 3/19/14.
//
//

#include "FirstScene.h"
#include "Constants.h"

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
  CCMenuItemImage* playButton = CCMenuItemImage::create("play_button.png", "play_button_selected.png",
                                                        this, menu_selector(FirstScene::startGame));
  CCMenu* playMenu = CCMenu::create(playButton, NULL);
  playMenu->setPosition( ccp(windowSize.width/2, windowSize.height/2));
  
  this->schedule(schedule_selector(FirstScene::GameUpdate));
  
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
// Called when the 'PLAY' button is pressed, cleans up this scene,
// and then starts GameScene to start the actual game.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::startGame(CCObject* pSender) {
  this->cleanup();
  //start the game scene
}