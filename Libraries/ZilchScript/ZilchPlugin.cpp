// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

// This is probably a bad dependency, but there's not many
// other places that make sense for launching the background task
#include "Editor/SimpleBackgroundTasks.hpp"

namespace Zero
{
ZilchDefineType(ZilchPluginSource, builder, type)
{
  ZilchBindMethodProperty(OpenIde);
  ZilchBindMethodProperty(OpenDirectory);
  ZilchBindMethodProperty(CopyPluginDependencies);
  ZilchBindMethodProperty(CompileDebug);
  ZilchBindMethodProperty(CompileRelease);
  ZilchBindMethodProperty(Clean);
  ZilchBindMethodProperty(InstallIdeTools);
}

ZilchPluginSource::ZilchPluginSource()
{
  mCompileTask = nullptr;
  mOpenIdeAfterToolsInstallCounter = 0;
}

ZilchPluginSource::~ZilchPluginSource()
{
}

void ZilchPluginSource::Serialize(Serializer& stream)
{
}

void ZilchPluginSource::EditorInitialize()
{
  ConnectThisTo(Z::gEngine, Events::EngineUpdate, OnEngineUpdate);
}

String ZilchPluginSource::GetContentDirectory()
{
  if (mContentItem == nullptr)
    return String();

  return mContentItem->mLibrary->SourcePath;
}

String ZilchPluginSource::GetCodeDirectory()
{
  return FilePath::Combine(GetContentDirectory(), Name);
}

String ZilchPluginSource::GetVersionsDirectory()
{
  return FilePath::Combine(GetCodeDirectory(), "Versions");
}

String ZilchPluginSource::GetCurrentVersionDirectory()
{
  return FilePath::Combine(GetVersionsDirectory(), ZilchPluginBuilder::GetSharedLibraryPlatformBuildName());
}

ZilchPluginLibrary* ZilchPluginSource::GetLibrary() const
{
  return ZilchPluginLibraryManager::FindOrNull(Name);
}

void CopyGeneratedSource(StringParam destFileName, StringParam code)
{
  if (FileExists(destFileName))
  {
    Status status;
    if (CompareFileAndString(status, destFileName, code) == false)
      DeleteFile(destFileName);
    else
      return;
  }
  WriteStringRangeToFile(destFileName, code);
}

void ZilchPluginSource::ForceCopyPluginDependencies()
{
  // Only do this code when we have content loaded
  if (mContentItem == nullptr)
    return;

  Cog* configCog = Z::gEngine->GetConfigCog();
  MainConfig* mainConfig = configCog->has(MainConfig);

  // All plugin dependencies (library and headers) are located within our data
  // directory This is copied here automatically by post build events We place
  // it under a directory mangled by the platform name so that we can support
  // both Debug/Release builds (and possibly x86/x64 in the future) This ALSO
  // needs to partially match where we place the lib/all-to-one header in the
  // Data directory (does not include the version number since every version is
  // separated by the launcher) This is setup within the Zero engine build
  // process (typically as a post build event)
  String platformName = ZilchPluginBuilder::GetSharedLibraryPlatformName();
  String sourceVersionDir = FilePath::Combine(mainConfig->DataDirectory, "ZilchCustomPluginShared", platformName);

  String destVersionDir = GetCurrentVersionDirectory();
  CreateDirectoryAndParents(destVersionDir);

  // Copy all the files from the source directory to the destination directory
  for (FileRange files(sourceVersionDir); !files.Empty(); files.PopFront())
  {
    String sourceFileName = files.Front();
    String sourceFile = FilePath::Combine(sourceVersionDir, sourceFileName);
    String destFile = FilePath::Combine(destVersionDir, sourceFileName);

    if (FileExists(destFile))
    {
      Status status;
      if (Zero::CompareFile(status, sourceFile, destFile) == false)
        CopyFile(destFile, sourceFile);
    }
    else
    {
      CopyFile(destFile, sourceFile);
    }
  }

  // Create Core library source files
  Zilch::LibraryRef coreLibrary = Zilch::Core::GetInstance().GetLibrary();

  Zilch::NativeStubCode coreStubber;
  coreStubber.PrecompiledHeader = BuildString("\"", Name, "Precompiled.hpp\"");
  coreStubber.HppHeader = "// MIT Licensed (see LICENSE.md).";
  coreStubber.CppHeader = "// MIT Licensed (see LICENSE.md).";

  coreStubber.Generate(coreLibrary);

  String coreName = coreLibrary->Name;
  String coreHppFileName = FilePath::CombineWithExtension(destVersionDir, coreName, ".hpp");
  String coreCppFileName = FilePath::CombineWithExtension(destVersionDir, coreName, ".cpp");

  CopyGeneratedSource(coreHppFileName, coreStubber.Hpp);
  CopyGeneratedSource(coreCppFileName, coreStubber.Cpp);

  // Create ZeroEngine library source files
  Zilch::NativeStubCode zeroStubber;
  zeroStubber.PrecompiledHeader = BuildString("\"", Name, "Precompiled.hpp\"");
  zeroStubber.HppHeader = "// MIT Licensed (see LICENSE.md).";
  zeroStubber.CppHeader = "// MIT Licensed (see LICENSE.md).";

  Zilch::BoundType* cogType = EngineLibrary::GetLibrary()->BoundTypes["Cog"];
  zeroStubber.CustomClassHeaderDefines[cogType] = "ZeroCogGetComponentTemplate;";

  zeroStubber.HppMiddle = "#define ZeroCogGetComponentTemplate                                     "
                          " \\\n"
                          "  template <typename ComponentType>                                     "
                          " \\\n"
                          "  ComponentType* GetComponent()                                         "
                          " \\\n"
                          "  {                                                                     "
                          " \\\n"
                          "    Zilch::String name = ZilchTypeId(ComponentType)->Name;              "
                          " \\\n"
                          "    return static_cast<ComponentType*>                                  "
                          " \\\n"
                          "      (this->GetComponentByName(name).Dereference());                   "
                          " \\\n"
                          "  }                                                                     "
                          "   \n"
                          "#define has(ComponentType) GetComponent<ComponentType>()                "
                          "   \n";

  zeroStubber.HppFooter = "namespace ZeroEngine                                                    "
                          "   \n"
                          "{                                                                       "
                          "   \n"
                          "  template <typename ClassType>                                         "
                          "   \n"
                          "  void Connect                                                          "
                          "   \n"
                          "  (                                                                     "
                          "   \n"
                          "    ZeroEngine::Object* sender,                                         "
                          "   \n"
                          "    const Zilch::String& eventName,                                     "
                          "   \n"
                          "    const Zilch::String& functionName,                                  "
                          "   \n"
                          "    ClassType* receiver                                                 "
                          "   \n"
                          "  )                                                                     "
                          "   \n"
                          "  {                                                                     "
                          "   \n"
                          "    Zilch::BoundType* type = ZilchTypeId(ClassType);                    "
                          "   \n"
                          "    Zilch::FunctionArray* instanceFunctions =                           "
                          "   \n"
                          "      type->InstanceFunctions.FindPointer(functionName);                "
                          "   \n"
                          "    ReturnIf(instanceFunctions == nullptr || "
                          "instanceFunctions->Empty(),,  \n"
                          "      \"In %s making an event connection to %s we could \"              "
                          "   \n"
                          "      \"not find a function by the name of %s\",                        "
                          "   \n"
                          "      type->Name.c_str(), eventName.c_str(), functionName.c_str());     "
                          "   \n"
                          "                                                                        "
                          "   \n"
                          "    Zilch::Delegate delegate;                                           "
                          "   \n"
                          "    delegate.ThisHandle = Zilch::Handle((byte*)receiver, type);         "
                          "   \n"
                          "    delegate.BoundFunction = (*instanceFunctions)[0];                   "
                          "   \n"
                          "    ZeroEngine::Zero::Connect(sender, eventName, delegate);             "
                          "   \n"
                          "  }                                                                     "
                          "   \n"
                          "}                                                                       "
                          "   \n"
                          "#define ZeroConnectThisTo(Sender, EventName, FunctionName)              "
                          " \\\n"
                          "  ::ZeroEngine::Connect(Sender, EventName, FunctionName, this);         "
                          "   \n";

  // Emit all the attributes we have in Zero as string constants
  ZilchScriptManager* manager = ZilchScriptManager::GetInstance();
  Zilch::StringBuilderExtended hppAttributes;
  hppAttributes.WriteLine("namespace ZeroEngine");
  hppAttributes.WriteLine("{");
  Zilch::StringBuilderExtended cppAttributes;
  cppAttributes.WriteLine("namespace ZeroEngine");
  cppAttributes.WriteLine("{");

  HashSet<String> attributes;
  AttributeExtensions* attributeExtensions = AttributeExtensions::GetInstance();
  attributes.Append(attributeExtensions->mClassExtensions.Keys());
  attributes.Append(attributeExtensions->mPropertyExtensions.Keys());
  attributes.Append(attributeExtensions->mFunctionExtensions.Keys());

  forRange (String& attribute, attributes.All())
  {
    hppAttributes.Write("  extern const Zilch::String ");
    hppAttributes.Write(attribute);
    hppAttributes.Write("Attribute;");
    hppAttributes.WriteLine();

    cppAttributes.Write("  const Zilch::String ");
    cppAttributes.Write(attribute);
    cppAttributes.Write("Attribute(\"");
    cppAttributes.Write(attribute);
    cppAttributes.Write("\");");
    cppAttributes.WriteLine();
  }

  hppAttributes.WriteLine("}");
  cppAttributes.WriteLine("}");
  zeroStubber.HppFooter = BuildString(zeroStubber.HppFooter, hppAttributes.ToString());
  zeroStubber.CppFooter = BuildString(zeroStubber.CppFooter, cppAttributes.ToString());

  // We need to make the Zero engine header, but we want to
  // remove the Zilch Core library from the array on meta database
  Array<LibraryRef> nativeLibrariesWithoutCore = MetaDatabase::GetInstance()->mNativeLibraries;
  nativeLibrariesWithoutCore.EraseValueError(coreLibrary);

  String zeroNamespace = zeroStubber.Generate(nativeLibrariesWithoutCore);

  String zeroHppFileName = FilePath::CombineWithExtension(destVersionDir, zeroNamespace, ".hpp");
  String zeroCppFileName = FilePath::CombineWithExtension(destVersionDir, zeroNamespace, ".cpp");

  CopyGeneratedSource(zeroHppFileName, zeroStubber.Hpp);
  CopyGeneratedSource(zeroCppFileName, zeroStubber.Cpp);
}

void ZilchPluginSource::CopyPluginDependencies()
{
  WriteCurrentVersionFile();
  ForceCopyPluginDependencies();
}

void ZilchPluginSource::CopyPluginDependenciesOnce()
{
  // Only do this code when we have content loaded
  if (mContentItem == nullptr)
    return;

  // We only bother to copy plugin dependencies if plugins exist
  if (DirectoryExists(GetCodeDirectory()) == false)
    return;

  // We always write the current version, regardless of if we've already copied
  // the dependencies
  WriteCurrentVersionFile();

  // If we already have a directory for the current version, just skip this step
  if (DirectoryExists(GetCurrentVersionDirectory()))
    return;

  ForceCopyPluginDependencies();
}

void ZilchPluginSource::WriteCurrentVersionFile()
{
  // Only do this code when we have content loaded
  if (mContentItem == nullptr)
    return;

#if defined(WelderTargetOsWindows)
  String revisionNumber = GetRevisionNumberString();
  String propsFile = BuildString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
                                 "<Project ToolsVersion=\"4.0\" "
                                 "xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n"
                                 "  <PropertyGroup>\r\n"
                                 "    <ZeroVersion>",
                                 revisionNumber,
                                 "</ZeroVersion>\r\n"
                                 "  </PropertyGroup>\r\n"
                                 "</Project>\r\n");

