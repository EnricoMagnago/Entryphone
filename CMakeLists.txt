cmake_minimum_required(VERSION 3.5)
project(Entryphone)

set(CMAKE_CXX_STANDARD 14)

# include Boost libraries
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# include threads
find_package (Threads)


# find zmq.
include(FindZMQ.cmake)

# find readline.
include (common/cpp_readline/FindReadline.cmake)

file(GLOB common_SRC
  "common/*.hpp"
  "common/*.cpp"
  "common/*.h"
  "common/*.c"
  "common/*/*.hpp"
  "common/*/*.cpp"
  "common/*/*.h"
  "common/*/*.c")

set(ENTRY_MAIN
  ${common_SRC}
  entrance/src/server/EntranceServer.cpp
  entrance/src/interface/EntranceServerDefinitions.cpp
  entrance/src/ConsoleInterface.cpp
  entrance/src/Algorithm.cpp
  entrance/src/entrance_main.cpp
  )

#set(APARTMENT_MAIN
#  ${common_SRC}
#  apartment/src/server/ApartmentServer.cpp
#  apartment/src/interface/ApartmentServerDefinitions.cpp
#  Algorithm.cpp
#  apartment_main.cpp)

add_executable(Entryphone_entry ${ENTRY_MAIN})
#add_executable(Entryphone_apartment ${APARTMENT_MAIN})

## add the include directory to our compile directives
target_include_directories(Entryphone_entry PUBLIC ${ZMQ_INCLUDE_DIR} ${Readline_INCLUDE_DIR})
#target_include_directories(Entryphone_apartment PUBLIC ${ZMQ_INCLUDE_DIR})

## at the 0mq library to our link directive
target_link_libraries(Entryphone_entry PUBLIC ${ZMQ_LIBRARIES} ${Readline_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(Entryphone_apartment PUBLIC ${ZMQ_LIBRARIES})
