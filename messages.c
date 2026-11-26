/*
    Génération de feuille de personnage complète AD&D 1 et 2
    Génération aléatoire de personnage

    Copyright (C) 2011-2026 Bernard SIAUD

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    a1024 with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "include.h"

void GPL (GtkMenuItem *menuitem, struct widgets * _widgets)
{
    ouverture_glade("GPL.glade",1);

    (void)menuitem;
    (void)_widgets;
}

void Explication  (GtkMenuItem *menuitem, struct widgets * _widgets)
{
    ouverture_glade("Explication.glade",1);

    (void)menuitem;
    (void)_widgets;
}

void traite_erreur(GError *error)
{
 printf("%s\n",error->message);                                      /* affichage de l'erreur                                       */
}

unsigned short demande_oui_non(char * message)
{
  GtkWidget *p_dialog = NULL;
  unsigned short retour;

  p_dialog = gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",message);
  switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
  {
  case GTK_RESPONSE_NO:
      gtk_widget_destroy(p_dialog);
      retour=0;
      break;
  case GTK_RESPONSE_YES:
      retour=1;
      break;
  default :
      retour=-1;
      break;
  }

  gtk_widget_destroy(p_dialog);

  return retour;
}

void donne_info(char * message)
{
  GtkWidget *p_dialog = NULL;

  p_dialog = gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_OK,"%s",message);
  gtk_dialog_run(GTK_DIALOG(p_dialog));

  gtk_widget_destroy(p_dialog);
}

void dialogue(char * ch, signed short i)
{
    GtkWidget * diag;

    diag=gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_CLOSE,ch,i);
    gtk_dialog_run(GTK_DIALOG(diag));
    gtk_widget_destroy(diag);
}
