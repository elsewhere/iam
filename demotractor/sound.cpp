//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>

#ifdef _DEBUG
	#include <stdlib.h>
	#include "mmgr.h"
#endif

#include "sound.hpp"
#include "globals.hpp"

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

using namespace TRACTION_DEMOTRACTOR;

//--------------------------------------------------------------------------------------------
//  Static members
//--------------------------------------------------------------------------------------------

bool Sound::createdFlag = false;
bool Sound::enabled = false;
bool Sound::syncFlag = false;
Sound *Sound::instance = NULL;

//--------------------------------------------------------------------------------------------
//  Sound class code
//--------------------------------------------------------------------------------------------

Sound::~Sound()
{
	
}

Sound *Sound::create()
{
	if(!createdFlag)
	{
		instance = new Sound;
		if(!instance)
		{						
			throw "Sound::create(): Memory allocation error";
			return NULL;
		}

		createdFlag = true;

		dmsMsg("Sound::create(): Sound instance created\n");
	}
	else
	{				
		throw "One instance of class Sound allready running";
		return NULL;
	}

	return instance;
}

bool Sound::init()
{		
	if(FSOUND_GetVersion() < FMOD_VERSION)
	{	
		throw "Wrong version of FMOD in use";
		return false;
	}

	// -1 = autodetect
	if(!FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND))
	{				
		throw "FMOD output type setting failed";
		return false;
	}

	if(!FSOUND_SetDriver(0))
	{				
		throw "Current sound driver selection doesn't work";
		return false;
	}

	if(!FSOUND_Init(44000, 32, 0))
	{				
		throw "FMOD initialization failed";
		FSOUND_Close();
		return false;
	}

	// Aktivoidaan FFT-unit jotta voidaan käyttää GetSpectrumia
	FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), true);
		
	return true;
}

void Sound::setSync(bool f)
{
	syncFlag = f;	
}

void Sound::setEnabled(bool f)
{
	enabled = f;
}

bool Sound::checkSync()
{		
	return syncFlag;
}

bool Sound::checkEnabled()
{
	return enabled;
}

void Sound::setVolume(FMUSIC_MODULE *mod, int i)
{
	if(i >= 0 && i <= 256)
	{
		FSOUND_SetSFXMasterVolume(i);
		if(mod)
		{
			FMUSIC_SetMasterVolume(mod, i);
		}
	}
}

float *Sound::getSpectrum()
{
	// palauttaa 512 alkion float taulukon
	return FSOUND_DSP_GetSpectrum();
}

void Sound::close()
{
	FSOUND_Close();

	if(createdFlag)
	{		
		createdFlag = false;
	
		if(instance)
		{
			delete instance;
			instance = NULL;
		}
		
		dmsMsg("Sound::close(): Instance deleted\n");
	}
}

//--------------------------------------------------------------------------------------------
//  Sample class code
//--------------------------------------------------------------------------------------------

Sample::Sample()
{
	sample = NULL;
}

Sample::~Sample()
{
	release();
}

bool Sample::load(char *name)
{	
	sample = FSOUND_Sample_Load(FSOUND_UNMANAGED, name, FSOUND_NORMAL, 0);
	if(!sample)
	{		
		return false;
	}
	else
	{		
	}

	return true;
}

void Sample::play()
{
	FSOUND_PlaySound(FSOUND_FREE, sample);
}

void Sample::release()
{
	if(sample != NULL)
	{
		FSOUND_Sample_Free(sample);
		sample = NULL;
	}
}

//--------------------------------------------------------------------------------------------
//  Song class code
//--------------------------------------------------------------------------------------------

Song::Song()
{
	stream = NULL;
	mod = NULL;
	mode = -1;

	data = NULL;
	isPlaying = false;
}

Song::~Song()
{

}

bool Song::loadMod(char *name)
{	
	mod = FMUSIC_LoadSong(name);
	if(!mod)
	{		
		return false;
	}
	mode = 0;

	lenght = FMUSIC_GetNumOrders(mod);

	return true;
}

