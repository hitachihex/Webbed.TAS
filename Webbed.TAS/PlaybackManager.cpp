#include "pch.h"
#include "DebugTools.h"
#include "Addresses.h"
#include "Webbed.TAS.h"
#include "PlaybackManager.h"
#include "GMLRoom.h"


PlaybackManager::PlaybackManager(const char* pszFileName)
{
	this->m_bRuntoActive = false;
	this->plugForJournal = false;
	this->lastInputWasEsc = false;
	//yygsForManager.Set("PlayerAbility");
	this->m_BreakState.m_BreakType = eBreakType::BREAKTYPE_DEFAULT_NONE;
	memset(&this->m_BreakState.m_szCurrentFile[0], 0, 56);
	this->m_BreakState.m_nLineNo = -1;
	this->lastMouseXInput = 0.0;
	this->lastMouseYInput = 0.0;
	this->m_nTotalFrameCount = 0;
	this->m_nLastReadSeed = 16;
	this->m_Fp = NULL;
	this->m_pSegmentedFile = NULL;
	this->m_bPlaybackReady = false;
	this->m_bPlayingBack = false;
	memset(&this->m_szCurrentManagerState[0], 0, 2000);

	auto nLen = GetModuleFileNameA(nullptr, this->m_szCurrentDirectory, 256);
	this->m_CWD.assign(this->m_szCurrentDirectory);
	auto indexOfLastBackSlash = this->m_CWD.rfind("\\");
	this->m_CWD.erase(indexOfLastBackSlash + 1, this->m_CWD.length() - indexOfLastBackSlash + 1);
	this->m_CWD += "\\Includes\\";

	if (pszFileName)
	{
		// _fsopen for _SH_DENYNO for shared access.
		this->m_Fp = _fsopen(pszFileName, "r", _SH_DENYNO);
	}
}


bool PlaybackManager::ReadMultiLevelInputFile(const char* _szFileName, unsigned long otherFileCount, unsigned long* pOutRecordsRead, unsigned long* pOutLinesRead)
{
	char LineBuffer[2048] = { 0 };
	unsigned int linecount = otherFileCount;

	unsigned int otherLineCount = 0;

	if (this->m_pSegmentedFile)
	{
		fclose(this->m_pSegmentedFile);
		this->m_pSegmentedFile = nullptr;
	}

	std::string pathToCurrentFile = this->m_CWD;
	pathToCurrentFile += _szFileName;

	//DebugOutput("PathToCurrentFile = %s", pathToCurrentFile.c_str());

	this->m_pSegmentedFile = _fsopen(pathToCurrentFile.c_str(), "r", _SH_DENYNO);

	if (this->m_pSegmentedFile == nullptr)
	{
		DebugOutput("!this->m_pSegmentedFile, null pointer from _fsopen , errno=%u", errno);
		return false;
	}

	rewind(this->m_pSegmentedFile);

	while (true)
	{
		if (fgets(LineBuffer, 2048, this->m_pSegmentedFile) == NULL)
			break;

		LineBuffer[strcspn(LineBuffer, "\n")] = 0;

		if (strlen(LineBuffer) == 0)
		{
			++linecount;
			++otherLineCount;
			memset(LineBuffer, 0, sizeof(LineBuffer) / sizeof(LineBuffer[0]));
			continue;
		}

		if (LineBuffer[0] == '#')
		{
			++linecount;
			++otherLineCount;
			memset(LineBuffer, 0, sizeof(LineBuffer) / sizeof(LineBuffer[0]));
			continue;
		}

		std::string stringBuffer(LineBuffer);

		unsigned long long indexRunto = stringBuffer.find("Runto");
		unsigned long long  indexWalkto = stringBuffer.find("Walkto");


		if (indexRunto != std::string::npos)
		{
			// ADDED - Setting break state variables
			strcpy(this->m_BreakState.m_szCurrentFile, _szFileName);
			this->m_BreakState.m_nLineNo = otherLineCount;
			this->m_BreakState.m_BreakType = eBreakType::BREAKTYPE_FAST;

			this->m_RuntoLineNo = otherLineCount;
			// still increase linecount
			linecount++;
			otherLineCount++;


			continue;
		}
		else if (indexWalkto != std::string::npos)
		{
			// ADDED - Setting break state variables
			strcpy(this->m_BreakState.m_szCurrentFile, _szFileName);
			this->m_BreakState.m_nLineNo = otherLineCount;
			this->m_BreakState.m_BreakType = eBreakType::BREAKTYPE_NORMAL;

			this->m_WalktoLineNo = otherLineCount;
			// still increase linecount
			linecount++;
			otherLineCount++;


			continue;
		}

		// Ok, we know he is multi leveled because we are inside ReadMultiLevelInputFile.

		InputRecord* p = new InputRecord(std::string(LineBuffer), ++linecount, _szFileName, ++otherLineCount);

		if (p->m_Frames == -1)
		{
			delete p;
			memset(LineBuffer, 0, sizeof(LineBuffer) / sizeof(LineBuffer[0]));
			continue;
		}

		this->m_nTotalFrameCount += p->m_Frames;
		this->m_Inputs.push_back(p);
		++* pOutRecordsRead;
		memset(LineBuffer, 0, sizeof(LineBuffer) / sizeof(LineBuffer[0]));
	}

	fclose(this->m_pSegmentedFile);
	this->m_pSegmentedFile = nullptr;

	return true;
}

