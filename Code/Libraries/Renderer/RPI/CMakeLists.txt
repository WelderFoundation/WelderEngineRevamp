add_library(RPI)

welder_setup_library(RPI ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(RPI ${CMAKE_CURRENT_LIST_DIR})

target_sources(RPI
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/RPIStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RPIStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

target_link_libraries(RPI
  PUBLIC
    RHI
)