  String codeDir = GetCodeDirectory();
  CreateDirectoryAndParents(codeDir);
  String versionFile = FilePath::Combine(codeDir, BuildString(Name, "Version.props"));
  WriteStringRangeToFile(versionFile, propsFile);
#endif
}

void ZilchPluginSource::OpenDirectory()
{
  CopyPluginDependenciesOnce();

  // Now open the folder up that we just created for the plugin
  Os::ShellOpenDirectory(GetCodeDirectory());
}

bool ZilchPluginSource::IsIdeInstalled()
{
  return Os::GetEnvironmentalVariable("VS140COMNTOOLS").SizeInBytes() != 0;
}

void ZilchPluginSource::OpenIde()
{
  if (mOpenIdeAfterToolsInstallCounter > 0)
    return;

  CopyPluginDependenciesOnce();
  String codeDir = GetCodeDirectory();

#if defined(WelderTargetOsWindows)

  if (CheckIdeAndInformUser() == false)
    return;

  String ideFile = FilePath::CombineWithExtension(codeDir, Name, ".bat");
  Os::ShellOpenFile(status, ideFile);
  DoNotifyStatus(status);

#else
  DoNotifyErrorNoAssert("Zilch Plugin", "No IDE was detected or supported on this platform");
#endif
}

void ZilchPluginSource::OnEngineUpdate(UpdateEvent* event)
{
#if defined(WelderTargetOsWindows)
  if (mOpenIdeAfterToolsInstallCounter > 0)
  {
    // If the installer process is finally stopped
    static const String VSIXInstaller("VSIXInstaller.exe");
    if (FindProcess(VSIXInstaller).mProcessId == 0)
    {
      ++mOpenIdeAfterToolsInstallCounter;

      if (mOpenIdeAfterToolsInstallCounter > InstallCounter)
      {
        mOpenIdeAfterToolsInstallCounter = 0;
        OpenIde();
      }
    }
  }
#endif
}

