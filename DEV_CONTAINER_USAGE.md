# ESP-Rainmaker Project Dev Container Setup

This project is configured to use VS Code Dev Containers to provide a consistent development environment for ESP-Rainmaker projects across Windows, macOS, and Linux.

## Prerequisites

1. Install [Docker Desktop](https://www.docker.com/products/docker-desktop/)
2. Install [Visual Studio Code](https://code.visualstudio.com/)
3. Install the [Remote - Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension in VS Code

## Getting Started

1. Open this project folder in VS Code
2. When prompted to "Reopen in Container", click "Reopen in Container"
3. Alternatively:
   - Press `F1` to open the Command Palette
   - Type "Remote-Containers: Reopen in Container" and select it

The first time you open the project in a container, it will build the development environment. This might take several minutes, but subsequent launches will be faster.

## Platform-Specific Notes

### Windows

1. **Serial Port Access:**
   - In Docker Desktop, go to Settings > Resources > WSL Integration and ensure it's enabled
   - When connecting your ESP32 device, note the COM port number (e.g., COM3)
   - When using the Flash or Monitor tasks, enter the COM port when prompted (e.g., COM3)

2. **Building and Flashing:**
   - All VS Code tasks work the same way on Windows
   - Use the "List Serial Ports" task to identify available COM ports
   - Windows may require additional USB drivers for your specific ESP32 development board

### macOS

1. **Serial Port Access:**
   - Serial ports appear as `/dev/tty.usbserial-*` or `/dev/tty.SLAB_USBtoUART`
   - When connecting your ESP32 device, use the "List Serial Ports" task to identify the correct port
   - Docker on macOS may have limitations accessing USB devices directly; the tasks will fall back to using your local ESP-IDF installation

2. **Building and Flashing:**
   - If using Docker for flashing fails, the system will automatically use your local ESP-IDF installation
   - Make sure ESP-IDF v5.4 is installed locally using the standard [ESP-IDF installation guide](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html)

### Linux

1. **Serial Port Access:**
   - Serial ports appear as `/dev/ttyUSB*` or `/dev/ttyACM*`
   - You may need to add your user to the `dialout` group to access serial ports: `sudo usermod -a -G dialout $USER`
   - Log out and log back in for the group change to take effect

2. **Building and Flashing:**
   - All VS Code tasks work seamlessly on Linux
   - Use the "List Serial Ports" task to identify available ports

## VS Code Tasks

This project includes the following VS Code tasks:

- **Build**: Compile the project
- **Flash**: Upload the firmware to the ESP32
- **Monitor**: View the serial output from the device
- **Flash and Monitor**: Flash and then immediately start monitoring
- **Clean**: Remove all build artifacts
- **Menuconfig**: Configure ESP-IDF parameters through a menu interface
- **List Serial Ports**: Show available serial ports on your system

To run any task:
1. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on macOS) to open the Command Palette
2. Type "Tasks: Run Task" and select it
3. Choose the task you want to run

## Cross-Platform Development

The configuration provided in this project makes it easy to share code between team members using different operating systems:

1. All build settings are consistently applied across platforms
2. Docker containers provide identical build environments regardless of the host OS
3. When Docker cannot access serial devices (particularly on macOS), the system automatically falls back to local ESP-IDF installations
4. Source code can be synced via Git without worrying about platform-specific issues

When transferring the project to a new machine:
1. Clone the repository
2. Open in VS Code with Dev Containers extension
3. Select "Reopen in Container"
4. Use the provided tasks to build and flash

## Troubleshooting

- **Serial Port Not Found**: Use the "List Serial Ports" task to verify your device is connected and detected
- **Build Errors**: Run the "Clean" task and try building again
- **Docker Issues**: Ensure Docker Desktop is running before opening the project
- **Flash Fails on macOS**: Make sure you have ESP-IDF v5.4 installed locally

## Project Structure

- `main/` - Main application code
- `components/` - Custom components (dht21, csms_v2)
- `managed_components/` - Components managed by ESP-IDF component manager
- `.devcontainer/` - Container configuration files

## Additional Resources

- [ESP-Rainmaker Documentation](https://rainmaker.espressif.com/docs/get-started.html)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html)
- [VS Code Dev Containers](https://code.visualstudio.com/docs/remote/containers) 