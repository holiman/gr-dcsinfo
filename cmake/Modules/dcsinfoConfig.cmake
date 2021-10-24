INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_DCSINFO dcsinfo)

FIND_PATH(
    DCSINFO_INCLUDE_DIRS
    NAMES dcsinfo/api.h
    HINTS $ENV{DCSINFO_DIR}/include
        ${PC_DCSINFO_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DCSINFO_LIBRARIES
    NAMES gnuradio-dcsinfo
    HINTS $ENV{DCSINFO_DIR}/lib
        ${PC_DCSINFO_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DCSINFO DEFAULT_MSG DCSINFO_LIBRARIES DCSINFO_INCLUDE_DIRS)
MARK_AS_ADVANCED(DCSINFO_LIBRARIES DCSINFO_INCLUDE_DIRS)

