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
│   ├── Escaperoom_client
│   ├── client
│   ├── include
│   │   └── Player.h
│   └── src
│       ├── Player.cpp
│       └── ZMQquest_client.cpp
├── LICENSE
├── Readme.md
├── Server_app
│   ├── Escaperoom_s2
│   ├── Escaperoom_server
│   ├── include
│   │   ├── GameManager.h
│   │   ├── GameServer.h
│   │   ├── Item.h
│   │   ├── PlayerSession.h
│   │   ├── Puzzle.h
│   │   └── Room.h
│   ├── server
│   └── src
│       ├── GameManager.cpp
│       ├── GameServer.cpp
│       ├── Item.cpp
│       ├── PlayerSession.cpp
│       ├── Puzzle.cpp
│       ├── Room.cpp
│       ├── ZMQquest_server.cpp
│       └── ZMQquest_server2.0.cpp
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


participant Client
participant "MQTT Server" as MqttServer
box Server
skinparam BackgroundColor grey
participant "Event Handler" as Server
participant "Thread Pool" as ThreadPool
end box

activate MqttServer

' Emphasize persistent subscription
MqttServer <--[#green]-- Server : Listen
activate Server

' Client announces its presence (optional)
activate Client
Client -> MqttServer : Client Connect
MqttServer -> Server : (forward) Client Connect
Server -> Server : Check availability

' Server receives and manages thread
Server -> ThreadPool : Acquire/Create Thread for Task
activate ThreadPool
ThreadPool -> MqttServer : ACK
MqttServer -> Client : (forward) ACK

Client -> MqttServer : Request Service
MqttServer -> ThreadPool : (forward) Request Service

ThreadPool -> ThreadPool : Process Task
ThreadPool -> MqttServer : Return Service
MqttServer -> Client : (forward) Return Service

Client -> MqttServer : Client Close
deactivate Client
MqttServer -> ThreadPool : (forward) Client Close
deactivate ThreadPool

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

package "Client" {
  class Player {
    + std::string username
    + std::string currentRoom
    --
    + void sendCommand(const std::string& command)
    + void move(const std::string& direction)
    + void interact(const std::string& target)
    + void inspect(const std::string& object)
    + void manageInventory(const std::string& action, const std::string& item)
    + void requestHelp()
    + void lookAround()
    + std::string joinChatRoom()

  }
}

package "Server" {
  class GameServer {
    + GameManager gameLogic
    + std::unordered_map<std::string, std::thread> clientThreads
    --
    + void handlePlayerConnect(const std::string& username)
  }

  class GameManager {
    + std::vector<Room> roomTemplates
    + std::unordered_map<std::string, PlayerSession> players
    --
    + void initializeGame()
    + void movePlayer(PlayerSession& session, const std::string& direction)
    + std::string handleAction(PlayerSession& session, const std::string& action, const std::string& target)
    + std::string getHint(PlayerSession& session)
    + std::string getStatus(PlayerSession& session)
    + std::string getChatTopic(PlayerSession& session)

  }

  class PlayerSession {
    + std::string username
    + Room personalRoom
    + std::chrono::system_clock::time_point lastActivityTime
    --
    + std::string processCommand(const std::string& command)
    + std::string getHint()
    + std::string getStatus()
    + std::string executeAction(const std::string& action, const std::string& target)
  }

  class Room {
    + std::string name
    + std::string description
    + std::vector<Puzzle> puzzles
    + std::vector<Item> items
    + std::vector<std::string> connectedPlayers
    --
    + std::string describeRoom(const PlayerSession& session)
    + std::string interact(PlayerSession& session, const std::string& action)
  }

  class Puzzle {
    + std::string name
    + std::string description
    + std::string solution
    + bool isSolved
    --
    + bool attemptSolution(const std::string& input)
    + std::string getClue()
  }

  class Item {
    + std::string name
    + std::string description
    + bool isUsable
    --
    + std::string use()
  }
}

' Relationships
Player --o GameServer : interacts with
GameServer --* GameManager : uses
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
| Client → Server | `escape_room/commands/<username>`  | Send player commands (e.g., move, solve). |
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

