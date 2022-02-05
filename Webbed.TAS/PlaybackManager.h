#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "InputRecord.h"
#include "Globals.h"
#include "EasyHookUtils.h"
#include "Addresses.h"

enum eBreakType
{
	BREAKTYPE_DEFAULT_NONE = 0x0,
	BREAKTYPE_FAST = 1 << 0,
	BREAKTYPE_NORMAL = 1 << 1
};

typedef struct t_BreakState
{
	// The file the break is in.
	char m_szCurrentFile[56];

	// The line number the break is in.
	unsigned int m_nLineNo;

	// The breakpoint type.
	eBreakType m_BreakType;
} BreakState;

class PlaybackManager
{
public:

	PlaybackManager(const char*);

	InputRecord* GetCurrentInput();

	InputRecord* GetCurrentInputIndexBased();

	unsigned long GetCurrentInputIndex();

	void DoPlayback(bool);

	unsigned long ReloadPlayback();

	bool ReadInputFile();

	bool ReadMultiLevelInputFile(const char*, unsigned long, unsigned long*, unsigned long*);

	void InitPlayback(bool);

	bool IsPlayingBack();

	unsigned long GetLastReadSeed();

	unsigned long GetTotalFrameCount();

	void FormatManagerString();


	char m_szCurrentManagerState[2000];

	unsigned long m_CurrentFrame;

	unsigned long m_nTotalFrameCount;

	bool m_bPlayingBack;

	bool m_bRuntoActive;

	std::string m_CWD;

	// Webbed specific stuff
	double lastMouseXInput;
	double lastMouseYInput;
	bool lastInputWasEsc;
	bool plugForJournal;
	// Webbed specific stuff

private:

	const char* m_szDefaultFileName = "Webbed.rec";

	FILE* m_pSegmentedFile;

	BreakState m_BreakState;

	char m_szCurrentDirectory[256];

	InputRecord* m_pCurrentInput;

	std::vector<InputRecord*> m_Inputs;

	unsigned long m_InputIndex;

	FILE* m_Fp;

	unsigned long m_nLastReadSeed;

	unsigned long m_RuntoLineNo;

	unsigned long m_WalktoLineNo;

	unsigned long m_TotalFrameCountOfInputFile;

	unsigned long m_FrameToNext;

	bool m_bPlaybackReady;

protected:

};
