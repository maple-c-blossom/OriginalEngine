#include "Sound.h"


using namespace MCB;

void MCB::SoundManager::ReleasexAudio2()
{
	xAudio2.Reset();
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
	//xAudio2.Reset();
}

void MCB::Sound::LoadWaveSound(const char* fileName)
{
	HRESULT result;

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

	this->wfex = format.fmt;
	this->pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	this->bufferSize = data.size;

	

}

void MCB::Sound::DeleteSound()
{
	delete[] this->pBuffer;

	this->pBuffer = 0;
	this->bufferSize = 0;
	this->wfex = {};
}

void MCB::Sound::PlaySoundWave(bool isLoop)
{
	HRESULT result = S_FALSE;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = soundManager->xAudio2.Get()->CreateSourceVoice(&pSourceVoice, &wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = pBuffer;
	buf.AudioBytes = bufferSize;
	if (isLoop) buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	else buf.LoopCount = 0;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();

}

Sound::Sound(SoundManager* soundManager)
{
	this->soundManager = soundManager;

}