/**
 * android_battery.c - Android battery information implementation
 *
 * Connects to the BatteryBridge Android app via TCP socket
 */

#include "include/battery_common.h"

#if defined(__ANDROID__)

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BRIDGE_PORT 8765

static int read_battery_data(int *level, int *charging, int *health, int *temp, int *voltage, char *tech, size_t tech_size)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(BRIDGE_PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sock);
        return -1;
    }

    char buffer[512] = {0};
    int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
    close(sock);

    if (bytes_read <= 0) {
        return -1;
    }

    // Parse JSON string: {"l":85,"c":1,"h":2,"t":35,"v":4120,"tech":"Li-ion"}
    int parsed = sscanf(buffer, "{\"l\":%d,\"c\":%d,\"h\":%d,\"t\":%d,\"v\":%d,\"tech\":\"%63[^\"]\"}", 
                        level, charging, health, temp, voltage, tech);

    if (parsed >= 2) {
        return 0; // Successfully parsed at least level and charging
    }

    return -1;
}

void android_battery_get_details(zval *return_value)
{
    zval batteries;
    array_init(&batteries);

    int level = -1, charging = -1, health = -1, temp = -1, voltage = -1;
    char tech[64] = {0};

    if (read_battery_data(&level, &charging, &health, &temp, &voltage, tech, sizeof(tech)) == 0) {
        zval bat;
        array_init(&bat);

        add_assoc_string(&bat, BATTERY_KEY_NAME, "main");

        /* Capacity level */
        if (level >= 0) {
            add_assoc_long(&bat, BATTERY_KEY_LEVEL, level);
        } else {
            add_assoc_null(&bat, BATTERY_KEY_LEVEL);
        }

        /* Charging status */
        if (charging >= 0) {
            add_assoc_bool(&bat, BATTERY_KEY_CHARGING, charging);
            add_assoc_string(&bat, BATTERY_KEY_STATUS,
                charging ? BATTERY_STATUS_CHARGING : BATTERY_STATUS_DISCHARGING);
        } else {
            add_assoc_null(&bat, BATTERY_KEY_CHARGING);
            add_assoc_null(&bat, BATTERY_KEY_STATUS);
        }

        add_next_index_zval(&batteries, &bat);

        /* Add batteries array */
        add_assoc_zval(return_value, BATTERY_KEY_BATTERIES, &batteries);

        /* Add aggregated values */
        BATTERY_ADD_INFO(return_value, level, charging);
    } else {
        /* Failed to connect or read from bridge */
        add_assoc_zval(return_value, BATTERY_KEY_BATTERIES, &batteries);
        BATTERY_ADD_NULL_INFO(return_value);
    }
}

int android_battery_level(void)
{
    int level = -1, charging = -1, health = -1, temp = -1, voltage = -1;
    char tech[64] = {0};
    if (read_battery_data(&level, &charging, &health, &temp, &voltage, tech, sizeof(tech)) == 0) {
        return level;
    }
    return -1;
}

int android_battery_is_charging(void)
{
    int level = -1, charging = -1, health = -1, temp = -1, voltage = -1;
    char tech[64] = {0};
    if (read_battery_data(&level, &charging, &health, &temp, &voltage, tech, sizeof(tech)) == 0) {
        return charging;
    }
    return -1;
}

#else

void android_battery_get_details(zval *return_value)
{
    BATTERY_INIT_EMPTY(return_value);
}

int android_battery_level(void)
{
    return -1;
}

int android_battery_is_charging(void)
{
    return -1;
}

#endif
