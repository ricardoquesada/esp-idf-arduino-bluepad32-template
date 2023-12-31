set(srcs "main.c" "sketch.cpp" "ble_server.cpp")

set(requires "bluepad32" "bluepad32_arduino" "arduino")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}")
