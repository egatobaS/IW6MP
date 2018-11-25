#pragma once
#define GetPointer(X) *(PDWORD)(X)
#define CSleep(X) Sleep(X * 1000)
#define ANGLE2SHORT(x)  ((int)((x)*65536/360) & 65535)
#define SHORT2ANGLE(x)  ((x)*(360.0/65536))
#define M_PI 3.141592653589793f
#define RadToDeg(X) ((float)(X * (180 / M_PI)))
#define DegToRad(X) ((float)(X * (M_PI / 180)))
extern bool KillThread;
extern const char* CheatName;
extern const char* CheatVersion;
extern HANDLE MODULE_DLL;
extern HANDLE MODULE_XAM;
extern HANDLE MODULE_KRNL;

#define DevBuild
#include "kernel.h"

struct RGBA
{
	float R, G, B, A;
	float Old_Colours[4];
	bool Rainbow, RainbowSaved;

	void ToggleRainbow()
	{
		if (Rainbow)
		{
			Old_Colours[0] = this->R;
			Old_Colours[1] = this->G;
			Old_Colours[2] = this->B;
			Old_Colours[3] = this->A;
			RainbowSaved = true;
		}
		else
		{
			if (RainbowSaved)
			{
				this->R = Old_Colours[0];
				this->G = Old_Colours[1];
				this->B = Old_Colours[2];
				this->A = Old_Colours[3];
			}
		}
	}

	operator float *() { return &R; }
};

struct Vector2
{
	float x, y;
};

typedef float vec_t;
struct Vector3
{
	// Construction/destruction
	float x, y, z;
	inline Vector3(void) { }
	inline Vector3(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	inline Vector3(double X, double Y, double Z) { x = (float)X; y = (float)Y; z = (float)Z; }
	inline Vector3(int X, int Y, int Z) { x = (float)X; y = (float)Y; z = (float)Z; }
	inline Vector3(float X, float Y, int Z) { x = (float)X; y = (float)Y; z = (int)Z; }
	inline Vector3(const Vector3& v) { x = v.x; y = v.y; z = v.z; }
	inline Vector3(float rgfl[3]) { x = rgfl[0]; y = rgfl[1]; z = rgfl[2]; }

	// Operators
	inline Vector3 operator-(void) const { return Vector3(-x, -y, -z); }
	inline int operator==(const Vector3& v) const { return x == v.x && y == v.y && z == v.z; }
	inline int operator!=(const Vector3& v) const { return !(*this == v); }
	inline Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	inline Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	inline Vector3 operator*(float fl) const { return Vector3(x*fl, y*fl, z*fl); }
	inline Vector3 operator/(float fl) const { return Vector3(x / fl, y / fl, z / fl); }
	inline Vector3 operator*(Vector3 fl) const { return Vector3(x*fl.x, y*fl.y, z*fl.z); }
	void operator += (const Vector3& add) { x += add.x; y += add.y; z += add.z; }
	void operator -= (const Vector3& sub) { x -= sub.x; y -= sub.y; z -= sub.z; }
	void operator *= (const float mul) { x *= mul; y *= mul; z *= mul; }
	void operator /= (const float div) { x /= div; y /= div; z /= div; }
	void operator += (const float add) { x += add; y += add; z += add; }
	void operator = (const float eql) { x = eql; y = eql; z = eql; }

	// Methods
	inline void CopyToArray(float* rgfl) const { rgfl[0] = x, rgfl[1] = y, rgfl[2] = z; }
	inline float Length(void) const { return (float)sqrtf(x*x + y*y + z*z); }
	inline float LengthXY(void) const { return (float)sqrtf(x*x + y*y); }
	operator float *() { return &x; } // Vector3s will now automatically convert to float * when needed
	operator const float *() const { return &x; } // Vector3s will now automatically convert to const float * when needed
	inline Vector3 Normalize(void) const
	{
		float flLen = Length();
		if (flLen == 0) return Vector3(0, 0, 1); // ????
		flLen = 1 / flLen;
		return Vector3(x * flLen, y * flLen, z * flLen);
	}

	inline float Length2D(void) const { return (float)sqrtf(x*x + y*y); }

	inline bool NormalizeAngle()
	{
		if (x != x || y != y || z != z) return false;

		if (x > 180) x -= 360.f;
		if (x < -180) x += 360.f;
		if (y > 180) y -= 360.f;
		if (y < -180) y += 360.f;

		return x >= -180.f && x <= 180.f && y >= -180.f && y <= 180.f;
	}

	inline bool ClampAngle()
	{
		if (x > 85.f) x = 85.f;
		if (x < -85.f) x = -85.f;

		z = 0.f;

		return x >= -85.f && x <= 85.f && y >= -180.f && y <= 180.f;
	}
};

struct Vector4
{
	float a, b, c, d;
};

extern unsigned char PrivateKey[464];

void CreateNewFile(const char* File);
#ifdef DEVKIT
HRESULT CreateSymbolicLink(PCHAR szDrive, PCHAR szDeviceName, BOOL System);
#endif
int filter(const char* Name, unsigned int code, struct _EXCEPTION_POINTERS *ep);
float AngleNormalize360(float angle);
Vector3 VectorNormalize(Vector3 vec);
PWCHAR toWCHAR(PCHAR Text, ...);
bool YesNoBox(LPCWSTR Title, LPCWSTR Body);
void PrintBox(int base, const char* Input);
float GetDistance(Vector3 c1, Vector3 c2);
BOOL IsDevkit();
void ReverseBytes(char *start, int size);
void CharArrayToString(char* buffer, unsigned char data[], int size, bool reverse = false);
float DotProduct(Vector3 Vec1, Vector3 Vec2);
bool in_circle(int center_x, int center_y, int radius, int x, int y);
void AngleVectors(Vector3 &angles, Vector3 *forward, Vector3 *right, Vector3 *up);
Vector3 AnglesToForward(Vector3 startOrigin, Vector3 Angles, float scale);
Vector3 vectorToAngles(Vector3 &Delta);
bool cuser_strcmp(const char* s1, const char* s2);
const char* ReturnXShowKeyboardUI(LPCWSTR wseDefaultText, LPCWSTR wszTitleText, LPCWSTR wszDescriptionText, DWORD Length);
const char* ReturnXShowKeyboardUIPassword(LPCWSTR wseDefaultText, LPCWSTR wszTitleText, LPCWSTR wszDescriptionText, DWORD Length, int& err);
const char* ReturnXShowKeyboardUI(LPCWSTR wseDefaultText, LPCWSTR wszTitleText, LPCWSTR wszDescriptionText, DWORD Length, int& err);
const char* ReturnXShowKeyboardUI(LPCWSTR wseDefaultText, LPCWSTR wszTitleText, LPCWSTR wszDescriptionText, DWORD Length, int& err, DWORD Flags);
char* va(const char* fmt, ...); 
DWORD ResolveFunction(PCHAR ModuleName, DWORD Ordinal);
//VOID XNotify(CONST PWCHAR NotifyText);
unsigned int ReverseInt(unsigned int val);
DWORD XNotifyThread(wchar_t *Message);
DWORD XamGetCurrentTitleID();
void PlayXMA2Sound(const char* Sound);
BOOL FileExists(LPCSTR lpFileName);

void DecryptData(unsigned char* in, int inLength, unsigned char* out);
