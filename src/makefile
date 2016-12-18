#
# Makefile for sys level program
#
# Bob Eager   May 2000
#
# Product names
#
PRODUCT		= syslev
#
# Target directory
#
TARGET		= f:\binp
#
# Compiler setup
#
CC		= icc
!IFDEF DEBUG
DBUG		= -DDEBUG
!ELSE
DBUG		=
!ENDIF
!IFDEF	PROD
CFLAGS		= $(DBUG) -Fi -G4 -O -Q -Se -Si
!ELSE
CFLAGS		= $(DBUG) -Fi -G4 -Q -Se -Si -Ti -Tm -Tx
!ENDIF
#
# Names of object files
#
OBJ =		syslev.obj
#
# Other files
#
DEF =		$(PRODUCT).def
LNK =		$(PRODUCT).lnk
#
# Final executable file
#
EXE =		$(PRODUCT).exe
#
#-----------------------------------------------------------------------------
#
install:	$(EXE)
		@copy $(EXE) $(TARGET) > nul
#
$(EXE):		$(OBJ) $(LNK) $(DEF)
!IFDEF	PROD
		ilink /nologo /exepack:2 @$(LNK)
!ELSE
		ilink /debug /nobrowse /nologo @$(LNK)
!ENDIF
#
# Object files
#
syslev.obj:	syslev.c
#
# Linker response file. Rebuild if makefile changes
#
$(LNK):		makefile
		@if exist $(LNK) erase $(LNK)
		@echo /map:$(PRODUCT) >> $(LNK)
		@echo /out:$(PRODUCT) >> $(LNK)
		@echo $(OBJ) >> $(LNK)
		@echo $(DEF) >> $(LNK)
#
clean:		
		-erase $(OBJ) $(LNK) $(PRODUCT).map csetc.pch
#
# End of makefile for sys level program
#
