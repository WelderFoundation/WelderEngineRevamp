add_library(RHIVulkan)

welder_setup_library(RHIVulkan ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(RHIVulkan ${CMAKE_CURRENT_LIST_DIR})

target_sources(RHIVulkan
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/RHIVulkanStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RHIVulkanStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

target_link_libraries(RHIVulkan
  PUBLIC
    RHI
)