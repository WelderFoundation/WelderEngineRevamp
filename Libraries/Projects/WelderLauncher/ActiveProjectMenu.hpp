// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

class ActiveProjectMenu : public Composite
{
public:
  /// Typedefs.
  typedef ActiveProjectMenu ZilchSelf;

  /// Returns the modal the project ui was created under.
  static Modal* OpenProject(Composite* modalParent, CachedProject* cachedProject, LauncherWindow* launcher);

  /// Constructor.
  ActiveProjectMenu(Composite* parent, LauncherWindow* launcher);
  void UpdateTransform() override;
  void SetSizeXToRemainder(Widget* widgetToSize);

  /// Update the launcher button when we're closed.
  void OnModalClosed(Event* e);

  /// Update the screenshot from the cached project.
  void UpdateScreenshot();
  void OnScreenshotUpdated(Event* e);

  /// Displays information about the given project.
  void SelectProject(CachedProject* cachedProject);

  /// The back button has been pressed (close the modal).
  void OnBackButtonPressed(Event* e);

  /// Check for F2 to attempt to rename the project file
  void OnKeyDown(KeyboardEvent* e);
  /// The user clicked the Edit Project Name button.
  void OnEditProjectName(Event* e);
  /// The user changed their project's name, attempt to confirm
  void OnProjectNameTextSubmit(Event* e);
  void OnProjectNameFocusLost(Event* e);
  void OnProjectRenameCanceled(Event* e);
  /// The user has confirmed if they want to rename their project file
  void OnProjectRenameConfirmed(ModalButtonEvent* e);
  /// The user has requested that their project be backed-up
  void OnBackupProject(Event* e);

  /// Update the project size text as the size is calculated.
  void OnSizeTaskUpdate(BackgroundTaskEvent* e);

  /// Update the main button when the state of the build changes.
  void OnBuildStateChanged(Event* e);
  void OnConfirmBuildVersionChange(ModalConfirmEvent* e);

  /// When the main button is pressed we want to launch the project.
  void OnLaunchProject(Event* e);

  /// New versions have been loaded, make sure to update the project's page
  /// (since it may have been associated with a previously un-available build).
  void OnVersionListLoaded(Event* e);
  /// Opens the project folder for this project.
  void OnShowProjectFolder(Event* e);
  void OnInstallPressed(Event* e);
  void OnRunWithDebugger(Event* e);

  void OnDownloadCompleted(Event* e);

  IconButton* mBackButton;
  TextBox* mProjectName;
  Text* mDate;
  Text* mSize;
  Text* mTags;
  Text* mLocation;

  IconButton* mEditProjectNameIconButton;
  IconButton* mInstallBuild;
  TextButton* mRunWithDebugger;
  TextButton* mBackupProjectButton;

  Widget* mNoScreenshotImage;
  TextureView* mProjectImage;

  CachedProject* mCachedProject;
  BackgroundTask* mActiveSizeTask;
  BuildSelector* mBuildSelector;

  LauncherWindow* mLauncher;
};

} // namespace Zero
