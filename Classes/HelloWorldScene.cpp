#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
  srand (time(NULL));
  
  // 'scene' is an autorelease object
  CCScene *scene = CCScene::create();
  
  HelloWorld *backgroundLayer = HelloWorld::create();
  
  // add layers as a child to scene
  scene->addChild(backgroundLayer);
  
  // return the scene
  return scene;
}

bool HelloWorld::init() {
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
    std::cout << randomX << " : " << randomY << std::endl;
    ballSprite = CCSprite::create(getRandomBallColor().c_str());
    ballSprite->setPosition(ccp(randomX, randomY));
    this->addChild(ballSprite, 1);
  }
  
  this->addChild(background, 0);
  
  return true;
}

std::string HelloWorld::getRandomBallColor() {
  double random = ((double) rand() / (RAND_MAX));
  
  std::cout << "random number: " << random << std::endl;
  
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

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
  CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0);
#endif
#endif
}
