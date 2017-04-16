AM_CPPFLAGS =					\
	-DG_LOG_DOMAIN=\"gnome-c-utils\"	\
	$(WARN_CFLAGS)				\
	$(DEP_CFLAGS)

AM_LDFLAGS = $(WARN_LDFLAGS)

LDADD = $(DEP_LIBS)

bin_PROGRAMS =					\
	gcu-check-chain-ups			\
	gcu-lineup-parameters			\
	gcu-lineup-substitution			\
	gcu-multi-line-substitution		\
	gcu-smart-c-comment-substitution

gcu_check_chain_ups_SOURCES = \
	gcu-check-chain-ups.c

gcu_lineup_parameters_SOURCES = \
	gcu-lineup-parameters.c

gcu_lineup_substitution_SOURCES = \
	gcu-lineup-substitution.c

gcu_multi_line_substitution_SOURCES = \
	gcu-multi-line-substitution.c

gcu_smart_c_comment_substitution_SOURCES = \
	gcu-smart-c-comment-substitution.c

-include $(top_srcdir)/git.mk
