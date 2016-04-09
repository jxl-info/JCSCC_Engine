/*******************************************************************
*                   JCSCC_Framework Version 1.0
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*   See LICENSE.txt for modification and distribution information
*		        Copyright (c) 2016 by Shen, Jen-Chieh
******************************************************************/

#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include "GameTool_StdAfx.h"

#include <GameSound_JCS\DirectSound\Sound.h>
#include <GameSound_JCS\DirectSound\SoundLayer.h>

namespace JCS_GameTool
{
    class AudioXML_Loader;

    //------------------------------------------------------------------------------------
    // Name : SoundManager 
    //
    // Desc : Control all the Sound Effect in the Program(Game). 
    //        Should have relationship with "ResourceManager".
    //------------------------------------------------------------------------------------
	class SoundManager
	{
	private:
		static SoundManager* s_pSoundManager;
		SoundLayer* mCSoundLayer;		// for sound
		Sound* backgroundMusic;
        Sound* sound1;
        Sound* sound2;

        AudioXML_Loader* m_pAudioXMLLoader;

		// Constructor
		SoundManager(HWND);

	public:
		static SoundManager* getInstance(HWND hWnd)
		{
            if (!s_pSoundManager)
                s_pSoundManager = new SoundManager(hWnd);
            return s_pSoundManager;
		}
		virtual ~SoundManager();

		void playBGM(TCHAR*, bool bLoop = true);	// 循環播放
		void playOneShot(TCHAR*);					// 只播放一次
		void playCompleteShot(TCHAR*);				// 播放一次完整的以後才能在播放

		//setter

		// getter
		SoundLayer* getSoundLayer() { return this->mCSoundLayer; }

	};

}

typedef JCS_GameTool::SoundManager SoundManager;

#endif // __SOUNDMANAGER_H__

