# CircuitMess Wheelson
> The repository for the core firmware that comes preloaded on every Wheelson.

Autonomous cars are the future and we’ll show you how it works.

This tiny wheeled robot has a camera and a microcomputer and can be programmed to autonomously navigate a small road while driving, just like an autonomous car would.

Jay-D is also a part of [CircuitMess STEM Box](https://igg.me/at/stem-box/x#/) - a series of fun electronic kits to help children and adults understand the basics of technologies everybody's talking about.


<img src="https://www.circuitmess.com/wp-content/uploads/2020/06/Wheelson_1.jpg">


#
src/Apps contains different *apps* selectable in the main menu. To add a new entry to the menu, add it in the MainMenu constructor.

Buttons and other board-specific defines should be in src/Wheelson.h

# Compiling

The firmware is based on the [Wheelson Library](https://github.com/CircuitMess/Wheelson-Library). It is, along other required libraries, automatically installed when you install the CircuitMess ESP32 Arduino platform. More info on [CircuitMess/Arduino-Packages](https://github.com/CircuitMess/Arduino-Packages).

## Using Arduino IDE

Simply open Wheelson-Firmware.ino using Arduino IDE, set the board to Wheelson, and compile.

## Using CMake

To compile and upload you need to have [CMake](https://cmake.org/) and [arduino-cli](https://github.com/arduino/arduino-cli)  installed. You also need to have both of them registered in the PATH.

In the CMakeLists.txt file change the port to your desired COM port (default is /dev/ttyUSB0):
```
set(PORT /dev/ttyUSB0)
```
Then in the root directory of the repository type:
```
mkdir cmake
cd cmake
cmake ..
cmake --build . --target CMBuild
```
This will compile the binaries, and place the .bin and .elf files in the build/ directory located in the root of the repository.

To compile the binary, and upload it according to the port set in CMakeLists.txt, run

```cmake --build . --target CMBuild```

in the cmake directory.

To make effective use of the makefile, edit the ACLI and PORT variables accordingly. ACLI should point to the arduino-cli executable, and PORT is the board COM port. 

Three targets are defined:
* **build** (default target) - builds the firmware in the build/ directory
* **upload** - builds if necessary and uploads to the board
* **clean** - removes the build/ directory

Usage (in the directory where the Makefile is):
```shell script
make <target>
```


Will probably add PlatformIO support if the need arises. Hit us up with an issue if you'd like us to add a platformio config.

# Meta


**CircuitMess**  - https://circuitmess.com/

**Facebook** - https://www.facebook.com/thecircuitmess/

**Instagram** - https://www.instagram.com/thecircuitmess/

**Twitter** - https://twitter.com/circuitmess

**YouTube** - https://www.youtube.com/channel/UCVUvt1CeoZpCSnwg3oBMsOQ

----
Copyright © 2021 CircuitMess

Licensed under [MIT License](https://opensource.org/licenses/MIT).