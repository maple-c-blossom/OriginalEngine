#include "Sound.h"


using namespace MCB;

void MCB::SoundManager::ReleasexAudio2()
{
	xAudio2.Reset();
}

void MCB::SoundManager::DeleteSound(unsigned int SoundHandle)
{
	delete[] sounds[SoundHandle].pBuffer;

	sounds[SoundHandle].pBuffer = 0;
	sounds[SoundHandle].bufferSize = 0;
	sounds[SoundHandle].wfex = {};
	sounds[SoundHandle].free = true;
	sounds[SoundHandle].pSourceVoice = nullptr;
}

void MCB::SoundManager::AllDeleteSound()
{
	for (int i = 0; i < MaxSound; i++)
	{
		if (sounds[i].pBuffer != nullptr)
		{
			delete[] sounds[i].pBuffer;

			sounds[i].pBuffer = 0;
			sounds[i].bufferSize = 0;
			sounds[i].wfex = {};
			sounds[i].free = true;
			sounds[i].pSourceVoice = nullptr;
		}
	}
}

SoundManager::SoundManager()
{
	HRESULT result = S_FALSE;

	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	assert(SUCCEEDED(result) && "XAudioインスタンス生成時エラー");

	result = xAudio2->CreateMasteringVoice(&masterVoice);

}

MCB::SoundManager::~SoundManager()
{
	xAudio2.Reset();
	for (int i = 0; i < MaxSound; i++)
	{
		if (sounds[i].pBuffer != nullptr)
		{
			delete[] sounds[i].pBuffer;

			sounds[i].pBuffer = 0;
			sounds[i].bufferSize = 0;
			sounds[i].wfex = {};
			sounds[i].pSourceVoice = nullptr;
		}
	}

}

unsigned int MCB::SoundManager::LoadWaveSound(const char* fileName)
{
	HRESULT result;
	unsigned int handleNum = 0;

	for (int i = 0; i < MaxSound; i++)
	{
		if (sounds[i].free)continue;
		if (sounds[i].name == fileName) return i;
	}

	for (int i = 0; i < MaxSound; i++)
	{
		if (sounds[i].free)
		{
 			handleNum = i;
			sounds[i].free = false;
			break;
		}
	}
	

	std::ifstream file;
	file.open(fileName, std::ios_base::binary);

	assert(file.is_open() && "SoundLoadError");

	SoundManager::RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	SoundManager::FormatChunk format{};
	file.read((char*)&format, sizeof(format.chunk));

	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	SoundManager::ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0)
	{
		assert(0);
	}

	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	file.close();

	sounds[handleNum].wfex = format.fmt;
	sounds[handleNum].pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	sounds[handleNum].bufferSize = data.size;
	sounds[handleNum].name = fileName;
	result = xAudio2.Get()->CreateSourceVoice(&sounds[handleNum].pSourceVoice, &sounds[handleNum].wfex);
	assert(SUCCEEDED(result));
	return handleNum;

}

void MCB::SoundManager::PlaySoundWave(unsigned int soundHandle,bool isLoop, unsigned short loopCount)
{
	HRESULT result = S_FALSE;
	//IXAudio2SourceVoice* pSourceVoice = nullptr;
	//result =xAudio2.Get()->CreateSourceVoice(&pSourceVoice, &sounds[soundHandle].wfex);
	//assert(SUCCEEDED(result));
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = sounds[soundHandle].pBuffer;
	buf.AudioBytes = sounds[soundHandle].bufferSize;
	if (isLoop) buf.LoopCount = loopCount;
	else 
	{
		sounds[soundHandle].pSourceVoice->ExitLoop();
		buf.LoopCount = 0;
		buf.LoopBegin = 0;
		buf.LoopLength = 0;
	}
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = sounds[soundHandle].pSourceVoice->SubmitSourceBuffer(&buf);
	result = sounds[soundHandle].pSourceVoice->Start();

}

void MCB::SoundManager::StopSoundWave(unsigned int soundHandle,bool startPosReset)
{
	HRESULT result = S_FALSE;

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = sounds[soundHandle].pBuffer;
	buf.AudioBytes = sounds[soundHandle].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (buf.LoopCount > 1)
	{
		buf.LoopCount = 0;
		sounds[soundHandle].pSourceVoice->ExitLoop();
	}
	result = sounds[soundHandle].pSourceVoice->SubmitSourceBuffer(&buf);
	result = sounds[soundHandle].pSourceVoice->Stop();
	//if (startPosReset)
	//{
		result = xAudio2.Get()->CreateSourceVoice(&sounds[soundHandle].pSourceVoice, &sounds[soundHandle].wfex);
		assert(SUCCEEDED(result));
		sounds[soundHandle].pSourceVoice->SetVolume(sounds[soundHandle].volume);
	//}
}

void MCB::SoundManager::SetVolume(unsigned int volume,unsigned int soundHandle)
{
	float tempVolume = volume / 100.0f;
	this->sounds[soundHandle].volume = tempVolume;
	HRESULT result = S_FALSE;
	result = xAudio2.Get()->CreateSourceVoice(&sounds[soundHandle].pSourceVoice, &sounds[soundHandle].wfex);
	sounds[soundHandle].pSourceVoice->SetVolume(sounds[soundHandle].volume);
	assert(SUCCEEDED(result));
}

