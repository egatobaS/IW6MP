#pragma once

class MemoryBuffer
{

public:
	MemoryBuffer(DWORD dwSize = 512) {
		m_pBuffer = NULL;
		m_dwDataLength = 0;
		m_dwBufferSize = 0;

		if ((dwSize < UINT_MAX) && (dwSize != 0)) {
			m_pBuffer = (BYTE*)malloc(dwSize + 1);    // one more char, in case when using string funcions
			if (m_pBuffer) {
				m_dwBufferSize = dwSize;
				m_pBuffer[0] = 0;
			}
		}
	};

	~MemoryBuffer() {
		if (m_pBuffer) free(m_pBuffer);

		m_pBuffer = NULL;
		m_dwDataLength = 0;
		m_dwBufferSize = 0;
	};

	// Add chunk of memory to buffer
	BOOL Add(CONST PVOID p, DWORD dwSize) {
		if (CheckSize(dwSize)) {
			memcpy(m_pBuffer + m_dwDataLength, p, dwSize);
			m_dwDataLength += dwSize;
			*(m_pBuffer + m_dwDataLength) = 0;    // fill end zero
			return TRUE;
		}
		else {
			return FALSE;
		}
	};

	// Get the data in buffer
	PBYTE GetData() CONST {
		return m_pBuffer;
	};

	// Get the length of data in buffer
	DWORD GetDataLength() CONST {
		return m_dwDataLength;
	};

	// Rewind the data pointer to the begining
	VOID Rewind() {
		m_dwDataLength = 0; m_pBuffer[0] = 0;
	};

	// Automatically adjust increase buffer size if necessary
	BOOL CheckSize(DWORD dwSize) {
		if (m_dwBufferSize >= (m_dwDataLength + dwSize)) {
			return TRUE;    // Enough space
		}
		else {
			// Try to double it
			DWORD dwNewSize = max(m_dwDataLength + dwSize, m_dwBufferSize * 2);
			BYTE* pNewBuffer = (PUCHAR)realloc(m_pBuffer, dwNewSize + 1);
			if (pNewBuffer) {
				m_pBuffer = pNewBuffer;
				m_dwBufferSize = dwNewSize;
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}

private:
	PBYTE m_pBuffer;
	DWORD m_dwDataLength;
	DWORD m_dwBufferSize;
};

enum PlayerSide_t
{
	fucked,
	LeftSide,
	RightSide,
	CenterSide
};

extern char VisionSet[100];

void CG_GameMessage(int localclient, const char* fmt, ...);
void CG_BoldGameMessage(int localclient, const char* fmt, ...);
void Cbuf_AddText(int localclient, const char* fmt, ...);
void CL_AddReliableCommand(int localclient, const char* fmt, ...);
void ConsoleCMD(char* cmd);
void CG_SetVision(int unk, int unk1, int unk2, char* vision, int duration);
void SetVision(char* Vision);
void ResetSetVision();
void SetName(char* name);
void Setname(char* fmt, ...);
void SetNameColour(char* colour, char* name);
void SetClantag(char* tag);
void DoKeyboardWork(LPVOID r3);
void ThreadKeyBoardAction(int CallBackPtr, LPCWSTR DefaultValue, LPCWSTR Title, LPCWSTR Desc, int MaxVal);
bool IsEnemy(int client);
bool IsFriendly(int client);
bool GetStructs();
const char* UI_GetMapName(const char* map);
const char* UI_GetGameTypeName(const char* gametype);
WeaponDef* BG_GetWeaponDef(int Weapon);
WeaponVariantDef* BG_GetWeaponVariantDef(int Weapon);
const char *GetWeaponName(int Weapon);
Material* GetWeaponShader(int Weapon);
bool GetTagPos(int EntNum, const char* Tag, Vector3* Pos, bool pointscale = false);
void DObjFree(DObj_s *obj);
void FixMovement(usercmd_s* pCmd, float CurAngle, float OldAngle, float fOldForward, float fOldSidemove);
void PosPrediction(centity_s* TargetEntity, Vector3& BonePos, int Scale, bool NotAllowZ);
void BG_EvaluateTrajectory(trajectory_t* tr, int atTime, Vector3* result);
void CG_GetPlayerVelocity(int LocalClientNum, centity_s* cent, Vector3* velocity_out);
Vector3 VelocityPrediction(centity_s* TargetEntity, Vector3 BonePos, int Scale);
bool CG_GetPlayerViewOrigin(int localClientNum, playerState_s *ps, Vector3 *origin);
unsigned short Trace_GetEntityHitId(trace_t* trace);
bool CheckVis(int client, Vector3 Pos);
bool CheckWalls(int client, Vector3 Pos);
bool HasShield(int client);
bool ShieldEquiped(int client);
bool ShieldStowed(int client);
inline void CheckVisAndWalls(int client, const char* Bones[], int number);
inline void CheckVisAndWalls(int client, const char* Bone);
float GetDistance2(Vector3 c1, Vector3 c2);
PlayerSide_t SideOfPlayer(int client);
void RunRiotChecks(int i);
void CacheTagOrigins(int client);
void DoClientChecks();
LONG WINAPI OurCrashHandler(EXCEPTION_POINTERS * ExceptionInfo);
clientMigState_t CL_GetLocalClientMigrationState(int localclient);
DWORD WINAPI ExecuteAutoWall(LPVOID Params);
void InitializeAutoWallThreads();
void BG_GetSpreadForWeapon(playerState_s* PlayerState, int Weapon, float *minSpread, float *maxSpread);
int TransformSeed(int *pHoldrand);
void RandomBulletDir(int* randSeed, float *x, float *y);
void GetWeaponSpread(float* Spread);
void NoSpread(usercmd_s *cmd);
float WallDepth(float *p1, float *p2);
float WallDepthA(Vector3 c1, Vector3 c2);
float GetAimDegree(int t);
float GetAimDegreeA(int t);
bool isAimingAtMe(int *t, float *a);
bool CloseToMe(int *outId);
bool isAnyVis(int mClient, int *outId);
bool GoodEnemy(int client);
bool GoodPlayer(int client);
void CG_PlayRumbleOnClient(char* rumblename);
void UI_PlaySound(const char* snd);
bool BulletTrace(int localClientNum, BulletFireParams* bp, centity_s* cent, BulletTraceResults* br, int lastSurfaceType);
bool BG_AdvanceTrace(BulletFireParams *bp, BulletTraceResults *br, float dist);
float BG_GetSurfacePenetrationDepth(int penetrateType, bool isAlternate, int surfaceFlags);
int BG_GetPerkIndexForName(const char *perkName);
int BG_GetAmmoInClip(playerState_s *ps, int weapon);
bool CL_IsServerRestarting();
bool CG_IsFullyInitialized();
bool BG_HasPerk(const char *perk);
bool CG_IsSelectingLocation(int localClientNum);
bool ShouldHookRun();

bool CReadFile(const char* FileName, MemoryBuffer &pBuffer);