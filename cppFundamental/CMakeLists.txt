# ==============================================================================
# CMakeLists.txt — Project management with CMake
# ==============================================================================
#
# CMake is the industry-standard build system for C++.
# It generates platform-specific build files (Makefiles on Linux, .sln on Windows).
#
# USAGE:
#   mkdir build && cd build
#   cmake ..          ← configure (reads this file)
#   make -j4          ← compile with 4 threads
#   ./hello_world     ← run an executable
#
# STRUCTURE:
#   project_root/
#   ├── CMakeLists.txt       ← this file
#   ├── src/
#   │   ├── main.cpp
#   │   └── mathutils.cpp
#   ├── include/
#   │   └── mathutils.h
#   └── build/               ← generated, never commit this
# ==============================================================================

cmake_minimum_required(VERSION 3.16)      # minimum CMake version required

project(
    CppFundamentals                        # project name
    VERSION 1.0.0
    DESCRIPTION "C++ learning project"
    LANGUAGES CXX                          # C++ (could add CUDA here too)
)

# ── C++ standard ──────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)               # use C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)      # error if compiler doesn't support it
set(CMAKE_CXX_EXTENSIONS OFF)            # disable GNU extensions — portable

# ── Compiler warnings (best practice — catch bugs early) ─────────────────────
add_compile_options(-Wall -Wextra -Wpedantic)

# ── Where to find headers ─────────────────────────────────────────────────────
include_directories(${CMAKE_SOURCE_DIR}/include)

# ── Executables ───────────────────────────────────────────────────────────────
# add_executable(<target_name> <source_files...>)

add_executable(hello_world
    src/01_hello_world.cpp
)

add_executable(variables
    src/02_variables.cpp
)

add_executable(data_structures
    src/03_data_structures.cpp
)

add_executable(classes
    src/04_classes.cpp
)

add_executable(imports
    src/05_imports_headers.cpp
    src/mathutils.cpp
)

# ── Static library (reusable code shared by multiple executables) ─────────────
add_library(mathutils STATIC          # STATIC = .a archive linked into binary
    src/mathutils.cpp
)
target_include_directories(mathutils PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Link the 'imports' executable against our library
target_link_libraries(imports PRIVATE mathutils)

# ── Optional: SQLite3 external package ───────────────────────────────────────
# find_package(SQLite3 REQUIRED)
# target_link_libraries(database PRIVATE SQLite::SQLite3)

# ── Build type options ───────────────────────────────────────────────────────
# cmake -DCMAKE_BUILD_TYPE=Release ..   → optimised (-O3)
# cmake -DCMAKE_BUILD_TYPE=Debug   ..   → debug symbols (-g)
# cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..  → optimised + debug info

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in DEBUG mode")
    add_compile_options(-g -O0 -fsanitize=address)   # AddressSanitizer
    add_link_options(-fsanitize=address)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in RELEASE mode")
    add_compile_options(-O3 -DNDEBUG)
endif()

# ── Install rules (optional — for system-wide install) ────────────────────────
install(TARGETS hello_world DESTINATION bin)
install(TARGETS mathutils   DESTINATION lib)
install(FILES include/mathutils.h DESTINATION include)

# ── Enable CTest (unit testing) ───────────────────────────────────────────────
enable_testing()
# add_test(NAME my_test COMMAND my_test_executable)

# ==============================================================================
# OTHER USEFUL CMAKE COMMANDS
#
#   message(STATUS "info")          → print during cmake configure
#   option(ENABLE_FOO "desc" OFF)   → -DENABLE_FOO=ON toggle
#   if(UNIX) / if(WIN32) / if(APPLE) → platform checks
#   find_library(...)               → find system libraries
#   target_compile_definitions(target PRIVATE FOO=1)  → add -DFOO=1
# ==============================================================================
