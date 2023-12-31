// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Ricardo Quesada
// http://retro.moe/unijoysticle2

#include "sdkconfig.h"

#include <stddef.h>

// BTstack related
#include <btstack_port_esp32.h>
#include <btstack_run_loop.h>

// Bluepad32 related
#include <arduino_platform.h>
#include <uni.h>

//
// Autostart
//
int app_main(void) {
    // hci_dump_open(NULL, HCI_DUMP_STDOUT);

    // Configure BTstack for ESP32 VHCI Controller
    btstack_init();

    // hci_dump_init(hci_dump_embedded_stdout_get_instance());

    // Must be called before uni_init()
    uni_platform_set_custom(get_arduino_platform());

    // Init Bluepad32.
    uni_init(0 /* argc */, NULL /* argv */);

    // Does not return.
    btstack_run_loop_execute();
    return 0;
}
