#include "Sound.h"
#include <combaseapi.h>
#include <memory>
#include <vector>
using namespace MCB;
using namespace std;

SoundManager* MCB::SoundManager::GetInstance()
{
	static SoundManager inst;
	return &inst;
}

void MCB::SoundManager::ReleasexAudio2()
{
	xAudio2_.Reset();
}

void MCB::SoundManager::DeleteSound(size_t SoundHandle)
{


	sounds_[SoundHandle]->bufferSize = 0;
	sounds_[SoundHandle]->wfex = {};
	sounds_[SoundHandle]->free = true;
	sounds_[SoundHandle]->pSourceVoice = nullptr;
}

void MCB::SoundManager::AllDeleteSound()
{
	for (size_t i = 0; i < sounds_.size(); i++)
	{
		if (!sounds_[i]->pBuffer.empty())
		{
			if(sounds_[i]->pSourceVoice != nullptr) sounds_[i]->pSourceVoice->Stop();

			
			sounds_[i]->bufferSize = 0;
			sounds_[i]->wfex = {};
			sounds_[i]->free = true;
			sounds_[i]->pSourceVoice = nullptr;
		}
	}
}

SoundManager::SoundManager()
{
	HRESULT result = S_FALSE;

	//音でなかったら試してみろ！
	//result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);

	assert(SUCCEEDED(result) && "XAudioインスタンス生成時エラー");

	result = xAudio2_->CreateMasteringVoice(&masterVoice_);

}

MCB::SoundManager::~SoundManager()
{
	for (size_t i = 0; i < sounds_.size(); i++)
	{
		if (!sounds_[i]->pBuffer.empty())
		{
			if(sounds_[i]->pSourceVoice)sounds_[i]->pSourceVoice->Stop();
			sounds_[i]->bufferSize = 0;
			sounds_[i]->wfex = {};
			sounds_[i]->pSourceVoice = nullptr;
		}
	}
	sounds_.clear();
	xAudio2_.Reset();
}

size_t MCB::SoundManager::LoadWaveSound(const string& fileName)
{

	size_t handleNum = 0;
	unique_ptr<SoundData> temp = make_unique<SoundData>();

	for (size_t i = 0; i < sounds_.size(); i++)
	{
		if (sounds_.empty())break;
		if (sounds_[i]->free)continue;
		if (sounds_[i]->name == fileName) return i;
	}

	for (size_t i = 0; i < sounds_.size(); i++)
	{
		if (sounds_.empty())break;
		if (sounds_[i]->free)
		{
 			handleNum = i;
			sounds_[i]->free = false;
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

	temp->wfex = format.fmt;
	temp->pBuffer = move(pBuffer);
	temp->bufferSize = data.size;
	temp->name = fileName;
	sounds_.push_back(move(temp));
	handleNum = sounds_.size() - 1;
	return handleNum;

}

void MCB::SoundManager::PlaySoundWave(size_t soundHandle,bool isLoop, uint16_t loopCount)
{
	if (soundHandle >= sounds_.size())return;
	HRESULT result = S_FALSE;
	
	result = xAudio2_.Get()->CreateSourceVoice(&sounds_[soundHandle]->pSourceVoice, &sounds_[soundHandle]->wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = reinterpret_cast<BYTE*>(&sounds_[soundHandle]->pBuffer[0]);
	buf.AudioBytes =static_cast<uint32_t>( sounds_[soundHandle]->bufferSize);
	if (isLoop) buf.LoopCount = loopCount;
	else 
	{
		buf.LoopCount = 0;
		buf.LoopBegin = 0;
		buf.LoopLength = 0;
	}
	buf.Flags = XAUDIO2_END_OF_STREAM;

	sounds_[soundHandle]->pSourceVoice->SetVolume(sounds_[soundHandle]->volume);
	assert(SUCCEEDED(result));

	result = sounds_[soundHandle]->pSourceVoice->SubmitSourceBuffer(&buf);
	result = sounds_[soundHandle]->pSourceVoice->Start();

}

void MCB::SoundManager::StopSoundWave(size_t soundHandle,bool startPosReset)
{
	HRESULT result = S_FALSE;
	if (soundHandle >= sounds_.size())return;
	if (sounds_[soundHandle]->pSourceVoice == nullptr)return;
	result = sounds_[soundHandle]->pSourceVoice->Stop();
	sounds_[soundHandle]->pSourceVoice = nullptr;

}

void MCB::SoundManager::SetVolume(size_t volume,size_t soundHandle)
{
	float tempVolume = volume / 100.0f;
	if (soundHandle >= sounds_.size())return;
	sounds_[soundHandle]->volume = tempVolume;
	if (sounds_[soundHandle]->pSourceVoice != nullptr)
	{
		sounds_[soundHandle]->pSourceVoice->SetVolume(sounds_[soundHandle]->volume);
	}
}

