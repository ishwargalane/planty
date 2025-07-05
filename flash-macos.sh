#!/bin/bash

# macOS-specific flash script for ESP-Rainmaker project
# Uses container for building, host esptool for flashing

set -e

# Get the port (auto-detect on macOS)
PORT=${1:-$(ls /dev/tty.usbserial-* /dev/tty.SLAB_USBtoUART* /dev/tty.wchusbserial* 2>/dev/null | head -1)}

# Check if port exists
if [ ! -e "$PORT" ]; then
    echo "❌ Error: No ESP32 device found."
    echo "💡 Usage: $0 [port]"
    echo "📋 Available ports:"
    ls /dev/tty.* 2>/dev/null | grep -E "(usb|serial)" || echo "No USB/serial devices found"
    exit 1
fi

echo "⚡ Flashing ESP-Rainmaker project on macOS..."
echo "📱 Using port: $PORT"

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "❌ Build directory not found. Please run ./build.sh first"
    exit 1
fi

# Check if required binary files exist
required_files=(
    "build/bootloader/bootloader.bin"
    "build/partition_table/partition-table.bin"
    "build/ota_data_initial.bin"
    "build/planty.bin"
)

for file in "${required_files[@]}"; do
    if [ ! -f "$file" ]; then
        echo "❌ Required file not found: $file"
        echo "Please run ./build.sh first"
        exit 1
    fi
done

echo "🔥 Flashing firmware..."

# Flash using host esptool
python3 -m esptool \
    --chip esp32 \
    -p "$PORT" \
    -b 460800 \
    --before default_reset \
    --after hard_reset \
    write_flash \
    --flash_mode dio \
    --flash_freq 40m \
    --flash_size 4MB \
    0x1000 build/bootloader/bootloader.bin \
    0xc000 build/partition_table/partition-table.bin \
    0x16000 build/ota_data_initial.bin \
    0x20000 build/planty.bin

echo "✅ Flash complete!"
echo "📋 To monitor output: ./monitor-macos.sh $PORT"
echo "🔄 Device should restart automatically..." 