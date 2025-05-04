cmake_minimum_required(VERSION 3.28)
project(CrabEditor VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES src/*.h src/*.hpp src/*.c src/*.cpp)
file(GLOB_RECURSE MODULE_FILES CONFIGURE_DEPENDS LIST_DIRECTORIES false src/*.cppm)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Uncomment to force DEV_MODE setting. DEV_MODE ON means the program will use the resources from the source root, not the build directory
# set(DEV_MODE OFF CACHE BOOL "Override DEV_MODE" FORCE)

add_executable(CrabApplication ${SOURCES})
set_target_properties(CrabApplication PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor/CrabEngine)
target_link_libraries(CrabApplication PRIVATE CrabEngine)

target_include_directories(CrabApplication PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(CrabApplication PRIVATE "$<$<CONFIG:Debug>:_DEBUG>")

if (DEV_MODE)
    target_compile_definitions(CrabApplication PUBLIC RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources")
else()
    target_compile_definitions(CrabApplication PUBLIC RESOURCE_DIR="./resources")
    target_copy_engine_resources(CrabApplication)

    add_custom_command(TARGET CrabApplication POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:CrabApplication>/resources"
            COMMENT "Copying CrabEngine resources to output directory"
    )
endif()

target_copy_crabengine_dependencies(CrabApplication)

target_sources(CrabApplication
        PUBLIC
        FILE_SET "modules"
        TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES ${MODULE_FILES}
)

add_dotnet_project(CrabApplication
        ${CMAKE_SOURCE_DIR}/script/CrabApplication/CrabApplication.csproj
        $<TARGET_FILE_DIR:CrabApplication>/Dotnet)

add_dotnet_project(CrabApplication
        ${CMAKE_SOURCE_DIR}/script/CrabModule/CrabModule.csproj
        $<TARGET_FILE_DIR:CrabApplication>/Dotnet)



set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CrabApplication)