void ZilchPluginSource::InstallIdeTools()
{
  MarkAttemptedIdeToolsInstAll();

#if defined(WelderTargetOsWindows)
  String extensionPath = FilePath::Combine(Z::gContentSystem->ToolPath, "ZeroZilchPlugins.vsix");
  Os::ShellOpenFile(extensionPath);
#else
  DoNotifyErrorNoAssert("Zilch Plugin", "No IDE Plugins were detected or supported on this platform");
#endif
}

ZilchPluginConfig* ZilchPluginSource::GetConfig()
{
  ZilchPluginConfig* config = HasOrAdd<ZilchPluginConfig>(Z::gEngine->GetConfigCog());
  return config;
}

void ZilchPluginSource::MarkAttemptedIdeToolsInstAll()
{
  // This resource is NOT actually modified, but I want to signal to the engine
  // to save the config (refactor me)
  GetConfig()->mAttemptedIdeToolsInstall = true;
  ResourceModified();
}

void ZilchPluginSource::CompileDebug()
{
  CompileConfiguration("Debug");
}

void ZilchPluginSource::CompileRelease()
{
  CompileConfiguration("Release");
}

bool ZilchPluginSource::CheckIdeAndInformUser()
{
  if (GetResourceTemplate())
    return false;

  if (IsIdeInstalled() == false)
  {
    DoNotifyWarning("Zilch Plugin",
                    "No IDE was detected, you must first install a C++ IDE for "
                    "your platform");

#if defined(WelderTargetOsWindows)
    Os::OpenUrl("https://www.visualstudio.com/"
                "post-download-vs?sku=community&clcid=0x409");
#endif
    return false;
  }
  return true;
}

