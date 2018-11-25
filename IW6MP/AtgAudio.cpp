//--------------------------------------------------------------------------------------
// AtgAudio.cpp
//
// Simple WAV file reader and other audio utilities.
//
// Xbox Advanced Technology Group.
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"

#include <xtl.h>
#include <ppcintrinsics.h>
#include <xgraphics.h>
#include <xboxmath.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "AtgAudio.h"


namespace ATG
{



	//--------------------------------------------------------------------------------------
	// RIFF chunk type that contains loop point information
	//--------------------------------------------------------------------------------------
	struct WAVESAMPLE
	{
		DWORD dwSize;
		WORD UnityNote;
		SHORT FineTune;
		LONG Gain;
		DWORD dwOptions;
		DWORD dwSampleLoops;
	};


	//--------------------------------------------------------------------------------------
	// Loop point (contained in WSMP chunk)
	//--------------------------------------------------------------------------------------
	struct WAVESAMPLE_LOOP
	{
		DWORD dwSize;
		DWORD dwLoopType;
		DWORD dwLoopStart;
		DWORD dwLoopLength;
	};


	//--------------------------------------------------------------------------------------
	// RIFF chunk that may contain loop point information
	//--------------------------------------------------------------------------------------
	struct SAMPLER
	{
		DWORD dwManufacturer;
		DWORD dwProduct;
		DWORD dwSamplePeriod;
		DWORD dwMIDIUnityNote;
		DWORD dwMIDIPitchFraction;
		DWORD dwSMPTEFormat;
		DWORD dwSMPTEOffset;
		DWORD dwNumSampleLoops;
		DWORD dwSamplerData;
	};


	//--------------------------------------------------------------------------------------
	// Loop point contained in SMPL chunk
	//--------------------------------------------------------------------------------------
	struct SAMPLER_LOOP
	{
		DWORD dwCuePointID;
		DWORD dwType;
		DWORD dwStart;
		DWORD dwEnd;
		DWORD dwFraction;
		DWORD dwPlayCount;
	};


	//--------------------------------------------------------------------------------------
	// Name: RiffChunk()
	// Desc: Constructor
	//--------------------------------------------------------------------------------------
	RiffChunk::RiffChunk()
	{
		// Initialize defaults
		m_fccChunkId = 0;
		m_pParentChunk = NULL;
		m_hFile = INVALID_HANDLE_VALUE;
		m_dwDataOffset = 0;
		m_dwDataSize = 0;
		m_dwFlags = 0;
	}


	//--------------------------------------------------------------------------------------
	// Name: Initialize()
	// Desc: Initializes the Riff chunk for use
	//--------------------------------------------------------------------------------------
	VOID RiffChunk::Initialize(FOURCC fccChunkId, const RiffChunk* pParentChunk,
		HANDLE hFile)
	{
		m_fccChunkId = fccChunkId;
		m_pParentChunk = pParentChunk;
		m_hFile = hFile;
	}


	//--------------------------------------------------------------------------------------
	// Name: Open()
	// Desc: Opens an existing chunk
	//--------------------------------------------------------------------------------------
	HRESULT RiffChunk::Open()
	{
		LONG lOffset = 0;

		// Seek to the first byte of the parent chunk's data section
		if (m_pParentChunk)
		{
			lOffset = m_pParentChunk->m_dwDataOffset;

			// Special case the RIFF chunk
			if (ATG_FOURCC_RIFF == m_pParentChunk->m_fccChunkId)
				lOffset += sizeof(FOURCC);
		}

		// Read each child chunk header until we find the one we're looking for
		for (; ; )
		{
			if (INVALID_SET_FILE_POINTER == SetFilePointer(m_hFile, lOffset, NULL, FILE_BEGIN))
				return HRESULT_FROM_WIN32(GetLastError());

			RIFFHEADER rhRiffHeader;
			DWORD dwRead;
			if (0 == ReadFile(m_hFile, &rhRiffHeader, sizeof(rhRiffHeader), &dwRead, NULL))
				return HRESULT_FROM_WIN32(GetLastError());
			rhRiffHeader.dwDataSize = __loadwordbytereverse(0, &rhRiffHeader.dwDataSize);

			// Hit EOF without finding it
			if (0 == dwRead)
				return E_FAIL;

			// Check if we found the one we're looking for
			if (m_fccChunkId == rhRiffHeader.fccChunkId)
			{
				// Save the chunk size and data offset
				m_dwDataOffset = lOffset + sizeof(rhRiffHeader);
				m_dwDataSize = rhRiffHeader.dwDataSize;

				// Success
				m_dwFlags |= RIFFCHUNK_FLAGS_VALID;

				return S_OK;
			}

			lOffset += sizeof(rhRiffHeader) + rhRiffHeader.dwDataSize;
		}
	}


