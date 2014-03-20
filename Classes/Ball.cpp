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

//void Ball::updateBallPositions(Ball[] ballList) {
//TODO
//}

bool Ball::compareColor(Ball otherBall) {
	//TODO
  return true;
}

void Ball::calculateNewVelocities(Ball thisBall, Ball otherBall) {
	//TODO
}

float Ball::distanceTo(Ball thisBall, Ball otherBall) {
	//TODO
  return 0.0;
}

//void Ball::setRandomCoordinates(Ball[] otherBalls,
//			int displayWidth, int displayHeight) {
//TODO
//}

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
  std::cout << randomX << " : " << randomY << std::endl;
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
