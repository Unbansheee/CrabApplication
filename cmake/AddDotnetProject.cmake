function(add_dotnet_project TARGET_NAME CSHARP_PROJECT_PATH OUTPUT_SUBDIR)
    set(CSHARP_OUTPUT_DIR ${CMAKE_BINARY_DIR}/${OUTPUT_SUBDIR})

    # Always build the .NET project
    get_filename_component(PROJ_NAME ${CSHARP_PROJECT_PATH} NAME_WE)

    add_custom_target(Build_${PROJ_NAME}_Scripts ALL
            COMMAND dotnet publish ${CSHARP_PROJECT_PATH} -c Release -o ${CSHARP_OUTPUT_DIR}
            COMMENT "Building .NET project: ${CSHARP_PROJECT_PATH}"
            USES_TERMINAL
    )

    # Native target depends on the .NET scripts
    add_dependencies(${TARGET_NAME} Build_${PROJ_NAME}_Scripts)

    # Copy .NET outputs after build
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${TARGET_NAME}>/Dotnet"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CSHARP_OUTPUT_DIR}/${PROJ_NAME}.dll
            ${CSHARP_OUTPUT_DIR}/${PROJ_NAME}.deps.json
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/Dotnet"
    )

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -DSRC=${CSHARP_OUTPUT_DIR}/${PROJ_NAME}.runtimeconfig.json
            -DDST=$<TARGET_FILE_DIR:${TARGET_NAME}>/Dotnet
            -P ${CMAKE_SOURCE_DIR}/cmake/CopyIfExists.cmake
    )

    # Copy nethost.dll for embedding
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/vendor/CrabEngine/vendor/nethost/nethost.dll
            "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    )
endfunction()