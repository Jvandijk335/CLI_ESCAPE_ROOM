# CLI ESCAPE ROOM
```
         ==============================================
          ||     ||     ||<(.)>||<(.)>||     ||     ||
          ||     ||     ||     ||     ||     ||     ||
 _____ ___||  ___||   _ || ____||_____|| ____|| ___ ||___  __  __ 
| ____/ ___| / ___|  / \  |  _ \| ____| |  _ \ / _ \ / _ \|  \/  |
|  _| \___ \| |     / _ \ | |_) |  _|   | |_) | | | | | | | |\/| |
| |___ ___) | |___ / ___ \|  __/| |___  |  _ <| |_| | |_| | |  | |
|_____|____/ \____/_/   \_\_|   |_____| |_| \_\\___/ \___/|_|  |_|
          ||     ||     ||     ||     ||     ||     ||
          ||    _||     ||     ||     ||     ||_    ||
          ||   (__D     ||     ||     ||     C__)   ||
          ||   (__D     ||     ||     ||     C__)   ||
          ||   (__D     ||     ||     ||     C__)   ||
          ||   (__D     ||     ||     ||     C__)   ||
          ||     ||     ||     ||     ||     ||     ||
         ==============================================
```

## Overview
CLI Escape Room is a multiplayer text-based puzzle adventure that runs entirely in your terminal. Players must collaborate, explore rooms, solve puzzles, and escape before time runs out.

## 🧱 Project Structure

```
.
├── Client_app
│   ├── CMakeLists.txt
│   ├── build
│   │   ├── CMakeCache.txt
│   │   ├── CMakeFiles
│   │   ├── EscapeRoomClient -> binary file
│   │   ├── Makefile
│   │   ├── cmake_install.cmake
│   │   └── libPlayerLib.a
│   ├── include
│   │   └── Player.h
│   └── src
│       ├── Player.cpp
│       └── main.cpp
├── LICENSE
├── Readme.md
├── Server_app
│   ├── CMakeLists.txt
│   ├── build
│   │   ├── CMakeCache.txt
│   │   ├── CMakeFiles
│   │   ├── EscapeRoomServer  -> binary file
│   │   ├── Makefile
│   │   └── cmake_install.cmake
│   ├── include
│   │   ├── GameManager.h
│   │   ├── GameServer.h
│   │   ├── Item.h
│   │   ├── PlayerSession.h
│   │   ├── Puzzle.h
│   │   └── Room.h
│   └── src
│       ├── GameManager.cpp
│       ├── GameServer.cpp
│       ├── Item.cpp
│       ├── PlayerSession.cpp
│       ├── Puzzle.cpp
│       ├── Room.cpp
│       └── main.cpp
├── build_all.sh
└── docs
    ├── CppClassDiagram.svg
    └── SequenceDiagram.svg
```

## 🧠 Architecture

### Sequence diagram
<!--
```plantuml
@startuml SequenceDiagram
skinparam BackgroundColor #AliceBluegit

box Client_App
skinparam BackgroundColor grey
participant Client
end box
participant "ZeroMQ Server" as ZeroMQServer
box Server_App
skinparam BackgroundColor grey
participant "Event Handler" as Server
end box

activate ZeroMQServer

' Emphasize persistent subscription
ZeroMQServer <--[#green]-- Server : Listen
activate Server

' Client announces its presence (optional)
activate Client
Client -> ZeroMQServer : Client Connect
ZeroMQServer -> Server : (forward) Client Connect
Server -> Server : Check availability

' Server receives and manages connection
Server -> ZeroMQServer : ACK
ZeroMQServer -> Client : (forward) ACK

Client -> ZeroMQServer : Request Service
ZeroMQServer -> Server : (forward) Request Service

Server -> Server : Process Task
Server -> ZeroMQServer : Return Service
ZeroMQServer -> Client : (forward) Return Service

Client -> ZeroMQServer : Client Close
deactivate Client
ZeroMQServer -> Server : (forward) Client Close
@enduml
```
-->

![](./docs/SequenceDiagram.svg)

### Class diagram

