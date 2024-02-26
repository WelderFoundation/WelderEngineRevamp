// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

namespace Events
{
DefineEvent(ImportOptionsModified);
}

String SanitizeContentFilename(StringParam filename)
{
  //String sanitizedName = Cog::SanitizeName(FilePath::GetFileNameWithoutExtension(filename));
  String sanitizedName = CleanAssetName(FilePath::GetFileNameWithoutExtension(filename));
  return BuildString(sanitizedName, ".", FilePath::GetExtension(filename));
}

ZilchDefineType(ImageOptions, builder, type)
{
  // These options are referred to directly by pointer on the import options
  // (unsafe for script)
  type->HandleManager = ZilchManagerId(PointerManager);

  ZeroBindExpanded();
  ZilchBindFieldProperty(mImportImages)->AddAttribute(PropertyAttributes::cInvalidatesObject);
}

ImageOptions::ImageOptions(ImportOptions* owner) : mOwner(owner)
{
  mImportImages = ImageImport::Textures;
}

ZilchDefineType(GeometryOptions, builder, type)
{
  // These options are referred to directly by pointer on the import options
  // (unsafe for script)
  type->HandleManager = ZilchManagerId(PointerManager);

  ZeroBindExpanded();

  ZilchBindFieldProperty(mImportMeshes)->AddAttribute(PropertyAttributes::cInvalidatesObject);
  ZilchBindFieldProperty(mGenerateSmoothNormals)
      ->AddAttributeChainable(PropertyAttributes::cInvalidatesObject)
      ->ZeroFilterBool(mImportMeshes);
  ZilchBindFieldProperty(mSmoothingAngleDegreesThreshold)->Add(new ShowNormalGenerationOptionsFilter());
  ZilchBindFieldProperty(mGenerateTangentSpace)->ZeroFilterBool(mImportMeshes);
  ZilchBindFieldProperty(mInvertUvYAxis)->ZeroFilterBool(mImportMeshes);
  ZilchBindFieldProperty(mFlipWindingOrder)->ZeroFilterBool(mImportMeshes);
  ZilchBindFieldProperty(mPhysicsImport)->ZeroFilterBool(mImportMeshes);

  ZilchBindFieldProperty(mCollapsePivots);
  ZilchBindFieldProperty(mImportAnimations);
  ZilchBindFieldProperty(mCreateArchetype);
  ZilchBindFieldProperty(mImportTextures);

  ZilchBindFieldProperty(mOriginOffset);
  ZilchBindFieldProperty(mScaleConversion)->AddAttribute(PropertyAttributes::cInvalidatesObject);
  ZilchBindFieldProperty(mScaleFactor)
      ->ZeroFilterEquality(mScaleConversion, ScaleConversion::Enum, ScaleConversion::Custom);
  ZilchBindFieldProperty(mChangeBasis)->AddAttribute(PropertyAttributes::cInvalidatesObject);

  ZilchBindFieldProperty(mXBasisTo)->ZeroFilterBool(mChangeBasis);
  ZilchBindFieldProperty(mYBasisTo)->ZeroFilterBool(mChangeBasis);
  ZilchBindFieldProperty(mZBasisTo)->ZeroFilterBool(mChangeBasis);
}

GeometryOptions::GeometryOptions(ImportOptions* owner) :
    mOwner(owner),
    mImportMeshes(true),
    mCombineMeshes(false),
    mOriginOffset(0.f, 0.f, 0.f),
    mScaleConversion(ScaleConversion::Custom),
    mScaleFactor(1),
    mChangeBasis(false),
    mXBasisTo(BasisType::PositiveX),
    mYBasisTo(BasisType::PositiveY),
    mZBasisTo(BasisType::PositiveZ),
    mGenerateSmoothNormals(false),
    mSmoothingAngleDegreesThreshold(30.f),
    mGenerateTangentSpace(true),
    mInvertUvYAxis(false),
    mFlipWindingOrder(false),
    mPhysicsImport(PhysicsImport::NoMesh),
    mCollapsePivots(false),
    mImportAnimations(false),
    mCreateArchetype(true),
    mImportTextures(false)
{
}

// ShowNormalGenerationOptionsFilter
ZilchDefineType(ShowNormalGenerationOptionsFilter, builder, type)
{
}

bool ShowNormalGenerationOptionsFilter::Filter(Member* prop, HandleParam instance)
{
  GeometryOptions* options = instance.Get<GeometryOptions*>(GetOptions::AssertOnNull);
  return options->mImportMeshes && options->mGenerateSmoothNormals;
}

ZilchDefineType(AudioOptions, builder, type)
{
  // These options are referred to directly by pointer on the import options
  // (unsafe for script)
  type->HandleManager = ZilchManagerId(PointerManager);

  ZeroBindExpanded();
  ZilchBindFieldProperty(mGenerateCue);
  ZilchBindFieldProperty(mGroupCueName);
  ZilchBindFieldProperty(mStreamingMode);
}

