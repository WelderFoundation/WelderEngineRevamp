// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

/// Forward declarations
class ObjectRestoreState;

/// Changes the property to the given value and queues the operation.
void ChangeAndQueueProperty(OperationQueue* queue, HandleParam object, PropertyPathParam property, AnyParam newValue);

/// Removes the component and queues the operation.
bool QueueRemoveComponent(OperationQueue* queue,
                          HandleParam object,
                          BoundType* componentMeta,
                          bool ignoreDependencies = false);
/// Queues a component remove operation with the given serialized component.
void QueueRemoveComponent(OperationQueue* queue,
                          HandleParam object,
                          BoundType* componentMeta,
                          StringParam componentData,
                          uint componentIndex);

/// Adds the component of the given name and queues the operation.
bool QueueAddComponent(OperationQueue* queue, HandleParam object, BoundType* componentType);

/// Queues the already added component.
void QueueAddComponent(OperationQueue* queue, HandleParam object, HandleParam component);

/// Moves the component to before the given index.
void QueueMoveComponent(OperationQueue* queue, HandleParam object, HandleParam component, uint index);

void MarkPropertyAsModified(OperationQueue* queue, HandleParam object, PropertyPathParam propertyPath);

void RevertProperty(OperationQueue* queue, HandleParam object, PropertyPathParam propertyPath);
void RestoreLocallyRemovedChild(OperationQueue* queue, HandleParam parent, ObjectState::ChildId& childId);
void RestoreChildOrder(OperationQueue* queue, HandleParam object);

class MetaOperation : public Operation
{
public:
  MetaOperation(HandleParam object);

  void Undo() override;
  void Redo() override;

  Handle GetUndoObject();

  UndoHandle mUndoHandle;
  Any mUndoClientData;
};

class PropertyOperation : public MetaOperation
{
public:
  ZilchDeclareType(PropertyOperation, TypeCopyMode::ReferenceType);

  PropertyOperation(HandleParam object, PropertyPathParam property, AnyParam before, AnyParam after);
  ~PropertyOperation();

  /// Operation Interface.
  void Undo() override;
  void Redo() override;

  /// Property operations are created before the value is set for side affect
  /// actions. Because of this, the 'after' value is not valid until after this
  /// operations construction. Before the operation is actually put in the
  /// operation queue, this will be called to query for the value after.
  void UpdateValueAfter();

private:
  /// When scripts change, the types in 'mValueBefore' and 'mValueAfter' could
  /// be deleted. We need to update them to either the new types, or null them
  /// out.
  void OnScriptsCompiled(ZilchCompileEvent* e);
  void OnMetaRemoved(MetaLibraryEvent* e);

  PropertyPath mPropertyPath;
  Any mValueBefore;
  Any mValueAfter;
  bool mPropertyWasModified;
};

DeclareEnum2(ComponentOperation, Add, Remove);

class AddRemoveComponentOperation : public MetaOperation
{
public:
  AddRemoveComponentOperation(HandleParam object,
                              BoundType* componentMeta,
                              ComponentOperation::Enum mode,
                              StringParam componentData = String(),
                              uint componentIndex = uint(-1));
  ~AddRemoveComponentOperation();

  /// Operation Interface.
  void Undo() override;
  void Redo() override;

  void AddComponentFromBuffer();
  void SaveComponentToBuffer();

  void ComponentAdded(HandleParam object);
  void ComponentRemoved(HandleParam object);

  // Temporary, for use in Material re-initialization. Should not be used
  // elsewhere.
  bool mNotifyModified;

private:
  /// The meta of the component we're adding/removing.
  BoundTypeHandle mComponentType;
  MetaComponentHandle<MetaComposition> mComposition;

  UndoHandle mComponentHandle;

  /// The index the component was at before it was removed so we can
  /// put it back in the same place.
  uint mComponentIndex;

  /// The component will be serialized to this buffer.
  String mSerializationBuffer;

  /// Whether or not this operation added or removed the component.
  ComponentOperation::Enum mMode;

  ObjectState* mRemovedObjectState;
};

class MoveComponentOperation : public MetaOperation
{
public:
  MoveComponentOperation(HandleParam object, HandleParam componentToMove, uint destinationIndex);
  ~MoveComponentOperation();

  /// Operation Interface.
  void Undo() override;
  void Redo() override;

private:
  void MoveComponent(uint from, uint to);

  /// Object data.
  MetaComponentHandle<MetaComposition> mComposition;

  uint mStartIndex, mEndIndex;

  /// Whether or not the component order was modified before this operation.
  /// This is used to update the LocalModifications for this object.
  bool mWasOrderLocallyModified;
};

class MarkPropertyModifiedOperation : public MetaOperation
{
public:
  /// Constructor.
  MarkPropertyModifiedOperation(HandleParam object, PropertyPathParam propertyPath);

  /// Operation Interface.
  void Undo() override;
  void Redo() override;

  void SetModifiedState(bool state);

  PropertyPath mPropertyPath;
};

class RevertPropertyOperation : public MetaOperation
{
public:
  /// Constructor.
  RevertPropertyOperation(HandleParam object, PropertyPathParam propertyPath);

  /// Operation Interface.
  void Undo() override;
  void Redo() override;

  Any mOldValue;
  PropertyPath mPropertyToRevert;
};

class RestoreChildOperation : public MetaOperation
{
public:
  /// Constructor.
  RestoreChildOperation(HandleParam parent, ObjectState::ChildId& childId);

  /// Operation Interface.
  void Undo() override;
  void Redo() override;

  MetaComponentHandle<MetaDataInheritance> mInheritance;
  ObjectState::ChildId mChildId;
};

class RestoreChildOrderOperation : public MetaOperation
{
public:
  /// Constructor.
  RestoreChildOrderOperation(HandleParam object);
  ~RestoreChildOrderOperation();

  /// Operation Interface.
  void Undo() override;
  void Redo() override;

  MetaComponentHandle<MetaOperations> mMetaOperations;
  MetaComponentHandle<MetaDataInheritance> mInheritance;
  ObjectRestoreState* mRestoreState;
};

} // namespace Zero