bool PlaybackManager::ReadInputFile()
{
	bool first = true;

	char LineBuffer[2048] = { 0 };
	unsigned int linecount = 0;

	if (this->m_Fp == NULL)
		return false;

	rewind(this->m_Fp);
	this->m_Inputs.clear();

	try
	{

		while (true)
		{

			if (fgets(LineBuffer, 2048, this->m_Fp) == NULL)
				break;

			// Remove the newline
			LineBuffer[strcspn(LineBuffer, "\n")] = 0;

			if (strlen(LineBuffer) == 0)
			{
				// still increase our linecount
				++linecount;
				memset(LineBuffer, 0, sizeof(LineBuffer) / sizeof(LineBuffer[0]));
				continue;
			}

			if (LineBuffer[0] == '#')
			{
				// still increase our linecount
				++linecount;
				memset(LineBuffer, 0, sizeof(LineBuffer) / sizeof(LineBuffer[0]));
				continue;
			}

			std::string stringBuffer(LineBuffer);
			unsigned int indexRunto = stringBuffer.find("Runto");
			unsigned int indexWalkto = stringBuffer.find("Walkto");
			unsigned int indexRead = stringBuffer.find("Read");

			if (indexRunto != std::string::npos)
			{
				strcpy(this->m_BreakState.m_szCurrentFile, this->m_szDefaultFileName);
				this->m_BreakState.m_nLineNo = linecount;
				this->m_RuntoLineNo = linecount;
				// still increase linecount
				linecount++;
				continue;
			}
			else if (indexWalkto != std::string::npos)
			{
				strcpy(this->m_BreakState.m_szCurrentFile, this->m_szDefaultFileName);
				this->m_BreakState.m_nLineNo = linecount;
				this->m_WalktoLineNo = linecount;
				// still increase linecount
				linecount++;
				continue;
			}

			if (indexRead != std::string::npos)
			{
				unsigned int iod = stringBuffer.find(",");
				if (iod != std::string::npos)
				{
					std::string fn = stringBuffer.substr(iod + 1);
					unsigned long inRecordsRead = 0, outLinesRead = 0;

					// yeah, so our out lines read is what we need, i think
					bool multiResult = this->ReadMultiLevelInputFile(fn.c_str(), linecount, &inRecordsRead, &outLinesRead);

					//if (multiResult)
						//DebugOutput("Read %u InputRecords from %s", inRecordsRead, fn.c_str());

					linecount++;
					continue;
				}

			}

			try
			{

				InputRecord* p = new InputRecord(std::string(LineBuffer), ++linecount, this->m_szDefaultFileName, 0);
				this->m_nTotalFrameCount += p->m_Frames;
				this->m_Inputs.push_back(p);
			}
			catch (std::exception& e)
			{
				DebugOutput("Caught exception: %s", e.what());
			}


			memset(LineBuffer, 0, sizeof(LineBuffer) / sizeof(LineBuffer[0]));

		}

	}
	// shut up c4101
#pragma warning(disable : 4101)
	catch (std::exception& e)
	{

	}

	return true;
}

