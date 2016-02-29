#MAKEFILE l-bniyat 3 ha-tokniot l-nittux curani llo histabbruyot 

#The SOURCEDIR is used with unix-like commands, so it is written with forward-slash.
SOURCEDIR=../cpp

#The OBJECTDIR is used with DOS commands, so it is written with back-slash.
OBJECTDIR=../cpp

CXX=g++
CXXFLAGS=-I$(SOURCEDIR) -I/devcpp/g++ -DUNIX $(CXXDEBUGFLAG)

LINK=g++
LINKFLAGS=$(CXXDEBUGFLAG)

.PHONY: all clean debug

all: mch.exe mcht.exe mc.exe

clean:
	rm $(OBJECTDIR)/cpplib/*.o
	rm $(OBJECTDIR)/morph/*.o
	rm $(OBJECTDIR)/cpplib/*.dep
	rm $(OBJECTDIR)/morph/*.dep


%.dep: %.cpp
	$(CXX) -M -o $(<D)/$(@F) $(CXXFLAGS) $<

%.o: %.cpp
	$(CXX) -c -o $(<D)/$(@F) $(CXXFLAGS) $<

MNTXCURNI_UNITS = \
  cpplib/cmdline1 \
  cpplib/klli1 \
  cpplib/stringt \
  cpplib/ui \
  morph/lexicon \
  morph/lexinf \
  morph/morphinf \
  morph/morphtav \
  morph/morphtyp \
  morph/morphanl \
  morph/tavnit 
MNTXCURNI_OBJECTS = $(MNTXCURNI_UNITS:%=$(SOURCEDIR)/%.o) $(SOURCEDIR)/morph/morphanl.test.o

MNTXCURNIHSTBRUTI_UNITS= $(MNTXCURNI_UNITS) \
  morph/addlex \
  morph/alghist \
  cpplib/argsfile \
  morph/bw2fw \
  morph/corpus \
  morph/hpmainit \
  morph/hpmaio \
  morph/hpmamain \
  morph/mip \
  morph/mone-nit \
  morph/morphsik \
  morph/morphst2 \
  morph/morphui \
  morph/sntncinf \
  cpplib/sgt \
  morph/similar \
  morph/tiqun3 \
  cpplib/wordcnt
MNTXCURNIHSTBRUTI_OBJECTS = $(MNTXCURNIHSTBRUTI_UNITS:%=$(SOURCEDIR)/%.o) $(SOURCEDIR)/morph/MntxCurniHstbruti.main.o

MNTXCURNIHSTBRUTITXBIRI_UNITS= $(MNTXCURNIHSTBRUTI_UNITS) \
  morph/sntncas \
  morph/soi \
  morph/cimcumim
MNTXCURNIHSTBRUTITXBIRI_OBJECTS = $(MNTXCURNIHSTBRUTITXBIRI_UNITS:%=$(SOURCEDIR)/%.o) $(SOURCEDIR)/morph/MntxCurniHstbrutiTxbiri.main.o

debug:
	echo $(MNTXCURNIHSTBRUTI_OBJECTS)

#include $(MNTXCURNIHSTBRUTITXBIRI_UNITS:%=$(SOURCEDIR)/%.dep)

mc.exe: $(MNTXCURNI_OBJECTS)
	$(LINK) -o $@ $(LINKFLAGS) $(MNTXCURNI_OBJECTS) $(LIBRARIES_FLAGS) $(LIBRARIES)

mch.exe: $(MNTXCURNIHSTBRUTI_OBJECTS)
	$(LINK) -o $@ $(LINKFLAGS) $(MNTXCURNIHSTBRUTI_OBJECTS) $(LIBRARIES_FLAGS) $(LIBRARIES)

mcht.exe: $(MNTXCURNIHSTBRUTITXBIRI_OBJECTS)
	$(LINK) -o $@ $(LINKFLAGS) $(MNTXCURNIHSTBRUTITXBIRI_OBJECTS) $(LIBRARIES_FLAGS) $(LIBRARIES)



TESTMORPHTAV_UNITS = \
	cpplib/klli1 \
	cpplib/stringt \
	cpplib/ui \
	morph/morphinf \
	morph/morphtav \
	morph/morphtyp \
	morph/tavnit \
	morph/morphtav.test
TestMorphTav.exe: $(TESTMORPHTAV_UNITS:%=$(SOURCEDIR)/%.o)
	$(LINK) -o $@ $(LINKFLAGS) $^ $(LIBRARIES_FLAGS) $(LIBRARIES)

TESTSKIP_UNITS = \
	cpplib/klli1 \
	cpplib/stringt \
	cpplib/ui \
	cpplib/klli1.test
TestSkip.exe: $(TESTSKIP_UNITS:%=$(SOURCEDIR)/%.o)
	$(LINK) -o $@ $(LINKFLAGS) $^ $(LIBRARIES_FLAGS) $(LIBRARIES)


CONVLANG_UNITS = \
  cpplib/cmdline1 \
  cpplib/klli1 \
  cpplib/stringt \
  cpplib/ui \
  cpplib/ivrit \
  cpplib/convert1 \
  morph/convlang
CONVLANG_OBJECTS=$(CONVLANG_UNITS:%=$(SOURCEDIR)/%.o) $(SOURCEDIR)/morph/convlang.test.o 

convlang.exe: $(CONVLANG_OBJECTS)
	$(LINK) -o $@ $(LINKFLAGS) $(CONVLANG_OBJECTS) $(LIBRARIES_FLAGS) $(LIBRARIES)

