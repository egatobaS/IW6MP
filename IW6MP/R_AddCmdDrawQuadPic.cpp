#include "stdafx.h"
#include "R_AddCmdDrawQuadPic.h"
#include "Menu.h"

pR_AddCmdDrawQuadPicStub R_AddCmdDrawQuadPicStub;

void R_AddCmdDrawQuadPicHook(float const* verts[2], float const* color, Material* material)
{
	__try
	{
		//printf("material - %s\n", material->Name);

		if (cuser_strcmp(material->Name, "hud_minimap_border"))
			return;

		R_AddCmdDrawQuadPicStub(verts, color, material);
	}
	__except (filter("R_AddCmdDrawQuadPic", GetExceptionCode(), GetExceptionInformation()))
	{
		R_AddCmdDrawQuadPicStub(verts, color, material);
	}
}