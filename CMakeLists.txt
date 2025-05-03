cmake_minimum_required(VERSION 3.28)
project(CrabEditor VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES src/*.h src/*.hpp src/*.c src/*.cpp)
file(GLOB_RECURSE MODULE_FILES CONFIGURE_DEPENDS LIST_DIRECTORIES false src/*.cppm)

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
target_compile_definitions(CrabApplication PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources")

target_sources(CrabApplication
        PUBLIC
        FILE_SET "modules"
        TYPE CXX_MODULES
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES ${MODULE_FILES}
)

target_copy_engine_resources(CrabApplication)
target_copy_crabengine_dependencies(CrabApplication)

add_custom_command(TARGET CrabApplication POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:CrabApplication>/resources"
        COMMENT "Copying CrabEngine resources to output directory"
)

add_dotnet_project(CrabApplication
        ${CMAKE_SOURCE_DIR}/script/CrabApplication/CrabApplication.csproj
        $<TARGET_FILE_DIR:CrabApplication>/Dotnet)

add_dotnet_project(CrabApplication
        ${CMAKE_SOURCE_DIR}/script/CrabModule/CrabModule.csproj
        $<TARGET_FILE_DIR:CrabApplication>/Dotnet)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CrabApplication)