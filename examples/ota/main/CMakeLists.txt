set(srcs "main.c" "sketch.cpp" "ble_server.cpp")

set(requires "bluepad32")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}")
