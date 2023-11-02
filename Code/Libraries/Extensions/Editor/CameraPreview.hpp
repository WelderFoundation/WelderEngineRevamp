#pragma once

namespace Zero
{
/// Forward Declarations
class EditorViewport;
class MetaSelection;
struct SelectionChangedEvent;
class Viewport;

/// Composite used for visualizing a camera
class CameraPreview : public FloatingComposite
{
public:
  typedef CameraPreview ZilchSelf;

  CameraPreview(
      Composite* parent, Cog* camera, Cog* renderer, EditorViewport* targetViewport, MetaSelection* selection);
  ~CameraPreview();

  /// Show / Hide when the viewport is activated / deactivated
  void Activate(Event* event);
  void Deactivate(Event* event);

  /// We want to destroy ourselves if we're no longer in the selection
  void OnSelectionChanged(SelectionChangedEvent* event);

  void UpdateTransform() override;
  void UpdateTransform(Vec3Param posOffset);

  /// Handle to the camera being displayed
  CogId mTargetCamera;

  /// The view port we're "attached" to
  EditorViewport* mTargetViewport;

  /// Our view port we're using to display what the target camera is looking at
  Viewport* mViewport;

  /// The renderer used to display the scene
  CogId mRenderer;
};

void TryOpenPreview(Cog* cameraCog, EditorViewport* targetViewport, MetaSelection* selection);
} // namespace Zero