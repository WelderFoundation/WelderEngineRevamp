// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
void YieldToOs()
{
}

void SetYieldToOsEnabled(bool enabled)
{
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
