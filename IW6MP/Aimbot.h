#pragma once

float Get3dDistance(Vector3 &c1, Vector3 &c2);
int GetTargetClosest(bool Priority = false);
int GetVisibleAndKillableTarget(bool Priority, bool AutoWall);
int GetMostViableClient();
void SetViewAngles();
void DoSetViewAngles();
void DoSilentAimbot(usercmd_s* cur, usercmd_s* old, usercmd_s* New);
void SetAimbotTarget();