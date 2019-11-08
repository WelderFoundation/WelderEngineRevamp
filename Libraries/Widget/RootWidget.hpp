// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

class UpdateEvent;
// class RootWidgetRenderTask;

// The root widget of the graphical user interface attached to
// an OS window.
class RootWidget : public Composite
{
public:
  ZilchDeclareType(RootWidget, TypeCopyMode::ReferenceType);

  RootWidget(OsWindow* osWindow);
  ~RootWidget();

  void OnUiUpdate(UpdateEvent* event);
  void OnUiRenderUpdate(Event* event);

  // Cancel all ui interactions on current focus object
  void FocusReset();
  void Refresh();
  Widget* GetFocusObject();
  OsWindow* GetOsWindow();
  void ResetHover();

  // Widget Interface
  void UpdateTransform() override;

  /// Get the area to attach pop up widget like tooltips
  /// this widget is forced on top of everything
  virtual Composite* GetPopUp();

  IntrusiveLink(RootWidget, link);

private:
  // Focus on object
  void RootRemoveFocus(Widget* object);
  void RootChangeFocus(Widget* widget, FocusMode::Type focusMode);
  void RootSoftTakeFocus(Widget* widget);

  // Mouse Capture
  void RootCaptureMouse(Widget* object);
  void RootReleaseMouseCapture(Widget* object);

  void MouseUpdate(float dt);
  void OnManagerUpdate(UpdateEvent* event);

  void OnOsResize(OsWindowEvent* windowEvent);
  void OnOsPaint(OsWindowEvent* windowEvent);

  void OnOsFocusGained(OsWindowEvent* mouseEvent);
  void OnOsFocusLost(OsWindowEvent* mouseEvent);

  void OnOsMouseButton(OsMouseEvent* mouseEvent, bool buttonState);
  void OnOsMouseUp(OsMouseEvent* mouseEvent);
  void OnOsMouseDown(OsMouseEvent* mouseEvent);
  void OnOsMouseMoved(OsMouseEvent* mouseEvent);
  void OnOsMouseScroll(OsMouseEvent* mouseEvent);
  void OnOsMouseDrop(OsMouseDropEvent* mouseDrop);
  void OnOsWindowBorderHitTest(OsWindowBorderHitTest* event);

  void OnCutCopyPaste(ClipboardEvent* event);

  void OnOsKeyDown(KeyboardEvent* keyboardEvent);
  void OnOsKeyUp(KeyboardEvent* keyboardEvent);
  void OnOsKeyTyped(KeyboardTextEvent* keyboardTextEvent);
  void OnClose(OsWindowEvent* windowEvent);

  void OnDebuggerPause(Event* event);
  void OnDebuggerResume(Event* event);
  void OnDebuggerResumeDelay();

  Widget* UpdateMousePosition(OsMouseEvent* mouseEvent);
  void BuildMouseEvent(MouseEvent& event, OsMouseEvent* mouseEvent);
  void UpdateMouseButtons(OsMouseEvent* mouseEvent);

  friend class Widget;
  friend class GameWidget;
  OsWindow* mOsWindow;
  Vec4 mClearColor;
  FocusMode::Type mFocusMode;
  uint mLastClickButton;
  Vec2 mLastClickPosition;
  HandleOf<Widget> mOver;
  HandleOf<Widget> mFocus;
  HandleOf<Widget> mFocusWaiting;
  HandleOf<Widget> mCaptured;
  HandleOf<Widget> mDown;
  Element* mDebuggerOverlay;
  Text* mDebuggerText;
  Vec2 mDownPosition;
  Vec2 mDragMovement;
  bool mDragged;
  bool mAnyMouseDown;
  bool mNeedsRedraw;
  float mHoverTime;
  float mHoldTime;
  float mTimeSinceLastClick;
};

// Get the previous sibling in the tree
Widget* PreviousSibling(Widget* object, bool ignoreInactive);

// Get the next sibling in the tree
Widget* NextSibling(Widget* object, bool ignoreInactive);

} // namespace Zero
