#include "Config.h"

using namespace std;
using namespace Escarabajo;

// GCC likes it when we do this.
extern int Config::screenWidth;
extern int Config::screenHeight;
extern int Config::widthOfLevelPiece;
extern int Config::heightOfLevelPiece;
extern int Config::widthOfLevelPiece9;
extern int Config::heightOfLevelPiece9;
extern int Config::widthOfFontPiece;
extern double Config::zoomFactor;
extern int Config::animationStep;
extern int Config::levelWidth;
extern int Config::levelHeight;

void Config::init()
{
	screenWidth = 640;
	screenHeight = 480;

	widthOfLevelPiece = 15;
	heightOfLevelPiece = 15;

	widthOfLevelPiece9 = 5;
	heightOfLevelPiece9 = 5;

	widthOfFontPiece = 6;

	zoomFactor = 3;

	animationStep = 3;

	levelWidth = 21;
	levelHeight = 15;
}

int Config::getScreenWidth()
{
	return levelWidth * widthOfLevelPiece * zoomFactor;
}

int Config::getScreenHeight()
{
	return ( levelHeight * heightOfLevelPiece + heightOfLevelPiece ) * zoomFactor;
}

double Config::getZoomFactor()
{
	return zoomFactor;
}


int Config::getWidthOfLevelPiece()
{
	return widthOfLevelPiece;
}

int Config::getHeightOfLevelPiece()
{
	return heightOfLevelPiece;
}

int Config::getWidthOfLevelPiece9()
{
	return widthOfLevelPiece9;
}

int Config::getHeightOfLevelPiece9()
{
	return heightOfLevelPiece9;
}

int Config::getWidthOfFontPiece()
{
	return widthOfFontPiece;
}

int Config::getAnimationStep()
{
	return animationStep;
}

