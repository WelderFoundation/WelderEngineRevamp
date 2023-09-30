if(NOT EMSCRIPTEN)
  set(BUILD_CLI OFF CACHE BOOL "")
  set(BUILD_EXAMPLES OFF CACHE BOOL "")
  set(BUILD_FUZZERS OFF CACHE BOOL "")
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
  set(BUILD_TESTS OFF CACHE BOOL "")

  add_subdirectory(Repo)

  add_library(Libgit2 INTERFACE)

  target_link_libraries(Libgit2
    INTERFACE
      libgit2package
  )
  
  target_include_directories(Libgit2
    INTERFACE
      ${CMAKE_CURRENT_LIST_DIR}/Repo/include/
  )
  
  if(WIN32)
	set_property(TARGET "http-parser" PROPERTY FOLDER "External/Libgit2")
	set_property(TARGET "Libgit2" PROPERTY FOLDER "External/Libgit2")
	set_property(TARGET "libgit2" PROPERTY FOLDER "External/Libgit2")
	set_property(TARGET "libgit2package" PROPERTY FOLDER "External/Libgit2")
	set_property(TARGET "pcre" PROPERTY FOLDER "External/Libgit2")
	set_property(TARGET "util" PROPERTY FOLDER "External/Libgit2")
	set_property(TARGET "zlib" PROPERTY FOLDER "External/Libgit2")
  endif()
endif()
