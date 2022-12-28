add_library(Image)

welder_setup_library(Image ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(Image ${CMAKE_CURRENT_LIST_DIR})

target_sources(Image
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/HdrSupport.cpp
    ${CMAKE_CURRENT_LIST_DIR}/HdrSupport.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ImageSupport.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ImageSupport.hpp
    ${CMAKE_CURRENT_LIST_DIR}/PngSupport.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PngSupport.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ImageStandard.hpp
)

welder_target_includes(Image
  PUBLIC
    Common
    RendererBase
    Libpng
    ZLib
)
