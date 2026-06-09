# Raccoglie automaticamente tutti i .cpp dentro le cartelle specifiche
file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/Server.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/serverimpl/*.cpp"
)

add_executable(server ${SERVER_SOURCES})

# Configura le directory in cui il compilatore cercherà gli header
target_include_directories(server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(server PRIVATE
    common
)

add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../common/network/auth"
        "$<TARGET_FILE_DIR:server>/auth"
        COMMENT "Copia della cartella dell'autenticazione in corso..."
)


if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(server PROPERTIES WIN32_EXECUTABLE FALSE) # da cambiare con TRUE in release
endif()