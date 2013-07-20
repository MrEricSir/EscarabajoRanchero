#ifndef LEVELFILEPARSER_H_
#define LEVELFILEPARSER_H_

#include "LevelFileReader.h"
#include "Level.h"
#include "SpriteMap.h"
#include "all.h"
#include "Log.h"

namespace Escarabajo
{

class Level;

/**
 * Parses data from a LevelFileReader into a level.
 */
class LevelFileParser
{

public:

    // Sets the level file reader.
    LevelFileParser( LevelFileReader& _reader, SpriteMap* _spriteMap );

    // Sets the level file reader.
    void setLevelFileReader( LevelFileReader& reader );

    void setSpriteMap( SpriteMap* spriteMap );

    // Parses the level.
    Level* parse();

private:

    LevelFileReader& reader;

    SpriteMap* spriteMap;

};

}

#endif /* LEVELFILEPARSER_H_ */
