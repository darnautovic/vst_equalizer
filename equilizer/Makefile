# change this to the location of your unpacked VST SDK:
VSTSDKDIR	=	../../../vstsdk2.4

CPP			= g++.exe
OBJ			= Ekvalizator.o $(VSTSDKDIR)/public.sdk/source/vst2.x/vstplugmain.o $(VSTSDKDIR)/public.sdk/source/vst2.x/audioeffect.o $(VSTSDKDIR)/public.sdk/source/vst2.x/audioeffectx.o
LIBS		= -L. --add-stdcall-alias -lole32 -lkernel32 -lgdi32 -luuid -luser32 -mwindows --no-export-all-symbols --def Ekvalizator.def   
CXXINCS		= -I"$(VSTSDKDIR)/pluginterfaces/vst2.x" -I"$(VSTSDKDIR)/public.sdk/source/vst2.x" -I"$(VSTSDKDIR)" -I"$(VSTSDKDIR)/vstgui.sf/vstgui" -I.
BIN			= Ekvalizator.dll
CXXFLAGS	= $(CXXINCS)-static-libgcc -static-libstdc++ -DBUILDING_DLL=1 -mwindows -O3 
RM			= rm -f

.PHONY:	all clean

all: Ekvalizator.dll

clean: 
		${RM} $(OBJ) $(BIN)

DLLWRAP		= dllwrap.exe
DEFFILE		= libEkvalizator.def
STATICLIB	= libEkvalizator.a

$(BIN):	$(OBJ)
		$(DLLWRAP) --output-def $(DEFFILE) --driver-name c++ -static --implib $(STATICLIB) $(OBJ) $(LIBS) -o $(BIN)

Ekvalizator.o: Ekvalizator.cpp
		$(CPP) -c Ekvalizator.cpp -o Ekvalizator.o $(CXXFLAGS)

$(VSTSDKDIR)/public.sdk/source/vst2.x/vstplugmain.o: $(VSTSDKDIR)/public.sdk/source/vst2.x/vstplugmain.cpp
		$(CPP) -c $(VSTSDKDIR)/public.sdk/source/vst2.x/vstplugmain.cpp -o $(VSTSDKDIR)/public.sdk/source/vst2.x/vstplugmain.o $(CXXFLAGS)

$(VSTSDKDIR)/public.sdk/source/vst2.x/audioeffect.o: $(VSTSDKDIR)/public.sdk/source/vst2.x/audioeffect.cpp
		$(CPP) -c $(VSTSDKDIR)/public.sdk/source/vst2.x/audioeffect.cpp -o $(VSTSDKDIR)/public.sdk/source/vst2.x/audioeffect.o $(CXXFLAGS)

$(VSTSDKDIR)/public.sdk/source/vst2.x/audioeffectx.o: $(VSTSDKDIR)/public.sdk/source/vst2.x/audioeffectx.cpp
		$(CPP) -c $(VSTSDKDIR)/public.sdk/source/vst2.x/audioeffectx.cpp -o $(VSTSDKDIR)/public.sdk/source/vst2.x/audioeffectx.o $(CXXFLAGS)