	//--------------------------------------------------------------------------------------
	// Name: ReadData()
	// Desc: Reads from the file
	//--------------------------------------------------------------------------------------
	HRESULT RiffChunk::ReadData(LONG lOffset, VOID* pData, DWORD dwDataSize, OVERLAPPED* pOL) const
	{
		HRESULT hr = S_OK;

		OVERLAPPED defaultOL = { 0 };
		OVERLAPPED* pOverlapped = pOL;
		if (!pOL)
		{
			pOverlapped = &defaultOL;
		}

		// Seek to the offset
		pOverlapped->Offset = m_dwDataOffset + lOffset;

		// Read from the file
		DWORD dwRead;
		if (SUCCEEDED(hr) && 0 == ReadFile(m_hFile, pData, dwDataSize, &dwRead, pOverlapped))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (SUCCEEDED(hr) && !pOL)
		{
			// we're using the default overlapped structure, which means that even if the
			// read was async, we need to act like it was synchronous.
			if (!GetOverlappedResult(m_hFile, pOverlapped, &dwRead, TRUE))
				hr = HRESULT_FROM_WIN32(GetLastError());
		}
		return S_OK;
	}


	//--------------------------------------------------------------------------------------
	// Name: WaveFile()
	// Desc: Constructor
	//--------------------------------------------------------------------------------------
	WaveFile::WaveFile()
	{
		m_hFile = INVALID_HANDLE_VALUE;
	}


	//--------------------------------------------------------------------------------------
	// Name: ~WaveFile()
	// Desc: Denstructor
	//--------------------------------------------------------------------------------------
	WaveFile::~WaveFile()
	{
		Close();
	}


