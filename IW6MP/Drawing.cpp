#include "stdafx.h"
#include "Drawing.h"

void SetShader(const char* material, float x, float y, float w, float h, float R, float G, float B, float A)
{
	float Color[4] = { R, G, B, A };

	R_AddCmdDrawStretchPic(x, y, w, h, 0, 0, 1, 1, Color, Material_RegisterHandle(material, 0));
}

void SetShader(Material* materialIndex, float x, float y, float w, float h, float R, float G, float B, float A)
{
	float Color[4] = { R, G, B, A };
	R_AddCmdDrawStretchPic(x, y, w, h, 0, 0, 1, 1, Color, materialIndex);
}

void SetText(const char *text, const char* font, float x, float y, float Scale, float R, float G, float B, float A)
{
	if (text != NULL && !cuser_strcmp(text, ""))
	{
		float Color[4] = { R, G, B, A };
		R_AddCmdDrawText(text, 0x7FFFFFFF, R_RegisterFont(font, 0), x, y, Scale, Scale, 0, Color, 0);
	}
}

void SetText(const char *text, const char* font, float x, float y, float Scale, float R, float G, float B, float A, float Angle)
{
	if (text != NULL && !cuser_strcmp(text, ""))
	{
		float Color[4] = { R, G, B, A };
		R_AddCmdDrawText(text, 0x7FFFFFFF, R_RegisterFont(font, 0), x, y, Scale, Scale, Angle, Color, 0);
	}
}

void DrawCheckBox(const char* material, float x, float y, float w, float h, float R, float G, float B, float A, bool status)
{
	DrawBox("white", x, y, w, h, 0.6, 0, 0, 0, 1, 1, 1, 1, 1);
	if (status)
		SetShader(material, x, y, w, h, R, G, B, A);
}

void DrawSwitch(float x, float y, float w, float h, float R, float G, float B, float A, bool status)
{
	/*
	DrawBox("white", x - (w / 2), y, w, h, 0.6, 0, 0, 0, 1, 1, 1, 1, 1);
	if (status)
	SetShader("white", x, y, w / 2, h, 0, 1, 0, A);
	else
	SetShader("white", x - (w / 2), y, w / 2, h, 1, 0, 0, A);
	*/
	if (status) {
		DrawBox("white", x - (w / 2), y, w, h, 0.6, 0, 1, 0, 0.6, 1, 1, 1, 1);
		SetShader("white", (x + (w / 2)) - 7.0f, (float)(y - 2.0f), 7.0f, (float)(h + 4.0f), 0.8, 0.8, 0.8, 1);
	}
	else {
		DrawBox("white", x - (w / 2), y, w, h, 0.6, 1, 0, 0, 0.5, 1, 1, 1, 1);
		SetShader("white", (x - (w / 2)), (float)(y - 2.0f), 7.0f, (float)(h + 4.0f), 0.8, 0.8, 0.8, 1);
	}
}

void DrawBox(const char* material, float x, float y, float w, float h, float R, float G, float B, float A, float OutlineR, float OutlineG, float OutlineB, float OutlineA)
{
	SetShader(material, x, y, w, h, R, G, B, A);
	SetShader(material, x, (y - 1), w, 1, OutlineR, OutlineG, OutlineB, OutlineA);//top
	SetShader(material, x, (y + h), w, 1, OutlineR, OutlineG, OutlineB, OutlineA);//bottom
	SetShader(material, (x - 1), (y - 1), 1, (h + 2), OutlineR, OutlineG, OutlineB, OutlineA);//Left
	SetShader(material, (x + w), (y - 1), 1, (h + 2), OutlineR, OutlineG, OutlineB, OutlineA);//Right
}

