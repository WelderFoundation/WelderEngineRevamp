add_library(UiWidget)

welder_setup_library(UiWidget ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(UiWidget ${CMAKE_CURRENT_LIST_DIR})

target_sources(UiWidget
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UiDockLayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UiDockLayout.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UiFillLayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UiFillLayout.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UiLayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UiLayout.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UiRootWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UiRootWidget.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UiStackLayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UiStackLayout.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UiWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UiWidget.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UiWidgetEvents.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UiWidgetEvents.hpp
    ${CMAKE_CURRENT_LIST_DIR}/UiWidgetStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UiWidgetStandard.hpp
)

target_link_libraries(UiWidget
  PUBLIC
    Engine
    Graphics
    ZilchShaders
)
