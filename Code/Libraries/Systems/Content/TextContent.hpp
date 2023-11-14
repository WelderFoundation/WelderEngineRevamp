// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

/// Text content item. Text content is content that is loaded
/// from plain text files. The editor can edit them with various
/// text editors.
class TextContent : public ContentComposition
{
public:
  ZilchDeclareType(TextContent, TypeCopyMode::ReferenceType);

  TextContent();
};

class BaseTextBuilder : public DirectBuilderComponent
{
public:
  ZilchDeclareType(BaseTextBuilder, TypeCopyMode::ReferenceType);

  void Generate(ContentInitializer& initializer) override;
  BaseTextBuilder(uint order, StringParam extension, StringParam resourceName) :
      DirectBuilderComponent(order, extension, resourceName)
  {
  }
};

const String TextResourceName = "Text";
const String TextExtension = ".txt";

/// Text Builder for generic text.
class TextBuilder : public BaseTextBuilder
{
public:
  ZilchDeclareType(TextBuilder, TypeCopyMode::ReferenceType);

  void SetDefaults()
  {
  }
  TextBuilder() : BaseTextBuilder(15, TextExtension, TextResourceName)
  {
  }
};

const String ZilchScriptResourceName = "ZilchScript";

/// Zilch Script File builder.
class ZilchScriptBuilder : public BaseTextBuilder
{
public:
  ZilchDeclareType(ZilchScriptBuilder, TypeCopyMode::ReferenceType);

  void SetDefaults()
  {
  }

  ZilchScriptBuilder() :
      BaseTextBuilder(
          15, FileExtensionManager::GetZilchScriptTypeEntry()->GetDefaultExtensionWithDot(), ZilchScriptResourceName)
  {
  }
};

class ZilchFragmentBuilder : public BaseTextBuilder
{
public:
  ZilchDeclareType(ZilchFragmentBuilder, TypeCopyMode::ReferenceType);

  void SetDefaults()
  {
  }

  ZilchFragmentBuilder()
      // Increase the load order to 9 so that these load before materials
      // (since a material may need the block created from this fragment)
      :
      BaseTextBuilder(
          9, FileExtensionManager::GetZilchFragmentTypeEntry()->GetDefaultExtensionWithDot(), "ZilchFragment")
  {
  }
};

} // namespace Zero
