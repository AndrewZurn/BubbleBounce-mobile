#include "Ball.h"
#include "Constants.h"

USING_NS_CC;

Ball* Ball::createBall(std::vector<Ball*> otherBalls, int ballId, const char* original_color) {
  Ball* ballSprite = new Ball();
  CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
  
  const char* ballColor;
  if ( strcmp(original_color, "") == 0) {
    ballColor = getRandomBallImage();
  }
  else {
    ballColor = original_color;
  }
  
  const char* ballSelectedColor = getSelectedBallColor(ballColor);
  const char* color = getBallColorFromFileName(ballColor);
  
  if ( ballSprite && ballSprite->initWithFile(ballColor) ) {
    ballSprite->setBall(ballColor);
    ballSprite->setBallSelected(ballSelectedColor);
    ballSprite->setBallColor(color);
    ballSprite->setBallId(ballId);
    
    ballSprite->setDisplayWidth(windowSize.width);
    ballSprite->setDisplayHeight(windowSize.height);
    ballSprite->setRadius(ballSprite->getTexture()->getPixelsHigh()/2);
    
    setNonOverlapRandomPoint(ballSprite, otherBalls);
    setBallPositionToOnScreen(ballSprite);
    ballSprite->setPosition(ccp(ballSprite->getX(), ballSprite->getY()));
    ballSprite->setXVelocity(getRandomVelocity());
    ballSprite->setYVelocity(getRandomVelocity());
    ballSprite->setState(BallNotSelected);
    
    ballSprite->setTopScreenAdjust(topScreenAdjust());
    
    ballSprite->autorelease();
    
    return ballSprite;
  }
  
  CC_SAFE_DELETE(ballSprite);
  return NULL;
}

void Ball::updateBallPositions(std::vector<Ball*> ballList) {
  detectCollisions(ballList);
  
  this->setX(this->getX() + this->getXVelocity());
  this->setY(this->getY() + this->getYVelocity());
  
  ////RIGHT SIDE OF SCREEN////
  if (this->getX() + this->getRadius() >= this->getDisplayWidth() ) {
    this->setXVelocity( -(this->getXVelocity() ));
    this->setX(this->getX() - 10); //10 is used to give a good pad if the ball were to get stuck
  }
  
  ////LEFT SIDE OF SCREEN////
  if(this->getX() - this->getRadius() < 0){
    this->setXVelocity( -(this->getXVelocity() ));
    this->setX(this->getX() + 10);
  }
  
  ////TOP SIDE OF SCREEN////
  if (this->getY() + this->getRadius() >= this->getDisplayHeight() - _topScreenAdjust ) { //TODO: Find HEIGHT of AdBanner
    this->setYVelocity(-(this->getYVelocity()));
    this->setY(this->getY() - 10); //10 is used to give a good pad if the ball were to get stuck
  }
  
  ////BOTTOM SIDE OF SCREEN////
  if(this->getY() - this->getRadius() < _progressBarHeight ){ //TODO: Find HEIGHT of ProgressTimer here.
    this->setYVelocity(-(this->getYVelocity()));
    this->setY(this->getY() + 10);
  }
  
  this->setPosition(ccp(this->getX(), this->getY()));
}

void Ball::detectCollisions(std::vector<Ball*> ballList) {
  std::vector<Ball*>::iterator iterator;
  for(iterator = ballList.begin(); iterator != ballList.end(); iterator++) {
    Ball* ball = (Ball*) (*iterator);
    
    if ( this->getBallId() == ball->getBallId() ) { continue; } //must continue, do not change

   if ( this->boundingBox().intersectsRect(ball->boundingBox()) ) {
  
     std::vector<Ball*> collisionArray = this->getCollisionArray();
     std::vector<Ball*> otherCollisionArray = ball->getCollisionArray();
      if ( distanceTo(this, ball) <= this->getRadius() + ball->getRadius() + 3) {
        if ( collisionArray.empty() && otherCollisionArray.empty() ) {
          calculateNewVelocities(this, ball);
          collisionArray.push_back(ball);
          otherCollisionArray.push_back(this);
          this->setCollisionArray(collisionArray);
          ball->setCollisionArray(otherCollisionArray);
        }
      }
      else { //once the balls are not colliding, clear the collision array
        collisionArray.clear();
        otherCollisionArray.clear();
        this->setCollisionArray(collisionArray);
        ball->setCollisionArray(otherCollisionArray);
      }
    }
  }
}

