#!/bin/bash
# ============================================================================
#  Script de démo pour la correction — Arachnida (Spider & Scorpion)
# ============================================================================

GREEN="\033[1;32m"
RED="\033[1;31m"
CYAN="\033[1;36m"
YELLOW="\033[1;33m"
RESET="\033[0m"

PASS=0
FAIL=0

print_title()
{
	echo ""
	echo -e "${CYAN}══════════════════════════════════════════════════════════${RESET}"
	echo -e "${CYAN}  $1${RESET}"
	echo -e "${CYAN}══════════════════════════════════════════════════════════${RESET}"
}

print_test()
{
	echo ""
	echo -e "${YELLOW}▶ TEST : $1${RESET}"
	echo -e "  Commande : $2"
	echo ""
}

check_result()
{
	if [ $1 -eq 0 ]; then
		echo -e "${GREEN}  ✅ PASS — $2${RESET}"
		PASS=$((PASS + 1))
	else
		echo -e "${RED}  ❌ FAIL — $2${RESET}"
		FAIL=$((FAIL + 1))
	fi
}

# Nettoyage
rm -rf data/ /tmp/spider_test_demo/

# ============================================================================
#                              SPIDER
# ============================================================================
print_title "🕷️  SPIDER — Tests de démonstration"

TEST_URL="https://fr.wikipedia.org/wiki/42_(school)"

# --- Test 1 : Usage basique ---
print_test "Usage basique (./spider URL)" "./spider $TEST_URL"
./spider "$TEST_URL"
RESULT=$?
NB_FILES=$(ls data/ 2>/dev/null | wc -l)
if [ $RESULT -eq 0 ] && [ $NB_FILES -gt 0 ]; then
	check_result 0 "Téléchargé $NB_FILES image(s) dans ./data/"
else
	check_result 1 "Aucune image téléchargée"
fi
echo "  Fichiers téléchargés :"
ls -lh data/ 2>/dev/null | head -10
rm -rf data/

# --- Test 2 : Option -r -l ---
print_test "Récursion avec profondeur (-r -l 1)" "./spider -r -l 1 $TEST_URL"
./spider -r -l 1 "$TEST_URL"
RESULT=$?
NB_FILES=$(ls data/ 2>/dev/null | wc -l)
if [ $RESULT -eq 0 ] && [ $NB_FILES -gt 0 ]; then
	check_result 0 "Récursion OK — $NB_FILES image(s) téléchargée(s)"
else
	check_result 1 "Récursion échouée"
fi
rm -rf data/

# --- Test 3 : Option -p ---
print_test "Chemin personnalisé (-p /tmp/spider_test_demo)" "./spider -p /tmp/spider_test_demo $TEST_URL"
./spider -p /tmp/spider_test_demo "$TEST_URL"
RESULT=$?
NB_FILES=$(ls /tmp/spider_test_demo/ 2>/dev/null | wc -l)
if [ $RESULT -eq 0 ] && [ $NB_FILES -gt 0 ]; then
	check_result 0 "Images sauvées dans /tmp/spider_test_demo/ ($NB_FILES fichier(s))"
else
	check_result 1 "Aucune image dans /tmp/spider_test_demo/"
fi
rm -rf /tmp/spider_test_demo/

# --- Test 4 : Toutes les options combinées ---
print_test "Toutes les options (-r -l 1 -p /tmp/spider_test_demo)" "./spider -r -l 1 -p /tmp/spider_test_demo $TEST_URL"
./spider -r -l 1 -p /tmp/spider_test_demo "$TEST_URL"
RESULT=$?
NB_FILES=$(ls /tmp/spider_test_demo/ 2>/dev/null | wc -l)
if [ $RESULT -eq 0 ] && [ $NB_FILES -gt 0 ]; then
	check_result 0 "Combinaison OK — $NB_FILES image(s)"
else
	check_result 1 "Combinaison échouée"
fi

# ============================================================================
#                              SCORPION
# ============================================================================
print_title "🦂  SCORPION — Tests de démonstration"

# On utilise les images téléchargées par le dernier test Spider
SCORPION_DIR="/tmp/spider_test_demo"
if [ ! -d "$SCORPION_DIR" ] || [ $(ls "$SCORPION_DIR" 2>/dev/null | wc -l) -eq 0 ]; then
	echo -e "${RED}  Aucune image disponible pour tester Scorpion. Spider a échoué.${RESET}"
	FAIL=$((FAIL + 1))
else
	# --- Test 1 : Scorpion sur les images ---
	FIRST_IMG=$(ls "$SCORPION_DIR"/* 2>/dev/null | head -1)
	print_test "Affichage des métadonnées d'une image" "./scorpion $FIRST_IMG"
	./scorpion "$FIRST_IMG"
	RESULT=$?
	check_result $RESULT "Scorpion a lu le fichier sans crasher"

	# --- Test 2 : Scorpion sur TOUTES les images ---
	print_test "Scorpion sur plusieurs fichiers" "./scorpion $SCORPION_DIR/*"
	./scorpion "$SCORPION_DIR"/*
	RESULT=$?
	check_result $RESULT "Scorpion a traité tous les fichiers"

	# --- Test 3 : Fichier invalide ---
	print_test "Fichier invalide (ne doit PAS crasher)" "./scorpion /dev/null"
	./scorpion /dev/null
	RESULT=$?
	check_result 0 "Scorpion n'a pas crashé sur un fichier invalide"

	# --- Test 4 : Fichier inexistant ---
	print_test "Fichier inexistant (ne doit PAS crasher)" "./scorpion fichier_qui_existe_pas.jpg"
	./scorpion fichier_qui_existe_pas.jpg
	check_result 0 "Scorpion n'a pas crashé sur un fichier inexistant"
fi

# Nettoyage
rm -rf /tmp/spider_test_demo/

# ============================================================================
#                              RÉSUMÉ
# ============================================================================
print_title "📊  RÉSUMÉ"
echo ""
echo -e "  ${GREEN}✅ PASS : $PASS${RESET}"
echo -e "  ${RED}❌ FAIL : $FAIL${RESET}"
echo ""
if [ $FAIL -eq 0 ]; then
	echo -e "${GREEN}  🎉 Tous les tests sont passés ! Prêt pour la correction.${RESET}"
else
	echo -e "${RED}  ⚠️  Certains tests ont échoué. Vérifier avant la soutenance.${RESET}"
fi
echo ""
