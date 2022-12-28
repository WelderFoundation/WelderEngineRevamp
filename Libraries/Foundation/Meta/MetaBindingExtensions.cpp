// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

const String cInvalidTypeName = "void";

// Object Attributes
namespace ObjectAttributes
{

const String cHidden("Hidden");
const String cDocumented("Documented");
const String cDoNotDocument("DoNotDocument");
const String cExpanded("Expanded");
const String cCore("Core");
const String cStoreLocalModifications("StoreLocalModifications");
const String cProxy("Proxy");
const String cExceptionProxy("ExceptionProxy");
const String cRunInEditor("RunInEditor");
const String cResourceInterface("ResourceInterface");
const String cComponentInterface("ComponentInterface");
const String cGizmo("Gizmo");
const String cCommand("Command");
const String cTags("Tags");
const String cShortcut("Shortcut");
const String cTool("Tool");

} // namespace ObjectAttributes

// Property Attributes
namespace PropertyAttributes
{

const String cProperty("Property");
const String cInternal("Internal");
const String cInvalidatesObject("InvalidatesObject");
const String cShaderInput("ShaderInput");
const String cLocalModificationOverride("LocalModificationOverride");
const String cSerialize("Serialize");
const String cDeprecatedSerialized("Serialized");
const String cDependency("Dependency");
const String cDisplay("Display");
const String cDeprecatedEditable("Editable");
const String cResourceProperty("ResourceProperty");
const String cRuntimeClone("RuntimeClone");
const String cRenamedFrom("RenamedFrom");
const String cNetProperty("NetProperty");
const String cNetPeerId("NetPeerId");
const String cGroup("Group");
const String cRange("Range");
const String cSlider("Slider");
const String cOptional("Optional");

} // namespace PropertyAttributes

// Function Attributes
namespace FunctionAttributes
{

const String cProperty("Property");
const String cInternal("Internal");
const String cDisplay("Display");
const String cInvalidatesObject("InvalidatesObject");

} // namespace FunctionAttributes

// Serialization Attributes
namespace SerializationAttributes
{

const String cSerializationPrimitive("SerializationPrimitive");

} // namespace SerializationAttributes

// Common groups (used with PropertyAttributes::cGroup)
namespace CommonGroups
{
const String cAdvanced("Advanced");
} // namespace CommonGroups

// Events
namespace Events
{
DefineEvent(PropertyModified);
DefineEvent(PropertyModifiedIntermediate);
DefineEvent(ComponentsModified);
DefineEvent(ObjectModified);
} // namespace Events

void PropertyModifiedDefault(
    HandleParam object, PropertyPathParam property, AnyParam oldValue, AnyParam newValue, bool intermediateChange)
{
  if (Object* zeroObject = object.Get<Object*>())
  {
    // Get the correct event
    String eventId = Events::PropertyModified;
    if (intermediateChange)
      eventId = Events::PropertyModifiedIntermediate;

    // Send the event to the object
    PropertyEvent eventToSend(object, property, oldValue, newValue);
    if (EventDispatcher* dispatcher = zeroObject->GetDispatcher())
      dispatcher->Dispatch(eventId, &eventToSend);
  }
}

void ComponentsChangedDefault(HandleParam object)
{
  if (Object* zeroObject = object.Get<Object*>())
  {
    ObjectEvent e(zeroObject);
    if (EventDispatcher* dispatcher = zeroObject->GetDispatcher())
      dispatcher->Dispatch(Events::ComponentsModified, &e);
  }
}

void ObjectModifiedDefault(HandleParam object)
{
  if (Object* zeroObject = object.Get<Object*>())
  {
    ObjectEvent e(zeroObject);
    if (EventDispatcher* dispatcher = zeroObject->GetDispatcher())
      dispatcher->Dispatch(Events::ObjectModified, &e);
  }
}

