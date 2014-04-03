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

CCScene* FirstScene::scene()
{
  FirstScene *backgroundLayer = FirstScene::create();
  
  CCScene *scene = CCScene::create();
  scene->addChild(backgroundLayer);
  
  return scene;
}

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

void FirstScene::GameUpdate() {
  //update all the balls positions (animate the balls)
  std::vector<Ball*>::iterator iterator;
  for(iterator = _ballArray.begin(); iterator != _ballArray.end(); iterator++) {
    Ball* ball = *iterator;
    ball->updateBallPositions(_ballArray);
  }
}

void FirstScene::createNewBalls() {
  Ball* firstBall = Ball::createBall(_ballArray, "");
  _ballArray.push_back(firstBall);
  this->addChild(firstBall, ZIndexBalls);
  
  Ball* secondBall = Ball::createBall(_ballArray, firstBall->getOriginalBallImage());
  _ballArray.push_back(secondBall);
  this->addChild(secondBall, ZIndexBalls);
}

void FirstScene::startGame(CCObject* pSender) {
  this->cleanup();
  //start the game scene
}