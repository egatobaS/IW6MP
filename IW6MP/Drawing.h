#pragma once

void SetShader(Material* materialIndex, float x, float y, float w, float h, float R, float G, float B, float A);
void SetShader(const char* material, float x, float y, float w, float h, float R, float G, float B, float A);
void SetText(const char *text, const char* font, float x, float y, float Scale, float R, float G, float B, float A);
void SetText(const char *text, const char* font, float x, float y, float Scale, float R, float G, float B, float A, float Angle);
void SetTextWithBackGround(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A, float BGR, float BGG, float BGB, float BGA);
void SetTextCenteredWithBackGround(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A, float BGR, float BGG, float BGB, float BGA);
void SetTextWithBackGround(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A, float BGR, float BGG, float BGB, float BGA, float OutlineR, float OutlineG, float OutlineB, float OutlineA);
void DrawCheckBox(const char* material, float x, float y, float w, float h, float R, float G, float B, float A, bool status);
void DrawSwitch(float x, float y, float w, float h, float R, float G, float B, float A, bool status);
void DrawBox(const char* material, float x, float y, float w, float h, float R, float G, float B, float A, float OutlineR, float OutlineG, float OutlineB, float OutlineA);
void DrawBox(const char* material, float x, float y, float w, float h, float OutW, float R, float G, float B, float A, float OutlineR, float OutlineG, float OutlineB, float OutlineA);
void DrawBoxNoTopMenu(const char* material, float x, float y, float w, float h, float OutW, float R, float G, float B, float A, float OutlineR, float OutlineG, float OutlineB, float OutlineA);
void DrawBoxNoTop(const char* material, float x, float y, float w, float h, float OutW, float R, float G, float B, float A, float OutlineR, float OutlineG, float OutlineB, float OutlineA);
void SetTextRightWithBackGround(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A, float BGR, float BGG, float BGB, float BGA, float OutlineR, float OutlineG, float OutlineB, float OutlineA);
void DropDownText(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A, float BGR, float BGG, float BGB, float BGA, float OutlineR, float OutlineG, float OutlineB, float OutlineA);
void DrawProgressBar(float x, float y, float w, float h, float min, float max, float current, float* ForeColour, float* BackColour);
void DrawLine(float X1, float Y1, float X2, float Y2, float Width, float R, float G, float B, float A);
void DrawWrappedText(const char *text, const char* fontname, float x, float y, float w, float h, float scale, float *color, int textAlignMode, rectDef_s *textRect);
float UI_DrawWrappedText(const char *text, const char* font, float x, float y, float w, float h, float scale, int textAlignMode, float R, float G, float B, float A);
void R_AddCmdDrawStretchPicRotateST(float x, float y, float w, float h, float centerS, float centerT, float radiusST, float scaleFinalS, float s1, float t1, float s2, float t2, float Angle, float* colour, Material* material);
void CL_DrawStretchPicRotatedST(int scrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, float centerS, float centerT, float radiusST, float scaleFinalS, float s1, float t1, float s2, float t2, float Angle, float* colour, Material* material);