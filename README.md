# battery_info PHP extension

[![Build Linux](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-linux.yml/badge.svg)](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-linux.yml)
[![Build macOS](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-macos.yml/badge.svg)](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-macos.yml)
[![Build Windows](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-windows.yml/badge.svg)](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-windows.yml)
[![Build Android](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-android.yml/badge.svg)](https://github.com/ChernegaSergiy/battery-php-ext/actions/workflows/build-android.yml)

This extension exposes a single function: `battery_info()` which returns an array with keys:

- `level` (int|null) — 0..100 or null if unknown
- `charging` (bool|null) — true/false or null
- `status` (string|null) — "charging"/"discharging"/null
- `platform` (string) — platform name

Build (typical Unix/macOS):

```sh
phpize
./configure --enable-battery_info
make
sudo make install
```

Notes:

- Android requires a small Java helper class and JNI glue — see `android_battery.c` comments.
- On Linux the extension reads `/sys/class/power_supply/*/capacity` and `status`.
- On macOS the extension uses IOKit (build with Xcode tools available).
- On Windows the extension uses `GetSystemPowerStatus`.

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