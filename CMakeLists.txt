cmake_minimum_required(VERSION 3.15)
project(Wheelson-Firmware)
set(PORT /dev/ttyUSB0)
set(DEVICE cm:esp32:wheelson) # Arduino device FQBN
set(CMAKE_CXX_STANDARD 11)

set(ROOT ${CMAKE_SOURCE_DIR})
if (WIN32 OR CYGWIN)
    set(Arduino15 $ENV{APPDATA}/../Local/Arduino15)
    set(ArduinoLibs $ENV{USERPROFILE}/OneDrive/Documents/Arduino/libraries)

    string(REPLACE "\\" "/" Arduino15 ${Arduino15})
    string(REPLACE "\\" "/" ArduinoLibs ${ArduinoLibs})

    if (CYGWIN)
        string(REGEX REPLACE "^([A-Za-z]):" "/cygdrive/\\1" Arduino15 ${Arduino15})
        string(REGEX REPLACE "^([A-Za-z]):" "/cygdrive/\\1" ArduinoLibs ${ArduinoLibs})
        string(REGEX REPLACE "^/cygdrive/([A-Za-z])" "\\1:/" ROOT ${ROOT})
    endif ()
else ()
    set(Arduino15 ~/.arduino15)
    set(ArduinoLibs ~/Arduino/libraries)
endif ()

# Include directories
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/Wire/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/FS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/SD/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/SPI/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/WiFi/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/SPIFFS/src)

include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/cores/esp32)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/tools/sdk)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/tools/sdk/include)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/tools/sdk/include/esp32)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/tools/sdk/include/freertos)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/tools/sdk/include/driver)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/tools/sdk/include/driver/driver)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/tools/sdk/include/spi_flash)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/tools/sdk/include/esp32-camera/)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/tools/sdk/include/esp_http_server/)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/tools/sdk/include/nghttp/)

include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/CircuitOS/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/TFT_eSPI)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/TFT_eSPI/Extensions)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/LovyanGFX/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/U8g2_for_TFT_eSPI/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/JPEGDecoder/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/Wheelson-Library/src)
include_directories(${Arduino15}/packages/cm/hardware/esp32/1.7.3/libraries/OpenCV-ESP32/src)


# Source directories
file(GLOB_RECURSE SOURCES "src/**.cpp" "src/**.c" "src/**.hpp" "src/**.h")


# Defines
add_definitions(-DARDUINO)
add_definitions(-DDEBUG)
add_definitions(-DESP32)
add_definitions(-DCIRCUITOS_FREERTOS)
add_definitions(-DCIRCUITOS_LOVYANGFX)
add_definitions(-DLOVYAN_MANUAL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(BUILD ${ROOT}/build)

SET_SOURCE_FILES_PROPERTIES(${CMAKE_PROJECT_NAME}.ino PROPERTIES LANGUAGE CXX)

add_custom_command(OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.ino.elf
        COMMAND arduino-cli --config-file ~/.arduino15 -v compile --fqbn ${DEVICE} --output-dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_PROJECT_NAME}.ino
        DEPENDS ${CMAKE_PROJECT_NAME}.ino ${SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_target(CMBuild
        SOURCES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.ino.elf)

add_custom_target(CMUpload ALL
        COMMAND arduino-cli --config-file ~/.arduino15 -v upload --fqbn ${DEVICE} -p ${PORT} --input-dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        DEPENDS CMBuild
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME}.bin ${CMAKE_PROJECT_NAME}.ino ${SOURCES})
