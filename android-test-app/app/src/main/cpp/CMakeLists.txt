cmake_minimum_required(VERSION 3.18.1)

project("battery_test")

add_definitions(-D__ANDROID__)

add_library( 
             battery_test
             SHARED
             native-lib.c
             android_battery.c )

find_library( 
              log-lib
              log )

target_link_libraries( 
                       battery_test
                       ${log-lib} )
