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
        //g_free(widgets);
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
    gtk_widget_destroy(GTK_WIDGET(widgets->window)); /* vide widgets-> builder et le met à NULL */

    (void)ChildWidget;
}
