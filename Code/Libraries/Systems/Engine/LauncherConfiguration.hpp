// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

class Cog;

/// To try to avoid accidentally changing command-line arguments for the
/// launcher, some of the common ones (ones used for communication) are added
/// here so they're shared between the engine and so on.
DeclareEnum8(LauncherStartupArguments,
             New,
             Open,
             Run,
             InstallAndRun,
             Upgrade,
             Projects,
             Tags,
             DebuggerMode /*Launcher in a special way so that the launcher gets
                             extra buttons where it will try to communicate back
                             to the active launcher*/
);

DeclareEnum3(LauncherAutoRunMode, None, IfInstalled, InstallAndRun);

namespace Events
{
DeclareEvent(ShowDevelopChanged);
DeclareEvent(ShowExperimentalBranchesChanged);
} // namespace Events

/// Configuration data for the version selector
class LauncherConfig : public Component
{
public:
  ZilchDeclareType(LauncherConfig, TypeCopyMode::ReferenceType);

  LauncherConfig();
  void Serialize(Serializer& stream);

  void ApplyCommandLineArguments();

  /// Will the version selector auto run? Will it install it if it wasn't
  /// installed?
  uint GetAutoRunMode();
  void SetAutoRunMode(uint mode);

  String GetTemplateInstallPath();
  String GetBuildsInstallPath();

  LauncherAutoRunMode::Type mAutoRunMode;

  /// The location that the launcher last ran.
  /// Used so zero can find out how to call the version selector.
  String mLauncherLocation;

  /// The root directory where templates, versions, etc... are installed to
  String mDownloadPath;
  String mDefaultProjectSaveLocation;

  /// Whether or not to display the build and build state on each
  /// project in the Recent Projects page.
  bool mDisplayBuildOnProjects;
  bool mAutoCheckForLauncherUpdates;
  bool mShowDevelopmentBuilds;
  bool mShowExperimentalBranches;
  bool mRunDebuggerMode;
  /// Should the launcher only show the preferred (current) platform builds or
  /// show all. Hardcoded to true for now since we only have one platform.
  bool mDisplayOnlyPreferredPlatform;
  /// How often the launcher should check for new builds/templates/etc...
  float mAutoUpdateFrequencyInSeconds;

  /// Number used to force the launcher to reinstall all versions
  int mForcedUpdateVersion;
  static int mCurrentForcedUpdateVersionNumber;
  static float mDefaultReloadFrequency;
  float mNewestLauncherUpdateCheckFrequency;

  // Should the launcher restart when it closes? Used to have
  // the launcher restart in order to update.
  bool mRestartOnClose;
};

class LauncherLegacySettings : public Component
{
  ZilchDeclareType(LauncherLegacySettings, TypeCopyMode::ReferenceType);

  LauncherLegacySettings();
  void Serialize(Serializer& stream);

  bool mDisplayLegacyBuilds;
};

} // namespace Zero
