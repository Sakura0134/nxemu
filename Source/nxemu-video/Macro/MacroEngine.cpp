#include "MacroEngine.h"
#include "MacroInterpreter.h"

std::unique_ptr<MacroEngine> GetMacroEngine(CMaxwell3D& maxwell3d)
{
    return std::make_unique<CMacroInterpreter>(maxwell3d);
}