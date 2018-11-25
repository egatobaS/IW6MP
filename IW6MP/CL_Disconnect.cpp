#include "stdafx.h"
#include "CL_Disconnect.h"
bool bDisConnected;

pCL_DisconnectStub CL_DisconnectStub;

void CL_DisconnectHook(int localClientNum, bool deactivateClient) {

	if (!localClientNum && deactivateClient)
		bDisConnected = deactivateClient;

	CL_DisconnectStub(localClientNum, deactivateClient);
}
