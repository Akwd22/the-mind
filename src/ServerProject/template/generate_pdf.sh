#!/bin/sh

echo "Generating PDF of the game stats..."

# Génération du fichier LaTeX à partir du template et des arguments reçus du serveur C.
if ! awk -f template/generate_latex.awk `echo $@` template/template.tex > template/out.tex 2> template/awk.log; then
  echo "Error occurred while generating the LaTeX file. Check logs in template/awk.log."
  exit
fi

# Génération du PDF.
if ! : | pdflatex -halt-on-error -no-shell-escape -output-directory=template template/out.tex > /dev/null 2> /dev/null ; then
  echo "Error occurred while generating PDF. Check logs in template/out.log."
  exit
fi

# Copier le PDF généré dans le répertoire des statistiques.
mkdir -p game_stats
cp template/out.pdf game_stats/`date +%d-%m-%Y_%Hh%Mm%Ss`.pdf

# Nettoyer les fichiers résiduels.
rm template/*.log
rm template/out.*

sleep 1

echo "PDF generated successfully. Check game_stats dir to view the file."
