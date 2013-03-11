/*
================================
 soundmanager.h

 Created on: 28 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

#include "waveheader.h"
/*
================================
 
 SoundManager

 A class to play sounds using DirectSound.
 
================================
*/
class SoundManager
{
public:
	SoundManager(void);
	~SoundManager(void);

	bool init(HWND);
	void shutdown();

private:
	IDirectSound8* directSound_;
	IDirectSoundBuffer* primaryBuffer_;
	IDirectSoundBuffer8* secondaryBuffer_;

	bool initDirectSound(HWND);
	void shutdownDirectSound();
	bool loadWaveFile(char*, IDirectSoundBuffer8**);
	void shutdownWaveFile(IDirectSoundBuffer8**);
	bool playWaveFile();
};

