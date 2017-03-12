AM_CPPFLAGS =					\
	-DG_LOG_DOMAIN=\"gnome-c-utils\"	\
	$(WARN_CFLAGS)				\
	$(DEP_CFLAGS)

AM_LDFLAGS = $(WARN_LDFLAGS)

LDADD = $(DEP_LIBS)

bin_PROGRAMS =				\
	check-chain-ups			\
	lineup-parameters		\
	lineup-substitution		\
	multi-line-substitution		\
	smart-c-comment-substitution

check_chain_ups_SOURCES =	\
	check-chain-ups.c

lineup_parameters_SOURCES =	\
	lineup-parameters.c

lineup_substitution_SOURCES =	\
	lineup-substitution.c

multi_line_substitution_SOURCES =	\
	multi-line-substitution.c

smart_c_comment_substitution_SOURCES =	\
	smart-c-comment-substitution.c

-include $(top_srcdir)/git.mk
