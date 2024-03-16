add_executable(ZeroLauncherShell)

zero_setup_library(ZeroLauncherShell ${CMAKE_CURRENT_LIST_DIR} TRUE)
zero_use_precompiled_header(ZeroLauncherShell ${CMAKE_CURRENT_LIST_DIR})

target_sources(ZeroLauncherShell
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/VirtualFileSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

target_link_libraries(ZeroLauncherShell
  PUBLIC
    Common
    Platform
    ZLib
    Support
)

add_dependencies(ZeroLauncherShell
  ZeroLauncher
)

zero_copy_from_linked_libraries(ZeroLauncherShell)
