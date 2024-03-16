add_subdirectory(RendererBase)

#Stub
#Emscripten
#SDLSTDEmpty
#Windows
#Linux

add_library(RendererImpl INTERFACE)

if(${ZERO_PLATFORM} STREQUAL "Windows" OR ${ZERO_PLATFORM} STREQUAL "Emscripten" OR ${ZERO_PLATFORM} STREQUAL "SDLSTDEmpty" OR ${ZERO_PLATFORM} STREQUAL "Linux")
  add_subdirectory(RendererGL)
  target_link_libraries(RendererImpl INTERFACE RendererGL)
elseif(${ZERO_PLATFORM} STREQUAL "Stub")
  add_subdirectory(RendererEmpty)
  target_link_libraries(RendererImpl INTERFACE RendererEmpty)
else()
  message(FATAL_ERROR "No renderer available for target '${ZERO_TARGETOS}'.")
endif()
