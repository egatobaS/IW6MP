#pragma once

bool CG_WorldPosToCompass(int compassType, cg_s *cgameGlob, rectDef_s *mapRect, const float *north, const float *playerWorldPos, const float *in, float *out, float *outClipped);
void SetRadarColor(int Client, float &R, float &G, float &B, float &A);
void DrawOnRadar(const char* Material, float w, float h, float R, float G, float B, float A, bool DrawAngles, int EntityIndex);
void DrawOnRadar(const char* Material, float w, float h, float R, float G, float B, float A, float Yaw, Vector3 Location);
void DrawOnRadarYaw(const char* Material, float w, float h, float R, float G, float B, float A, float Yaw, Vector3 Location);
void DrawOnRadar(const char* Text, Vector3 Location);
void DrawMapOnRadar();
void DrawRadar();
bool CL_ClientIsInMyParty(int localclientnum, int Client);
void CalcCompassFriendlySize(int CompassType, float* W, float* H);
void CG_CompassDrawFriendlies();
void CG_CompassDrawEnemies();
void CG_CompassDrawPlayer();
void DrawWeaponsOnRadar(int i);
void DrawExplosivesOnRadar(int i);
void VehicleHud_DrawCompass();
void CG_CompassDrawMissiles();
void CG_CompassDrawPlanes();
void CG_CompassDrawTurrets();
void CG_CompassDrawPlayerPointers_MP();