// Meta Operations
void MetaOperations::NotifyPropertyModified(
    HandleParam object, PropertyPathParam property, AnyParam oldValue, AnyParam newValue, bool intermediateChange)
{
  Array<Handle> instances;

  // It's most likely that we have Cog/Component/Property, so reserve 3
  // We could impose a depth limit and avoid more heap allocations
  instances.Reserve(3);
  instances.PushBack(object);

  property.GetInstanceHierarchy(object, &instances);

  // The leaf will be at the end
  Handle& leafVariant = instances.Back();
  ReturnIf(leafVariant.IsNull(), , "Instances should always be valid if in undo/redo.");

  // Make sure the property path is always relative to the current object we're
  // sending an event on
  PropertyPath localPath = property;
  forRange (Handle& localObject, instances.All())
  {
    BoundType* objectType = localObject.StoredType;
    if (MetaOperations* metaOps = objectType->HasInherited<MetaOperations>())
      metaOps->PropertyModified(localObject, localPath, oldValue, newValue, intermediateChange);
    else
      PropertyModifiedDefault(localObject, localPath, oldValue, newValue, intermediateChange);

    // The object has been modified, so notify that as well
    NotifyObjectModified(localObject, intermediateChange);

    localPath.mPath.PopFront();
  }
}

void MetaOperations::NotifyComponentsModified(HandleParam object)
{
  BoundType* objectType = object.StoredType;
  if (MetaOperations* metaOps = objectType->HasInherited<MetaOperations>())
  {
    metaOps->ComponentsModified(object);
    metaOps->ObjectModified(object, false);
  }
  else
  {
    ComponentsChangedDefault(object);
    ObjectModifiedDefault(object);
  }
}

void MetaOperations::NotifyObjectModified(HandleParam object, bool intermediateChange)
{
  BoundType* objectType = object.StoredType;
  if (MetaOperations* metaOps = objectType->HasInherited<MetaOperations>())
    metaOps->ObjectModified(object, intermediateChange);
  else
    ObjectModifiedDefault(object);
}

u64 MetaOperations::GetUndoHandleId(HandleParam object)
{
  return (u64)-1;
}

Any MetaOperations::GetUndoData(HandleParam object)
{
  return Any();
}

void MetaOperations::PropertyModified(
    HandleParam object, PropertyPathParam property, AnyParam oldValue, AnyParam newValue, bool intermediateChange)
{
  PropertyModifiedDefault(object, property, oldValue, newValue, intermediateChange);
}

void MetaOperations::ComponentsModified(HandleParam object)
{
  ComponentsChangedDefault(object);
}

void MetaOperations::ObjectModified(HandleParam object, bool intermediateChange)
{
  ObjectModifiedDefault(object);
}

// Property Event
ZilchDefineType(PropertyEvent, builder, type)
{
  ZeroBindEvent(Events::PropertyModified, PropertyEvent);
  ZeroBindEvent(Events::PropertyModifiedIntermediate, PropertyEvent);
}

PropertyEvent::PropertyEvent(HandleParam object, PropertyPathParam property, AnyParam oldValue, AnyParam newValue) :
    mObject(object),
    mProperty(property),
    mOldValue(oldValue),
    mNewValue(newValue)
{
}

// Type Event
ZilchDefineType(TypeEvent, builder, type)
{
}

// Getter for bound events extensions e.g. Events.LogicUpdate
void GetEventNameProperty(Call& call, ExceptionReport& report)
{
  Function* currentFunction = call.GetFunction();
  String& eventName = currentFunction->ComplexUserData.ReadObject<String>(0);

  // We have no parameters, no this pointer,
  // so the only thing on the stack is the return value
  call.Set<String>(Call::Return, eventName);
}

