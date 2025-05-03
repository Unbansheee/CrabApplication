set(SOURCE_FILE "${SRC}")
set(DEST_DIR "${DST}")

if(EXISTS "${SOURCE_FILE}")
    file(COPY "${SOURCE_FILE}" DESTINATION "${DEST_DIR}")
endif()