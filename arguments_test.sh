#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}============================================${NC}"
echo -e "${CYAN}   🕷️  Spider - Suite de Tests Complète   ${NC}"
echo -e "${CYAN}============================================${NC}\n"

PASS_COUNT=0
FAIL_COUNT=0

# ==========================================
# FONCTION : Test standard (exit code + string)
# ==========================================
run_test() {
    local desc=$1
    local cmd=$2
    local expected_code=$3
    local expected_str=$4

    echo -n "  Test: $desc... "
    
    output=$(eval "$cmd" 2>&1)
    exit_code=$?

    if [ $exit_code -eq $expected_code ]; then
        if [[ -z "$expected_str" ]] || [[ "$output" == *"$expected_str"* ]]; then
            echo -e "${GREEN}PASS${NC}"
            PASS_COUNT=$((PASS_COUNT + 1))
        else
            echo -e "${RED}FAIL (String not found)${NC}"
            echo -e "    Expected: '$expected_str'"
            echo -e "    Got:      '$output'"
            FAIL_COUNT=$((FAIL_COUNT + 1))
        fi
    else
        echo -e "${RED}FAIL (Exit code: $exit_code, expected: $expected_code)${NC}"
        echo -e "    Output: $output"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

# ==========================================
# FONCTION : Test Valgrind (memory leaks)
# ==========================================
run_valgrind_test() {
    local desc=$1
    local cmd=$2

    echo -n "  Valgrind: $desc... "
    
    valgrind_output=$(valgrind --leak-check=full --error-exitcode=42 --log-fd=3 $cmd 3>&1 1>/dev/null 2>/dev/null)
    exit_code=$?

    # Chercher "no leaks are possible" ou "All heap blocks were freed"
    if echo "$valgrind_output" | grep -q "no leaks are possible\|All heap blocks were freed"; then
        echo -e "${GREEN}PASS (No leaks)${NC}"
        PASS_COUNT=$((PASS_COUNT + 1))
    elif echo "$valgrind_output" | grep -q "definitely lost: 0 bytes"; then
        echo -e "${GREEN}PASS (0 bytes lost)${NC}"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo -e "${RED}FAIL (Possible memory leak)${NC}"
        # Afficher les lignes pertinentes
        echo "$valgrind_output" | grep -E "definitely lost|indirectly lost|possibly lost|still reachable" | while read line; do
            echo -e "    ${RED}$line${NC}"
        done
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
}

# ==========================================
# SECTION 1 : Arguments - Cas de SUCCES
# ==========================================
echo -e "${YELLOW}📋 SECTION 1 : Arguments - Cas de succès${NC}"

run_test "Single URL" "./spider https://example.com" 0 "reçus"
run_test "Recursive Flag (-r)" "./spider -r https://example.com" 0 "reçus"
run_test "Depth Flag (-l 10)" "./spider -l 10 https://example.com" 0 "reçus"
run_test "Path Flag (-p /tmp/)" "./spider -p /tmp/ https://example.com" 0 "reçus"
run_test "All flags combined" "./spider -r -l 3 -p ./data/ https://example.com" 0 "reçus"
run_test "Flags stuck (-rl5)" "./spider -rl5 https://example.com" 0 "reçus"
run_test "Flags stuck separate val" "./spider -rl 8 https://example.com" 0 "reçus"
run_test "URL between flags" "./spider -r https://example.com -l 2" 0 "reçus"
run_test "Path with spaces" "./spider -p './my folder/' https://example.com" 0 "reçus"

# ==========================================
# SECTION 2 : Arguments - Cas d'ERREUR
# ==========================================
echo -e "\n${YELLOW}📋 SECTION 2 : Arguments - Cas d'erreur${NC}"

run_test "Missing URL" "./spider -r -l 5" 1 "Error, Missing URL."
run_test "Missing -l value" "./spider -r -l" 1 "option requires an argument"
run_test "Invalid depth (abc)" "./spider -l abc https://example.com" 1 "valid number"
run_test "Depth out of range" "./spider -l 9999999999999999999 https://example.com" 1 "too large"
run_test "Invalid option (-z)" "./spider -z https://example.com" 1 "invalid option"
run_test "No arguments" "./spider" 1 "Usage"

# ==========================================
# SECTION 3 : Robustesse réseau
# ==========================================
echo -e "\n${YELLOW}📋 SECTION 3 : Robustesse réseau${NC}"

run_test "Invalid URL (no crash)" "./spider https://this-url-does-not-exist-xyz123.com" 0 "Error:"
run_test "Negative depth" "./spider -l -1 https://example.com" 0 ""
run_test "HTTP redirect (google)" "./spider https://www.google.com" 0 "reçus"
run_test "HTTPS site" "./spider https://example.com" 0 "reçus"

# ==========================================
# SECTION 4 : Valgrind - Fuites mémoire
# ==========================================
echo -e "\n${YELLOW}🔬 SECTION 4 : Valgrind - Fuites mémoire${NC}"

# Test 1 : Cas de succès (téléchargement normal)
run_valgrind_test "Normal download" "./spider https://example.com"

# Test 2 : Cas d'erreur d'arguments (pas d'URL)
run_valgrind_test "Missing URL error" "./spider -r -l 5"

# Test 3 : Invalid depth
run_valgrind_test "Invalid depth error" "./spider -l abc https://example.com"

# Test 4 : URL invalide (erreur réseau)
run_valgrind_test "Invalid URL error" "./spider https://this-url-does-not-exist-xyz123.com"

# Test 5 : No arguments
run_valgrind_test "No arguments error" "./spider"

# Test 6 : All flags
run_valgrind_test "All flags combined" "./spider -r -l 3 -p /tmp/ https://example.com"

# ==========================================
# RÉSUMÉ
# ==========================================
TOTAL=$((PASS_COUNT + FAIL_COUNT))

echo -e "\n${CYAN}============================================${NC}"
echo -e "  Tests réussis : ${GREEN}$PASS_COUNT${NC} / $TOTAL"
echo -e "  Tests échoués : ${RED}$FAIL_COUNT${NC} / $TOTAL"
echo -e "${CYAN}============================================${NC}"

if [ $FAIL_COUNT -eq 0 ]; then
    echo -e "${GREEN}Tout est parfait ! Spider est prêt pour la correction. 🕷️✨${NC}"
    exit 0
else
    echo -e "${RED}Certains tests ont échoué. Vérifie ton code ! 🕷️💀${NC}"
    exit 1
fi
