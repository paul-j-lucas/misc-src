##
#	misc-src -- Miscellaneous C or C++ programs.
#	Makefile
#	https://github.com/paul-j-lucas/misc-src
#
#	Copyright (C) 2014-2020  Paul J. Lucas
#
#	This program is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; either version 2 of the Licence, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

# Directories.
BIN=		$(HOME)/bin

# Compiler & linker options.
CFLAGS=		-O2 -Wall
CXXFLAGS=	$(CFLAGS) -std=c++2a
LDFLAGS=	

# Commands.
RM=		rm -fr

# Targets.
ARGS=		$(BIN)/args
GETHOSTNAME=	$(BIN)/gethostname
MOD=		$(BIN)/mod
PSYSCONF=	$(BIN)/psysconf
SIZES=		$(BIN)/sizes
SUNDIAL=	$(BIN)/sundial
TARGETS=	$(ARGS) $(GETHOSTNAME) $(MOD) $(PSYSCONF) $(SIZES) $(SUNDIAL)

###############################################################################

all: $(TARGETS)

$(ARGS): args.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(GETHOSTNAME): gethostname.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(MOD): mod.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(PSYSCONF): psysconf.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(SIZES): sizes.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $<

$(SUNDIAL): sundial.c
	$(CC) $(CFLAGS) $(LDFLAGS) -lm -o $@ $<

clean:

distclean: clean
	$(RM) $(TARGETS)

# vim:set noet sw=8 ts=8:
