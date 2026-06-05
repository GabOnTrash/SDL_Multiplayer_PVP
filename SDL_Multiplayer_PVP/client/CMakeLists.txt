file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/game/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/renderer/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/inputmanager/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/statemachine/playingstate/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/statemachine/menustate/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Client.cpp"
)

add_executable(client ${CLIENT_SOURCES})

target_include_directories(client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(client PRIVATE
    asio_lib
    SDL3::SDL3
    SDL3_image::SDL3_image
)

# ── Copia le DLL di runtime nella cartella dell'eseguibile (solo Windows) ─────
if(WIN32)
    add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:client>
            $<TARGET_FILE_DIR:client>
        COMMAND_EXPAND_LISTS
    )
endif()

add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:client>/assets"
    COMMENT "Copia della cartella degli asset in corso..."
)

# ── Console visibile in debug, nascosta in release ────────────────────────────
if(WIN32)
    set_target_properties(client PROPERTIES
        WIN32_EXECUTABLE $<IF:$<CONFIG:Release>,TRUE,FALSE>
    )
endif()