add_library(RHI)

welder_setup_library(RHI ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(RHI ${CMAKE_CURRENT_LIST_DIR})

target_sources(RHI
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/RHIStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHIStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

target_link_libraries(RHI
  PUBLIC
    Common
)