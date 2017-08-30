cmake_minimum_required(VERSION 3.5)

project(Entryphone)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)


# CMake does not support c++17 yet, force compilation with -std=c++17
# -g : debug
# other flags to make compiler complain about "everything".
#set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -Wpedantic -Wchkp -Wformat -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wnonnull-compare -Wignored-attributes -Wmisleading-indentation -Wmissing-include-dirs -Wuninitialized -Wstrict-overflow=5 -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -Wbool-compare -Wduplicated-cond -Wtautological-compare -Wfloat-equal -Wshadow -Wunused-macros -Wundef -Wcast-qual -Wcast-align -Wdelete-incomplete -Wuseless-cast -Wc++-compat -Wconversion -Wlogical-op -Waggregate-return -Wmissing-field-initializers -Wredundant-decls -Winline")

set(CMAKE_CXX_FLAGS "-std=c++14 -g")

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include Boost libraries
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# include threads
find_package(Threads REQUIRED)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)


# find wiringPi
#include(FindWiringPi.cmake)

# find zmq.
include(FindZMQ.cmake)

# find readline.
include(common/cpp_readline/FindReadline.cmake)

file(GLOB common_SRC
                "common/*.cpp"
                "common/*.c"
                "common/*/*.cpp"
                "common/*/*.c"
                apartment/src/interface/ApartmentServerDefinitions.cpp
                )

set(ENTRY_MAIN
                ${common_SRC}
                entrance/src/HwMgr/HardwareManager.cpp
                entrance/src/interface/EntranceServerDefinitions.cpp
                entrance/src/Algorithm.cpp
                entrance/src/ConsoleInterface.cpp
                entrance/src/server/EntranceServer.cpp
                apartment/src/client/ApartmentClient.cpp
                entrance/src/entrance_main.cpp
                entrance/src/gui/mainwindow.cpp
                )

set(APARTMENT_MAIN
                ${common_SRC}
                apartment/src/Algorithm.cpp
                apartment/src/ConsoleInterface.cpp
                apartment/src/server/ApartmentServer.cpp
                apartment/src/apartment_main.cpp
                )

add_executable(Entryphone_entry ${ENTRY_MAIN})
add_executable(Entryphone_apartment ${APARTMENT_MAIN})

## add the include directory to our compile directives
target_include_directories(Entryphone_entry PUBLIC
  ${ZMQ_INCLUDE_DIR} ${Readline_INCLUDE_DIR}
  ${Qt5Widgets_INCLUDE_DIRS})

target_include_directories(Entryphone_apartment PUBLIC
  ${ZMQ_INCLUDE_DIR} ${Readline_INCLUDE_DIR}
  ${Qt5Widgets_INCLUDE_DIRS})

## at the 0mq library to our link directive
target_link_libraries(Entryphone_entry PUBLIC
  ${ZMQ_LIBRARIES} ${Readline_LIBRARY} ${CMAKE_THREAD_LIBS_INIT}
  Qt5::Widgets Qt5::Core) # ${WIRINGPI_LIBRARIES}

target_link_libraries(Entryphone_apartment PUBLIC
  ${ZMQ_LIBRARIES} ${Readline_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
