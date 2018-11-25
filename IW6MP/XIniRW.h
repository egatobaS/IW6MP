/*

INI Reader/Writer for XBOX 360
Created by ExiLe's House

Notes:
Requires in stdafx.h or main.h:
#include <string>
#include <fstream>
using namespace std;

*/

class CXIniRW {
public:
	bool ROpenINI(const char *fileName);
	bool RCloseINI();
	bool WOpenINI(const char *fileName);
	bool WCloseINI();

	int GetValue(const char *Key, const char *Member, string *szLine);

	int ReadInt(const char *Group, const char *Member);
	float ReadFloat(const char *Group, const char *Member);
	char* ReadString(const char *Group, const char *Member);
	bool ReadBool(const char *Group, const char *Member);

	void AddComment(const char *comment);
	void CreateGroup(const char *Group);
	void WriteInt(const char *Member, int Value);
	void WriteFloat(const char *Member, float Value);
	void WriteString(const char *Member, const char* Value);
	void WriteBool(const char *Member, bool Value);

}extern XIniRW;