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

# ==========================================
# SECTION 1 : Tests de SUCCES (exit code 0)
# Le programme doit se connecter et récupérer du HTML
# ==========================================

# 1. Simple URL → doit réussir et recevoir du HTML
run_test "Single URL" "./spider https://example.com" 0 "reçus"

# 2. Flag -r → doit réussir (le flag est stocké, mais l'output ne change pas pour l'instant)
run_test "Recursive Flag (-r)" "./spider -r https://example.com" 0 "reçus"

# 3. Depth Flag → doit réussir
run_test "Depth Flag (-l 10)" "./spider -l 10 https://example.com" 0 "reçus"

# 4. Path Flag → doit réussir
run_test "Path Flag (-p /tmp/)" "./spider -p /tmp/ https://example.com" 0 "reçus"

# 5. Tous les flags combinés → doit réussir
run_test "All flags separated" "./spider -r -l 3 -p ./data/ https://example.com" 0 "reçus"

# 6. Flags collés (-rl5) → doit réussir
run_test "Flags stuck (-rl5)" "./spider -rl5 https://example.com" 0 "reçus"

# 7. Flags collés avec valeur séparée → doit réussir
run_test "Flags stuck with separate value" "./spider -rl 8 https://example.com" 0 "reçus"

# 8. URL entre les flags → doit réussir
run_test "URL between flags" "./spider -r https://example.com -l 2" 0 "reçus"

# ==========================================
# SECTION 2 : Tests d'ERREUR (exit code != 0)
# Le programme doit détecter les erreurs et quitter proprement
# ==========================================

# 9. Pas d'URL → erreur
run_test "Missing URL" "./spider -r -l 5" 1 "Error, Missing URL."

# 10. -l sans valeur → erreur getopt
run_test "Missing -l value" "./spider -r -l" 1 "option requires an argument"

# 11. Depth invalide (lettres) → erreur try/catch
run_test "Invalid depth (abc)" "./spider -l abc https://example.com" 1 "valid number"

# 12. Depth trop grand → erreur out_of_range
run_test "Depth out of range" "./spider -l 9999999999999999999 https://example.com" 1 "too large"

# 13. Option inconnue (-z) → erreur getopt
run_test "Invalid option (-z)" "./spider -z https://example.com" 1 "invalid option"

# 14. Aucun argument → erreur
run_test "No arguments" "./spider" 1 "Usage"

# 15. Chemin avec espaces → doit réussir
run_test "Path with spaces" "./spider -p './my folder/' https://example.com" 0 "reçus"

# ==========================================
# SECTION 3 : Tests de ROBUSTESSE
# ==========================================

# 16. URL invalide → curl doit échouer proprement (pas de crash)
run_test "Invalid URL (no crash)" "./spider https://this-url-does-not-exist-xyz123.com" 0 "Error:"

# 17. Depth négatif → doit accepter (getopt ne filtre pas ça)
run_test "Negative depth" "./spider -l -1 https://example.com" 0 ""

# ==========================================
# RÉSUMÉ
# ==========================================

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
