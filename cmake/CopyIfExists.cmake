# CopyIfExists.cmake
set(SOURCE_FILE "$ENV{SRC}")
set(DEST_DIR "$ENV{DST}")

if(EXISTS "${SOURCE_FILE}")
    file(COPY "${SOURCE_FILE}" DESTINATION "${DEST_DIR}")
endif()