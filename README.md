# ESP-IDF + Arduino + Bluepad32 template app

[![discord](https://img.shields.io/discord/775177861665521725.svg)](https://discord.gg/r5aMn6Cw5q)

![logo](https://gitlab.com/ricardoquesada/bluepad32-arduino/-/raw/main/img/bluepad32-arduino-logo.png)

This is a template application to be used with [Espressif IoT Development Framework](https://github.com/espressif/esp-idf).

Please check [ESP-IDF docs](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for getting started instructions.

Requires ESP-IDF v4.4 or newer.

Includes the following ESP-IDF components, with a pre-configured `sdkconfig` file:

* [Arduino Core for ESP32](https://github.com/espressif/arduino-esp32) component
* [Bluepad32](https://gitlab.com/ricardoquesada/bluepad32/) component
* [BTStack](https://github.com/bluekitchen/btstack) component

## How to compile it

Requirements: **Linux** (**macOS** might work as well). It should be possible to compile ESP-IDF + Bluepad32 on Windows. I justs don't have one to test it. So if you have Windows and don't know how to do it yourself, install a VM with Linux and follow these instructions:

1. Requirements and permissions

    Install ESP-IDF dependencies (taken from [here][toolchain-deps]):

    ```sh
    # For Ubuntu / Debian
    sudo apt-get install git wget flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
    ```

    And in case you don't have permissions to open `/dev/ttyUSB0`, do:
    (taken from [here][ttyusb0])

    ```sh
    # You MUST logout/login (or in some cases reboot Linux) after running this command
    sudo usermod -a -G dialout $USER
    ```

2. Install and setup ESP-IDF

    ```sh
    # Needs to be done just once
    # Clone the ESP-IDF git repo
    mkdir ~/esp && cd ~/esp
    git clone -b release/v4.4 --recursive https://github.com/espressif/esp-idf.git

    # Then install the toolchain
    cd ~/esp/esp-idf
    ./install.sh
    ```

3. Compile the template

    Clone the template:

    ```sh
    # Do it everytime you want to start a new project
    # Clone the template somewhere
    mkdir ~/src && cd ~/src
    git clone --recursive https://gitlab.com/ricardoquesada/esp-idf-arduino-bluepad32-template.git my_project
    ```

    Export the ESP-IDF environment variables in your shell:

    ```sh
    # Do it everytime you open a new shell
    # Optional: add it in your ~/.bashrc or ~/.profile
    source ~/esp/esp-idf/export.sh
    ```

    And finally compile and install your project.

    ```sh
    # Option A: using make

    # Compile it
    cd ~/src/my_project
    make -j

    # Run it
    make flash monitor
    ```

    Alternative, you can compile it using the "new" way, but experimental. E.g:

    ```sh
    # Option A: using idf.py (new way, experimental)

    # Compile it
    cd ~/src/my_project
    idf.py build

    # Run it
    idf.py flash monitor
    ```


[toolchain-deps]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-setup.html
[ttyusb0]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html#linux-dialout-group

## Using 3rd party Arduino libraries

To include 3rd party Arduino libraries in your project, you have to:

* Add them to the `components` folder.
* Add a file `component.mk` and/or `CMakeLists.txt` inside the component's folder

`component.mk` is needed if you use `make` to compile it. And `CMakeLists.txt` is needed if you use `idf.py` to compile it.

Let's use two real 3rd party libraries as example:

### Example: Adding ESP32MotorControl

[ESP32MotorControl][esp32motorcontrol] has the source code in the "root" folder. So we have to add a `component.mk` and/or `CMakeLists.txt` with that in mind. Let's see:

```sh
# Add ESP32MotorControl into the components folder
cd components
git clone https://github.com/JoaoLopesF/ESP32MotorControl
cd ESP32MotorControl
```

And now create create these files files inside `components/ESP32MotorControl` folder:

```sh
# Create component.mk file
# Needed if you use "make" to compile the project
# Copy & paste the following lines to the terminal:
cat << EOF > component.mk
COMPONENT_ADD_INCLUDEDIRS := .
EOF
```

```sh
# Create CMakeLists.txt file
# Needed if you use "idf.py" to compile the project
# Copy & paste the following lines to the terminal:
cat << EOF > CMakeLists.txt
idf_component_register(SRC_DIRS "."
                    INCLUDE_DIRS "."
                    REQUIRES "arduino")
EOF
```

Finally, if you use `idf.py`, you have to update the dependencies in the `main/CMakeLists.txt`. E.g:

```sh
# Needed if you use "idf.py" to compile the project
cd main
edit CMakeLists.txt
```

...and append `ESP32MotorControl` to `REQUIRES`. The `main/CMakeLists.txt` should look like this:

```cmake
idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}" "ESP32MotorControl")
```

And that's it. Now you can include `ESP32MotorControl` from your code. E.g:

```cpp
// Add this include in your arduino_main.cpp file
#include <ESP32MotorControl.h>
```

[esp32motorcontrol]: https://github.com/JoaoLopesF/ESP32MotorControl

### Example: Adding ESP32Servo

[ESP32Servo] has the source code placed in `src`. So the we have to make `component.mk` and/or `CMakeLists.txt` to point to that folder. Let's see:

```sh
# Add ESP32Servo into components folder
cd components
git clone https://github.com/madhephaestus/ESP32Servo.git
cd ESP32Servo
```

And now create create these files files inside `components/ESP32Servo` folder:

```sh
# Create component.mk file
# Needed if you use "make" to compile the project
# Copy & paste the following lines to the terminal:
cat << EOF > component.mk
COMPONENT_ADD_INCLUDEDIRS := src
COMPONENT_SRCDIRS := src
EOF
```

```sh
# Create CMakeLists.txt file
# Needed if you use "idf.py" to compile the project
# Copy & paste the following lines to the terminal:
cat << EOF > CMakeLists.txt
idf_component_register(SRC_DIRS "src"
                    INCLUDE_DIRS "src"
                    REQUIRES "arduino")
EOF
```

Finally, if you use `idf.py`, you have to update the dependencies in the `main/CMakeLists.txt`. E.g:

```sh
# Needed if you use "idf.py" to compile the project
cd main
edit CMakeLists.txt
```

...and append `ESP32Servo` to `REQUIRES`. The `main/CMakeLists.txt` should look like this:

```cmake
idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}" "ESP32Servo")
```

And that's it. Now you can include `ESP32Servo` from your code. E.g:

```cpp
// Add this include in your arduino_main.cpp file
#include <ESP32Servo.h>
```

[esp32servo]: https://github.com/madhephaestus/ESP32Servo.git

## Further info

* [Bluepad32 for Arduino](https://gitlab.com/ricardoquesada/bluepad32/-/blob/main/docs/plat_arduino.md)
* [Arduino as ESP-IDF component](https://docs.espressif.com/projects/arduino-esp32/en/latest/esp-idf_component.html)

## Support

* [Discord][discord]: any question? Ask them in our Discord server.

[discord]: https://discord.gg/r5aMn6Cw5q