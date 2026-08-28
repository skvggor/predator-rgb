add_library(backend STATIC
    src/device_backend.cpp
)

target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(backend PUBLIC core)
