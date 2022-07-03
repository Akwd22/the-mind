#!/bin/sh

while true
do
	clear
	doxygen Shared.cfg
	doxygen ServerProject.cfg
	doxygen ClientProject.cfg
	doxygen BotProject.cfg
	echo Appuyez sur une touche pour regénérer la documentation...
	read NONE
done