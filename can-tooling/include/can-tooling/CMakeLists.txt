set(FILES "${CMAKE_CURRENT_SOURCE_DIR}/dladapterrp1210.h"
          "${CMAKE_CURRENT_SOURCE_DIR}/dla_info.h"
          "${CMAKE_CURRENT_SOURCE_DIR}/rp1210_defs.h"
          "${CMAKE_CURRENT_SOURCE_DIR}/can_frame.h"
          "${CMAKE_CURRENT_SOURCE_DIR}/array_to_types.h"
          "${CMAKE_CURRENT_SOURCE_DIR}/can_socket.h"
          "${CMAKE_CURRENT_SOURCE_DIR}/mesg_fifo.h"
          "${CMAKE_CURRENT_SOURCE_DIR}/SendMesgCntl.h")


set(SUBDIRECTORIES "")
set(RELATIVE_PATH "include/can-tooling")


foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources("${PROJECT_NAME}" PRIVATE ${FILES})
	
	install(FILES ${FILES} DESTINATION ${RELATIVE_PATH})
endif()
