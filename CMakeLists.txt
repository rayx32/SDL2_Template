cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

OPTION(USE_EMSCRIPTEN "Use Emscripten"	OFF)
if(USE_EMSCRIPTEN)
    if(NOT EMSDK_PATH)
        message(FATAL_ERROR "Could not locate the Emscripten compiler toolchain directory! Set -DEMSDK_PATH=<path/to/emsdk> to CMake to explicitly specify the location of the compiler!")
    else()
        message("In Emscripten world...")
        get_filename_component(EMSDK_PATH "${EMSDK_PATH}" ABSOLUTE)
        set(CMAKE_TOOLCHAIN_FILE ${EMSDK_PATH}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)
    endif(NOT EMSDK_PATH)
else()
    message("Emscripten is OFF...")
endif()

# Project name
project(SDL2_Template)

set(SOURCE_FILES 
	src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(USE_EMSCRIPTEN)
    message("Emscripten build options...${CMAKE_SOURCE_DIR}")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    configure_file(web/index.html ${CMAKE_BINARY_DIR}/index.html)
    target_compile_options(${PROJECT_NAME} PRIVATE "-sUSE_SDL=2" "-sUSE_SDL_IMAGE=2" "-sUSE_SDL_TTF=2" "-sUSE_SDL_MIXER=2" "-sUSE_SDL_NET=2" "-sSDL2_IMAGE_FORMATS=[\"png\"]")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-sUSE_WEBGL2=0 -sFULL_ES3=0 -sALLOW_MEMORY_GROWTH=1 -sMAXIMUM_MEMORY=1gb --use-preload-plugins --preload-file ${CMAKE_SOURCE_DIR}/data@")
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_image SDL2_mixer SDL2_ttf SDL2_net)
else()
    add_subdirectory(thirdparty)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2_mixer::SDL2_mixer SDL2_image::SDL2_image SDL2_net::SDL2_net SDL2_ttf::SDL2_ttf)
    file(COPY data/assets DESTINATION ${CMAKE_BINARY_DIR})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE src)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json)
endif()
