// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

// Space
ZilchDefineType(ActionSpace, builder, type)
{
  ZeroBindComponent();
  type->AddAttribute(ObjectAttributes::cCore);
  type->AddAttribute(ObjectAttributes::cHidden);
}

ActionSpace::ActionSpace()
{
}
ActionSpace::~ActionSpace()
{
}

void ActionSpace::Initialize(CogInitializer& initializer)
{
  ConnectThisTo(GetOwner(), Events::ActionFrameUpdate, OnActionFrameUpdate);
  ConnectThisTo(GetOwner(), Events::ActionLogicUpdate, OnActionLogicUpdate);
}

void ActionSpace::OnActionFrameUpdate(UpdateEvent* updateEvent)
{
  UpdateActions(updateEvent, ActionExecuteMode::FrameUpdate);
}

void ActionSpace::OnActionLogicUpdate(UpdateEvent* updateEvent)
{
  UpdateActions(updateEvent, ActionExecuteMode::LogicUpdate);
}

void ActionSpace::UpdateActions(UpdateEvent* updateEvent, ActionExecuteMode::Enum mode)
{
  float dt = updateEvent->Dt;
  float rDt = updateEvent->RealDt;

  InList<Actions>::iterator actionListIt = ActiveLists.Begin();
  while (actionListIt != ActiveLists.End())
  {
    InList<Actions>::iterator next = actionListIt;
    ++next;
    actionListIt->Update(dt, rDt, mode);
    actionListIt = next;
  }
}

} // namespace Zero
