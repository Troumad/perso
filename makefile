.PHONY: win

all: option

option:
	@echo "Utilisation : man <option>"
	@echo "option mrproper et clean pour nettoyer plus ou moins"
	@echo "option windows ou win pour créer un exécutable windows 32 bits"
	@echo "option windows64 ou win64 pour créer un exécutable windows 64 bits"
	@echo "option linux pour créer un exécutable linux en mode débug"
	@echo "option lin pour créer un exécutable linux"
	@echo ""
	@echo "exemple pour créer un exécutabe pour windows 64 bits : \"man win64\""

windows:
	make -f makefile.windows

win: windows

win64:
	make -f makefile.windows64

lin: 
	make -f makefile.lin
	
lin4: 
	make -f makefile.lin4	
	
linux: 
	 make -f makefile.linux


clean:
		rm *.o *.obj *.obj32
		 
mrproper: clean
		rm perso.exe perso

