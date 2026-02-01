# battery_info PHP extension

[![Latest Stable Version](https://img.shields.io/packagist/v/chernegasergiy/battery-info.svg?label=Packagist&logo=packagist)](https://packagist.org/packages/chernegasergiy/battery-info)
[![Total Downloads](https://img.shields.io/packagist/dt/chernegasergiy/battery-info.svg?label=Downloads&logo=packagist)](https://packagist.org/packages/chernegasergiy/battery-info)
[![License](https://img.shields.io/packagist/l/chernegasergiy/battery-info.svg?label=Licence&logo=open-source-initiative)](https://packagist.org/packages/chernegasergiy/battery-info)
[![Build Linux](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-linux.yml/badge.svg)](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-linux.yml)
[![Build macOS](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-macos.yml/badge.svg)](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-macos.yml)
[![Build Windows](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-windows.yml/badge.svg)](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-windows.yml)

This extension exposes a single function: `battery_info()` which returns an array with keys:

- `batteries` (array) — array of all detected batteries, each containing:
  - `name` (string) — battery identifier (e.g., "BAT0", "BAT1")
  - `level` (int|null) — 0..100 or null if unknown
  - `charging` (bool|null) — true/false or null
  - `status` (string|null) — "charging"/"discharging"/"full"/"not charging"/null
- `level` (int|null) — primary battery level (0..100 or null)
- `charging` (bool|null) — primary battery charging state
- `status` (string|null) — primary battery status
- `platform` (string) — platform name ("linux", "macos", "windows", "android")

## Installation

### Install with PIE (Recommended)

The easiest way to install the extension is using **PIE (PHP Installer for Extensions)**:

```sh
pie install chernegasergiy/battery-info
```

### Manual Installation

1. **Build or Download** the extension binary for your OS.
2. **Locate your extensions directory**: `php -i | grep extension_dir`.
3. **Copy the file**:
   - Linux/macOS/Android: `battery_info.so`
   - Windows: `php_battery_info.dll`
4. **Enable in `php.ini`**:
   - Add `extension=battery_info` (or `extension=php_battery_info.dll` on Windows).

## Build (typical Unix/macOS)

```sh
phpize
./configure --enable-battery_info
make
sudo make install
```

## Usage Example

```php
<?php
if (!function_exists('battery_info')) {
    die("Extension not loaded");
}

$info = battery_info();

echo "Platform: " . $info['platform'] . "\n";

// Check all batteries
if (!empty($info['batteries'])) {
    foreach ($info['batteries'] as $battery) {
        echo "Battery: " . $battery['name'] . "\n";
        echo "  Level: " . ($battery['level'] ?? 'unknown') . "%\n";
        echo "  Status: " . ($battery['status'] ?? 'unknown') . "\n";
    }
} else {
    echo "No batteries detected.\n";
}

// Or use primary battery info directly
if ($info['level'] !== null) {
    echo "\nPrimary battery: " . $info['level'] . "% (" . $info['status'] . ")\n";
}
```

## Notes

- **Android** requires a small Java helper class and JNI glue — see `android_battery.c` comments.
- On **Linux** the extension reads `/sys/class/power_supply/*/capacity` and `status`.
- On **macOS** the extension uses IOKit (build with Xcode tools available).
- On **Windows** the extension uses `GetSystemPowerStatus`.

## Contributing

Contributions are welcome and appreciated! Here's how you can contribute:

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

Please make sure to update tests as appropriate and adhere to the existing coding style.

## License

This library is licensed under the CSSM Unlimited License v2.0 (CSSM-ULv2). See the [LICENSE](LICENSE) file for details.
