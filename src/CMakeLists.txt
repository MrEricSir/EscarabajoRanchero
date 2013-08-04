set(GAME_SRC
all.h
Array2D.h
Audio.cpp
Audio.h
AudioSamples.h
CastleDetect.cpp
CastleDetect.h
Config.cpp
Config.h
Dispatcher.cpp
Dispatcher.h
DispatchEventHandler.cpp
DispatchEventHandler.h
DispatchKeyHandler.cpp
DispatchKeyHandler.h
GameContext.cpp
GameContext.h
GameFont.cpp
GameFont.h
GameInput.cpp
GameInput.h
GameObject.cpp
GameObject.h
GameSettings.cpp
GameSettings.h
Graphics.cpp
Graphics.h
Level.cpp
LevelFactory.cpp
LevelFactory.h
LevelFileParser.cpp
LevelFileParser.h
LevelFileReader.cpp
LevelFileReader.h
LevelFileReaderPng.cpp
LevelFileReaderPng.h
Level.h
LevelPiece.h
Log.cpp
Log.h
Main.cpp
MapFileReader.cpp
MapFileReader.h
MoveDirection.h
Sprite.cpp
Sprite.h
SpriteMapFactory.cpp
SpriteMapFactory.h
SpriteMap.h
TrailSprites.h
Universe.cpp
Universe.h
Utilities.cpp
Utilities.h
World.cpp
World.h
)

Find_Package(SDL REQUIRED)
Find_Package(SDL_image REQUIRED)
Find_Package(SDL_mixer REQUIRED)
Find_Package(OpenGL REQUIRED)

set(CFLAGS
    ${OPENGL_CFLAGS}
    ${GL_CFLAGS}
    ${SDL_CFLAGS}
    ${SDLIMAGE_CFLAGS}
    ${SDLMIXER_CFLAGS}
    -D_VERSION=\"${VERSION}\"
    -D_INSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"
    -D_SOURCE_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\"
    -g
)

if (NOT DEBUG)
    set(CFLAGS
        ${CFLAGS}
        -O2
    )
endif ()

include_directories(${OPENGL_INCLUDE_DIRS} ${SDL_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR})
link_directories()
add_definitions(${CFLAGS})

add_executable(escarabajo ${GAME_SRC})
target_link_libraries(escarabajo ${OPENGL_LIBRARIES} ${GL_LIBRARY} ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLMIXER_LIBRARY})

# Copy binary to project root.
IF(WIN32)
   set(BINARY_NAME escarabajo.exe)
else()
   set(BINARY_NAME escarabajo)
endif()

add_custom_command(
    TARGET
        escarabajo
    POST_BUILD
    COMMAND 
        ${CMAKE_COMMAND} -E copy ${BINARY_NAME} ${CMAKE_SOURCE_DIR}/
)

# For make clean.
set_property(
    DIRECTORY ..
    APPEND
    PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        escarabajo
)