void ZilchPluginSource::Clean()
{
#if defined(WelderTargetOsWindows)
  String codeDir = GetCodeDirectory();
  String ideFile = FilePath::Combine(codeDir, BuildString(Name, "Clean.bat"));
  Os::ShellOpenFile(ideFile);
#endif
}

void ZilchPluginSource::CompileConfiguration(StringParam configuration)
{
  // Don't allow compilation more than once at a time
  if (mCompileTask != nullptr && mCompileTask->IsCompleted() == false)
    return;

  if (CheckIdeAndInformUser() == false)
    return;

  CopyPluginDependenciesOnce();
  String codeDir = GetCodeDirectory();
  String taskName = BuildString("Compiling ", configuration, " ", Name);

#if defined(WelderTargetOsWindows)
  String configurationBatchFileName = BuildString(Name, "Build", configuration);
  String configurationBatchFilePath = FilePath::CombineWithExtension(codeDir, configurationBatchFileName, ".bat");
  String process = BuildString("cmd /C \"", configurationBatchFilePath, "\"");

  ExecuteProcessTaskJob* job = new ExecuteProcessTaskJob(process);
  mCompileTask = Z::gBackgroundTasks->Execute(job, taskName);
  mCompileTask->mActivateOnCompleted = true;
  // Listen for the task completion events
  ConnectThisTo(mCompileTask, Events::BackgroundTaskCompleted, OnCompilationCompleted);
  ConnectThisTo(mCompileTask, Events::BackgroundTaskFailed, OnCompilationCompleted);

  // We can't get progress, so we'll have to estimate the time to complete
  mCompileTask->mIndeterminate = true;
  mCompileTask->mEstimatedTotalDuration = 15.0f;

  // Other parts of the engine may want to know when a plugin is currently
  // compiling We decrement this above in the 'CompletedCompilation' callback
  ZilchPluginSourceManager* manager = ZilchPluginSourceManager::GetInstance();
  ++manager->mCompilingPluginCount;
#else
  DoNotifyErrorNoAssert("Zilch Plugin", "Cannot automatically compile the plugin for this platform");
#endif
}

