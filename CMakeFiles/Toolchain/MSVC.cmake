add_definitions(-DZeroCompilerMsvc=1 -DZeroCompilerName="Msvc")

if (ZERO_EXCEPTIONS)
  set(MSVC_EXCEPTION_OPTION "-EHsc")
else()
  set(MSVC_EXCEPTION_OPTION "-EHs-c-")
endif()

set(ZERO_C_CXX_FLAGS "\
  -W3\
  -Zc:wchar_t\
  -Zc:inline\
  -Zc:forScope\
  -fp:fast\
  -errorReport:prompt\
  -Gd\
  ${MSVC_EXCEPTION_OPTION}\
  -nologo\
  -analyze-\
  -bigobj\
")

set(ZERO_C_CXX_FLAGS_DEBUG "\
  -Zi\
  -MDd\
  -GS\
  -Od\
  -Ob0\
  -Oy-\
")

set(ZERO_C_CXX_FLAGS_RELWITHDEBINFO "\
  -Zi\
  -MT\
  -MP\
  -GS\
  -O2\
  -Oi\
  -Oy-\
")

set(ZERO_C_CXX_FLAGS_RELEASE "\
  -MT\
  -MP\
  -GL\
  -GS-\
  -O2\
  -Oi\
")

set(ZERO_C_CXX_FLAGS_MINSIZEREL "\
  -MT\
  -MP\
  -GL\
  -GS-\
  -O1\
")

set(ZERO_LINKER_FLAGS "/ignore:4099,4221,4075,4251")
set(ZERO_LINKER_FLAGS_RELEASE "/LTCG")

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO /SUBSYSTEM:WINDOWS /STACK:8388608")

set(ZERO_C_CXX_EXTERNAL_FLAGS /W0 /wd4267)

function(zero_toolchain_setup_library target internal)
    set(TARGET_CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO /STACK:8388608")
    if (${internal})
        set(TARGET_CMAKE_EXE_LINKER_FLAGS "${TARGET_CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    endif()
    set_target_properties(${target} PROPERTIES LINK_FLAGS "${TARGET_CMAKE_EXE_LINKER_FLAGS}")
endfunction()

function(zero_use_precompiled_header target directory)
  target_compile_options(${target} PRIVATE "/FIPrecompiled.hpp")
  set_source_files_properties(${directory}/Precompiled.cpp PROPERTIES COMPILE_FLAGS "/YcPrecompiled.hpp")
  set_target_properties(${target} PROPERTIES COMPILE_FLAGS "/YuPrecompiled.hpp")
endfunction()

function(zero_source_ignore_precompiled_header source)
  set_source_files_properties(${source} PROPERTIES COMPILE_FLAGS "/Y-")
endfunction()
