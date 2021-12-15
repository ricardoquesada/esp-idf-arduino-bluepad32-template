set(srcs "arduino_main.cpp"
         "main.c")
     
set(requires "arduino"
             "bluepad32"
             "bluepad32_arduino")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}")
