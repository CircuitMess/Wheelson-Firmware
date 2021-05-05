# CircuitMess Wheelson
<img src="https://www.circuitmess.com/wp-content/uploads/2020/06/Wheelson_1.jpg">

[Support our work on Kickstarter](https://www.kickstarter.com/projects/albertgajsak/circuitmess-stem-box)

src/Apps contains different *apps* selectable in the main menu. To add a new entry to the menu, add it in the MainMenu constructor.

Buttons and other board-specific defines should be in src/Wheelson.h

# Using GNU Make
To make effective use of the makefile, edit the ACLI and PORT variables accordingly. ACLI should point to the arduino-cli executable, and PORT is the board COM port. 

Three targets are defined:
* **build** (default target) - builds the firmware in the build/ directory
* **upload** - builds if necessary and uploads to the board
* **clean** - removes the build/ directory

Usage (in the directory where the Makefile is):
```shell script
make <target>
```

Pretty simple :)

Will probably add PlatformIO support if the need arises. Hit us up with an issue if you'd like us to add a platformio config.
