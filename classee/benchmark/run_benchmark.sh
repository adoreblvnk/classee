#!/bin/bash

# --- Automated Benchmark Script for INF1002 CMS Project ---
# This script compares the performance of the CMS with and without the
# Fisher-Yates shuffle optimization for BST insertion.
#
# USAGE:
# 1. Make sure you are in the project's root directory.
# 2. Run `chmod +x benchmark/run_benchmark.sh` to make it executable.
# 3. Run `./benchmark/run_benchmark.sh`

# Exit immediately if a command exits with a non-zero status.
set -e

# --- Configuration ---
# CORRECTED: Paths are now relative to the project root, where the script is executed from.
CONFIG_FILE="include/config.h"
BUILD_DIR="build"
EXECUTABLE="${BUILD_DIR}/bin/classee"
INPUT_CMDS="benchmark/cmds.txt"

# --- Colors for better output ---
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# --- Cleanup Function ---
# This function is called automatically when the script exits,
# ensuring the config file is always restored.
cleanup() {
  echo -e "\n${YELLOW}Restoring config.h to enable shuffle (SHUFFLE=1)...${NC}"
  # Check if the config file exists before trying to modify it
  if [ -f "$CONFIG_FILE" ]; then
    sed -i.bak 's/#define SHUFFLE 0/#define SHUFFLE 1/' "$CONFIG_FILE"
    # remove the backup file created by sed
    rm -f "${CONFIG_FILE}.bak"
    echo -e "${GREEN}Cleanup complete.${NC}"
  fi
}

# Trap the EXIT signal to run the cleanup function automatically.
trap cleanup EXIT

# --- Step 1: Benchmark the Optimized Version (SHUFFLE = 1) ---

echo -e "${BLUE}--- Benchmarking OPTIMIZED Version (BST with Shuffle) ---${NC}"
echo "Ensuring SHUFFLE is set to 1 in config.h..."
# This command ensures we are in the correct state, even if run multiple times
sed -i.bak 's/#define SHUFFLE 0/#define SHUFFLE 1/' "$CONFIG_FILE"

echo "Building the OPTIMIZED version..."
cmake --build ${BUILD_DIR} > /dev/null # Hide build output for cleaner demo

echo -e "\n${GREEN}Running benchmark on the OPTIMIZED executable...${NC}"
hyperfine --warmup 3 --command-name "Optimized (BST with Shuffle)" "${EXECUTABLE} < ${INPUT_CMDS}"

# --- Step 2: Benchmark the Unoptimized Version (SHUFFLE = 0) ---

echo -e "\n${BLUE}--- Benchmarking UNOPTIMIZED Version (Simulated Linked List) ---${NC}"
echo "Modifying config.h to set SHUFFLE to 0..."
sed -i.bak 's/#define SHUFFLE 1/#define SHUFFLE 0/' "$CONFIG_FILE"

echo "Rebuilding the UNOPTIMIZED version..."
cmake --build ${BUILD_DIR} > /dev/null # Hide build output

echo -e "\n${GREEN}Running benchmark on the UNOPTIMIZED executable...${NC}"
hyperfine --warmup 3 --command-name "Unoptimized (Simulated Linked List)" "${EXECUTABLE} < ${INPUT_CMDS}"

echo -e "\n${GREEN}--- Benchmark Complete ---${NC}"
# The cleanup trap will automatically run now
