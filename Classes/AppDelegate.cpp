#include "AppDelegate.h"
#include "GameScene.h"
#include "FirstScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
  
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
  //start here to insure random numbers are generated
  srand(time(NULL));
  
  // initialize director
  CCDirector* pDirector = CCDirector::sharedDirector();
  CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
  
  pDirector->setOpenGLView(pEGLView);
	
  // turn on display FPS
  pDirector->setDisplayStats(false);
  
  // set FPS. the default value is 1.0/60 if you don't call this
  pDirector->setAnimationInterval(1.0 / 60);
  
  // create a scene. it's an autorelease object
  CCScene *pScene = FirstScene::scene();
  
  // run
  pDirector->runWithScene(pScene);
  
  setGameScene(pScene);
  pScene->retain();
  
  return true;
}

// This function will be called when the app is inactive. When comes a phone call, it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
  CCDirector::sharedDirector()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
  CCDirector::sharedDirector()->startAnimation();
}
