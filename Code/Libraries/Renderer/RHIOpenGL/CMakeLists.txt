add_library(RHIOpenGL)

welder_setup_library(RHIOpenGL ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(RHIOpenGL ${CMAKE_CURRENT_LIST_DIR})

target_sources(RHIOpenGL
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/RHIOpenGLStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHIOpenGLStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

target_link_libraries(RHIOpenGL
  PUBLIC
    RHI
)