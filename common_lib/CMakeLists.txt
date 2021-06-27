PROJECT(common_lib)

message("Process Project: ${PROJECT_NAME}")

file(GLOB SRC_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

include_directories(${PROJECT_SOURCE_DIR}/include )

set(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

# 生成动态库目标
add_library(cpc SHARED ${SRC_LIST})
