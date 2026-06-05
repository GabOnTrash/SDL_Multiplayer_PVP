# SDL Multiplayer PVP

A real-time multiplayer PvP game framework developed in **C++20** using **SDL3**. The project implements a modular state architecture (State Pattern) with logical resolution management, an action-based input manager, and native support for secure network connections via the TLS/SSL protocol.

---

## Architecture

The game is structured following modern 2D engine development best practices, clearly separating business logic, rendering, and input system management.

### Game Loop and Lifecycle
The Client entry point configures the window parameters and starts the main loop. The loop sequentially coordinates OS event retrieval, input hardware updates, logical advancement of the current state via a time delta, and scene rendering.

### Resolution Management
To prevent game logic from depending on the user's screen resolution, the Renderer adopts a Fixed Virtual Resolution (3840x2160 - 4K) managed via SDL3's Letterbox logical presentation mode. This ensures game proportions remain unchanged regardless of physical window or monitor size.

### Action-Based Input Manager
Instead of directly mapping physical keys in the game logic, the system implements an action-based abstraction. Continuous states are used for fluid movements or prolonged actions. Single states are used for instantaneous events like jumping or opening menus. Mouse coordinates provided by the OS are automatically translated into the game's 4K logical space using the native SDL3 coordinate conversion functions, ensuring perfect pointing even on scaled screens.

### State Machine
The game logic is divided into independent states. Each state encapsulates its own rendering and update logic via dedicated methods, facilitating game expansion without creating monolithic code.

---

## Directory Structure

```text
assets/ - Graphic resources, sprites, textures, and fonts
auth/ - Reserved files for TLS security (Ignored by Git)
auth/server.crt - Public server certificate
auth/server.key - Private server key (CONFIDENTIAL)
src/core/ - Central engine logic and GameContext
src/game/ - Game states
src/input/ - InputManager and key mapping
src/renderer/ - SDL3 Renderer wrapper
Client.cpp - Client application entry point
Server.cpp - Server application entry point
CMakeLists.txt - Build System configuration
.gitignore - Git exclusions
```

---

## Security and TLS Connection

To ensure the integrity of PvP game packets, prevent Man-in-the-Middle attacks, and protect authentication data, the communication between Client and Server is encrypted via Transport Layer Security. Sensitive files must be placed exclusively in the auth directory, which is protected by the project's gitignore file to prevent accidental uploads to public repositories.

### OpenSSL Key and Certificate Generation Guide

For the local development environment, a Self-Signed Certificate can be generated using the OpenSSL command-line utility. Run the following commands sequentially in the terminal from the project root.

First, create the auth directory and enter it.

```bash
mkdir auth
cd auth
```

Next, generate the private server key. This generates a cryptographically secure 2048-bit RSA key which must never be shared or published.

```bash
openssl genrsa -out server.key 2048
```

Then, create a Certificate Signing Request. OpenSSL will prompt for information. For local testing, use localhost as the Common Name.

```bash
openssl req -new -key server.key -out server.csr -subj "/C=IT/ST=Italy/L=Local/O=DevGame/CN=localhost"
```

Afterward, generate the Self-Signed Certificate valid for 365 days using the newly created private key and CSR.

```bash
openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt
```

Finally, remove the temporary CSR file as it is no longer needed.

```bash
rm server.csr
```

### Code Integration

In the Server, the network module must load both files to configure the SSL context.

```cpp
ssl_context.use_certificate_chain_file("auth/server.crt");
ssl_context.use_private_key_file("auth/server.key", boost::asio::ssl::context::pem);
```

In the Client, only the public certificate is needed to verify the server identity and prevent security warnings during the handshake.

---

## Requirements and Compilation

The project requires a C++20 compatible compiler, CMake 3.20 or higher, and the core SDL3, SDL3_image, and OpenSSL libraries. To compile the project, create a build directory, enter it, run CMake to configure the project, and then build it.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```
