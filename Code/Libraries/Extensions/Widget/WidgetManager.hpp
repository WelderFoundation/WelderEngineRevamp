// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

class RootWidget;

class WidgetManager : public ExplicitSingleton<WidgetManager, EventObject>
{
public:
  ZilchDeclareType(WidgetManager, TypeCopyMode::ReferenceType);

  WidgetManager();
  ~WidgetManager();

  u64 IdCounter;
  HashMap<u64, Widget*> Widgets;
  InList<RootWidget> RootWidgets;
  Array<Widget*> DestroyList;
  ActionSpace* mWidgetActionSpace;

  void CleanUp();
  void OnEngineUpdate(UpdateEvent* event);
  void OnShutdown(Event* event);
};

namespace Z
{
extern WidgetManager* gWidgetManager;
}

} // namespace Zero
