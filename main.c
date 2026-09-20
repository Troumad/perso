#include "include.h"
#include <locale.h>

const char Lettre1_caract[6]={'F','I','S','D','C','c'};
const char * Nom_Caract_Sans_Accent[6]={"Force","Intelligence","Sagesse","Dexterite","Constitution","Charisme"};
void command_line (GtkApplication *app, GApplicationCommandLine *cmdline,gpointer param);

void cb_save (GtkMenuItem *menuitem, struct widgets *widgets);
gboolean cb_quit (GtkWidget *widget, GdkEvent  *event, struct widgets *widgets);

int main(int argc, char **argv)
{
  GError *error = NULL;
  struct widgets widgets;
  GtkCssProvider *css_provider = NULL;
  GdkDisplay *display=NULL;
  GdkScreen *screen=NULL;
  gchar *chemin=NULL, * ici=NULL;
  unsigned short i;
  GApplication *app=NULL;
  gboolean vrai;

  gtk_init(&argc, &argv);
  srand((unsigned int)time(0)); /* initialisation de l'aléatoire */
  ici=chemin_exe();
  setlocale(LC_NUMERIC,"C"); /* imposer le . comme séparateur décimal */
  if (g_application_id_is_valid ("org.troumad.perso"))
  {
    // test normalement inutile sur la validité du nom */
      app = g_application_new ("org.troumad.perso",
                               G_APPLICATION_HANDLES_COMMAND_LINE);
      g_application_register(G_APPLICATION(app),NULL,NULL);
      vrai=g_application_get_is_remote(G_APPLICATION(app));

      if (vrai)
      {
        char ** argv_bis;

        chemin=(char *)malloc(LONG*sizeof(char));
        argv_bis=(char**)malloc((argc+1)*sizeof(char*));
        argv_bis[argc]=NULL;
        getcwd(chemin,LONG-1);
        argv_bis[0]=chemin; /* si ouverture en ligne de commande, envoyer aussi l'emplacement */
        for(i=1;i<argc;i++)
        {
                argv_bis[i]=argv[i];
        }
        g_application_set_inactivity_timeout (G_APPLICATION(app), 10);
        g_application_run (G_APPLICATION(app), argc, argv_bis); /* attend un retour de l'application mère si elle existe */
        g_object_unref (app);
        g_free(chemin);
        g_free(argv_bis);
      }
      else
      {
          g_signal_connect (app, "command-line", G_CALLBACK (command_line), NULL);

          envp_init();
          lecture_fichiers(ici);
          widgets.builder = gtk_builder_new();
          chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","glade","menu_perso.glade",NULL);
          if (!gtk_builder_add_from_file(widgets.builder, chemin , &error))
          {
              g_warning("%s", error->message);
              g_error_free(error);
              exit(EXIT_FAILURE);
          }
          g_free(chemin);

          css_provider= gtk_css_provider_new();
          chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","mongtk.css",NULL);
          display = gdk_display_get_default();
          screen = gdk_display_get_default_screen(display);
          gtk_style_context_add_provider_for_screen(screen,
                                  GTK_STYLE_PROVIDER(css_provider),
                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

          gtk_css_provider_load_from_path (css_provider,chemin,&error);
          if (error!=NULL)
          {
              g_warning("%s", error->message);
              g_error_free(error);
              exit(EXIT_FAILURE);
          }
          else
          {       /* ouverture OK du css */
          }
          g_free(chemin);

          get_widgets(&widgets);
          gtk_builder_connect_signals(widgets.builder, &widgets);

          gtk_widget_show(GTK_WIDGET(widgets.window));
          for(i=1;i<argc;i++)
          {
            ouvre_perso(argv[i]);
          }

          init_race(&widgets);
          gtk_main();

          g_object_unref(G_OBJECT(widgets.builder));

      }
  }
  else
  {
      printf("identifiant non valide\n");
  }

  exit(EXIT_SUCCESS);
}

gboolean cb_quit (GtkWidget *widget, GdkEvent  *event, struct widgets *widgets)
{
  FeuilleDeChoix(NULL,NULL);
  gtk_main_quit();


    (void)widget;
    (void)widgets;
    (void)event;

  return TRUE;
}

void command_line (GtkApplication *app, GApplicationCommandLine *cmdline,gpointer param)
{ /* retour appel si application déjà ouverte */
  gchar **argv;
  gint argc;
  unsigned short i,lo;
  char chemin[LONG];

  argv = g_application_command_line_get_arguments (cmdline, &argc);
  for(i=1;i<argc;i++)
  {
    if (ouvre_perso(argv[i])==NULL)
    {   /* ouverture ligne de commande => on prend en compte l'emplacement de la commande */
        g_stpcpy(chemin,argv[0]);
        lo=strlen(chemin);
        strcpy(chemin+lo+1,argv[i]);
        chemin[lo]=SEPARATEUR;
        ouvre_perso(chemin);
    }
    else
    {
        /* l'ouverture c'est bien passée */
    }
  }
  g_strfreev (argv);

  (void)app;
  (void)param;
}
