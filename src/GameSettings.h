#ifndef GAMESETTINGS_H_
#define GAMESETTINGS_H_

#include "all.h"
#include "MapFileReader.h"

#define SETTING_WORLD_LAST    "WorldLast"
#define SETTING_WORLD_MAX    "WorldMax"
#define SETTING_LEVEL_LAST    "LevelLast"
#define SETTING_LEVEL_MAX    "LevelMax"

namespace Escarabajo
{

typedef std::map<std::string, std::string> SettingMap;

class GameSettings
{
public:

    // Loads settings.
    static bool load();

    // Saves settings.
    static bool save();

    static int getMaxWorld();
    static int getMaxLevel();

    static int getLastWorld();
    static int getLastLevel();

    static void setLastWorld( int w );
    static void setLastLevel( int l );

private:

    // Populates initial data.
    static void init();

    static std::string getFilename();

    static SettingMap settings;

    // Parser for loading setting map.
    class SettingMapParser : public MapFileReader
    {
    public:
        SettingMapParser( SettingMap* sm );
        virtual void section( std::string sectionName );
        virtual void pair( std::string name, std::string value );
        SettingMap* sm;
    };

};

}

#endif /* GAMESETTINGS_H_ */
