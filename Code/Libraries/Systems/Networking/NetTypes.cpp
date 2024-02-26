// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

// Editor Tags
namespace Tags
{
DefineTag(Networking);
}

bool IsValidNetProperty(Property* property)
{
  // Is read-only?
  if (property->IsReadOnly())
  {
    // Unsupported property
    return false;
  }

  // Is supported type?
  return IsValidNetPropertyType(property->PropertyType);
}
bool IsValidNetPropertyType(Type* propertyType)
{
  //    Bitstream can serialize the underlying property type?
  // OR This a cog type?
  // OR This is a cog path type?
  // (We allow NetObject net properties to serialize these types, but not
  // NetEvents, for now)
  return BitStreamCanSerializeType(propertyType) || propertyType == ZilchTypeId(Cog) ||
         propertyType == ZilchTypeId(CogPath);
}
bool IsValidNetPeerIdPropertyType(Type* propertyType)
{
  return propertyType == ZilchTypeId(int);
}

//                                 Network Types //

ComponentPropertyInstanceData::ComponentPropertyInstanceData(String propertyName, Component* component) :
    mPropertyName(propertyName), mComponent(component)
{
}

//
// NetChannel Authority Property Getter / Setter
//

Variant GetNetChannelAuthorityProperty(const Variant& propertyData)
{
  // Get associated property instance data
  ReplicaChannel* replicaChannel = propertyData.GetOrError<ReplicaChannel*>();

  // Success
  // (We wrap the enum in an Any to take advantage of Zilch meta later during
  // serialization to serialize the enum quantized, using only the bits
  // necessary to represent all enum values)
  return Variant(Any(replicaChannel->GetAuthority()));
}
void SetNetChannelAuthorityProperty(const Variant& value, Variant& propertyData)
{
  // Get associated property instance data
  ReplicaChannel* replicaChannel = propertyData.GetOrError<ReplicaChannel*>();

  // Success
  replicaChannel->SetAuthority(value.GetOrError<Any>().Get<Authority::Enum>());
}

//
// NetObject Name Property Getter / Setter
//

Variant GetNetObjectNameProperty(const Variant& propertyData)
{
  // Get associated property instance data
  NetObject* netObject = propertyData.GetOrError<NetObject*>();
  Cog* owner = netObject->GetOwner();

  // Return the object's name
  return Variant(owner->GetName());
}
void SetNetObjectNameProperty(const Variant& value, Variant& propertyData)
{
  // Get associated property instance data
  NetObject* netObject = propertyData.GetOrError<NetObject*>();
  Cog* owner = netObject->GetOwner();

  // Set the object's name
  owner->SetName(value.GetOrError<String>());
}

//
// NetObject Parent Property Getter / Setter
//

