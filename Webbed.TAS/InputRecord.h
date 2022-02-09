#pragma once

#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <limits>
#include <cmath>
#include "Globals.h"

enum EInputState {
	DEFAULT_NONE = 0x0,
	LEFT = 1 << 0,
	RIGHT = 1 << 1,
	UP = 1 << 2,
	DOWN = 1 << 3,
	LEFTP = 1 << 4,
	RIGHTP = 1 << 5,
	UPP = 1 << 6,
	DOWNP = 1 << 7,
	JUMP = 1 << 8,
	GRAPPLE = 1 << 9,
	WEAVE = 1 << 10,
	ESCAPE = 1 << 11,
	JOURNAL = 1 << 12,
	LASER = 1 << 13,
	DANCE = 1 << 14,
	INTERACT = 1 << 15,
	ANGLE = 1 << 16,
	MOUSE = 1 << 17,
	SLOWDOWN = 1 << 18,
	JUMPP = 1 << 19
};

struct InputRecord
{
	EInputState m_InputState;

	int m_Frames;

	int m_TotalFrames;

	int m_Done;

	unsigned int m_LineNo;

	unsigned int m_nInternalLineNo;

	unsigned long m_nSeed;

	bool m_bMultiLevelFile;

	char m_szFromFile[56];

	unsigned long roomGoto;

	bool hasAngle;

	double angle;

	double maxWalkSpeed;
	bool hasMaxWalkSpeed;

	double pullPower;
	bool hasPullPower;

	bool restartRoom;

	bool hasPos;

	bool hasSpeedX;

	bool hasSpeedY;

	//bool camToPlayer;

	double posX;

	double posY;

	double speedX;

	double speedY;

	bool hasMouse;

	double mouseX;

	double mouseY;

	bool handledByCreate;
	bool hasPlugJournal;
	bool hasUnplugJournal;

	std::string ToString()
	{
		std::string result = "";

		if (this->IsLeft())
			result += ",Left";

		if (this->IsLeftPress())
			result += ",LeftP";

		if (this->IsRight())
			result += ",Right";

		if (this->IsRightPress())
			result += ",RightP";

		if (this->IsUp())
			result += ",Up";

		if (this->IsUpPress())
			result += ",UpP";

		if (this->IsDown())
			result += ",Down";

		if (this->IsDownPress())
			result += ",DownP";

		if (this->IsJump())
			result += ",Jump";

		if (this->IsGrapple())
			result += ",Grapple";

		if (this->IsWeave())
			result += ",Weave";

		if (this->IsJournal())
			result += ",Journal";

		if (this->IsLaser())
			result += ",Laser";

		if (this->IsInteract())
			result += ",Interact";

		if (this->IsMouse())
			result += ",Mouse : " + std::to_string(this->mouseX) + ", " + std::to_string(this->mouseY);

		if (this->IsEscape())
			result += ",Esc";

		if (this->hasAngle)
			result += ",Angle : " + std::to_string(this->angle);

		return result;
	}


	bool HasFlag(EInputState state, EInputState which)
	{
		return (state & which) == which;
	}

	bool IsLeft()
	{
		return this->HasFlag(this->m_InputState, EInputState::LEFT);
	}

	bool IsRight()
	{
		return this->HasFlag(this->m_InputState, EInputState::RIGHT);
	}

	bool IsUp()
	{
		return this->HasFlag(this->m_InputState, EInputState::UP);
	}

	bool IsDown()
	{
		return this->HasFlag(this->m_InputState, EInputState::DOWN);
	}

	bool IsLeftPress()
	{
		return this->HasFlag(this->m_InputState, EInputState::LEFTP);
	}

	bool IsRightPress()
	{
		return this->HasFlag(this->m_InputState, EInputState::RIGHTP);
	}

	bool IsUpPress()
	{
		return this->HasFlag(this->m_InputState, EInputState::UPP);
	}

	bool IsDownPress()
	{
		return this->HasFlag(this->m_InputState, EInputState::DOWNP);
	}

	bool IsJump()
	{
		return this->HasFlag(this->m_InputState, EInputState::JUMP);
	}

	bool IsJumpPress() {
		return this->HasFlag(this->m_InputState, EInputState::JUMPP);
	}

	bool IsGrapple() {
		return this->HasFlag(this->m_InputState, EInputState::GRAPPLE);
	}