void DrawBox(const char* material, float x, float y, float w, float h, float OutW, float R, float G, float B, float A, float OutlineR, float OutlineG, float OutlineB, float OutlineA)
{
	SetShader(material, x, y, w, h, R, G, B, A); //1
	SetShader(material, x, (y - OutW), w, OutW, OutlineR, OutlineG, OutlineB, OutlineA);//top
	SetShader(material, x, (y + h), w, OutW, OutlineR, OutlineG, OutlineB, OutlineA);//bottom
	SetShader(material, (x - OutW), (y - OutW), OutW, (h + (OutW * 2)), OutlineR, OutlineG, OutlineB, OutlineA);//Left
	SetShader(material, (x + w), (y - OutW), OutW, (h + (OutW * 2)), OutlineR, OutlineG, OutlineB, OutlineA);//Right
}

void DrawBoxNoTopMenu(const char* material, float x, float y, float w, float h, float OutW, float R, float G, float B, float A, float OutlineR, float OutlineG, float OutlineB, float OutlineA)
{
	SetShader(material, x, y, w, h, R, G, B, A); //1
	SetShader(material, x, (y + h), w, OutW, OutlineR, OutlineG, OutlineB, OutlineA);//bottom
	SetShader(material, (x - OutW), (y - OutW), OutW, (h + (OutW * 2)), OutlineR, OutlineG, OutlineB, OutlineA);//Left
	SetShader(material, (x + w), (y - OutW), OutW, (h + (OutW * 2)), OutlineR, OutlineG, OutlineB, OutlineA);//Right
}

void DrawBoxNoTop(const char* material, float x, float y, float w, float h, float OutW, float R, float G, float B, float A, float OutlineR, float OutlineG, float OutlineB, float OutlineA)
{
	SetShader(material, x, y, w, h, R, G, B, A); //1
	SetShader(material, (x - OutW), (y + h), (w + (OutW * 2)), OutW, OutlineR, OutlineG, OutlineB, OutlineA);//bottom
	SetShader(material, (x - OutW), y, OutW, h, OutlineR, OutlineG, OutlineB, OutlineA);//Left
	SetShader(material, (x + w), y, OutW, h, OutlineR, OutlineG, OutlineB, OutlineA);//Right
}

void SetTextWithBackGround(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A, float BGR = 0, float BGG = 0, float BGB = 0, float BGA = 0.7)
{
	SetShader("white", (x - 2), (y - TextHeight(font, yScale)), ((TextWidth(text, font) * xScale) + 4), TextHeight(font, yScale), BGR, BGG, BGB, BGA);
	float Color[4] = { R, G, B, A };
	R_AddCmdDrawText(text, 0x7FFFFFFF, R_RegisterFont(font, 0), x, y, xScale, yScale, 0, Color, 0);
}

void SetTextCenteredWithBackGround(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A, float BGR = 0, float BGG = 0, float BGB = 0, float BGA = 0.7)
{
	SetShader("white", ((x - 2) - ((TextWidth(text, font) * xScale) / 2)), (y - TextHeight(font, yScale)), ((TextWidth(text, font) * xScale) + 4), TextHeight(font, yScale), BGR, BGG, BGB, BGA);
	float Color[4] = { R, G, B, A };
	R_AddCmdDrawText(text, 0x7FFFFFFF, R_RegisterFont(font, 0), (x - ((TextWidth(text, font) * xScale) / 2)), y, xScale, yScale, 0, Color, 0);
}

void SetTextWithBackGround(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A, float BGR, float BGG, float BGB, float BGA, float OutlineR, float OutlineG, float OutlineB, float OutlineA)
{
	DrawBox("white", (x - 2), (y - TextHeight(font, yScale)), ((TextWidth(text, font) * xScale) + 4), TextHeight(font, yScale), 3, BGR, BGG, BGB, BGA, OutlineR, OutlineG, OutlineB, OutlineA);
	float Color[4] = { R, G, B, A };
	R_AddCmdDrawText(text, 0x7FFFFFFF, R_RegisterFont(font, 0), x, y, xScale, yScale, 0, Color, 3);
}

