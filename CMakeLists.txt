cmake_minimum_required(VERSION 3.20)
project(rasteriser LANGUAGES CXX)

add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    Rasteriser/Scripts/*.cpp
)


add_executable(rasteriser WIN32 ${SOURCES})

target_link_libraries(rasteriser PRIVATE
    user32
)

set(OUTPUT_ASSETS "$<TARGET_FILE_DIR:rasteriser>/Assets")

add_custom_command(TARGET rasteriser POST_BUILD
    # Step 1: Copy assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Rasteriser/Assets"
        "$<TARGET_FILE_DIR:rasteriser>/Assets"
)

file(GLOB_RECURSE WAV_FILES "${CMAKE_SOURCE_DIR}/Rasteriser/Assets/Audio/*.wav")

foreach(WAV_FILE IN LISTS WAV_FILES)
    # Compute relative path inside Assets
    file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}/Rasteriser/Assets" "${WAV_FILE}")
    set(OUTPUT_OGG "${OUTPUT_ASSETS}/${REL_PATH}")
    string(REPLACE ".wav" ".ogg" OUTPUT_OGG "${OUTPUT_OGG}")

    # Make sure output folder exists
    get_filename_component(OUTPUT_DIR "${OUTPUT_OGG}" DIRECTORY)
    add_custom_command(TARGET rasteriser POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}"
        COMMAND ffmpeg -y -i "${WAV_FILE}" -c:a libvorbis -q:a 4 "${OUTPUT_OGG}"
    )
endforeach()

add_custom_command(TARGET rasteriser POST_BUILD
    COMMAND powershell -Command "Remove-Item -Path '$<TARGET_FILE_DIR:rasteriser>/Assets/Audio/*.wav' -Force"
)