#!/bin/bash

# ==========================================
# SCORPION TEST SCRIPT (BASH VERSION)
# ==========================================

TEST_DIR="/tmp/scorpion_sh_test"
SCORPION="./scorpion"
GREEN="\033[92m"
RED="\033[91m"
RESET="\033[0m"

# Ensure scorpion is compiled
if [ ! -f "$SCORPION" ]; then
    echo -e "${RED}Error: $SCORPION not found. Please run 'make' first.${RESET}"
    exit 1
fi

# Clean & Create test dir
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

echo "Generating test images in $TEST_DIR..."

# ------------------------------------------
# 1. JPEG
# SOI (FF D8) + APP1 (FF E1) + Length (00 10) + "Exif\0\0" + TIFF Header
# ------------------------------------------
printf "\xFF\xD8\xFF\xE1\x00\x10Exif\0\0II\x2A\x00\x08\x00\x00\x00\x00\x00\xFF\xD9" > "$TEST_DIR/test.jpg"

# ------------------------------------------
# 2. PNG
# Signature + IHDR (200x100) + eXIf chunk + IEND
# ------------------------------------------
# Warning: PNG chunks need valid CRCs or some parsers fail. 
# Scorpion parser is simple and might skip CRC check or just jump over bytes.
# We will construct a simpler stream that mimics the structure Scorpion expects.
# Header
printf "\x89PNG\r\n\x1a\n" > "$TEST_DIR/test.png"
# IHDR: Len=13 (00 00 00 0D), Type=IHDR
printf "\x00\x00\x00\x0DIHDR" >> "$TEST_DIR/test.png"
# Width=200 (00 00 00 C8), Height=100 (00 00 00 64), 8, 2, 0, 0, 0
printf "\x00\x00\x00\xC8\x00\x00\x00\x64\x08\x02\x00\x00\x00" >> "$TEST_DIR/test.png"
# CRC (Fake)
printf "\xDE\xAD\xBE\xEF" >> "$TEST_DIR/test.png"
# eXIf Chunk: Len=14 (00 00 00 0E), Type=eXIf
printf "\x00\x00\x00\x0EeXIf" >> "$TEST_DIR/test.png"
# Payload: TIFF Header (II 2A 00 08 00 00 00 00 00) + padding if needed?
# My parser reads TIFF right away.
printf "II\x2A\x00\x08\x00\x00\x00\x00\x00" >> "$TEST_DIR/test.png"
# Padding to match length 14? We wrote 10 bytes (II..). Need 4 more.
printf "\x00\x00\x00\x00" >> "$TEST_DIR/test.png"
# CRC (Fake)
printf "\xDE\xAD\xBE\xEF" >> "$TEST_DIR/test.png"
# IEND
printf "\x00\x00\x00\x00IEND\xAE\x42\x60\x82" >> "$TEST_DIR/test.png"

# ------------------------------------------
# 3. GIF
# GIF89a + Width (320=40 01) + Height (240=F0 00)
# ------------------------------------------
printf "GIF89a\x40\x01\xF0\x00\xF7\x00\x00\x3B" > "$TEST_DIR/test.gif"

# ------------------------------------------
# 4. BMP
# BM + Size + Reserved + Offset(54) + DIB Header(40) + Width(800) + Height(600)
# ------------------------------------------
printf "BM" > "$TEST_DIR/test.bmp"
# File Size (54 + 0 = 54) -> 36 00 00 00
printf "\x36\x00\x00\x00" >> "$TEST_DIR/test.bmp"
# Reserved
printf "\x00\x00\x00\x00" >> "$TEST_DIR/test.bmp"
# Offset (54) -> 36 00 00 00
printf "\x36\x00\x00\x00" >> "$TEST_DIR/test.bmp"
# DIB Header Size (40) -> 28 00 00 00
printf "\x28\x00\x00\x00" >> "$TEST_DIR/test.bmp"
# Width (800) -> 20 03 00 00
printf "\x20\x03\x00\x00" >> "$TEST_DIR/test.bmp"
# Height (600) -> 58 02 00 00
printf "\x58\x02\x00\x00" >> "$TEST_DIR/test.bmp"
# Planes(1), BPP(24) -> 01 00 18 00
printf "\x01\x00\x18\x00" >> "$TEST_DIR/test.bmp"
# Fill rest of header with 0
dd if=/dev/zero bs=1 count=24 2>/dev/null >> "$TEST_DIR/test.bmp"

# ------------------------------------------
# 5. WEBP
# RIFF + Size + WEBP + VP8X + Size + Flags/Dims + EXIF
# ------------------------------------------
# We build parts then assemble
# VP8X:
# Flags (00 00 00 00) - bit 3 for EXIF is 0x08. So 08 00 00 00
# Width (500-1 = 499 = 1F3) -> F3 01 00
# Height (500-1 = 499 = 1F3) -> F3 01 00
VP8X_DATA="\x08\x00\x00\x00\xF3\x01\x00\xF3\x01\x00"
# Size of VP8X data = 10 bytes -> 0A 00 00 00

# EXIF: "Exif\0\0" + TIFF Header...
EXIF_DATA="Exif\0\0II\x2A\x00\x08\x00\x00\x00\x00\x00"
# Size = 16 bytes -> 10 00 00 00

# File construction:
printf "RIFF" > "$TEST_DIR/test.webp"
# Total Size (Dummy for now, 4 bytes)
printf "\xFF\x00\x00\x00" >> "$TEST_DIR/test.webp"
printf "WEBP" >> "$TEST_DIR/test.webp"
# VP8X Chunk
printf "VP8X\x0A\x00\x00\x00$VP8X_DATA" >> "$TEST_DIR/test.webp"
# EXIF Chunk
printf "EXIF\x10\x00\x00\x00$EXIF_DATA" >> "$TEST_DIR/test.webp"

# ==========================================
# RUNNING TESTS
# ==========================================

echo -e "\nRunning Scorpion...\n"
OUTPUT=$($SCORPION "$TEST_DIR/test.jpg" "$TEST_DIR/test.png" "$TEST_DIR/test.gif" "$TEST_DIR/test.bmp" "$TEST_DIR/test.webp" 2>&1)

# Helper function
check_output() {
    local TEST_NAME="$1"
    local NEEDLE="$2"
    if echo "$OUTPUT" | grep -q "$NEEDLE"; then
        echo -e "${GREEN}[PASS] ${TEST_NAME}${RESET}"
    else
        echo -e "${RED}[FAIL] ${TEST_NAME} - Expected '$NEEDLE'${RESET}"
    fi
}

echo "------------------------------"
echo "       TEST RESULTS       "
echo "------------------------------"

check_output "JPEG Detection" "Format de l'image : JPEG"
check_output "JPEG EXIF Scan" "Found APP1 segment"

check_output "PNG Format" "Format de l'image : PNG"
check_output "PNG Width 200" "Dimensions : 200"
check_output "PNG EXIF Chunk" "Found eXIf chunk"

check_output "GIF Format" "Format de l'image : GIF"
check_output "GIF Dimensions 320x240" "Dimensions : 320 x 240"

check_output "BMP Format" "Format de l'image : BMP"
check_output "BMP Dimensions 800x600" "Dimensions : 800 x 600"

check_output "WEBP Format" "Format de l'image : WEBP"
check_output "WEBP Dimensions 500x500" "Dimensions : 500 x 500"
check_output "WEBP EXIF Chunk" "Found EXIF chunk"

echo "------------------------------"
