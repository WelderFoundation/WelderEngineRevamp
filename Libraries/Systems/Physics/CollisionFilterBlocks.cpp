// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

ZilchDefineType(CollisionFilterBlock, builder, type)
{
  ZeroBindDocumented();

  ZilchBindGetter(BlockType);
  ZilchBindGetterSetterProperty(SendEventsToA);
  ZilchBindGetterSetterProperty(SendEventsToB);
  ZilchBindGetterSetterProperty(SendEventsToSpace);
  ZilchBindFieldProperty(mEventOverride);
}

CollisionFilterBlock::CollisionFilterBlock()
{
  mStates.Clear();
}

void CollisionFilterBlock::Serialize(Serializer& stream)
{
  uint defaultFlags = CollisionBlockStates::SendEventsToA | CollisionBlockStates::SendEventsToB;
  SerializeBits(stream, mStates, CollisionBlockStates::Names, 0, defaultFlags);
  SerializeNameDefault(mEventOverride, String());
}

bool CollisionFilterBlock::GetSendEventsToA()
{
  return mStates.IsSet(CollisionBlockStates::SendEventsToA);
}

void CollisionFilterBlock::SetSendEventsToA(bool state)
{
  mStates.SetState(CollisionBlockStates::SendEventsToA, state);
}

bool CollisionFilterBlock::GetSendEventsToB()
{
  return mStates.IsSet(CollisionBlockStates::SendEventsToB);
}

void CollisionFilterBlock::SetSendEventsToB(bool state)
{
  mStates.SetState(CollisionBlockStates::SendEventsToB, state);
}

bool CollisionFilterBlock::GetSendEventsToSpace()
{
  return mStates.IsSet(CollisionBlockStates::SendEventsToSpace);
}

void CollisionFilterBlock::SetSendEventsToSpace(bool state)
{
  mStates.SetState(CollisionBlockStates::SendEventsToSpace, state);
}

CollisionFilterBlockType::Enum CollisionFilterBlock::GetBlockType() const
{
  if (mBlockType & FilterFlags::StartEvent)
    return CollisionFilterBlockType::CollisionStartedBlock;
  if (mBlockType & FilterFlags::PersistedEvent)
    return CollisionFilterBlockType::CollisionPersistedBlock;
  if (mBlockType & FilterFlags::EndEvent)
    return CollisionFilterBlockType::CollisionEndedBlock;
  if (mBlockType & FilterFlags::PreSolveEvent)
    return CollisionFilterBlockType::PreSolveBlock;
  return CollisionFilterBlockType::CollisionStartedBlock;
}

ZilchDefineType(CollisionStartBlock, builder, type)
{
  ZeroBindComponent();
  type->HasOrAdd<::Zero::CogComponentMeta>(type);
  type->Add(new MetaSerialization());
  ZeroBindSetup(SetupMode::DefaultSerialization);
  ZeroBindDocumented();
}

CollisionStartBlock::CollisionStartBlock()
{
  mBlockType = FilterFlags::StartEvent;
}

ZilchDefineType(CollisionPersistedBlock, builder, type)
{
  ZeroBindComponent();
  type->HasOrAdd<::Zero::CogComponentMeta>(type);
  type->Add(new MetaSerialization());
  ZeroBindSetup(SetupMode::DefaultSerialization);
  ZeroBindDocumented();
}

CollisionPersistedBlock::CollisionPersistedBlock()
{
  mBlockType = FilterFlags::PersistedEvent;
}

ZilchDefineType(CollisionEndBlock, builder, type)
{
  ZeroBindComponent();
  type->HasOrAdd<::Zero::CogComponentMeta>(type);
  type->Add(new MetaSerialization());
  ZeroBindSetup(SetupMode::DefaultSerialization);
  ZeroBindDocumented();
}

CollisionEndBlock::CollisionEndBlock()
{
  mBlockType = FilterFlags::EndEvent;
}

ZilchDefineType(PreSolveBlock, builder, type)
{
  ZeroBindComponent();
  type->HasOrAdd<::Zero::CogComponentMeta>(type);
  type->Add(new MetaSerialization());
  ZeroBindSetup(SetupMode::DefaultSerialization);
  ZeroBindDocumented();
}

PreSolveBlock::PreSolveBlock()
{
  mBlockType = FilterFlags::PreSolveEvent;
}

ZilchDefineTemplateType(CollisionFilterMetaComposition, builder, type)
{
}

} // namespace Zero
