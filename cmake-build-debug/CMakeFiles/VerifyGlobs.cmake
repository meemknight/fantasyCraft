# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.17
cmake_policy(SET CMP0009 NEW)

# MY_SOURCES at CMakeLists.txt:22 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/mihui/Dev/fantasyCraft/src/*.cpp")
set(OLD_GLOB
  "/home/mihui/Dev/fantasyCraft/src/block.cpp"
  "/home/mihui/Dev/fantasyCraft/src/camera.cpp"
  "/home/mihui/Dev/fantasyCraft/src/chunks.cpp"
  "/home/mihui/Dev/fantasyCraft/src/game.cpp"
  "/home/mihui/Dev/fantasyCraft/src/headerOnly.cpp"
  "/home/mihui/Dev/fantasyCraft/src/main.cpp"
  "/home/mihui/Dev/fantasyCraft/src/renderer.cpp"
  "/home/mihui/Dev/fantasyCraft/src/shader.cpp"
  "/home/mihui/Dev/fantasyCraft/src/texture.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/mihui/Dev/fantasyCraft/cmake-build-debug/CMakeFiles/cmake.verify_globs")
endif()

# MY_SOURCES at thirdparty/FastNoiseSIMD/CMakeLists.txt:6 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/mihui/Dev/fantasyCraft/thirdparty/FastNoiseSIMD/FastNoiseSIMD/*.cpp")
set(OLD_GLOB
  "/home/mihui/Dev/fantasyCraft/thirdparty/FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD.cpp"
  "/home/mihui/Dev/fantasyCraft/thirdparty/FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_avx2.cpp"
  "/home/mihui/Dev/fantasyCraft/thirdparty/FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_avx512.cpp"
  "/home/mihui/Dev/fantasyCraft/thirdparty/FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_internal.cpp"
  "/home/mihui/Dev/fantasyCraft/thirdparty/FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_neon.cpp"
  "/home/mihui/Dev/fantasyCraft/thirdparty/FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_sse2.cpp"
  "/home/mihui/Dev/fantasyCraft/thirdparty/FastNoiseSIMD/FastNoiseSIMD/FastNoiseSIMD_sse41.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/mihui/Dev/fantasyCraft/cmake-build-debug/CMakeFiles/cmake.verify_globs")
endif()
