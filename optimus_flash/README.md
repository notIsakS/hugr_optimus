# Main MCU flash script

This Arduino program parses serial data, and has a watchdog timer incase serial commands is not recieved in-time.

## PlatformIO

Uses PlatformIO to flash MCU. Reason being it is easy to install dependencies and native in VScode.

### Install all dependencies

```bash
pio pkg install
```

## Known errors

- **Linux:** If you get "permission denied" on the serial port, add the PlatformIO udev rules and add your user to the `dialout` group:

  ```bash
  curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
  sudo udevadm control --reload-rules && sudo udevadm trigger
  sudo usermod -aG dialout $USER   # log out and back in afterwards
  ```
