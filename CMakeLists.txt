cmake_minimum_required(VERSION 3.12)
project(colcon_submodules_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "" FORCE)


include(cmake/UpdateSubmodules.cmake)


find_package(Vulkan)
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR 
        "Vulkan SDK not found! \n"
        "Please install it using: sudo apt install libvulkan-dev vulkan-tools")
endif()

set(KOMPUTE_OPT_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(KOMPUTE_OPT_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(KOMPUTE_OPT_LOG_LEVEL Off CACHE STRING "" FORCE)
set(KOMPUTE_OPT_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external)

# find dependencies
set(DEPENDENCIES
  ament_cmake
  ament_index_cpp
  rclcpp
  rclcpp_components
)

foreach(DEP IN LISTS DEPENDENCIES)
  find_package(${DEP} REQUIRED)
endforeach()

## --------------------------------------------------------------
## |                       compile                              |
## --------------------------------------------------------------


add_executable(kompute_test src/kompute_test.cpp)

# This is the correct way to attach ROS 2 deps:
ament_target_dependencies(kompute_test
  rclcpp
  rclcpp_components
  ament_index_cpp
)

# Your non-ROS libs:
target_link_libraries(kompute_test
    kompute
    Vulkan::Vulkan
)


## --------------------------------------------------------------
## |                           install                          |
## --------------------------------------------------------------
install(
  DIRECTORY include
  DESTINATION include/${PROJECT_NAME}
)

install(TARGETS
    kompute_test
  DESTINATION lib/${PROJECT_NAME}
)


ament_package()