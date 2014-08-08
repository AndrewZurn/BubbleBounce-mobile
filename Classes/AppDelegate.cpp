#include "AppDelegate.h"
#include "GameScene.h"
#include "FirstScene.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
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

    // get the screen size of the device
    CCSize screenSize = pEGLView->getFrameSize();
    std::cout << "INFO: Screen Size - " << screenSize.height << " x " << screenSize.width << std::endl;

    // check which resources should be used
    std::vector<std::string> resDirOrders;
    if (1920 <= screenSize.width || 1920 <= screenSize.height) { //TODO: MAKE SURE THIS IS CORRECT
        resDirOrders.push_back("HD");
        resDirOrders.push_back("MD");
        resDirOrders.push_back("SD");
    } else if (1200 <= screenSize.width || 1200 <= screenSize.height) {
        resDirOrders.push_back("MD");
        resDirOrders.push_back("SD");
    } else {
        resDirOrders.push_back("SD");
    }

    // set the resource directory for the resources
    CCFileUtils::sharedFileUtils()->setSearchPaths(resDirOrders);
    std::cout << "INFO: Resource Directory - " << resDirOrders.front() << std::endl;

    // create a scene. it's an autorelease object
    CCScene* pScene = FirstScene::scene();

    // run
    pDirector->runWithScene(pScene);

    setGameScene(pScene);
    pScene->retain();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call, it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
}
