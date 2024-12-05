# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)



# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("OpenRayTracingRenderer")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable (OpenRayTracingRenderer main.cpp "include/ThreadPool.h" "src/ThreadPool.cpp" "include/FrameBuffer.h"  "src/FrameBuffer.cpp" "include/SpinLock.h" "include/Ray.h" "include/Camera.h" "src/Camera.cpp" "include/Sphere.h" "src/Sphere.cpp" "include/Hittable.h" "src/Hittable.cpp" "include/Tool.h"  "include/Material.h" "src/Tool.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/3rdParty)

target_compile_definitions(OpenRayTracingRenderer PUBLIC GLM_FORCE_DEPTH_ZERO_TO_ONE GLM_FORCE_RADIANS GLM_FORCE_RIGHT_HANDED)


