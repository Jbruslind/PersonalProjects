
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include <string.h>

#include "esp_system.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "cmd_decl.h"
#include "esp_vfs_fat.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "temp_task.h"

#define TEMP_TASK_STACK_SIZE    3000
#define CONSOLE_TASK_STACK_SIZE 4000

#define CONFIG_CONSOLE_STORE_HISTORY 0

#define BUF_SIZE (1024)

#ifdef CONFIG_ESP_CONSOLE_USB_CDC
#error This example is incompatible with USB CDC console. Please try "console_usb" example instead.
#endif // CONFIG_ESP_CONSOLE_USB_CDC

static const char* TAG = "example";
#define PROMPT_STR CONFIG_IDF_TARGET

/* Console command history can be stored to and loaded from a file.
 * The easiest way to do this is to use FATFS filesystem on top of
 * wear_levelling library.
 */
#if CONFIG_CONSOLE_STORE_HISTORY

#define MOUNT_PATH "/data"
#define HISTORY_PATH MOUNT_PATH "/history.txt"

static void initialize_filesystem(void)
{
    static wl_handle_t wl_handle;
    const esp_vfs_fat_mount_config_t mount_config = {
            .max_files = 4,
            .format_if_mount_failed = true
    };
    esp_err_t err = esp_vfs_fat_spiflash_mount(MOUNT_PATH, "storage", &mount_config, &wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return;
    }
}
#endif // CONFIG_STORE_HISTORY

static void initialize_nvs(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void app_main(void)
{   
    
    // gpio_config_t io_conf;
    // io_conf.intr_type = GPIO_INTR_DISABLE; //disable interrupt
    // io_conf.mode = GPIO_MODE_OUTPUT; //set as output mode
    // io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL; //bit mask of the pins that you want to set,e.g.GPIO18/19
    // io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; //disable pull-down mode
    // io_conf.pull_up_en = GPIO_PULLUP_ENABLE; //enable pull-up mode
    // gpio_config(&io_conf);  //configure GPIO with the given settings
    
    // gpio_set_level(PWOK_L, 0x01); //Ensure pins enabling PSU are pulled UP (disable main output)
    // gpio_set_level(PSON_L, 0x01);

     xTaskCreate(temp_task, "temp_task", TEMP_TASK_STACK_SIZE, NULL, 10, NULL); //Create a task for the I2C register calls
    
    // xTaskCreate(asdisplaytask, "display_task", TM1637_TASK_STACK_SIZE, NULL, 10, NULL); //Create a task for the TM1637 displays

    //All this stuff is setting up the REPL system ********************************
    esp_console_repl_t *repl = NULL; 
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    /* Prompt to be printed before each line.
        * This can be customized, made dynamic, etc.
        */
    repl_config.prompt = PROMPT_STR ">";
    repl_config.task_priority = 5; 

    initialize_nvs();

    #if CONFIG_CONSOLE_STORE_HISTORY
    initialize_filesystem();
    repl_config.history_save_path = HISTORY_PATH;
    ESP_LOGI(TAG, "Command history enabled");
    #else
    ESP_LOGI(TAG, "Command history disabled");
    #endif

    /* Register commands */
    esp_console_register_help_command(); //Registers the help command (lists all other registered commands)
    register_system(); //Register a set of basic system commands (available memory, restart, sleep device)
    // register_StreamData(); //Custom command set for Delta environment (start psu, status, etc)

    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl)); //Setup the REPL console on a UART port

    ESP_ERROR_CHECK(esp_console_start_repl(repl)); //Start REPL environment
    //End REPL Setup ***********************************************************
}
