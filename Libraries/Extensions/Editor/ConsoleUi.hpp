// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

namespace Events
{
DeclareEvent(ConsolePrint);
} // namespace Events

/// Console Event sent when text is printed.
class ConsoleTextEvent : public Event
{
public:
  ZilchDeclareType(ConsoleTextEvent, TypeCopyMode::ReferenceType);
  String Text;
};

/// Editor Console Window
class ConsoleUi : public TextEditor
{
public:
  ZilchDeclareType(ConsoleUi, TypeCopyMode::ReferenceType);

  ConsoleUi(Composite* parent);
  ~ConsoleUi();

  bool TakeFocusOverride() override;

  // Internals
  class UiConsoleListener : public ConsoleListener
  {
  public:
    ConsoleUi* Owner;
    UiConsoleListener(ConsoleUi* console) : Owner(console)
    {
      Console::Add(this);
    }
    void Print(FilterType filterType, cstr message);
  };

  void ConsoleLog(FilterType filterType, cstr message);
  UiConsoleListener mListener;
  StringBuilder mCurrentLine;
  String mLastLine;
  uint mRepeatCount;

  void OnKeyDown(KeyboardEvent* event) override;
  void OnLeftClick(MouseEvent* event);
  void OnConsolePrint(ConsoleTextEvent* event);
  void OnWidgetShown(Event* event);
  void AddLine(StringParam line);
  /// Adds a block of text that may contain newlines (since each line needs
  /// to be added independently for hotspots to work correctly)
  void AddBlock(StringParam text);
};

} // namespace Zero
