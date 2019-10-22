// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{
namespace Events
{
DefineEvent(OsShellUpdate);
DefineEvent(FileDialogComplete);
} // namespace Events

ZilchDefineType(OsShell, builder, type)
{
  type->HandleManager = ZilchManagerId(PointerManager);

  ZilchBindMethod(IsClipboardText);
  ZilchBindGetterSetterProperty(ClipboardText);

  ZilchBindGetterProperty(WindowCount);
  ZilchBindMethod(GetWindow);
  ZilchBindSetter(MouseCursor);

  ZilchBindMethod(DumpMemoryDebuggerStats);
}

OsShell* CreateOsShellSystem()
{
  return new OsShell();
}

OsShell::OsShell() : mOsShellHook(nullptr)
{
  mShell.mUserData = this;
}

cstr OsShell::GetName()
{
  return "OsShell";
}

void OsShell::Update(bool debugger)
{
  ProfileScopeTree("ShellSystem", "Engine", Color::Red);

  Keyboard* keyboard = Keyboard::GetInstance();
  keyboard->Update();

  // Zero the cursor movement before the windows message pump to clear last
  // frames movement
  Z::gMouse->mCursorMovement = Vec2::cZero;
  Z::gMouse->mRawMovement = Vec2(0, 0);

  if (mOsShellHook)
    mOsShellHook->HookUpdate();

  mShell.Update();

  // This is a special place to update for other systems like the
  // that may cause the message pump to run (originally used for CEF browser).
  Event toSend;
  DispatchEvent(Events::OsShellUpdate, &toSend);
  Z::gEngine->DispatchEvent(Events::OsShellUpdate, &toSend);
}

String OsShell::GetOsName()
{
  return mShell.GetOsName();
}

uint OsShell::GetScrollLineCount()
{
  return mShell.GetScrollLineCount();
}

IntRect OsShell::GetPrimaryMonitorRectangle()
{
  return mShell.GetPrimaryMonitorRectangle();
}

IntVec2 OsShell::GetPrimaryMonitorSize()
{
  return mShell.GetPrimaryMonitorSize();
}

OsWindow* OsShell::CreateOsWindow(StringParam windowName,
                                  IntVec2Param clientSize,
                                  IntVec2Param monitorClientPos,
                                  OsWindow* parentWindow,
                                  WindowStyleFlags::Enum flags,
                                  WindowState::Enum state)
{
  return new OsWindow(this, windowName, clientSize, monitorClientPos, parentWindow, flags, state);
}

ByteColor OsShell::GetColorAtMouse()
{
  return mShell.GetColorAtMouse();
}

void OsShell::SetMouseCursor(Cursor::Enum cursorId)
{
  return mShell.SetMouseCursor(cursorId);
}

bool OsShell::IsClipboardText()
{
  return mShell.IsClipboardText();
}

String OsShell::GetClipboardText()
{
  return mShell.GetClipboardText();
}

void OsShell::SetClipboardText(StringParam text)
{
  return mShell.SetClipboardText(text);
}

bool OsShell::IsClipboardImage()
{
  return mShell.IsClipboardImage();
}

bool OsShell::GetClipboardImage(Image* imageBuffer)
{
  return mShell.GetClipboardImage(imageBuffer);
}

bool OsShell::GetPrimaryMonitorImage(Image* imageBuffer)
{
  return mShell.GetPrimaryMonitorImage(imageBuffer);
}

void OsShell::OpenFile(FileDialogConfig* config)
{
  return mShell.OpenFile(*config);
}

void OsShell::SaveFile(FileDialogConfig* config)
{
  return mShell.SaveFile(*config);
}

void OsShell::ShowMessageBox(StringParam title, StringParam message)
{
  return mShell.ShowMessageBox(title, message);
}

void OsShell::ScanInputDevices()
{
  // DeactivateAll because joysticks may have been removed in device changed
  Z::gJoysticks->DeactivateAll();

  const Array<PlatformInputDevice>& devices = mShell.ScanInputDevices();
  forRange (PlatformInputDevice& device, devices)
  {
    // Tell the Joysticks system that a Joystick is present
    Z::gJoysticks->AddJoystickDevice(device);
  }

  Z::gJoysticks->JoysticksChanged();
}

size_t OsShell::GetWindowCount()
{
  return mShell.mWindows.Size();
}

OsWindow* OsShell::GetWindow(size_t index)
{
  if (index >= mShell.mWindows.Size())
  {
    DoNotifyException("Shell", "Invalid window index");
    return nullptr;
  }
  return (OsWindow*)mShell.mWindows[index]->mUserData;
}

void OsShell::DumpMemoryDebuggerStats()
{
  Memory::DumpMemoryDebuggerStats("MyProject");
}

ZilchDefineType(OsFileSelection, builder, type)
{
}

FileDialogConfig* FileDialogConfig::Create()
{
  return new FileDialogConfig();
}

FileDialogConfig::FileDialogConfig()
{
  EventName = Events::FileDialogComplete;
  CallbackObject = nullptr;
  mCallback = &Callback;
  mUserData = this;
}

void FileDialogConfig::Callback(Array<String>& files, void* userData)
{
  FileDialogConfig* self = (FileDialogConfig*)userData;

  if (Object* callbackObject = self->CallbackObject)
  {
    EventDispatcher* dispatcher = callbackObject->GetDispatcherObject();
    if (dispatcher)
    {
      OsFileSelection fileEvent;
      fileEvent.Files = files;
      fileEvent.Success = !files.Empty();
      dispatcher->Dispatch(self->EventName, &fileEvent);
    }
  }

  // At the end of the callback we need to free the memory for the config.
  delete self;
}

SimpleSaveFileDialog::SimpleSaveFileDialog(StringParam data,
                                           StringParam title,
                                           StringParam filterName,
                                           StringParam filter,
                                           StringParam defaultExtension,
                                           StringParam defaultFileName) :
    mData(data)
{
  FileDialogConfig* config = FileDialogConfig::Create();
  config->CallbackObject = this;
  config->Title = title;
  config->AddFilter(filterName, filter);
  config->DefaultFileName = defaultFileName;
  config->mDefaultSaveExtension = defaultExtension;

  ConnectThisTo(this, Events::FileDialogComplete, OnFileDialogComplete);
  Z::gEngine->has(OsShell)->SaveFile(config);
}

void SimpleSaveFileDialog::OnFileDialogComplete(OsFileSelection* event)
{
  if (event->Success)
  {
    String filePath = event->Files[0];
    WriteStringRangeToFile(filePath, mData);
    DownloadFiles(filePath);
  }
  delete this;
}

} // namespace Zero
