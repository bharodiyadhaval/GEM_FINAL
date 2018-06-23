#include"SoundEngine.h"

void SoundEngine::ReadToThePoint(std::ifstream &file, std::string token)
{
	bool ContinueReading = true;
	std::string garbage;
	do
	{
		file >> garbage;		// Title_End??
		if (garbage == token)
		{
			return;
		}
	} while (ContinueReading);
	return;
}


std::string SoundEngine::AppendToThePoint(std::ifstream &file, std::string token)
{
	bool ContinueReading = true;
	std::string garbage;
	std::string mystring;
	do
	{
		file >> garbage;		// Title_End??
		mystring.append(garbage.c_str());
		mystring.append(" ");
		if (garbage == token)
		{
			return mystring;
		}
	} while (ContinueReading);
	return mystring;
}


void SoundEngine::ReadFiles()
{
	char* skip = new char[10];
	std::string filename;
	filename = "LoadFiles.txt";
	std::ifstream Loader(filename.c_str());
	if (!Loader.is_open())
	{
		printf("Sorry the Loader File could not be opened");
	}
	ReadToThePoint(Loader, "StreamFiles");
	Loader >> StreamSounds;

	for (int i = 0; i <= StreamSounds - 1; i++)
	{
		Loader >> Files[i];
	}

	printf("All Files Loaded Successfuly\nStream Sounds:%d \n\n", StreamSounds);

}


void SoundEngine::Initializer()
{
	FMOD_SOUND_TYPE type = FMOD_SOUND_TYPE_UNKNOWN;
	FMOD_SOUND_FORMAT format = FMOD_SOUND_FORMAT_NONE;
	OutputResult = System_Create(&SoundSystem);
	ErrorCheck(OutputResult);
	OutputResult = SoundSystem->init(512, FMOD_INIT_NORMAL, 0);
	ErrorCheck(OutputResult);
}



void SoundEngine::ErrorCheck(FMOD_RESULT OutputResult)
{
	if (OutputResult != FMOD_OK)
	{
		fprintf(stderr, "Something's wrong, Error code: %d Error Detail: %s \n", OutputResult, FMOD_ErrorString(OutputResult));
		//Return Error Code
		exit(8);
	}
}


void SoundEngine::LoadFiles()
{
	char* skip = new char[10];
	std::string filename;
	filename = "LoadFiles.txt";
	std::ifstream Loader(filename.c_str());
	if (!Loader.is_open())
	{
		printf("Sorry the Loader File could not be opened");
	}
	ReadToThePoint(Loader, "StreamFiles");
	Loader >> StreamSounds;

	ReadToThePoint(Loader, "Uncompressed");
	for (int i = 0; i <= StreamSounds - 1; i++)
	{
		Loader >> UncompFiles[i];
	}
	ReadToThePoint(Loader, "Compressed");
	for (int j = 0; j <= StreamSounds - 1; j++)
	{
		Loader >> CompFiles[j];
	}

	ReadToThePoint(Loader, "SampledFiles");
	Loader >> SampledSounds;

	ReadToThePoint(Loader, "SampledSounds");
	for (int k = 0; k <= SampledSounds - 1; k++)
	{
		Loader >> SampFiles[k];
	}

	printf("All Files Loaded Successfuly\nStream Sounds:%d \nSampled Sounds:%d\n", StreamSounds, SampledSounds);




}




void SoundEngine::LoadCompressedStreams()
{
	for (int i = 0; i <= StreamSounds - 1; i++)
	{
		OutputResult = SoundSystem->createSound(CompFiles[i].c_str(), FMOD_DEFAULT, 0, &Sounds[i]);
		ErrorCheck(OutputResult);
		OutputResult = Sounds[i]->setMode(FMOD_LOOP_NORMAL);
		OutputResult = SoundSystem->playSound(Sounds[i], 0, false, &ChannelList[i]);
		ErrorCheck(OutputResult);
		printf("Playing Compressed Track %d\n", i);
	}
}


void SoundEngine::LoadUncompressedStreams()
{
	for (int i = 0; i <= StreamSounds - 1; i++)
	{
		OutputResult = SoundSystem->createSound(UncompFiles[i].c_str(), FMOD_DEFAULT, 0, &Sounds[i]);
		ErrorCheck(OutputResult);
		OutputResult = Sounds[i]->setMode(FMOD_LOOP_NORMAL);
		OutputResult = SoundSystem->playSound(Sounds[i], 0, false, &ChannelList[i]);
		ErrorCheck(OutputResult);
		printf("Playing Uncompressed Track %d\n", i);
	}
}


void SoundEngine::LoadSampledSounds()
{
	for (int i = 0; i <= SampledSounds - 1; i++)
	{
		OutputResult = SoundSystem->createSound(SampFiles[i].c_str(), FMOD_DEFAULT, 0, &Sounds[StreamSounds + i]);
		ErrorCheck(OutputResult);
		//OutputResult = Sounds[StreamSounds + i]->setMode(FMOD_LOOP_NORMAL);
		//OutputResult = SoundSystem->playSound(Sounds[StreamSounds + i], 0, false, &ChannelList[StreamSounds + i]);
		//ErrorCheck(OutputResult);
	}
}