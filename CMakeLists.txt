cmake_minimum_required(VERSION 3.16)

if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  # TUTTO racchiuso in un'unica stringa tra virgolette:
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
    "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
  )
endif()

project("SDL_Multiplayer_PVP")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ── Opzioni di build ──────────────────────────────────────────────────────────
option(BUILD_CLIENT "Compila il client (richiede SDL3)" ON)
option(BUILD_SERVER "Compila il server"                 ON)

# ── Threads ───────────────────────────────────────────────────────────────────
find_package(Threads REQUIRED)

# ── Asio standalone ───────────────────────────────────────────────────────────
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-30-2
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(asio)

add_library(asio_lib INTERFACE)
target_include_directories(asio_lib INTERFACE ${asio_SOURCE_DIR}/asio/include)
target_compile_definitions(asio_lib INTERFACE
    ASIO_STANDALONE
    ASIO_NO_DEPRECATED
)
target_link_libraries(asio_lib INTERFACE
    Threads::Threads
    $<$<PLATFORM_ID:Windows>:ws2_32>
    $<$<PLATFORM_ID:Windows>:mswsock>
)

# ── SDL3 e SDL3_image (solo per il client) ────────────────────────────────────
if(BUILD_CLIENT)

    # ── SDL3 ──────────────────────────────────────────────────────────────────
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-3.2.14
        GIT_SHALLOW    TRUE
    )
    set(SDL_TESTS             OFF CACHE BOOL "" FORCE)
    set(SDL_EXAMPLES          OFF CACHE BOOL "" FORCE)
    set(SDL_INSTALL           OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SDL3)

    # ── SDL3_image ────────────────────────────────────────────────────────────
    # Usiamo release-3.2.4: tag stabile, senza submodule pesanti come libjxl/dav1d
    FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG        release-3.2.4
        GIT_SHALLOW    TRUE
    )

    # Abilita solo PNG e JPG — sufficienti per qualsiasi gioco 2D
    set(SDLIMAGE_PNG          ON  CACHE BOOL "" FORCE)
    set(SDLIMAGE_JPG          ON  CACHE BOOL "" FORCE)
    set(SDLIMAGE_BMP          ON  CACHE BOOL "" FORCE)

    # Disablita tutto il resto (evita NASM, dav1d, libjxl, ecc.)
    set(SDLIMAGE_AVIF         OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_WEBP         OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_TIF          OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_JXL          OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_XCF          OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_XPM          OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_LBM          OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_XV           OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_PCX          OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_PNM          OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_QOI          OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_SVG          OFF CACHE BOOL "" FORCE)

    # Usa le librerie interne (no dipendenze di sistema)
    set(SDLIMAGE_PNG_VENDORED ON  CACHE BOOL "" FORCE)
    set(SDLIMAGE_JPG_VENDORED ON  CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(SDL3_image)

endif()

# ── Sottoprogetti ─────────────────────────────────────────────────────────────
add_subdirectory("SDL_Multiplayer_PVP/common")

if(BUILD_CLIENT)
    add_subdirectory("SDL_Multiplayer_PVP/client")
endif()

if(BUILD_SERVER)
    add_subdirectory("SDL_Multiplayer_PVP/server")
endif()