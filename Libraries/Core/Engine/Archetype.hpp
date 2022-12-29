// MIT Licensed (see LICENSE.md).
#pragma once
#include "ObjectLoader.hpp"

namespace Zero
{

namespace Memory
{
class Heap;
}

class CogCreationContext;
class ObjectState;

// Archetype
/// An archetype is a resource containing the serialized data definition of an
/// object. The archetype stores a binary cache of the serialization data and
/// the source file for debugging and for archetype updating.
class Archetype : public Resource
{
public:
  ZilchDeclareType(Archetype, TypeCopyMode::ReferenceType);

  /// Constructor / Destructor.
  Archetype();
  ~Archetype();

  /// Resource Interface
  void Save(StringParam filename) override;
  void UpdateContentItem(ContentItem* contentItem) override;

  /// Cache this Archetype to binary. The binary cache will be used when
  /// creating an object from the Archetype resource.
  void BinaryCache(Cog* cog, CogCreationContext* context = nullptr);

  /// Cache the Archetype to a data tree. This cache
  void CacheDataTree();
  void ClearDataTreeCache();

  DataNode* GetCachedDataTree();
  CachedModifications& GetLocalCachedModifications();
  CachedModifications& GetAllCachedModifications();

  /// Remove the binary cache. Used when binary archetypes
  /// have been invalidated.
  void ClearBinaryCache();

  DataNode* GetDataTree() override;
  String GetStringData();

  /// Attempt to get a base class if we have one.
  Archetype* GetBaseArchetype();

  /// Name of the file from which this archetype was created.
  String mLoadPath;
  /// An Archetype can be a Cog, Space, or GameSession. It's okay for this to be
  /// a raw BoundType* because native types will never be destructed.
  BoundType* mStoredType;

  /// Heap for tracking and storing archetype data.
  static Memory::Heap* CacheHeap;

  /// Cached Binary Archetype Data
  DataBlock mBinaryCache;

  /// Used by the editor when uploading.
  CogId mCachedObject;

  /// When re-initializing Cogs due to script changes, we don't want Component
  /// removals to cause changes to Archetype's with Cogs that are in
  /// ArchetypeDefinition mode. This will be set before re-initializing Cogs,
  /// and should stop the ArchetypeDefinition mode from doing extra logic. There
  /// is possibly a better solution for this, but it was causing data loss and
  /// needed to be fixed quickly.
  static bool sRebuilding;

private:
  DataNode* mCachedTree;
  CachedModifications mLocalCachedModifications;
  CachedModifications mAllCachedModifications;
};

// Archetype Manager
/// Resource Manager for Archetypes.
class ArchetypeManager : public ResourceManager
{
public:
  DeclareResourceManager(ArchetypeManager, Archetype);

  /// Constructor / Destructor.
  ArchetypeManager(BoundType* resourceType);
  ~ArchetypeManager();

  /// Create a new archetype
  Archetype*
  MakeNewArchetypeWith(Cog* cog, StringParam newName, ResourceId id = 0, Archetype* inheritedArchetype = nullptr);
  bool SaveToContent(Cog* object, Archetype* archetype, ResourceId id = 0);
  void FlushBinaryArchetypes();
  void ArchetypeModified(Archetype* archetype);
};

} // namespace Zero