Variant GetNetObjectParentProperty(const Variant& propertyData)
{
  // Get associated property instance data
  NetObject* netObject = propertyData.GetOrError<NetObject*>();
  Cog* owner = netObject->GetOwner();

  // Get parent
  Cog* parent = owner->GetParent();
  if (!parent) // Empty?
    return Variant(NetObjectId(0));

  // Get parent net object
  NetObject* parentNetObject = parent->has(NetObject);
  if (!parentNetObject) // Unable?
    return Variant(NetObjectId(0));

  // Success
  return Variant(parentNetObject->GetNetObjectId());
}
void SetNetObjectParentProperty(const Variant& value, Variant& propertyData)
{
  // Get associated property instance data
  NetObject* netObject = propertyData.GetOrError<NetObject*>();
  Cog* owner = netObject->GetOwner();
  Cog* currentParent = owner->GetParent();

  // Parent set?
  NetObjectId newParentNetObjectId = value.GetOrError<NetObjectId>();
  if (newParentNetObjectId)
  {
    // Has current parent?
    if (currentParent)
    {
      // The new parent is our current parent?
      NetObject* currentParentNetObject = currentParent->has(NetObject);
      if (currentParentNetObject && (currentParentNetObject->GetNetObjectId() == newParentNetObjectId))
      {
        // Done (This case needs to be explicitly checked because of internal
        // hierarchies!)
        return;
      }
    }

    // Get net peer
    NetPeer* netPeer = netObject->GetNetPeer();
    if (!netPeer)
    {
      Assert(false);
      return;
    }

    // Find new parent
    Cog* newParent = netPeer->GetNetObject(newParentNetObjectId);
    if (!newParent) // Unable?
    {
      // Get net space
      NetSpace* netSpace = netObject->GetNetSpace();

      // We are receiving a net game clone?
      if (netSpace && netPeer->IsReceivingNetGame())
      {
        Assert(netPeer->IsClient());

        // Add delayed attachment, will be performed when the parent exists
        // locally
        netSpace->AddDelayedAttachment(netObject->GetNetObjectId(), newParentNetObjectId);
      }
      // We are not receiving a net game clone?
      else
      {
        DoNotifyWarning("Unable To Set NetObject Parent",
                        String::Format("There was an error setting the parent of NetObject "
                                       "'%s' - Parent NetObject does not exist locally!",
                                       owner->GetDescription().c_str()));
      }
      return;
    }

    // Parent has changed?
    if (currentParent != newParent)
    {
      // Detach from old parent
      owner->DetachPreserveLocal();

      // Attach to new parent
      owner->AttachToPreserveLocal(newParent);
    }
  }
  // Parent Cleared?
  else
  {
    // Has current parent?
    if (currentParent)
    {
      // Detach from parent
      owner->DetachPreserveLocal();
    }
  }
}

//
// Component Cog Property Getter / Setter
//

Variant GetComponentCogProperty(const Variant& propertyData)
{
  // Get associated property instance data
  String propertyName = propertyData.GetOrError<ComponentPropertyInstanceData>().mPropertyName;
  Component* component = propertyData.GetOrError<ComponentPropertyInstanceData>().mComponent;
  BoundType* componentBoundType = ZilchVirtualTypeId(component);

  // Get property instance
  Property* property = componentBoundType->GetProperty(propertyName);
  if (!property) // Unable?
    return Variant();

  // Result
  return Variant(GetNetPropertyCogAsNetObjectId(property, component));
}
void SetComponentCogProperty(const Variant& value, Variant& propertyData)
{
  // Get associated property instance data
  String propertyName = propertyData.GetOrError<ComponentPropertyInstanceData>().mPropertyName;
  Component* component = propertyData.GetOrError<ComponentPropertyInstanceData>().mComponent;
  BoundType* componentBoundType = ZilchVirtualTypeId(component);
  NetObject* netObject = component->GetOwner()->has(NetObject);

  // Get property instance
  Property* property = componentBoundType->GetProperty(propertyName);
  if (!property) // Unable?
    return;

  // Result
  SetNetPropertyCogAsNetObjectId(property, component, netObject->GetNetPeer(), value.GetOrError<NetObjectId>());
}

//
// Component CogPath Property Getter / Setter
//

Variant GetComponentCogPathProperty(const Variant& propertyData)
{
  // Get associated property instance data
  String propertyName = propertyData.GetOrError<ComponentPropertyInstanceData>().mPropertyName;
  Component* component = propertyData.GetOrError<ComponentPropertyInstanceData>().mComponent;
  BoundType* componentBoundType = ZilchVirtualTypeId(component);

  // Get property instance
  Property* property = componentBoundType->GetProperty(propertyName);
  if (!property) // Unable?
    return Variant();

  // Get cog path value
  Any cogPathAny = property->GetValue(component);
  if (!cogPathAny.IsHoldingValue()) // Unable?
  {
    DoNotifyError("NetProperty",
                  "Error getting CogPath NetProperty - Unable to get property "
                  "instance value");
    return Variant();
  }
  CogPath* cogPath = cogPathAny.Get<CogPath*>();
  Assert(cogPath);

  // Get cog path string
  String cogPathString = cogPath->GetPath();
  return Variant(cogPathString);
}
void SetComponentCogPathProperty(const Variant& value, Variant& propertyData)
{
  // Get associated property instance data
  String propertyName = propertyData.GetOrError<ComponentPropertyInstanceData>().mPropertyName;
  Component* component = propertyData.GetOrError<ComponentPropertyInstanceData>().mComponent;
  BoundType* componentBoundType = ZilchVirtualTypeId(component);

  // Get property instance
  Property* property = componentBoundType->GetProperty(propertyName);
  if (!property) // Unable?
    return;

  // Get cog path value
  Any cogPathAny = property->GetValue(component);
  if (!cogPathAny.IsHoldingValue()) // Unable?
  {
    DoNotifyError("NetProperty",
                  "Error setting CogPath NetProperty - Unable to get property "
                  "instance value");
    return;
  }
  CogPath* cogPath = cogPathAny.Get<CogPath*>();
  Assert(cogPath);

  // Set cog path string
  String cogPathString = value.GetOrError<String>();
  cogPath->SetPath(cogPathString);
}

