ICONV_VERSION = 1.12
WX_VERSION = 2.6.4
BOOST_VERSION = 1_34_1
QT_VERSION = 4.3.3

PREFIX = $(shell pwd)/inst
WGET = wget -c

# XXX: Hardcoded for mingw on linux, at the moment
CC = i586-mingw32msvc-gcc
CXX = i586-mingw32msvc-g++
CPPFLAGS += -I$(PREFIX)/include
CONFIGURE = CC=$(CC) CXX=$(CXX) CPPFLAGS=$(CPPFLAGS) ./configure --host=i586-mingw32msvc --build=i386-linux --prefix=$(PREFIX)

.PHONY: help all

help:
	echo "Usage: make all"

all: .iconv .wxWidgets .boost .qt


### iconv ###

ICONV_DIR = libiconv-$(ICONV_VERSION)
ICONV_ARCHIVE = libiconv-$(ICONV_VERSION).tar.gz

$(ICONV_DIR):
	$(WGET) http://ftp.gnu.org/pub/gnu/libiconv/$(ICONV_ARCHIVE)
	tar xzf $(ICONV_ARCHIVE)

.iconv: $(ICONV_DIR)
	(cd $< && $(CONFIGURE) --enable-static --disable-shared && make && make install)
	touch $@


### wxWidgets ###

WX_ARCHIVE = wxMSW-$(WX_VERSION).zip
WX_DIR = wxWidgets-$(WX_VERSION)
WX_DOS_FILES = config* *.sh install-sh mkinstalldirs

$(WX_DIR):
	$(WGET) http://heanet.dl.sourceforge.net/sourceforge/wxwindows/$(WX_ARCHIVE)
	unzip $(WX_ARCHIVE)

.wxWidgets: $(WX_DIR)
	(cd $< && dos2unix $(WX_DOS_FILES) && chmod +x $(WX_DOS_FILES))
	(cd $< && $(CONFIGURE) --disable-shared --enable-optimise --disable-debug --enable-unicode --without-libtiff --without-expat --without-zlib --without-libpng --without-libjpeg --without-regex --disable-mediactrl && make && make install)
	dos2unix $(PREFIX)/bin/wx-config
	touch $@


### Boost ###

BOOST_DIR = boost_$(BOOST_VERSION)
BOOST_ARCHIVE = boost_$(BOOST_VERSION).tar.bz2

$(BOOST_DIR):
	$(WGET) http://garr.dl.sourceforge.net/sourceforge/boost/$(BOOST_ARCHIVE)
	tar xjf $(BOOST_ARCHIVE)

# We don't build any library, because we don't need them (and it is really
# hard to cross-compile with their crappy build system)
.boost: $(BOOST_DIR)
	#(cd $< && ./configure --prefix=$(PREFIX) && ./tools/jam/src/bin.linuxx86/bjam --toolset=gcc --prefix=$(PREFIX) --without-date_time --without-filesystem --without-graph --without-iostreams --without-program_options --without-python --without-regex --without-serialization --without-signals --without-test --without-thread --without-wave install)
	cp -r $</boost $(PREFIX)/include
	touch $@



### Qt ###

# FIXME: No automated way at the moment :-(
QT_ARCHIVE = qt-win-opensource-$(QT_VERSION)-mingw.exe

$(QT_ARCHIVE):
	$(WGET) ftp://ftp.trolltech.com/qt/source/qt-win-opensource-4.3.3-mingw.exe

.qt: $(QT_ARCHIVE)
	@echo "=============== Important note ==============="
	@echo "You need to install $(QT_ARCHIVE) yourself using wine!"
	@echo "=============================================="
	touch $@
