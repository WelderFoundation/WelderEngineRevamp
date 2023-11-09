// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

ZilchDefineStaticLibrary(TBUILibrary)
{
  builder.CreatableInScriptDefault = false;

  ZilchInitializeType(TBUIWidget);
  ZilchInitializeType(TBUIRenderer);
  ZilchInitializeType(TBUIBitmap);
  ZilchInitializeType(TBUIFile);
  ZilchInitializeType(TBUIImageLoader);

  EngineLibraryExtensions::AddNativeExtensions(builder);
}

void TBUILibrary::Initialize()
{
  BuildStaticLibrary();
  MetaDatabase::GetInstance()->AddNativeLibrary(GetLibrary());

  TBUIManager::Initialize();
}

void TBUILibrary::Shutdown()
{
  TBUIManager::Destroy();

  GetLibrary()->ClearComponents();
}

} // namespace Zero