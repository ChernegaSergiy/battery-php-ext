cmake_minimum_required(VERSION 3.10)

project(battery_android)

add_library(battery_android SHARED
    android_battery.c
)

target_link_libraries(battery_android
    android
    log
)
