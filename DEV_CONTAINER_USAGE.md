# ESP-Rainmaker Project Dev Container Setup

This project is configured to use VS Code Dev Containers to provide a consistent development environment for ESP-Rainmaker projects.

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

The first time you open the project in a container, it will build the development environment, which may take several minutes.

## Building and Flashing

Once the container is running, you can use the integrated terminal in VS Code to build and flash the project:

```bash
# Build the project
idf.py build

# Flash (replace /dev/ttyUSB0 with your device's port)
idf.py -p /dev/ttyUSB0 flash

# Monitor serial output
idf.py -p /dev/ttyUSB0 monitor

# Build, flash and monitor in one command
idf.py -p /dev/ttyUSB0 flash monitor
```

## Debugging Hardware Connection

If you're having trouble with the USB connection to your ESP device:

1. Make sure your device is connected to your computer
2. For macOS, check the port with: `ls -l /dev/cu.*`
3. For Linux, check with: `ls -l /dev/ttyUSB*`
4. For Windows, check Device Manager under "Ports (COM & LPT)"

Use the appropriate port in your `idf.py` commands.

## Project Structure

- `main/` - Main application code
- `components/` - Custom components (dht21, csms_v2)
- `managed_components/` - Components managed by ESP-IDF component manager
- `.devcontainer/` - Container configuration files

## Additional Resources

- [ESP-Rainmaker Documentation](https://rainmaker.espressif.com/docs/get-started.html)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html)
- [VS Code Dev Containers](https://code.visualstudio.com/docs/remote/containers) 