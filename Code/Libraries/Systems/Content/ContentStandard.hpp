// MIT Licensed (see LICENSE.md).
#pragma once

#include "Common/CommonStandard.hpp"
#include "Platform/PlatformStandard.hpp"
#include "Engine/EngineStandard.hpp"
#include "SpatialPartition/SpatialPartitionStandard.hpp"
#include "Image/ImageStandard.hpp"
#include "RendererBase/RendererBaseStandard.hpp"

namespace Zero
{
ImageFormat::Enum TextureFormatToImageFormat(TextureFormat::Enum format);
TextureFormat::Enum ImageFormatToTextureFormat(ImageFormat::Enum format);
} // namespace Zero

namespace Zero
{
// Forward declarations
/*
class ContentLibrary;
class ContentItem;
class ContentComponent;
class BuildOptions;
class ContentComposition;
*/

// Content library
class ZeroNoImportExport ContentMetaLibrary : public Zilch::StaticLibrary
{
public:
  ZilchDeclareStaticLibraryInternals(ContentMetaLibrary, "ZeroEngine");

  static void Initialize();
  static void Shutdown();
};

} // namespace Zero

// Our includes
#include "FileExtensionManager.hpp"
#include "ZilchPluginContent.hpp"
#include "TextureBuilder.hpp"
#include "AudioFileEncoder.hpp"
#include "AudioContent.hpp"
#include "ImportOptions.hpp"
#include "ImageContent.hpp"
#include "SpriteBuilder.hpp"
#include "TextContent.hpp"
#include "GeometryContent.hpp"
#include "MeshBuilder.hpp"
