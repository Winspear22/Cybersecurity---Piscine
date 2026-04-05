#!/bin/bash

# ============================================================================
#  Script de démo XXD — Arachnida (aligné sur le code source Scorpion)
# ============================================================================

GREEN="\033[1;32m"
CYAN="\033[1;36m"
YELLOW="\033[1;33m"
RED="\033[1;31m"
MAGENTA="\033[1;35m"
DIM="\033[2m"
RESET="\033[0m"

DIR="Examples"

separator() {
	echo -e "${DIM}────────────────────────────────────────────────────────────${RESET}"
}

explain() {
	echo -e "${MAGENTA}💡 $1${RESET}"
}

run_cmd() {
	echo -e "${CYAN}▶ Commande :${RESET} $1"
	separator
	eval "$1"
	separator
}

# ============================================================================
#                              MODE JPEG
# ============================================================================
demo_jpeg() {
	local FILE="Examples/best_jpeg.jpg"
	echo -e "${GREEN}═══════════════════════════════════════════════════${RESET}"
	echo -e "${GREEN}  📸 ANALYSE JPEG — $FILE${RESET}"
	echo -e "${GREEN}═══════════════════════════════════════════════════${RESET}\n"

	# --- Étape 1 : Magic Number (FileValidator.cpp ligne 64) ---
	echo -e "${YELLOW}[1/4] Magic Number — Les 3 premiers octets${RESET}"
	explain "Ton code dans FileValidator.cpp vérifie : header[0]==0xFF, header[1]==0xD8, header[2]==0xFF"
	explain "C'est la signature universelle JPEG. Si ces 3 octets ne matchent pas → rejeté."
	run_cmd "xxd -l 3 -g 1 $FILE"
	echo ""

	# --- Étape 2 : Segment APP1 / JFIF (ExifParserJPEG.cpp ligne 67-82) ---
	echo -e "${YELLOW}[2/4] Recherche du marqueur APP1 (segment Exif)${RESET}"
	explain "Ton code dans ExifParserJPEG parcourt les segments JPEG (marqueurs FF xx)."
	explain "Il cherche FF E1 = APP1, puis vérifie que les 6 octets suivants sont 'Exif\\\\00\\\\00'."
	explain "Après ces 6 octets → le Header TIFF commence (MM=big-endian ou II=little-endian)."
	run_cmd "xxd -l 32 -g 1 $FILE"
	echo -e "${DIM}  Octets 0-1 : FF D8 = SOI (Start Of Image)${RESET}"
	echo -e "${DIM}  Octet  3   : E0=APP0(JFIF) ou E1=APP1(Exif)${RESET}"
	echo ""

	# --- Étape 3 : Chercher les strings Exif en clair (ExifParserJPEG.cpp ligne 195-213) ---
	echo -e "${YELLOW}[3/4] Métadonnées en clair dans le binaire${RESET}"
	explain "Ton parser JPEG cherche les tags IFD : 0x010F=Make, 0x0110=Model,"
	explain "0x0131=Software, 0x0132=DateTime (type ASCII = type 2 dans l'IFD)."
	explain "Ces strings sont stockées EN CLAIR dans le fichier. On peut les voir avec grep :"
	run_cmd "xxd $FILE | grep -iE 'canon|nikon|sony|software|adobe|exif|date' | head -n 10"
	echo ""

	# --- Étape 4 : Byte Order (ExifParserJPEG.cpp ligne 98-102) ---
	echo -e "${YELLOW}[4/4] Byte Order — Big vs Little Endian${RESET}"
	explain "Ton code _detectByteOrder() cherche 'MM' (Motorola=Big) ou 'II' (Intel=Little)."
	explain "Cherchons le marqueur d'endianness dans le fichier :"
	run_cmd "xxd $FILE | grep -E '4d4d|4949|MM|II' | head -n 3"
	echo -e "${DIM}  4d4d = 'MM' = Big Endian (Motorola)${RESET}"
	echo -e "${DIM}  4949 = 'II' = Little Endian (Intel)${RESET}"
}

# ============================================================================
#                              MODE PNG
# ============================================================================
demo_png() {
	local FILE="Examples/best_png.png"
	echo -e "${GREEN}═══════════════════════════════════════════════════${RESET}"
	echo -e "${GREEN}  🖼️  ANALYSE PNG — $FILE${RESET}"
	echo -e "${GREEN}═══════════════════════════════════════════════════${RESET}\n"

	# --- Étape 1 : Signature PNG (FileValidator.cpp ligne 66) ---
	echo -e "${YELLOW}[1/3] Signature PNG — Les 8 premiers octets${RESET}"
	explain "Ton code vérifie les 4 premiers : 0x89, 0x50('P'), 0x4E('N'), 0x47('G')."
	explain "En réalité, la signature complète fait 8 octets : 89 50 4E 47 0D 0A 1A 0A"
	explain "Le '0D 0A' (\\r\\n) est un piège anti-corruption réseau : si un transfert"
	explain "FTP corrompt les fins de ligne, la signature change → fichier détecté comme invalide."
	run_cmd "xxd -l 8 -g 1 $FILE"
	echo ""

	# --- Étape 2 : Chunk IHDR (ExifParserPNG.cpp ligne 68-104) ---
	echo -e "${YELLOW}[2/3] Premier chunk : IHDR (dimensions et couleurs)${RESET}"
	explain "Ton parser PNG lit les chunks en boucle : [4 len][4 type][N data][4 CRC]."
	explain "IHDR est toujours le 1er chunk. Ton code lit :"
	explain "  → _read32 à offset 0 du chunk = width  (big-endian)"
	explain "  → _read32 à offset 4 = height"
	explain "  → data[8] = bit depth, data[9] = color type (0=Gray, 2=RGB, 6=RGBA)"
	run_cmd "xxd -l 33 -g 1 $FILE"
	echo -e "${DIM}  Octets  8-11 : longueur du chunk IHDR (00 00 00 0D = 13 octets)${RESET}"
	echo -e "${DIM}  Octets 12-15 : 'IHDR' en ASCII (49 48 44 52)${RESET}"
	echo -e "${DIM}  Octets 16-19 : largeur (big-endian)${RESET}"
	echo -e "${DIM}  Octets 20-23 : hauteur (big-endian)${RESET}"
	echo ""

	# --- Étape 3 : Chunks tEXt (ExifParserPNG.cpp ligne 108-133) ---
	echo -e "${YELLOW}[3/3] Chunks tEXt — Métadonnées textuelles${RESET}"
	explain "Ton parser cherche les chunks 'tEXt' qui contiennent : keyword\\0texte."
	explain "C'est comme ça que PNG stocke Author, Comment, Software, etc."
	explain "Cherchons des mots-clés en clair :"
	run_cmd "xxd $FILE | grep -iE 'text|author|comment|software|title|creat' | head -n 10"
}

