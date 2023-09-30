// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
ContentItem* MakeZilchPluginContent(ContentInitializer& initializer);

class ZilchPluginBuilder : public DataBuilder
{
public:
  ZilchDeclareType(ZilchPluginBuilder, TypeCopyMode::ReferenceType);
  ZilchPluginBuilder();

  ResourceId mSharedLibraryResourceId;

  void Rename(StringParam newName) override;

  // Gets a platform and Zero revision dependent extension (ends with
  // zilchPlugin)
  static String GetSharedLibraryPlatformName();
  static String GetSharedLibraryPlatformBuildName();
  static String GetSharedLibraryExtension(bool includeDot);

  String GetSharedLibraryFileName();

  void Serialize(Serializer& stream) override;
  void Generate(ContentInitializer& initializer) override;
  void BuildContent(BuildOptions& buildOptions) override;
  void BuildListing(ResourceListing& listing) override;
};

void CreateZilchPluginContent(ContentSystem* system);
ContentItem* MakeZilchPluginContent(ContentInitializer& initializer);

} // namespace Zero
