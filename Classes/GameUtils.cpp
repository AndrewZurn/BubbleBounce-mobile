//
//  GameUtils.cpp
//  BouncingBallsCPP
//
//  Created by Andrew Zurn on 7/31/14.
//
//

#include "GameUtils.h"

std::vector<const char*> GameUtils::getBackgrounds()
{
    char buffer[22];
    std::vector<const char*> backgrounds;
    for (int i = 0; i < 24; i++) {
        sprintf(buffer, "background_%d.png", i);
        backgrounds.push_back(buffer);
    }

    return backgrounds;
}

const char* GameUtils::getRandomPopSound()
{
    double random = ((double)rand() / (RAND_MAX));

    if (random <= 0.5) {
        return "bubble_pop.mp3";
    } else {
        return "bubble_pop_2.mp3";
    }
}

cocos2d::ccColor4F GameUtils::getColor4F(const char* color)
{
    cocos2d::ccColor4F oppColor;

    if (strcmp(color, "blue") == 0) {
        oppColor.r = 136.0f / 255.0f;
        oppColor.g = 240.0f / 255.0f;
        oppColor.b = 255.0f / 255.0f;
        oppColor.a = 1.0f;
    } else if (strcmp(color, "orange") == 0) {
        oppColor.r = 243.0f / 255.0f;
        oppColor.g = 108.0f / 255.0f;
        oppColor.b = 18.0f / 255.0f;
        oppColor.a = 1.0f;
    } else if (strcmp(color, "pink") == 0) {
        oppColor.r = 198.0f / 255.0f;
        oppColor.g = 17.0f / 255.0f;
        oppColor.b = 235.0f / 255.0f;
        oppColor.a = 1.0f;
    } else if (strcmp(color, "red") == 0) {
        oppColor.r = 235.0f / 255.0f;
        oppColor.g = 17.0f / 255.0f;
        oppColor.b = 17.0f / 255.0f;
        oppColor.a = 1.0f;
    } else if (strcmp(color, "yellow") == 0) {
        oppColor.r = 8.0f / 255.0f;
        oppColor.g = 255.0f / 255.0f;
        oppColor.b = 0.0f / 255.0f;
        oppColor.a = 1.0f;
    }

    return oppColor;
}

cocos2d::ccColor3B GameUtils::getColor3B(const char* color)
{
    if (strcmp(color, "blue") == 0) {
        return cocos2d::ccc3(136, 240, 255);
    } else if (strcmp(color, "orange") == 0) {
        return cocos2d::ccc3(243, 108, 18);
    } else if (strcmp(color, "pink") == 0) {
        return cocos2d::ccc3(198, 17, 235);
    } else if (strcmp(color, "red") == 0) {
        return cocos2d::ccc3(235, 17, 17);
    } else if (strcmp(color, "yellow") == 0) {
        return cocos2d::ccc3(8, 255, 0);
    } else {
        return cocos2d::ccc3(0, 0, 0);
    }
}

cocos2d::ccColor3B GameUtils::getRandomColor3B()
{
    double random = ((double)rand() / (RAND_MAX));

    if (random <= 0.2) {
        return cocos2d::ccc3(136, 240, 255); //blue
    } else if (random <= 0.4) {
        return cocos2d::ccc3(243, 108, 18); //orange
    } else if (random <= 0.6) {
        return cocos2d::ccc3(198, 17, 235); //pink
    } else if (random <= 0.8) {
        return cocos2d::ccc3(235, 17, 17); //red
    } else {
        return cocos2d::ccc3(8, 255, 0); //yellow
    }
}

cocos2d::ccColor4F GameUtils::getRandomColor4F()
{
    cocos2d::ccColor4F color;

    double random = ((double)rand() / (RAND_MAX));
    if (random <= 0.2) { //blue
        color.r = 136.0f / 255.0f;
        color.g = 240.0f / 255.0f;
        color.b = 255.0f / 255.0f;
        color.a = 1.0f;
    } else if (random <= 0.4) { //orange
        color.r = 243.0f / 255.0f;
        color.g = 108.0f / 255.0f;
        color.b = 18.0f / 255.0f;
        color.a = 1.0f;
    } else if (random <= 0.6) { //pink
        color.r = 198.0f / 255.0f;
        color.g = 17.0f / 255.0f;
        color.b = 235.0f / 255.0f;
        color.a = 1.0f;
    } else if (random <= 0.8) { //red
        color.r = 235.0f / 255.0f;
        color.g = 17.0f / 255.0f;
        color.b = 17.0f / 255.0f;
        color.a = 1.0f;
    } else { //yellow
        color.r = 8.0f / 255.0f;
        color.g = 255.0f / 255.0f;
        color.b = 0.0f / 255.0f;
        color.a = 1.0f;
    }

    return color;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Private method to return the current time of the in milliseconds.
//////////////////////////////////////////////////////////////////////////////////////////
long GameUtils::getCurrentTime()
{
    timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}