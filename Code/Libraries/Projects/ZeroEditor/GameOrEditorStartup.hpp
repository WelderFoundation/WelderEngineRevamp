// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

class GameOrEditorStartup : public ZeroStartup
{
private:
  bool mPlayGame = false;
  Cog* mProjectCog = nullptr;
  String mProjectFile;
  String mNewProject;
  bool mEmbeddedPackage = false;
  String mEmbeddedOutputDirectory;

  void UserInitializeConfig(Cog* configCog) override;
  StartupPhaseResult::Enum UserInitialize() override;
  void UserStartup() override;
  void UserCreation() override;
};

} // namespace Zero
