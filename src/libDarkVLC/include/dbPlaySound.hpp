#ifndef DE_PLAY_SOUND_V3_HPP
#define DE_PLAY_SOUND_V3_HPP

#include <sstream>
#include <cstdint>

// int sound1 = dbLoadSound( string uri );
// if (!sound1) {DE_ERROR("No sound id")} <- a valid sound id must be > 0.

// int sound2 = dbLoadSound( string uri );
// int sound3 = dbLoadSound( string uri );
// int sound4 = dbLoadSound( string uri );

// dbPlaySound( sound1, int loops = 1 );
// dbPlaySound( sound2, int loops = 1 );
// dbPauseSound( sound2, int loops = 1 );
// dbStopSound( sound2, int loops = 1 );

// void dbSetSoundVolume( int sound, int percent0_400 );
// int dbSoundVolume( int sound );

// double dbSoundDurationInSec( int soundId );
// flaot dbSoundRateInHz( int soundId );
// uint32_t dbSoundChannels( int soundId );
// uint32_t dbSoundFrames( int soundId );

int
dbLoadSound( std::string uri );

void
dbDeleteSound( int soundId );


void
dbPlaySound( int soundId, int loops = 1 );

void
dbStopSound( int soundId );

void
dbPauseSound( int soundId );

void
dbSetSoundVolume( int soundId, int percent0_400 );

int
dbGetSoundVolume( int soundId );

inline void
dbPlaySound( std::string uri, int loops = 0 )
{
   int soundId = dbLoadSound( uri );
   dbPlaySound( soundId, loops );
}

#endif

