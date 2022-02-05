// Webbed.PEPatch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <ImageHlp.h>
#include <string>

#pragma warning(disable : 4996)
int main(int argc, char **argv)
{

	FILE* fp = fopen("webbed.exe", "rb+");

	if (!fp) return -1;

	rewind(fp);

	fseek(fp, 0x176, SEEK_SET);
	fputc(0x00, fp);
	fclose(fp);
}


