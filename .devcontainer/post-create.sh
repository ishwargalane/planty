#!/bin/bash

# Source IDF environment
source /opt/esp/idf/export.sh

# Print versions of tools
echo "ESP-IDF version:"
cd $IDF_PATH && git describe --tags

echo "ESP-Rainmaker version:"
cd $RMAKER_PATH && git describe --tags || echo "No tag found"

echo "Python version:"
python --version

# Install project-specific dependencies
cd /workspaces/planty

# Verify the configuration
echo "Verifying ESP-IDF configuration..."
idf.py --version

echo "Verifying port..."
ls -l /dev/tty*

echo "Setup complete. You can now build the project with 'idf.py build'" 