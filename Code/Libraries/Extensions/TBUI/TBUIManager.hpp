// MIT Licensed (see LICENSE.md).
#pragma once

namespace Zero
{

class TBUIWidget;

class TBUIManager : public ExplicitSingleton<TBUIManager, EventObject>
{
public:
  ZilchDeclareType(TBUIManager, TypeCopyMode::ReferenceType);

  TBUIManager();
  ~TBUIManager();

  void OnEngineUpdate(UpdateEvent* event);
  void OnShutdown(Event* event);

  ZeroForceInline tb::TBWidget* GetRootWidget()
  {
    return mRoot;
  }

private:
  void OnUiUpdate(UpdateEvent* event);
  void OnUiRenderUpdate(Event* event);

  void OnOsMouseUp(OsMouseEvent* mouseEvent);
  void OnOsMouseDown(OsMouseEvent* mouseEvent);
  void OnOsMouseMoved(OsMouseEvent* mouseEvent);

private:
  tb::TBWidget* mRoot;
  TBUIRenderer* mRenderer;
};

namespace Z
{
extern TBUIManager* gTBUIManager;
}

} // namespace Zero