// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
void Build2dTransform(Mat4& m, Vec3Param t, float r);

class TBUIRenderer : public tb::TBRendererBatcher
{
public:
  /// Meta Initialization.
  ZilchDeclareType(TBUIRenderer, TypeCopyMode::ReferenceType);

  tb::TBBitmap* CreateBitmap(int width, int height, uint32* data) override;
  void RenderBatch(Batch* batch) override;
  void SetClipRect(const tb::TBRect& rect) override;

private:
  void RenderBatch(ViewBlock& viewBlock, FrameBlock& frameBlock, tb::TBRendererBatcher::Batch* data, RectangleParam clipRect);
  void CreateRenderData(ViewBlock& viewBlock,
                        FrameBlock& frameBlock,
                        RectangleParam clipRect,
                        Array<StreamedVertex>& vertices,
                        Texture* texture,
                        PrimitiveType::Enum primitiveType);
  ViewNode& AddRenderNodes(ViewBlock& viewBlock, FrameBlock& frameBlock, RectangleParam clipRect, Texture* texture);

private:
  Vec3 mTranslation;
  float mAngle;

  Mat4 mWorldTx = Mat4::cIdentity;
  Texture* mFontTexture = nullptr;
  int mCurrentUpdateFrame = -1;
  int mCurrentRenderFrame = -1;

  tb::TBRect mClipRect;
};
} // namespace Zero