//
// Component Any Property Getter / Setter
//

Variant GetComponentAnyProperty(const Variant& propertyData)
{
  // Get associated property instance data
  String propertyName = propertyData.GetOrError<ComponentPropertyInstanceData>().mPropertyName;
  Component* component = propertyData.GetOrError<ComponentPropertyInstanceData>().mComponent;
  BoundType* componentBoundType = ZilchVirtualTypeId(component);

  // Get property instance
  Property* property = componentBoundType->GetProperty(propertyName);
  if (!property) // Unable?
    return Variant();

  // Get any value
  Any anyValue = property->GetValue(component);

  // Attempt to convert basic any value to variant value
  Variant variantValue = ConvertBasicAnyToVariant(anyValue);
  if (variantValue.IsEmpty()) // Unable? (The any's stored type is not a basic
                              // native type?)
  {
    // Assign the any value itself to the variant value
    // (Some property types like enums, resources, and bitstream are expected to
    // be wrapped in an any this way)
    variantValue = anyValue;
  }

  // Return the property value
  return variantValue;
}
void SetComponentAnyProperty(const Variant& value, Variant& propertyData)
{
  // Get associated property instance data
  String propertyName = propertyData.GetOrError<ComponentPropertyInstanceData>().mPropertyName;
  Component* component = propertyData.GetOrError<ComponentPropertyInstanceData>().mComponent;
  BoundType* componentBoundType = ZilchVirtualTypeId(component);

  // Get property instance
  Property* property = componentBoundType->GetProperty(propertyName);
  if (!property) // Unable?
    return;

  // Get variant value
  const Variant& variantValue = value;

  // Attempt to convert basic variant value to any value
  Any anyValue = ConvertBasicVariantToAny(variantValue);
  if (!anyValue.IsHoldingValue()) // Unable? (The variant's stored type is not a
                                  // basic native type?)
  {
    // Get the any value itself from the variant value
    // (Some property types like enums, resources, and bitstream are expected to
    // be wrapped in an any this way)
    anyValue = variantValue.GetOrError<Any>();
  }

  // Set the property value
  property->SetValue(component, anyValue);
}

//
// Helper Methods
//

