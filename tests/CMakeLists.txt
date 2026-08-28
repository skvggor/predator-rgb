add_executable(test_payload core/test_payload.cpp)
target_link_libraries(test_payload PRIVATE core GTest::gtest_main)
add_test(NAME payload COMMAND test_payload)

add_executable(test_keyboard core/test_keyboard.cpp)
target_link_libraries(test_keyboard PRIVATE core GTest::gtest_main)
add_test(NAME keyboard COMMAND test_keyboard)

add_executable(test_color core/test_color.cpp)
target_link_libraries(test_color PRIVATE core GTest::gtest_main)
add_test(NAME color COMMAND test_color)

add_executable(test_effect core/test_effect.cpp)
target_link_libraries(test_effect PRIVATE core GTest::gtest_main)
add_test(NAME effect COMMAND test_effect)

add_executable(test_zone core/test_zone.cpp)
target_link_libraries(test_zone PRIVATE core GTest::gtest_main)
add_test(NAME zone COMMAND test_zone)

add_executable(test_device_backend backend/test_device_backend.cpp)
target_link_libraries(test_device_backend PRIVATE backend GTest::gtest_main)
add_test(NAME device_backend COMMAND test_device_backend)
