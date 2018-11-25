#include "stdafx.h"
#include "XIniRW.h"
#include <algorithm>

CXIniRW XIniRW;

#pragma region File Handling

fstream _RFile;
ofstream _WFile;

bool CXIniRW::ROpenINI(const char *fileName)
{
	if (_RFile.is_open()) {
		return false;
	}

	_RFile = fstream(fileName, std::ios::in);

	if (!_RFile.is_open()) {
		return false;
	}

	return true;
}

bool CXIniRW::RCloseINI()
{
	if (!_RFile.is_open()) {
		return false;
	}

	_RFile.close();
}


bool CXIniRW::WOpenINI(const char *fileName)
{
	if (_WFile.is_open()) {
#if defined(DEVKIT)
		printf("Failed to open File (%s) - File already open\n", fileName);
#endif
		return false;
	}

	_WFile = ofstream(fileName, ofstream::out | ofstream::trunc);

	if (!_WFile.is_open()) {
#if defined(DEVKIT)
		printf("Failed to open File (%s)\n", fileName);
#endif
		return false;
	}

	return true;
}

bool CXIniRW::WCloseINI()
{
	if (!_WFile.is_open()) {
		return false;
	}

	_WFile.close();
}

#pragma endregion

#pragma region Reading
int CXIniRW::GetValue(const char *Menu, const char *Member, string *szReturn) {
	char CurrentMenu[0x100];
	int line = 0;
	string szLine;
	while (getline(_RFile, szLine))
	{
		line++;
		//printf("[%s][%s] %i %s\n", Menu, Member, line, szLine.c_str());
		if ((strstr((szLine.substr(0, 1)).c_str(), ";")) || szLine.empty()) {
			//printf("Comment \"%s\" found at line %i\n", szLine.c_str(), line);
			continue;
		}

		if (strstr((szLine.substr(0, 1)).c_str(), "["))
		{
			//printf("Menu \"%s\" found at line %i\n", szLine.c_str(), line);
			szLine.erase(0, 1);
			szLine.erase(szLine.length() - 1, szLine.length());
			user_strcpy(CurrentMenu, szLine.c_str());
			continue;
		}

		//printf("CurrentMenu = %s\n", CurrentMenu);
		if (strstr(CurrentMenu, Menu))
		{
			//printf("Menu Match (%s == %s)\n", CurrentMenu, Menu);

			if (!strstr(szLine.c_str(), Member))
				continue;

			*szReturn = szLine.substr(szLine.find("=") + 2);
			//printf("Value \"%s\" found at line %i\n", szReturn, line);
			return true;
		}
	}
	return false;
}

/*if ((szLine.substr(0, 1)) == "[")
{
szLine.erase(0, 1);
szLine.erase(szLine.length() - 1, szLine.length());
CurentSub = szLine.c_str();
}

if (CurentSub == Key)
{*/
/*string::size_type dwValueOffset = szLine.find_first_of('=');
if (!strstr(szLine.c_str(), Member))
return false;

szReturn = szLine.substr(dwValueOffset + 2, ((szLine.length() - (dwValueOffset + 2)) - 1));
/*}
else
return false;*/

//printf("Sub = %s - Option = %s\nValue = %s\n", Key, Member, szReturn);

bool CXIniRW::ReadBool(const char *Menu, const char *Member)
{
	if (!_RFile.is_open()) {
		return 0;
	}

	_RFile.clear();
	_RFile.seekg(0, ios::beg);

	if (_RFile.good()) {
		string szValue;
		if (!GetValue(Menu, Member, &szValue))
			return false;

		//printf("[%s][%s] %s\n", Menu, Member, szValue.c_str());

		if (strstr(szValue.c_str(), "true"))
			return true;
		else
			return false;
	}

	return false;
}

