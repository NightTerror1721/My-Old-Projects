cmake_minimum_required(VERSION 3.20)
project(BingoReborn VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----- Allegro 5 Configuration -----
if (WIN32)
    find_package(Allegro CONFIG REQUIRED)

    file(GLOB SOURCES "src/*.cpp" "src/**/*.cpp")
    add_executable(BingoReborn ${SOURCES})

    target_link_libraries(BingoReborn PRIVATE 
        Allegro::allegro
        Allegro::allegro_main
        Allegro::allegro_image
        Allegro::allegro_font
        Allegro::allegro_ttf
        Allegro::allegro_primitives
        Allegro::allegro_audio
        Allegro::allegro_acodec
        Allegro::allegro_dialog
    )

elseif (APPLE OR UNIX)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(ALLEGRO5 REQUIRED
        allegro-5
        allegro_main-5
        allegro_image-5
        allegro_font-5
        allegro_ttf-5
        allegro_primitives-5
        allegro_audio-5
        allegro_acodec-5
        allegro_dialog-5
    )

    file(GLOB SOURCES "src/*.cpp" "src/**/*.cpp")
    add_executable(BingoReborn ${SOURCES})

else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()
