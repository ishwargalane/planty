#!/bin/bash

# Clean script for ESP-Rainmaker project using dev container

set -e

echo "🧹 Cleaning ESP-Rainmaker project using dev container..."

# Clean the project
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
        
        # Clean the project
        idf.py fullclean
    "

echo "✅ Clean complete!"
echo "📋 Next step: ./build.sh" 