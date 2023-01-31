#pragma once
#include <xaudio2.h>
#include <fstream>
#include "Dx12.h"
#include <wrl.h>
#include <array>

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

		typedef struct SoundData
		{
			WAVEFORMATEX wfex;
			BYTE* pBuffer;
			unsigned int bufferSize;
			bool free = true;
			const char* name;
			IXAudio2SourceVoice* pSourceVoice = nullptr;
			float volume = 1;
		};

		Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
		IXAudio2MasteringVoice* masterVoice;
		static const int MaxSound = 1026;

		std::array<SoundData, MaxSound> sounds;

		void ReleasexAudio2();

		unsigned int LoadWaveSound(const char* fileName);
		void DeleteSound(unsigned int SoundHandle);
		void AllDeleteSound();

		void PlaySoundWave(unsigned int soundHandle, bool isLoop = false,unsigned short loopCount = XAUDIO2_LOOP_INFINITE);
		void StopSoundWave(unsigned int soundHandle, bool startPosReset = true);
		//void SetVolume(unsigned int volume, unsigned int soundHandle);
		void SetVolume(unsigned int volume, unsigned int soundHandle);
		SoundManager();
		~SoundManager();

	};

}
