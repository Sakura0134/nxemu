#include "MacroInterpreter.h"
#include "Engine\Maxwell3D.h"
#include "VideoNotification.h"

CMacroInterpreter::CMacroInterpreter(CMaxwell3D & Maxwell3d) :
    m_Maxwell3d(Maxwell3d)
{
}

void CMacroInterpreter::AddCode(uint32_t Method, uint32_t Data)
{
    m_MacroCode[Method].push_back(Data);
}

void CMacroInterpreter::Execute(uint32_t /*Method*/, const MacroParams & /*Params*/)
{
    g_Notify->BreakPoint(__FILE__, __LINE__);
}