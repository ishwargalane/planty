#!/bin/bash

# Build script for ESP-Rainmaker project using dev container
# Edit in Cursor, build with container for portability

set -e

echo "🔧 Building ESP-Rainmaker project using dev container..."

# Build the dev container image
echo "📦 Building dev container..."
docker build -t planty-dev .devcontainer/

# Create a simple build function
build_project() {
    docker run --rm -it \
        -v "$(pwd):/workspaces/planty" \
        -w /workspaces/planty \
        --privileged \
        planty-dev \
        bash -c "
            # Source ESP-IDF environment
            source /opt/esp/idf/export.sh
            
            # Set ESP-Rainmaker path
            export RMAKER_PATH=/opt/esp/rainmaker
            
            # Set target if needed
            idf.py set-target esp32
            
            # Build the project
            idf.py build
        "
}

# Run the build
echo "🚀 Starting build process..."
build_project

echo "✅ Build complete!"
echo "📋 Next steps:"
echo "   - To flash: ./flash.sh"
echo "   - To clean: ./clean.sh"
echo "   - To monitor: ./monitor.sh" 