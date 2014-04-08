//
//  LossScene.cpp
//  BouncingBallsCPP
//
//  Created by Andrew Zurn on 3/19/14.
//
//

#include "LossScene.h"
#include "Constants.h"
#include "GameScene.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////////////////////
// Adds the first scene layer to this scene.
//////////////////////////////////////////////////////////////////////////////////////////
CCScene* LossScene::scene()
{
  LossScene *backgroundLayer = LossScene::create();
  
  CCScene *scene = CCScene::create();
  scene->addChild(backgroundLayer);
  
  return scene;
}

bool LossScene::init() {
  if ( !CCLayer::init() ) {
    return false;
  }
  
  CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
  CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
  
  //add background
  CCSprite* background = CCSprite::create("background.png");
  background->setPosition(ccp(windowSize.width/2 + origin.x, windowSize.height/2 + origin.y));
  this->addChild(background, ZIndexBackground);
  
  //add start game button
  CCMenuItemImage* gameLogo = CCMenuItemImage::create("text_you_lose.png", "text_you_lose.png", this, NULL);
  CCMenuItemImage* playButton = CCMenuItemImage::create("play_again_button.png", "play_again_button_pressed.png",
                                                        this, menu_selector(LossScene::playButtonCallback));
  CCMenuItemImage* statsButton = CCMenuItemImage::create("stats_button.png", "stats_button_pressed.png",
                                                         this, menu_selector(LossScene::statsButtonCallback));
  CCMenu* buttonMenu = CCMenu::create(gameLogo, playButton, statsButton, NULL);
  buttonMenu->alignItemsVerticallyWithPadding(15);
  buttonMenu->setPosition(ccp(windowSize.width/2, windowSize.height/1.75));
  this->addChild(buttonMenu, ZIndexButtonMenu);
  
  this->setTouchEnabled(true);
  
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Called when the 'PLAY!' button is pressed, cleans up this scene,
// and then starts GameScene to start the actual game.
//////////////////////////////////////////////////////////////////////////////////////////
void LossScene::playButtonCallback(CCObject* pSender) {
  this->cleanup();
  
  CCScene* gameScene = GameScene::scene();
  CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, gameScene));
  CCDirector::sharedDirector()->retain();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Called when the 'STATS!' button is pressed, cleans up this scene,
// and then starts RankingsScene to to view the stats.
//////////////////////////////////////////////////////////////////////////////////////////
void LossScene::statsButtonCallback(CCObject* pSender) {
  std::cout << "STATS STATS AND MORE STATS!" << std::endl;
}