BasicNativeType::Enum BasicNetworkToNativeTypeEnum(BasicNetType::Enum value)
{
  switch (value)
  {
  default:
  case BasicNetType::Other:
    return BasicNativeType::Unknown;

  case BasicNetType::Byte:
    return BasicNativeType::Uint8;

  case BasicNetType::Boolean:
    return BasicNativeType::Bool;
  case BasicNetType::Boolean2:
    return BasicNativeType::BoolVector2;
  case BasicNetType::Boolean3:
    return BasicNativeType::BoolVector3;
  case BasicNetType::Boolean4:
    return BasicNativeType::BoolVector4;

  case BasicNetType::Integer:
    return BasicNativeType::Int32;
  case BasicNetType::Integer2:
    return BasicNativeType::IntVector2;
  case BasicNetType::Integer3:
    return BasicNativeType::IntVector3;
  case BasicNetType::Integer4:
    return BasicNativeType::IntVector4;

  case BasicNetType::Real:
    return BasicNativeType::Float;
  case BasicNetType::Real2:
    return BasicNativeType::Vector2;
  case BasicNetType::Real3:
    return BasicNativeType::Vector3;
  case BasicNetType::Real4:
    return BasicNativeType::Vector4;

  case BasicNetType::Quaternion:
    return BasicNativeType::Quaternion;

  case BasicNetType::Real2x2:
    return BasicNativeType::Matrix2;
  case BasicNetType::Real3x3:
    return BasicNativeType::Matrix3;
  case BasicNetType::Real4x4:
    return BasicNativeType::Matrix4;

  case BasicNetType::DoubleInteger:
    return BasicNativeType::Int64;
  case BasicNetType::DoubleReal:
    return BasicNativeType::Double;

  case BasicNetType::String:
    return BasicNativeType::String;
  }
}
BasicNetType::Enum BasicNativeToNetworkTypeEnum(BasicNativeType::Enum value)
{
  switch (value)
  {
  default:
  case BasicNativeType::Unknown:
    return BasicNetType::Other;

  case BasicNativeType::Uint8:
    return BasicNetType::Byte;

  case BasicNativeType::Bool:
    return BasicNetType::Boolean;
  case BasicNativeType::BoolVector2:
    return BasicNetType::Boolean2;
  case BasicNativeType::BoolVector3:
    return BasicNetType::Boolean3;
  case BasicNativeType::BoolVector4:
    return BasicNetType::Boolean4;

  case BasicNativeType::Int32:
    return BasicNetType::Integer;
  case BasicNativeType::IntVector2:
    return BasicNetType::Integer2;
  case BasicNativeType::IntVector3:
    return BasicNetType::Integer3;
  case BasicNativeType::IntVector4:
    return BasicNetType::Integer4;

  case BasicNativeType::Float:
    return BasicNetType::Real;
  case BasicNativeType::Vector2:
    return BasicNetType::Real2;
  case BasicNativeType::Vector3:
    return BasicNetType::Real3;
  case BasicNativeType::Vector4:
    return BasicNetType::Real4;

  case BasicNativeType::Quaternion:
    return BasicNetType::Quaternion;

  case BasicNativeType::Matrix2:
    return BasicNetType::Real2x2;
  case BasicNativeType::Matrix3:
    return BasicNetType::Real3x3;
  case BasicNativeType::Matrix4:
    return BasicNetType::Real4x4;

  case BasicNativeType::Int64:
    return BasicNetType::DoubleInteger;
  case BasicNativeType::Double:
    return BasicNetType::DoubleReal;

  case BasicNativeType::String:
    return BasicNetType::String;
  }
}

bool HasNetPeerIdProperty(Event* event)
{
  // Get event type
  BoundType* eventType = ZilchVirtualTypeId(event);
  if (!eventType)
  {
    Assert(false);
    return false;
  }

  // For all properties
  MemberRange<Property> properties = eventType->GetProperties(Members::InheritedInstanceExtension);
  forRange (Property* property, properties)
  {
    // Is a net peer ID property?
    if (property->HasAttribute(PropertyAttributes::cNetPeerId) &&
        IsValidNetPeerIdPropertyType(property->GetValue(event).StoredType))
      return true;
  }

  return false;
}
void SetNetPeerIdProperties(Event* event, NetPeerId netPeerId)
{
  // Get event type
  BoundType* eventType = ZilchVirtualTypeId(event);
  if (!eventType)
  {
    Assert(false);
    return;
  }

  // For all properties
  MemberRange<Property> properties = eventType->GetProperties(Members::InheritedInstanceExtension);
  forRange (Property* property, properties)
  {
    // Is a net peer ID property?
    if (property->HasAttribute(PropertyAttributes::cNetPeerId) &&
        IsValidNetPeerIdPropertyType(property->GetValue(event).StoredType))
    {
      // Set net peer ID
      property->SetValue(event, netPeerId);
    }
  }
}

