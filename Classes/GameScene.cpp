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
#include "GameUtils.h"

#include <sys/time.h>
#include <unistd.h>
#include "SimpleAudioEngine.h"

USING_NS_CC;

int nextBallId = 0;
int _pairsMatched = 0;
static int STARTING_BALLS = 10;
int time_interval = 2800;
int penaltyTime = 0;
int addMoreBallsCount = 1;
int _oldScore = 0;
static int BALL_COUNT_CEILING = 24;

static int LABEL_FONT_SIZE = 65;
static int POINTS_LABEL_FONT_SIZE = 60;
static int CLEARED_BONUS_LABEL_FONT_SIZE = 60;
static int LABEL_MARGIN = 15;

CCSize windowSize;

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

    windowSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    // setup game scheduling/handling/other attributes
    _score = 0;
    _modifier = 1;
    _lastPairMatched = false;
    _pairsMatched = 0;
    this->_gameOver = false;
    this->_lastElapsedTime = GameUtils::getCurrentTime();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(
        "bubble_pop.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(
        "bubble_pop_2.mp3");
    this->setTouchEnabled(true);

    // add background
    _background = CCSprite::create(BackgroundImage);
    _background->setPosition(
        ccp(windowSize.width / 2 + origin.x, windowSize.height / 2 + origin.y));
    this->addChild(_background, ZIndexBackground);

    // add score label
    char scoreText[25];
    sprintf(scoreText, " %d", _score);
    _scoreLabel = CCLabelTTF::create(scoreText, "Marker Felt.ttf", LABEL_FONT_SIZE);
    _scoreLabel->setColor(GameUtils::getRandomColor3B());
    _scoreLabel->setAnchorPoint(ccp(0, 0));
    _scoreLabel->cocos2d::CCNode::setPosition(
        ccp(LABEL_MARGIN, windowSize.height - topScreenAdjust()));
    this->addChild(_scoreLabel, ZIndexGameTextLabels);

    //add modifier label
    char modifierText[15];
    sprintf(modifierText, "%dx", _modifier);
    _modifierLabel = CCLabelTTF::create(modifierText, "Marker Felt.ttf", LABEL_FONT_SIZE);
    _modifierLabel->setColor(GameUtils::getRandomColor3B());
    _modifierLabel->setAnchorPoint(ccp(0, 0));
    _modifierLabel->cocos2d::CCNode::setPosition(ccp(windowSize.width - _modifierLabel->getContentSize().width - (LABEL_MARGIN * 1.75), windowSize.height - topScreenAdjust()));
    this->addChild(_modifierLabel, ZIndexGameTextLabels);

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
            increaseGameDifficulty2(false);
            giveBonus();
            _lastElapsedTime = GameUtils::getCurrentTime();
        } else if (didTimeElapse()) {
            increaseGameDifficulty2(true);
        }
        // Update all the balls positions (animate the balls);
        updateBallPositions();
        // updateTintByBackground();
    } else { // GAME OVER
        unschedule(schedule_selector(GameScene::GameUpdate));
        this->schedule(schedule_selector(GameScene::GameEnd), 0.05);
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
                    this->setSelectedBall(ball);
                    ball->changeBallImage();
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

    time_interval = time_interval - 10;
}

void GameScene::increaseGameDifficulty2(bool timeElapsed)
{

    double random = ((double)rand() / (RAND_MAX)); // number between 0 and 1
    if (!timeElapsed || random <= 0.35) { //if they cleared the screen, or if random <= 0.35
        addMoreBallsCount = 2;
    } else {
        addMoreBallsCount = 1;
    }

    for (int i = 0; i < addMoreBallsCount; i++) {
        createNewBalls();
    }

    random = ((double)rand() / (RAND_MAX)); // number between 0 and 1
    if (random <= 0.33) {
        time_interval = 600;
    } else if (random <= 0.8) {
        time_interval = 1100;
        penaltyTime = penaltyTime + 5;
    } else {
        time_interval = 1700;
        penaltyTime = penaltyTime + 10;
    }

    if (timeElapsed && penaltyTime < 500) {
        time_interval = time_interval - penaltyTime;
        penaltyTime = penaltyTime + 25;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Given a ball and it's position in the ballArray, pop that ball,
// and it's corresponding selectedBall, and remove it from the ballArray.
//////////////////////////////////////////////////////////////////////////////////////////
void GameScene::popBalls(Ball* ball,
                         std::vector<Ball*>::iterator indexOfBall)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
        GameUtils::getRandomPopSound());
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
        GameUtils::getRandomPopSound());

    ballPopExplosion(ball, true);
    ballPopExplosion(this->getSelectedBall(), true);

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

void GameScene::popBall(Ball* ball,
                        std::vector<Ball*>::iterator indexOfBall)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
        GameUtils::getRandomPopSound());

    ballPopExplosion(ball, false);
    this->removeChild(ball);
    _ballArray.erase(indexOfBall);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Checks the current time against the last elapsed time,
// if it is greater than the TIME_INTERVAL then add new balls to the game.
//////////////////////////////////////////////////////////////////////////////////////////
bool GameScene::didTimeElapse()
{
    long currentTime = GameUtils::getCurrentTime();
    long lastElapsedTime = _lastElapsedTime;

    if (currentTime - lastElapsedTime > time_interval) {
        _lastElapsedTime = GameUtils::getCurrentTime();
        return true;
    }
    return false;
}

