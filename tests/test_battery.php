<?php
if (!function_exists('battery_info')) {
    echo "Extension not loaded\n";
    exit(1);
}

$info = battery_info();

// Basic assertions
if (!array_key_exists('level', $info)) {
    echo "FAIL: Missing 'level' key\n";
    exit(1);
}
if (!array_key_exists('charging', $info)) {
    echo "FAIL: Missing 'charging' key\n";
    exit(1);
}
if (!array_key_exists('status', $info)) {
    echo "FAIL: Missing 'status' key\n";
    exit(1);
}
if (!array_key_exists('platform', $info)) {
    echo "FAIL: Missing 'platform' key\n";
    exit(1);
}

echo "PASS: Structure check passed\n";
print_r($info);

