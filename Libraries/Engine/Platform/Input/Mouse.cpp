// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

namespace Z
{
Mouse* gMouse = nullptr;
}

Mouse::Mouse()
{
  mPlatform = Z::gEngine->has(OsShell);
  mCurrentCursor = Cursor::Arrow;
  mCursorMovement = Vec2::cZero;
  mClientPosition = Vec2::cZero;
  mRawMovement = Vec2::cZero;
  mActiveWindow = nullptr;
  for (uint i = 0; i < MouseButtons::Size; ++i)
    mButtonDown[i] = false;
  Z::gMouse = this;
}

ZilchDefineType(Mouse, builder, type)
{
  type->HandleManager = ZilchManagerId(PointerManager);

  ZeroBindDocumented();
  ZilchBindGetterSetterProperty(Cursor);
  ZilchBindMethod(IsButtonDown);

  ZilchBindGetterProperty(ClientPosition);
  ZilchBindGetterProperty(CursorMovement);
  ZilchBindGetterSetterProperty(Trapped);
  ZilchBindMethod(ToggleTrapped);

  ZilchBindFieldProperty(mRawMovement);
}

bool Mouse::IsButtonDown(MouseButtons::Enum button)
{
  return mButtonDown[button] != 0;
}

Cursor::Enum Mouse::GetCursor()
{
  return mCurrentCursor;
}

void Mouse::SetCursor(Cursor::Enum cursor)
{
  Z::gEngine->has(OsShell)->SetMouseCursor(cursor);
  mCurrentCursor = cursor;
}

bool Mouse::GetTrapped()
{
  if (mActiveWindow)
    return mActiveWindow->GetMouseTrap();
  return false;
}

void Mouse::SetTrapped(bool state)
{
  if (mActiveWindow)
    mActiveWindow->SetMouseTrap(state);
}

void Mouse::ToggleTrapped()
{
  if (mActiveWindow)
    mActiveWindow->SetMouseTrap(!mActiveWindow->GetMouseTrap());
}

} // namespace Zero
