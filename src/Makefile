#
# File:		Makefile for samples
# Author:	Robert Roebling
# Created:	1999
# Updated:	
# Copyright:	(c) 1998 Robert Roebling
#
# This makefile requires a Unix version of wxWindows
# to be installed on your system. This is most often
# done typing "make install" when using the complete
# sources of wxWindows or by installing the two
# RPM packages wxGTK.XXX.rpm and wxGTK-devel.XXX.rpm
# under Linux.

# this makefile may also be used as a template for simple makefiles for your
# own programs, the comments explain which parts of it you may have to modify
# to use it for your own programs


# you may change WX_CONFIG value to use a specific wx-config and to pass it
# some options, for example "--inplace" to use an uninstalled wxWindows
# version
#
# by default, wx-config from the PATH is used
WX_CONFIG := wx-config

# set this to the name of the main executable file
PROGRAM = wxtorus

# if your program has more than one source file, add more .o files to the line
# below
OBJECTS = wxtorus.o pin.o pipe.o preferencedialog.o roller.o\
	scorefile.o scorepanel.o top10dialog.o torus.o torusinternal.o\
	torusthread.o torusview.o


# you shouldn't have to edit anything below this line
CXX = $(shell $(WX_CONFIG) --cxx)

.SUFFIXES:	.o .cpp

.cpp.o :
	$(CXX) -c  `$(WX_CONFIG) --cxxflags` -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) `$(WX_CONFIG) --libs`

clean: 
	rm -f *.o $(PROGRAM)
