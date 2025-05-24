# ESP-Rainmaker Dev Container

This directory contains configuration for a development container that sets up a complete environment for ESP-Rainmaker project development.

## Features

- ESP-IDF v5.4.0
- ESP-Rainmaker SDK
- All required compilation tools and dependencies
- Pre-configured for ESP32 target

## Requirements

- Docker Desktop installed and running
- Visual Studio Code with the "Remote - Containers" extension installed

## How to Use

1. Open this project folder in Visual Studio Code
2. When prompted to "Reopen in Container", click "Reopen in Container"
3. Alternatively, click on the green icon in the bottom-left corner and select "Reopen in Container"
4. Wait for the container to build and start (this may take some time on first run)
5. Once the container is running, you can build and flash the project:

```bash
# Build the project
idf.py build

# Flash to ESP32 device (when connected via USB)
idf.py -p /dev/ttyUSB0 flash

# Monitor the device output
idf.py -p /dev/ttyUSB0 monitor
```

## USB Device Access

To enable access to USB devices (for flashing and monitoring), the container runs with the `--privileged` flag. Make sure your user has the necessary permissions to access USB devices.

## Environment Variables

- `IDF_PATH`: Set to `/opt/esp/idf`
- `RMAKER_PATH`: Set to `/opt/esp/rainmaker`
- `IDF_TARGET`: Set to `esp32`

## Troubleshooting

If you encounter USB device access issues:
1. Exit the container
2. Ensure your user has appropriate USB access permissions on the host
3. Reconnect your device
4. Reopen the project in the container

For WSL users on Windows, additional configuration may be needed to pass through USB devices. 