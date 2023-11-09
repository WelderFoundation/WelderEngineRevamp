// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
ZilchDefineType(TBUIRenderer, builder, type)
{
  ZeroBindDocumented();
}
tb::TBBitmap* TBUIRenderer::CreateBitmap(int width, int height, uint32* data)
{
  TBUIBitmap* bitmap = new TBUIBitmap(this);
  if (!bitmap->Init(width, height, data))
  {
    delete bitmap;
    return nullptr;
  }

  return bitmap;
}
void TBUIRenderer::RenderBatch(Batch* batch)
{
  ColorTransform colorTx = {Vec4(1, 1, 1, 1)};
  Mat4 localTx;
  this->mTranslation = Vec3{0, 0, 0};
  this->mAngle = 0;
  Build2dTransform(localTx, this->mTranslation, this->mAngle);
  mWorldTx = localTx * Mat4::cIdentity;

  tb::TBRect rect = GetClipRect();

  Vec2 size = Vec2{(real)rect.w, (real)rect.h};
  Vec2 pos = Vec2{(real)rect.x, (real)rect.y};

  Rectangle clipRect = Rectangle::PointAndSize(Vec2(pos.x, pos.y), Vec2(size.x, size.y));

  Vec4 clearColor = Vec4{0.5f, 0.7f, 0.3f, 0.1f};

  GraphicsEngine* graphics = Z::gEngine->has(GraphicsEngine);
  RenderTasks& renderTasks = *graphics->mRenderTasksBack;
  RenderQueues& renderQueues = *graphics->mRenderQueuesBack;
  renderQueues.mRenderTasks = &renderTasks;

  FrameBlock& frameBlock = renderQueues.mFrameBlocks.PushBack();
  ViewBlock& viewBlock = renderQueues.mViewBlocks.PushBack();
  frameBlock.mRenderQueues = &renderQueues;

  Mat4 translation;
  translation.Translate(size.x * -0.5f, size.y * -0.5f, 0.0f);
  Mat4 scale;
  scale.Scale(1.0f, -1.0f, 1.0f);
  viewBlock.mWorldToView = scale * translation;
  BuildOrthographicTransformZero(viewBlock.mViewToPerspective, size.y, size.x / size.y, -1.0f, 1.0f);

  Mat4 apiPerspective;
  Z::gRenderer->BuildOrthographicTransform(apiPerspective, size.y, size.x / size.y, -1.0f, 1.0f);
  viewBlock.mZeroPerspectiveToApiPerspective = apiPerspective * viewBlock.mViewToPerspective.Inverted();

  //
  RenderBatch(viewBlock, frameBlock, batch, clipRect);
  //

  IndexRange& indexRange = viewBlock.mRenderGroupRanges.PushBack();
  indexRange.start = 0;
  indexRange.end = viewBlock.mViewNodes.Size();

  RenderTaskRange& renderTaskRange = renderTasks.mRenderTaskRanges.PushBack();
  renderTaskRange.mFrameBlockIndex = renderQueues.mFrameBlocks.Size() - 1;
  renderTaskRange.mViewBlockIndex = renderQueues.mViewBlocks.Size() - 1;
  renderTaskRange.mTaskIndex = renderTasks.mRenderTaskBuffer.mCurrentIndex;
  renderTaskRange.mTaskCount = 0;

  HandleOf<RenderTarget> renderTarget =
      Z::gEngine->has(GraphicsEngine)->GetRenderTarget((uint)size.x, (uint)size.y, TextureFormat::RGBA8);

  GraphicsRenderSettings renderSettings;
  renderSettings.SetColorTarget(renderTarget);
  renderSettings.mBlendSettings[0].SetBlendAlpha();
  renderSettings.mScissorMode = ScissorMode::Enabled;

  BoundType* defaultRenderPass = MetaDatabase::GetInstance()->FindType("ColorOutput");
  ReturnIf(defaultRenderPass == nullptr, , "We expected to have a type defined called ColorOutput");

  HandleOf<MaterialBlock> renderPassHandle = ZilchAllocate(MaterialBlock, defaultRenderPass);
  MaterialBlock& renderPass = renderPassHandle;

  Material* spriteMaterial = MaterialManager::FindOrNull("AlphaSprite");
  uint shaderInputsId = 0;

  {
    Pair<u64, uint> key((u64)spriteMaterial->mResourceId, shaderInputsId);
    IndexRange range = spriteMaterial->AddShaderInputs(renderTasks.mShaderInputs, renderTasks.mShaderInputsVersion);
    renderTasks.mShaderInputRanges.Insert(key, range);
  }
  {
    Pair<u64, uint> key(cFragmentShaderInputsId, shaderInputsId);
    IndexRange range = renderPass.AddShaderInputs(renderTasks.mShaderInputs);
    renderTasks.mShaderInputRanges.Insert(key, range);
  }

  RenderTaskHelper helper(renderTasks.mRenderTaskBuffer);
  //helper.AddRenderTaskClearTarget(renderSettings, clearColor, 0, 0, 0xFF);
  helper.AddRenderTaskRenderPass(renderSettings, 0, defaultRenderPass->Name, shaderInputsId);

  ScreenViewport viewport = {pos.x, pos.y, (int)size.x, (int)size.y};
  helper.AddRenderTaskBackBufferBlit(renderTarget, viewport);

  renderTaskRange.mTaskCount = 2;

  Z::gEngine->has(GraphicsEngine)->ClearRenderTargets();
}
void TBUIRenderer::SetClipRect(const tb::TBRect& rect)
{
  mClipRect = rect;
}

