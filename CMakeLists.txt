cmake_minimum_required(VERSION 3.10)
project(GomokuAI)

set(CMAKE_CXX_STANDARD 17)

# Tìm gói SFML
# Lưu ý: Cần set SFML_DIR nếu CMake không tự tìm thấy
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(GomokuAI
    main.cpp
    Game.cpp
    Game.h
    Board.cpp
    Board.h
    View.cpp
    View.h
    AI.cpp
    AI.h
    Global.h
)

if (SFML_FOUND)
    target_link_libraries(GomokuAI sfml-graphics sfml-window sfml-system)
endif()
