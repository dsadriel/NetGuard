cmake_minimum_required(VERSION 3.10)

project(NetGuard)

set(SOURCES
  src/main.cpp
  src/collisions.cpp
  src/utils/text_rendering.cpp
  src/utils/model_utils.cpp
  src/utils/texture_utils.cpp
  src/core/glad.c
  src/utils/stb_image.cpp
  src/core/shader_loader.cpp
  src/input/mouse_keyboard_callbacks.cpp
  src/input/window_callbacks.cpp
  src/matrices.cpp
  src/utils/obj_loader_utils.cpp
  include/external/tinyObjLoader/tiny_obj_loader.cpp
)

set(CMAKE_CXX_STANDARD          11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

if(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/bin/Debug")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/Release")
elseif(UNIX AND NOT APPLE)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/Linux")
elseif(APPLE)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/macOS")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS
  "Build type: ${CMAKE_BUILD_TYPE}

               Change the build type on the command line with

                   -DCMAKE_BUILD_TYPE=type

               for type in {Release, Debug, RelWithDebInfo}.
")

set(EXECUTABLE_NAME main)

# Verifies if all source files are present in the current directory.
# If not, it warns about a misconfigured CMakeLists.
foreach(source_file IN LISTS SOURCES)
  if(NOT EXISTS ${PROJECT_SOURCE_DIR}/${source_file})
    message(FATAL_ERROR "
The file ${PROJECT_SOURCE_DIR}/${source_file} does not exist.
Please, update the source file list in the CMakeLists.txt file.")
    break()
  endif()
endforeach()

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} BEFORE PRIVATE ${PROJECT_SOURCE_DIR}/include)

if(WIN32)

  if(MINGW)

    # Here we try to figure out which Windows libc is being used
    # by the MinGW compiler: msvcrt (old) or ucrt (new). We also
    # differentiate between a 32-bit (old) or 64-bit compiler.
    # For this, we search for the occurrence of some specific strings
    # in the output of the "-v" command of GCC, which lists the
    # compiler's configuration parameters.
    # TODO: Test with llvm/clang compiler.
    execute_process(
      COMMAND ${CMAKE_CXX_COMPILER} "-v"
      ERROR_VARIABLE  COMPILER_VERSION_OUTPUT
      RESULT_VARIABLE COMPILER_VERSION_RESULT
    )

    if (COMPILER_VERSION_RESULT EQUAL 0)
      # NOTE: It is important that the first test is to search for the
      # ucrt64 string in the compiler output, as the "mingw64"
      # string always appears in the output (even when ucrt64 is the libc used).
      if (COMPILER_VERSION_OUTPUT MATCHES "ucrt64")
        set(LIBGLFW ${PROJECT_SOURCE_DIR}/lib/lib-ucrt-64/libglfw3.a)
      elseif (COMPILER_VERSION_OUTPUT MATCHES "mingw64")
        set(LIBGLFW ${PROJECT_SOURCE_DIR}/lib/lib-mingw-64/libglfw3.a)
      else()
        set(LIBGLFW ${PROJECT_SOURCE_DIR}/lib/lib-mingw-32/libglfw3.a)
      endif()
    else()
      message(FATAL_ERROR "Failed to get MinGW compiler version.")
    endif()

  elseif(MSVC)
    set(LIBGLFW ${PROJECT_SOURCE_DIR}/lib/lib-vc2022/glfw3.lib)
  else()
    message(FATAL_ERROR "This CMakeLists.txt file only supports MINGW or MSVC toolchain on Windows.")
  endif()

  message(STATUS "LIBGLFW = ${LIBGLFW}")

  target_link_libraries(${EXECUTABLE_NAME} ${LIBGLFW} gdi32 opengl32)

elseif(APPLE)

  # Use find_package to locate the GLFW library automatically.
  # This is more robust than hardcoding paths.
  # Ensure you have installed glfw (e.g., via 'brew install glfw')
  find_package(glfw3 3.3 REQUIRED)

  # For macOS, we need to link against frameworks.
  # OpenGL is a framework on macOS.
  find_package(OpenGL REQUIRED)

  # Add custom target for 'run'
  add_custom_target(run
      COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ./main
      DEPENDS main
      USES_TERMINAL
  )
  
  target_link_libraries(${EXECUTABLE_NAME}
    # Link to the glfw target found by find_package
    glfw
    ${OPENGL_LIBRARIES}
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreFoundation"
  )

elseif(UNIX)

  target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wno-unused-function)

  # Add custom target for 'run'
  add_custom_target(run
      COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ./main
      DEPENDS main
      USES_TERMINAL
  )

  find_package(OpenGL REQUIRED)
  find_package(X11 REQUIRED)
  find_library(MATH_LIBRARY m)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  target_link_libraries(${EXECUTABLE_NAME}
    ${CMAKE_DL_LIBS}
    ${MATH_LIBRARY}
    ${PROJECT_SOURCE_DIR}/lib/lib-linux/libglfw3.a
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENGL_LIBRARIES}
    ${X11_LIBRARIES}
    ${X11_Xrandr_LIB}
    ${X11_Xcursor_LIB}
    ${X11_Xinerama_LIB}
    ${X11_Xxf86vm_LIB}
  )

endif()