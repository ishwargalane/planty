#!/bin/bash

# This script helps flash the ESP32 from your host machine
# Usage: ./flash_script.sh [PORT]
# Example: ./flash_script.sh /dev/ttyUSB0

PORT=${1:-/dev/ttyUSB0}
echo "Using port: $PORT"

# Check if the port exists
if [ ! -e "$PORT" ]; then
  echo "Error: Port $PORT does not exist. Please connect your ESP32 device."
  echo "Available ports:"
  ls -l /dev/tty* | grep -E "USB|ACM"
  exit 1
fi

# Flash the firmware
echo "Flashing firmware to ESP32..."
python -m esptool --chip esp32 -p $PORT -b 460800 --before default_reset --after hard_reset write_flash \
  --flash_mode dio --flash_size 4MB --flash_freq 40m \
  0x1000 build/bootloader/bootloader.bin \
  0xc000 build/partition_table/partition-table.bin \
  0x16000 build/ota_data_initial.bin \
  0x20000 build/planty.bin

echo "Flashing complete!"
echo "If flashing was successful, your device should now restart with the new firmware." 