unsigned long PlaybackManager::GetTotalFrameCount()
{
	return this->m_nTotalFrameCount;
}

void PlaybackManager::InitPlayback(bool bReload = true)
{
	// Init playback is resetting this shit
	this->m_BreakState.m_nLineNo = -1;

	this->m_RuntoLineNo = -1;
	this->m_WalktoLineNo = -1;
	this->m_nTotalFrameCount = 0;

	if (this->m_bPlayingBack && bReload)
	{
		// if we are cancelling playback, set lastInputWasEsc to false.
		this->plugForJournal = false;
		this->m_bPlayingBack = false;
		this->m_bPlaybackReady = false;
		memset(&m_szCurrentManagerState[0], 0, 2000);
		this->lastInputWasEsc = false;

		return;
	}


	bool result = this->ReadInputFile();

	if (!result)
	{
		DebugOutput("failed to read input file");
		return;
	}


	this->m_bPlayingBack = true;

	this->m_CurrentFrame = 0;
	this->m_InputIndex = 0;

	if (this->m_Inputs.size() > 0)
	{
		this->m_pCurrentInput = this->m_Inputs[0];
		this->m_FrameToNext = m_pCurrentInput->m_Frames;
	}
	else
	{
		this->m_FrameToNext = 1;
		// Disable playback
		this->m_bPlaybackReady = false;
		this->m_bPlayingBack = false;
		return;
	}

	this->m_bPlaybackReady = true;
}

unsigned long PlaybackManager::ReloadPlayback()
{
	// Save it
	unsigned long dwPlayedBackFrames = this->m_CurrentFrame;
	this->InitPlayback(false);

	// Restore it
	this->m_CurrentFrame = dwPlayedBackFrames;

	// Step on the index until we get  back to where we were.
	while (this->m_CurrentFrame > this->m_FrameToNext)
	{
		if (this->m_InputIndex + 1 >= this->m_Inputs.size())
		{
			this->m_InputIndex++;
			return this->m_Inputs.size();
		}

		this->m_pCurrentInput = this->m_Inputs[++this->m_InputIndex];
		this->m_FrameToNext += this->m_pCurrentInput->m_Frames;
	}

	return this->m_Inputs.size();
}

bool PlaybackManager::IsPlayingBack()
{
	return this->m_bPlayingBack;
}

unsigned long PlaybackManager::GetCurrentInputIndex()
{
	return this->m_InputIndex;
}


unsigned long PlaybackManager::GetLastReadSeed()
{
	return this->m_nLastReadSeed;
}

InputRecord* PlaybackManager::GetCurrentInput()
{
	return this->m_pCurrentInput;
}


