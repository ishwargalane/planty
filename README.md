# Planty - ESP-Rainmaker Project

A portable ESP-Rainmaker project that uses Cursor for editing and Docker containers for building.

## Features

- **🚀 Portable**: Build environment runs in containers
- **✏️ Cursor-friendly**: Edit code in Cursor with full IDE features
- **🔧 Container-based builds**: Uses ESP-IDF v5.4 with ESP-Rainmaker
- **📱 ESP32 support**: Configured for ESP32 development
- **🛠️ Simple workflow**: Easy-to-use scripts for common tasks
- **🖥️ Cross-platform**: Works on macOS, Linux, and Windows

## Requirements

### Essential
- **Docker Desktop** (running)
- **Cursor** (for editing)
- **ESP32 device** (for flashing and testing)

### Platform-specific
- **macOS**: `esptool` (install via `pip3 install esptool`)
- **Linux**: USB device permissions for Docker passthrough
- **Windows**: WSL2 with USB passthrough configuration

## Architecture

**Hybrid Approach for Optimal Performance:**
- **Building**: Uses Docker containers (consistent across all platforms)
- **Flashing**: Uses host system tools (better USB device access)
- **Monitoring**: Uses host system tools (real-time serial communication)

## Quick Start

### 1. Edit in Cursor
```bash
# Open the project in Cursor
cursor .
```

### 2. Build the project
```bash
# Build using the dev container (works on all platforms)
./build.sh
```

### 3. Flash to ESP32

**macOS:**
```bash
# Auto-detects ESP32 port and flashes
./flash-macos.sh

# Or specify a specific port
./flash-macos.sh /dev/tty.usbserial-XXXXXX
```

**Linux:**
```bash
# Uses containerized flashing
./flash.sh

# Or specify a specific port
./flash.sh /dev/ttyUSB0
```

### 4. Monitor output

**macOS:**
```bash
# Monitor ESP32 output
./monitor-macos.sh

# Or specify a specific port
./monitor-macos.sh /dev/tty.usbserial-XXXXXX
```

**Linux:**
```bash
# Uses containerized monitoring
./monitor.sh

# Or specify a specific port
./monitor.sh /dev/ttyUSB0
```

## Available Scripts

| Script | Platform | Description |
|--------|----------|-------------|
| `./build.sh` | All | Build the project using dev container |
| `./clean.sh` | All | Clean build artifacts using dev container |
| `./flash-macos.sh [port]` | macOS | Flash firmware using host esptool |
| `./monitor-macos.sh [port]` | macOS | Monitor ESP32 output using host Python |
| `./flash.sh [port]` | Linux | Flash firmware using containerized esptool |
| `./monitor.sh [port]` | Linux | Monitor ESP32 output using containerized tools |

## Project Structure

```
planty/
├── main/                    # Main application code
│   ├── app_csms_v2.c       # CSMS v2 implementation
│   ├── app_dht21.c         # DHT21 sensor code
│   └── app_driver.c        # Driver implementation
├── components/             # Custom components
│   ├── csms_v2/           # CSMS v2 component
│   └── dht21/             # DHT21 component
├── .devcontainer/         # Dev container configuration
│   ├── Dockerfile         # ESP-IDF + ESP-Rainmaker environment
│   ├── devcontainer.json  # VS Code dev container settings
│   └── post-create.sh     # Container initialization script
├── managed_components/    # ESP-IDF managed components (auto-downloaded)
├── build/                 # Build output directory (created during build)
├── build.sh              # Cross-platform build script
├── clean.sh              # Cross-platform clean script
├── flash-macos.sh         # macOS-specific flash script
├── monitor-macos.sh       # macOS-specific monitor script
├── flash.sh              # Linux flash script (containerized)
├── monitor.sh            # Linux monitor script (containerized)
├── CMakeLists.txt        # CMake configuration
└── README.md             # This file
```

## Development Workflow

### Complete Development Cycle
1. **Edit code** in Cursor with full IntelliSense and syntax highlighting
2. **Build** using `./build.sh` (containerized ESP-IDF build)
3. **Flash** using platform-specific script (direct USB access)
4. **Monitor** using platform-specific script (real-time serial output)
5. **Repeat** the cycle for iterative development

