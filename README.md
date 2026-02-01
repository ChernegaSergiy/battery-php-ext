# battery_info PHP extension

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

## License

This library is licensed under the CSSM Unlimited License v2.0 (CSSM-ULv2). See the [LICENSE](LICENSE) file for details.