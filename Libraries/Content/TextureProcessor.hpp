// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

class TextureProcessor
{
public:
  TextureProcessor(TextureContent* textureContent, String outputPath, String inputFile);

  void ExtractAndImportTextures(const aiScene* scene);
  void CreateTexture(aiTexture* texture, uint textureIndex, StringParam extension);

  TextureContent* mTextureContent;
  String mOutputPath;
  String mFilename;
};

} // namespace Zero
