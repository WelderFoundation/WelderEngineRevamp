// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace tb
{
// static
TBFile* tb::TBFile::Open(const char* filename, TBFileMode mode)
{
  if (mode != TBFileMode::MODE_READ)
    return nullptr;

  String resolvedFilename(filename);

  if (resolvedFilename.StartsWith("@TBUI"))
  {
    // Look up the data folder in the configuration file
    Zero::Cog* configCog = Zero::Z::gEngine->GetConfigCog();
    Zero::MainConfig* mainConfig = configCog->has(Zero::MainConfig);
    String directory = mainConfig->DataDirectory;

    resolvedFilename = resolvedFilename.Replace("@TBUI", Zero::FilePath::Combine(directory, "TBUI"));
  }

  Zero::TBUIFile* file = new ::Zero::TBUIFile(resolvedFilename);
  if (!file->IsOpen())
  {
    file = nullptr;
  }

  return file;
}
} // namespace tb

namespace Zero
{
ZilchDefineType(TBUIFile, builder, type)
{
  ZeroBindDocumented();
}
TBUIFile::TBUIFile(StringParam filepath) : mFile()
{
  mFile.Open(filepath, FileMode::Enum::Read, FileAccessPattern::Enum::Random);
}
TBUIFile::~TBUIFile()
{
  if (mFile.IsOpen())
    mFile.Close();
}
long TBUIFile::Size()
{
  return mFile.Size();
}
size_t TBUIFile::Read(void* buf, size_t elemSize, size_t count)
{
  Status status;
  return mFile.Read(status, (::byte*)buf, count);
}
} // namespace Zero