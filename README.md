# Web Macros

Web Macros is the second version of my Roku Macros project. The original version used a logic analyzer, Arduino, and infrared signal replay to simulate button presses from a physical Roku remote. This version takes the same idea and moves it into software by using Roku's network API.

Instead of replaying IR signals, this project sends commands to a Roku device over Wi-Fi. This makes the project more flexible, more accurate, and easier to expand because the TV can be controlled through API requests instead of hardware timing and signal reproduction.

## Why I Built This

After finishing the hardware version of Roku Macros, I wanted to see if there was a cleaner way to control the TV. The IR version was useful for learning electronics, signal capture, and Arduino programming, but it had limits. It depended on remote signal timing, physical placement, and the specific TV/remote behavior I captured.

Roku's API gave me a better path. By sending commands over the local network, I could control the TV directly and avoid a lot of the complexity that came with replaying infrared signals.

A good example is text input. With IR, typing would require building a mapping of remote button presses and calculating how to move through an on-screen keyboard. With the API, text can be sent much more directly.

## What It Does

The goal of Web Macros is to send Roku remote-style commands through software.

Current project goals include:

* Sending commands to a Roku device over the local network
* Replacing physical IR signal replay with API-based control
* Building reusable C functions for Roku command requests
* Creating simple command-line macros for repeated TV actions
* Keeping the project small, understandable, and easy to modify

## Tech Used

* C
* Make
* Linux
* Roku External Control Protocol API
* HTTP requests
* Local network communication

## Project Structure

```text
Web_Macros/
├── include/        Header files
├── src/            Source files
├── Makefile        Build commands
├── debug.txt       Debug notes/output
├── roku_ecp_req.txt
└── README.md
```

## Build

```bash
make
```

## Run

```bash
./roku-macros
```

The Roku device needs to be on the same local network as the computer running the program.

## Project Status

This is a working/experimental learning project. The main purpose is to explore local network device control in C and improve on the original hardware-based Roku Macros project.

This project is not meant to be a polished consumer application. It is a practical project for learning API communication, C project structure, command-line tooling, and network-based automation.

## Related Project

The original Roku Macros project used hardware IR signal replay with an Arduino. Web Macros is the software/API version of the same idea.