	//--------------------------------------------------------------------------------------
	// Name: Open()
	// Desc: Initializes the object
	//--------------------------------------------------------------------------------------
	HRESULT WaveFile::Open(const CHAR* strFileName)
	{
		// If we're already open, close
		Close();

		// Open the file
		m_hFile = CreateFile(
			strFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (INVALID_HANDLE_VALUE == m_hFile)
			return HRESULT_FROM_WIN32(GetLastError());

		// Initialize the chunk objects
		m_RiffChunk.Initialize(ATG_FOURCC_RIFF, NULL, m_hFile);
		m_FormatChunk.Initialize(ATG_FOURCC_FORMAT, &m_RiffChunk, m_hFile);
		m_DataChunk.Initialize(ATG_FOURCC_DATA, &m_RiffChunk, m_hFile);
		m_WaveSampleChunk.Initialize(ATG_FOURCC_WSMP, &m_RiffChunk, m_hFile);
		m_SamplerChunk.Initialize(ATG_FOURCC_SMPL, &m_RiffChunk, m_hFile);
		m_DpdsChunk.Initialize(ATG_FOURCC_DPDS, &m_RiffChunk, m_hFile);
		m_SeekChunk.Initialize(ATG_FOURCC_SEEK, &m_RiffChunk, m_hFile);

		HRESULT hr = m_RiffChunk.Open();
		if (FAILED(hr))
			return hr;

		hr = m_FormatChunk.Open();
		if (FAILED(hr))
			return hr;

		hr = m_DataChunk.Open();
		if (FAILED(hr))
			return hr;

		// Wave Sample and Sampler chunks are not required
		m_WaveSampleChunk.Open();
		m_SamplerChunk.Open();

		// Dpds chunk is valid only for XWMA files
		m_DpdsChunk.Open();

		// Seek chunk is valid only for XMA files
		m_SeekChunk.Open();

		// Validate the file type
		FOURCC fccType;
		hr = m_RiffChunk.ReadData(0, &fccType, sizeof(fccType), NULL);
		if (FAILED(hr))
			return hr;

		if (ATG_FOURCC_WAVE != fccType && ATG_FOURCC_XWMA != fccType)
			return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

		return S_OK;
	}


	//--------------------------------------------------------------------------------------
	// Name: GetFormat()
	// Desc: Gets the wave file format.
	//--------------------------------------------------------------------------------------
	HRESULT WaveFile::GetFormat(WAVEFORMATEXTENSIBLE* pwfxFormat, XMA2WAVEFORMATEX* pXma2Format) const
	{
		assert(pwfxFormat);

		DWORD dwValidSize = m_FormatChunk.GetDataSize();

		// Must be at least as large as a WAVEFORMAT to be valid
		if (dwValidSize < sizeof(WAVEFORMAT))
			return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);

		// Read the format chunk into the buffer
		HRESULT hr = m_FormatChunk.ReadData(0, pwfxFormat, min(dwValidSize, sizeof(WAVEFORMATEXTENSIBLE)), NULL);
		if (FAILED(hr))
			return hr;

		// Endian conversion for WAVEFORMAT base structure
		pwfxFormat->Format.wFormatTag = __loadshortbytereverse(0, &pwfxFormat->Format.wFormatTag);
		pwfxFormat->Format.nChannels = __loadshortbytereverse(0, &pwfxFormat->Format.nChannels);
		pwfxFormat->Format.nSamplesPerSec = __loadwordbytereverse(0, &pwfxFormat->Format.nSamplesPerSec);
		pwfxFormat->Format.nAvgBytesPerSec = __loadwordbytereverse(0, &pwfxFormat->Format.nAvgBytesPerSec);
		pwfxFormat->Format.nBlockAlign = __loadshortbytereverse(0, &pwfxFormat->Format.nBlockAlign);

		switch (pwfxFormat->Format.wFormatTag)
		{
		case WAVE_FORMAT_PCM:
		case WAVE_FORMAT_WMAUDIO3:
		case WAVE_FORMAT_WMAUDIO2:
			if (dwValidSize < sizeof(PCMWAVEFORMAT))
				return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);

			pwfxFormat->Format.wBitsPerSample = __loadshortbytereverse(0, &pwfxFormat->Format.wBitsPerSample);
			break;

		case WAVE_FORMAT_EXTENSIBLE:
			if (dwValidSize < sizeof(WAVEFORMATEXTENSIBLE))
				return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);

			// Endianness conversion
			pwfxFormat->Format.wBitsPerSample = __loadshortbytereverse(0, &pwfxFormat->Format.wBitsPerSample);
			pwfxFormat->Format.cbSize = __loadshortbytereverse(0, &pwfxFormat->Format.cbSize);

			pwfxFormat->Samples.wReserved = __loadshortbytereverse(0, &pwfxFormat->Samples.wReserved);
			pwfxFormat->dwChannelMask = __loadwordbytereverse(0, &pwfxFormat->dwChannelMask);
			pwfxFormat->SubFormat.Data1 = __loadwordbytereverse(0, &pwfxFormat->SubFormat.Data1);
			pwfxFormat->SubFormat.Data2 = __loadshortbytereverse(0, &pwfxFormat->SubFormat.Data2);
			pwfxFormat->SubFormat.Data3 = __loadshortbytereverse(0, &pwfxFormat->SubFormat.Data3);
			// Data4 is a array of char, not needed to convert
			break;

