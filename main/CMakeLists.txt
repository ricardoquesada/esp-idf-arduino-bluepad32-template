set(srcs
        "main.c"
        "sketch.cpp")

set(requires "bluepad32" "btstack")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}")
