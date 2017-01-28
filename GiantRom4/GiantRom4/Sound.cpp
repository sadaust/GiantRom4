#include "Sound.h"

Sound::Sound() {
	_sys = 0;
	_ver = 0;
	_device = 0;
	_mChannel = 100;
	_sRate = 48000;
	_sMode = FMOD_SPEAKERMODE_STEREO;
	FMOD::System_Create(&_sys);
	_sys->getVersion(&_ver);
}

void Sound::setMaxChannels(int maxChannels) {
	_mChannel = maxChannels;
}

void Sound::setSampleRate(int sampleRate) {
	_sRate = sampleRate;
}

std::string Sound::getDeviceName(int devNum) {
	char temp[256];
	FMOD_RESULT res;
	res = _sys->getDriverInfo(devNum,temp,256,0);
	if(res == FMOD_OK)
		return temp;
	else
		return "ERROR";
}

int Sound::getNumDevices() {
	int temp = 0;

	if(_sys != 0)
		_sys->getNumDrivers(&temp);

	return temp;
}

FMOD_SPEAKERMODE Sound::getSpeakerMode() {
	FMOD_SPEAKERMODE out;
	out = _sMode;
	if(_sys != 0)
		_sys->getSpeakerMode(&out);
	return out;
}

void Sound::init() {
	FMOD_SPEAKERMODE mode;
	if(getNumDevices() == 0) {
		_sys->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
	} else {
		_sys->getDriverCaps(_device,&_caps,0,&mode);
		if(_caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			_sys->setDSPBufferSize(1024,10);
		}
		_sys->setSoftwareFormat(_sRate,FMOD_SOUND_FORMAT_PCMFLOAT,0,0,FMOD_DSP_RESAMPLER_LINEAR);
	}

	if(_sys->init(_mChannel,FMOD_INIT_NORMAL,0) == FMOD_ERR_OUTPUT_CREATEBUFFER) {
		_sys->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		_sys->init(_mChannel,FMOD_INIT_NORMAL,0);
	} else {
		_sys->setSpeakerMode(mode);
	}

	for(int i = 0; i < numSTypes; ++i) {
		_sys->createChannelGroup(NULL,&_groups[i]);
	}
	_sys->set3DNumListeners(1);
}

void Sound::restart() {
	//copy volumes
	float volumes[numSTypes];
	for(int i = 0; i < numSTypes; ++i) {
		_groups[i]->getVolume(volumes);
	}
	shutdown();
	FMOD::System_Create(&_sys);
	init();
	_sys->setSpeakerMode(_sMode);
	//reset volumes
	for(int i = 0; i < numSTypes; ++i) {
		_groups[i]->setVolume(volumes[i]);
	}
}

void Sound::update() {
	_sys->update();
}

void Sound::update(listenProp& prop) {
	setListenProp(prop);
	_sys->update();
}

void Sound::shutdown() {
	for(int i = 0; i < numSTypes; ++i) {
		if(_groups[i]) {
			_groups[i]->release();
			_groups[i] = 0;
		}
	}

	if(_sys != 0) {
		_sys->release();
		_sys = 0;
	}

}

Sound::~Sound() {
	shutdown();
}


void Sound::load(const char* name, FMOD::Sound **sound) {
	_sys->createSound(name,FMOD_3D|FMOD_HARDWARE,0,sound);
}

void Sound::loadStream(const char* name, FMOD::Sound **stream) {
	_sys->createStream(name,FMOD_LOOP_NORMAL|FMOD_2D|FMOD_HARDWARE,0,stream);
}

void Sound::setListenProp(listenProp& prop) {
	_sys->set3DListenerAttributes(0,&prop.pos,&prop.vel,&prop.forward,&prop.up);
}

void Sound::setVolume(soundType type,float volume) {
	if(type < numSTypes && type >= 0) {
		_groups[type]->setVolume(volume);
	}
}

void Sound::play(soundStruct sound, vector position, vector velocity) {
	FMOD::Channel* temp;
	FMOD_VECTOR pos, vel;

	pos.x = position.x;
	pos.y = position.y;
	pos.z = position.z;

	vel.x = velocity.x;
	vel.y = velocity.y;
	vel.z = velocity.z;

	//create channel
	_sys->playSound(FMOD_CHANNEL_FREE,sound.asset,false,&temp);
	//set sound properties
	temp->setChannelGroup(_groups[sound.type]);
	temp->set3DMinMaxDistance(sound.minDist,sound.maxDist);
	temp->set3DAttributes(&pos,&vel);
}

void Sound::playStream(musicStruct music, bool mute) {
	FMOD::Channel* temp;

	_sys->playSound(FMOD_CHANNEL_FREE,music.asset,mute,&temp);
	temp->setChannelGroup(_groups[music.type]);
}