# add FindWiringPi.cmake in /usr/share/cmake-3.6/Modules on device, with content:
## find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
## find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h)
## include(FindPackageHandleStandardArgs)
## find_package_handle_standard_args(wiringPi DEFAULT_MSG WIRINGPI_LIBRARIES WIRINGPI_INCLUDE_DIRS)

#######

# Locate libraries and headers
find_package(WiringPi REQUIRED)
find_package(Threads REQUIRED)

# Include headers
include_directories(${WIRINGPI_INCLUDE_DIRS})

# Link against libraries
target_link_libraries(<yourProjectName> ${WIRINGPI_LIBRARIES})
target_link_libraries(<yourProjectName> ${CMAKE_THREAD_LIBS_INIT})