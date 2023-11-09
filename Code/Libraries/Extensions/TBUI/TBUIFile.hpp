// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{
class TBUIFile : public tb::TBFile
{
public:
  /// Meta Initialization.
  ZilchDeclareType(TBUIFile, TypeCopyMode::ReferenceType);

  TBUIFile(StringParam filepath);

  ~TBUIFile();

  long Size() override;
  size_t Read(void* buf, size_t elemSize, size_t count) override;

  ZeroForceInline bool IsOpen()
  {
    return mFile.IsOpen();
  }

private:
  File mFile;
};
} // namespace Zero