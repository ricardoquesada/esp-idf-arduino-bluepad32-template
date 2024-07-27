// SPDX-License-Identifier: Apache-2.0
// Copyright 2021 Ricardo Quesada
// http://retro.moe/unijoysticle2

#include "arduino_bootstrap.h"

#include <esp_task_wdt.h>

#include "sdkconfig.h"

#if (ARDUINO_USB_CDC_ON_BOOT | ARDUINO_USB_MSC_ON_BOOT | ARDUINO_USB_DFU_ON_BOOT) && !ARDUINO_USB_MODE
#include "USB.h"
#if ARDUINO_USB_MSC_ON_BOOT
#include "FirmwareMSC.h"
#endif
#endif

#ifndef ARDUINO_LOOP_STACK_SIZE
#ifndef CONFIG_ARDUINO_LOOP_STACK_SIZE
#define ARDUINO_LOOP_STACK_SIZE 8192
#else
#define ARDUINO_LOOP_STACK_SIZE CONFIG_ARDUINO_LOOP_STACK_SIZE
#endif
#endif

#if !CONFIG_AUTOSTART_ARDUINO

// Sanity check. It seems that when Arduino is added as a component, this define is present.
#if !defined(CONFIG_ENABLE_ARDUINO_DEPENDS)
#error \
    "Arduino not enabled as a component. Check: https://docs.espressif.com/projects/arduino-esp32/en/latest/esp-idf_component.html"
#endif

#include <Arduino.h>
#include <freertos/FreeRTOS.h>

static TaskHandle_t arduino_task_handle;

// Exported
bool loopTaskWDTEnabled;

__attribute__((weak)) size_t getArduinoLoopTaskStackSize() {
    return ARDUINO_LOOP_STACK_SIZE;
}

#if CONFIG_FREERTOS_UNICORE
static void yield_if_necessary(void) {
    static uint64_t last_yield = 0;
    uint64_t now = millis();
    if ((now - last_yield) > 2000) {
        last_yield = now;
        vTaskDelay(5);  // delay 1 RTOS tick
    }
}
#endif

extern "C" {
[[noreturn]] static void arduino_task(void* params) {
    setup();
    for (;;) {
#if CONFIG_FREERTOS_UNICORE
        yield_if_necessary();
#endif
        if (loopTaskWDTEnabled) {
            esp_task_wdt_reset();
        }
        loop();
        if (serialEventRun)
            serialEventRun();
    }
}

void arduino_bootstrap() {
#if ARDUINO_USB_CDC_ON_BOOT && !ARDUINO_USB_MODE
    Serial.begin();
#endif
#if ARDUINO_USB_MSC_ON_BOOT && !ARDUINO_USB_MODE
    MSC_Update.begin();
#endif
#if ARDUINO_USB_DFU_ON_BOOT && !ARDUINO_USB_MODE
    USB.enableDFU();
#endif
#if ARDUINO_USB_ON_BOOT && !ARDUINO_USB_MODE
    USB.begin();
#endif

    loopTaskWDTEnabled = false;
    initArduino();

    xTaskCreateUniversal(arduino_task, "ArduinoTask", getArduinoLoopTaskStackSize(), nullptr, 1, &arduino_task_handle,
                         CONFIG_ARDUINO_RUNNING_CORE);
    assert(arduino_task_handle != nullptr);
}
}  // extern "C"

// Public functions from Arduino Core that are disabled when CONFIG_ARDUINO_AUTOSTART is disabled.
// These are C++ (NOT C) functions.
void enableLoopWDT() {
    if (arduino_task_handle != nullptr) {
        if (esp_task_wdt_add(arduino_task_handle) != ESP_OK) {
            log_e("Failed to add loop task to WDT");
        } else {
            loopTaskWDTEnabled = true;
        }
    }
}

void disableLoopWDT() {
    if (arduino_task_handle != nullptr && loopTaskWDTEnabled) {
        loopTaskWDTEnabled = false;
        if (esp_task_wdt_delete(arduino_task_handle) != ESP_OK) {
            log_e("Failed to remove loop task from WDT");
        }
    }
}

void feedLoopWDT() {
    esp_err_t err = esp_task_wdt_reset();
    if (err != ESP_OK) {
        log_e("Failed to feed WDT! Error: %d", err);
    }
}

#endif // !CONFIG_AUTOSTART_ARDUINO
