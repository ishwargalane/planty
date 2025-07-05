#!/bin/bash

# macOS-specific monitor script for ESP-Rainmaker project

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

echo "📟 Monitoring ESP-Rainmaker project on macOS..."
echo "📱 Using port: $PORT"
echo "🔴 Press Ctrl+C to stop monitoring"
echo "📝 Serial output:"
echo "----------------------------------------"

# Monitor using host Python
python3 -m serial.tools.miniterm "$PORT" 115200 --eol CRLF 