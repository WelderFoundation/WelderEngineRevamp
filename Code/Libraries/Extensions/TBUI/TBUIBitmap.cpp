// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(TBUIBitmap, builder, type)
{
  ZeroBindDocumented();
}
TBUIBitmap::TBUIBitmap(TBUIRenderer* renderer) : mRenderer(renderer), mTexture(nullptr)
{
}
TBUIBitmap::~TBUIBitmap()
{
  mRenderer->FlushBitmap(this);
}
bool TBUIBitmap::Init(int width, int height, uint32* data)
{
  mTexture = Texture::CreateRuntime();
  StringBuilder nameBuilder;
  nameBuilder.AppendFormat("tbui_bitmap_%i_%i", width, height);
  mTexture->Name = nameBuilder.ToString();

  mTexture->SetAddressingX(TextureAddressing::Enum::Repeat);
  mTexture->SetAddressingY(TextureAddressing::Enum::Repeat);
  mTexture->SetMipMapping(TextureMipMapping::Enum::GpuGenerated);

  mWidth = width;
  mHeight = height;

  SetData(data);
  return true;
}
int TBUIBitmap::Width()
{
  return mWidth;
}
int TBUIBitmap::Height()
{
  return mHeight;
}
void TBUIBitmap::SetData(uint32* data)
{
  uint size = GetPixelSize(TextureFormat::RGBA8) * mWidth * mHeight;
  mTexture->Upload(mWidth, mHeight, TextureFormat::Enum::RGBA8, (::byte*)data, size);
}
} // namespace Zero