void GameScene::GameEnd()
{
    popBall(_ballArray.front(), _ballArray.begin());

    if (_ballArray.empty()) {
        unschedule(schedule_selector(GameScene::GameEnd));
        CCScene* lossScene = LossScene::scene(_score);
        //        CCDirector::sharedDirector()->replaceScene(
        //  CCTransitionFade::create(0.5, lossScene));
        CCDirector::sharedDirector()->replaceScene(lossScene);
        CCDirector::sharedDirector()->retain();
        resetGame();
    }
}

void GameScene::resetGame()
{
    nextBallId = 0;
    _pairsMatched = 0;
    time_interval = 2500;
    penaltyTime = 0;
    addMoreBallsCount = 1;
    _oldScore = 0;
}

void GameScene::ballPopExplosion(Ball* ball, bool showScore)
{
    const char* color = ball->getBallColor();
    ccColor4F effectColor = GameUtils::getColor4F(color);
    ccColor3B textColor = GameUtils::getColor3B(color);

    CCParticleExplosion* popEffect = CCParticleExplosion::createWithTotalParticles(45);
    popEffect->setStartColor(effectColor);
    popEffect->setEndColor(effectColor);
    popEffect->setLife(0.075);
    popEffect->setPosition(ccp(ball->getX(), ball->getY()));
    this->addChild(popEffect);

    if (showScore) {
        char pointsEarnedArray[6];
        sprintf(pointsEarnedArray, "+%d", getPointsEarned());
        CCLabelTTF* pointsEarnedLabel = CCLabelTTF::create(
            pointsEarnedArray, "Marker Felt.ttf", POINTS_LABEL_FONT_SIZE);
        pointsEarnedLabel->setColor(textColor);
        pointsEarnedLabel->setPosition(ccp(ball->getX(), ball->getY()));
        this->addChild(pointsEarnedLabel, ZIndexGameTextLabels);

        CCAction* fadeOut = CCFadeOut::create(0.75);
        pointsEarnedLabel->runAction(fadeOut);
        pointsEarnedLabel = NULL;
    }
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
    //return (10 * _modifier) + ((GameUtils::getCurrentTime() - _lastElapsedTime) * 0.1);
    return 100 * _modifier;
}

void GameScene::updateGameScoreAndText()
{
    _score = _score + getPointsEarned();
    if (_score - _oldScore > 1000) {
        _scoreLabel->setColor(GameUtils::getRandomColor3B());
        _oldScore = _score;
    }

    char scoreText[25];
    sprintf(scoreText, " %d", _score);
    _scoreLabel->setString(scoreText);
}

int GameScene::topScreenAdjust()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return LABEL_FONT_SIZE * 2.90;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return LABEL_FONT_SIZE * 2.15;
#endif
}

void GameScene::updateModifierAndText(bool ballsMatched)
{
    if (ballsMatched) {
        if (_modifier <= 8 && _pairsMatched == pow(_modifier, 2)) {
            _modifier++;
            _pairsMatched = 0;
            _modifierLabel->setColor(GameUtils::getRandomColor3B());
        } else {
            _pairsMatched++;
        }
    } else {
        _modifier = 1;
        _pairsMatched = 0;
    }

    char modifierText[15];
    sprintf(modifierText, "%dx", _modifier);
    _modifierLabel->setString(modifierText);
    _modifierLabel->setFontSize(LABEL_FONT_SIZE + (_modifier * 6));
    _modifierLabel->cocos2d::CCNode::setPosition(ccp(windowSize.width - _modifierLabel->getContentSize().width - (LABEL_MARGIN * 1.75), windowSize.height - topScreenAdjust() - (_modifier * 7)));
}

void GameScene::giveBonus()
{
    int bonus = 1000 + ((GameUtils::getCurrentTime() - _lastElapsedTime) * 0.1);
    _score = _score + bonus;
    updateGameScoreAndText();

    char bonusText[10];
    sprintf(bonusText, "+%d", bonus);
    CCLabelTTF* bonusEarnedLabel = CCLabelTTF::create(
        bonusText, "Marker Felt.ttf", CLEARED_BONUS_LABEL_FONT_SIZE);
    bonusEarnedLabel->setColor(GameUtils::getRandomColor3B());
    //bonusEarnedLabel->setPosition(ccp((windowSize.width / 2) - (bonusEarnedLabel->getDimensions().width / 2), (windowSize.height / 2) - (bonusEarnedLabel->getDimensions().height / 2)));
    bonusEarnedLabel->setPosition(ccp(_scoreLabel->getPositionX() + _scoreLabel->getDimensions().width + 200, _scoreLabel->getPositionY()));
    this->addChild(bonusEarnedLabel, ZIndexGameTextLabels);

    CCAction* fadeOut = CCFadeOut::create(1);
    bonusEarnedLabel->runAction(fadeOut);
    bonusEarnedLabel = NULL;
}

void GameScene::updateTintByBackground()
{
    //FIXME: This is not working as the game is progressing... need to understand
    CCAction* action = CCTintBy::create(0.05, -255, -255, -255);
    _background->runAction(action);
}