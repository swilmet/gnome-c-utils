AM_CPPFLAGS =					\
	-DG_LOG_DOMAIN=\"gnome-c-utils\"	\
	$(WARN_CFLAGS)				\
	$(DEP_CFLAGS)

AM_LDFLAGS = $(WARN_LDFLAGS)

LDADD = $(DEP_LIBS)

bin_PROGRAMS =			\
	check-chain-ups		\
	lineup-parameters	\
	lineup-substitution

check_chain_ups_SOURCES =	\
	check-chain-ups.c

lineup_parameters_SOURCES =	\
	lineup-parameters.c

lineup_substitution_SOURCES =	\
	lineup-substitution.c

-include $(top_srcdir)/git.mk
