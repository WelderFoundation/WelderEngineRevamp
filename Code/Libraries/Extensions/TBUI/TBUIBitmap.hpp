// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
class TBUIBitmap : public tb::TBBitmap
{
public:
  /// Meta Initialization.
  ZilchDeclareType(TBUIBitmap, TypeCopyMode::ReferenceType);

  TBUIBitmap(TBUIRenderer* renderer);

  /** Note: Implementations for batched renderers should call TBRenderer::FlushBitmap
to make sure any active batch is being flushed before the bitmap is deleted. */
  ~TBUIBitmap() override;

  bool Init(int width, int height, uint32* data);

  int Width() override;
  int Height() override;

  /** Update the bitmap with the given data (in BGRA32 format).
    Note: Implementations for batched renderers should call TBRenderer::FlushBitmap
    to make sure any active batch is being flushed before the bitmap is changed. */
  void SetData(uint32* data) override;

  ZeroForceInline Texture* GetTexture()
  {
    return mTexture;
  }

private:
  TBUIRenderer* mRenderer;
  int mWidth = 0;
  int mHeight = 0;
  HandleOf<Texture> mTexture;
};
} // namespace Zero