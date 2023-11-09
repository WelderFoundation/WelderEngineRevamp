// MIT Licensed (see LICENSE.md).
#pragma once

// External dependencies
#include "Engine/EngineStandard.hpp"
#include "Graphics/GraphicsStandard.hpp"
//#include "Widget/WidgetStandard.hpp"
#include "Gameplay/GameplayStandard.hpp"

namespace Zero
{

// TBUI library
class ZeroNoImportExport TBUILibrary : public Zilch::StaticLibrary
{
public:
  ZilchDeclareStaticLibraryInternals(TBUILibrary, "ZeroEngine");

  static void Initialize();
  static void Shutdown();
};

} // namespace Zero

// TB
#include "tb/animation/tb_animation_utils.h"
#include "tb/animation/tb_animation.h"
#include "tb/animation/tb_widget_animation.h"
#include "tb/image/tb_image_manager.h"
#include "tb/image/tb_image_widget.h"
#include "tb/parser/tb_parser.h"
#include "tb/renderers/tb_renderer_batcher.h"
//#include "tb/tests/tb_test.h"
#include "tb/thirdparty/stb_image.h"
#include "tb/thirdparty/stb_truetype.h"
#include "tb/utf8/utf8.h"
#include "tb/tb_bitmap_fragment.h"
#include "tb/tb_color.h"
#include "tb/tb_config.h"
#include "tb/tb_core.h"
#include "tb/tb_debug.h"
#include "tb/tb_dimension.h"
#include "tb/tb_editfield.h"
#include "tb/tb_font_desc.h"
#include "tb/tb_font_renderer.h"
#include "tb/tb_geometry.h"
#include "tb/tb_hash.h"
#include "tb/tb_hashtable.h"
#include "tb/tb_id.h"
#include "tb/tb_inline_select.h"
#include "tb/tb_language.h"
#include "tb/tb_layout.h"
#include "tb/tb_linklist.h"
#include "tb/tb_list.h"
#include "tb/tb_menu_window.h"
#include "tb/tb_message_window.h"
#include "tb/tb_msg.h"
#include "tb/tb_node_ref_tree.h"
#include "tb/tb_node_tree.h"
#include "tb/tb_object.h"
#include "tb/tb_popup_window.h"
#include "tb/tb_renderer.h"
#include "tb/tb_scroller.h"
#include "tb/tb_scroll_container.h"
#include "tb/tb_select.h"
#include "tb/tb_select_item.h"
#include "tb/tb_skin.h"
#include "tb/tb_skin_util.h"
#include "tb/tb_sort.h"
#include "tb/tb_str.h"
#include "tb/tb_style_edit.h"
#include "tb/tb_style_edit_content.h"
#include "tb/tb_system.h"
#include "tb/tb_tab_container.h"
#include "tb/tb_tempbuffer.h"
#include "tb/tb_toggle_container.h"
#include "tb/tb_types.h"
#include "tb/tb_value.h"
#include "tb/tb_widgets.h"
#include "tb/tb_widgets_common.h"
#include "tb/tb_widgets_listener.h"
#include "tb/tb_widgets_reader.h"
#include "tb/tb_widget_skin_condition_context.h"
#include "tb/tb_widget_value.h"
#include "tb/tb_window.h"

// TBUILibrary Core
#include "TBUIWidget.hpp"
#include "TBUIRenderer.hpp"
#include "TBUIBitmap.hpp"
#include "TBUIImageLoader.hpp"
#include "TBUIFile.hpp"
#include "TBUIManager.hpp"