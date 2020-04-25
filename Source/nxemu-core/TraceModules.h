#pragma once
#include <Common/TraceModulesCommon.h>

enum TraceModuleNXEmu
{
    TraceSettings = MaxTraceModuleCommon,
    TraceAppInit,
    TraceAppCleanup,
    TraceUserInterface,
    TraceGameFile,
    TraceServiceCall,
    MaxTraceModuleNXEmu,
};