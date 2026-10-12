#include "include.h"

void get_widgets(struct widgets *widgets)
{
  widgets->window = GTK_WIDGET(gtk_builder_get_object(widgets->builder,"window"));
}

void ouverture_glade(char * _nom, unsigned short connect)
{
    struct widgets * widgets=NULL;

    widgets=ouverture_glade_retour(_nom,connect);
    if (widgets!=NULL)
    {
        /* la structure est libérée à la destruction de la fenêtre,
           pas ici : les handlers du glade l'utilisent comme user_data */
        g_object_set_data_full(G_OBJECT(widgets->window),"widgets_glade",widgets,libere_widgets);
    }
}

struct widgets * ouverture_glade_retour(char * _nom, unsigned short connect)
{
  GError *error = NULL;
  struct widgets * widgets=NULL;
  char *chemin, * ici=chemin_exe();

  widgets=(struct widgets *)g_malloc(sizeof(struct widgets));

  widgets->builder = gtk_builder_new();
  chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","glade",_nom,NULL);
  if (!gtk_builder_add_from_file(widgets->builder, chemin, &error))
  {
      g_warning("%s", error->message);
      g_error_free(error);
      printf("Fichier %s inexistant à l'emplacement %s\n",_nom,chemin);
      widgets=NULL;
  }
  else
  {
      get_widgets(widgets);
      if ((connect&GLADE_CONNECT)!=0)
      {
          gtk_builder_connect_signals(widgets->builder, widgets);
      }

      gtk_widget_show(GTK_WIDGET(widgets->window));
  }
  g_free(chemin);
  g_free(ici);

  return widgets;
}

void ferme_glade(GtkWidget *ChildWidget , struct widgets * widgets)
{
    gtk_widget_hide(GTK_WIDGET(widgets->window));
    g_signal_handlers_disconnect_matched(widgets->window,G_SIGNAL_MATCH_DATA,0,0,NULL,NULL,widgets);
    gtk_widget_destroy(GTK_WIDGET(widgets->window));

    (void)ChildWidget;
}

void libere_widgets(gpointer data)
{
    struct widgets * w=(struct widgets *)data;
    g_object_unref(w->builder);
    g_free(w);
}
