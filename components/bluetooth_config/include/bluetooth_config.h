#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"

#include "esp_log.h"
#include "esp_err.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "esp_vfs.h"


#define SPP_TAG "SPP_DEBUG"
#define GAP_TAG "GAP_DEBUG"
#define SPP_SERVER_NAME "ESK8_SERVER"

static const esp_spp_sec_t sec_mask = ESP_SPP_SEC_NONE;
static const esp_spp_role_t role = ESP_SPP_ROLE_MASTER;
static const char local_device_name[] = "ESK8";

static esp_err_t ret;

esp_err_t nvs_init();
esp_err_t bt_controller_init();
esp_err_t bt_bluedroid_init();
esp_err_t bt_gap_init();
esp_err_t bt_spp_init();

