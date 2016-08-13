
#------------------------------------------------------------------------------
#-
#- Build Makefile for Decoder 
#-
#------------------------------------------------------------------------------

info:
	echo "rules: all clean install uninstall"


GSMRR_BASE=$(DIAG_BASE)/decoder/csnRRDecoder
GSMRR_BIN=$(GSMRR_BASE)/bin
LIBOUT=../bin/libgsmrr.a
#TARGET=IPC_FRM
#BIN_NAME=IPC_FRM

#------------------------------------------------------------
#- Macros for Include Directories
#------------------------------------------------------------
GSMRR_HDR=$(GSMRR_BASE)/inc/

HDRFILES = -I$(GSMRR_HDR)

#-------------------------------------------------------------
#- Define local c/cpp file(s) 
#-------------------------------------------------------------
COBJECTS = CSNRR.o        \
		CSNRRDec.o	\
		CSNRRPrtToStrm.o        \
		csnRRDecoder.o
#-------------------------------------------------------------
#- Define possible compiler flags
#-------------------------------------------------------------
#COMP_FLAG := -Wall
#CC = gcc $(COMP_FLAG) $(HDRFILES)
CC += $(CFLAGS) $(HDRFILES)

#-------------------------------------------------------------
#- Define make targets
#-------------------------------------------------------------
.PHONY: all
all : compile install 


#compile: $(COBJECTS)
#	$(CC) -o $(COBJECTS) 
compile: $(COBJECTS)
%.o: %.c
	$(CC) -c $< -o $@
install:
	$(AR) rcs $(LIBOUT) $(COBJECTS)
	cp $(LIBOUT) $(LIB_PATH)/

clean:
	rm -f *.o $(GSMRR_BIN)/*.o $(GSMRR_BIN)/*.a


uninstall:	
	rm -f *.o $(GSMRR_BIN)/*.o $(GSMRR_BIN)/*.a


.PHONY: debug
debug: $(COBJECTS)
	$(CC) -g -o $(COBJECTS)
