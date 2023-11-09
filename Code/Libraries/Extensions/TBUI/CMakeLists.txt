add_library(TBUI)

welder_setup_library(TBUI ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(TBUI ${CMAKE_CURRENT_LIST_DIR})

set(TBUI_TB_SOURCES_ANIMATION
    ${CMAKE_CURRENT_LIST_DIR}/tb/animation/tb_animation_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/animation/tb_animation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/animation/tb_animation.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/animation/tb_widget_animation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/animation/tb_widget_animation.h
)

set(TBUI_TB_SOURCES_IMAGE
    ${CMAKE_CURRENT_LIST_DIR}/tb/image/tb_image_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/image/tb_image_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/image/tb_image_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/image/tb_image_widget.h
)

set(TBUI_TB_SOURCES_PARSER
    ${CMAKE_CURRENT_LIST_DIR}/tb/parser/tb_parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/parser/tb_parser.h
)

set(TBUI_TB_SOURCES_RENDERERS
    ${CMAKE_CURRENT_LIST_DIR}/tb/renderers/tb_renderer_batcher.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/renderers/tb_renderer_batcher.h
#    ${CMAKE_CURRENT_LIST_DIR}/tb/renderers/tb_renderer_gl.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/tb/renderers/tb_renderer_gl.h
)

set(TBUI_TB_SOURCES_TESTS
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/tb_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/tb_test.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_color.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_dimension.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_geometry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_hashtable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_linklist.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_node_ref_tree.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_object.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_parser_definitions.tb.txt
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_parser_included.tb.txt
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_parser.tb.txt
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_space_allocator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_style_edit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_tempbuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_test.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_value.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tests/test_tb_widget_value.cpp
)

set(TBUI_TB_SOURCES_THIRDPARTY
    ${CMAKE_CURRENT_LIST_DIR}/tb/thirdparty/stb_image.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/thirdparty/stb_truetype.h
)

set(TBUI_TB_SOURCES_UTF8
    ${CMAKE_CURRENT_LIST_DIR}/tb/utf8/utf8.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/utf8/utf8.h
)

set(TBUI_TB_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_bitmap_fragment.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_bitmap_fragment.h
#    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_clipboard_dummy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_clipboard_glfw.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_clipboard_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_color.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_color.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_config.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_core.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_core.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_debug.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_debug.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_dimension.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_dimension.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_editfield.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_editfield.h
#    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_file_posix.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_font_desc.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_font_renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_font_renderer.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_font_renderer_freetype.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_font_renderer_stb.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_font_renderer_tbbf.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_geometry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_geometry.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_hash.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_hash.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_hashtable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_hashtable.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_id.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_id.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_image_loader_stb.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_inline_select.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_inline_select.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_language.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_language.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_layout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_layout.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_linklist.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_linklist.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_list.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_list.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_menu_window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_menu_window.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_message_window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_message_window.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_msg.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_msg.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_node_ref_tree.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_node_ref_tree.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_node_tree.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_node_tree.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_object.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_object.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_popup_window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_popup_window.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_renderer.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_scroller.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_scroller.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_scroll_container.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_scroll_container.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_select.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_select.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_select_item.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_select_item.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_skin.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_skin.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_skin_util.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_skin_util.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_sort.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_str.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_str.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_style_edit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_style_edit.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_style_edit_content.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_style_edit_content.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_system.h
#    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_system_android.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_system_linux.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_system_win.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_tab_container.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_tab_container.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_tempbuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_tempbuffer.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_toggle_container.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_toggle_container.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_types.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_value.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_value.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widgets.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widgets.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widgets_common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widgets_common.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widgets_listener.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widgets_listener.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widgets_reader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widgets_reader.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widget_skin_condition_context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widget_skin_condition_context.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widget_value.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_widget_value.h
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tb/tb_window.h
)

set(TBUI_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIBitmap.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIBitmap.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIClipboard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIFile.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIFile.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIImageLoader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIImageLoader.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIManager.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIRenderer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUISystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/TBUIWidget.hpp
)

target_sources(TBUI
  PRIVATE
    ${TBUI_TB_SOURCES_ANIMATION}
    ${TBUI_TB_SOURCES_IMAGE}
    ${TBUI_TB_SOURCES_PARSER}
    ${TBUI_TB_SOURCES_RENDERERS}
#    ${TBUI_TB_SOURCES_TESTS}
    ${TBUI_TB_SOURCES_THIRDPARTY}
    ${TBUI_TB_SOURCES_UTF8}
    ${TBUI_TB_SOURCES}
    ${TBUI_SOURCES}
)

target_link_libraries(TBUI
  PUBLIC
    Engine
    Graphics
    ZilchShaders
)

target_include_directories(TBUI
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/tb/
)
