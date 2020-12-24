#!/bin/sh

./autogen.sh
CFLAGS="-fcommon -O2" LDFLAGS="-fcommon -O2" ./configure --prefix=/usr
make -j8 || make
