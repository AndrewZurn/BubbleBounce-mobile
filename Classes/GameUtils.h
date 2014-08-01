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
    static cocos2d::ccColor4F getOppositeColor4F(const char*);
    static cocos2d::ccColor3B getOppositeColor3B(const char*);
};

#endif /* defined(__BouncingBallsCPP__GameUtils__) */
