# ESP32 Flashing Instructions

This document provides instructions for flashing your ESP32 device with the latest firmware using a wired connection.

## Prerequisites

1. ESP32 device connected to your computer via USB
2. Python and esptool installed on your host machine
3. The firmware binary files (already built in the `build` directory)

## Installing esptool (if not already installed)

```bash
pip install esptool
```

## Flashing the Device

### Option 1: Using the provided script

1. Connect your ESP32 device to your computer via USB
2. Identify the serial port (e.g., `/dev/ttyUSB0` on Linux, `COM3` on Windows)
3. Run the flashing script with your port:

```bash
./flash_script.sh /dev/ttyUSB0  # Replace with your port
```

### Option 2: Manual flashing

You can also flash the device manually using the following command:

```bash
python -m esptool --chip esp32 -p /dev/ttyUSB0 -b 460800 --before default_reset --after hard_reset write_flash \
  --flash_mode dio --flash_size 4MB --flash_freq 40m \
  0x1000 build/bootloader/bootloader.bin \
  0xc000 build/partition_table/partition-table.bin \
  0x16000 build/ota_data_initial.bin \
  0x20000 build/planty.bin
```

Replace `/dev/ttyUSB0` with your actual port.

## Troubleshooting

If you encounter issues:

1. Make sure the ESP32 is properly connected
2. Verify you have the correct port
3. Try pressing the BOOT button on your ESP32 during the initial connection phase
4. If using Windows, ensure you have installed the appropriate USB-to-UART drivers

After flashing, the device will restart with the new firmware (version 1.3). 