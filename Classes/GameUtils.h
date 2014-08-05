//
//  GameUtils.h
//  BouncingBallsCPP
//
//  Created by Andrew Zurn on 7/31/14.
//
//

#ifndef __BouncingBallsCPP__GameUtils__
#define __BouncingBallsCPP__GameUtils__

#include <iostream>
#include "cocos2d.h"

class GameUtils {
public:
    static std::vector<const char*> getBackgrounds();
    static const char* getRandomPopSound();
    static cocos2d::ccColor4F getColor4F(const char*);
    static cocos2d::ccColor3B getColor3B(const char*);
    static cocos2d::ccColor3B getRandomColor3B();
    static cocos2d::ccColor4F getRandomColor4F();
    static long getCurrentTime();
};

#endif /* defined(__BouncingBallsCPP__GameUtils__) */
