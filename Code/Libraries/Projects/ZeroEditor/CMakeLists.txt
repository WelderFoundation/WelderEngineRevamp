add_executable(ZeroEditor)

zero_setup_library(ZeroEditor ${CMAKE_CURRENT_LIST_DIR} TRUE)
zero_use_precompiled_header(ZeroEditor ${CMAKE_CURRENT_LIST_DIR})

target_sources(ZeroEditor
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/VirtualFileSystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GameOrEditorStartup.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GameOrEditorStartup.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ZeroCrashCallbacks.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ZeroCrashCallbacks.hpp
)

target_link_libraries(ZeroEditor
  PUBLIC
    Assimp
    CodeTranslator
    Common
    Content
    Editor
    Engine
    FreeType
    Gameplay
    Geometry
    Graphics
    Image
    Libpng
    Meta
    Networking
    Nvtt
    Opus
    Physics
    Platform
    RendererBase
    RendererImpl
    Replication
    Scintilla
    Serialization
    Sound
    SpatialPartition
    SpirvCross
    SpirvHeaders
    SpirvTools
    Startup
    Support
    UiWidget
    Widget
    ZLib
    Zilch
    ZilchScript
    ZilchShaders
)

zero_copy_from_linked_libraries(ZeroEditor)

set_target_properties(ZeroEditor PROPERTIES ENABLE_EXPORTS TRUE)
set(platform_plugin_lib_dir ${ZERO_ZILCH_CUSTOM_PLUGIN_SHARED_DIR}${ZERO_TARGETOS}-${ZERO_ARCHITECTURE}/)
add_custom_command(TARGET ZeroEditor POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E make_directory ${platform_plugin_lib_dir})
add_custom_command(TARGET ZeroEditor POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy $<TARGET_LINKER_IMPORT_FILE:ZeroEditor> ${platform_plugin_lib_dir})
    
add_custom_command(TARGET ZeroEditor POST_BUILD COMMAND
    ${ZERO_ALL_TO_ONE_TOOL} -Zero ${ZERO_CODE_DIR} ${platform_plugin_lib_dir}Zilch.hpp)
