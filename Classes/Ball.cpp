#include "Ball.h"
#include "Constants.h"

USING_NS_CC;

Ball* ballSprite;

Ball* Ball::createBall(std::vector<Ball*> otherBalls, int displayWidth, int displayHeight) {
  
  ballSprite = new Ball();
  const char* ballColor = getRandomBallColor();
  const char* ballSelectedColor = getSelectedBallColor(ballColor);
  std::cout << ballColor << ":" << ballSelectedColor << std::endl;
  const char* color = getBallColorFromFileName(ballColor);
  
  if ( ballSprite && ballSprite->initWithFile(ballColor)) {
    ballSprite->setBallSelected(ballSelectedColor);
    ballSprite->setBallColor(color);
    
    ballSprite->setDisplayWidth(displayWidth);
    ballSprite->setDisplayHeight(displayHeight);
    ballSprite->setRadius(ballSprite->getTexture()->getPixelsHigh()/2);
    
    setNonOverlapRandomPoint(otherBalls);
    setBallPositionToOnScreen();
    ballSprite->setPosition(ccp(ballSprite->getX(), ballSprite->getY()));
    ballSprite->setXVelocity(getRandomVelocity());
    ballSprite->setYVelocity(getRandomVelocity());
    ballSprite->setState(BallNotSelected);
    
    ballSprite -> autorelease();
    
    return ballSprite;
  }
  
  CC_SAFE_DELETE(ballSprite);
  return NULL;
}

void Ball::updateBallPositions(std::vector<Ball*> ballList) {
  detectCollisions(ballList);
  
  this->setX(this->getX() + this->getXVelocity());
  this->setY(this->getY() + this->getYVelocity());
  
  if (this->getX() + this->getRadius() >= this->getDisplayWidth() ) {
    this->setXVelocity( -(this->getXVelocity() ));
    this->setX(this->getX() - 10); //10 is used to give a good pad if the ball were to get stuck
  }
  
  if(this->getX() - this->getRadius() < 0){
    this->setXVelocity( -(this->getXVelocity() ));
    this->setX(this->getX() + 10);
  }
  
  if (this->getY() + this->getRadius() >= this->getDisplayHeight() ) {
    this->setYVelocity(-(this->getYVelocity()));
    this->setY(this->getY() - 10); //10 is used to give a good pad if the ball were to get stuck
  }
  
  if(this->getY() - this->getRadius() < 0){
    this->setYVelocity(-(this->getYVelocity()));
    this->setY(this->getY() + 10);
  }
  
}

void Ball::detectCollisions(std::vector<Ball*> ballList) {
  std::vector<Ball*>::iterator iterator;
  for(iterator = ballList.begin(); iterator != ballList.end(); iterator++) {
    Ball* ball = *iterator;
    if (ballSprite->getX() != ball->getX() && ballSprite->getY() != ball->getY()) {
      
      // if the bounding rec of ball is over the other bounding rec
      if ( this->getX() + this->getRadius() + ball->getRadius() >= ball->getX()
          && this->getX() <= ball->getX() + this->getRadius() + ball->getRadius()
          && this->getY() + this->getRadius() + ball->getRadius() >= ball->getY()
          && this->getY() <= ball->getY() + this->getRadius() + ball->getRadius() ) {
        
        std::vector<Ball*> collisionArray = ball->getCollisionArray();
        
        //they collided
        if ( distanceTo(ballSprite, ballSprite) <= ballSprite->getRadius() + ball->getRadius() ) {
          
          //if colliding ball is not in the collision array
          if ( collisionArray.empty() ) {
            calculateNewVelocities(ballSprite, ball);
            this->getCollisionArray().push_back(ball);
          }
        }
        else { //once the balls are not colliding, clear the collision array
          if ( !collisionArray.empty() ) {
            ballSprite->getCollisionArray().clear();
          }
        }
      }
    }
  }
}

void Ball::calculateNewVelocities(Ball* thisBall, Ball* otherBall) {
  int tempVelHolder = thisBall->getXVelocity();
  thisBall->setXVelocity(otherBall->getXVelocity());
  otherBall->setXVelocity(tempVelHolder);
  
  tempVelHolder = thisBall->getYVelocity();
  thisBall->setXVelocity(otherBall->getYVelocity());
  thisBall->setYVelocity(tempVelHolder);
  
  thisBall->setX(thisBall->getX() + (thisBall->getXVelocity()));
  thisBall->setY(thisBall->getY() + (thisBall->getYVelocity()));
  otherBall->setX(otherBall->getX() + (otherBall->getXVelocity()));
  otherBall->setX(otherBall->getY() + (otherBall->getYVelocity()));
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

bool Ball::compareColor(Ball otherBall) {
  if ( strcmp(_ballColor, otherBall.getBallColor()) == 0) {
    return true;
  }
  return false;
}

const char* Ball::getRandomBallColor() {
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
  else if( strcmp(ball, "yelloball.png") == 0) {
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
void Ball::setNonOverlapRandomPoint(std::vector<Ball*> otherBalls) {
  CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
  
  int randomX = rand() % (int)(windowSize.width + 1);
  int randomY = rand() % (int)(windowSize.height + 1);
  ballSprite->setX(randomX);
  ballSprite->setY(randomY);
  
  Ball* otherBall;
  std::vector<Ball*>::iterator iterator;
  for(iterator = otherBalls.begin(); iterator != otherBalls.end(); iterator++) {
    otherBall = *iterator;
    while ( ballSprite->distanceTo(ballSprite, otherBall) < ballSprite->getRadius()
           + otherBall->getRadius()) {
      randomX = rand() % (int)(windowSize.width + 1);
      randomY = rand() % (int)(windowSize.height + 1);
      ballSprite->setX(randomX);
      ballSprite->setY(randomY);
    }
  }
}

void Ball::setBallPositionToOnScreen() {
  CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
  
  int radius = ballSprite->getRadius();
  int x = ballSprite->getX();
  int y = ballSprite->getY();
  
  if (x + radius > windowSize.width) {
    ballSprite->setX(windowSize.width - (radius * 2) );
  }
  else if (x - radius < 0) {
    ballSprite->setX(radius * 2);
  }
  
  if (y + radius > windowSize.height) {
    ballSprite->setY(windowSize.height - (radius * 2));
  }
  else if (y - radius < 0) {
    ballSprite->setY(radius * 2);
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
