lib_LTLIBRARIES = libparanormal.la
bin_PROGRAMS = pndoc paranormal

libdir = @XMMS_PLUGIN_DIR@/Visualization

libparanormal_la_LDFLAGS = -module -avoid-version

COMMON_SOURCES = actuators.c actuators.h paranormal.c paranormal.h
EXEC_SOURCES = presets.c presets.h cfg.c
BUILTIN_SOURCES = builtins.c pn_utils.h containers.c containers.h cmaps.c \
	freq.c wave.c general.c xform.c
XMMS_SOURCES = xmms.c
STANDALONE_SOURCES = standalone.c
PNDOC_SOURCES = pndoc.c

LIBS = @LIBS@ @XMMS_LIBS@ @GLIB_LIBS@ @GTK_LIBS@ @XML_LIBS@ @SDL_LIBS@
CFLAGS = @CFLAGS@ -Wall @XMMS_CFLAGS@ @GLIB_CFLAGS@ @GTK_CFLAGS@ \
	@XML_CFLAGS@ @SDL_CFLAGS@

libparanormal_la_SOURCES = $(COMMON_SOURCES) $(BUILTIN_SOURCES) $(EXEC_SOURCES) \
	$(XMMS_SOURCES)
paranormal_SOURCES = $(COMMON_SOURCES) $(BUILTIN_SOURCES) $(EXEC_SOURCES) \
	$(STANDALONE_SOURCES)
pndoc_SOURCES = $(COMMON_SOURCES) $(BUILTIN_SOURCES) $(PNDOC_SOURCES)
