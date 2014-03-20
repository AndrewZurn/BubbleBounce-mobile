//
//  GameScene.cpp
//  BouncingBallsCPP
//
//  Created by Andrew Zurn on 3/19/14.
//
//
#include "GameScene.h"
#include "Constants.h"

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
  
  CCSprite* ballSprite = NULL;
  for (int i = 0; i < 10; i++) {
    int randomX = rand() % (int)(windowSize.width + 1);
    int randomY = rand() % (int)(windowSize.height + 1);
    
    ballSprite = CCSprite::create(getRandomBallColor().c_str());
    ballSprite->setPosition(ccp(randomX, randomY));
    this->addChild(ballSprite, ZIndexBalls);
  }
  
  this->addChild(background, ZIndexBackground);
  
  return true;
}

std::string GameScene::getRandomBallColor() {
  double random = ((double) rand() / (RAND_MAX));
  
  if( random <= 0.2 ) {
    return "blueball.png";
  }
  else if( random <= 0.4 ) {
    return "orangeball.png";
  }
  else if( random <= 0.6 ) {
    return "pinkball.png";
  }
  else if( random <= 0.8 ) {
    return "redball.png";
  }
  else {
    return "blueball.png";
  }
}

void updateBallPositions() {
  
}

void detectBallToBallCollision() {
  
}

float distanceBallToBall() {
  
  return 0.0;
}

void updateBallVelocities() {
  
}