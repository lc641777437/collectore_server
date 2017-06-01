SET(SOURCE_FILE

    log.c
    log.h

    object.c
    object.h

    session.h
    session.c

    timer.h
    timer.c
    )

MESSAGE(STATUS "configure common library")
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_library(collector SHARED ${SOURCE_FILE})
TARGET_LINK_LIBRARIES(collector m event zlog)
