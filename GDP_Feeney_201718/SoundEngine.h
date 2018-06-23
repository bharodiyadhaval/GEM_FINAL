#pragma once
#include <AtlBase.h>
#include <atlconv.h>
#include<iostream>
#include<fmod/fmod.hpp>
#include<fmod/fmod_errors.h>
#include<string>
#include<Windows.h>
#include<fstream>
#include<sstream>
#include<string>
#include<sapi.h>

#define NUMBER_OF_TAGS 4
#define STREAM_BUFFER_SIZE 65536

using namespace FMOD;

class SoundEngine
{
public:
	FMOD_RESULT OutputResult;
	System *SoundSystem = NULL;
	Sound *Sounds[20];
	Channel *ChannelList[20];
	std::string Files[16];
	int StreamSounds = 0, SampledSounds = 0;
	bool pauser;
	std::string UncompFiles[10];
	std::string CompFiles[10];
	std::string  SampFiles[10];


	void ReadToThePoint(std::ifstream &file, std::string token);
	std::string AppendToThePoint(std::ifstream &file, std::string token);
	void ReadFiles();
	void ErrorCheck(FMOD_RESULT OutputResult);
	void Initializer();
	void SoundEngine::LoadFiles();
	void LoadCompressedStreams();
	void LoadUncompressedStreams();
	void LoadSampledSounds();
};