void ZilchPluginSource::OnCompilationCompleted(BackgroundTaskEvent* e)
{
  ZilchPluginSourceManager* manager = ZilchPluginSourceManager::GetInstance();
  --manager->mCompilingPluginCount;

  ExecuteProcessTaskJob* job = (ExecuteProcessTaskJob*)e->mTask->GetFinishedJob();
  // Check for failure
  if (e->State == BackgroundTaskState::Failed || job->mExitCode != 0)
  {
    String msg = String::Format("Plugin '%s' failed to compile", Name.c_str());
    DoNotifyError("Plugin Compilation Failed", msg);
  }
}

HandleOf<Resource> ZilchPluginSourceLoader::LoadFromFile(ResourceEntry& entry)
{
  ZilchPluginSource* plugin = new ZilchPluginSource();
  ZilchPluginSourceManager::GetInstance()->AddResource(entry, plugin);
  return plugin;
}

void ZilchPluginSourceLoader::ReloadFromFile(Resource* resource, ResourceEntry& entry)
{
}

ImplementResourceManager(ZilchPluginSourceManager, ZilchPluginSource);

ZilchPluginSourceManager::ZilchPluginSourceManager(BoundType* resourceType) : ResourceManager(resourceType)
{
  mCategory = "Code";
  mCanDuplicate = false;
  mCanReload = true;
  mCanAddFile = false;
  mPreview = false;
  mSearchable = true;
  mExtension = DataResourceExtension;
  mNoFallbackNeeded = true;
  mCanCreateNew = true;
  mCompilingPluginCount = 0;

  AddLoader("ZilchPluginSource", new ZilchPluginSourceLoader());

  ConnectThisTo(this, Events::ResourceAdded, OnResourceEvent);
  ConnectThisTo(this, Events::ResourceRemoved, OnResourceEvent);
  ConnectThisTo(this, Events::ResourceModified, OnResourceEvent);
}

ZilchPluginSourceManager::~ZilchPluginSourceManager()
{
}

void ZilchPluginSourceManager::ValidateNewName(Status& status, StringParam name, BoundType* optionalType)
{
  ZilchDocumentResource::ValidateNewScriptName(status, name);
}

void ZilchPluginSourceManager::ValidateRawName(Status& status, StringParam name, BoundType* optionalType)
{
  ZilchDocumentResource::ValidateRawScriptName(status, name);
}

bool ZilchPluginSourceManager::IsCompilingPlugins()
{
  return mCompilingPluginCount > 0;
}

