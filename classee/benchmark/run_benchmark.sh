#!/bin/bash

# --- auto benchmark script for inf1002 cms proj ---
# compares perf of cms w/ & w/o fisher-yates shuffle opt for bst insert
#
# usage:
# 1. ensure in proj root dir
# 2. run `./benchmark/run_benchmark.sh`

# exit immediately if cmd exits w/ non-zero status
set -e

# --- config ---
CONFIG_FILE="include/config.h"
BUILD_DIR="build"
EXECUTABLE="${BUILD_DIR}/bin/classee"
INPUT_CMDS="benchmark/cmds.txt"

# --- colors for better output ---
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # no color

# --- cleanup func ---
# func called auto when script exits, ensure config file always restored
cleanup() {
  echo -e "\n${YELLOW}Restoring config.h to enable shuffle (SHUFFLE=1)...${NC}"
  # check if config file exists before mod
  if [ -f "$CONFIG_FILE" ]; then
    sed -i.bak 's/#define SHUFFLE 0/#define SHUFFLE 1/' "$CONFIG_FILE"
    # rm backup file created by sed
    rm -f "${CONFIG_FILE}.bak"
    echo -e "${GREEN}Cleanup complete.${NC}"
  fi
}

# trap exit sig to run cleanup func auto
trap cleanup EXIT

# --- benchmark optimized (shuffle = 1) ---

echo -e "${BLUE}--- Benchmarking OPTIMIZED Version (BST with Shuffle) ---${NC}"
echo "Ensuring SHUFFLE is set to 1 in config.h..."
# cmd ensures correct state even if run mult times
sed -i.bak 's/#define SHUFFLE 0/#define SHUFFLE 1/' "$CONFIG_FILE"

echo "Building the OPTIMIZED version..."
cmake --build ${BUILD_DIR} > /dev/null # hide build output

echo -e "\n${GREEN}Running benchmark on the OPTIMIZED executable...${NC}"
hyperfine --warmup 3 --command-name "Optimized (BST with Shuffle)" "${EXECUTABLE} < ${INPUT_CMDS}"

# --- benchmark unoptimized (shuffle = 0) ---

echo -e "\n${BLUE}--- Benchmarking UNOPTIMIZED Version (Simulated Linked List) ---${NC}"
echo "Modifying config.h to set SHUFFLE to 0..."
sed -i.bak 's/#define SHUFFLE 1/#define SHUFFLE 0/' "$CONFIG_FILE"

echo "Rebuilding the UNOPTIMIZED version..."
cmake --build ${BUILD_DIR} > /dev/null # hide build output

echo -e "\n${GREEN}Running benchmark on the UNOPTIMIZED executable...${NC}"
hyperfine --warmup 3 --command-name "Unoptimized (Simulated Linked List)" "${EXECUTABLE} < ${INPUT_CMDS}"

echo -e "\n${GREEN}--- Benchmark Complete ---${NC}"
# cleanup trap auto runs now
