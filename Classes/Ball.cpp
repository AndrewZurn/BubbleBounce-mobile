#include "Ball.h"
#include "Constants.h"

USING_NS_CC;

Ball* Ball::createBall(int displayWidth, int displayHeight) {
  
  Ball *ballSprite = new Ball();
  const char* ballColor = getRandomBallColor();
  const char* ballSelectedColor = getSelectedBallColor(ballColor);
  const char* color = getBallColorFromFileName(ballColor);
  
  if ( ballSprite && ballSprite->initWithFile(ballColor)) {
    ballSprite->setBallSelected(ballSelectedColor);
    ballSprite->setBallColor(color);
    
    ballSprite->setDisplayWidth(displayWidth);
    ballSprite->setDisplayHeight(displayHeight);
    
    CCPoint randomPoint = getRandomPoint();
    ballSprite->setPosition(randomPoint);
    ballSprite->setX(randomPoint.x);
    ballSprite->setY(randomPoint.y);
    ballSprite->setRadius(ballSprite->getTexture()->getPixelsHigh()/2); //don't know if this will work...
    ballSprite->setXVelocity(getRandomVelocity());
    ballSprite->setY(getRandomVelocity());
    
    ballSprite -> autorelease();
    return ballSprite;
  }
  
  CC_SAFE_DELETE(ballSprite);
  return NULL;
}

void Ball::updateBallPositions(std::vector<Ball> ballList) {
  detectCollisions(ballList);
  
  this->setX(this->getX() + this->getXVelocity());
  this->setY(this->getY() + this->getYVelocity());
  
  if (this->getX() >= this->getDisplayWidth()/* - this.getBitmap().getBitmap().getWidth() */) {
    this->setXVelocity( -(this->getXVelocity() ));
    this->setX(this->getX() - 10); //10 is used to give a good pad if the ball were to get stuck
  }
  
  if(this->getX() < 0){
    this->setXVelocity( -(this->getXVelocity() ));
    this->setX(this->getX() + 10);
  }
  
  if (this->getY() >= this->getDisplayHeight()/* - this.getBitmap().getBitmap().getWidth() */) {
    this->setYVelocity(-(this->getYVelocity()));
    this->setY(this->getY() - 10); //10 is used to give a good pad if the ball were to get stuck
  }
  
  if(this->getY() < 0){
    this->setYVelocity(-(this->getYVelocity()));
    this->setY(this->getY() + 10);
  }
  
}

void Ball::detectCollisions(std::vector<Ball> ballList) {
  std::vector<Ball>::iterator iterator;
  for(iterator = ballList.begin(); iterator != ballList.end(); iterator++) {
    if (!(*this == *iterator)) {
      Ball ball = *iterator;
      
      //if balls close to overlapping
      if ( this->getX() + this->getRadius() + ball.getRadius() >= ball.getX()
          && this->getX() <= ball.getX() + this->getRadius() + ball.getRadius()
          && this->getY() + this->getRadius() + ball.getRadius() >= ball.getY()
          && this->getY() <= ball.getY() + this->getRadius() + ball.getRadius() ) {
        
        std::vector<Ball> collisionArray = ball.getCollisionArray();
        if ( distanceTo(*this, ball) <= this->getRadius() + ball.getRadius() ) { //if balls collided
          
          //if ball is not in the collision array
          if (!(std::find(collisionArray.begin(), collisionArray.end(), ball) != collisionArray.end()) ) {
            calculateNewVelocities(*this, ball);
            this->getCollisionArray().push_back(ball);
          }
        }
        else { //balls did not collide, remove ball from collision array
          if (std::find(collisionArray.begin(), collisionArray.end(), ball) != collisionArray.end() ){
            std::vector<Ball>::iterator ballIndex = std::find(collisionArray.begin(), collisionArray.end(), ball);
            this->getCollisionArray().erase(ballIndex);
          }
        }
      }
    }
  }
}

void Ball::calculateNewVelocities(Ball thisBall, Ball otherBall) {
  int tempVelHolder = thisBall.getXVelocity();
  thisBall.setXVelocity(otherBall.getXVelocity());
  otherBall.setXVelocity(tempVelHolder);
  
  tempVelHolder = thisBall.getYVelocity();
  thisBall.setXVelocity(otherBall.getYVelocity());
  thisBall.setYVelocity(tempVelHolder);
  
  thisBall.setX(thisBall.getX() + (thisBall.getXVelocity()));
  thisBall.setY(thisBall.getY() + (thisBall.getYVelocity()));
  otherBall.setX(otherBall.getX() + (otherBall.getXVelocity()));
  otherBall.setX(otherBall.getY() + (otherBall.getYVelocity()));
}

float Ball::distanceTo(Ball thisBall, Ball otherBall) {
  float a = (thisBall.getX() - otherBall.getX()) * (thisBall.getX() - otherBall.getX());
  float b = (thisBall.getY() - otherBall.getY()) * (thisBall.getY() - otherBall.getY());
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

cocos2d::CCPoint Ball::getRandomPoint() {
  CCSize windowSize = CCDirector::sharedDirector()->getVisibleSize();
  int randomX = rand() % (int)(windowSize.width + 1);
  int randomY = rand() % (int)(windowSize.height + 1);
  
  return ccp(randomX, randomY);
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
