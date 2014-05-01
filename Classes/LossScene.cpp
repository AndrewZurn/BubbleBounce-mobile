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
#include "NativeUtils.h"
#include "PlayGameConstants.h"

USING_NS_CC;

int _score;
static int LABEL_FONT_SIZE = 65;

//////////////////////////////////////////////////////////////////////////////////////////
// Adds the first scene layer to this scene.
//////////////////////////////////////////////////////////////////////////////////////////
CCScene* LossScene::scene(int score)
{
  _score = score;
  
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
  CCSprite* background = CCSprite::create("background_2.png");
  background->setPosition(ccp(windowSize.width/2 + origin.x, windowSize.height/2 + origin.y));
  this->addChild(background, ZIndexBackground);
  
  //setup score label
  char scoreText[10];
  sprintf(scoreText, "Score: %d", _score);
  CCLabelTTF* scoreLabel = CCLabelTTF::create(scoreText, "Marker Felt", LABEL_FONT_SIZE);
  
  //add game buttons
  CCMenuItemImage* gameLogo = CCMenuItemImage::create("text_you_lose.png", "text_you_lose.png", this, NULL);
  CCMenuItemLabel* scoreImage = CCMenuItemLabel::create(scoreLabel, this, NULL);
  CCMenuItemImage* playButton = CCMenuItemImage::create("play_again_button.png", "play_again_button_pressed.png",
                                                        this, menu_selector(LossScene::playButtonCallback));
  CCMenuItemImage* statsButton = CCMenuItemImage::create("stats_button.png", "stats_button_pressed.png",
                                                         this, menu_selector(LossScene::statsButtonCallback));
  CCMenu* buttonMenu = CCMenu::create(gameLogo, scoreImage, playButton, statsButton, NULL);
  buttonMenu->alignItemsVerticallyWithPadding(15);
  buttonMenu->setPosition(ccp(windowSize.width/2, windowSize.height/1.85));
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
  NativeUtils::submitScore(LEADERBOARD, _score);
  NativeUtils::showLeaderboards();
}
