// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

/// Represents a label for a Collider to be used with a CollisionTable.
class CollisionGroup : public DataResource
{
public:
  ZilchDeclareType(CollisionGroup, TypeCopyMode::ReferenceType);

  CollisionGroup();
  ~CollisionGroup();

  // Data Resource Interface
  void Serialize(Serializer& stream) override{};
  void Initialize(){};

  // Runtime interface

  /// Creates a CollisionGroup for run-time modifications.
  static HandleOf<CollisionGroup> CreateRuntime();
  HandleOf<Resource> Clone() override;
  /// Creates a clone of this CollisionGroup for run-time modifications.
  HandleOf<CollisionGroup> RuntimeClone();

  CollisionGroupInstance* GetNewInstance();
};

/// An instance of a CollisionGroup group for a specific space. Stores the bit
/// field for the filters on this space as well as the group id. Objects get
/// references to this in order to deal with different data per space.
struct CollisionGroupInstance
{
  CollisionGroupInstance();

  /// Checks to see if collision detection should be skipped with the given
  /// group type.
  bool SkipDetection(const CollisionGroupInstance& rhs) const;
  /// Checks to see if collision resolution should be skipped with the given
  /// group type.
  bool SkipResolution(const CollisionGroupInstance& rhs) const;
  /// The name of the CollisionGroup that this is an instance of.
  String GetGroupName() const;

  /// The internal mask used to filter collision detection among groups.
  /// Changed by the filter whenever new groups or filters are added.
  /// Also changed when a filter changes state.
  u32 mDetectionMask;
  /// The internal mask used to filter groups. Changed by the filter
  /// whenever new groups or filters are added.
  /// Also changed when a filter changes state.
  u32 mResolutionMask;
  /// A bit field that signifies the index of this group.
  u32 mGroupId;
  /// A reference to the group that this is an instance of.
  HandleOf<CollisionGroup> mResource;
  /// The filter that this instance belongs to. Used for event sending.
  CollisionTable* mTable;
};

class CollisionGroupManager : public ResourceManager
{
public:
  DeclareResourceManager(CollisionGroupManager, CollisionGroup);

  CollisionGroupManager(BoundType* resourceType);
};

} // namespace Zero
