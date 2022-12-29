// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

// Info used during attach/detach to know who the
// original pair was being attached/detached.
struct AttachmentInfo
{
  Cog* Parent;
  Cog* Child;
};

// Events
namespace Events
{
DeclareEvent(Attached);
DeclareEvent(Detached);
DeclareEvent(ChildAttached);
DeclareEvent(ChildDetached);
DeclareEvent(ChildrenOrderChanged);
} // namespace Events

// Hierarchy Event
class HierarchyEvent : public Event
{
public:
  ZilchDeclareType(HierarchyEvent, TypeCopyMode::ReferenceType);

  Cog* Parent;
  Cog* Child;
};

// Hierarchy
/// Component that gives an game object the ability to have child objects and
/// Contains a list of children.
class Hierarchy : public Component
{
public:
  ZilchDeclareType(Hierarchy, TypeCopyMode::ReferenceType);

  Hierarchy();
  ~Hierarchy();

  // Component Interface
  void Serialize(Serializer& stream) override;
  void Initialize(CogInitializer& initializer) override;
  void OnAllObjectsCreated(CogInitializer& initializer) override;
  void OnDestroy(uint flags) override;
  void TransformUpdate(TransformUpdateInfo& info) override;
  void AttachTo(AttachmentInfo& info) override;
  void Detached(AttachmentInfo& info) override;

  // We do custom serialization for Hierarchy
  bool ShouldSerialize() override
  {
    return false;
  }

  HierarchyList Children;
  HierarchyList::range GetChildren();
  HierarchyList::reverse_range GetChildrenReversed();
  void DestroyChildren();
};

// Hierarchy Composition
class HierarchyComposition : public MetaComposition
{
public:
  ZilchDeclareType(HierarchyComposition, TypeCopyMode::ReferenceType);

  HierarchyComposition();

  /// MetaComposition Interface.
  uint GetComponentCount(HandleParam instance) override;
  Handle GetComponentAt(HandleParam instance, uint index) override;
  bool CanAddComponent(HandleParam owner, BoundType* typeToAdd, AddInfo* info);
};

class Transform;
void RelativeAttach(Transform* child, Transform* parent);

} // namespace Zero