void PlaybackManager::DoPlayback(bool wasFramestepped)
{
	if (!this->m_bPlayingBack)
		return;


	InputRecord* prev = nullptr;
	InputRecord* next = nullptr;

	if (this->m_InputIndex < this->m_Inputs.size())
	{
		if (wasFramestepped)
		{
			unsigned long OldInputDoneCount = m_pCurrentInput->m_Done;
			unsigned long ReloadedCount = this->ReloadPlayback();
			m_pCurrentInput->m_Done += OldInputDoneCount;
		}

		if (this->m_CurrentFrame >= this->m_FrameToNext)
		{
			if (this->m_InputIndex + 1 >= this->m_Inputs.size())
			{
				if (wasFramestepped)
				{
					unsigned long ReloadedCountScope2 = this->ReloadPlayback();
					if (this->m_InputIndex + 1 >= ReloadedCountScope2)
					{
						this->m_InputIndex++;

						// disable playback
						this->m_bPlaybackReady = false;
						this->m_bPlayingBack = false;
						memset(&m_szCurrentManagerState[0], 0, 2000);

						return;
					}
				}
				else
				{
					if (this->m_InputIndex + 1 >= this->m_Inputs.size())
					{
						this->m_InputIndex++;

						// Disable playback
						this->m_bPlaybackReady = false;
						this->m_bPlayingBack = false;
						memset(&m_szCurrentManagerState[0], 0, 2000);

						return;
					}
				}

			} // index + 1 > = inputs.size() scope end

			this->m_pCurrentInput = this->m_Inputs[++this->m_InputIndex];

			if (this->m_InputIndex > 0)
				prev = this->m_Inputs[this->m_InputIndex - 1];

			// Seed set

			/*
			if (m_pCurrentInput->m_nSeed != -1)
			{
				g_dwConstantSeed = m_pCurrentInput->m_nSeed;
				g_mt1997.seed(m_pCurrentInput->m_nSeed);
				g_pUnifDist->reset();
				this->m_nLastReadSeed = m_pCurrentInput->m_nSeed;
			}*/

			if (this->m_BreakState.m_nLineNo != -1)
			{
				if (this->m_BreakState.m_BreakType == eBreakType::BREAKTYPE_FAST)
				{
					// it's a multi level file, and we're *IN* the file
					if (this->m_pCurrentInput->m_bMultiLevelFile &&
						!strcmpi(this->m_pCurrentInput->m_szFromFile, this->m_BreakState.m_szCurrentFile))
					{
						// Right now we haven't reached the internal line number, just fast forward, unless it's a slow.
						if (this->m_pCurrentInput->m_nInternalLineNo < this->m_BreakState.m_nLineNo)
						{
							// Is slow, don't speed up.
							if (this->m_pCurrentInput->IsSlow()) { g_GameSpeedPlayerStep = 1;   this->m_bRuntoActive = false;/*SetRoomSpeed(60);*/  SetDrawEnabled(false); }

							// Isn't slow, speed up.
							else { 
								g_GameSpeedPlayerStep = 120;
								//GameSetSpeed(500.0);
								/*SetRoomSpeed(9999)*/; 
								SetDrawEnabled(false); 
								this->m_bRuntoActive = true; 

							}
						}
						else
						{
							g_GameSpeedPlayerStep = 1;
							// we've met the breakpoint condition, stop!
							this->m_BreakState.m_nLineNo = -1;
							this->m_RuntoLineNo = -1;
							g_bPaused = true;
							SetDrawEnabled(true);
							this->m_bRuntoActive = false;
							//SetRoomSpeed(60);
							//GameSetSpeed(60.0);
	
						}
					}
					else
					{
						// It's not a multi level file but we have a break point set.
						// Handle this!
					}
				}
				// walkto
				else
				{
					// it's a multi level file, and we're *IN* the file
					if (this->m_pCurrentInput->m_bMultiLevelFile &&
						!strcmpi(this->m_pCurrentInput->m_szFromFile, this->m_BreakState.m_szCurrentFile))
					{
						// Right now we haven't reached the internal line number, just keep the normal speed.
						if (this->m_pCurrentInput->m_nInternalLineNo < this->m_BreakState.m_nLineNo)
						{
							g_GameSpeedPlayerStep = 1;
							//GameSetSpeed(60.0);
						}
						else
						{
							// we've met the breakpoint condition, stop!
							this->m_WalktoLineNo = -1;
							this->m_BreakState.m_nLineNo = -1;
							g_bPaused = true;
							SetDrawEnabled(true);
							//SetRoomSpeed(60);
							g_GameSpeedPlayerStep = 1;
							//GameSetSpeed(60.0);
						}
					}
				}
			}
			/*
			if (this->m_RuntoLineNo != -1)
			{
				if (m_pCurrentInput->m_LineNo < this->m_RuntoLineNo)
				{
					if (this->m_pCurrentInput->IsSlow()) {
						g_GameSpeedPlayerStep = 1;
						SetDrawEnabled(true);
					}
					else {
						//SetRoomSpeed(123400000);
						g_GameSpeedPlayerStep = 100;
						SetDrawEnabled(false);
						//g_bRuntoActive = true;
					}
				}
				else
				{
					this->m_RuntoLineNo = -1;
					g_bPaused = true;
					//g_bRuntoActive = false;
					SetDrawEnabled(true);
					g_GameSpeedPlayerStep = 1;
				}
			}
			else if (this->m_WalktoLineNo != -1)
			{
				if (m_pCurrentInput->m_LineNo < this->m_WalktoLineNo)
					g_GameSpeedPlayerStep = 1;
				else
				{
					this->m_WalktoLineNo = -1;
					g_bPaused = true;
					//SetRoomSpeed(60);
					g_GameSpeedPlayerStep = 1;
				}
			}*/

			this->m_FrameToNext += this->m_pCurrentInput->m_Frames;
		} // frame to next scope end
		else
		{
			this->m_pCurrentInput->m_Done++;
		}

		//  Increase current frame.
		this->m_CurrentFrame++;


		if ((this->m_InputIndex + 1) < this->m_Inputs.size())
			next = this->m_Inputs[this->m_InputIndex + 1];


		// make sure create didn't satisfy this, make a toggle or some crap
		if (m_pCurrentInput->hasPos)
		{
			if (g_pBottomBranch) {
				g_pBottomBranch->m_fX = m_pCurrentInput->posX;
				g_pBottomBranch->m_fY = m_pCurrentInput->posY;
				//g_pBottomBranch->m_pPhysProp->physInst->m_fPhysY = m_pCurrentInput->posY;
			}
		}

		if (m_pCurrentInput->hasAngle) {
			//this->lastMouseXInput = m_pCurrentInput->mouseX;
			//this->lastMouseYInput = m_pCurrentInput->mouseY;
			/*
			if (g_pIOObject) {
				double* mode = g_pIOObject->get_dbl_ptr(0x18A10);
				*mode = 1.0;
				double * rs_h = g_pIOObject->get_dbl_ptr(RSTICK_HORIZONTAL_INDEX);
				double * rs_v = g_pIOObject->get_dbl_ptr(RSTICK_VERTICAL_INDEX);

				
				double rads = (m_pCurrentInput->angle * (MPI / 180.0));
					*rs_h = sin(rads);
					*rs_v = -cos(rads);
			}*/
		}

		if (m_pCurrentInput->hasMaxWalkSpeed) {
			if (g_pPlayerObject) {
				//18A00
				auto max_walk = g_pPlayerObject->get_dbl_ptr(0x18A00);
				*max_walk = m_pCurrentInput->maxWalkSpeed;
			}
		}

		if (m_pCurrentInput->hasPullPower) {
			if (g_pPlayerObject) {
				//VarName with id 00018AFC and index 61 is pull_power
				auto pull_power = g_pPlayerObject->get_dbl_ptr(0x18AFC);
				*pull_power = m_pCurrentInput->pullPower;
			}
		}
		if (m_pCurrentInput->IsRoomGoto())
		{
			// reset wave counter.
			//g_WaveTimeCounter = 0.0;
			//room_goto(m_pCurrentInput->roomGoto);

		}

		if (m_pCurrentInput->IsRoomRestart())
		{
			// reset wave counter.
			//g_WaveTimeCounter = 0.0;
			//room_restart();

		}

		if (m_pCurrentInput->IsLeft())
		{
			HoldKey('A', 0x01);
		}

		
		if (m_pCurrentInput->IsRight())
		{
			PressKey('D', 0x01);
			ReleaseKey('D', 0x00);
			HoldKey('D', 0x01);
			HoldKey(VK_RIGHT, 0x01);
		}

		if (m_pCurrentInput->IsUp())
		{
			HoldKey('W', 0x01);
		}

		if (m_pCurrentInput->IsDown())
		{
			HoldKey('S', 0x01);
		}

		if (m_pCurrentInput->IsEscape()) {
			this->lastInputWasEsc = true;
			PressKey(VK_ESCAPE, 0x01);
		}

		if (m_pCurrentInput->IsUpPress())
			PressKey('W', 0x01);
		if (m_pCurrentInput->IsDownPress())
			PressKey('S', 0x01);
		if (m_pCurrentInput->IsLeftPress())
			PressKey('A', 0x01);
		if (m_pCurrentInput->IsRightPress())
			PressKey('D', 0x01);

		if (m_pCurrentInput->IsInteract())
			PressKey('F', 0x01);

		if (m_pCurrentInput->IsJournal())
			PressKey(VK_TAB, 0x01);

		if (m_pCurrentInput->IsJumpPress())
			PressKey(0x20, 0x01);

		if (m_pCurrentInput->hasPlugJournal) {
			this->plugForJournal = true;
		}
		else if (m_pCurrentInput->hasUnplugJournal) {
			this->plugForJournal = false;
		}

		/*
		if (m_pCurrentInput->IsUnJump())
		{
			PressKey(0x20, 0x01);
			//HoldKey(0x20, 0x00);
			ReleaseKey(0x20, 0x01);
		}*/

		// ---------------------------------------------------









		// Handle holding \ pressing of dance
		if (m_pCurrentInput->IsDance())
		{

			if (m_pCurrentInput->m_Done == 0 || (this->m_InputIndex == 0 && m_pCurrentInput->m_Done == 1))
			{
				if (prev != nullptr)
				{
					// Previous wasn't jump, we can press it again.
					if (!prev->IsDance())
						PressKey('Q', 0x01);
				}
				else
				{
					PressKey('Q', 0x01);
				}

			}

			if (m_pCurrentInput->m_Done < this->m_pCurrentInput->m_Frames || (this->m_InputIndex == 0 && (m_pCurrentInput->m_Done <= this->m_pCurrentInput->m_Frames)))
			{
				HoldKey('Q', 0x01);
				//ReleaseKey('G', 0x00);
			}
			else if (next != nullptr)
			{
				if (next->IsDance())
					HoldKey('Q', 0x01);
				//ReleaseKey('G', 0x00);
			}


		}

		// Handle release of dance
		if (!m_pCurrentInput->IsDance() && prev != nullptr)
		{

			if (prev->IsDance())
				ReleaseKey('Q', 0x01);
		}


		// Handle holding \ pressing of laser
		if (m_pCurrentInput->IsLaser())
		{

			if (m_pCurrentInput->m_Done == 0 || (this->m_InputIndex == 0 && m_pCurrentInput->m_Done == 1))
			{
				if (prev != nullptr)
				{
					// Previous wasn't jump, we can press it again.
					if (!prev->IsLaser())
						PressKey('E', 0x01);
				}
				else
				{
					PressKey('E', 0x01);
				}

			}

			if (m_pCurrentInput->m_Done < this->m_pCurrentInput->m_Frames || (this->m_InputIndex == 0 && (m_pCurrentInput->m_Done <= this->m_pCurrentInput->m_Frames)))
			{
				HoldKey('E', 0x01);
				//ReleaseKey('G', 0x00);
			}
			else if (next != nullptr)
			{
				if (next->IsLaser())
					HoldKey('E', 0x01);
				//ReleaseKey('G', 0x00);
			}


		}

		// Handle release of laser
		if (!m_pCurrentInput->IsLaser() && prev != nullptr)
		{

			if (prev->IsLaser())
				ReleaseKey('E', 0x01);
		}


		
		// Handle holding \ pressing of grapple
		if (m_pCurrentInput->IsGrapple())
		{

			if (m_pCurrentInput->m_Done == 0 || (this->m_InputIndex == 0 && m_pCurrentInput->m_Done == 1))
			{
				if (prev != nullptr)
				{
					// Previous wasn't jump, we can press it again.
					if (!prev->IsGrapple())
						PressKey('G', 0x01);
				}
				else
				{
					PressKey('G', 0x01);
				}

			}

			if (m_pCurrentInput->m_Done < this->m_pCurrentInput->m_Frames || (this->m_InputIndex == 0 && (m_pCurrentInput->m_Done <= this->m_pCurrentInput->m_Frames)))
			{
				HoldKey('G', 0x01);
				//ReleaseKey('G', 0x00);
			}
			else if (next != nullptr)
			{
				if (next->IsGrapple())
					HoldKey('G', 0x01);
					//ReleaseKey('G', 0x00);
			}


		}

		// Handle release of grapple
		if (!m_pCurrentInput->IsGrapple() && prev != nullptr)
		{

			if (prev->IsGrapple())
				ReleaseKey('G', 0x01);
		}


		// Handle holding \ pressing of weave
		if (m_pCurrentInput->IsWeave())
		{

			if (m_pCurrentInput->m_Done == 0 || (this->m_InputIndex == 0 && m_pCurrentInput->m_Done == 1))
			{
				if (prev != nullptr)
				{
					// Previous wasn't jump, we can press it again.
					if (!prev->IsWeave())
						PressKey('B', 0x01);
				}
				else
				{
					PressKey('B', 0x01);
				}

			}

			if (m_pCurrentInput->m_Done < this->m_pCurrentInput->m_Frames || (this->m_InputIndex == 0 && (m_pCurrentInput->m_Done <= this->m_pCurrentInput->m_Frames)))
			{
				HoldKey('B', 0x01);
				//ReleaseKey('B', 0x00);
			}
			else if (next != nullptr)
			{
				if (next->IsWeave())
					HoldKey('B', 0x01);
					//ReleaseKey('B', 0x00);
			}

		}

		// Handle release of weave
		if (!m_pCurrentInput->IsWeave() && prev != nullptr)
		{

			if (prev->IsWeave())
				ReleaseKey('B', 0x01);
		}

		// Handle holding \ pressing of jump
		if (m_pCurrentInput->IsJump())
		{

			if (m_pCurrentInput->m_Done == 0 || (this->m_InputIndex == 0 && m_pCurrentInput->m_Done == 1))
			{
				if (prev != nullptr)
				{
					// Previous wasn't jump, we can press it again.
					if (!prev->IsJump())
						PressKey(0x20, 0x01);
				}
				else
				{
					PressKey(0x20, 0x01);
				}

			}

			if (m_pCurrentInput->m_Done < this->m_pCurrentInput->m_Frames || (this->m_InputIndex == 0 && (m_pCurrentInput->m_Done <= this->m_pCurrentInput->m_Frames)))
			{
				HoldKey(0x20, 0x01);
				//ReleaseKey(0x20, 0x00);
			}
			else if (next != nullptr)
			{
				if (next->IsJump())
					HoldKey(0x20, 0x01);
					//ReleaseKey(0x20, 0x00);
			}

		}

		// Handle release of jump
		if (!m_pCurrentInput->IsJump() && prev != nullptr)
		{

			if (prev->IsJump())
				ReleaseKey(0x20, 0x01);
		}

		this->FormatManagerString();

	}
	return;
}

