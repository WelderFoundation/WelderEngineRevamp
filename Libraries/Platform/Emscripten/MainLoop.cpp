// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
bool gYieldToOsEnabled = true;
void YieldToOs()
{
  if (gYieldToOsEnabled)
    emscripten_sleep(0);
}

void SetYieldToOsEnabled(bool enabled)
{
  gYieldToOsEnabled = enabled;
}

ZeroThreadLocal bool gStopMainLoop = false;

void RunMainLoop(MainLoopFn callback, void* userData)
{
  while (!gStopMainLoop)
    callback(userData);

  gStopMainLoop = false;
}

void StopMainLoop()
{
  gStopMainLoop = true;
}

} // namespace Zero