# ============================================================================
#                              MODE GIF
# ============================================================================
demo_gif() {
	local FILE="Examples/best_gif.gif"
	echo -e "${GREEN}═══════════════════════════════════════════════════${RESET}"
	echo -e "${GREEN}  🎞️  ANALYSE GIF — $FILE${RESET}"
	echo -e "${GREEN}═══════════════════════════════════════════════════${RESET}\n"

	# --- Étape 1 : Signature GIF (FileValidator.cpp ligne 68) ---
	echo -e "${YELLOW}[1/3] Signature GIF — Les 6 premiers octets${RESET}"
	explain "Ton code vérifie 4 octets : 0x47('G'), 0x49('I'), 0x46('F'), 0x38('8')."
	explain "Mais la version complète fait 6 octets : 'GIF87a' ou 'GIF89a'."
	explain "Ton parser GIF lit les 6 avec : version += (char)data[i] (boucle 0..5)."
	explain "  → GIF87a = version originale (1987), pas de transparence."
	explain "  → GIF89a = version étendue (1989), supporte transparence et animation."
	run_cmd "xxd -l 6 -g 1 $FILE"
	echo ""

	# --- Étape 2 : Dimensions (ExifParserGIF.cpp ligne 75-78) ---
	echo -e "${YELLOW}[2/3] Dimensions — Octets 6 à 9 (LITTLE-endian)${RESET}"
	explain "Ton code utilise _read16(data, 6, false) pour la largeur (false = little-endian)."
	explain "Et _read16(data, 8, false) pour la hauteur."
	explain "⚠️  GIF utilise le LITTLE-endian : l'octet faible est EN PREMIER."
	explain "Ex : si tu vois '90 01' → c'est 0x0190 = 400 pixels (pas 0x9001 !)"
	run_cmd "xxd -l 10 -g 1 $FILE"
	echo -e "${DIM}  Octets 0-5  : Signature 'GIF89a'${RESET}"
	echo -e "${DIM}  Octets 6-7  : Largeur (little-endian)${RESET}"
	echo -e "${DIM}  Octets 8-9  : Hauteur (little-endian)${RESET}"
	echo ""

	# --- Étape 3 : Packed Byte (ExifParserGIF.cpp ligne 81-93) ---
	echo -e "${YELLOW}[3/3] Packed Byte — Octet 10 (palette de couleurs)${RESET}"
	explain "Ton code lit data[10] et extrait :"
	explain "  → bit 7 (masque 0x80) : palette globale présente ?"
	explain "  → bits 0-2 (masque 0x07) : taille palette = 2^(n+1) couleurs."
	explain "Ex : packed=0xF7 → bit7=1 (palette), bits 0-2 = 7 → 2^8 = 256 couleurs."
	run_cmd "xxd -l 13 -g 1 $FILE"
	echo -e "${DIM}  Octet 10    : Packed byte (palette + résolution couleur)${RESET}"
	echo -e "${DIM}  Octet 11    : Index couleur de fond${RESET}"
	echo -e "${DIM}  Octet 12    : Pixel Aspect Ratio${RESET}"
}

# ============================================================================
#                              MODE BMP
# ============================================================================
demo_bmp() {
	local FILE="Examples/best_bmp.bmp"
	echo -e "${GREEN}═══════════════════════════════════════════════════${RESET}"
	echo -e "${GREEN}  🟦 ANALYSE BMP — $FILE${RESET}"
	echo -e "${GREEN}═══════════════════════════════════════════════════${RESET}\n"

	# --- Étape 1 : Magic Number (FileValidator.cpp ligne 70) ---
	echo -e "${YELLOW}[1/3] Magic Number — Les 2 premiers octets${RESET}"
	explain "Ton code vérifie seulement 2 octets : header[0]==0x42('B'), header[1]==0x4D('M')."
	explain "C'est la signature la plus courte parmi les 4 formats supportés."
	explain "BMP est aussi le format le plus SIMPLE : pas de chunks, pas de segments."
	run_cmd "xxd -l 2 -g 1 $FILE"
	echo ""

	# --- Étape 2 : File Header complet [14 octets] ---
	echo -e "${YELLOW}[2/3] File Header — Les 14 premiers octets${RESET}"
	explain "Structure BMP File Header (tout en little-endian) :"
	explain "  Octets  0-1  : 'BM' (Magic Number)"
	explain "  Octets  2-5  : Taille totale du fichier en octets"
	explain "  Octets  6-9  : Réservé (toujours 00 00 00 00)"
	explain "  Octets 10-13 : Offset vers le début des pixels (important !)"
	run_cmd "xxd -l 14 -g 1 $FILE"
	echo ""

	# --- Étape 3 : DIB Header (ExifParserBMP.cpp ligne 60-90) ---
	echo -e "${YELLOW}[3/3] DIB Header — Informations image (offset 14+)${RESET}"
	explain "Ton parser BMP commence à l'offset 14 et lit (little-endian) :"
	explain "  → _read32(data, 18, false) = largeur"
	explain "  → _read32(data, 22, false) = hauteur (négatif = image top-down)"
	explain "  → _read16(data, 28, false) = bits par pixel (1, 4, 8, 24, 32)"
	explain "  → _read32(data, 30, false) = compression (0=aucune, 1=RLE8, 2=RLE4)"
	run_cmd "xxd -l 34 -g 1 $FILE"
	echo -e "${DIM}  Octets 14-17 : Taille du DIB header (40 = BITMAPINFOHEADER)${RESET}"
	echo -e "${DIM}  Octets 18-21 : Largeur (little-endian, signé)${RESET}"
	echo -e "${DIM}  Octets 22-25 : Hauteur (little-endian, signé)${RESET}"
	echo -e "${DIM}  Octets 28-29 : Bits par pixel${RESET}"
	echo -e "${DIM}  Octets 30-33 : Type de compression${RESET}"
}

