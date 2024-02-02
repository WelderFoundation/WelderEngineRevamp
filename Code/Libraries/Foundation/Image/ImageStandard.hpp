// MIT Licensed (see LICENSE.md).
#pragma once

#include "Common/CommonStandard.hpp"
#include "Common/Platform/PlatformStandard.hpp"

namespace Zero
{
// This must mirror TextureFormat in RendererBase
DeclareEnum25(ImageFormat,
              None,
              R8,
              RG8,
              RGB8,
              RGBA8, // byte
              R16,
              RG16,
              RGB16,
              RGBA16, // short
              R16f,
              RG16f,
              RGB16f,
              RGBA16f, // half float
              R32f,
              RG32f,
              RGB32f,
              RGBA32f, // float
              SRGB8,
              SRGB8A8, // gamma
              Depth16,
              Depth24,
              Depth32,
              Depth32f, // depth
              Depth24Stencil8,
              Depth32fStencil8Pad24); // depth-stencil
} // namespace Zero

#include "ImageSupport.hpp"
#include "PngSupport.hpp"
#include "HdrSupport.hpp"
