set(srcs
        "arduino_bootstrap.cpp"
        "arduino_platform.c"
        "main.c"
        "sketch.cpp"
        "api/ArduinoBluepad32.cpp"
        "api/ArduinoConsole.cpp"
        "api/ArduinoController.cpp"
        "api/ArduinoGamepad.cpp"
)

set(requires "bluepad32" "arduino" "btstack")

if(NOT CMAKE_BUILD_EARLY_EXPANSION)
    include_directories(AFTER
            api/include)
endif()

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}")
