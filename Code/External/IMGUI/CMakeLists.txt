add_library(IMGUI)

welder_setup_library(IMGUI ${CMAKE_CURRENT_LIST_DIR} FALSE)
welder_use_precompiled_header(IMGUI ${CMAKE_CURRENT_LIST_DIR})

target_sources(IMGUI
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Repo/imconfig.h
    ${CMAKE_CURRENT_LIST_DIR}/Repo/imgui.h
    ${CMAKE_CURRENT_LIST_DIR}/Repo/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Repo/imgui_demo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Repo/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Repo/imgui_internal.h
    ${CMAKE_CURRENT_LIST_DIR}/Repo/imgui_tables.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Repo/imgui_widgets.cpp

    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

target_include_directories(IMGUI
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Repo/
)