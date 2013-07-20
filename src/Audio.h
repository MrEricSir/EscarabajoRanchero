#ifndef AUDIO_H_
#define AUDIO_H_

#include "all.h"
#include <string>
#include <map>
#include "MapFileReader.h"

namespace Escarabajo
{

typedef std::map<std::string, Mix_Chunk*> SoundMap;
typedef std::map<std::string, Mix_Music*> MusicMap;

class Audio
{

public:

    // Initialize the sound system.
    static void init();

    // Uninitialize the sound system.
    static void deinit();

    // Initialize an audio map file.
    static bool loadSoundMap( std::string filename );

    // Play an audio file.
    static bool playSound( std::string name );

    // Loads a music map file.
    static bool loadMusicMap( std::string filename );

    // Plays music based on name.
    static bool playMusic( std::string name );

    // Stops music.
    static void stopMusic();

private:

    // Parser for loading audio map.
    class AudioMapParser : public MapFileReader
    {
    public:
        AudioMapParser( SoundMap* sounds );
        virtual void section( std::string sectionName );
        virtual void pair( std::string name, std::string value );
        SoundMap* soundMap;
    };

    // Parser for loading music map.
    class MusicMapParser : public MapFileReader
    {
    public:
        MusicMapParser( MusicMap* musics );
        virtual void section( std::string sectionName );
        virtual void pair( std::string name, std::string value );
        MusicMap* musicMap;
    };

    // Map of names to sounds.
    static SoundMap sounds;

    // Map of names to music.
    static MusicMap musics;

};

}

#endif /* AUDIO_H_ */
