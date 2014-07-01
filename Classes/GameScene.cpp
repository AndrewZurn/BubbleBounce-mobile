//
//  GameScene.cpp
//  BouncingBallsCPP
//
//  Created by Andrew Zurn on 3/19/14.
//
//
#include "GameScene.h"
#include "Constants.h"
#include "Ball.h"
#include "LossScene.h"

#include <sys/time.h>
#include "SimpleAudioEngine.h"

USING_NS_CC;

int nextBallId = 0;
static int STARTING_BALLS = 12;
int time_interval = 3750;
int addMoreBallsCount = 4;
static int BALL_COUNT_CEILING = 24;

static int LABEL_FONT_SIZE = 60;
static int POINTS_LABEL_FONT_SIZE = 60;
static int LABEL_MARGIN = 15;
static int LABEL_COLOR_R = 91;
static int LABEL_COLOR_B = 236;
static int LABEL_COLOR_G = 255;

//////////////////////////////////////////////////////////////////////////////////////////
// Creates the game layer, and adds it to this scene.
//////////////////////////////////////////////////////////////////////////////////////////
CCScene* GameScene::scene()
{
    GameScene* backgroundLayer = GameScene::create();

    CCScene* scene = CCScene::create();
    scene->addChild(backgroundLayer);

    return scene;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Creates the context of the game.  Adds background, the score text onto the
// game.
//  Also setups the game scheduling and other handlers.
//////////////////////////////////////////////////////////////////////////////////////////
bool GameScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    // setup game scheduling/handling/other attributes
    _score = 0;
    _modifier = 1;
    _lastPairMatched = false;
    this->_gameOver = false;
    this->_lastElapsedTime = getCurrentTime();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(
        "bubble_pop.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(
        "bubble_pop_2.mp3");
    this->setTouchEnabled(true);

    // add background
    CCSprite* background = CCSprite::create("background_2.png");
    background->setPosition(
        ccp(windowSize.width / 2 + origin.x, windowSize.height / 2 + origin.y));
    this->addChild(background, ZIndexBackground);

    // add score label
    char scoreText[25];
    sprintf(scoreText, " %d", _score);
    _scoreLabel = CCLabelTTF::create(scoreText, "Marker Felt.ttf", LABEL_FONT_SIZE);
    _scoreLabel->setColor(ccc3(LABEL_COLOR_R, LABEL_COLOR_B, LABEL_COLOR_G));
    _scoreLabel->setAnchorPoint(ccp(0, 0));
    _scoreLabel->cocos2d::CCNode::setPosition(
        ccp(LABEL_MARGIN, windowSize.height - topScreenAdjust()));
    this->addChild(_scoreLabel, ZIndexGameTextLabels);

    // add go image
    _goTextImage = CCMenuItemImage::create("text_go.png", "text_go.png", this, NULL);
    _goTextImage->setPosition(ccp(windowSize.width / 2, windowSize.height / 2));
    this->addChild(_goTextImage, ZIndexGoImage);

    // add initial balls
    _ballArray = *new std::vector<Ball*>();
    for (int i = 0; i < STARTING_BALLS / 2; i++) {
        createNewBalls();
    }

    this->schedule(schedule_selector(GameScene::GameUpdate), 0.01);

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Keeps track of moving the balls, tracking the time to add new balls,
// and watching the status of the game (if balls < ADD_MORE_BALLS_CEILING)
// the game will be over.
//////////////////////////////////////////////////////////////////////////////////////////
void GameScene::GameUpdate()
{
    if (!_gameOver) {
        removeGoLabel();

        // if too many balls on the screen, game over
        if (_ballArray.size() >= BALL_COUNT_CEILING) {
            _gameOver = true;
        } else if (_ballArray.size() == 0) {
            _lastElapsedTime = getCurrentTime();
            increaseGameDifficulty();
        } else if (didTimeElapse()) {
            increaseGameDifficulty();
        }
        // Update all the balls positions (animate the balls)
        updateBallPositions();
    } else { // GAME OVER
        CCScene* lossScene = LossScene::scene(_score);
        CCDirector::sharedDirector()->replaceScene(
            CCTransitionFade::create(2, lossScene));
        CCDirector::sharedDirector()->retain();
        resetGame();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Looks for when the user's fingers leaves the screen.  Will make a
//  call to the handleBallTouch in the event of the user pressing the screen.
//////////////////////////////////////////////////////////////////////////////////////////
void GameScene::ccTouchesEnded(cocos2d::CCSet* pTouches,
                               cocos2d::CCEvent* event)
{
    CCSetIterator i;
    CCTouch* touch;

    // for every touch
    for (i = pTouches->begin(); i != pTouches->end(); i++) {
        touch = (CCTouch*)(*i);
        if (touch) { // if touch was found
            handleBallTouch(touch);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Handles the touching of a ball.
//  If the ball was already the previously selected ball then do nothing.
//  Else if its a valid other ball, and the ball colors are equal then remove
// them.
//    Else if they don't compare then deselect them and remove any holding of
//    the selected ball information.
//  Then update the score of the screen.
//////////////////////////////////////////////////////////////////////////////////////////
void GameScene::handleBallTouch(cocos2d::CCTouch* touch)
{

    std::vector<Ball*>::iterator i;
    Ball* ball;
    for (i = _ballArray.begin(); i != _ballArray.end(); i++) {
        ball = (Ball*)(*i);

        // the tapped ball was the already selected ball
        if (ball->getState() == BallSelected)
            continue; // do not change

        if (ball->boundingBox().containsPoint(
                this->convertTouchToNodeSpace(touch))) {
            if (this->getSelectedBall() == NULL) { // if no ball was previously
                // touched
                ball->changeBallImage();
                this->setSelectedBall(ball);
                break;
            } else { // a ball was previously touched
                if (ball->compareColor(this->getSelectedBall())) {
                    updateModifierAndText(true);
                    updateGameScoreAndText();
                    popBalls(ball, i);
                    break;
                } else { // the colors don't match
                    updateModifierAndText(false);
                    this->getSelectedBall()->changeBallImage();
                    this->setSelectedBall(NULL);
                    break;
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Create the first ball, giving it the next ballId, then
// create the second ball, and add both of them to the ball array.
//////////////////////////////////////////////////////////////////////////////////////////
void GameScene::createNewBalls()
{
    Ball* firstBall = Ball::createBall(_ballArray, nextBallId++, "");
    Ball* secondBall = Ball::createBall(_ballArray, nextBallId++,
                                        firstBall->getOriginalBallImage());

    _ballArray.push_back(firstBall);
    _ballArray.push_back(secondBall);

    this->addChild(firstBall, ZIndexBalls);
    this->addChild(secondBall, ZIndexBalls);
}

void GameScene::updateBallPositions()
{
    std::vector<Ball*>::iterator iterator;
    for (iterator = _ballArray.begin(); iterator != _ballArray.end();
         iterator++) {
        Ball* ball = *iterator;
        ball->updateBallPositions(_ballArray);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Randomly add to the addMoreBallsCount variable.
// Can subtract 1, add 1, or add 2 to the current count.
// Also subtract 50ms from the interval time.
//////////////////////////////////////////////////////////////////////////////////////////
void GameScene::increaseGameDifficulty()
{
    for (int i = 0; i < addMoreBallsCount / 2; i++) {
        createNewBalls();
    }

    double random = ((double)rand() / (RAND_MAX)); // number between 0 and 1
    if (random <= 0.30 && addMoreBallsCount > 4) {
        addMoreBallsCount = addMoreBallsCount - 2;
    } else if (random <= 0.66 && addMoreBallsCount > 8) {
        addMoreBallsCount = addMoreBallsCount + 2;
    } else if (random <= 1 && addMoreBallsCount > 8) {
        addMoreBallsCount = addMoreBallsCount + 4;
    } else {
        addMoreBallsCount = addMoreBallsCount + 2;
    }

    time_interval = time_interval - 15;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Given a ball and it's position in the ballArray, pop that ball,
// and it's corresponding selectedBall, and remove it from the ballArray.
//////////////////////////////////////////////////////////////////////////////////////////
void GameScene::popBalls(Ball* ball,
                         std::vector<Ball*>::iterator indexOfBall)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
        getRandomPopSound());
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
        getRandomPopSound());

    ballPopExplosion(ball);
    ballPopExplosion(this->getSelectedBall());

    this->removeChild(ball);
    this->removeChild(this->getSelectedBall());

    _ballArray.erase(indexOfBall);
    std::vector<Ball*>::iterator j; // find and erase the other ball
    for (j = _ballArray.begin(); j != _ballArray.end(); j++) {
        Ball* otherBall = (Ball*)(*j);
        if (otherBall->getBallId() == this->getSelectedBall()->getBallId()) {
            _ballArray.erase(j);
            break;
        }
    }
    this->setSelectedBall(NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Checks the current time against the last elapsed time,
// if it is greater than the TIME_INTERVAL then add new balls to the game.
//////////////////////////////////////////////////////////////////////////////////////////
bool GameScene::didTimeElapse()
{
    long currentTime = getCurrentTime();
    long lastElapsedTime = _lastElapsedTime;

    if (currentTime - lastElapsedTime > time_interval) {
        _lastElapsedTime = getCurrentTime();
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Private method to return the current time of the in milliseconds.
//////////////////////////////////////////////////////////////////////////////////////////
long GameScene::getCurrentTime()
{
    timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

const char* GameScene::getRandomPopSound()
{
    double random = ((double)rand() / (RAND_MAX));

    if (random <= 0.5) {
        return "bubble_pop.mp3";
    } else {
        return "bubble_pop_2.mp3";
    }
}

void GameScene::resetGame()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
        getRandomPopSound());
    this->removeAllChildren();
    // this->cleanup(); //using this would stop the multiple ball pops, and would
    // actually make nice trans to next scene
    _ballArray.clear();

    nextBallId = 0;
    addMoreBallsCount = 4;
    time_interval = 3750;
}

void GameScene::ballPopExplosion(Ball* ball)
{
    ccColor4F effectColor;
    ccColor3B textColor;
    const char* color = ball->getBallColor();
    if (strcmp(color, "blue") == 0) {
        effectColor.r = 18.0f / 255.0f;
        effectColor.g = 40.0f / 255.0f;
        effectColor.b = 243.0f / 255.0f;
        effectColor.a = 1.0f;
        textColor = ccc3(243, 221, 18);
    } else if (strcmp(color, "orange") == 0) {
        effectColor.r = 243.0f / 255.0f;
        effectColor.g = 108.0f / 255.0f;
        effectColor.b = 18.0f / 255.0f;
        effectColor.a = 1.0f;
        textColor = ccc3(18, 153, 243);
    } else if (strcmp(color, "pink") == 0) {
        effectColor.r = 198.0f / 255.0f;
        effectColor.g = 17.0f / 255.0f;
        effectColor.b = 235.0f / 255.0f;
        effectColor.a = 1.0f;
        textColor = ccc3(54, 235, 17);
    } else if (strcmp(color, "red") == 0) {
        effectColor.r = 235.0f / 255.0f;
        effectColor.g = 17.0f / 255.0f;
        effectColor.b = 17.0f / 255.0f;
        effectColor.a = 1.0f;
        textColor = ccc3(17, 235, 235);
    } else if (strcmp(color, "yellow") == 0) {
        effectColor.r = 236.0f / 255.0f;
        effectColor.g = 243.0f / 255.0f;
        effectColor.b = 18.0f / 255.0f;
        effectColor.a = 1.0f;
        textColor = ccc3(25, 18, 243);
    }

    CCParticleExplosion* popEffect = CCParticleExplosion::createWithTotalParticles(75);
    popEffect->setStartColor(effectColor);
    popEffect->setEndColor(effectColor);
    popEffect->setLife(0.35);
    popEffect->setPosition(ccp(ball->getX(), ball->getY()));
    this->addChild(popEffect);

    char pointsEarnedArray[6];
    sprintf(pointsEarnedArray, "+%d", getPointsEarned());
    CCLabelTTF* pointsEarnedLabel = CCLabelTTF::create(
        pointsEarnedArray, "Marker Felt.ttf", POINTS_LABEL_FONT_SIZE);
    pointsEarnedLabel->setColor(textColor);
    pointsEarnedLabel->setPosition(ccp(ball->getX(), ball->getY()));
    this->addChild(pointsEarnedLabel, ZIndexGameTextLabels);

    CCAction* fadeOut = CCFadeOut::create(1.5);
    pointsEarnedLabel->runAction(fadeOut);
    pointsEarnedLabel = NULL;
}

void GameScene::removeGoLabel()
{
    if (_goTextImage != NULL) {
        CCAction* fadeOut = CCFadeOut::create(2.25);
        _goTextImage->runAction(fadeOut);
        _goTextImage = NULL;
    }
}

int GameScene::getPointsEarned()
{
    return (10 * _modifier) + ((getCurrentTime() - _lastElapsedTime) * 0.1);
}

void GameScene::updateGameScoreAndText()
{
    _score = _score + getPointsEarned();
    char scoreText[25];
    sprintf(scoreText, " %d", _score);
    _scoreLabel->setString(scoreText);
}

int GameScene::topScreenAdjust()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return LABEL_FONT_SIZE * 2.75;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return LABEL_FONT_SIZE * 2.15;
#endif
}

void GameScene::updateModifierAndText(bool ballsMatched)
{
    if (ballsMatched) {
        if (_lastPairMatched) {
            _lastPairMatched = false;
            _modifier++;
        } else {
            _lastPairMatched = true;
        }
    } else {
        _modifier = 1;
    }
}