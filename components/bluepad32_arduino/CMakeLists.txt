set(srcs "ArduinoBluepad32.cpp"
         "ArduinoGamepad.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "arduino"
                             "bluepad32")
