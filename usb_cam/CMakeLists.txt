PROJECT(usb_cam)

message(STATUS "Project: usb_cam")

aux_source_directory(./src DIR_SRCS)

SET(EXECUTABLE_OUTPUT_PATH "../bin")
# set_target_properties( OUTPUT_NAME "usb_cam")
# set_target_properties( ARCHIVE_OUTPUT_DIRECTOR "../bin")