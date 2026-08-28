add_library(core STATIC
    src/color.cpp
    src/effect.cpp
    src/keyboard.cpp
    src/payload.cpp
    src/zone.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
