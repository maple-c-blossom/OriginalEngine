#pragma once
#include <xaudio2.h>
#include <fstream>
#include "Dx12.h"
#include <wrl.h>
#include <array>
#include <memory>

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
		}ChunkHeader;

		typedef struct RiffHeader
		{
			ChunkHeader chunk;
			char type[4];
		}RiffHeader;

		typedef struct FormatChunk
		{
			ChunkHeader chunk;
			WAVEFORMATEX fmt;
		}FormatChunk;

		typedef struct SoundData
		{
			WAVEFORMATEX wfex;
			std::unique_ptr<BYTE> pBuffer;
			size_t bufferSize;
			bool free = true;
			const std::string name;
			IXAudio2SourceVoice* pSourceVoice = nullptr;
			float volume = 1;
		}SoundData;

		Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
		IXAudio2MasteringVoice* masterVoice_;
		static const size_t sMAX_SOUND_ = 1026;

		std::vector<SoundData> sounds_;

		void ReleasexAudio2();

		size_t LoadWaveSound(const std::string& fileName);
		void DeleteSound(const size_t& SoundHandle);
		void AllDeleteSound();

		void PlaySoundWave(const size_t& soundHandle, const bool& isLoop = false,const uint16_t& loopCount = XAUDIO2_LOOP_INFINITE);
		void StopSoundWave(const size_t& soundHandle, const bool& startPosReset = true);
		//void SetVolume(size_t volume, size_t soundHandle);
		void SetVolume(const size_t& volume, const size_t& soundHandle);
		SoundManager();
		~SoundManager();

	};

}
