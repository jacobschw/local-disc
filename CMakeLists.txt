cmake_minimum_required(VERSION 3.15) # Use a reasonably modern CMake

project(YourProjectName LANGUAGES CXX)

# --- Global Settings ---
set(CMAKE_CXX_STANDARD 20)              # Set C++ standard (e.g., C++20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)           # Prefer standard C++ over GNU extensions

# --- Build Type ---
# Common build types: Debug, Release, RelWithDebInfo, MinSizeRel
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

# --- Compiler Specific Settings ---
if(MSVC)
  # Add common warning flags for MSVC
  target_compile_options(local_disc_core_lib INTERFACE /W4) # INTERFACE applies to consumers if this was static
  # For DLLs on Windows, you often want MultiThreadedDLL runtime
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
  # Add common warning flags for GCC/Clang
  # We'll set these in cpp_core/CMakeLists.txt using target_compile_options
  # for more fine-grained control per target.
endif()

# --- Include Subdirectories ---
# This is where your actual library code will be defined and built
add_subdirectory(cpp_core)

# --- Optional: An executable for testing the library directly (good for early dev) ---
# You can uncomment this later if you want a small C++ program to test your library
# add_executable(core_tester main_test.cpp) # Assuming main_test.cpp is in your_project_name/
# target_link_libraries(core_tester PRIVATE local_disc_core_lib)
# message(STATUS "To build and run core_tester: cmake --build . --target core_tester && ./core_tester")