MODULE_big = textsearch_ja
DATA_built = textsearch_ja.sql
DATA = uninstall_textsearch_ja.sql
OBJS = textsearch_ja.o encoding_eucjp.o encoding_utf8.o pgut/pgut-be.o
REGRESS = init convert textsearch_ja
SHLIB_LINK = -lmecab

ifndef USE_PGXS
top_builddir = ../..
makefile_global = $(top_builddir)/src/Makefile.global
ifeq "$(wildcard $(makefile_global))" ""
USE_PGXS = 1	# use pgxs if not in contrib directory
endif
endif

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/$(MODULE_big)
include $(makefile_global)
include $(top_srcdir)/contrib/contrib-global.mk
endif

# remove dependency to libxml2 and libxslt
LIBS := $(filter-out -lxml2, $(LIBS))
LIBS := $(filter-out -lxslt, $(LIBS))
