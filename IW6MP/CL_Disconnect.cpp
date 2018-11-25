#include "stdafx.h"
#include "CL_Disconnect.h"
#include "Functions.h"
bool bDisConnected;

pCL_DisconnectStub CL_DisconnectStub;

void CL_DisconnectHook(int localClientNum, bool deactivateClient) {

	__try
	{
		if (!localClientNum && deactivateClient)
			bDisConnected = deactivateClient;

		CL_DisconnectStub(localClientNum, deactivateClient);
	}
	__except (filter("CL_Disconnect", GetExceptionCode(), GetExceptionInformation()))
	{
		CL_DisconnectStub(localClientNum, deactivateClient);
	}
}