void TBUIRenderer::RenderBatch(ViewBlock& viewBlock,
                                  FrameBlock& frameBlock,
                                  tb::TBRendererBatcher::Batch* data,
                                  RectangleParam clipRect)
{
  Array<StreamedVertex> vertices = {};

  // Extract vertex data
  for (int i = 0; i < batch.vertex_count; i++)
  {
    {
      tb::TBRendererBatcher::Vertex tbVertex = batch.vertex[i];
      StreamedVertex vertex =
          StreamedVertex(Vec3(tbVertex.x, tbVertex.y, 0), Vec2(tbVertex.u, tbVertex.v), ToFloatColor(tbVertex.col));
      vertices.PushBack(vertex);
    }
  }

  // draw
  {

    //Rectangle clip = Rectangle::PointAndSize(Vec2(clipMin.x, clipMin.y), Vec2(clipMax.x, clipMax.y));

    TBUIBitmap* bitmap = (TBUIBitmap*)batch.bitmap;
    Texture* texture = reinterpret_cast<Texture*>(bitmap->GetTexture());

    CreateRenderData(viewBlock, frameBlock, clipRect, vertices, texture, PrimitiveType::Triangles);
  }
}

void TBUIRenderer::CreateRenderData(ViewBlock& viewBlock,
                                 FrameBlock& frameBlock,
                                 RectangleParam clipRect,
                                 Array<StreamedVertex>& vertices,
                                 Texture* texture,
                                 PrimitiveType::Enum primitiveType)
{
  if (vertices.Empty())
    return;

  StreamedVertexArray& streamedVertices = frameBlock.mRenderQueues->mStreamedVertices;

  ViewNode& viewNode = AddRenderNodes(viewBlock, frameBlock, clipRect, texture);

  for (uint i = 0; i < vertices.Size(); ++i)
  {
    StreamedVertex vertex = vertices[i];
    vertex.mPosition = Math::TransformPoint(viewNode.mLocalToView, vertex.mPosition);
    streamedVertices.PushBack(vertex);
  }

  viewNode.mStreamedVertexType = primitiveType;
  viewNode.mStreamedVertexCount = streamedVertices.Size() - viewNode.mStreamedVertexStart;

  // viewNode.mStreamedVertexStart = offset;
  // viewNode.mStreamedVertexCount = count;
}

ViewNode&
TBUIRenderer::AddRenderNodes(ViewBlock& viewBlock, FrameBlock& frameBlock, RectangleParam clipRect, Texture* texture)
{
  FrameNode& frameNode = frameBlock.mFrameNodes.PushBack();
  ViewNode& viewNode = viewBlock.mViewNodes.PushBack();

  frameNode.mGraphicalEntry = nullptr;
  viewNode.mGraphicalEntry = nullptr;

  frameNode.mBorderThickness = 1.0f;
  frameNode.mBlendSettingsOverride = false;
  frameNode.mRenderingType = RenderingType::Streamed;
  frameNode.mCoreVertexType = CoreVertexType::Streamed;
  frameNode.mLocalToWorld = mWorldTx.Transposed();
  viewNode.mFrameNodeIndex = frameBlock.mFrameNodes.Size() - 1;
  viewNode.mLocalToView = viewBlock.mWorldToView * frameNode.mLocalToWorld;

  // frameNode.mClip = Vec4(clipRect.GetLeft(), clipRect.GetTop(), clipRect.GetRight(), clipRect.GetBottom());
  frameNode.mClip = Vec4(clipRect.GetLeft(), clipRect.GetBottom(), clipRect.GetRight(), clipRect.GetTop());

  // maybe cache this lookup on root
  Material* spriteMaterial = nullptr;

  spriteMaterial = MaterialManager::FindOrNull("AlphaSprite");

  frameNode.mMeshRenderData = nullptr;
  frameNode.mMaterialRenderData = spriteMaterial->mRenderData;
  frameNode.mTextureRenderData = texture->mRenderData;

  // default setup for adding streamed data
  viewNode.mStreamedVertexType = PrimitiveType::Triangles;
  viewNode.mStreamedVertexStart = frameBlock.mRenderQueues->mStreamedVertices.Size();
  viewNode.mStreamedVertexCount = 0;

  return viewNode;
}
} // namespace Zero