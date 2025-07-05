#!/bin/bash

# Monitor script for ESP-Rainmaker project using dev container

set -e

# Get the port (default to /dev/ttyUSB0 on Linux, /dev/tty.usbserial-* on macOS)
PORT=${1:-/dev/ttyUSB0}

# On macOS, try to find the USB serial device
if [[ "$OSTYPE" == "darwin"* ]]; then
    # Look for common USB serial devices on macOS
    POTENTIAL_PORTS=$(ls /dev/tty.usbserial-* /dev/tty.SLAB_USBtoUART* /dev/tty.wchusbserial* 2>/dev/null | head -1)
    if [ -n "$POTENTIAL_PORTS" ]; then
        PORT="$POTENTIAL_PORTS"
    else
        echo "⚠️  No USB serial device found on macOS. Available devices:"
        ls /dev/tty.* 2>/dev/null | grep -E "(usb|serial)" || echo "No USB/serial devices found"
        echo "Please specify the port: $0 /dev/tty.usbserial-XXXXXX"
        exit 1
    fi
fi

# Check if port exists
if [ ! -e "$PORT" ]; then
    echo "❌ Error: Port $PORT not found."
    echo "💡 Usage: $0 [port]"
    echo "📋 Available ports:"
    if [[ "$OSTYPE" == "darwin"* ]]; then
        ls /dev/tty.* 2>/dev/null | grep -E "(usb|serial)" || echo "No USB/serial devices found"
    else
        ls -l /dev/tty* 2>/dev/null | grep -E "(USB|ACM)" || echo "No USB/ACM ports found"
    fi
    exit 1
fi

echo "📟 Monitoring ESP-Rainmaker project using dev container..."
echo "📱 Using port: $PORT"
echo "🔴 Press Ctrl+C to stop monitoring"

# Monitor the project
docker run --rm -it \
    -v "$(pwd):/workspaces/planty" \
    -w /workspaces/planty \
    --privileged \
    --device="$PORT" \
    planty-dev \
    bash -c "
        # Source ESP-IDF environment
        source /opt/esp/idf/export.sh
        
        # Set ESP-Rainmaker path
        export RMAKER_PATH=/opt/esp/rainmaker
        
        # Monitor the project
        idf.py -p $PORT monitor
    " 