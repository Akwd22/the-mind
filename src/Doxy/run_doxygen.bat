@echo off
:loop
	cls
	doxygen Shared.cfg
	doxygen ServerProject.cfg
	doxygen ClientProject.cfg
	doxygen BotProject.cfg
	echo Appuyez sur une touche pour reg‚n‚rer la documentation...
	pause >NUL
goto loop