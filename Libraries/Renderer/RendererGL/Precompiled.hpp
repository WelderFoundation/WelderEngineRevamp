// MIT Licensed (see LICENSE.md).

#include "RendererGLStandard.hpp"

// Include glew before OpenGl
#include <GL/glew.h>
#if defined(WelderTargetOsWindows)
#  include <GL/wglew.h>
#endif

// Include OpenGl
#include <GL/gl.h>