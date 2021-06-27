SET(CROSS arm-linux-)
# set(CMAKE_C_COMPILER ${CROSS}gcc)  
set(CMAKE_CXX_COMPILER "${CROSS}g++")

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)         #cmake 最低版本要求，低于2.6 构建过程会被终止。 

PROJECT(lin_nanopi)                         #定义工程名称

MESSAGE(STATUS "Project: lin_nanopi")       #打印相关消息 

SET(CMAKE_BUILD_TYPE DEBUG)                 #指定编译类型

SET(COMPILE_DIR "/mnt/d/nanopi/toolchain/4.9.3/")
SET(KERNEL_DIR "/mnt/d/nanopi/kernel/linux-sunxi-4.14.y/")

SET(CMAKE_C_FLAGS_DEBUG "-O0 -Wall -g3 -rdynamic -Wall -ggdb  -lm")            #指定编译选项
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g3 -rdynamic -Wall -ggdb -Wall")          #指定编译选项

### add 第三方库头文件路径
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${COMPILE_DIR}/arm-cortexa9-linux-gnueabihf/include/c++/4.9.3/")  
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${KERNEL_DIR}/include/")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${KERNEL_DIR}/include/uapi")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${KERNEL_DIR}/include/asm-generic")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/common_lib/src")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${COMPILE_DIR}/arm-cortexa9-linux-gnueabihf/include/c++/4.9.3/")  
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${KERNEL_DIR}/include/")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${KERNEL_DIR}/include/uapi")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${KERNEL_DIR}/include/asm-generic")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/common_lib/src")
SET(CMAKE_CXX_FLAGS "-std=c++11")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET(COMMON_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/common_lib/")

#for acl
ADD_DEFINITIONS(-DLINUX2)

#添加子目录
ADD_SUBDIRECTORY(common_lib) 
ADD_SUBDIRECTORY(usb_cam)