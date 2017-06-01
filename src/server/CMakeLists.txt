SET(SOURCE_FILE
    main.c

    msg_proc_collector.c
    msg_proc_collector.h

    msg_collector.c
    msg_collector.h

    protocol.h

    server_collector.c
    server_collector.h

    version.h
    )

MESSAGE(STATUS "configure Server for collector_server")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(collector_server ${SOURCE_FILE})
TARGET_LINK_LIBRARIES(collector_server collector)
