src/Apps contains different *apps* selectable in the main menu. Too add a new entry to the menu, add it in the MainMenu constructor.

Buttons and other board-specific defines should be in src/defs.hpp

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