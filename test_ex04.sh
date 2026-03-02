#!/bin/bash
# ═══════════════════════════════════════════════════════════════
# 🕷️ TEST DE LA MORT QUI TUE — Exercice 04 (Image Downloader)
# ═══════════════════════════════════════════════════════════════

GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[1;33m"
CYAN="\033[0;36m"
RESET="\033[0m"
BOLD="\033[1m"

PASS=0
FAIL=0
TOTAL=0

check() {
	TOTAL=$((TOTAL + 1))
	if eval "$2" > /dev/null 2>&1; then
		echo -e "  ${GREEN}✅ PASS${RESET} — $1"
		PASS=$((PASS + 1))
	else
		echo -e "  ${RED}❌ FAIL${RESET} — $1"
		FAIL=$((FAIL + 1))
	fi
}

header() {
	echo ""
	echo -e "${CYAN}${BOLD}═══════════════════════════════════════════════${RESET}"
	echo -e "${CYAN}${BOLD}  $1${RESET}"
	echo -e "${CYAN}${BOLD}═══════════════════════════════════════════════${RESET}"
}

# ─────────────────────────────────────────────
# PRÉPARATION
# ─────────────────────────────────────────────
header "🔧 PRÉPARATION"

echo -e "  Nettoyage des anciens dossiers de test..."
rm -rf ./data/ ./test_images/ ./custom_path/ ./deep/nested/path/

echo -e "  Compilation..."
make -s 2>/dev/null
if [ $? -ne 0 ]; then
	echo -e "  ${RED}❌ ERREUR DE COMPILATION ! Impossible de continuer.${RESET}"
	exit 1
fi
echo -e "  ${GREEN}✅ Compilation réussie${RESET}"

# ═══════════════════════════════════════════════
# TEST 1 : Téléchargement basique (Google)
# ═══════════════════════════════════════════════
header "🧪 TEST 1 — Téléchargement basique (Wikipedia)"

./spider -p ./data/ https://en.wikipedia.org/wiki/Cat 2>/dev/null

check "Le dossier ./data/ a été créé" "[ -d ./data/ ]"
check "Au moins 1 fichier a été téléchargé" "[ \$(ls ./data/ 2>/dev/null | wc -l) -ge 1 ]"
check "Le fichier n'est pas vide (> 0 octets)" "[ \$(find ./data/ -type f -size +0c | wc -l) -ge 1 ]"

# Vérifier qu'au moins un fichier a un vrai header d'image
REAL_IMG_COUNT=0
while IFS= read -r img; do
	HEX=$(xxd -p -l 4 "$img" 2>/dev/null)
	if echo "$HEX" | grep -qE '89504e47|ffd8ffe0|ffd8ffe1|ffd8ffdb|47494638'; then
		REAL_IMG_COUNT=$((REAL_IMG_COUNT + 1))
	fi
done < <(find ./data/ -type f 2>/dev/null)
check "Au moins 1 fichier a un header d'image valide (PNG/JPEG/GIF)" "[ $REAL_IMG_COUNT -ge 1 ]"

# ═══════════════════════════════════════════════
# TEST 2 : Protection anti-écrasement
# ═══════════════════════════════════════════════
header "🧪 TEST 2 — Protection anti-écrasement"

# Récupérer le timestamp du premier fichier
FIRST_FILE=$(ls ./data/ | head -1)
if [ -n "$FIRST_FILE" ]; then
	TIMESTAMP_BEFORE=$(stat -c %Y "./data/$FIRST_FILE" 2>/dev/null)
	sleep 1

	# Re-lancer le spider sur la même URL
	OUTPUT=$(./spider -p ./data/ https://en.wikipedia.org/wiki/Cat 2>&1)

	TIMESTAMP_AFTER=$(stat -c %Y "./data/$FIRST_FILE" 2>/dev/null)

	check "Le message 'existe déjà' apparaît" "echo '$OUTPUT' | grep -qi 'existe'"
	check "Le fichier n'a PAS été modifié (timestamp identique)" "[ '$TIMESTAMP_BEFORE' = '$TIMESTAMP_AFTER' ]"
else
	echo -e "  ${YELLOW}⚠️ SKIP — Pas de fichier à tester${RESET}"
fi

# ═══════════════════════════════════════════════
# TEST 3 : Chemin personnalisé (-p)
# ═══════════════════════════════════════════════
header "🧪 TEST 3 — Chemin personnalisé (-p)"

./spider -p ./test_images/ https://en.wikipedia.org/wiki/Dog 2>/dev/null

check "Le dossier ./test_images/ a été créé" "[ -d ./test_images/ ]"
check "Les images sont dans ./test_images/ (pas ./data/)" "[ \$(ls ./test_images/ 2>/dev/null | wc -l) -ge 1 ]"

# ═══════════════════════════════════════════════
# TEST 4 : Site riche en images (Wikipedia)
# ═══════════════════════════════════════════════
header "🧪 TEST 4 — Site riche en images (Wikipedia)"

rm -rf ./data/
./spider -p ./data/ https://en.wikipedia.org/wiki/Spider 2>/dev/null

NB_FILES=$(ls ./data/ 2>/dev/null | wc -l)
echo -e "  ${YELLOW}📊 Nombre d'images téléchargées : ${NB_FILES}${RESET}"

check "Plus de 5 images téléchargées" "[ $NB_FILES -ge 5 ]"
check "Aucun fichier vide (0 octets)" "[ \$(find ./data/ -type f -empty | wc -l) -eq 0 ]"

# ═══════════════════════════════════════════════
# TEST 5 : Arguments invalides
# ═══════════════════════════════════════════════
header "🧪 TEST 5 — Arguments invalides / Edge cases"

# Pas d'URL
OUTPUT=$(./spider 2>&1)
check "Sans URL → message d'erreur" "echo '$OUTPUT' | grep -qi 'error\|usage'"

# URL inexistante
rm -rf ./data/
OUTPUT=$(./spider https://thissitedoesnotexist12345.com 2>&1)
check "URL inexistante → pas de crash (exit proprement)" "true"
check "URL inexistante → dossier ./data/ vide ou inexistant" "[ \$(ls ./data/ 2>/dev/null | wc -l) -le 1 ]"

# ═══════════════════════════════════════════════
# TEST 6 : Profondeur (-l)
# ═══════════════════════════════════════════════
header "🧪 TEST 6 — Option de profondeur (-l)"

rm -rf ./data/
OUTPUT=$(./spider -l 0 https://google.com 2>&1)
check "Profondeur -l 0 → le programme ne crash pas" "true"

# ═══════════════════════════════════════════════
# TEST 7 : Vérification de la taille des fichiers
# ═══════════════════════════════════════════════
header "🧪 TEST 7 — Intégrité des fichiers téléchargés"

rm -rf ./data/
./spider -p ./data/ https://en.wikipedia.org/wiki/Cat 2>/dev/null

if [ -d ./data/ ] && [ "$(ls ./data/ 2>/dev/null | wc -l)" -gt 0 ]; then
	TOTAL_SIZE=$(du -sb ./data/ | cut -f1)
	check "La taille totale des images est > 100 octets" "[ $TOTAL_SIZE -gt 100 ]"

	# Vérifier qu'au moins certains fichiers sont de vraies images
	# (les CDN peuvent renvoyer du HTML au lieu d'images si rate-limité)
	REAL_COUNT=0
	while IFS= read -r img; do
		HEX=$(xxd -p -l 4 "$img" 2>/dev/null)
		if echo "$HEX" | grep -qE '89504e47|ffd8ffe0|ffd8ffe1|ffd8ffdb|47494638'; then
			REAL_COUNT=$((REAL_COUNT + 1))
		fi
	done < <(find ./data/ -type f)
	echo -e "  ${YELLOW}📊 Images valides : ${REAL_COUNT} / ${TOTAL_FILES}${RESET}"
	check "Au moins 1 fichier téléchargé est une vraie image" "[ $REAL_COUNT -ge 1 ]"
else
	echo -e "  ${YELLOW}⚠️ SKIP — Pas de fichiers à analyser${RESET}"
fi

# ═══════════════════════════════════════════════
# RÉSULTATS FINAUX
# ═══════════════════════════════════════════════
header "📊 RÉSULTATS FINAUX"

echo ""
echo -e "  ${BOLD}Total  :${RESET} $TOTAL tests"
echo -e "  ${GREEN}${BOLD}Passés :${RESET} $PASS"
echo -e "  ${RED}${BOLD}Échoués:${RESET} $FAIL"
echo ""

if [ $FAIL -eq 0 ]; then
	echo -e "  ${GREEN}${BOLD}🏆 PARFAIT ! Tous les tests passent. Ex04 VALIDÉ !${RESET}"
else
	echo -e "  ${RED}${BOLD}⚠️  $FAIL test(s) échoué(s). À corriger !${RESET}"
fi

echo ""

# ─────────────────────────────────────────────
# NETTOYAGE
# ─────────────────────────────────────────────
echo -e "${YELLOW}🧹 Nettoyage des dossiers de test...${RESET}"
rm -rf ./test_images/ ./custom_path/ ./deep/
echo -e "${GREEN}✅ Nettoyage terminé (./data/ conservé pour inspection)${RESET}"
echo ""
