#include "ble_server.h"

const char *test_string = "Delayed response";
const uint8_t adv_data[] = {
    // Flags general discoverable, BR/EDR not supported
    0x02,
    0x01,
    0x06,
    // Name
    0x08,
    0x09,
    'D',
    'e',
    'l',
    'a',
    'y',
    'e',
    'd',
};
const uint8_t adv_data_len = sizeof(adv_data);

bool updateFlag = false;
esp_ota_handle_t otaHandler = 0;

static uint16_t att_read_callback(hci_con_handle_t con_handle,
                                  uint16_t att_handle,
                                  uint16_t offset,
                                  uint8_t *buffer,
                                  uint16_t buffer_size);
static int att_write_callback(hci_con_handle_t connection_handle,
                              uint16_t att_handle,
                              uint16_t transaction_mode,
                              uint16_t offset,
                              uint8_t *buffer,
                              uint16_t buffer_size);

void BLE_SERVER_SETUP()
{
    // setup ATT server
    att_server_init(profile_data, att_read_callback, att_write_callback);

    // setup advertisements
    uint16_t adv_int_min = 0x0030;
    uint16_t adv_int_max = 0x0030;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
    gap_advertisements_set_data(adv_data_len, (uint8_t *)adv_data);
    gap_advertisements_enable(1);
}

/*
 * The ATT Server handles all reads to constant data. For dynamic data like the custom characteristic, the
 * registered att_read_callback is called. To handle long characteristics and long reads, the att_read_callback is first
 * called with buffer == NULL, to request the total value length. Then it will be called again requesting a chunk of the
 * value.
 */

// ATT Client Read Callback for Dynamic Data
// - if buffer == NULL, don't copy data, just return size of value
// - if buffer != NULL, copy data and return number bytes copied
// @param offset defines start of attribute value
static uint16_t att_read_callback(hci_con_handle_t connection_handle,
                                  uint16_t att_handle,
                                  uint16_t offset,
                                  uint8_t *buffer,
                                  uint16_t buffer_size)
{
    UNUSED(connection_handle);

    if (att_handle == ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE)
    {
        printf("Responding right away");
        return att_read_callback_handle_blob((const uint8_t *)test_string, (uint16_t)strlen(test_string), offset, buffer,
                                             buffer_size);
    }

    return 0;
}

/*
    Any write to our custom characteristic triggers the OTA update process.

    The first write after boot is considered the start of the process.

    All subsequent writes append binary data, the Web App sends data in blocks of 244.

    Any block received less than that block size is considered the final block and terminates the OTA process rebooting
   the ESP.

    A more robust mechanism would be to have a second custom characteristic where the UI could indicate states
    (begin OTA, End OTA, apply OTA etc).
*/
static int att_write_callback(hci_con_handle_t connection_handle,
                              uint16_t att_handle,
                              uint16_t transaction_mode,
                              uint16_t offset,
                              uint8_t *buffer,
                              uint16_t buffer_size)
{
    UNUSED(transaction_mode);
    UNUSED(offset);
    UNUSED(connection_handle);

    if (att_handle == ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE)
    {
        if (!updateFlag)
        {
            printf(" Begin OTA");
            esp_err_t resp = esp_ota_begin(esp_ota_get_next_update_partition(NULL), OTA_SIZE_UNKNOWN, &otaHandler);

            if (resp != ESP_OK)
            {
                printf(" Error starting OTA");
            }

            updateFlag = true;
        }

        if (buffer_size > 0)
        {
            printf(" Writing Block");
            esp_err_t respr = esp_ota_write(otaHandler, buffer, buffer_size);

            if (respr != ESP_OK)
            {
                printf(" Error writing OTA Block");
            }

            // Consider the OTA complete if the block is smaller than the blocksize
            // There is an edge case if the final block is exactly 244
            if (buffer_size != 244)
            {
                printf(" Calling OTA End");

                if (ESP_OK != esp_ota_end(otaHandler))
                {
                    printf(" Error ending OTA");
                }
                else
                {
                    printf(" OTA has finished");
                }

                if (ESP_OK == esp_ota_set_boot_partition(esp_ota_get_next_update_partition(NULL)))
                {
                    delay(2000);
                    esp_restart();
                }
                else
                {
                    printf(" Setting boot Partition Error");
                }
            }
        }
    }

    return 0;
}