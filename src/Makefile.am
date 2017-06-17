AM_CPPFLAGS =					\
	-DG_LOG_DOMAIN=\"gnome-c-utils\"	\
	$(WARN_CFLAGS)

AM_LDFLAGS = $(WARN_LDFLAGS)

bin_PROGRAMS =					\
	gcu-case-converter			\
	gcu-check-chain-ups			\
	gcu-include-config-h			\
	gcu-lineup-parameters			\
	gcu-lineup-substitution			\
	gcu-multi-line-substitution		\
	gcu-smart-c-comment-substitution

gcu_case_converter_SOURCES =	\
	gcu-case-converter.c
gcu_case_converter_CFLAGS =	\
	$(GIO_DEPS_CFLAGS)
gcu_case_converter_LDADD =	\
	$(GIO_DEPS_LIBS)

gcu_check_chain_ups_SOURCES =	\
	gcu-check-chain-ups.c
gcu_check_chain_ups_CFLAGS =	\
	$(GIO_DEPS_CFLAGS)	\
	$(TEPL_DEPS_CFLAGS)
gcu_check_chain_ups_LDADD =	\
	$(GIO_DEPS_LIBS)	\
	$(TEPL_DEPS_LIBS)

gcu_include_config_h_SOURCES =	\
	gcu-include-config-h.c
gcu_include_config_h_CFLAGS =	\
	$(GIO_DEPS_CFLAGS)	\
	$(TEPL_DEPS_CFLAGS)
gcu_include_config_h_LDADD =	\
	$(GIO_DEPS_LIBS)	\
	$(TEPL_DEPS_LIBS)

gcu_lineup_parameters_SOURCES =	\
	gcu-lineup-parameters.c
gcu_lineup_parameters_CFLAGS =	\
	$(GIO_DEPS_CFLAGS)
gcu_lineup_parameters_LDADD =	\
	$(GIO_DEPS_LIBS)

gcu_lineup_substitution_SOURCES =	\
	gcu-lineup-substitution.c
gcu_lineup_substitution_CFLAGS =	\
	$(GIO_DEPS_CFLAGS)		\
	$(TEPL_DEPS_CFLAGS)
gcu_lineup_substitution_LDADD =		\
	$(GIO_DEPS_LIBS)		\
	$(TEPL_DEPS_LIBS)

gcu_multi_line_substitution_SOURCES =	\
	gcu-multi-line-substitution.c
gcu_multi_line_substitution_CFLAGS =	\
	$(GIO_DEPS_CFLAGS)		\
	$(TEPL_DEPS_CFLAGS)
gcu_multi_line_substitution_LDADD =	\
	$(GIO_DEPS_LIBS)		\
	$(TEPL_DEPS_LIBS)

gcu_smart_c_comment_substitution_SOURCES =	\
	gcu-smart-c-comment-substitution.c
gcu_smart_c_comment_substitution_CFLAGS =	\
	$(GIO_DEPS_CFLAGS)			\
	$(TEPL_DEPS_CFLAGS)
gcu_smart_c_comment_substitution_LDADD =	\
	$(GIO_DEPS_LIBS)			\
	$(TEPL_DEPS_LIBS)

-include $(top_srcdir)/git.mk
