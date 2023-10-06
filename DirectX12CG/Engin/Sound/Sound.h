#pragma once
#include "Dx12.h"
#include "IgnoreWarning.h"
WarningIgnoreBegin
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <array>
#include <memory>
WarningIgnoreEnd



namespace MCB
{
	class SoundManager
	{

		private:
			SoundManager(const SoundManager& ) = delete;
			SoundManager& operator=(const SoundManager& )=delete;
			SoundManager();
			~SoundManager();

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
			Byte2 pad;
		}FormatChunk;

		typedef struct SoundData
		{
			WAVEFORMATEX wfex;
			Byte6 pad;
			std::vector<char> pBuffer;
			size_t bufferSize;
			bool free = true;
			Byte7 pad2;
			std::string name;
			IXAudio2SourceVoice* pSourceVoice = nullptr;
			float volume = 1;
			Byte4 pad3;
		}SoundData;

		Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
		IXAudio2MasteringVoice* masterVoice_;
		static const size_t sMAX_SOUND_ = 1026;

		std::vector < std::unique_ptr< SoundData> > sounds_;
		public:
		static SoundManager* GetInstance();
		void ReleasexAudio2();

		size_t LoadWaveSound(const std::string& fileName);
		void DeleteSound( size_t SoundHandle);
		void AllDeleteSound();

		void PlaySoundWave( size_t soundHandle,  bool isLoop = false, uint16_t loopCount = XAUDIO2_LOOP_INFINITE);
		void StopSoundWave( size_t soundHandle,  bool startPosReset = true);
		//void SetVolume(size_t volume, size_t soundHandle);
		void SetVolume( size_t volume,  size_t soundHandle);


	};

}
