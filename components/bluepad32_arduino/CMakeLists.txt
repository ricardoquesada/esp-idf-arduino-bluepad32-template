include(CMakePrintHelpers)

set(srcs
        "ArduinoBluepad32.cpp"
        "ArduinoConsole.cpp"
        "ArduinoController.cpp"
        "ArduinoGamepad.cpp"
        "arduino_bootstrap.cpp"
        "arduino_platform.c")

 set(requires
         arduino
         bluepad32
         btstack)


idf_component_register(SRCS "${srcs}"
                INCLUDE_DIRS "include"
                REQUIRES ${requires})

 # Bluepad32 contains reference to setup() and loop().  If we add main
 # component to priv_requires then we create a large circular dependency
 # (bluepad32 -> main -> bluepad32) and can get linker errors, so
 # instead we add setup() and loop() to the undefined symbols list so the
 # linker will always include them.
 #
 # (As they are C++ symbol, we need to add the C++ mangled names.)
 set(ext_symbols "-u _Z6setupv -u _Z4loopv")

string(REPLACE ";" " " ext_symbols "${ext_symbols}")
target_link_libraries(${COMPONENT_LIB} INTERFACE ${ext_symbols})
