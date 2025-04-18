cmake_minimum_required(VERSION 3.28)
project(AmazingGame LANGUAGES CXX)
set(GameName "Asteroidal")

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(${GameName} src/main.cpp)
target_link_libraries(${GameName} PRIVATE sfml-graphics)
target_link_libraries(${GameName} PRIVATE sfml-audio)
target_compile_features(${GameName} PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
	TARGET ${GameName}
        COMMENT "Copy OpenAL DLL"
	PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${GameName}>
        VERBATIM)
endif()

# Create Highscore File
file( WRITE "highscore.txt" "highscore 0\ntwoplayerhighscore 0" )

install(TARGETS ${GameName})
