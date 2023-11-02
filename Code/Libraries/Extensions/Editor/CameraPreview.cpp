#include "Precompiled.hpp"
#include "CameraPreview.hpp"
#include "Widget/Viewport.hpp"
#include "EditorViewport.hpp"
#include "Engine/EditorSupport.hpp"
#include "Engine/Archetype.hpp"
#include "Engine/Action/BasicActions.hpp"
#include "Engine/Action/ActionGenerator.hpp"
#include "Widget/SimpleAnimation.hpp"
#include "Engine/Factory.hpp"
//#include "Gameplay/CameraViewport.hpp"
#include "Widget/RootWidget.hpp"
#include "Editor.hpp"

namespace Zero
{

// The amount of room (as a scalar of the target view-ports size)between the
// lower right corner of the target view-port and the Camera Preview window
const float cViewportMarginSize = 0.01f;
// The size of the Camera Preview window (as a scalar of the target
// view-ports size)
const float cPreviewViewportScalar = 0.25f;

// The speed at which the preview window pops in and out
const float cPopupSpeed = 0.2f;

// The amount of pixels off screen the window will pop in and out from
const float cPopPixels = Pixels(400.0f);

// The direction in which the window will slide (popping in / out)
const Vec3 cSlideDirection = Vec3(0, 1, 0);

CameraPreview::CameraPreview(
    Composite* parent, Cog* cameraObject, Cog* renderer, EditorViewport* targetViewport, MetaSelection* selection) :
    FloatingComposite(parent->GetRootWidget()->GetPopUp())
{
  mViewport = new Viewport(this, cameraObject->GetSpace(), cameraObject->Has<Camera>());

  mTargetViewport = targetViewport;
  mTargetCamera = cameraObject;
  mRenderer = renderer;

  //mViewport->SetCamera(cameraObject);
  //mViewport->SetTargetSpace(cameraObject->GetSpace());
  //// mViewport->SetRenderer(renderer);

  // We want to activate / deactivate ourself when the target viewport
  // is activated / deactivated
  ConnectThisTo(mTargetViewport, Events::Activated, Activate);
  ConnectThisTo(mTargetViewport, Events::Deactivated, Deactivate);

  // We want to destroy ourself when we're no longer selected
  ConnectThisTo(selection, Events::SelectionChanged, OnSelectionChanged);

  // We want to put ourselves off the bottom of the
  // screen so that we can slide in
  UpdateTransform(cSlideDirection * cPopPixels);

  // Slide into the viewport
  Slide(cSlideDirection * -cPopPixels, cPopupSpeed);
}

CameraPreview::~CameraPreview()
{
  // Renderer* renderer = mRenderer.has(Renderer);
  // if(renderer)
  //   renderer->DestroyBuffers();
}

void CameraPreview::Activate(Event* event)
{
  SetActive(true);
}

void CameraPreview::Deactivate(Event* event)
{
  SetActive(false);
}

void CameraPreview::OnSelectionChanged(SelectionChangedEvent* event)
{
  Cog* primaryCog = event->Selection->GetPrimaryAs<Cog>();

  // We want to delete the preview if there's nothing selected
  // or if the camera we're visualizing isn't the primary selected object
  if (primaryCog == NULL || primaryCog->GetId() != mTargetCamera)
  {
    // Slide it off the screen
    Slide(cSlideDirection * cPopPixels, cPopupSpeed);

    // Wait and destroy ourself
    ActionSequence* seq = new ActionSequence(this);
    seq->Add(new ActionDelay(cPopupSpeed * 0.95f));
    seq->Add(DestroyAction(this));
  }
}

void CameraPreview::UpdateTransform()
{
  // This updates the view-port to be in the lower right corner of the
  // target view-port.  We only want to do this if it's not currently
  // animating in or out of the view-port
  if (GetActions()->IsEmpty())
    UpdateTransform(Vec3::cZero);

  LayoutResult lr = RemoveThickness(mBackground->GetBorderThickness(), mSize);
  //mViewport->SetTranslation(SnapToPixels(lr.Translation));
  //mViewport->SetSize(SnapToPixels(lr.Size));

  FloatingComposite::UpdateTransform();
}

void CameraPreview::UpdateTransform(Vec3Param posOffset)
{
  Vec3 viewportSize = ToVector3(mTargetViewport->GetSize());
  Vec2 size = mTargetViewport->GetSize() * cPreviewViewportScalar;

  // Set our position to the lower right corner of the target view port
  Vec3 previewPosition = mTargetViewport->GetScreenPosition();

  // Move it up and to the left so that it doesn't sit directly on the corner
  previewPosition -= viewportSize * cViewportMarginSize;
  Vec3 pos = viewportSize - ToVector3(size) + previewPosition;
  pos += posOffset;

  SetTranslationAndSize(SnapToPixels(pos), SnapToPixels(size));
}

 void TryOpenPreview(Cog* cameraCog, EditorViewport* targetViewport, MetaSelection* selection)
{
   Camera* camera = cameraCog->has(Camera);
   if(camera == NULL)
     return;

   Cog* viewport = camera->GetCameraViewportCog();
   if(viewport == NULL)
     return;

   CameraViewport* cameraViewport = viewport->has(CameraViewport);
   if(cameraViewport == NULL)
     return;

   Cog* rendererCog = cameraViewport->mRendererPath.GetCog();
   if(rendererCog == NULL)
     return;

   ////No renderer found use editor renderer
   ////if(rendererCog == NULL)
   ////  rendererCog = cameraCog->GetSpace()->FindObjectByName(SpecialCogNames::EditorCamera);

   ////Renderer* renderer = rendererCog->has(Renderer);
   ////if(renderer == NULL)
   ////  return;

   ////if(renderer->InUse())
   ////  return;

   CameraPreview* camPreview = new CameraPreview(Z::gEditor, cameraCog, rendererCog, targetViewport, selection);

 }

} // namespace Zero