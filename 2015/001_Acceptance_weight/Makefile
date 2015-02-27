ROOTCFLAGS	=	$(shell root-config --cflags)
ROOTGLIBS		=	$(shell root-config --glibs)

CPP					=	g++
CPPFLAGS		=	-g -fPIC -Wno-deprecated -O2 -ansi
LD					=	g++
LDFLAGS			=	-g
SOFLAGS			=	-shared

CPPFLAGS		+= $(ROOTCFLAGS)
NGLIBS			=	$(ROOTGLIBS)
#NGLIBS      += -L/afs/cern.ch/work/m/miheejo/public/root_v5.28.00d/lib -lMathMore -lMinuit -lRooFit -lRooFitCore -lFoam
#NGLIBS      += -L/afs/cern.ch/user/k/kyolee/public/root_v5.28.00h/lib -lMathMore -lMinuit -lRooFit -lRooFitCore -lFoam
#NGLIBS      += -L/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/root/5.32.00-cms/lib -lMathMore -lMinuit -lRooFit -lRooFitCore -lFoam
NGLIBS      += -L/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/root/5.32.00-cms/lib -lMathMore -lMinuit -lFoam
GLIBS				= $(filter-out -lNew, $(NGLIBS))

INCLUDEDIR	=	./
CPP					+= -I$(INCLUDEDIR)
#CPP         += -I/afs/cern.ch/work/m/miheejo/public/root_v5.28.00d/include
#CPP         += -I/afs/cern.ch/user/k/kyolee/public/root_v5.28.00h/include
CPP         += -I/cvmfs/cms.cern.ch/slc6_amd64_gcc472/lcg/root/5.32.00-cms/include
OUTLIB			= ./

.SUFFIXES:	.cc,.C,.hh,.h
.PREFIXES:	./

AccDistMaker_batch:	$(INCLUDEDIR)S2_AccDistMaker_batch.cpp 
	$(CPP) $(CPPFLAGS) -o S2_AccDistMaker_batch $(GLIBS) $ $<

clean:
	rm -f $(OUTLIB)*.o $(OUTLIB)*.so