AudioOptions::AudioOptions(ImportOptions* owner) : mOwner(owner)
{
  mGenerateCue = AudioCueImport::None;
  mGroupCueName = "NoName";
  mStreamingMode = AudioFileLoadType::Auto;
}

ZilchDefineType(ConflictOptions, builder, type)
{
  // These options are referred to directly by pointer on the import options
  // (unsafe for script)
  type->HandleManager = ZilchManagerId(PointerManager);

  ZeroBindExpanded();
  ZilchBindGetterSetterProperty(Action);
}

ConflictOptions::ConflictOptions(ImportOptions* owner) : mOwner(owner)
{
  mAction = ConflictAction::Replace;
}

void ConflictOptions::SetAction(ConflictAction::Enum action)
{
  mAction = action;
  EventDispatcher* dispatcher = mOwner->GetDispatcher();
  Event e;
  dispatcher->Dispatch(Events::ImportOptionsModified, &e);
}

ConflictAction::Enum ConflictOptions::GetAction()
{
  return mAction;
}

ZilchDefineType(ImportOptions, builder, type)
{
  // These options are referred to directly by pointer on the import options
  // (unsafe for script)
  type->HandleManager = ZilchManagerId(PointerManager);

  ZilchBindFieldProperty(mImageOptions);
  ZilchBindFieldProperty(mGeometryOptions);
  ZilchBindFieldProperty(mAudioOptions);
  ZilchBindFieldProperty(mConflictOptions);
  ZeroBindExpanded();
}

ImportOptions::ImportOptions()
{
  mImageOptions = nullptr;
  mGeometryOptions = nullptr;
  mAudioOptions = nullptr;
  mConflictOptions = nullptr;
  mLibrary = nullptr;
}

ImportOptions::~ImportOptions()
{
  SafeDelete(mImageOptions);
  SafeDelete(mGeometryOptions);
  SafeDelete(mAudioOptions);
  SafeDelete(mConflictOptions);
}

void ImportOptions::Initialize(Array<String>& files, ContentLibrary* library)
{
  mLibrary = library;
  Array<String> invalidFiles;

  for (uint i = 0; i < files.Size(); ++i)
  {
    String fullPath = files[i];

    // Strip the path
    String originalFilename = FilePath::GetFileName(fullPath.All());
    String fileName = SanitizeContentFilename(originalFilename);

    // Check to see if the filename contained any valid characters
    if (FilePath::GetFileNameWithoutExtension(fileName) == Zilch::EmptyUpperIdentifier &&
        !originalFilename.Contains(Zilch::EmptyUpperIdentifier))
    {
      invalidFiles.PushBack(originalFilename);
      continue;
    }

    // Check to see if it already exists
    if (mLibrary->FindContentItemByFileName(fileName))
      mConflictedFiles.PushBack(fullPath);
    else
      mFiles.PushBack(fullPath);
  }

  // If there are any invalid filenames create a do notify error message
  if (!invalidFiles.Empty())
  {
    StringBuilder builder;
    builder.Append(invalidFiles.Front());
    for (unsigned i = 1; i < invalidFiles.Size(); ++i)
      builder.AppendFormat(", %s", invalidFiles[i].c_str());

    String errorMessage =
        String::Format("The following files do not contain any valid characters: %s", builder.ToString().c_str());
    DoNotifyError("File Import Failed", errorMessage);
  }

  BuildOptions();
}

template <typename ClassType>
void BuildContentOptions(ClassType** object, ImportOptions* owner, HashSet<String>& set, StringParam ContentType)
{
  if (set.FindValue(ContentType, "None") != "None")
  {
    if (*object == nullptr)
      *object = new ClassType(owner);
  }
  else
  {
    SafeDelete(*object);
  }
}

void InsertExtension(Array<String>& files, HashSet<String>& set)
{
  for (uint i = 0; i < files.Size(); ++i)
  {
    // Get the file extension
    String file = FilePath::GetFileName(files[i].All());
    String extension = FilePath::GetExtension(file);
    extension = extension.ToLower();

    // Check known extensions
    ContentTypeEntry* entry = Z::gContentSystem->CreatorsByExtension.FindPointer(extension);
    if (entry)
      set.Insert(entry->Meta->Name);
  }
}

void ImportOptions::BuildOptions()
{
  // We want to find unique content types (Image, Geometry, Audio)
  HashSet<String> fileSet;

  InsertExtension(mFiles, fileSet);
  if (!mConflictOptions && mConflictedFiles.Size() > 0)
    mConflictOptions = new ConflictOptions(this);

  if (mConflictOptions)
    InsertExtension(mConflictedFiles, fileSet);

  // prepare to build the new files we are importing
  BuildContentOptions(&mImageOptions, this, fileSet, "ImageContent");
  BuildContentOptions(&mGeometryOptions, this, fileSet, "GeometryContent");
  BuildContentOptions(&mAudioOptions, this, fileSet, "AudioContent");
}

bool ImportOptions::ShouldAutoImport()
{
  // If any options are present, we cannot auto import
  return !(mImageOptions || mGeometryOptions || mAudioOptions || mConflictOptions);
}

} // namespace Zero
