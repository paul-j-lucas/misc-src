#! /bin/sh
##
#       make_version.sh
#       src/Makefile.am
#
#       Copyright (C) 2016  Paul J. Lucas
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the Licence, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

ME=`basename $0`
[ $# -eq 3 ] || { echo "usage: $ME header guard symbol" >&2; exit 1; }

HEADER="$1"
GUARD="$2"
SYMBOL="$3"

[ -f $HEADER ] && FILE_SHA=`grep $SYMBOL $HEADER | cut -f2 -d'"'`

command -v git >/dev/null &&
  CURRENT_SHA=`git --no-pager describe --always --dirty`

[ "$FILE_SHA" = "$CURRENT_SHA" ] || {
  echo "/* Generated from Makefile by make_version-sh. */"
  echo "#ifndef $GUARD"
  [ "$CURRENT_SHA" ] && echo "#define $SYMBOL \"$CURRENT_SHA\""
  echo "#endif /* $GUARD */"
} > $HEADER

# vim:set et sw=2 ts=2:
