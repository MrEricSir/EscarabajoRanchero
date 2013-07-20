#include "SpriteMap.h"
#include "MapFileReader.h"
#include <string>

#ifndef SPRITEMAPFACTORY_H_
#define SPRITEMAPFACTORY_H_

namespace Escarabajo
{

/**
 * Factory for creating sprite maps.
 */
class SpriteMapFactory
{

public:

    static SpriteMap load( std::string filename );

private:

    class SpriteMapParser : public MapFileReader
    {
    public:
        virtual void section( std::string sectionName );
        virtual void pair( std::string name, std::string value );
        SpriteMap spriteMap;
    };

};

}

#endif /* SPRITEMAPFACTORY_H_ */