bool Song::loadStream(char *name)
{
	FILE *fp;
	int length;

		data = NULL;
		fp = fopen(name, "rb");

		if (!fp)
		{			
			FSOUND_Close();
			return false;
		}

		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		data = new char[length];
		if(!data) return false;

		fread(data, length, 1, fp);
		fclose(fp);

		stream = FSOUND_Stream_OpenFile(data, FSOUND_NORMAL | FSOUND_LOADMEMORY, length);
		if(!stream)
		{				
			return false;
		}
		mode = 1;
		
		FSOUND_Stream_SetEndCallback(stream, endcallback, 0);
		FSOUND_Stream_SetSynchCallback(stream, endcallback, 0);

		lenght = FSOUND_Stream_GetLengthMs(stream);

	return true;
}

bool Song::loadStream2(char *memdata, int lenght)
{
	data = new char[lenght];
	if(!data) return false;

	memcpy(data, memdata, lenght);

	stream = FSOUND_Stream_OpenFile(data, FSOUND_NORMAL | FSOUND_LOADMEMORY, lenght);
	if(!stream)
	{				
		return false;
	}
	mode = 1;
		
	//FSOUND_Stream_SetEndCallback(stream, endcallback, 0);
	//FSOUND_Stream_SetSynchCallback(stream, endcallback, 0);	

	return true;
}

void Song::start()
{
	switch(mode)
	{
		case 0: 
		{
			if(!FMUSIC_PlaySong(mod))
			{					
				return;
			}

			isPlaying = true;

		} break;

		case 1:
		{
			channel = FSOUND_Stream_PlayEx(FSOUND_FREE, stream, NULL, true);
			if(channel == -1)
			{							
				return;
			}

			FSOUND_SetPaused(channel, false);
			setPause(false);
			isPlaying = true;

		} break;

	}	
}

void Song::stop()
{
	switch(mode)
	{
		case 0:
		{
			FMUSIC_StopSong(mod);
			isPlaying = false;
		} break;

		case 1:
		{
			FSOUND_Stream_Stop(stream);
			isPlaying = false;
		} break;
	}
}

bool Song::checkPlaying()
{
	return isPlaying;
}

void Song::release()
{
	switch(mode)
	{
		case 0:
		{
			if(mod)
			{
				FMUSIC_FreeSong(mod);
				mod = NULL;				
			}
		} break;

		case 1:
		{
			if(stream)
			{					
				FSOUND_Stream_Close(stream);
				stream = NULL;
			}
		} break;
	}

	if(data)
	{
		delete [] data;
		data = NULL;
	}

	stream = NULL;
	mod = NULL;
	mode = -1;

	data = NULL;
}

void Song::setPosition(unsigned int offset)
{
	switch(mode)
	{
		case 0:
		{
			// Tässä offsetti on patterin järjestysnumero
			if(mod)
			{
				FMUSIC_SetOrder(mod, offset);
			}
		}
		break;

		case 1:
		{
			// Tässä offsetti on millisekunteina
			if(stream)
			{
				FSOUND_Stream_SetTime(stream, offset);
			}
		} break;
	}
}

void Song::setPause(bool f)
{
	(f) ? FSOUND_SetPaused(FSOUND_ALL, true) : FSOUND_SetPaused(FSOUND_ALL, false);
}

int Song::getLenght()
{
	return lenght;
}

int Song::getPosition()
{
	int time;
	
	time = 0;
	switch(mode)
	{
		// Palauttaa patterin järjestysnumeron
		case 0:
		{
			if(mod)
			{
				time = FMUSIC_GetOrder(mod);
			}
		} break;

		// Palauttaa soittoajan millisekunteina
		case 1:
		{
			if(stream)
			{					
				time = FSOUND_Stream_GetTime(stream);
			}
		} break;
	}	


	return time;
}

FMUSIC_MODULE *Song::getMod()
{
	if(mod)
	{
		return mod;
	}

	return NULL;
}

//--------------------------------------------------------------------------------------------
// This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

namespace TRACTION_DEMOTRACTOR
{

	signed char endcallback(FSOUND_STREAM *stream, void *buff, int len, int param)
	{
		// end of stream callback doesnt have a 'buff' value, if it doesnt it could be a synch point.	
		if (buff)
		{	
			Sound::setSync(true);
		}	
		
	//	printf("String: %s Offset: %d\n", buff, len);
		return true;
	}

}
