#ifndef ALL_H_
#define ALL_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include <set>
#include <list>

// Allow/disallow level completion
// 0 is false, 1 is true
#define ALLOW_LEVEL_END 1

// File sections.
#define LEVELFILE_SECTION_STRUCTURE "Structure"
#define LEVELFILE_SECTION_SPRITES   "Sprites"
#define LEVELFILE_SECTION_VALUES    "Values"

// Values section in file.
#define SECTION_VALUES_STARTING_FUEL    0
#define SECTION_VALUES_STARTING_FUEL_OFFSET    1

#endif