void SetTextRightWithBackGround(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A, float BGR, float BGG, float BGB, float BGA, float OutlineR, float OutlineG, float OutlineB, float OutlineA)
{
	DrawBox("white", ((x - 4) - ((TextWidth(text, font)) * xScale)), (y - TextHeight(font, yScale)), ((TextWidth(text, font) * xScale) + 8), TextHeight(font, yScale), 1, BGR, BGG, BGB, BGA, OutlineR, OutlineG, OutlineB, OutlineA);
	float Color[4] = { R, G, B, A };
	R_AddCmdDrawText(text, 0x7FFFFFFF, R_RegisterFont(font, 0), (x - ((TextWidth(text, font)) * xScale)), y, xScale, yScale, 0, Color, 0);
}

void DropDownText(const char *text, const char* font, float x, float y, float xScale, float yScale, const float R, const float G, const float B, const float A, float BGR, float BGG, float BGB, float BGA, float OutlineR, float OutlineG, float OutlineB, float OutlineA)
{
	DrawBox("white", (x - 88), (y - TextHeight(font, yScale)), 88, TextHeight(font, yScale), 1, BGR, BGG, BGB, BGA, OutlineR, OutlineG, OutlineB, OutlineA);
	float Color[4] = { R, G, B, A };
	R_AddCmdDrawText(text, 0x7FFFFFFF, R_RegisterFont(font, 0), ((x - 44) - ((TextWidth(text, font) * xScale) / 2)), y, xScale, yScale, 0, Color, 0);
}

void DrawProgressBar(float x, float y, float w, float h, float min, float max, float current, float* ForeColour, float* BackColour)
{
	float BackGroundOffset = 2.5;
	float fMultiplier = (w / (max - min));
	float ProgressBarLength = ((current - min) * fMultiplier);

	SetShader("white", x - (w + BackGroundOffset), y - ((h + BackGroundOffset) / 2), w + BackGroundOffset, h + BackGroundOffset, BackColour[0], BackColour[1], BackColour[2], BackColour[3]);
	SetShader("white", x - (w + (BackGroundOffset / 2)), y - (h / 2), ProgressBarLength, h, ForeColour[0], ForeColour[1], ForeColour[2], ForeColour[3]);
}

void DrawLine(float X1, float Y1, float X2, float Y2, float Width, float R, float G, float B, float A)
{
	float X, Y, Angle, L1, L2, H1;
	H1 = (Y2 - Y1);
	L1 = (X2 - X1);
	L2 = sqrt(L1 * L1 + H1 * H1);
	X = X1 + ((L1 - L2) / 2);
	Y = Y1 + (H1 / 2);
	Angle = (float)(atanf(H1 / L1) * 57.29577951308233);

	float Color[4] = { R, G, B, A };
	R_AddCmdDrawStretchPicRotateXY(X, Y, L2, Width, 0, 0, 0, 0, Angle, Color, "white");
}

void DrawWrappedText(const char *text, const char* fontname, float x, float y, float w, float h, float scale, float *color, int textAlignMode, rectDef_s *textRect)
{
	int font = R_RegisterFont(fontname, 0);
	const char *wrapPosition;
	float lineX;
	float height;
	float textWidth;
	int len;
	signed int targetLineWidth;
	float lineWidth;
	char buff[1024];
	int xAlignMode;
	const char *p;
	float xa;
	float ya;

	textRect->x = x + w;
	textRect->y = y;
	textRect->w = 0;
	textRect->h = 0;
	height = (float)TextHeight(fontname, scale);
	textWidth = (float)TextWidth(text, fontname) * scale;
	xAlignMode = textAlignMode & 3;
	xa = x;
	ya = y;

	targetLineWidth = w;
	p = text;
	while (*p)
	{
		const char *(__cdecl *R_TextLineWrapPosition)(const char *text, int bufferSize, int pixelsAvailable, int font, float scale) = (const char *(__cdecl*)(const char *text, int bufferSize, int pixelsAvailable, int font, float scale))addr->R_TextLineWrapPosition;
		wrapPosition = R_TextLineWrapPosition(p, 1024, targetLineWidth, font, scale);
		len = wrapPosition - p;

		memcpy(buff, (char *)p, len);
		buff[len] = 0;
		lineWidth = (float)TextWidth(buff, fontname) * scale;
		if (xAlignMode == 1)
		{
			lineX = xa - (float)(lineWidth * 0.5);
		}
		else if (xAlignMode == 2)
			lineX = xa - lineWidth;
		else
			lineX = xa;

		R_AddCmdDrawText(buff, 0x7FFFFFFF, font, lineX, ya, scale, scale, 0, color, 0);

		ya = ya + height;
		if (lineWidth > textRect->w)
		{
			textRect->x = lineX;
			textRect->w = lineWidth;
		}
		for (p += len; *p != 10 && isspace(*p); ++p)
			;
	}
	textRect->h = ya - textRect->y;
}

