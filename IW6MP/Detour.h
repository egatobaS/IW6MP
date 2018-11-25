#pragma once
class Detour
{
public:
	bool Hooked;
	void RestoreFunction();
	void* HookFunction(unsigned int FuncAddress, unsigned int OurDestination);
private:
	int* OrStub;
	int IndexDetour;
	int Address;
	static void GLPR(void);
	static int DetourCount;
	unsigned char OriginalBytes[16];
	static unsigned char DetourSection[0xF000];
	static unsigned int RelinkGPLR(unsigned int Offset, unsigned int SaveStubAddress, unsigned int OriginalAddress);
	void CopyOriginalInstructions(unsigned int Address, unsigned int SaveStub, DWORD* StubInstructions);
	void DetourFunction(unsigned int Address, void* Destination, void* Stub);
	void PatchInJump(unsigned int Address, void* Destination, bool Linked);

};


