#pragma once
#include <xaudio2.h>
#include <fstream>
#include "Dx12.h"
#include <wrl.h>


#pragma comment(lib,"xaudio2.lib")

namespace MCB
{
	class SoundManager
	{
	public:
		typedef struct ChunkHeader
		{
			char id[4];
			int32_t size;
		};

		typedef struct RiffHeader
		{
			ChunkHeader chunk;
			char type[4];
		};

		typedef struct FormatChunk
		{
			ChunkHeader chunk;
			WAVEFORMATEX fmt;
		};

		Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
		IXAudio2MasteringVoice* masterVoice;

		void ReleasexAudio2();

		SoundManager();
		~SoundManager();
	};

	class Sound
	{
	public:
		SoundManager* soundManager;
		WAVEFORMATEX wfex;
		BYTE* pBuffer;
		unsigned int bufferSize;

		Sound(SoundManager* soundManager);
		void LoadWaveSound(const char* fileName);
		void DeleteSound();

		void PlaySoundWave(bool isLoop);

	};

}
