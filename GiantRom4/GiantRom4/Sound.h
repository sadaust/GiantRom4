/*===========================
=Sound.h/.cpp				=
=by Tyler van Gastel		=
=latest update 11/10/2015	=
=Sound frame for FMOD		=
=============================*/

#pragma once
#include <fmod.hpp>
#pragma comment(lib,"Fmodex_vc.lib")
#include <string>
#include "ResourceDefines.h"

struct listenProp {
	FMOD_VECTOR pos;
	FMOD_VECTOR vel;
	FMOD_VECTOR forward;
	FMOD_VECTOR up;
};

class Sound {
private:
	FMOD::System* _sys;
	//FMOD Version
	unsigned int _ver;
	//Speaker Mode
	FMOD_SPEAKERMODE _sMode;
	FMOD_CAPS _caps;
	//sound device number
	int _device;
	//sample rate
	int _sRate;
	//max channels
	int _mChannel;
	//
	FMOD::ChannelGroup* _groups[numSTypes];
public:
	Sound();
	~Sound();

	void init();
	void restart();
	void shutdown();
	void update();
	void update(listenProp& prop);
	//get FMOD version
	unsigned int getVersion() {return _ver;}
	//sample rate
	int getSampleRate() {return _sRate;}
	//sets sample rate. call befor init or use restart to apply
	void setSampleRate(int sampleRate);
	//sound channels
	int getMaxChannels() {return _mChannel;}
	//sets max channels. call befor init or use restart to apply
	void setMaxChannels(int maxChannels);
	//speaker mode
	FMOD_SPEAKERMODE getSpeakerMode();
	void setSpeakerMode(FMOD_SPEAKERMODE speakerMode);
	//
	int getNumDevices();
	std::string getDeviceName(int deviceNum);
	

	void load(const char* name, FMOD::Sound* *sound);
	void loadStream(const char* name, FMOD::Sound* *stream);
	void setListenProp(listenProp& prop);
	void setVolume(soundType type,float volume);

	void play(soundStruct sound, vector pos, vector vel);
	void playStream(musicStruct stream, bool muted);
};