# CMake配置文件
cmake_minimum_required (VERSION 3.8)
project ("ArchSceneGraph")

# 搜索目录
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/include/")            # 包含文件搜索目录
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/include/glew-2.1.0/") # 包含文件搜索目录
link_directories   ("${CMAKE_CURRENT_SOURCE_DIR}/deps/lib")                 # 连接器搜索目录
#link_directories   ("${CMAKE_CURRENT_SOURCE_DIR}/src/core")                # 核心库

# 添加编译宏
add_compile_definitions(
  _CRT_SECURE_NO_WARNINGS  #fopen sscanf 警告
)

# src内的文件夹
set( SRC_DIRES   
	 "WinApp"             # 应用窗口	 
)

# glad文件
set(GLAD_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/deps/include/glad/glad.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/deps/include/glad/glad.c"
)

# 框架文件
file(GLOB CORE_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*"   
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Shader/*"  
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Entity/*"   
)

# 复制textures编译目录
file(GLOB Textures "${CMAKE_CURRENT_SOURCE_DIR}/deps/resources/*" 
                   "${CMAKE_CURRENT_SOURCE_DIR}/deps/dll/*") 
file(COPY ${Textures} DESTINATION  ${CMAKE_CURRENT_BINARY_DIR} )

message(---------->"${CORE_SRC}")

# 添加glad库
add_library(glad ${GLAD_SRC})
add_library(core ${CORE_SRC})

# 遍历src文件夹内文件，添加exe程序
foreach(Src ${SRC_DIRES} )	
    message(${Src})
    file(GLOB SrcFile  "src/${Src}/*.h"  "src/${Src}/*.hpp"  "src/${Src}/*.cpp" "src/${Src}/*.c" )
    add_executable (${Src}  ${SrcFile} )
    target_link_libraries(${Src} glfw3 openGL32 glad core)
endforeach()

# target_link_libraries(Test03-Nuklear  glew32 )