	bool IsWeave() {
		return this->HasFlag(this->m_InputState, EInputState::WEAVE);
	}

	bool IsDance() {
		return this->HasFlag(this->m_InputState, EInputState::DANCE);
	}

	bool IsJournal() {
		return this->HasFlag(this->m_InputState, EInputState::JOURNAL);
	}

	bool IsLaser() {
		return this->HasFlag(this->m_InputState, EInputState::LASER);
	}

	bool IsSlow()
	{
		return this->HasFlag(this->m_InputState, EInputState::SLOWDOWN);
	}

	bool IsEscape()
	{
		return this->HasFlag(this->m_InputState, EInputState::ESCAPE);
	}

	bool IsMouse() {
		return this->HasFlag(this->m_InputState, EInputState::MOUSE);
	}

	bool IsRoomGoto()
	{
		bool ret = (this->roomGoto == -1) ? false : true;
		return ret;
	}

	bool IsRoomRestart()
	{
		return this->restartRoom;
	}

	bool IsInteract() {
		return this->HasFlag(this->m_InputState, EInputState::INTERACT);
	}

	InputRecord(unsigned long frames, EInputState state)
	{
		this->pullPower = 0.0;
		this->hasPullPower = false;
		this->maxWalkSpeed = 0.0;;
		this->hasMaxWalkSpeed = false;
		this->hasPlugJournal = false;
		this->hasUnplugJournal = false;
		this->handledByCreate = false;
		this->hasPos = false;
		this->hasSpeedX = false;
		this->hasSpeedY = false;
		this->roomGoto = -1;
		this->m_Frames = frames;
		this->m_InputState = state;
		this->m_nSeed = -1;
		this->hasAngle = false;
		this->angle = 0.0;
		this->restartRoom = false;
		this->roomGoto = -1;
		this->hasMouse = false;
		this->mouseX = 0.0;
		this->mouseY = 0.0;
		this->speedX = 0.0;
		this->speedY = 0.0;
		this->posX = 0.0;
		this->posY = 0.0;

	}

#pragma warning(disable : 4996)
	InputRecord(std::string line, unsigned int ln, const char* filename, unsigned int otherln)
	{
		this->pullPower = 0.0;
		this->hasPullPower = false;
		this->maxWalkSpeed = 0.0;;
		this->hasMaxWalkSpeed = false;
		this->hasPlugJournal = false;
		this->hasUnplugJournal = false;
		this->handledByCreate = false;
		this->hasPos = false;
		this->hasSpeedX = false;
		this->hasSpeedY = false;
		this->roomGoto = -1;
		this->m_nSeed = -1;
		this->hasAngle = false;
		this->angle = 0.0;
		this->restartRoom = false;
		this->roomGoto = -1;
		this->hasMouse = false;
		this->mouseX = 0.0;
		this->mouseY = 0.0;
		this->speedX = 0.0;
		this->speedY = 0.0;
		this->posX = 0.0;
		this->posY = 0.0;
		this->m_nInternalLineNo = 0;
		this->m_LineNo = 0;
		this->m_Done = 0;
		this->m_nSeed = -1;


		strncpy(this->m_szFromFile, filename, sizeof(this->m_szFromFile) / sizeof(this->m_szFromFile[0]));
		this->m_bMultiLevelFile = false;

		if (strcmpi(filename, "Webbed.rec"))
		{
			this->m_nInternalLineNo = 0;
			this->m_bMultiLevelFile = true;
		}

		this->m_nInternalLineNo = otherln;
		std::istringstream ss(line);
		std::string token;

		std::vector<std::string> tokens;
		auto delimited = line.find(',');

		while (std::getline(ss, token, ','))
			tokens.push_back(token);

		this->m_Frames = (delimited == std::string::npos) ? std::stoul(line) : std::stoul(tokens[0]);
		this->m_TotalFrames = this->m_Frames;

		unsigned int TempState = EInputState::DEFAULT_NONE;

		if (tokens.size() > 1 && delimited != std::string::npos)
		{
			for (unsigned int i = 1; i < tokens.size(); i++)
			{
				token = tokens[i];

				// Just continue again, we already handled cases where we needed to process integers.
				if (isdigit(tokens[i][0]))
					continue;

				auto negativelamb = [](char& ch) { ch = toupper((unsigned char)ch); };
				std::for_each(token.begin(), token.end(), negativelamb);


				if (token == "LEFT")
				{
					TempState |= EInputState::LEFT;
					continue;
				}
				else if (token == "RIGHT")
				{
					TempState |= EInputState::RIGHT;
					continue;
				}
				else if (token == "UP")
				{
					TempState |= EInputState::UP;
					continue;
				}
				else if (token == "DOWN")
				{
					TempState |= EInputState::DOWN;
					continue;
				}
				else if (token == "LEFTP")
				{
					TempState |= EInputState::LEFTP;
					continue;
				}
				else if (token == "RIGHTP")
				{
					TempState |= EInputState::RIGHTP;
					continue;
				}
				else if (token == "UPP")
				{
					TempState |= EInputState::UPP;
					continue;
				}
				else if (token == "DOWNP")
				{
					TempState |= EInputState::DOWNP;
					continue;
				}
				else if (token == "JUMP")
				{
					TempState |= EInputState::JUMP;
					continue;
				}
				else if (token == "JUMPP") {
					TempState |= EInputState::JUMPP;
					continue;
				}
				else if (token == "SLOW")
				{
					TempState |= EInputState::SLOWDOWN;
					continue;
				}
				else if (token == "ESC")
				{
					TempState |= EInputState::ESCAPE;
					continue;
				}
				else if (token == "ANGLE")
				{
					this->angle = std::strtod(tokens[i + 1].c_str(), nullptr);
					this->hasAngle = true;
					continue;
				}
				else if (token == "INTERACT") {
					TempState |= EInputState::INTERACT;
					continue;
				}
				else if (token == "MAXWALK") {
					this->hasMaxWalkSpeed = true;
					this->maxWalkSpeed = std::strtod(tokens[i + 1].c_str(), nullptr);
					continue;
				}
				else if (token == "PULLPOWER") {
					this->hasPullPower = true;
					this->pullPower = std::strtod(tokens[i + 1].c_str(), nullptr);
					continue;
				}
				else if (token == "DANCE") {
					TempState |= EInputState::DANCE;
					continue;
				}
				else if (token == "JOURNAL") {
					TempState |= EInputState::JOURNAL;
					continue;
				}
				else if (token == "PLUGJOURNAL") {
					this->hasPlugJournal = true;
					continue;
				}
				else if (token == "UNPLUGJOURNAL") {
					this->hasUnplugJournal = true;
					continue;
				}
				else if (token == "SPEEDX")
				{
					this->hasSpeedX = true;
					this->speedX = std::strtod(tokens[i + 1].c_str(), nullptr);
					continue;
				}
				else if (token == "SPEEDY")
				{
					this->hasSpeedY = true;
					this->speedY = std::strtod(tokens[i + 1].c_str(), nullptr);
					continue;
				}
				else if (token == "POS")
				{
					this->hasPos = true;
					this->posX = std::strtof(tokens[i + 1].c_str(), nullptr);
					this->posY = std::strtof(tokens[i + 2].c_str(), nullptr);
					continue;
				}
				else if (token == "BLOGPOS") {
					this->hasPos = true;
					this->posX = std::strtof(tokens[i + 1].c_str(), nullptr);
					this->posY = std::strtof(tokens[i + 2].c_str(), nullptr);
				
					continue;
				}
				else if (token == "RESTART")
				{
					this->restartRoom = true;
					continue;
				}
				else if (token == "GOTO")
				{
					this->roomGoto = std::stoul(tokens[i + 1].c_str(), nullptr);
					continue;
				}
				else if (token == "GRAPPLE") {
					TempState |= EInputState::GRAPPLE;
					continue;
				}
				else if (token == "WEAVE") {
					TempState |= EInputState::WEAVE;
					continue;
				}
				else if (token == "MOUSE") {
					this->hasMouse = true;
					this->mouseX = std::strtof(tokens[i + 1].c_str(), nullptr);
					this->mouseY = std::strtof(tokens[i + 2].c_str(), nullptr);
					TempState |= EInputState::MOUSE;
					continue;
				}
				else if(token == "LASER" || token == "PEW") {
				    TempState |= EInputState::LASER;
					continue;
                }
			}
		}

		this->m_InputState = (EInputState)(TempState);

	}

	InputRecord() {}
};
