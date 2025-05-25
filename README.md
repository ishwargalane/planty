# Planty

An ESP-Rainmaker project for plant monitoring and control system.

## Development Environment

This project supports development using VS Code Dev Containers, providing a consistent development environment across Windows, macOS, and Linux. For detailed setup instructions, see [DEV_CONTAINER_USAGE.md](DEV_CONTAINER_USAGE.md).

### Quick Start

1. Install Docker Desktop and VS Code with the Dev Containers extension
2. Open the project in VS Code and click "Reopen in Container" when prompted
3. Use the provided VS Code tasks to build, flash, and monitor the device

## Build and Flash firmware

### Using VS Code Tasks (Recommended)

The easiest way to build and flash is using the built-in VS Code tasks:
1. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on macOS) to open the Command Palette
2. Type "Tasks: Run Task" and select it
3. Choose "Build" to compile or "Flash" to upload to your device

### Over-The-Air (OTA) Updates

This project supports OTA updates through ESP-Rainmaker:

1. Initially flash your device using USB (only needed once)
2. Provision the device with the ESP-Rainmaker mobile app
3. For subsequent updates, use the "OTA Update" VS Code task
4. Enter your device ID when prompted
5. The new firmware will be deployed wirelessly

This eliminates the need to physically connect to your device for each update.

### Using ESP-IDF Commands

Alternatively, follow the ESP RainMaker Documentation [Get Started](https://rainmaker.espressif.com/docs/get-started.html) section to build and flash this firmware manually.

```bash
# Build the project
idf.py build

# Flash to your device
idf.py -p <PORT> flash

# Monitor serial output
idf.py -p <PORT> monitor
```

## Pins Allocated
DHT21 -> tx2= GPIO17
Relay -> D21= GPIO21
Button-> D0 = GPIO0
sig -> VP   = GPIO36
sig0-> D15  = GPIO15 
sig1-> D13  = GPIO13
sig2-> D12  = GPIO12
sig3-> D14  = GPIO14

## Cross-Platform Development

This project can be synchronized between different machines and operating systems:

1. **Windows**: Uses COM ports (e.g., COM3) for device connection
2. **macOS**: Uses `/dev/tty.usbserial-*` ports for device connection
3. **Linux**: Uses `/dev/ttyUSB*` or `/dev/ttyACM*` ports for device connection

The VS Code "List Serial Ports" task will help identify the correct port on any platform.

### Reset to Factory

Press and hold the BOOT button for more than 3 seconds to reset the board to factory defaults. You will have to provision the board again to use it.
