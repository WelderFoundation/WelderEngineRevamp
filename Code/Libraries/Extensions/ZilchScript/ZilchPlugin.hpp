// MIT Licensed (see LICENSE.md).
#pragma once

#include "ZilchScript.hpp"

namespace Zero
{
class BackgroundTask;
class UpdateEvent;
class ZilchPluginConfig;
class ZilchPluginLibrary;
class BackgroundTaskEvent;

class ZilchPluginSource : public DataResource
{
public:
  ZilchDeclareType(ZilchPluginSource, TypeCopyMode::ReferenceType);

  ZilchPluginSource();
  ~ZilchPluginSource();

  // Resource interface
  void Serialize(Serializer& stream) override;
  void EditorInitialize();

  void OnEngineUpdate(UpdateEvent* event);

  String GetContentDirectory();
  String GetCodeDirectory();
  String GetVersionsDirectory();
  String GetCurrentVersionDirectory();
  ZilchPluginLibrary* GetLibrary() const;

  // Copies the library and header files to the plugin shared directory
  // We pretty much litter this function in every call to open the plugin (ide,
  // folder, upon loading, etc)
  void ForceCopyPluginDependencies();
  void CopyPluginDependencies();
  void CopyPluginDependenciesOnce();
  void WriteCurrentVersionFile();

  void OpenDirectory();
  void OpenIde();
  void CompileDebug();
  void CompileRelease();
  void CompileConfiguration(StringParam configuration);
  void Clean();
  void InstallIdeTools();

  void OnCompilationCompleted(BackgroundTaskEvent* e);

  bool CheckIdeAndInformUser();
  bool IsIdeInstalled();
  void MarkAttemptedIdeToolsInstAll();

  ZilchPluginConfig* GetConfig();

  // Internals
  BackgroundTask* mCompileTask;

  // This counter starts at 0, and if the user attempts to open the IDE
  // without installing the IDE tools, then we launch the IDE tools installer
  // We have to wait for that installer to close, however the installer
  // seems to run itself multiple times (perhaps elevating permissions)
  // So we need a counter to be sure the installer is finished
  static const int InstallCounter = 20;
  int mOpenIdeAfterToolsInstallCounter;
};

class ZilchPluginSourceLoader : public ResourceLoader
{
public:
  HandleOf<Resource> LoadFromFile(ResourceEntry& entry) override;
  void ReloadFromFile(Resource* resource, ResourceEntry& entry) override;
};

class ZilchPluginSourceManager : public ResourceManager
{
public:
  DeclareResourceManager(ZilchPluginSourceManager, ZilchPluginSource);

  ZilchPluginSourceManager(BoundType* resourceType);
  ~ZilchPluginSourceManager();

  // ResourceManager Interface
  void ValidateNewName(Status& status, StringParam name, BoundType* optionalType) override;
  void ValidateRawName(Status& status, StringParam name, BoundType* optionalType) override;

  void OnResourceEvent(ResourceEvent* event);

  bool IsCompilingPlugins();

  // Internal
  // How many plugins we're currently compiling
  size_t mCompilingPluginCount;
};

class ZilchPluginLibrary : public ZilchLibraryResource
{
public:
  ZilchDeclareType(ZilchPluginLibrary, TypeCopyMode::ReferenceType);

  ZilchPluginLibrary();
  ~ZilchPluginLibrary();

  String SharedLibraryPath;
  String GetSharedLibraryPath() const override;
  Resource* GetOriginResource() const override;

  ZilchPluginSource* GetSource() const;
};

class ZilchPluginLibraryLoader : public ResourceLoader
{
public:
  HandleOf<Resource> LoadFromFile(ResourceEntry& entry) override;
};

class ZilchPluginLibraryManager : public ResourceManager
{
public:
  DeclareResourceManager(ZilchPluginLibraryManager, ZilchPluginLibrary);

  ZilchPluginLibraryManager(BoundType* resourceType);
  ~ZilchPluginLibraryManager();

  void OnResourceEvent(ResourceEvent* event);
};

} // namespace Zero
