add_executable(WelderEditor)

welder_setup_library(WelderEditor ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(WelderEditor ${CMAKE_CURRENT_LIST_DIR})

target_sources(WelderEditor
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

target_link_libraries(WelderEditor
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

welder_copy_from_linked_libraries(WelderEditor)

set_target_properties(WelderEditor PROPERTIES ENABLE_EXPORTS TRUE)
set(platform_plugin_lib_dir ${WELDER_ZILCH_CUSTOM_PLUGIN_SHARED_DIR}${WELDER_TARGETOS}-${WELDER_ARCHITECTURE}/)
add_custom_command(TARGET WelderEditor POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E make_directory ${platform_plugin_lib_dir})
add_custom_command(TARGET WelderEditor POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy $<TARGET_LINKER_IMPORT_FILE:WelderEditor> ${platform_plugin_lib_dir})
    
add_custom_command(TARGET WelderEditor POST_BUILD COMMAND
    ${WELDER_ALL_TO_ONE_TOOL} -Zero ${WELDER_CODE_DIR} ${platform_plugin_lib_dir}Zilch.hpp)