void SetNetPropertyCogAsNetObjectId(Property* property, const Any& instance, NetPeer* netPeer, NetObjectId netObjectId)
{
  // Cog set?
  Cog* cog = nullptr;
  if (netObjectId)
  {
    // Find cog
    cog = netPeer->GetNetObject(netObjectId);
    if (!cog) // Unable?
    {
      DoNotifyWarning("NetProperty",
                      "Unable to set Cog NetProperty - Network object does not "
                      "exist locally");
      return;
    }
  }

  // Success
  property->SetValue(instance, cog);
}
NetObjectId GetNetPropertyCogAsNetObjectId(Property* property, const Any& instance)
{
  // Get cog value
  Cog* cog = property->GetValue(instance).Get<Cog*>();
  if (!cog) // Empty?
    return NetObjectId(0);

  // Get cog net object
  NetObject* cogNetObject = cog->has(NetObject);
  if (!cogNetObject) // Unable?
  {
    DoNotifyError("NetProperty",
                  "Error getting Cog NetProperty - Cog being referenced must "
                  "have a NetObject component");
    return NetObjectId(0);
  }

  // Success
  return cogNetObject->GetNetObjectId();
}

//                                 FamilyTree //

FamilyTree::FamilyTree() :
    mFamilyTreeId(0), mAncestorDisplayName(), mAncestorCreateContext(), mAncestorReplicaType(), mReplicas()
{
  // (Should not actually get called)
  Assert(false);
}
FamilyTree::FamilyTree(FamilyTreeId familyTreeId, NetObject* ancestor) :
    mFamilyTreeId(familyTreeId),
    mAncestorDisplayName(ancestor->GetOwner()->GetDescription()),
    mAncestorCreateContext(ancestor->GetCreateContext()),
    mAncestorReplicaType(ancestor->GetReplicaType()),
    mReplicas()
{
  // (Family tree ID should be valid)
  Assert(familyTreeId != 0);

  AddNetObject(ancestor);
}

FamilyTree::~FamilyTree()
{
}

bool FamilyTree::operator==(const FamilyTree& rhs) const
{
  return mFamilyTreeId == rhs.mFamilyTreeId;
}
bool FamilyTree::operator!=(const FamilyTree& rhs) const
{
  return mFamilyTreeId != rhs.mFamilyTreeId;
}
bool FamilyTree::operator<(const FamilyTree& rhs) const
{
  return mFamilyTreeId < rhs.mFamilyTreeId;
}
bool FamilyTree::operator==(const FamilyTreeId& rhs) const
{
  return mFamilyTreeId == rhs;
}
bool FamilyTree::operator!=(const FamilyTreeId& rhs) const
{
  return mFamilyTreeId != rhs;
}
bool FamilyTree::operator<(const FamilyTreeId& rhs) const
{
  return mFamilyTreeId < rhs;
}

//
// Operations
//

bool FamilyTree::AddNetObject(NetObject* netObject)
{
  // Add net object
  mReplicas.PushBack(static_cast<Replica*>(netObject));

  // Set net object's family tree ID
  netObject->SetFamilyTreeId(mFamilyTreeId);

  // Success
  return true;
}
bool FamilyTree::RemoveNetObject(NetObject* netObject)
{
  // Find pointer to net object
  if (Replica** pointer = mReplicas.FindPointer(static_cast<Replica*>(netObject)))
  {
    // Clear net object's family tree ID
    netObject->SetFamilyTreeId(0);

    // Clear pointer (we intentionally leave the nullptr element in the array)
    *pointer = nullptr;

    // Found
    return true;
  }

  // Not found
  return false;
}

FamilyTreeId FamilyTree::GetFamilyTreeId() const
{
  return mFamilyTreeId;
}

const String& FamilyTree::GetAncestorDisplayName() const
{
  return mAncestorDisplayName;
}
const CreateContext& FamilyTree::GetAncestorCreateContext() const
{
  return mAncestorCreateContext;
}
const ReplicaType& FamilyTree::GetAncestorReplicaType() const
{
  return mAncestorReplicaType;
}

const ReplicaArray& FamilyTree::GetReplicas() const
{
  return mReplicas;
}

bool FamilyTree::IsEmpty() const
{
  // For All net objects
  forRange (Replica* netObject, mReplicas.All())
    if (netObject) // Is present?
    {
      // There is at least one present net object
      return false;
    }

  // All net objects are absent
  return true;
}

} // namespace Zero
