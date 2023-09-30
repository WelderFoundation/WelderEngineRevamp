add_library(RendererEmpty)

welder_setup_library(RendererEmpty ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(RendererEmpty ${CMAKE_CURRENT_LIST_DIR})

target_sources(RendererEmpty
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Renderer.cpp    
    ${CMAKE_CURRENT_LIST_DIR}/RendererEmptyStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RendererEmptyStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

welder_target_includes(RendererEmpty
  PUBLIC
    RendererBase
)
target_link_libraries(RendererEmpty
  PUBLIC
    RendererBase
)