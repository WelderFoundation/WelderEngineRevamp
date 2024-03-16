add_library(RendererGL)

zero_setup_library(RendererGL ${CMAKE_CURRENT_LIST_DIR} TRUE)
zero_use_precompiled_header(RendererGL ${CMAKE_CURRENT_LIST_DIR})

#Emscripten
#SDLSTDEmpty
#Windows
#Linux

if(${ZERO_TARGETOS} STREQUAL "Windows")
  if(${ZERO_PLATFORM} STREQUAL "SDLSTDEmpty")
    set(RendererPlatformSource 
      ${CMAKE_CURRENT_LIST_DIR}/SDLSTDEmpty/OpenglRendererSDL.cpp
    )
  else()
    set(RendererPlatformSource 
      ${CMAKE_CURRENT_LIST_DIR}/Windows/OpenglRendererWindows.cpp
    )
  endif()
elseif(${ZERO_TARGETOS} STREQUAL "Emscripten")
  set(RendererPlatformSource 
    ${CMAKE_CURRENT_LIST_DIR}/Emscripten/OpenglRendererEmscripten.cpp
  )
elseif(${ZERO_TARGETOS} STREQUAL "Linux")
  set(RendererPlatformSource 
    ${CMAKE_CURRENT_LIST_DIR}/SDLSTDEmpty/OpenglRendererSDL.cpp
  )
else()
  message(FATAL_ERROR "No renderer available for target '${ZERO_TARGETOS}'.")
endif()

target_sources(RendererGL
  PRIVATE
    ${RendererPlatformSource}
    ${CMAKE_CURRENT_LIST_DIR}/OpenglRenderer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/OpenglRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RendererGLStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RendererGLStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

target_link_libraries(RendererGL
  PUBLIC
    RendererBase
)
target_link_libraries(RendererGL
  PUBLIC
    GL
    Glew
)

if(NOT ${ZERO_TARGETOS} STREQUAL "Windows" OR ${ZERO_PLATFORM} STREQUAL "SDLSTDEmpty")
  target_link_libraries(RendererGL
    PUBLIC
      SDL
  )
endif()
