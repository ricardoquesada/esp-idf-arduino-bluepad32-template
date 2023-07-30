/****************************************************************************
http://retro.moe/unijoysticle2

Copyright 2019 Ricardo Quesada

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****************************************************************************/

#include "uni_platform_unijoysticle_2.h"

#include "sdkconfig.h"
#include "uni_common.h"
#include "uni_config.h"
#include "uni_log.h"

// Unijoysticle v2: Through-hole version
static const struct uni_platform_unijoysticle_gpio_config gpio_config_2 = {
    .port_a = {GPIO_NUM_26, GPIO_NUM_18, GPIO_NUM_19, GPIO_NUM_23, GPIO_NUM_14, GPIO_NUM_33, GPIO_NUM_16},
    .port_b = {GPIO_NUM_27, GPIO_NUM_25, GPIO_NUM_32, GPIO_NUM_17, GPIO_NUM_12, -1, -1},
    .leds = {GPIO_NUM_5, GPIO_NUM_13, -1},
    .push_buttons = {{
                         .gpio = GPIO_NUM_10,
                         .callback = uni_platform_unijoysticle_on_push_button_mode_pressed,
                     },
                     {
                         .gpio = -1,
                         .callback = NULL,
                     }},
    .sync_irq = {-1, -1},
};

//
// Variant overrides
//

const struct uni_platform_unijoysticle_variant* uni_platform_unijoysticle_2_create_variant(void) {
    const static struct uni_platform_unijoysticle_variant variant = {
        .name = "2",
        .gpio_config = &gpio_config_2,
        .flags = UNI_PLATFORM_UNIJOYSTICLE_VARIANT_FLAG_QUADRATURE_MOUSE,
        .supported_modes =
            UNI_PLATFORM_UNIJOYSTICLE_GAMEPAD_MODE_NORMAL | UNI_PLATFORM_UNIJOYSTICLE_GAMEPAD_MODE_TWINSTICK,
        .default_mouse_emulation = UNI_PLATFORM_UNIJOYSTICLE_MOUSE_EMULATION_AMIGA,
    };

    return &variant;
}