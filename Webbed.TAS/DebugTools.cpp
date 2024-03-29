#include "pch.h"
#include "DebugTools.h"
#include <stdio.h>
#include <stdlib.h>

void DebugOutput(const char* fmt, ...)
{
	char outputBuffer[2048] = { 0 };
	va_list vaList;

	va_start(vaList, fmt);
	vsnprintf(outputBuffer, 2048, fmt, vaList);
	va_end(vaList);

	OutputDebugStringA(outputBuffer);
}

void DebugOutputW(const wchar_t* fmt, ...)
{
	wchar_t outputBuffer[2048] = { 0 };

	va_list args;
	va_start(args, fmt);
	vswprintf_s(outputBuffer, 2048, fmt, args);
	va_end(args);

	OutputDebugStringW(outputBuffer);
}