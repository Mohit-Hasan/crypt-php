#!/bin/bash

# Setup script for C++ PHP Encoder
# This script downloads YAK Pro and PHP-Parser 4.x to be embedded in the C++ binary

set -e

PROJECT_ROOT=$(pwd)
TOOL_DIR="$PROJECT_ROOT/yakpro-po"

echo "Checking for existing tools..."
if [ -d "$TOOL_DIR" ]; then
    echo "YAK Pro already exists. Skipping download."
else
    echo "Cloning YAK Pro..."
    git clone --depth 1 https://github.com/pk-fr/yakpro-po.git "$TOOL_DIR"
fi

cd "$TOOL_DIR"

if [ -d "PHP-Parser" ]; then
    echo "PHP-Parser already exists. Skipping download."
else
    echo "Cloning PHP-Parser (branch 4.x)..."
    git clone --depth 1 --branch 4.x https://github.com/nikic/PHP-Parser.git
fi

# Cleanup git folders to minimize resource size
find . -name ".git" -type d -exec rm -rf {} + || true

cd "$PROJECT_ROOT"

echo "Ensuring src directory exists..."
mkdir -p src

echo "Packaging PHP resources into src/EmbeddedResources.h..."
python3 scripts/package_resources.py yakpro-po src/EmbeddedResources.h

echo "Done! You can now compile the C++ application."