		case WAVE_FORMAT_XMA2:
			if (dwValidSize != sizeof(XMA2WAVEFORMATEX))
				return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);

			// Need second structure to store the result
			if (!pXma2Format)
				return HRESULT_FROM_WIN32(ERROR_INVALID_PARAMETER);

			hr = m_FormatChunk.ReadData(0, pXma2Format, sizeof(XMA2WAVEFORMATEX), NULL);
			if (FAILED(hr))
				return hr;

			// Endian conversion
			{
				pwfxFormat->Format.wBitsPerSample = __loadshortbytereverse(0, &pwfxFormat->Format.wBitsPerSample);
				pwfxFormat->Format.cbSize = __loadshortbytereverse(0, &pwfxFormat->Format.cbSize);

				pXma2Format->wfx = pwfxFormat->Format;

				pXma2Format->NumStreams = __loadshortbytereverse(0, &pXma2Format->NumStreams);
				pXma2Format->ChannelMask = __loadwordbytereverse(0, &pXma2Format->ChannelMask);
				pXma2Format->SamplesEncoded = __loadwordbytereverse(0, &pXma2Format->SamplesEncoded);
				pXma2Format->BytesPerBlock = __loadwordbytereverse(0, &pXma2Format->BytesPerBlock);
				pXma2Format->PlayBegin = __loadwordbytereverse(0, &pXma2Format->PlayBegin);
				pXma2Format->PlayLength = __loadwordbytereverse(0, &pXma2Format->PlayLength);
				pXma2Format->LoopBegin = __loadwordbytereverse(0, &pXma2Format->LoopBegin);
				pXma2Format->LoopLength = __loadwordbytereverse(0, &pXma2Format->LoopLength);
				pXma2Format->BlockCount = __loadshortbytereverse(0, &pXma2Format->BlockCount);
			}
			break;

		default:
			// Unsupported!
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
		}

		// Zero out remaining bytes, in case enough bytes were not read
		if (dwValidSize < sizeof(WAVEFORMATEXTENSIBLE))
			ZeroMemory((BYTE*)pwfxFormat + dwValidSize, sizeof(WAVEFORMATEXTENSIBLE) - dwValidSize);

		return S_OK;
	}

	//--------------------------------------------------------------------------------------
	// Name: ReadSampleRaw()
	// Desc: Reads data from the audio file. No endianness conversion is performed.
	//--------------------------------------------------------------------------------------
	HRESULT WaveFile::ReadSampleRaw(DWORD dwPosition, VOID* pBuffer,
		DWORD dwBufferSize, DWORD* pdwRead) const
	{
		// Don't read past the end of the data chunk
		DWORD dwDuration;
		GetDuration(&dwDuration);

		if (dwPosition + dwBufferSize > dwDuration)
			dwBufferSize = dwDuration - dwPosition;

		HRESULT hr = S_OK;
		if (dwBufferSize)
			hr = m_DataChunk.ReadData((LONG)dwPosition, pBuffer, dwBufferSize, NULL);

		if (pdwRead)
			*pdwRead = dwBufferSize;

		return hr;
	}


	//--------------------------------------------------------------------------------------
	// Name: ReadSample()
	// Desc: Reads data from the audio file.
	//--------------------------------------------------------------------------------------
	HRESULT WaveFile::ReadSample(DWORD dwPosition, VOID* pBuffer,
		DWORD dwBufferSize, DWORD* pdwRead) const
	{
		HRESULT hr = S_OK;
		hr = ReadSampleRaw(dwPosition, pBuffer, dwBufferSize, pdwRead);

		if (FAILED(hr))
		{
			return hr;
		}

		// Check bit size for endianness conversion.
		WAVEFORMATEXTENSIBLE wfxFormat;
		GetFormat(&wfxFormat);

		//Endianness conversion
		switch (wfxFormat.Format.wFormatTag)
		{
		case WAVE_FORMAT_PCM:
			if (wfxFormat.Format.wBitsPerSample == 16)
			{
				SHORT* pBufferShort = (SHORT*)pBuffer;
				for (DWORD i = 0; i < dwBufferSize / sizeof(SHORT); i++)
					pBufferShort[i] = __loadshortbytereverse(0, &pBufferShort[i]);
			}
			break;

		case WAVE_FORMAT_EXTENSIBLE:
			if (wfxFormat.Format.wBitsPerSample == 16)
			{
				SHORT* pBufferShort = (SHORT*)pBuffer;
				for (DWORD i = 0; i < dwBufferSize / sizeof(SHORT); i++)
					pBufferShort[i] = __loadshortbytereverse(0, &pBufferShort[i]);
			}
			else if (wfxFormat.Format.wBitsPerSample == 32)
			{
				DWORD* pBufferDWord = (DWORD*)pBuffer;
				for (DWORD i = 0; i < dwBufferSize / sizeof(DWORD); i++)
					pBufferDWord[i] = __loadwordbytereverse(0, &pBufferDWord[i]);
			}
			// 24 doesn't need converted...
			break;
		}

		return hr;
	}


	//--------------------------------------------------------------------------------------
	// Name: Close()
	// Desc: Closes the object
	//--------------------------------------------------------------------------------------
	VOID WaveFile::Close()
	{
		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}


} // namespace ATG