int CXIniRW::ReadInt(const char *Menu, const char *Member)
{
	if (!_RFile.is_open()) {
		return 0;
	}

	_RFile.clear();
	_RFile.seekg(0, ios::beg);

	while (_RFile.good()) {
		string szValue;
		if (!GetValue(Menu, Member, &szValue))
			return 0;

		return atoi(szValue.c_str());
	}
	return 0;
}

float CXIniRW::ReadFloat(const char *Menu, const char *Member)
{
	if (!_RFile.is_open()) {
		return 0;
	}

	_RFile.clear();
	_RFile.seekg(0, ios::beg);

	while (_RFile.good()) {
		string szValue;
		if (!GetValue(Menu, Member, &szValue))
			return 0.0f;

		return atof(szValue.c_str());
	}
	return 0.0f;
}

void removeCharsFromString(string &str, char* charsToRemove) {
	for (unsigned int i = 0; i < strlen(charsToRemove); ++i) {
		str.erase(remove(str.begin(), str.end(), charsToRemove[i]), str.end());
	}
}

char OutputBuffer[0x200];
char* CXIniRW::ReadString(const char *Menu, const char *Member)
{
	if (!_RFile.is_open()) {
		return 0;
	}

	_RFile.clear();
	_RFile.seekg(0, ios::beg);

	if (_RFile.good()) {
		string szValue;
		if (!GetValue(Menu, Member, &szValue))
			return false;

		//removeCharsFromString(szValue, " ");
		removeCharsFromString(szValue, ";");
		user_strcpy(OutputBuffer, szValue.c_str());
		return OutputBuffer;
	}

	user_strcpy(OutputBuffer, "Error");
	return OutputBuffer;
}
#pragma endregion

#pragma region Writing
void CXIniRW::CreateGroup(const char *Group)
{
	if (!_WFile.is_open()) {
		return;
	}

	char szWriteBuffer[256];
	_snprintf(szWriteBuffer, 256, "[%s]\n", Group);
	_WFile.write(szWriteBuffer, strlen(szWriteBuffer));
	//_WFile << szWriteBuffer;
}

void CXIniRW::AddComment(const char *comment)
{
	if (!_WFile.is_open()) {
		return;
	}

	char szWriteBuffer[256];
	_snprintf(szWriteBuffer, 256, "%s\n", comment);

	_WFile.write(szWriteBuffer, strlen(szWriteBuffer));
	//_WFile << szWriteBuffer;
}

void CXIniRW::WriteInt(const char *Member, int Value)
{
	if (!_WFile.is_open()) {
		return;
	}

	char szWriteBuffer[256];
	_snprintf(szWriteBuffer, 256, "%s = %i;\n", Member, Value);

	_WFile.write(szWriteBuffer, strlen(szWriteBuffer));
	//_WFile << szWriteBuffer;
}

void CXIniRW::WriteFloat(const char *Member, float Value)
{
	if (!_WFile.is_open()) {
		return;
	}

	char szWriteBuffer[256];
	_snprintf(szWriteBuffer, 256, "%s = %f;\n", Member, Value);

	_WFile.write(szWriteBuffer, strlen(szWriteBuffer));
	//_WFile << szWriteBuffer;
}

void CXIniRW::WriteString(const char *Member, const char* Value)
{
	if (!_WFile.is_open()) {
		return;
	}

	char szWriteBuffer[300];
	_snprintf(szWriteBuffer, 300, "%s = %s;\n", Member, Value);

	_WFile.write(szWriteBuffer, strlen(szWriteBuffer));
	//_WFile << szWriteBuffer;
}

void CXIniRW::WriteBool(const char *Member, bool Value)
{
	if (!_WFile.is_open()) {
		return;
	}

	char szWriteBuffer[300];
	_snprintf(szWriteBuffer, 300, "%s = %s;\n", Member, Value ? "true" : "false");

	_WFile.write(szWriteBuffer, strlen(szWriteBuffer));
	//_WFile << szWriteBuffer;
}
#pragma endregion