<!-- 
```plantuml
@startuml CppClassDiagram
skinparam BackgroundColor #AliceBlue

skinparam class {
  BackgroundColor PaleGreen
  BorderColor DarkGreen
  ArrowColor DarkSlateGray
}

package "Server" {
  class GameManager {
    + void initializeGame()
    + std::string handleMessage(const std::string &username)
    + std::string handleMessage(const std::string &username, const std::string &message)
    --
    - std::vector<Room> roomTemplates
    - std::unordered_map<std::string, PlayerSession> players
  }

  class PlayerSession {
    + <<constructor>> PlayerSession(const std::string& username, const Room& room)
    + std::string processCommand(const std::string& command)
    + std::string getHint()
    + std::string getStatus()
    + std::string executeAction(const std::string& action, const std::string& target)
    + std::string move(const std::string& direction)
    + std::string getChatTopic()
    --
    - std::string username
    - Room personalRoom
    - std::chrono::system_clock::time_point lastActivityTime
  }

  class Room {
    + <<constructor>> Room(const std::string& name, const std::string& description)
    + std::string describeRoom(const PlayerSession& session)
    + std::string interact(PlayerSession& session, const std::string& action)
    --
    - std::string name
    - std::string description
    - std::vector<Puzzle> puzzles
    - std::vector<Item> items
  }

  class Puzzle {
    + <<constructor>> Puzzle(const std::string& name, const std::string& description, const std::string& solution)
    + bool attemptSolution(const std::string& input)
    + std::string getClue()
    --
    - std::string name
    - std::string description
    - std::string solution
    - bool isSolved
  }

  class Item {
    + <<constructor>> Item(const std::string& name, const std::string& description, bool isUsable)
    + std::string use()
    --
    - std::string name
    - std::string description
    - bool isUsable
  }
}

' Relationships
GameManager --* Room : template for
GameManager --* PlayerSession : manages
PlayerSession --* Room : owns
Room --* Puzzle : contains
Room --* Item : contains
@enduml
```
-->

![](./docs/CppClassDiagram.svg)

## 🔧 Setup Instructions
### Dependencies

- libzmq (ZeroMQ messaging library)
- CMake (build system)
- pkg-config (Windows-specific build support)

### Install `libzmq`

1. Visit: https://github.com/zeromq/libzmq
2. Select the v4.3.4 tag, download, and unzip.
3. Build using CMake + MinGW:
 - Open CMake GUI 
 - Source dir: path to libzmq (e.g., `C:/Users/you/libzmq-4.3.4`)
 - Build dir: same path + `/build`
 - Configure → Select *MinGW Makefiles*
 - Uncheck `ZMQ_BUILD_TESTS`
 - Generate → Open terminal in `/build` folder
 - Run: `mingw32-make -j4 install`

### Install `pkg-config` (Windows Only)
1. Download from: http://ftp.gnome.org/pub/gnome/binaries/win32/
 - `pkg-config_0.26-1_win32.zip` → extract `pkg-config.exe` to `C:\MinGW\bin`
 - `gettext-runtime_0.18.1.1-2_win32.zip` → extract `intl.dll` to `C:\MinGW\bin`
 - `glib_2.28.8-1_win32.zip` → extract `libglib-2.0-0.dll` to `C:\MinGW\bin`




## 📡 MQTT Topic Overview

| Direction       | Topic                              | Description                               |
|----------------|-------------------------------------|-------------------------------------------|
| Client → Server | `escape_room/commands/<username>?` | Send player commands (e.g., move, solve). |
| Server → Client | `escape_room/commands/<username>!` | Send server response                      |
| Server → Client | `escape_room/status/<username>`    | Send room status and game info.           |
| Server → Client | `escape_room/hints/<username>`     | Server-sent hints or clues.               |
| Server → Client | `escape_room/errors/<username>`    | Error messages (e.g., invalid command).   |
| Bidirectional   | `escape_room/mp_assistance/<room>` | Chat between players in the same room.    |

## 🎮 Command-Line Interface API
This section is for players who use the game via the terminal. Below you can see what commands you can enter and what they do.

### Available Commands

| Action    | Input                  | Description                            |
| --------- | ---------------------- | -------------------------------------- |
| Move      | `move room101`         | Move to a different room.              |
| Interact  | `interact with key`    | Interact with an item.                 |
| Inspect   | `inspect lantern`      | Inspect an item.                       |
| Pick Up   | `pick up lantern`      | Pick up an item.                       |
| Inventory | `view inventory`       | Check your items.                      |
| Solve     | `solve puzzle 2419`    | Solve a puzzle.                        |
| Chat      | `say "I found a key!"` | Talk to others in the chat.            |
| Search    | `search room`          | Search the room for items or clues.    |
| Hint      | `get hint`             | Get a hint for your current situation. |
| Status    | `check status`         | View your current status.              |


### Examples

