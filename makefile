

CC=gcc
KERN=Kernel.c Serv.c
FULLSRC=Kernel.c Serv.c MyCd.c MyPwd.c MyDu.c MyRm.c MyEcho.c MyCat.c MyChmod.c MyCp.c MyLs.c MyMkdir.c MyChown.c MyChgrp.c
DEFAULTSRC=MyPwd.c MyDu.c MyRm.c MyEcho.c MyCat.c MyChmod.c MyCp.c MyLs.c MyMkdir.c MyChown.c MyChgrp.c
FLAGC= -Wall
FLAGO= -fPIC
FLAGLIB= -shared
FLAGKERN= -Wl,--no-as-needed -ldl
DEFAULTSRCO= $(patsubst My%.c,My%.o,$(DEFAULTSRC))
WITHOUTMY= $(patsubst My%.c,%,$(DEFAULTSRC))


all: bashfull bashfile bashlib Cli

bashfull:
	-rm bash;
	cd BashFullStatic ;\
	$(CC) $(FLAGC) $(FULLSRC) -o  ../bashfull
	
	
	
bashfile:
		@-mkdir lib;
		cd BashUsingFile ;\
		for number in $(WITHOUTMY) ; do \
			$(CC) $(FLAGC) My$$number.c -o  `echo $$number | tr '[:upper:]' '[:lower:]'`;\
			mv ./`echo $$number | tr '[:upper:]' '[:lower:]'` ../lib/`echo $$number | tr '[:upper:]' '[:lower:]'`;\
		done
		cd BashUsingFile ;\
		$(CC) $(FLAGC) $(KERN) -o ../bashfile;\


		
bashlib:
		@-mkdir lib;\
		cd BashLib/lib ;\
		$(CC) $(FLAGO)  -c $(DEFAULTSRC) ;\
		$(CC) -o LibFunc.so $(FLAGLIB) $(DEFAULTSRCO) ;\
		cd .. ;\
		$(CC) $(FLAGC) $(KERN) -o  ../bashlib $(FLAGKERN)  ;\
		mv ./lib/LibFunc.so ../lib/LibFunc.so;\
	
		
clean:
		-rm -rf lib
		-rm bashlib
		-rm bashfull
		-rm bashfile
		-rm BashLib/lib/*.o
		-rm client

Cli:
	$(CC) $(FLAGC) Client/Cli.c -o  client
