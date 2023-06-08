#include "Sound.h"
#include <combaseapi.h>
#include <memory>
#include <vector>
using namespace MCB;
using namespace std;

void MCB::SoundManager::ReleasexAudio2()
{
	xAudio2.Reset();
}

void MCB::SoundManager::DeleteSound(size_t SoundHandle)
{


	sounds[SoundHandle].bufferSize = 0;
	sounds[SoundHandle].wfex = {};
	sounds[SoundHandle].free = true;
	sounds[SoundHandle].pSourceVoice = nullptr;
}

void MCB::SoundManager::AllDeleteSound()
{
	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds[i].pBuffer != nullptr)
		{
			if(sounds[i].pSourceVoice != nullptr) sounds[i].pSourceVoice->Stop();

			
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

	//音でなかったら試してみろ！
	//result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	assert(SUCCEEDED(result) && "XAudioインスタンス生成時エラー");

	result = xAudio2->CreateMasteringVoice(&masterVoice);

}

MCB::SoundManager::~SoundManager()
{
	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds[i].pBuffer != nullptr)
		{
			if(sounds[i].pSourceVoice)sounds[i].pSourceVoice->Stop();
			sounds[i].bufferSize = 0;
			sounds[i].wfex = {};
			sounds[i].pSourceVoice = nullptr;
		}
	}
	xAudio2.Reset();

}

size_t MCB::SoundManager::LoadWaveSound(const string& fileName)
{

	size_t handleNum = 0;
	SoundData temp;

	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds.empty())break;
		if (sounds[i].free)continue;
		if (sounds[i].name == fileName) return i;
	}

	for (size_t i = 0; i < sounds.size(); i++)
	{
		if (sounds.empty())break;
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

	vector<char> pBuffer (data.size);
	file.read(&pBuffer.front(), data.size);

	file.close();

	temp.wfex = format.fmt;
	temp.pBuffer.reset((reinterpret_cast<BYTE*>(pBuffer.data())));
	temp.bufferSize = data.size;
	temp.name = fileName;
	sounds.push_back(move(temp));
	handleNum = sounds.size() - 1;
	return handleNum;

}

void MCB::SoundManager::PlaySoundWave(size_t soundHandle,bool isLoop, uint16_t loopCount)
{
	if (soundHandle >= sounds.size())return;
	HRESULT result = S_FALSE;
	
	result = xAudio2.Get()->CreateSourceVoice(&sounds[soundHandle].pSourceVoice, &sounds[soundHandle].wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = sounds[soundHandle].pBuffer.get();
	buf.AudioBytes =static_cast<uint32_t>( sounds[soundHandle].bufferSize);
	if (isLoop) buf.LoopCount = loopCount;
	else 
	{
		buf.LoopCount = 0;
		buf.LoopBegin = 0;
		buf.LoopLength = 0;
	}
	buf.Flags = XAUDIO2_END_OF_STREAM;

	sounds[soundHandle].pSourceVoice->SetVolume(sounds[soundHandle].volume);
	assert(SUCCEEDED(result));

	result = sounds[soundHandle].pSourceVoice->SubmitSourceBuffer(&buf);
	result = sounds[soundHandle].pSourceVoice->Start();

}

void MCB::SoundManager::StopSoundWave(size_t soundHandle,bool startPosReset)
{
	HRESULT result = S_FALSE;
	if (soundHandle >= sounds.size())return;
	if (sounds[soundHandle].pSourceVoice == nullptr)return;
	result = sounds[soundHandle].pSourceVoice->Stop();
	sounds[soundHandle].pSourceVoice = nullptr;

}

void MCB::SoundManager::SetVolume(size_t volume,size_t soundHandle)
{
	float tempVolume = volume / 100.0f;
	if (soundHandle >= sounds.size())return;
	this->sounds[soundHandle].volume = tempVolume;
	if (sounds[soundHandle].pSourceVoice != nullptr)
	{
		sounds[soundHandle].pSourceVoice->SetVolume(sounds[soundHandle].volume);
	}
}

