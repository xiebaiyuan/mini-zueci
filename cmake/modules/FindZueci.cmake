# - Find Zueci
# Find the native Zueci includes and library
#
#  ZUECI_INCLUDE_DIR - where to find zueci.h, etc.
#  ZUECI_LIBRARIES   - List of libraries when using zueci.
#  ZUECI_FOUND       - True if zueci found.

################### FIND ZUECI ######################

if(ZUECI_INCLUDE_DIR)
    # Already in cache, be silent
    set(ZUECI_FIND_QUIETLY TRUE)
endif()

find_path(ZUECI_INCLUDE_DIR zueci.h)

find_library(ZUECI_LIBRARY NAMES zueci)

# handle the QUIETLY and REQUIRED arguments and set ZUECI_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Zueci DEFAULT_MSG ZUECI_LIBRARY ZUECI_INCLUDE_DIR)

if(ZUECI_FOUND)
    set(ZUECI_LIBRARIES ${ZUECI_LIBRARY})
else()
    set(ZUECI_LIBRARIES)
endif()

mark_as_advanced(ZUECI_LIBRARY ZUECI_INCLUDE_DIR)
