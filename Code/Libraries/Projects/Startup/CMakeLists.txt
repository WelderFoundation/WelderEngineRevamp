add_library(Startup)

welder_setup_library(Startup ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(Startup ${CMAKE_CURRENT_LIST_DIR})

target_sources(Startup
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Startup.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Startup.hpp
    ${CMAKE_CURRENT_LIST_DIR}/StartupStandard.hpp
)

target_link_libraries(Startup
  PUBLIC
    Common
    Engine
    Geometry
    Graphics
    Meta
    Physics
    Sound
    ZilchShaders
)
