#pragma once

bool WorldToScreen(Vector3& WorldLocation, Vector2& Screen, bool on_screen = false);
void MakeESPLine(Vector3 Center, float X1, float Y1, float Z1, float X2, float Y2, float Z2, float R, float G, float B, float A);
void DrawCroshair();
void DrawHealthBar();
void SetESPColor(int Client, float &R, float &G, float &B, float &A, bool snapLine = false);
void DrawBoneLine(int entity, const char* tag1, const char* tag2, float R, float G, float B, float A);
void DrawBoneLine(centity_s entity, const char* tag1, const char* tag2, float R, float G, float B, float A);
bool GetDimentions(int client, float *Width, float *Height, Vector2* Center, bool on_screen = false);
void DrawSnapLine(int Client);
void DrawESPBones(int Client);
float GetDistance(Vector3 c1, Vector3 c2);
void DrawWeaponsOnScreen(int i);
void DrawExplosivesOnScreen(int i);
void DrawPlayerPointer(int i);
void DrawESP();