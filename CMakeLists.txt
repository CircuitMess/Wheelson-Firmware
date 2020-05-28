cmake_minimum_required(VERSION 3.15)
project(AutoCar)

set(CMAKE_CXX_STANDARD 14)

include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/libraries/Wire/src)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/libraries/FS/src)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/libraries/SD/src)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/libraries/SPI/src)

include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/cores/esp32)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/tools/sdk)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/tools/sdk/include)
include_directories(~/.arduino15/packages/cm/hardware/esp32/1.0.4/tools/sdk/include/freertos)
include_directories(~/Arduino/libraries/TFT_eSPI)
include_directories(~/Arduino/libraries/CircuitOS/src)

AUX_SOURCE_DIRECTORY(src SOURCES)
AUX_SOURCE_DIRECTORY(src/Apps SOURCES)
AUX_SOURCE_DIRECTORY(src/Apps/Timeline SOURCES)
AUX_SOURCE_DIRECTORY(src/Apps/Timeline/Bitmaps SOURCES)
AUX_SOURCE_DIRECTORY(src/Bitmaps SOURCES)
AUX_SOURCE_DIRECTORY(src/Components SOURCES)
AUX_SOURCE_DIRECTORY(src/Elements SOURCES)

add_definitions(-DDEBUG)

SET_SOURCE_FILES_PROPERTIES(Truckr.ino PROPERTIES LANGUAGE CXX)
add_executable(AutoCar ${SOURCES} Truckr.ino)