void Ball::calculateNewVelocities(Ball* thisBall, Ball* otherBall) {
  int tempVelHolder = thisBall->getXVelocity();
  thisBall->setXVelocity(otherBall->getXVelocity());
  otherBall->setXVelocity(tempVelHolder);
  
  tempVelHolder = thisBall->getYVelocity();
  thisBall->setYVelocity(otherBall->getYVelocity());
  otherBall->setYVelocity(tempVelHolder);
  
  thisBall->setX(thisBall->getX() + (thisBall->getXVelocity()*2));
  thisBall->setY(thisBall->getY() + (thisBall->getYVelocity()*2));
  thisBall->setPosition(ccp(thisBall->getX(), thisBall->getY()));
  
  otherBall->setX(otherBall->getX() + (otherBall->getXVelocity()*2));
  otherBall->setY(otherBall->getY() + (otherBall->getYVelocity()*2));
  otherBall->setPosition(ccp(otherBall->getX(), otherBall->getY()));
}

float Ball::distanceTo(Ball *thisBall, Ball *otherBall) {
  float a = (thisBall->getX() - otherBall->getX()) * (thisBall->getX() - otherBall->getX());
  float b = (thisBall->getY() - otherBall->getY()) * (thisBall->getY() - otherBall->getY());
  float distance = (float) sqrt(a + b);
  
  if ( distance < 0) {
    distance = -distance;
  }
  return distance;
}

bool Ball::compareColor(Ball* otherBall) {
  if ( strcmp(this->getOriginalBallImage(),
              otherBall->getOriginalBallImage()) == 0) {
    return true;
  }
  return false;
}

const char* Ball::getRandomBallImage() {
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
    return "yellowball.png";
  }
}

const char* Ball::getSelectedBallColor(const char *ball) {
  if ( strcmp(ball, "blueball.png") == 0) {
    return "blueballselected.png";
  }
  else if( strcmp(ball, "orangeball.png") == 0) {
    return "orangeballselected.png";
  }
  else if( strcmp(ball, "pinkball.png") == 0) {
    return "pinkballselected.png";
  }
  else if( strcmp(ball, "redball.png") == 0) {
    return "redballselected.png";
  }
  else if( strcmp(ball, "yellowball.png") == 0) {
    return "yellowballselected.png";
  }
  else {
    throw BallSelectedColorNotFound;
  }
}

const char* Ball::getBallColorFromFileName(const char* ball) {
  if ( strcmp(ball, "blueball.png") == 0) {
    return "blue";
  }
  else if( strcmp(ball, "orangeball.png") == 0) {
    return "orange";
  }
  else if( strcmp(ball, "pinkball.png") == 0) {
    return "pink";
  }
  else if( strcmp(ball, "redball.png") == 0) {
    return "red";
  }
  else if( strcmp(ball, "yellowball.png") == 0) {
    return "yellow";
  }
  else {
    throw BallColorFromFileNotFound;
  }
}

//TODO: This needs to be refined further.
void Ball::setNonOverlapRandomPoint(Ball* thisBall, std::vector<Ball*> otherBalls) {
  CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
  
  int randomX = rand() % (int)(windowSize.width + 1);
  int randomY = rand() % (int)(windowSize.height + 1);
  thisBall->setX(randomX);
  thisBall->setY(randomY);
  
  Ball* otherBall;
  std::vector<Ball*>::iterator iterator;
  for(iterator = otherBalls.begin(); iterator != otherBalls.end(); iterator++) {
    otherBall = *iterator;
    while ( thisBall->distanceTo(thisBall, otherBall) < thisBall->getRadius()
           + otherBall->getRadius() ) {
      randomX = rand() % (int)(windowSize.width + 1);
      randomY = rand() % (int)(windowSize.height + 1);
      thisBall->setX(randomX);
      thisBall->setY(randomY);
    }
  }
}

void Ball::setBallPositionToOnScreen(Ball* thisBall) {
  CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
  
  int radius = thisBall->getRadius();
  int x = thisBall->getX();
  int y = thisBall->getY();
  
  if (x + radius > windowSize.width) {
    thisBall->setX(windowSize.width - (radius * 2) );
  }
  else if (x - radius < 0) {
    thisBall->setX(radius * 2);
  }
  
  if (y + radius > windowSize.height - topScreenAdjust() ) {
    thisBall->setY(windowSize.height - (radius * 2));
  }
  else if (y - radius < thisBall->getProgressBarHeight() + 25 ) {
    thisBall->setY(radius * 4);
  }
}

int Ball::getRandomVelocity() {
  double random = ((double) rand() / (RAND_MAX));
  
  if (random >= 0.5) {
    return BallSpeed;
  }
  else {
    return -BallSpeed;
  }
}

int Ball::changeBallImage() {
  if (getState() == BallNotSelected) {
    this->setTexture(CCTextureCache::sharedTextureCache()
                     ->addImage(this->getSelectedBallImage()));
    setState(BallSelected);
    return BallSelected;
  }
  else {
    this->setTexture(CCTextureCache::sharedTextureCache()
                     ->addImage(this->getOriginalBallImage()));
    setState(BallNotSelected);
    return BallNotSelected;
  }
}

int Ball::topScreenAdjust() {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	return 72;
#endif
  
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  return 55;
#endif
}