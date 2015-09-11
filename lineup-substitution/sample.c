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