# ============================================================================
#                          COMPARAISON GLOBALE
# ============================================================================
demo_compare() {
	echo -e "${GREEN}═══════════════════════════════════════════════════${RESET}"
	echo -e "${GREEN}  🔍 COMPARAISON DES MAGIC NUMBERS${RESET}"
	echo -e "${GREEN}═══════════════════════════════════════════════════${RESET}\n"
	explain "Voici ce que ton FileValidator.cpp vérifie pour chaque format."
	explain "C'est la même logique que dans ExifParser::displayExifData() (Factory)."
	echo ""

	echo -e "${YELLOW}JPEG (3 octets) :${RESET}"
	run_cmd "xxd -l 8 -g 1 Examples/best_jpeg.jpg"
	echo -e "  ${DIM}→ Vérifié par : header[0]==0xFF && header[1]==0xD8 && header[2]==0xFF${RESET}\n"

	echo -e "${YELLOW}PNG (4 octets) :${RESET}"
	run_cmd "xxd -l 8 -g 1 Examples/best_png.png"
	echo -e "  ${DIM}→ Vérifié par : header[0]==0x89 && header[1]==0x50 && header[2]==0x4E && header[3]==0x47${RESET}\n"

	echo -e "${YELLOW}GIF (4 octets) :${RESET}"
	run_cmd "xxd -l 8 -g 1 Examples/best_gif.gif"
	echo -e "  ${DIM}→ Vérifié par : header[0]==0x47 && header[1]==0x49 && header[2]==0x46 && header[3]==0x38${RESET}\n"

	echo -e "${YELLOW}BMP (2 octets) :${RESET}"
	run_cmd "xxd -l 8 -g 1 Examples/best_bmp.bmp"
	echo -e "  ${DIM}→ Vérifié par : header[0]==0x42 && header[1]==0x4D${RESET}\n"

	explain "Si extension ≠ magic number → ton code affiche 'Mismatch detected'."
	explain "C'est la cross-validation extension vs contenu réel."
}

# ============================================================================
#                              MENU
# ============================================================================

case $1 in
	1|jpeg|jpg)
		demo_jpeg ;;
	2|png)
		demo_png ;;
	3|gif)
		demo_gif ;;
	4|bmp)
		demo_bmp ;;
	5|compare|all)
		demo_compare ;;
	*)
		echo -e "${CYAN}📜 XXD DEMO — Guide de soutenance Arachnida${RESET}"
		separator
		echo -e "${YELLOW}  1 | jpeg${RESET}   → Analyse JPEG (Magic FF D8 FF, Exif, Byte Order)"
		echo -e "${YELLOW}  2 | png${RESET}    → Analyse PNG  (Signature 8 octets, Chunks IHDR/tEXt)"
		echo -e "${YELLOW}  3 | gif${RESET}    → Analyse GIF  (GIF89a, Dimensions LE, Packed byte)"
		echo -e "${YELLOW}  4 | bmp${RESET}    → Analyse BMP  (Magic 'BM', DIB Header, BPP)"
		echo -e "${YELLOW}  5 | compare${RESET} → Compare les Magic Numbers des 4 formats"
		separator
		echo -e "${CYAN}💡 Utilisation :${RESET} ./xxd_demo.sh [1-5]"
		echo -e "${CYAN}💡 Exemples   :${RESET} ./xxd_demo.sh jpeg  |  ./xxd_demo.sh 5"
		separator
		echo -e "${DIM}Chaque mode explique le lien entre le binaire et TON code Scorpion.${RESET}"
		;;
esac
