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
    # Option A: using idf.py

    # Compile it
    cd ~/src/my_project
    idf.py build

    # Run it
    idf.py flash monitor
    ```

    Alternative, you can compile it using the "old" way. E.g:

    ```sh
    # Option B: using make (old way)

    # Compile it
    cd ~/src/my_project
    make -j

    # Run it
    make flash monitor
    ```

[toolchain-deps]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-setup.html
[ttyusb0]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html#linux-dialout-group

## Using 3rd party Arduino libraries

To include 3rd party Arduino libraries in your project, you have to:

* Add them to the `components` folder.
* Add a file `component.mk` inside the component's folder (Use this file as reference: [component.mk])

ESP-IDF requires that the compoonent's source code must be placed in the component's "root" folder.

Let's use two real 3rd party libraries as example:

[component.mk]: https://gitlab.com/ricardoquesada/esp-idf-arduino-bluepad32-template/-/blob/main/components/bluepad32_arduino/component.mk

### Example: Adding ESP32MotorControl

[ESP32MotorControl][esp32motorcontrol] has the source code in the "root" folder. This means that the source code is placed in the correct place. E.g:

```sh
# Add ESP32MotorControl into the components folder
cd components
git clone https://github.com/JoaoLopesF/ESP32MotorControl
```

The only thing that we have to do is to create the `component.mk` file. You can copy the one from `components/bluepad32_arduino/`. E.g:

```sh
# Finally add the component.mk file.
cd ESP32MotorControl
cp ../bluepad32_arduino/component.mk .
```

And that's it. Now you can include `ESP32MotorControl` from your code. E.g:

```cpp
// Add this include in your arduino_main.cpp file
#include <ESP32MotorControl.h>
```

[esp32motorcontrol]: https://github.com/JoaoLopesF/ESP32MotorControl

### Example: Adding ESP32Servo

[ESP32Servo] has the source code placed in `src`. So the source code must be moved (or copied) to the root folder. E.g:

```sh
# Add ESP32Servo into components folder
cd components
git clone https://github.com/madhephaestus/ESP32Servo.git

# And then move the "src" into "root"
cd ESP32Servo
mv src/* .
```

And as in the previous example, add the `component.mk` file.

```sh
# Finally add the component.mk file.
cd ESP32Servo
cp ../bluepad32_arduino/component.mk .
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