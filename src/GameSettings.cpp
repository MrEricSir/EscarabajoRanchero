#include "GameSettings.h"
#include "Log.h"
#include <fstream>
#include <iostream>

#ifdef WINDOWS
#include <windows.h>
#endif

using namespace Escarabajo;
using namespace std;


SettingMap GameSettings::settings;


string GameSettings::getFilename()
{
	string ret = "";
#ifdef WINDOWS
	string path = "%AppData%\\.escarabajo";
	DWORD length = ExpandEnvironmentStringsA( path.c_str(), NULL, 0 );
	cout << "length: " << length << endl;
	char* cvalue = new char[ length + 1 ];
	ZeroMemory( cvalue, length + 1 );
	ExpandEnvironmentStringsA( path.c_str(), cvalue, length + 2 );
	ret = cvalue;
	delete[] cvalue;
#else
	string home = getenv( "HOME" );
	ret = home + "/.escarabajo";
#endif
	return ret;
}

// Loads settings.
bool GameSettings::load()
{
 	SettingMapParser smp( &settings );
	if ( smp.load( getFilename() ) )
	{
		return true;
	}
	else
	{
		// First time!
		init();
		save();
		return smp.load( getFilename() );		
	}
}

// Saves settings.
bool GameSettings::save()
{
	ofstream out( getFilename().c_str(), ios::out );

	if ( !out )
	{
		Log::write( "Failed to open our level file: " + getFilename() );
		return false;
	}

	out << "[Settings]" << endl;

	// World settings.
	out << SETTING_WORLD_LAST << " = " << settings[ SETTING_WORLD_LAST ] << endl;
	out << SETTING_WORLD_MAX << " = " << settings[ SETTING_WORLD_MAX ] << endl;

	// Level settings.
	out << SETTING_LEVEL_LAST << " = " << settings[ SETTING_LEVEL_LAST ] << endl;
	out << SETTING_LEVEL_MAX << " = " << settings [ SETTING_LEVEL_MAX ] << endl;

	return true;
}


void GameSettings::init()
{
	settings[ SETTING_WORLD_LAST ] = "0";
	settings[ SETTING_WORLD_MAX ] = "0";
	settings[ SETTING_LEVEL_LAST ] = "0";
	settings[ SETTING_LEVEL_MAX ] = "0";
}


int GameSettings::getMaxWorld()
{
	return (int)Utilities::stringToLong( settings[ SETTING_WORLD_MAX ] );
}

int GameSettings::getMaxLevel()
{
	return (int)Utilities::stringToLong( settings[ SETTING_LEVEL_MAX ] );
}

int GameSettings::getLastWorld()
{
	return (int)Utilities::stringToLong( settings[ SETTING_WORLD_LAST ] );
}

int GameSettings::getLastLevel()
{
	return (int)Utilities::stringToLong( settings[ SETTING_LEVEL_LAST ] );
}

void GameSettings::setLastWorld( int w )
{
	settings[ SETTING_WORLD_LAST ] = Utilities::longToString( w );
}

void GameSettings::setLastLevel( int l )
{
	settings[ SETTING_LEVEL_LAST ] = Utilities::longToString( l );
}


// Setting map parser!!!!!!!!!!11111

GameSettings::SettingMapParser::SettingMapParser( SettingMap* sm )
{
	this->sm = sm;
}

void GameSettings::SettingMapParser::section( std::string sectionName )
{
	// uuuhhhhh????
}

void GameSettings::SettingMapParser::pair( std::string name, std::string value )
{
	(*sm)[ name ] = value;
}
