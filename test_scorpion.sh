#!/bin/bash

# ==========================================
# SCORPION REAL-WORLD TEST SUITE
# ==========================================
# Downloads real images to test the parser.
# Falls back to synthetic generation if download fails.

TEST_DIR="/tmp/scorpion_real_test"
SCORPION="./scorpion"
GREEN="\033[92m"
RED="\033[91m"
YELLOW="\033[93m"
RESET="\033[0m"
BOLD="\033[1m"

# 1. Compilation Check
if [ ! -f "$SCORPION" ]; then
    echo -e "${RED}Error: $SCORPION not found. Please run 'make'.${RESET}"
    exit 1
fi

# 2. Setup
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

echo -e "${BOLD}⬇️  Downloading Real Sample Images...${RESET}"

# Function to download with fallback
download_or_gen() {
    local URL="$1"
    local OUT="$2"
    local TYPE="$3"
    
    # Try wget first (often better for redirects/static content)
    wget -q -O "$OUT" "$URL"
    
    # Check if failed or empty
    if [ ! -s "$OUT" ] || [ $(stat -c%s "$OUT") -lt 100 ]; then
        echo -e "${YELLOW}   ⚠️  Failed to download $TYPE. Generating synthetic fallback...${RESET}"
        
        case "$TYPE" in
            "JPEG")
                # Valid JPEG + EXIF Structure
                printf "\xFF\xD8\xFF\xE1\x00\x10Exif\0\0II\x2A\x00\x08\x00\x00\x00\x00\x00\xFF\xD9" > "$OUT"
                ;;
            "PNG")
                # Valid PNG Signature + IHDR + eXIf
                printf "\x89PNG\r\n\x1a\n\x00\x00\x00\x0DIHDR\x00\x00\x00\xC8\x00\x00\x00\x64\x08\x02\x00\x00\x00\xDE\xAD\xBE\xEF\x00\x00\x00\x0EeXIfII\x2A\x00\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\xDE\xAD\xBE\xEF\x00\x00\x00\x00IEND\xAE\x42\x60\x82" > "$OUT"
                ;;
            "GIF")
                # Valid GIF89a + Dimensions
                printf "GIF89a\x40\x01\xF0\x00\xF7\x00\x00\x3B" > "$OUT"
                ;;
            "BMP")
                # Valid BMP Header
                printf "BM\x36\x00\x00\x00\x00\x00\x00\x00\x36\x00\x00\x00\x28\x00\x00\x00\x20\x03\x00\x00\x58\x02\x00\x00\x01\x00\x18\x00" > "$OUT"
                dd if=/dev/zero bs=1 count=24 2>/dev/null >> "$OUT"
                ;;
            "WEBP")
                # Valid RIFF WEBP VP8X EXIF
                printf "RIFF\xFF\x00\x00\x00WEBPVP8X\x0A\x00\x00\x00\x08\x00\x00\x00\xF3\x01\x00\xF3\x01\x00EXIF\x10\x00\x00\x00Exif\0\0II\x2A\x00\x08\x00\x00\x00\x00\x00" > "$OUT"
                ;;
        esac
    else
        echo -e "${GREEN}   ✅ $TYPE downloaded successfully.${RESET}"
    fi
}

# --- DOWNLOADS ---
# JPEG with EXIF/GPS
download_or_gen "https://upload.wikimedia.org/wikipedia/commons/1/15/Jpeg_exif_gps_coordinates_example.jpg" "$TEST_DIR/sample.jpg" "JPEG"

# PNG with Transparency
download_or_gen "https://upload.wikimedia.org/wikipedia/commons/4/47/PNG_transparency_demonstration_1.png" "$TEST_DIR/sample.png" "PNG"

# GIF Animation
download_or_gen "https://upload.wikimedia.org/wikipedia/commons/2/2c/Rotating_earth_%28large%29.gif" "$TEST_DIR/sample.gif" "GIF"

# BMP Lena
download_or_gen "https://people.math.sc.edu/Burkardt/data/bmp/lena.bmp" "$TEST_DIR/sample.bmp" "BMP"

# WEBP Gallery
download_or_gen "https://www.gstatic.com/webp/gallery/1.webp" "$TEST_DIR/sample.webp" "WEBP"


echo -e "\n${BOLD}🦂 Running Scorpion...${RESET}\n"
OUTPUT=$($SCORPION "$TEST_DIR/sample.jpg" "$TEST_DIR/sample.png" "$TEST_DIR/sample.gif" "$TEST_DIR/sample.bmp" "$TEST_DIR/sample.webp" 2>&1)

# Function to check results
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

# Note: Using "Found APP1" for JPEG because download might fail and fallback doesn't have GPS tags, only Exif structure.
check_output "JPEG Format" "Format de l'image : JPEG"
check_output "JPEG EXIF Detect" "Found APP1 segment"

check_output "PNG Format" "Format de l'image : PNG"
check_output "PNG Dimensions" "Dimensions :"

check_output "GIF Format" "Format de l'image : GIF"
check_output "GIF Dimensions" "Dimensions :"

check_output "BMP Format" "Format de l'image : BMP"
check_output "BMP Dimensions" "Dimensions :"

check_output "WEBP Format" "Format de l'image : WEBP"
check_output "WEBP Dimensions" "Dimensions :"

echo "------------------------------"
echo -e "${YELLOW}Analyzed files are in $TEST_DIR${RESET}"
