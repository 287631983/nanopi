PROJECT(usb_cam)

message("Process Project: ${PROJECT_NAME}")

file(GLOB SRC_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/${CLEAN_TASK_DEV_DIR}/*.cpp"
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

ADD_EXECUTABLE(usb_cam ${SRC_LIST})

# INCLUDE_DIRECTORIES(${INNER_LIB_PATH}/include/solution_config_lib)
# LINK_DIRECTORIES(${INNER_LIB_PATH}/libs/solution_config_lib/${BUILD_SYS_PATH})

ADD_CUSTOM_COMMAND(TARGET clean_task
  POST_BUILD
  COMMAND cp ${EXECUTABLE_OUTPUT_PATH}/clean_task ${EXECUTABLE_OUTPUT_PATH}/clean_task.debug
  COMMAND ${CMAKE_STRIP} ${EXECUTABLE_OUTPUT_PATH}/clean_task
)