void BindEventSent(LibraryBuilder& builder, BoundType* boundType, StringParam eventName, BoundType* eventType)
{
  ErrorIf(eventType == nullptr, "Event type must be provided");

  ErrorIf(!eventType->IsA(ZilchTypeId(Event)),
          "Attempting to bind '%s' as an event that isn't an Event type: "
          "BindBase(Event)",
          eventType->Name.c_str());

  builder.AddSendsEvent(boundType, eventName, eventType);
}

ZilchDefineType(MetaOperations, builder, type)
{
}

ZilchDefineType(MetaDisplay, builder, type)
{
}

ZilchDefineType(TypeNameDisplay, builder, type)
{
}

ZilchDefineType(StringNameDisplay, builder, type)
{
}

ZilchDefineType(MetaTransform, builder, type)
{
}

// TypeNameDisplay
String TypeNameDisplay::GetName(HandleParam object)
{
  return object.StoredType->Name;
}

String TypeNameDisplay::GetDebugText(HandleParam object)
{
  return GetName(object);
}

// StringNameDisplay
StringNameDisplay::StringNameDisplay(StringParam string) : mString(string)
{
}

String StringNameDisplay::GetName(HandleParam)
{
  return mString;
}

String StringNameDisplay::GetDebugText(HandleParam)
{
  return mString;
}

// Meta Transform
bool MetaTransformInstance::IsNull()
{
  return mInstance.IsNull();
}

bool MetaTransformInstance::IsNotNull()
{
  return !IsNull();
}

Vec3 MetaTransformInstance::GetLocalTranslation()
{
  if (mLocalTranslation)
    return mLocalTranslation->GetValue(mInstance).Get<Vec3>();
  return Vec3::cZero;
}

Quat MetaTransformInstance::GetLocalRotation()
{
  if (mLocalRotation)
    return mLocalRotation->GetValue(mInstance).Get<Quat>();
  return Quat::cIdentity;
}

Vec3 MetaTransformInstance::GetLocalScale()
{
  if (mLocalScale)
    return mLocalScale->GetValue(mInstance).Get<Vec3>();
  return Vec3::cZero;
}

void MetaTransformInstance::SetLocalTranslation(Vec3Param value)
{
  if (mLocalTranslation)
    mLocalTranslation->SetValue(mInstance, value);
}

void MetaTransformInstance::SetLocalRotation(QuatParam value)
{
  if (mLocalRotation)
    mLocalRotation->SetValue(mInstance, value);
}

void MetaTransformInstance::SetLocalScale(Vec3Param value)
{
  if (mLocalScale)
    mLocalScale->SetValue(mInstance, value);
}

Vec3 MetaTransformInstance::GetWorldTranslation()
{
  if (mWorldTranslation)
    return mWorldTranslation->GetValue(mInstance).Get<Vec3>();
  return Vec3::cZero;
}

Quat MetaTransformInstance::GetWorldRotation()
{
  if (mWorldRotation)
    return mWorldRotation->GetValue(mInstance).Get<Quat>();
  return Quat::cIdentity;
}

Vec3 MetaTransformInstance::GetWorldScale()
{
  if (mWorldScale)
    return mWorldScale->GetValue(mInstance).Get<Vec3>();
  return Vec3::cZero;
}

Mat4 MetaTransformInstance::GetParentWorldMatrix()
{
  if (mParentWorldMatrix)
    return mParentWorldMatrix->GetValue(mParentInstance).Get<Mat4>();
  return Mat4::cIdentity;
}

Mat4 MetaTransformInstance::GetParentLocalMatrix()
{
  if (mParentLocalMatrix)
    return mParentLocalMatrix->GetValue(mParentInstance).Get<Mat4>();
  return Mat4::cIdentity;
}

Vec3 MetaTransformInstance::ToParent(Vec3Param local)
{
  if (mParentLocalMatrix)
    return Math::TransformPoint(mParentLocalMatrix->GetValue(mParentInstance).Get<Mat4>(), local);
  return local;
}

// Meta Attribute
ZilchDefineType(MetaAttribute, builder, type)
{
}

} // namespace Zero
