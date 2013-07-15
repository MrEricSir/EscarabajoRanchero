#include "Audio.h"
#include "Log.h"

using namespace Escarabajo;
using namespace std;

extern SoundMap Audio::sounds;
extern MusicMap Audio::musics;

void Audio::init()
{
	//int audio_rate = 22050;
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
	int audio_channels = 2;
	int audio_buffers = 4096;

	if ( Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) )
	{
		Log::write( "Could not open sound system." );
	}
}


void Audio::deinit()
{
	stopMusic();

	// Deallocate our sounds.
	{
		SoundMap::iterator i;
		for( i = sounds.begin(); i != sounds.end(); ++i )
		{
			Mix_FreeChunk( i->second );
		}
	}

	// Deallocate our musics.
	{
		MusicMap::iterator i;
		for( i = musics.begin(); i != musics.end(); ++i )
		{
			Mix_FreeMusic( i->second );
		}
	}

	// Close audio system.
	Mix_CloseAudio();
}


bool Audio::loadSoundMap( std::string filename )
{
	AudioMapParser parser( &sounds );
	return parser.load( filename );
}


bool Audio::playSound( string name )
{
	int channel;
	Mix_Chunk* chunk = sounds[ name ];
	if ( chunk )
	{
		Log::write( "Audio: playing sound: " + name );
		channel = Mix_PlayChannel( -1, chunk, 0 );
		if ( channel != -1 )
		{
			return true;
		}
	}

	Log::write( "Audio: cannot start sound: " + name );
	return false;
}


bool Audio::loadMusicMap( std::string filename )
{
	MusicMapParser parser( &musics );
	return parser.load( filename );
}


bool Audio::playMusic( string name )
{
	int channel;
	Mix_Music* chunk = musics[ name ];
	if ( chunk )
	{
		Log::write( "Audio: playing music: " + name );
		channel = Mix_PlayMusic( chunk, -1 );
		if ( channel != -1 )
		{
			return true;
		}
	}

	Log::write( "Audio: cannot start sound: " + name );
	return false;
}

void Audio::stopMusic()
{
	Mix_HaltMusic();
}


Audio::AudioMapParser::AudioMapParser( SoundMap* sounds )
{
	this->soundMap = sounds;
}

void Audio::AudioMapParser::section( std::string sectionName )
{
	// Do nothing?
}

void Audio::AudioMapParser::pair( std::string name, std::string value )
{
	Log::write( "Loading sound: " + name );
	Mix_Chunk *sample;
	sample = Mix_LoadWAV( value.c_str() );

	if ( sample == NULL )
	{
		Log::write( "Audio: could not load sound file: " + value );
	}

	(*this->soundMap)[ name ] = sample;
}

Audio::MusicMapParser::MusicMapParser( MusicMap* musics )
{
	this->musicMap = musics;
}

void Audio::MusicMapParser::section( std::string sectionName )
{
	// Do nothing?
}

void Audio::MusicMapParser::pair( std::string name, std::string value )
{
	Log::write( "Loading music: " + name );
	Mix_Music *sample;
	sample = Mix_LoadMUS( value.c_str() );

	if ( sample == NULL )
	{
		Log::write( "Audio: could not load music file: " + value );
	}

	(*this->musicMap)[ name ] = sample;
}