### Tested Workflow Example
```bash
# Complete clean build and flash cycle
./clean.sh          # Clean all artifacts
./build.sh          # Build from scratch
./flash-macos.sh    # Flash to ESP32 (macOS)
./monitor-macos.sh  # View real-time output
```

## Platform-Specific Setup

### macOS Setup
```bash
# Install esptool for flashing
pip3 install esptool

# Verify ESP32 connection
ls /dev/tty.* | grep -E "(usb|serial)"

# Use macOS-specific scripts
./flash-macos.sh
./monitor-macos.sh
```

### Linux Setup
```bash
# Add user to dialout group for USB access
sudo usermod -a -G dialout $USER

# Logout and login again for changes to take effect
# Use Linux scripts (containerized)
./flash.sh
./monitor.sh
```

### Windows Setup
```bash
# Use WSL2 with Docker Desktop
# Install USB/IP support for WSL2
# Use Linux scripts within WSL2
```

## Troubleshooting

### Docker Issues
```bash
# Rebuild container from scratch
docker system prune -f
./build.sh
```

### Build Issues
```bash
# Clean and rebuild
./clean.sh
./build.sh
```

### USB Device Issues

**macOS:**
```bash
# List available serial ports
ls /dev/tty.* | grep -E "(usb|serial)"

# Check if esptool is installed
python3 -m esptool version

# Install if missing
pip3 install esptool
```

**Linux:**
```bash
# Check USB device permissions
ls -l /dev/ttyUSB* /dev/ttyACM*

# Add user to dialout group if needed
sudo usermod -a -G dialout $USER
```

### Container Port Issues
**Why We Use Host Tools for Flashing:**
- Docker on macOS doesn't support USB device passthrough
- Host-based flashing provides better reliability and speed
- Containerized building ensures consistent compilation environment

### Component Hash Issues
```bash
# If managed components have hash conflicts
find managed_components -name ".component_hash" -delete
./clean.sh
./build.sh
```

## Docker Container Details

### Container Features
- **ESP-IDF v5.4**: Latest stable ESP-IDF framework
- **ESP-Rainmaker**: Cloud connectivity and IoT features
- **Toolchain**: Complete GCC toolchain for ESP32
- **Dependencies**: All required build tools and libraries
- **User Context**: Runs as `esp` user (non-root)

### Container Usage
```bash
# Container is built automatically by scripts
# Manual container interaction (if needed):
docker run -it --rm -v $(pwd):/workspaces/planty planty-dev bash
```

## Hardware Configuration

### Pin Allocation
- **DHT21 Sensor** → GPIO17 (TX2)
- **Relay** → GPIO21 (D21)
- **Button** → GPIO0 (D0)
- **Analog Signal 0** → GPIO36 (VP)
- **Analog Signal 1** → GPIO15 (D15)
- **Analog Signal 2** → GPIO13 (D13)
- **Analog Signal 3** → GPIO12 (D12)
- **Analog Signal 4** → GPIO14 (D14)

### Factory Reset
Press and hold the BOOT button for more than 3 seconds to reset to factory defaults.

## Features

- **Temperature & Humidity Monitoring**: DHT21 sensor support
- **CSMC v2**: Charging Station Management System
- **ESP-Rainmaker Integration**: Cloud connectivity
- **OTA Updates**: Over-the-air firmware updates
- **WiFi Provisioning**: Easy WiFi setup via mobile app
- **Local Control**: HTTP server for local device control
- **Scheduling**: Time-based automation
- **Time Sync**: SNTP synchronization

## Getting Started with ESP-Rainmaker

1. **Create RainMaker Account**: Visit [ESP RainMaker](https://rainmaker.espressif.com/)
2. **Download App**: Install ESP RainMaker app on your phone
3. **Build & Flash**: Use the provided scripts
4. **Provision Device**: Use the app to connect your device to WiFi
5. **Monitor**: View your device data in the app and console

## License

This project is open source. See individual component licenses for details.

---

**Happy Coding!** 🎉
