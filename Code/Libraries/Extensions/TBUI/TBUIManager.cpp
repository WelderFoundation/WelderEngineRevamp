// MIT Licensed (see LICENSE.md).
#include "Precompiled.hpp"

namespace Zero
{

ZilchDefineType(TBUIManager, builder, type)
{
}

TBUIManager::TBUIManager() : mRoot(nullptr)
{
  Z::gTBUIManager = this;

  mRenderer = new TBUIRenderer();

  ConnectThisTo(Z::gEngine, Events::EngineUpdate, OnEngineUpdate);
  ConnectThisTo(Z::gEngine, Events::EngineDebuggerUpdate, OnEngineUpdate);
  ConnectThisTo(Z::gEngine, Events::EngineShutdown, OnShutdown);
}

TBUIManager::~TBUIManager()
{
  if (mRoot != nullptr)
  {
    delete mRoot;
    mRoot = nullptr;
  }
  delete mRenderer;
}

void TBUIManager::OnUiUpdate(UpdateEvent* event)
{
  OsShell* shell = Z::gEngine->has(OsShell);
  if (shell != nullptr)
  {
    OsWindow* osWindow = shell->GetWindow(0);
    IntVec2 clientSize = osWindow->GetClientSize();
    IntVec2 clientPos = osWindow->GetMonitorClientPosition();

    mRoot->SetRect(tb::TBRect(clientPos.x, clientPos.y, clientSize.x, clientSize.y));
    tb::TBAnimationManager::Update();
    mRoot->InvokeProcessStates();
    mRoot->InvokeProcess();

    tb::TBMessageHandler::ProcessMessages();
  }
}

void TBUIManager::OnUiRenderUpdate(Event* event)
{
  OsShell* shell = Z::gEngine->has(OsShell);
  OsWindow* osWindow = shell->GetWindow(0);
  IntVec2 clientSize = osWindow->GetClientSize();
  //IntVec2 clientPos = osWindow->GetMonitorClientPosition();
  if (shell != nullptr)
  {
    mRenderer->BeginPaint(clientSize.x, clientSize.y);
    tb::TBWidget::PaintProps props;
    mRoot->InvokePaint(props);
    //mRoot->GetFont()->DrawString(5, 5, tb::TBColor(255, 255, 255), "Hello from TB");
    mRenderer->EndPaint();
  }
}

tb::MODIFIER_KEYS GetModifierKeys(OsMouseEvent* mouseEvent)
{
  tb::MODIFIER_KEYS modifierKeys = tb::MODIFIER_KEYS::TB_MODIFIER_NONE;

  if (mouseEvent->AltPressed)
    modifierKeys |= tb::MODIFIER_KEYS::TB_ALT;

  if (mouseEvent->ShiftPressed)
    modifierKeys |= tb::MODIFIER_KEYS::TB_SHIFT;

  if (mouseEvent->CtrlPressed)
    modifierKeys |= tb::MODIFIER_KEYS::TB_CTRL;

  return modifierKeys;
}

void TBUIManager::OnOsMouseUp(OsMouseEvent* mouseEvent)
{
  tb::MODIFIER_KEYS modifierKeys = GetModifierKeys(mouseEvent);

  mRoot->InvokePointerUp(mouseEvent->ClientPosition.x, mouseEvent->ClientPosition.y, modifierKeys, false);
}

void TBUIManager::OnOsMouseDown(OsMouseEvent* mouseEvent)
{
  tb::MODIFIER_KEYS modifierKeys = GetModifierKeys(mouseEvent);

  mRoot->InvokePointerDown(mouseEvent->ClientPosition.x, mouseEvent->ClientPosition.y, 1, modifierKeys, false);
}

void TBUIManager::OnOsMouseMoved(OsMouseEvent* mouseEvent)
{
  tb::MODIFIER_KEYS modifierKeys = GetModifierKeys(mouseEvent);

  mRoot->InvokePointerMove(mouseEvent->ClientPosition.x, mouseEvent->ClientPosition.y, modifierKeys, false);
}

void TBUIManager::OnEngineUpdate(UpdateEvent* event)
{
  if (!tb::tb_core_is_initialized())
  {
    if (tb::tb_core_init(mRenderer))
    {
      // Load language file
      tb::g_tb_lng->Load("@TBUI/resources/language/lng_en.tb.txt");

      // Load the default skin
      tb::g_tb_skin->Load("@TBUI/resources/default_skin/skin.tb.txt", "@TBUI/demo/skin/skin.tb.txt");

      // Register font renderers.
#ifdef TB_FONT_RENDERER_TBBF
      void register_tbbf_font_renderer();
      register_tbbf_font_renderer();
#endif

#ifdef TB_FONT_RENDERER_STB
      void register_stb_font_renderer();
      register_stb_font_renderer();
#endif
#ifdef TB_FONT_RENDERER_FREETYPE
      void register_freetype_font_renderer();
      register_freetype_font_renderer();
#endif

      // Add fonts we can use to the font manager.
#if defined(TB_FONT_RENDERER_STB) || defined(TB_FONT_RENDERER_FREETYPE)
      tb::g_font_manager->AddFontInfo("@TBUI/resources/vera.ttf", "Vera");
#endif
#ifdef TB_FONT_RENDERER_TBBF
      tb::g_font_manager->AddFontInfo("@TBUI/resources/default_font/segoe_white_with_shadow.tb.txt", "Segoe");
#endif

      // Set the default font description for widgets to one of the fonts we just added
      tb::TBFontDescription fd;
#ifdef TB_FONT_RENDERER_TBBF
      fd.SetID(TBIDC("Segoe"));
#else
      fd.SetID(TBIDC("Vera"));
#endif
      fd.SetSize(tb::g_tb_skin->GetDimensionConverter()->DpToPx(14));
      tb::g_font_manager->SetDefaultFontDescription(fd);

      // Create the font now.
      tb::TBFontFace* font = tb::g_font_manager->CreateFontFace(tb::g_font_manager->GetDefaultFontDescription());

      // Render some glyphs in one go now since we know we are going to use them. It would work fine
      // without this since glyphs are rendered when needed, but with some extra updating of the glyph bitmap.
      if (font)
        font->RenderGlyphs(" !\"#$%&'()*+,-./"
                           "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~•·åäöÅÄÖ");

      if (mRoot == nullptr)
      {
        OsShell* shell = Z::gEngine->has(OsShell);
        OsWindow* osWindow = shell->GetWindow(0);
        IntVec2 clientSize = osWindow->GetClientSize();
        IntVec2 clientPos = osWindow->GetMonitorClientPosition();

        mRoot = new tb::TBWidget();
        mRoot->SetRect(tb::TBRect(clientPos.x, clientPos.y, clientSize.x, clientSize.y));
        //mRoot->SetOpacity(0.1);

        tb::TBWindow* mainWindow = new tb::TBWindow();

        mRoot->AddChild(mainWindow);

        tb::TBNode node;
        if (node.ReadFile("@TBUI/demo/ui_resources/test_ui.tb.txt"))
        {
          tb::g_widgets_reader->LoadNodeTree(mainWindow, &node);

          // Get title from the WindowInfo section (or use "" if not specified)
          mainWindow->SetText(node.GetValueString("WindowInfo>title", ""));

          const tb::TBRect parent_rect(0, 0, mainWindow->GetParent()->GetRect().w, mainWindow->GetParent()->GetRect().h);
          const tb::TBDimensionConverter* dc = tb::g_tb_skin->GetDimensionConverter();
          tb::TBRect window_rect = mainWindow->GetResizeToFitContentRect();

          // Use specified size or adapt to the preferred content size.
          tb::TBNode* tmp = node.GetNode("WindowInfo>size");
          if (tmp && tmp->GetValue().GetArrayLength() == 2)
          {
            window_rect.w = dc->GetPxFromString(tmp->GetValue().GetArray()->GetValue(0)->GetString(), window_rect.w);
            window_rect.h = dc->GetPxFromString(tmp->GetValue().GetArray()->GetValue(1)->GetString(), window_rect.h);
          }

          // Use the specified position or center in parent.
          tmp = node.GetNode("WindowInfo>position");
          if (tmp && tmp->GetValue().GetArrayLength() == 2)
          {
            window_rect.x = dc->GetPxFromString(tmp->GetValue().GetArray()->GetValue(0)->GetString(), window_rect.x);
            window_rect.y = dc->GetPxFromString(tmp->GetValue().GetArray()->GetValue(1)->GetString(), window_rect.y);
          }
          else
            window_rect = window_rect.CenterIn(parent_rect);

          // Make sure the window is inside the parent, and not larger.
          window_rect = window_rect.MoveIn(parent_rect).Clip(parent_rect);

          mainWindow->SetRect(window_rect);

          //mainWindow->SetOpacity(1);
        }

        //ConnectThisTo(osWindow, Events::OsResized, OnOsResize);
        ConnectThisTo(osWindow, Events::OsMouseDown, OnOsMouseDown);
        ConnectThisTo(osWindow, Events::OsMouseUp, OnOsMouseUp);
        ConnectThisTo(osWindow, Events::OsMouseMove, OnOsMouseMoved);

        //ConnectThisTo(osWindow, Events::OsWindowBorderHitTest, OnOsWindowBorderHitTest);

        //ConnectThisTo(osWindow, Events::OsMouseScroll, OnOsMouseScroll);

        //ConnectThisTo(osWindow, Events::OsKeyTyped, OnOsKeyTyped);
        //ConnectThisTo(osWindow, Events::OsKeyRepeated, OnOsKeyDown);
        //ConnectThisTo(osWindow, Events::OsKeyDown, OnOsKeyDown);
        //ConnectThisTo(osWindow, Events::OsKeyUp, OnOsKeyUp);

        //ConnectThisTo(osWindow, Events::OsFocusGained, OnOsFocusGained);
        //ConnectThisTo(osWindow, Events::OsFocusLost, OnOsFocusLost);

        //ConnectThisTo(osWindow, Events::OsMouseFileDrop, OnOsMouseDrop);
        //ConnectThisTo(osWindow, Events::OsPaint, OnOsPaint);

        //ConnectThisTo(osWindow, Events::OsClose, OnClose);
      }

      // Give the root widget a background skin
      mRoot->SetSkinBg(TBIDC("background"));

      tb::TBWidgetsAnimationManager::Init();

      ConnectThisTo(Z::gEngine->has(TimeSystem), "UiUpdate", OnUiUpdate);
      ConnectThisTo(Z::gEngine->has(GraphicsEngine), "UiRenderUpdate", OnUiRenderUpdate);

    }
  }
}

void TBUIManager::OnShutdown(Event* event)
{
  if (tb::tb_core_is_initialized())
  {
    tb::TBWidgetsAnimationManager::Shutdown();
    tb::tb_core_shutdown();
  }
}

namespace Z
{
TBUIManager* gTBUIManager = nullptr;
}

} // namespace Zero