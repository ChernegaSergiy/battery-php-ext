<?php
// Example usage of the extension once built and loaded
if (!function_exists('battery_info')) {
    echo "Extension not loaded\n";
    exit(1);
}

$info = battery_info();
print_r($info);
