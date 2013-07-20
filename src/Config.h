#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "all.h"

namespace Escarabajo
{

class Config
{

private:

    // Screen constants.
    static int screenWidth;
    static int screenHeight;

    // Dimensions of level pieces.
    static int widthOfLevelPiece;
    static int heightOfLevelPiece;

    static double zoomFactor;

    static int widthOfFontPiece;

    static int animationStep;

    static int levelWidth;
    static int levelHeight;

    static int widthOfLevelPiece9;
    static int heightOfLevelPiece9;

public:

    // Call this ONCE!!! when you start up.
    static void init();

    // Screen dimensions.
    static int getScreenWidth();
    static int getScreenHeight();

    // Level piece size.
    static int getWidthOfLevelPiece();
    static int getHeightOfLevelPiece();

    // Level piece size (snakes)
    static int getWidthOfLevelPiece9();
    static int getHeightOfLevelPiece9();

    // Font dimensions.
    static int getWidthOfFontPiece();

    // Zoom factor.
    static double getZoomFactor();

    // Step of animation.
    static int getAnimationStep();

};

}

#endif /* CONFIGURATION_H_ */
