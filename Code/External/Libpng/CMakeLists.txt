add_library(Libpng)

welder_setup_library(Libpng ${CMAKE_CURRENT_LIST_DIR} FALSE)

target_sources(Libpng
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Repo/png.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/png.h
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngconf.h
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngdebug.h
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngerror.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngget.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pnginfo.h
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngmem.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngpread.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngpriv.h
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngread.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngrio.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngrtran.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngrutil.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngset.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngstruct.h
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngtrans.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngwio.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngwrite.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngwtran.c
    ${CMAKE_CURRENT_LIST_DIR}/Repo/pngwutil.c
)

welder_target_includes(Libpng
  PUBLIC
    ZLib
)

target_include_directories(Libpng
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/Repo/
)
