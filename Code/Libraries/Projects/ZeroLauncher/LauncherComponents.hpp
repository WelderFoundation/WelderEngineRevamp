// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

/// Information about a build sent by the server. Contains all of the
/// necessary information to display, filter, and download builds.
class ZeroBuildContent : public Component
{
public:
  ZilchDeclareType(ZeroBuildContent, TypeCopyMode::ReferenceType);

  void Serialize(Serializer& stream) override;
  void Initialize(CogInitializer& initializer) override;

  /// Clears and re-parses the mTagSet from mTags.
  void Parse();

  /// Information used to uniquely identify this build.
  BuildId GetBuildId();
  void SetBuildId(const BuildId& buildId);

  /// Adds a tag if it doesn't already exist
  void AddTag(StringParam tag);
  bool ContainsTag(StringParam tag);
  String GetTagString();

  BuildId mBuildId;
  // A display name for the build independent of the actual build name
  String mDisplayName;
  String mPackageName;
  String mPackageExtension;
  String mTags;
  String mDownloadUrl;
  HashSet<String> mTagSet;
};

/// The release notes for a build.
class ZeroBuildReleaseNotes : public Component
{
public:
  ZilchDeclareType(ZeroBuildReleaseNotes, TypeCopyMode::ReferenceType);

  void Serialize(Serializer& stream) override;

  String mNotes;
};

/// Information about a build being deprecated. This can be both from the server
/// and locally set.
class ZeroBuildDeprecated : public Component
{
public:
  ZilchDeclareType(ZeroBuildDeprecated, TypeCopyMode::ReferenceType);

  void Serialize(Serializer& stream) override;

  String GetDeprecatedString();

  String mServerMessage;
  String mUserMessage;
};

/// Information about a build being deprecated. This can be both from the server
/// and locally set.
class ZeroTemplate : public Component
{
public:
  ZilchDeclareType(ZeroTemplate, TypeCopyMode::ReferenceType);

  void Serialize(Serializer& stream) override;
  void Initialize(CogInitializer& initializer) override;

  /// Clears and re-parses the mTagSet from mTags and the mBuildIds.
  void Parse();

  /// Get a string used to uniquely identify this template project. Built from
  /// the SKU and version id.
  String GetIdString();
  /// The full name of this template build from the SKU and version ranges.
  /// Used to get a path to the template that is nicely formatted.
  String GetFullTemplateVersionName();
  /// Parse mVersionId into valid build ranges so that TestBuildId can work
  bool ParseVersionId();
  /// Test if this project can run with the given build id
  bool TestBuildId(const BuildId& buildId);
  /// Returns if 'this' template contains a more exact matching build range than
  /// the 'other' template.
  bool IsMoreExactRangeThan(const BuildId& buildId, ZeroTemplate* otherTemplate);

  /// The unique name of this template (versions aside)
  String mSKU;
  /// Represents a list of ranges of build ids that this project can run in. The
  /// format allows comma separated build ids that can be ranges. Examples
  /// include "1.0.0,1.0-1.3,Branch.1" etc... Currently no validation on id
  /// ranges is performed (1.0-2.0 isn't valid).
  String mVersionId;
  /// The name to use when displaying this project in the launcher.
  String mDisplayName;
  /// Who authored this project
  String mAuthor;
  /// A small description of this template project
  String mDescription;
  /// The date the project was authored
  String mDate;
  /// A sorting bias. Projects are this priority then alphabetically by name.
  float mSortPriority;

  /// The name of the template file relative to the template path.
  String mDownloadUrl;
  /// The url of the icon to display for this template
  String mIconUrl;

  /// A comma delimited of tags for this project.
  String mTags;
  /// The tags split into an easier to query format
  HashSet<String> mTagSet;
  /// Is this template for a specific version of the engine?
  BuildId mSpecificVersion;

private:
  /// Build ids for a project are quite complicated/gross and so I want them
  /// locked up behind private.

  /// A project can have several ranges of allowed build ranges. A build range
  /// could be two values separated by a '-' or it could just be one build
  /// number. If there's a range then we have a min/max value pair to use. If
  /// there isn't a max value then all versions above up to a
  /// major-version/branch change are allowed so we have to store whether or not
  /// to check the upper limit.
  struct BuildIdRange
  {
    BuildIdRange()
    {
      mHasMax = false;
    }
    BuildId mMin;
    BuildId mMax;
    /// Did we actually load a max value or is this an unbounded range up to
    /// Branch/Major changes?
    bool mHasMax;
  };
  typedef Array<BuildIdRange> BuildIdList;

  /// From a string parse a list of builds. Splits on each ',' and then parses
  /// as a range.
  static bool ParseVersionId(StringParam versionId, BuildIdList& buildIds);
  /// Parses a version id range. This string is expected to have already been
  /// split by ','.
  static bool ParseVersionId(StringParam versionId, BuildIdRange& buildIdRange);
  /// Parses a version id string. This should properly handle loading partial
  /// version strings. A valid version string can optionally have a branch id
  /// that must contain a non-numeric value. If there is no branch then a major
  /// version id is required. Afterward, ids are optional in the order of minor,
  /// patch, and revision id. All id strings must strictly by integer values.
  static bool ParseVersionId(StringParam versionId, BuildId& buildId);

  /// Returns the first build id range that matches the given build. Returns
  /// null if one doesn't exist.
  BuildIdRange* FindBestBuildRange(const BuildId& buildId);

  /// Tests if the given build id is "contained" with the id range. If the id
  /// range doesn't have a max value then it is assumed to be unbounded up to
  /// branch/major version changes.
  bool TestBuildId(const BuildId& buildId, BuildIdRange& buildIdRange);
  BuildIdList mBuildIds;
};

} // namespace Zero
