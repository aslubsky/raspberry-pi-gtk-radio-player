#include <gtk/gtk.h>


GtkWidget *currButton;
GtkWidget *label;
GtkWidget *window;

char* labels[] = { "Громадське", "Промінь","Перший","Культура" };
int stationNumber = 0;

static void
run_audio_player ()
{
    gchar *cmdStr;
    switch(stationNumber) {
        case 0:
            cmdStr = "omxplayer http://91.218.212.67:8000/stream";
        break;
        case 1:
            cmdStr = "omxplayer https://radio.nrcu.gov.ua:8443/ur2-mp3";
        break;
        case 2:
            cmdStr = "omxplayer https://radio.nrcu.gov.ua:8443/ur1-mp3";
        break;
        case 3:
            cmdStr = "omxplayer https://radio.nrcu.gov.ua:8443/ur3-mp3";
        break;
    }
    
    gboolean res0 = g_spawn_command_line_sync ("killall -9 omxplayer.bin", NULL, NULL, NULL, NULL);
    gboolean res = g_spawn_command_line_async (cmdStr, NULL);
}


static void poweroff_click (GtkWidget *widget,
             gpointer   data)
{
  gboolean res = g_spawn_command_line_async ("poweroff", NULL);
  gtk_widget_destroy(window);
}

static void
run_audio_player_click (GtkWidget *widget,
             gpointer   data)
{
  if(stationNumber == data) {
    return;
  }

  stationNumber = data;
  GtkStyleContext *context;
  if(currButton != NULL) {
    context = gtk_widget_get_style_context (currButton);
    gtk_style_context_remove_class (context, "selected");
  }
  currButton = widget;
  context = gtk_widget_get_style_context (widget);
  gtk_style_context_add_class (context, "selected");


  char *markup;
  markup = g_markup_printf_escaped ("<span size=\"xx-large\">%s</span>", labels[stationNumber]);
  gtk_label_set_markup (GTK_LABEL(label), markup);

  run_audio_player();
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *button1;
  GtkWidget *button2;
  GtkWidget *button3;
  GtkWidget *button4;
  GtkWidget *button5;
  GtkGrid *grid;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Radio");
  gtk_window_set_default_size (GTK_WINDOW (window), 180, 250);

  grid = gtk_grid_new();

  gtk_container_set_border_width (GTK_CONTAINER (grid), 20);
  gtk_container_add (GTK_CONTAINER (window), grid);

  label = gtk_label_new ("");
  char *markup;
  markup = g_markup_printf_escaped ("<span size=\"xx-large\">%s</span>", labels[0]);
  gtk_label_set_markup (GTK_LABEL(label), markup);

  button1 = gtk_button_new_with_label (labels[0]);
  g_signal_connect (button1, "clicked", G_CALLBACK (run_audio_player_click), 0);
  currButton = button1;
  GtkStyleContext *context;
  context = gtk_widget_get_style_context (button1);
  gtk_style_context_add_class (context, "selected");

  GtkCssProvider *provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_data (provider, ".selected { background: #ffff00}", -1, NULL);

  button2 = gtk_button_new_with_label (labels[1]);
  g_signal_connect (button2, "clicked", G_CALLBACK (run_audio_player_click), 1);

  button3 = gtk_button_new_with_label (labels[2]);
  g_signal_connect (button3, "clicked", G_CALLBACK (run_audio_player_click), 2);

  button4 = gtk_button_new_with_label (labels[3]);
  g_signal_connect (button4, "clicked", G_CALLBACK (run_audio_player_click), 3);

  button5 = gtk_button_new_with_label ("Вимкнути");
  g_signal_connect (button5, "clicked", G_CALLBACK (poweroff_click), NULL);


  gtk_style_context_add_provider (gtk_widget_get_style_context (button1), GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider (gtk_widget_get_style_context (button2), GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider (gtk_widget_get_style_context (button3), GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider (gtk_widget_get_style_context (button4), GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_style_context_add_provider (gtk_widget_get_style_context (button5), GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);


  gtk_grid_set_row_spacing(GTK_GRID (grid), 50);
  gtk_grid_set_column_spacing(GTK_GRID (grid), 50);


  gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button5, 1, 0, 1, 1);

  gtk_grid_attach (GTK_GRID (grid), button1, 0, 1, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button2, 1, 1, 1, 1);

  gtk_grid_attach (GTK_GRID (grid), button3, 0, 2, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), button4, 1, 2, 1, 1);

  gtk_widget_show_all (window);

  stationNumber = 0;
  run_audio_player();
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("com.davintoo.aslubsky.radio", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}