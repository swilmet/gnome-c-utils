/* Indentation with spaces */
void
gtk_text_buffer_insert_at_cursor (GtkTextBuffer *buffer,
                                  const gchar   *text,
                                  gint           len)
{
  GtkTextIter iter;

  g_return_if_fail (GTK_IS_TEXT_BUFFER (buffer));
  g_return_if_fail (text != NULL);

  gtk_text_buffer_get_iter_at_mark (buffer,
                                    &iter,
                                    gtk_text_buffer_get_insert (buffer));

  gtk_text_buffer_insert (buffer, &iter, text, len);
}

static gboolean
gtk_source_view_extend_selection (GtkTextView            *text_view,
                                  GtkTextExtendSelection  granularity,
                                  const GtkTextIter      *location,
                                  GtkTextIter            *start,
                                  GtkTextIter            *end)
{
  if (granularity == GTK_TEXT_EXTEND_SELECTION_WORD)
  {
    _gtk_source_iter_extend_selection_word (location, start, end);
    return GDK_EVENT_STOP;
  }

  /* Alignment on second opening parenthesis. */
  return GTK_TEXT_VIEW_CLASS (gtk_source_view_parent_class)->extend_selection (text_view,
                                                                               granularity,
                                                                               location,
                                                                               start,
                                                                               end);
}

static void
gtk_other_example (void)
{
  gtk_function_call (foo (param1,
                          param2,
                          param3),
                     will_this_parameter_be_correctly_aligned);

  gtk_function_call (param0,
                     foo (param1,
                          param2,
                          param3),
                     will_this_parameter_be_correctly_aligned);

  gtk_function_call (param0,
                     gtk_foo (param1,
                              param2,
                              param3),
                     will_this_parameter_be_correctly_aligned);
}

/* Indentation with tabs */
void
gtk_text_buffer_insert_at_cursor (GtkTextBuffer *buffer,
				  const gchar   *text,
				  gint           len)
{
	GtkTextIter iter;

	g_return_if_fail (GTK_IS_TEXT_BUFFER (buffer));
	g_return_if_fail (text != NULL);

	gtk_text_buffer_get_iter_at_mark (buffer,
					  &iter,
					  gtk_text_buffer_get_insert (buffer));

	gtk_text_buffer_insert (buffer, &iter, text, len);
}

static gboolean
gtk_source_view_extend_selection (GtkTextView            *text_view,
				  GtkTextExtendSelection  granularity,
				  const GtkTextIter      *location,
				  GtkTextIter            *start,
				  GtkTextIter            *end)
{
	if (granularity == GTK_TEXT_EXTEND_SELECTION_WORD)
	{
		_gtk_source_iter_extend_selection_word (location, start, end);
		return GDK_EVENT_STOP;
	}

        /* Alignment on second opening parenthesis. */
	return GTK_TEXT_VIEW_CLASS (gtk_source_view_parent_class)->extend_selection (text_view,
										     granularity,
										     location,
										     start,
										     end);
}

static void
gtk_other_example (void)
{
	gtk_function_call (foo (param1,
				param2,
				param3),
			   will_this_parameter_be_correctly_aligned);

	gtk_function_call (param0,
			   foo (param1,
				param2,
				param3),
			   will_this_parameter_be_correctly_aligned);

	gtk_function_call (param0,
			   gtk_foo (param1,
				    param2,
				    param3),
			   will_this_parameter_be_correctly_aligned);
}