void PlaybackManager::FormatManagerString()
{
	memset(&this->m_szCurrentManagerState[0], 0, 2000);


	std::string playerInfo = "";

	playerInfo += "\nRoom ID: " + std::to_string(*(unsigned long*)(WEBBED_CURRENTROOMID_ADDR));
	playerInfo += "\nCursor Locked: ";
	if (g_bCursorLocked) playerInfo += "true";
	else playerInfo += "false";

	//VarName with id 000189C0 and index 65 is lock_to_room
	//VarName with id 000189C1 and index 115 is locked
	if (g_pCameraObject) {
		//playerInfo += "\nCamLocked: " + std::to_string(*g_pCameraObject->get_dbl_ptr(0x189C1));
		//playerInfo += "\nCamLockToRoom: " + std::to_string(*g_pCameraObject->get_dbl_ptr(0x189C0));
	}

	// make sure we're not in main menu
	if (g_pPlayerObject && GetCurrentRoomID() != 13)
	{
		if (!this->plugForJournal) {
			playerInfo += "\nPos: " + std::to_string(g_pPlayerObject->m_fX) + ", " + std::to_string(g_pPlayerObject->m_fY);

			//double* hspeed = g_pPlayerObject->getXSpeedPtr();
			//double* vspeed = g_pPlayerObject->getYSpeedPtr();

			//VarName with id 00018AA7 and index 121 is phy_buffer_speed_x
			//VarName with id 00018AA8 and index 121 is phy_buffer_speed_y

			//double* phy_buffer_speed_x = g_pPlayerObject->get_dbl_ptr(0x18AA7);
			//double* phy_buffer_speed_y = g_pPlayerObject->get_dbl_ptr(0x18AA8);


			float* speeds = g_pPlayerObject->m_pPhysProp->physInst->getPhysicsSpeedPtr();
			float speed_x = g_pPlayerObject->m_pPhysProp->physInst->getPhysicsStepSpeedX(GetPhysicsSystem()->m_pPhysRoom->m_fStepRate);
			float speed_y = g_pPlayerObject->m_pPhysProp->physInst->getPhysicsStepSpeedY(GetPhysicsSystem()->m_pPhysRoom->m_fStepRate);

			//	playerInfo += "\nSpeed: " + std::to_string(speeds[0]) + ", " + std::to_string(speeds[1]);
			playerInfo += "\nSpeed: " + std::to_string(speed_x) + ", " + std::to_string(speed_y);
			//playerInfo += "\nSpeed: " + std::to_string(*hspeed) + ", " + std::to_string(*vspeed);


			PhysicsSystem* pPhys = GetPhysicsSystem();
			if (pPhys) {
				if (pPhys->m_pPhysRoom) {
					if (g_pTopBranch != nullptr) {
						/*
						float phys_x = g_pTopBranch->m_pPhysProp->physInst->m_fPhysX / pPhys->m_pPhysRoom->m_fStepRate;
						float phys_y = g_pTopBranch->m_pPhysProp->physInst->m_fPhysY / pPhys->m_pPhysRoom->m_fStepRate;
						playerInfo += "\nTopBranch: " + std::to_string(phys_x) + ", " + std::to_string(phys_y);*/
					}

					if (g_pBottomBranch != nullptr) {
						/*
						float phys_x = g_pBottomBranch->m_pPhysProp->physInst->m_fPhysX / pPhys->m_pPhysRoom->m_fStepRate;
						float phys_y = g_pBottomBranch->m_pPhysProp->physInst->m_fPhysY / pPhys->m_pPhysRoom->m_fStepRate;
						playerInfo += "\nBottomBranch: " + std::to_string(phys_x) + ", " + std::to_string(phys_y);*/
					}
				}
			}

		}


	}


	if (this->IsPlayingBack())
	{
		sprintf(this->m_szCurrentManagerState, "[%s]-Ln: %u (%u / %u) - [%s]\n(Cur: %u / Total: %u)%s\nInputIndex:%u",
			this->m_pCurrentInput->m_szFromFile,
			(m_pCurrentInput->m_bMultiLevelFile) ? this->m_pCurrentInput->m_nInternalLineNo : this->m_pCurrentInput->m_LineNo,
			this->m_pCurrentInput->m_Done, this->m_pCurrentInput->m_Frames,
			this->m_pCurrentInput->ToString().c_str(), this->m_CurrentFrame, this->m_nTotalFrameCount, playerInfo.c_str(),
			this->m_InputIndex);
	}
	else
	{
		sprintf(this->m_szCurrentManagerState, "%s", playerInfo.c_str());
	}


}
