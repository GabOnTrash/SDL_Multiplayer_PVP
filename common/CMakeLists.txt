find_package(OpenSSL REQUIRED)

add_library(common INTERFACE)
target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 2. Collega i target di OpenSSL (SSL e Crypto) insieme ad asio_lib
target_link_libraries(common INTERFACE asio_lib OpenSSL::SSL OpenSSL::Crypto)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(common INTERFACE rt)
endif()