#pragma once

typedef int(*pCG_ObituaryStub)(int localClientNum, entityState_s *ent, int eventparam);
extern pCG_ObituaryStub CG_ObituaryStub;
void CG_ObituaryHook(int localClientNum, entityState_s *ent, int eventparam);