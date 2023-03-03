get_filename_component(PROJECTNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SHADERS "*.glsl")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

add_executable(${PROJECTNAME} ${SOURCES} ${HEADERS} ${SHADERS})
if(WIN32)
        target_link_libraries(${PROJECTNAME} "legacy_stdio_definitions.lib")
endif()
target_link_libraries(${PROJECTNAME} ${COMMON_LIBS})
