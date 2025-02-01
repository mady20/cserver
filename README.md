# Simple TCP Socket Server

## Description

This project implements a simple TCP socket server in C. The server listens for client connections, receives messages from the client, and responds with a confirmation message. It demonstrates basic socket programming concepts such as creating a socket, binding, listening, accepting connections, reading, and writing data.

## Features

- Listens for incoming client connections.
- Reads data sent by the client.
- Sends a confirmation message back to the client.
- Uses a `Makefile` for easy compilation.
- Error handling for failed operations.

## Prerequisites

- **GCC Compiler**: Required to compile the C source files.
- **POSIX-compliant system**: Works on Linux/macOS.
- **Basic knowledge of networking**: Helps in understanding socket programming.

---

## Installation

Clone the repository:

```bash
git clone https://github.com/mady20/cserver.git
cd cserver
```

Compile the project using:

```bash
make
```

This will generate two executable files:

- `server`: The server application.
- `client`: The client application.

To remove compiled binaries:

```bash
make clean
```

---

## Usage

### **Starting the Server**

Run the server with a specified port number:

```bash
./server <port_number>
```

Example:

```bash
./server 8080
```

### **Starting the Client**

Once the server is running, start the client (assuming `client.c` is implemented):

```bash
./client <server_ip> <port_number>
```

Example:

```bash
./client 127.0.0.1 8080
```

---

## How It Works

1. The server binds to a specified port and listens for incoming connections.
2. When a client connects, the server accepts the connection.
3. The server continuously reads data sent by the client.
4. The received data is printed on the server's console.
5. The server responds with a confirmation message.
6. The connection continues until terminated.

---


## Notes

- Ensure the specified port is not blocked by a firewall.
- Modify `client.c` accordingly to communicate with the server.
- The server currently runs in an infinite loop; you may need to terminate it manually using `Ctrl + C`.

## License

This project is licensed under the MIT License.

