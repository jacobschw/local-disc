# --- Library Target ---
# This will be local_disc_core_lib.dll on Windows, liblocal_disc_core_lib.so on Linux, liblocal_disc_core_lib.dylib on macOS
# CMake handles the lib prefix and extension automatically based on platform.
add_library(local_disc_core_lib SHARED) # SHARED makes it a DLL/SO/DYLIB

# --- Library Properties ---
set_target_properties(local_disc_core_lib PROPERTIES
    OUTPUT_NAME "local_disc_core_lib" # Explicitly set output name if different from target name
    VERSION "0.1.0"
    SOVERSION "0"
    PUBLIC_HEADER "include/local_disc/api.h" # For installation
)

# --- Source Files ---
target_sources(local_disc_core_lib PRIVATE
    src/api.cpp
    src/engine.cpp
    # Add other .cpp files here as you create them
)

# --- Include Directories ---
# Public: For headers that consumers of this library (like your Rust/Go server) need.
# Private: For internal headers or dependencies not exposed.
target_include_directories(local_disc_core_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> # Headers for consuming targets (and for export)
        $<INSTALL_INTERFACE:include>                          # Path after installation
    PRIVATE
        src # If you have internal headers in src/ that api.cpp needs, but not public
)

# --- Preprocessor Definitions for Exporting/Importing Symbols ---
# This is crucial for shared libraries, especially on Windows.
# We define LOCAL_DISC_LIB_EXPORTS when building this library.
# The api.h header will use this to dllexport/dllimport symbols.
target_compile_definitions(local_disc_core_lib PRIVATE LOCAL_DISC_LIB_EXPORTS)

# --- Compiler Flags (more specific to this target) ---
if(MSVC)
    target_compile_options(local_disc_core_lib PRIVATE /W4 /WX- ) # /WX- to not treat all warnings as errors initially
    # Enable generation of PDB files for debugging
    target_compile_options(local_disc_core_lib PRIVATE "$<$<CONFIG:Debug>:/Zi>" "$<$<NOT:$<CONFIG:Debug>>:/Oy- /Zi>") # /Oy- disables frame pointer omission in Release for better stack traces
    target_link_options(local_disc_core_lib PRIVATE "$<$<CONFIG:Debug>:/DEBUG>" "$<$<NOT:$<CONFIG:Debug>>:/DEBUG:NONE>")
else() # GCC/Clang
    target_compile_options(local_disc_core_lib PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -fPIC # Position Independent Code, essential for shared libraries
    )
endif()

# --- Link Libraries (Dependencies) ---
# Example for later when you add libpqxx (assuming you use vcpkg or find_package)
# find_package(PostgreSQL REQUIRED)
# find_package(libpqxx REQUIRED)
# target_link_libraries(local_disc_core_lib PRIVATE PkgConfig::libpqxx) # Or however your package manager provides it

# --- Installation ---
# Defines where to install the library and its public headers.
# This is useful if you want to "install" your library system-wide or to a specific prefix.
# For FFI with Rust/Go, you'll typically point them to the build output directory.
include(GNUInstallDirs) # Provides standard installation directories like CMAKE_INSTALL_LIBDIR

install(TARGETS local_disc_core_lib
    EXPORT LocalDiscTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib # For .lib on Windows
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/local_disc COMPONENT dev
)

install(FILES include/local_disc/api.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/local_disc
    COMPONENT dev
)

# Generate an export file for CMake to allow other CMake projects to easily find and use this library
install(EXPORT LocalDiscTargets
  FILE
    LocalDiscTargets.cmake
  NAMESPACE
    YourProjectName::
  DESTINATION
    lib/cmake/YourProjectName
)

# Generate a config file for CMake (LocalDiscConfig.cmake)
# This helps other CMake projects find your library using find_package(YourProjectName)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/LocalDiscConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
  "Config.cmake.in" # You'll create this file
  "${CMAKE_CURRENT_BINARY_DIR}/LocalDiscConfig.cmake"
  INSTALL_DESTINATION lib/cmake/YourProjectName
)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/LocalDiscConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/LocalDiscConfigVersion.cmake"
  DESTINATION lib/cmake/YourProjectName
)

# --- Output directory ---
# By default, libraries go to cpp_core/build (or similar)
# You can customize this if needed, e.g.:
# set_target_properties(local_disc_core_lib PROPERTIES
#     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#     LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
# )