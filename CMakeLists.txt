cmake_minimum_required(VERSION 3.20)
project(rasteriser LANGUAGES CXX)

add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS Rasteriser/Scripts/*.cpp)

add_executable(rasteriser WIN32 ${SOURCES})

target_link_libraries(rasteriser PRIVATE
    user32
)

# Copy assets folder to the build output directory
set(ASSET_FOLDERS
    Models
    Textures
)

foreach(ASSET ${ASSET_FOLDERS})
    if(EXISTS "${CMAKE_SOURCE_DIR}/Rasteriser/${ASSET}")
        add_custom_command(TARGET rasteriser POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/Rasteriser/${ASSET}"
                "$<TARGET_FILE_DIR:rasteriser>/${ASSET}"
        )
    endif()
endforeach()