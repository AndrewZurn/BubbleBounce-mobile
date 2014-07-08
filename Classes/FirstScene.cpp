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

static int STARTING_BALLS = 14;
int ballId = 0;

//////////////////////////////////////////////////////////////////////////////////////////
// Adds the first scene layer to this scene.
//////////////////////////////////////////////////////////////////////////////////////////
CCScene* FirstScene::scene()
{
    FirstScene* backgroundLayer = FirstScene::create();

    CCScene* scene = CCScene::create();
    scene->addChild(backgroundLayer);

    return scene;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Creates the background layer, adds the initial balls to the game,
// adds the buttons to the screen, and adds the timing scheduler.
//////////////////////////////////////////////////////////////////////////////////////////
bool FirstScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    //add background
    CCSprite* background = CCSprite::create(BackgroundImage);
    background->setPosition(ccp(windowSize.width / 2 + origin.x, windowSize.height / 2 + origin.y));
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
    buttonMenu->setPosition(ccp(windowSize.width / 2, windowSize.height / 1.75));
    this->addChild(buttonMenu, ZIndexButtonMenu);

    this->setTouchEnabled(true);
    this->schedule(schedule_selector(FirstScene::GameUpdate));
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bubble_pop.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bubble_pop_2.mp3");

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the positions of all the balls in the game.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::GameUpdate()
{
    //update all the balls positions (animate the balls)
    std::vector<Ball*>::iterator iterator;
    for (iterator = _ballArray.begin(); iterator != _ballArray.end(); iterator++) {
        Ball* ball = (Ball*)*iterator;
        ball->updateBallPositions(_ballArray);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Looks for when the user's fingers leaves the screen.  Will make a
//  call to the handleBallTouch in the event of the user pressing the screen.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* event)
{
    CCSetIterator i;
    CCTouch* touch;

    //for every touch
    for (i = pTouches->begin(); i != pTouches->end(); i++) {
        touch = (CCTouch*)(*i);
        if (touch) { //if touch was found
            handleBallTouch(touch);
        }
    }
}

void FirstScene::handleBallTouch(cocos2d::CCTouch* touch)
{
    std::vector<Ball*>::iterator i;
    Ball* ball;
    for (i = _ballArray.begin(); i != _ballArray.end(); i++) {
        ball = (Ball*)(*i);

        //the tapped ball was the already selected ball
        if (ball->getState() == BallSelected)
            continue; //do not change

        if (ball->boundingBox().containsPoint(this->convertTouchToNodeSpace(touch))) {
            popBall(ball, i);
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Create the first ball, giving it the next ballId, then
// create the second ball, and add both of them to the ball array.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::createNewBalls()
{
    Ball* firstBall = Ball::createBall(_ballArray, ballId++, "");
    firstBall->setProgressBarHeight(0);
    Ball* secondBall = Ball::createBall(_ballArray, ballId++, firstBall->getOriginalBallImage());
    secondBall->setProgressBarHeight(0);

    _ballArray.push_back(firstBall);
    _ballArray.push_back(secondBall);

    this->addChild(firstBall, ZIndexBalls);
    this->addChild(secondBall, ZIndexBalls);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Given a ball and it's position in the ballArray, pop that ball.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::popBall(Ball* ball, std::vector<Ball*>::iterator indexOfBall)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(getRandomPopSound());

    ballPopExplosion(ball);

    this->removeChild(ball);
    _ballArray.erase(indexOfBall);
}

const char* FirstScene::getRandomPopSound()
{
    double random = ((double)rand() / (RAND_MAX));

    if (random <= 0.5) {
        return "bubble_pop.mp3";
    } else {
        return "bubble_pop_2.mp3";
    }
}

void FirstScene::ballPopExplosion(Ball* ball)
{
    CCParticleExplosion* popEffect = CCParticleExplosion::create();
    ccColor4F effectColor;

    const char* color = ball->getBallColor();
    if (strcmp(color, "blue") == 0) {
        effectColor.r = 18.0f / 255.0f;
        effectColor.g = 40.0f / 255.0f;
        effectColor.b = 243.0f / 255.0f;
        effectColor.a = 1.0f;
    } else if (strcmp(color, "orange") == 0) {
        effectColor.r = 243.0f / 255.0f;
        effectColor.g = 108.0f / 255.0f;
        effectColor.b = 18.0f / 255.0f;
        effectColor.a = 1.0f;
    } else if (strcmp(color, "pink") == 0) {
        effectColor.r = 198.0f / 255.0f;
        effectColor.g = 17.0f / 255.0f;
        effectColor.b = 235.0f / 255.0f;
        effectColor.a = 1.0f;
    } else if (strcmp(color, "red") == 0) {
        effectColor.r = 235.0f / 255.0f;
        effectColor.g = 17.0f / 255.0f;
        effectColor.b = 17.0f / 255.0f;
        effectColor.a = 1.0f;
    } else if (strcmp(color, "yellow") == 0) {
        effectColor.r = 236.0f / 255.0f;
        effectColor.g = 243.0f / 255.0f;
        effectColor.b = 18.0f / 255.0f;
        effectColor.a = 1.0f;
    }

    popEffect->setStartColor(effectColor);
    popEffect->setEndColor(effectColor);
    popEffect->setTotalParticles(75);
    popEffect->setLife(0.15);
    popEffect->setPosition(ccp(ball->getX(), ball->getY()));

    this->addChild(popEffect);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Called when the 'PLAY!' button is pressed, cleans up this scene,
// and then starts GameScene to start the actual game.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::playButtonCallback(CCObject* pSender)
{
    this->cleanup();

    CCScene* gameScene = GameScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, gameScene));
    CCDirector::sharedDirector()->retain();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Called when the 'STATS!' button is pressed, cleans up this scene,
// and then starts RankingsScene to to view the stats.
//////////////////////////////////////////////////////////////////////////////////////////
void FirstScene::statsButtonCallback(CCObject* pSender)
{
    NativeUtils::showLeaderboards();
}