float UI_DrawWrappedText(const char *text, const char* font, float x, float y, float w, float h, float scale, int textAlignMode, float R, float G, float B, float A)
{
	if (text)
	{
		rectDef_s textRect;
		textRect.x = x;
		textRect.y = y;
		textRect.w = w;
		textRect.h = h;
		float Colour[4] = { R, G, B, A };
		DrawWrappedText(text, font, x, y, w, h, scale, Colour, textAlignMode, &textRect);
		return textRect.h;
	}
	return 0;
}

void R_AddCmdDrawStretchPicRotateST(float x, float y, float w, float h, float centerS, float centerT, float radiusST, float scaleFinalS, float s1, float t1, float s2, float t2, float Angle, float* colour, Material* material)
{
	int r11 = *(DWORD*)0x8431C270;
	if ((signed int)(((*(DWORD *)(r11 + 8) - *(DWORD *)(r11 + 4)) + *(DWORD*)0x84445380) - 0x2000) >= 0x40)
	{
		int r31 = (*(DWORD*)(r11)+*(DWORD*)(r11 + 0x4));

		*(DWORD*)(r11 + 0x4) = (*(DWORD*)(r11 + 0x4) + 0x40);
		*(DWORD*)(r11 + 0xC) = r31;
		*(WORD*)(r31) = 0xC;
		*(WORD*)(r31 + 0x2) = 0x40;
		Material* materialPointer = material;
		if (!material)
			materialPointer = *(Material**)0x842BE2AC;
		*(float*)(r31 + 0x8) = x;
		*(Material**)(r31 + 0x4) = materialPointer;
		DWORD* r4 = (DWORD *)(r31 + 0x38);
		*(float*)(r31 + 0xC) = y;
		*(float*)(r31 + 0x10) = w;
		*(float*)(r31 + 0x14) = h;
		*(float*)(r31 + 0x18) = centerS;
		*(float*)(r31 + 0x1C) = centerT;
		*(float*)(r31 + 0x20) = radiusST;
		*(float*)(r31 + 0x24) = scaleFinalS;
		*(float*)(r31 + 0x28) = s1;
		*(float*)(r31 + 0x2C) = t1;
		*(float*)(r31 + 0x30) = s2;
		*(float*)(r31 + 0x34) = t2;
		if (colour)
			((void(*)(...))addr->R_ConvertColorToBytes)(colour, r4); //R_ConvertColorToBytes
		else
			*r4 = -1;
		((void(*)(...))addr->AngleNormalize360)(Angle); //Normalize Angle
		*(float*)(r31 + 0x3C) = Angle;
	}
	else
		*(DWORD*)(r11 + 0xC) = 0;
}

void CL_DrawStretchPicRotatedST(int scrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, float centerS, float centerT, float radiusST, float scaleFinalS, float s1, float t1, float s2, float t2, float Angle, float* colour, Material* material)
{
	//((void(*)(...))0x82315010)(scrPlace, &x, &y, &w, &h, horzAlign, vertAlign);
	((void(*)(...))0x8265A6C0)(material, &s1, &t1, &s2, &t2);
	R_AddCmdDrawStretchPicRotateST(x, y, w, h, centerS, centerT, radiusST, scaleFinalS, s1, t1, s2, t2, Angle, colour, material);
}