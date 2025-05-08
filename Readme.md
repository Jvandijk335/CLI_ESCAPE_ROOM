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
Client_app/
├── 

Server_app/
├──

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

![](SequenceDiagram.svg)

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

![](CppClassDiagram.svg)

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

| Action                    | Input                             | Description                                                          |
|---------------------------|-----------------------------------|----------------------------------------------------------------------|
| Move                      | `move north`                      | Move to a different room in a specific direction.                    |
| Interact                  | `use key`                         | Use an item from your inventory in the current room.                 |
| Ask for Hint              | `hint`                            | Receive a hint for your current room or situation.                   |
| Check Status              | `status`                          | Check your progress, remaining time, and current room information.   |
| Solve Puzzle              | `solve 2419`                      | Solve a puzzle by entering the correct solution.                     |
| Chat                      | `say "I found a key!"`            | Send a message to your teammates or other players in the room.       |
| Examine                   | `examine painting`                | Examine a specific object or item in the room for clues.             |
| Pick Up                   | `pick up lantern`                 | Pick up an item in the room and add it to your inventory.            |
| Drop Item                 | `drop key`                        | Drop an item from your inventory in the room.                        |
| View Inventory            | `inventory`                       | Check the items in your current inventory.                           |
| Combine Items             | `combine key with rope`           | Combine two or more items from your inventory.                       |
| Look Around               | `look around`                     | Look around your current room to gather more information.            |
| Use Door                  | `use door`                        | Attempt to open or interact with a door or entryway in the room.     |
| Unlock Door               | `unlock door with key`            | Unlock a door using an item from your inventory (e.g., a key).       |
| Activate Mechanism        | `activate lever`                  | Activate a mechanism or contraption in the room.                     |
| Read Object               | `read book`                       | Read text or instructions on an object (e.g., a book, note, etc.).   |
| Inspect Item              | `inspect key`                     | Get a description or details about an item in your inventory.        |
| Ask for Help              | `help`                            | Request a list of available commands and help with the game.         |
| Search Room               | `search room`                     | Search the entire room for hidden clues, objects, or passages.       |

### Examples

