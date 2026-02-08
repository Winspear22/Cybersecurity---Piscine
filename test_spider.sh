#!/bin/bash
# test_spider.sh (VERSION ECLAIR ⚡️)

echo "🛠️  Reset: Nettoyage et Recompilation..."
make re > /dev/null
rm -rf ./data
rm -rf ./test_mix/
rm -rf ./test_double/

echo ""
echo "=========================================="
echo "🧪 TEST 1 : Basique (Profondeur 0)"
echo "=========================================="
./spider -l 0 http://books.toscrape.com/ 
ls -l ./data/ | head -5
echo "✅ Vérifie qu'il y a des images ci-dessus."

echo ""
echo "=========================================="
echo "🧪 TEST 2 : Le Mélangeur (-p -l -r)"
echo "=========================================="
# MODIF : -l 0 pour aller vite !
./spider -p ./test_mix/ -l 0 -r http://books.toscrape.com/
if [ -d "./test_mix/" ]; then
    echo "✅ Dossier test_mix créé."
else
    echo "❌ ÉCHEC : Dossier test_mix absent."
fi

echo ""
echo "=========================================="
echo "🧪 TEST 3 : Erreur Profondeur (-l -5)"
echo "=========================================="
./spider -l -5 http://books.toscrape.com/
echo "✅ Doit afficher 'Error : max depth must be a positive integer'"

echo ""
echo "=========================================="
echo "🧪 TEST 4 : Le Doppelgänger (Doublons)"
echo "=========================================="
echo "Passage 1 (Profondeur 0)..."
./spider -l 0 -p ./test_double/ http://books.toscrape.com/ > /dev/null
echo "Passage 2 (Profondeur 0)..."
./spider -l 0 -p ./test_double/ http://books.toscrape.com/ > /dev/null
count=$(ls ./test_double/ | wc -l)
echo "📂 Nombre total de fichiers dans test_double : $count"
if [ "$count" -ge 40 ]; then
    echo "✅ SUCCÈS : Doublons gérés ($count images)."
else
    echo "❌ ÉCHEC : Écrasement suspect ($count images)."
fi

echo ""
echo "=========================================="
echo "🧪 TEST 5 : URL Invalide"
echo "=========================================="
./spider http://site-qui-nexiste-pas-12345.com
echo "✅ Doit afficher une erreur Curl (pas de crash)."

echo ""
echo "🦂 Fin des tests ! Si tout est vert, tu es prêt."
