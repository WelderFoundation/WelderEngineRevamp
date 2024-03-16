cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

if (POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
  set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
endif ()

# Configure CMake global variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CONFIGURATION_TYPES Debug RelWithDebInfo Release MinSizeRel)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ZeroEditor)

find_program(ZERO_IWYU_PATH NAMES include-what-you-use iwyu)
if(ZERO_IWYU_PATH)
  message(STATUS "Found iwyu at ${ZERO_IWYU_PATH}")
endif()

find_program(ZERO_CCACHE_PATH ccache)
if(ZERO_CCACHE_PATH)
  message(STATUS "Found ccache at ${ZERO_CCACHE_PATH}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${ZERO_CCACHE_PATH}")
endif()

project(Zero)

add_definitions(-DZeroMsSinceEpoch=${ZERO_MS_SINCE_EPOCH})
add_definitions(-DZeroBranchName="${ZERO_BRANCH}")
add_definitions(-DZeroRevisionId=${ZERO_REVISION})
add_definitions(-DZeroShortChangeSet=${ZERO_SHORT_CHANGESET})
add_definitions(-DZeroChangeSet=${ZERO_CHANGESET})
add_definitions(-DZeroChangeSetDate=${ZERO_CHANGESET_DATE})
add_definitions(-DZeroChangeSetDate=${ZERO_CHANGESET_DATE})
add_definitions(-DZeroMajorVersion=${ZERO_MAJOR_VERSION})
add_definitions(-DZeroMinorVersion=${ZERO_MINOR_VERSION})
add_definitions(-DZeroPatchVersion=${ZERO_PATCH_VERSION})
add_definitions(-DZeroConfigName="${ZERO_CONFIG}")
add_definitions(-DZeroArchitectureName="${ZERO_ARCHITECTURE}")
add_definitions(-DZeroHostOs${ZERO_HOSTOS}=1)
add_definitions(-DZeroHostOsName="${ZERO_HOSTOS}")
add_definitions(-DZeroTargetOs${ZERO_TARGETOS}=1)
add_definitions(-DZeroTargetOsName="${ZERO_TARGETOS}")
add_definitions(-DZeroPlatform${ZERO_PLATFORM}=1)
add_definitions(-DZeroPlatformName="${ZERO_PLATFORM}")
add_definitions(-DZeroToolchain${ZERO_TOOLCHAIN}=1)
add_definitions(-DZeroToolchainName="${ZERO_TOOLCHAIN}")

option(ZERO_EXCEPTIONS "Enable exception throwing/catching" ON)
if (ZERO_EXCEPTIONS)
  add_definitions(-DZeroExceptions)
endif()

set(ZERO_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ZERO_CODE_DIR ${ZERO_ROOT_DIR}/Code/)
set(ZERO_CMAKE_DIR ${ZERO_ROOT_DIR}/CMakeFiles/)
set(ZERO_TOOLCHAIN_DIR ${ZERO_CMAKE_DIR}/Toolchain/)
set(ZERO_TARGETOS_DIR ${ZERO_CMAKE_DIR}/TargetOs/)
set(ZERO_PLATFORM_DATA_DIR ${ZERO_ROOT_DIR}/PlatformData/${ZERO_TOOLCHAIN})
set(ZERO_ZILCH_CUSTOM_PLUGIN_SHARED_DIR ${ZERO_ROOT_DIR}/Data/ZilchCustomPluginShared/)
set(ZERO_ALL_TO_ONE_TOOL ${ZERO_ROOT_DIR}/Tools/${ZERO_PLATFORM}/ZilchToOneCpp)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

function(zero_setup_library target directory internal)
  get_filename_component(PARENT_DIR ${directory} DIRECTORY)
  target_include_directories(${target} PUBLIC ${directory} ${PARENT_DIR})
  if (${internal})
    #set_property(TARGET ${target} PROPERTY C_INCLUDE_WHAT_YOU_USE ${ZERO_IWYU_PATH})
    #set_property(TARGET ${target} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${ZERO_IWYU_PATH})
  endif()
  zero_toolchain_setup_library(${target} ${internal})
  
  # globally enable sorting targets into folders in IDEs
  get_filename_component (PARENT_FOLDER ${directory} PATH)
  get_filename_component (FOLDER_NAME ${PARENT_FOLDER} NAME)
  set_property(TARGET ${target} PROPERTY FOLDER ${FOLDER_NAME})
endfunction()

function(zero_get_all_dependencies outputList TARGET)
    list(APPEND visitedTargets ${TARGET})
    get_target_property(dependencies ${TARGET} INTERFACE_LINK_LIBRARIES)
    get_target_property(targetType ${TARGET} TYPE)
    if (NOT targetType STREQUAL "INTERFACE_LIBRARY")
      get_target_property(manualDependencies ${TARGET} MANUALLY_ADDED_DEPENDENCIES)
      list(APPEND dependencies ${manualDependencies})
    endif()
    set(allLinkedLibs "")
    foreach(dependency ${dependencies})
        if (TARGET ${dependency})
            list(FIND visitedTargets ${dependency} VISITED)
            if (${VISITED} EQUAL -1)
                zero_get_all_dependencies(childLinkedLibs ${dependency})
                list(APPEND allLinkedLibs ${dependency} ${childLinkedLibs})
            endif()
        endif()
    endforeach()
    set(visitedTargets ${visitedTargets} PARENT_SCOPE)
    set(${outputList} ${allLinkedLibs} PARENT_SCOPE)
endfunction()

function(zero_copy_from_linked_libraries target)
  zero_get_all_dependencies(dependencies ${target})
  foreach(dependency ${dependencies})
    set(directory "${${dependency}CopyDirectories}")
    if(directory)
      add_custom_command(TARGET ${target} POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${directory} $<TARGET_FILE_DIR:${target}>)
    endif()
  endforeach()
endfunction()

function(zero_target_includes target)
  set(multiValueArgs PUBLIC)
  cmake_parse_arguments(ZERO "" "" "${multiValueArgs}" "${ARGN}")

  foreach(fromTarget ${ZERO_PUBLIC})
  target_include_directories(${target} PUBLIC $<TARGET_PROPERTY:${fromTarget},INTERFACE_INCLUDE_DIRECTORIES>)
  target_compile_definitions(${target} PUBLIC $<TARGET_PROPERTY:${fromTarget},INTERFACE_COMPILE_DEFINITIONS>)
  endforeach()
endfunction()

#Emscripten
#MSVC
include(${ZERO_TOOLCHAIN_DIR}/${ZERO_TOOLCHAIN}.cmake)
include(${ZERO_TARGETOS_DIR}/${ZERO_TARGETOS}.cmake)

set(CMAKE_C_FLAGS                             "${CMAKE_C_FLAGS}                             ${ZERO_C_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS                           "${CMAKE_CXX_FLAGS}                           ${ZERO_C_CXX_FLAGS}")

set(CMAKE_C_FLAGS_DEBUG                       "${CMAKE_C_FLAGS_DEBUG}                       ${ZERO_C_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_DEBUG                     "${CMAKE_CXX_FLAGS_DEBUG}                     ${ZERO_C_CXX_FLAGS_DEBUG}")

set(CMAKE_C_FLAGS_RELWITHDEBINFO              "${CMAKE_C_FLAGS_RELWITHDEBINFO}              ${ZERO_C_CXX_FLAGS_RELWITHDEBINFO}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO            "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}            ${ZERO_C_CXX_FLAGS_RELWITHDEBINFO}")

set(CMAKE_C_FLAGS_RELEASE                     "${CMAKE_C_FLAGS_RELEASE}                     ${ZERO_C_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELEASE                   "${CMAKE_CXX_FLAGS_RELEASE}                   ${ZERO_C_CXX_FLAGS_RELEASE}")

set(CMAKE_C_FLAGS_MINSIZEREL                  "${CMAKE_C_FLAGS_MINSIZEREL}                  ${ZERO_C_CXX_FLAGS_MINSIZEREL}")
set(CMAKE_CXX_FLAGS_MINSIZEREL                "${CMAKE_CXX_FLAGS_MINSIZEREL}                ${ZERO_C_CXX_FLAGS_MINSIZEREL}")

set(CMAKE_STATIC_LINKER_FLAGS                 "${CMAKE_STATIC_LINKER_FLAGS}                 ${ZERO_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS                 "${CMAKE_SHARED_LINKER_FLAGS}                 ${ZERO_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS                 "${CMAKE_MODULE_LINKER_FLAGS}                 ${ZERO_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS                    "${CMAKE_EXE_LINKER_FLAGS}                    ${ZERO_LINKER_FLAGS}")

set(CMAKE_STATIC_LINKER_FLAGS_DEBUG           "${CMAKE_STATIC_LINKER_FLAGS_DEBUG}           ${ZERO_LINKER_FLAGS_DEBUG}")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG           "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}           ${ZERO_LINKER_FLAGS_DEBUG}")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG           "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}           ${ZERO_LINKER_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG              "${CMAKE_EXE_LINKER_FLAGS_DEBUG}              ${ZERO_LINKER_FLAGS_DEBUG}")

set(CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO  "${CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO}  ${ZERO_LINKER_FLAGS_RELWITHDEBINFO}")
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO  "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO}  ${ZERO_LINKER_FLAGS_RELWITHDEBINFO}")
set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO  "${CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO}  ${ZERO_LINKER_FLAGS_RELWITHDEBINFO}")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO     "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO}     ${ZERO_LINKER_FLAGS_RELWITHDEBINFO}")

set(CMAKE_STATIC_LINKER_FLAGS_RELEASE         "${CMAKE_STATIC_LINKER_FLAGS_RELEASE}         ${ZERO_LINKER_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE         "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}         ${ZERO_LINKER_FLAGS_RELEASE}")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE         "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}         ${ZERO_LINKER_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE            "${CMAKE_EXE_LINKER_FLAGS_RELEASE}            ${ZERO_LINKER_FLAGS_RELEASE}")

set(CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL      "${CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL}      ${ZERO_LINKER_FLAGS_MINSIZEREL}")
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL      "${CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL}      ${ZERO_LINKER_FLAGS_MINSIZEREL}")
set(CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL      "${CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL}      ${ZERO_LINKER_FLAGS_MINSIZEREL}")
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL         "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL}         ${ZERO_LINKER_FLAGS_MINSIZEREL}")

#Windows
#Emscripten
#SDLSTDEmpty
#Stub
#add_subdirectory(Libraries/Platform/${ZERO_PLATFORM})

add_subdirectory(Code)