void ZilchPluginSourceManager::OnResourceEvent(ResourceEvent* event)
{
  ZilchPluginSource* resource = Type::DebugOnlyDynamicCast<ZilchPluginSource*>(event->EventResource);

  // If the resource was added (may be at load time, or may be the first time of
  // adding in the editor) Only do this if its in the editor with a content item
  if (event->EventId == Events::ResourceAdded && resource->mContentItem != nullptr)
  {
    resource->EditorInitialize();

    // The shared library that we build (dll/so) should be the same name as our
    // source
    String extension = ZilchPluginBuilder::GetSharedLibraryExtension(true);
    String sharedLibraryPath =
        FilePath::CombineWithExtension(resource->mContentItem->mLibrary->SourcePath, resource->Name, extension);

    // If the shared library already exists, just make sure the plugin
    // dependencies are up to date
    if (FileExists(sharedLibraryPath) && GetFileSize(sharedLibraryPath) != 0)
      resource->CopyPluginDependenciesOnce();
    // Otherwise, attempt to build it once in release so we can get a working
    // plugin immediately
    else
      resource->CompileRelease();
  }

  if (ZilchPluginLibrary* libraryResource = resource->GetLibrary())
  {
    ResourceLibrary* library = resource->mResourceLibrary;
    library->mSwapPlugins[libraryResource].mCompileStatus = ZilchCompileStatus::Modified;
    library->PluginsModified();
  }
}

ZilchDefineType(ZilchPluginLibrary, builder, type)
{
}

String ZilchPluginLibrary::GetSharedLibraryPath() const
{
  // If for some reason the 'SharedLibraryPath' was not set on loading, then try
  // and use the resource library location
  if (SharedLibraryPath.Empty())
    return FilePath::CombineWithExtension(
        mResourceLibrary->Location, Name, ZilchPluginBuilder::GetSharedLibraryExtension(true));

  return SharedLibraryPath;
}

Resource* ZilchPluginLibrary::GetOriginResource() const
{
  Resource* zilchPluginOrigin = GetSource();
  if (zilchPluginOrigin == nullptr)
    zilchPluginOrigin = const_cast<ZilchPluginLibrary*>(this);
  return zilchPluginOrigin;
}

ZilchPluginSource* ZilchPluginLibrary::GetSource() const
{
  return ZilchPluginSourceManager::FindOrNull(Name);
}

ZilchPluginLibrary::ZilchPluginLibrary()
{
}

ZilchPluginLibrary::~ZilchPluginLibrary()
{
}

HandleOf<Resource> ZilchPluginLibraryLoader::LoadFromFile(ResourceEntry& entry)
{
  ZilchPluginLibrary* plugin = new ZilchPluginLibrary();
  plugin->SharedLibraryPath = entry.FullPath;
  ZilchPluginLibraryManager::GetInstance()->AddResource(entry, plugin);
  return plugin;
}

ImplementResourceManager(ZilchPluginLibraryManager, ZilchPluginLibrary);

ZilchPluginLibraryManager::ZilchPluginLibraryManager(BoundType* resourceType) : ResourceManager(resourceType)
{
  mCanDuplicate = false;
  mCanReload = true;
  mCanAddFile = false;
  mPreview = false;
  mSearchable = true;
  mExtension = ZilchPluginBuilder::GetSharedLibraryExtension(false);
  mNoFallbackNeeded = true;
  mCanCreateNew = false;

  AddLoader("ZilchPluginLibrary", new ZilchPluginLibraryLoader());

  ConnectThisTo(this, Events::ResourceAdded, OnResourceEvent);
  ConnectThisTo(this, Events::ResourceRemoved, OnResourceEvent);
  ConnectThisTo(this, Events::ResourceModified, OnResourceEvent);
}

ZilchPluginLibraryManager::~ZilchPluginLibraryManager()
{
}

void ZilchPluginLibraryManager::OnResourceEvent(ResourceEvent* event)
{
  ZilchPluginLibrary* libraryResource = Type::DebugOnlyDynamicCast<ZilchPluginLibrary*>(event->EventResource);
  ResourceLibrary* library = libraryResource->mResourceLibrary;

  if (event->RemoveMode != RemoveMode::None)
  {
    library->mSwapPlugins.Erase(libraryResource);
  }
  else
  {
    library->mSwapPlugins[libraryResource].mCompileStatus = ZilchCompileStatus::Modified;
  }
  library->PluginsModified();
}

} // namespace Zero
