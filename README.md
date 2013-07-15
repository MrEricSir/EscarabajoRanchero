Escarabajo Ranchero
===================

Escarabajo Ranchero is a (currently unfinished) 2D puzzle game from Eric Gregory and Stevie Hryciw.  

How to play
-----------
You play a silk beetle who must lasso insects without running out of silk.  Use the arrow keys to move around the board.  If your silk runs out, the level will restart.

License
-------
All source code and level designs are released under GPLv2 or newer.  All graphics and audio are released as public domain.

Building
--------
A C++ compiler and CMake are required.  You will need the following libraries and headers:
* OpenGL
* GLU (often distributed with OpenGL)
* SDL
* SDL_mixer
* SDL_image

On Ubuntu, these packages can be installed with:
> sudo apt-get install build-essential cmake libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libgl1-mesa-dev

The recommended way to build on *nix systems is as follows:
> $ cd build
>
> $ cmake ..
>
> $ make

Running and installing
----------------------
To run from the build directory, execute ./escarabajo from the root project folder.  The game will only look for the resources directory from there.

Escarabajo Ranchero cannot currently be installed.  We ran out of silk.

Bugs
----
To file any bugs, suggestions, complaints, or good knock-knock jokes, please visit the project on GitHub.
