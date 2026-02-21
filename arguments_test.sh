#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}Executing Spider Argument Parsing Tests...${NC}\n"

PASS_COUNT=0
FAIL_COUNT=0

# Function to run a test
# Usage: run_test "description" "command" "expected_exit_code" "expected_string_in_output"
run_test() {
    local desc=$1
    local cmd=$2
    local expected_code=$3
    local expected_str=$4

    echo -n "Test: $desc... "
    
    # Run command and capture output
    output=$(eval "$cmd" 2>&1)
    exit_code=$?

    if [ $exit_code -eq $expected_code ]; then
        if [[ -z "$expected_str" ]] || [[ "$output" == *"$expected_str"* ]]; then
            echo -e "${GREEN}PASS${NC}"
            PASS_COUNT=$((PASS_COUNT + 1))
        else
            echo -e "${RED}FAIL (String not found)${NC}"
            echo -e "  Expected string: '$expected_str'"
            echo -e "  Actual output:   '$output'"
            FAIL_COUNT=$((FAIL_COUNT + 1))
        fi
    else
        echo -e "${RED}FAIL (Exit code error)${NC}"
        echo -e "  Expected code: $expected_code"
        echo -e "  Actual code:   $exit_code"
        echo -e "  Output: $output"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

# ----------------- TEST CASES -----------------

# 1. Simple Valid URL
run_test "Single URL" "./spider https://example.com" 0 "URL : https://example.com"

# 2. Recursive Flag
run_test "Recursive Flag (-r)" "./spider -r https://example.com" 0 "Recursive : Oui"

# 3. Depth Flag
run_test "Depth Flag (-l 10)" "./spider -l 10 https://example.com" 0 "Depth : 10"

# 4. Path Flag
run_test "Path Flag (-p /tmp/)" "./spider -p /tmp/ https://example.com" 0 "Path : /tmp/"

# 5. Combined Flags (separated)
run_test "All flags separated" "./spider -r -l 3 -p ./data/ https://example.com" 0 "Recursive : Oui"

# 6. Combined Flags (stuck)
run_test "Flags stuck (-rl5)" "./spider -rl5 https://example.com" 0 "Depth : 5"

# 7. Flags stuck with value
run_test "Flags stuck with separate value" "./spider -rl 8 https://example.com" 0 "Depth : 8"

# 8. URL in middle (Should still work with getopt)
run_test "URL between flags" "./spider -r https://example.com -l 2" 0 "URL : https://example.com"

# 9. Error: Missing URL
run_test "Missing URL" "./spider -r -l 5" 1 "Error, Missing URL."

# 10. Error: Missing Argument for -l (if it's the last arg)
# Note: getopt usually complains "option requires an argument"
run_test "Missing -l value" "./spider -r -l" 1 "option requires an argument"

# 11. Error: Invalid depth (String)
run_test "Invalid depth (abc)" "./spider -l abc https://example.com" 1 "valid number"

# 12. Error: Depth too large
run_test "Depth out of range" "./spider -l 9999999999999999999 https://example.com" 1 "too large"

# 13. Error: Invalid option
run_test "Invalid option (-z)" "./spider -z https://example.com" 1 "invalid option"

# 14. Error: No arguments at all
run_test "No arguments" "./spider" 1 "Usage"

# 15. Complex path with spaces (if quotes used)
run_test "Path with spaces" "./spider -p './my folder/' https://example.com" 0 "Path : ./my folder/"

# ----------------- SUMMARY -----------------

echo -e "\n------------------------------------"
echo -e "Tests passed: ${GREEN}$PASS_COUNT${NC}"
echo -e "Tests failed: ${RED}$FAIL_COUNT${NC}"
echo -e "------------------------------------"

if [ $FAIL_COUNT -eq 0 ]; then
    echo -e "${GREEN}Everything looks perfect! Spider is ready. 🕷️✨${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed. Check your logic! 🕷️💀${NC}"
    exit 1
fi
