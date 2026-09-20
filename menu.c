#include "include.h"
#include <string.h>

#define NB_LIGNE 15

void coche_FeuilleDeChoix( struct widgets * widgets);
void change_choix_pref (GtkComboBox *widget, struct widgets * widgets);
void change_choix_niv (GtkComboBox *widget, struct widgets * widgets);
void change_choix_tirage (GtkRadioButton *widget, struct widgets * widgets);
void change_choix_pdv(GtkRadioButton *widget, struct widgets * widgets);
void change_choix_version(GtkRadioButton *widget, struct widgets * widgets);
void Change_Choix_Classe (GtkComboBox *widget, struct widgets * widgets);
void Change_Choix_Race (GtkComboBox *widget, struct widgets * widgets);
void Change_Tirage (GtkMenuItem *menuitem, struct widgets * widgets);
void Change_Sexe (GtkMenuItem *menuitem, struct widgets * widgets);
void Change_Pdv (GtkMenuItem *menuitem, struct widgets * widgets);
void Change_Race (GtkMenuItem *menuitem, struct widgets * widgets);
void Change_Classe (GtkMenuItem *menuitem, struct widgets * widgets);
void Change_Version (GtkMenuItem *menuitem, struct widgets * widgets);
void quite_choix(GtkWidget *widget, struct widgets * widgets);
void change_choix_sexe(GtkRadioButton *widget, struct widgets * widgets);
void Change_Niv(GtkMenuItem *menuitem, struct widgets * widgets);
void Aff_Fichiers_Recents(GtkMenuItem *menuitem, struct widgets * widgets);
signed short trouve_version(struct widgets * widgets);



void init_race(struct widgets * widgets)
{
    GtkMenuItem * menu_race;
    signed short j;
    GtkWidget *pMenu ;
    GList * gl, *gl1;

    menu_race = GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"Race"));
    if (menu_race==NULL)
    {
          dialogue("Entrée Race non trouvée\n",0);
    }
    else
    {
      pMenu=gtk_menu_item_get_submenu(GTK_MENU_ITEM(menu_race));                                      /* on trouve le sous-menu                             */
      if (pMenu==NULL)
      {
        dialogue("Erreur : menu absent\n",0);
      }
      else
      {
          gl1=gtk_container_get_children(GTK_CONTAINER(pMenu));
          for (gl=gl1,j=0; gl != NULL; gl= g_list_next(gl))
          {
              if (compare_sans_casse(RACE[j].nom,"humain")==0)
              {
                  gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(gl->data),TRUE);
              }
              else
              {
                  /* pas la classe humain */
              }
              j++;
          }
          g_list_free(gl1);
      }
    }
}

signed short trouve_version(struct widgets * widgets)
{
    unsigned short version;

    if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(gtk_builder_get_object(widgets->builder,"ADD"))))
    {
        version=ADD;
    }
    else
    {
        version=ADD2;
    }    /* tester la compatibilité de la version et de la race */

    return version;
}



void Change_Pref (GtkMenuItem *menuitem, struct widgets * widgets)
{
    static unsigned short drap=0; /* éviter plusieurs traitements en // */
    static GtkWidget *** Pref=NULL;
    const char * nom_case; /* nom de la case qui vient d'être changée */
    unsigned short i,j,ligne_a_changer=0,case_changee,ligne_changee=0;
    char ch[16];
    unsigned short v_case[7]={7,7,7,7,7,7,7};


    if (drap==0)
    {
        drap=1;

        if (Pref==NULL) /* initialisation du tableau au premier appel */
        {
            ch[2]=0;
            Pref=(GtkWidget ***)g_malloc(6*sizeof(GtkWidget **));
            for (i=0;i<6;i++)
            {
                ch[0]=Lettre1_caract[i];
                Pref[i]=(GtkWidget **)g_malloc(7*sizeof(GtkWidget *));
                Pref[i][0]=GTK_WIDGET((gtk_builder_get_object(widgets->builder,Nom_Caract_Sans_Accent[i])));
                if (Pref[i][0]==NULL)
                {
                    printf("L'entrée %s n'existe pas pour i=%hu\n",Nom_Caract_Sans_Accent[i],i);
                }
                for (j=1;j<7;j++)
                {
                    ch[1]=j+'0';
                    Pref[i][j]=GTK_WIDGET((gtk_builder_get_object(widgets->builder,ch)));
                    if (Pref[i][j]==NULL)
                    {
                        printf("L'entrée %s n'existe pas\n",ch);
                    }
                }
            }
        }
        else
        {
            /* le tableau appekl était déjà initialisé : ce n'était pas le premier appel */
        }

        nom_case=gtk_widget_get_name (GTK_WIDGET(menuitem));
        for (ligne_changee=0;ligne_changee<5 && nom_case[0]!=Lettre1_caract[ligne_changee];ligne_changee++); /* cherche le rang choisi */
                                                                                                            /* le paramêtre Lettre1_caract[i] a été modifié : première lettre du nom */
        case_changee=nom_case[1]-'0'; /* ne marche pas car on reçoit aussi la case qui a été désactivée => d'où le if suivant */
        /*for (case_changee=1;gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(Pref[ligne_changee][case_changee]))==FALSE && case_changee<7;case_changee++); *//* Pref[i][j] est actif */
        if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(Pref[ligne_changee][case_changee]))==TRUE)
        {
            ligne_a_changer=8;
            for (i=0;i<6;i++)
            {
                for(j=1;j<7;j++)
                {
                    if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(Pref[i][j]))==TRUE)
                    {
                        if (i!=ligne_changee && j==case_changee) /* on a trouvé la ligne à changer */
                        {
                            ligne_a_changer=i;
                        }
                        else
                        {
                            v_case[j]=i; /* on mémorise toutes les lignes */
                        }                /* pour dire que le choix j est pris */
                    }
                }
            }
            if (ligne_a_changer!=8)
            {
                for (j=1;j<7;j++)
                {
                    if (v_case[j]==7) /* on vient de trouver la valeur qui est nulle part => à mettre dans la ligne à changer */
                    {
                        drap=2; /* dire qu'un changement a été fait => comme ça, on ne repassera pas par les tests */
                        sprintf(ch,"%hu %s",j,nom[ligne_a_changer]); /* on refait le label de la ccaractéristique pour y include le nouveau rang */
                        gtk_menu_item_set_label(GTK_MENU_ITEM(Pref[ligne_a_changer][0]),ch); /* mise en place du label */
                        /* modification de l'état le plus pard possible */
                        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(Pref[ligne_a_changer][j]),TRUE); /* activation du bon bouton radio */
                        j=8; /* sortie de la boucle for : on a trouvé ce qu'on cherchait */
                    }
                }
                sprintf(ch,"%hu %s",case_changee,nom[ligne_changee]);
                gtk_menu_item_set_label(GTK_MENU_ITEM(Pref[ligne_changee][0]),ch);
            }
            else
            {
                /* rien n'était sélectionné : problème ! */
            }
        }
        else
        {
            /* on est dans le cas : l'item qu'on reçoit est l'itel déselectionné */
        }

        if (drap==1)
        {
            drap=0;
            FeuilleDeChoix(NULL,NULL);
        }  /* traitement fini : on peut refaire des traitements */
        else
        {
            /* drap doit être à 2 pour indiquer qu'on a fait une autre modification => c'est au test de ce 2 qu'on passera drap à 0 */
        }
    }
    else if (drap==2)
    {
        drap=0; /* un changement a été généré pour tout remettre d'aplomb : pas besoin de retester. Mais, on laisse la main libre aux changements suivants */
    }
    else
    {
        /* drap doit être à 1 : on est déjà en train de travailler sur le système */
    }
}

void complete_menu (GtkMenuItem *menuitem, struct widgets *widgets)
{
  GtkMenuItem *menuaffichage;
  GtkWidget *pMenu ;
  GtkWidget *e_menu ;
  unsigned short i;
  static unsigned short passage=0;
  GSList * e_menu0;
  char ch[16];


  if (passage==0)
  {
      passage=1;
      menuaffichage = GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"Classe"));
      if (menuaffichage==NULL)
      {
          printf("Entrée classe non trouvée\n");
      }
      else
      {
          pMenu=gtk_menu_item_get_submenu(GTK_MENU_ITEM(menuaffichage));                                      /* on trouve le sous-menu                             */
          if (pMenu==NULL)
          {
              pMenu=gtk_menu_new ();
          }
          else
          {
          }
          for (i=0;i<NB_CLASSE;i++)
          {
              e_menu =gtk_check_menu_item_new_with_mnemonic(CLASSE[i].nom);
              gtk_menu_item_set_label(GTK_MENU_ITEM(e_menu),CLASSE[i].nom);
              sprintf(ch,"classe_%hu",i);
              gtk_widget_set_name(e_menu,ch);
              gtk_menu_shell_insert(GTK_MENU_SHELL(pMenu),e_menu,i);
              if (e_menu==NULL) printf("Erreur !");
              gtk_menu_item_set_submenu (menuaffichage, pMenu);
              if (compare_sans_casse(CLASSE[i].nom,"guerrier")==0)
              { /* on met guerrier par défaut */
                  gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(e_menu),TRUE);
              }
              g_signal_connect(G_OBJECT(e_menu), "draw", G_CALLBACK(style_classe), widgets);
              g_signal_connect(G_OBJECT(e_menu), "toggled", G_CALLBACK(Change_Classe), widgets);
          }
          gtk_widget_show_all(pMenu);
      }

      menuaffichage = GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"Race"));
      if (menuaffichage==NULL)
      {
          printf("Entrée Race non trouvée\n");
      }
      else
      {
          pMenu=gtk_menu_item_get_submenu(GTK_MENU_ITEM(menuaffichage));                                      /* on trouve le sous-menu                             */
          if (pMenu==NULL)
          {
              pMenu=gtk_menu_new ();
          }
          else
          {
          }
          for (i=0;i<NB_RACE;i++)
          {
              if (i==0)
              {
                  e_menu =gtk_radio_menu_item_new_with_mnemonic(NULL,RACE[0].nom);
                  e_menu0=gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM(e_menu));
              }
              else
              {
                  e_menu =gtk_radio_menu_item_new_with_mnemonic((GSList *)e_menu0,RACE[i].nom);
              }
              sprintf(ch,"race_%hu",i);
              gtk_widget_set_name(e_menu,ch);
              gtk_menu_item_set_label(GTK_MENU_ITEM(e_menu),RACE[i].nom);
              gtk_menu_shell_insert(GTK_MENU_SHELL(pMenu),e_menu,i);
              if (e_menu==NULL) printf("Erreur !");
              gtk_menu_item_set_submenu (menuaffichage, pMenu);
              g_signal_connect(G_OBJECT(e_menu), "draw", G_CALLBACK(style_race), widgets);
              g_signal_connect(G_OBJECT(e_menu), "toggled", G_CALLBACK(Change_Race), widgets);
          }
          gtk_widget_show_all(pMenu);
      }

  }
  else
  {
      /*printf("Complete_menu : passage n°%hu\n",passage);*/
  }

  (void)menuitem;
}

signed short trouve_race(struct widgets *widgets)
{
    GtkMenuItem * menu_race;
    signed short nb_race=-1,j;
    GtkWidget *pMenu ;
    GList * gl, *gl1;

    menu_race = GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"Race"));
    if (menu_race==NULL)
    {
          dialogue("Entrée Race non trouvée\n",0);
    }
    else
    {
      pMenu=gtk_menu_item_get_submenu(GTK_MENU_ITEM(menu_race));                                      /* on trouve le sous-menu                             */
      if (pMenu==NULL)
      {
        dialogue("Erreur : menu absent\n",0);
      }
      else
      {
          gl1=gtk_container_get_children(GTK_CONTAINER(pMenu));
          for (gl=gl1,j=0; gl != NULL; gl= g_list_next(gl))
          {
              if (j<NB_RACE && gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(gl->data))==TRUE)
              {
                  nb_race=j;
              }
              else
              {
                  /* pas le bon */
              }
              j++;
          }
          g_list_free(gl1);
      }
    }

    return nb_race;
}

signed short * trouve_classe(struct widgets *widgets) /* -1 pour marquer le dernier élément */
{
    GtkMenuItem * menu_classe=NULL;
    signed short j,n=0;
    signed short * nb_classe=(signed short *)g_malloc(sizeof(signed short));
    GtkWidget *pMenu=NULL ;
    GList * gl=NULL, *gl1=NULL;

    nb_classe[0]=-1;
    menu_classe = GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"Classe"));
    if (menu_classe==NULL)
    {
          printf("Entrée Classe non trouvée\n");
    }
    else
    {
      pMenu=gtk_menu_item_get_submenu(GTK_MENU_ITEM(menu_classe));                                      /* on trouve le sous-menu                             */
      if (pMenu==NULL)
      {
          printf("Erreur : menu absent\n");
      }
      else
      {
          gl1=gtk_container_get_children(GTK_CONTAINER(pMenu));
          for (gl=gl1,j=0; gl != NULL; gl= g_list_next(gl))
          {
              if (j<NB_CLASSE && gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(gl->data))==TRUE)
              {
                  nb_classe=(signed short *)g_realloc(nb_classe,(n+2)*sizeof(signed short));
                  nb_classe[n]=j;
                  n++;
              }
              else
              {
                  /* pas la bonne entrée */
              }
              j++;
          }
          nb_classe[n]=-1; /* on marque la fin de la chaîne */
          g_list_free(gl1);
      }
    }

    return nb_classe;
}


void FeuilleDeChoix(GtkMenuItem *menuitem, struct widgets * _widgets)
{
    static struct widgets * widgets=NULL;
    GtkWidget * boite=NULL, *coche=NULL , * coche0=NULL, * colonne=NULL;
    static unsigned short drap=0; /* 0 : pas de modif, 1 on peut modifier, 2 on doit attendre pour modifier, 3 on attend déjà pour modifier */
    unsigned short i,j,c,l;
    char ch[16];



    if (menuitem==NULL) /* appel lors d'une modification du menu ou pour la fermeture si de plus _widgets==NULL */
    {     /* fermeture de la fenêtre */
        if (_widgets!=NULL)
        {
            gtk_widget_destroy(GTK_WIDGET(widgets->window));
            g_free(widgets);
            widgets=NULL;
            coche_FeuilleDeChoix(NULL);
        }
        else if (widgets!=NULL && widgets->builder!=NULL) /* la fenêtre est ouverte */
        {
            if (drap==1 || drap==2)
            {
                if (drap!=1)
                {
                    drap=3;
                    while (drap!=1); /* on attend le feu vert pour modifier */
                }
                drap=2;
                coche_FeuilleDeChoix(widgets);
                drap=1;
            }
        }
        else
        {
                /* je ne sais pourquoi on passe là ! */
        }

    }
    else
    {
        if (widgets==NULL || widgets->builder==NULL) /* pour ne pas ouvrir la fenêtre deux fois */
        {                                            /* widgets->builder est mis automatiquement à NULL quand on ferme la fenêtre */
            drap=0;
            widgets=ouverture_glade_retour("ChoixTirage.glade",0);
            if (widgets!=NULL)
            {
                widgets=(struct widgets *)g_realloc(widgets,2*sizeof(struct widgets));
                widgets[1].builder=_widgets->builder;
                widgets[1].window=_widgets->window;
                gtk_builder_connect_signals(widgets->builder, widgets);
                /* gestion des races */
                /* l : nb de lignes pour l'affichage et c : nb de colonnes */
                if (NB_RACE%NB_LIGNE==0)
                {
                    c=NB_RACE/NB_LIGNE;
                    l=NB_LIGNE;
                }
                else
                {
                    c=NB_RACE/NB_LIGNE+1;
                    l=NB_RACE/c;
                    if (NB_RACE%c!=0)
                    {
                        l++;
                    }
                }
                boite=GTK_WIDGET( gtk_builder_get_object(widgets->builder,"box_race"));
                for (i=0;i<c;i++)
                {
                    colonne=gtk_box_new (GTK_ORIENTATION_VERTICAL,2);
                    gtk_box_pack_start (GTK_BOX(boite),GTK_WIDGET(colonne),TRUE,TRUE,2);
                    for (j=0;j<l;j++)
                    {
                        if (i+j==0)
                        {
                            coche0=gtk_radio_button_new_with_label (NULL,RACE[0].nom);
                            g_signal_connect(G_OBJECT(coche0), "clicked", G_CALLBACK(Change_Choix_Race), _widgets);
                            gtk_box_pack_start (GTK_BOX(colonne),coche0,FALSE,TRUE,2);
                            gtk_widget_set_name (GTK_WIDGET(coche0),"race_0");
                        }
                        else if (j+i*l<NB_RACE) /* prévoir les derniers cas où la case peut être vide */
                        {
                            coche=gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(coche0),RACE[j+i*l].nom);
                            g_signal_connect(G_OBJECT(coche), "clicked", G_CALLBACK(Change_Choix_Race), _widgets);
                            gtk_box_pack_start (GTK_BOX(colonne),coche,TRUE,TRUE,2);
                            sprintf(ch,"race_%hu",j+i*l);
                            gtk_widget_set_name (GTK_WIDGET(coche),ch);
                        }
                    }
                }
                gtk_widget_show_all(boite);


                /* gestion des classes */
                /* l : nb de lignes pour l'affichage et c : nb de colonnes */
                if (NB_CLASSE%NB_LIGNE==0)
                {
                    c=NB_CLASSE/NB_LIGNE;
                    l=NB_LIGNE;
                }
                else
                {
                    c=NB_CLASSE/NB_LIGNE+1;
                    l=NB_CLASSE/c;
                    if (NB_CLASSE%c!=0)
                    {
                        l++;
                    }
                }
                boite=GTK_WIDGET( gtk_builder_get_object(widgets->builder,"box_classe"));
                for (i=0;i<c;i++)
                {
                    colonne=gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
                    gtk_box_pack_start(GTK_BOX(boite),GTK_WIDGET(colonne),TRUE,TRUE,2);
                    for (j=0;j<l;j++)
                    {
                        if (j+i*l<NB_CLASSE) /* prévoir les derniers cas où la case peut être vide */
                        {
                            coche=gtk_check_button_new_with_label(CLASSE[j+i*l].nom);
                            g_signal_connect(G_OBJECT(coche), "toggled", G_CALLBACK(Change_Choix_Classe), _widgets);
                            gtk_box_pack_start (GTK_BOX(colonne),coche,TRUE,TRUE,2);
                            sprintf(ch,"classe_%hu",j+i*l);
                            gtk_widget_set_name (GTK_WIDGET(coche),ch);
                        }
                    }
                }
                gtk_widget_show_all(boite);
                coche_FeuilleDeChoix(widgets);
                drap=1;
            }
            else
            {
                printf("erreur d'ouverture de ChoixTirage.glade\n");
            }
        }
        else
        {
            gtk_window_present(GTK_WINDOW(widgets->window));
        }
    }

}

void quite_choix(GtkWidget *widget, struct widgets * widgets)
{
    FeuilleDeChoix(NULL,(struct widgets *)1);


    (void)widget;
    (void)widgets;
}


void coche_FeuilleDeChoix( struct widgets * widgets)
{
    static GtkWidget *** Pref=NULL;
    unsigned short i,j;
    gboolean coch;
    char ch[128];
    GtkWidget * wid;
    static struct widgets * tmp_wid=NULL;
    signed short race, *classe=NULL,versi;

    if (widgets==NULL)
    {
        Pref=NULL;
        tmp_wid=NULL;
    }
    else
    {
        race=trouve_race(widgets+1);
        classe=trouve_classe(widgets+1);
        if (Pref==NULL && widgets==NULL) /* initialisation du tableau au premier appel */
        {
            printf("Erreur dans la fonction coche_FeuilleDeChoix : on y passe dans des conditions anormales\n");
        }
        else
        {
            if (Pref==NULL) /* initialisation du tableau au premier appel */
            {
                tmp_wid=widgets;
                ch[2]=0;
                Pref=(GtkWidget ***)g_malloc(6*sizeof(GtkWidget **));
                for (i=0;i<6;i++)
                {
                    ch[0]=Lettre1_caract[i];
                    Pref[i]=(GtkWidget **)g_malloc(7*sizeof(GtkWidget *)); /* 7 car indice de 1 à 6 */
                    Pref[i][0]=GTK_WIDGET((gtk_builder_get_object(widgets[1].builder,Nom_Caract_Sans_Accent[i])));
                    if (Pref[i][0]==NULL)
                    {
                        printf("L'entrée %s n'existe pas pour i=%hu\n",Nom_Caract_Sans_Accent[i],i);
                    }
                    for (j=1;j<7;j++)
                    {
                        ch[1]=j+'0';
                        Pref[i][j]=GTK_WIDGET((gtk_builder_get_object(widgets[1].builder,ch)));
                        if (Pref[i][j]==NULL)
                        {
                            printf("L'entrée %s n'existe pas\n",ch);
                        }
                    }
                }
            }

            for (i=0;i<6;i++) /* les préférences */
            {
                for(j=1;j<7;j++)
                {
                    if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(Pref[i][j]))==TRUE)
                    {
                        sprintf(ch,"CB_%c",Lettre1_caract[i]);
                        if (gtk_combo_box_get_active (GTK_COMBO_BOX(gtk_builder_get_object(widgets->builder,ch)))!=j-1)
                        {
                            gtk_combo_box_set_active (GTK_COMBO_BOX(gtk_builder_get_object(widgets->builder,ch)),j-1);
                        }
                        else
                        {
                            /* il était déjà activé */
                        }
                        j=7; /* accélérer la sortie de la boucle */
                    }
                }
            }

            for (i=0;Tirages[i][0]!=0;i++) /* les tirages */
            {
                if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(gtk_builder_get_object(tmp_wid[1].builder,Tirages[i]))))
                {
                    wid=GTK_WIDGET(gtk_builder_get_object(tmp_wid->builder,Tirages[i]));
                    if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wid)))
                    {
                        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wid),TRUE);
                    }
                    else
                    {
                        /* il était déjà activé */
                    }
                }


            }

            /* le sexe */
            if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(gtk_builder_get_object(tmp_wid[1].builder,"Feminin"))))
            {
                wid=GTK_WIDGET(gtk_builder_get_object(tmp_wid->builder,"Feminin"));
                if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wid)))
                {
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wid),TRUE);
                }
                else
                {
                    /* il était déjà activé */
                }
            }
            else if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(gtk_builder_get_object(tmp_wid[1].builder,"Masculin"))))
            {
                wid=GTK_WIDGET(gtk_builder_get_object(tmp_wid->builder,"Masculin"));
                if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wid)))
                {
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wid),TRUE);
                }
                else
                {
                    /* il était déjà activé */
                }
            }

            /* les pdv */
            if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(gtk_builder_get_object(tmp_wid[1].builder,"1D"))))
            {
                wid=GTK_WIDGET(gtk_builder_get_object(tmp_wid->builder,"1D"));
                if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wid)))
                {
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wid),TRUE);
                }
                else
                {
                    /* il était déjà activé */
                }
            }
            else if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(gtk_builder_get_object(tmp_wid[1].builder,"2D"))))
            {
                wid=GTK_WIDGET(gtk_builder_get_object(tmp_wid->builder,"2D"));
                if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wid)))
                {
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wid),TRUE);
                }
                else
                {
                    /* il était déjà activé */
                }
            }
            else if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(gtk_builder_get_object(tmp_wid[1].builder,"Dmax"))))
            {
                wid=GTK_WIDGET(gtk_builder_get_object(tmp_wid->builder,"Dmax"));
                if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wid)))
                {
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wid),TRUE);
                }
                else
                {
                    /* il était déjà activé */
                }
            }

            /* la version */
            if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(gtk_builder_get_object(tmp_wid[1].builder,"ADD"))))
            {
                versi=ADD;
                wid=GTK_WIDGET(gtk_builder_get_object(tmp_wid->builder,"ADD"));
                if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wid)))
                {
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wid),TRUE);
                }
                else
                {
                    /* il était déjà activé */
                }
            }
            else if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(gtk_builder_get_object(tmp_wid[1].builder,"ADD2"))))
            {
                versi=ADD2;
                wid=GTK_WIDGET(gtk_builder_get_object(tmp_wid->builder,"ADD2"));
                if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wid)))
                {
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wid),TRUE);
                }
                else
                {
                    /* il était déjà activé */
                }
            }
            else
            {
                versi=0;
                printf("Surprenant : aucune version sélectionnée !");
            }

            sprintf(ch,"Niveau");
            for (i=0;i<=20;i++) /* les niveaux */
            {
                sprintf(ch+6,"%hu",i);
                wid=GTK_WIDGET((gtk_builder_get_object(widgets[1].builder,ch)));
                if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(wid))==TRUE)
                {
                    if (gtk_combo_box_get_active (GTK_COMBO_BOX(gtk_builder_get_object(widgets->builder,"Niveau")))!=i)
                    {
                        gtk_combo_box_set_active (GTK_COMBO_BOX(gtk_builder_get_object(widgets->builder,"Niveau")),i);
                    }
                    else
                    {
                        /* il était déjà activé */
                    }
                    i=20; /* accélérer la sortie de la boucle */
                }
            }

            /* la race */
            sprintf(ch,"race_%hd",race);
            wid=gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(widgets->builder,"box_race")),ch);
            if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wid)))
            {
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wid),TRUE);
            }
            else
            {
                /* il était déjà activé */
            }
            for(i=0;i<NB_RACE;i++)
            {
                sprintf(ch,"race_%hd",i);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(widgets->builder,"box_race")),ch);
                if (RACE[i].add[versi].nom!=NULL)
                {
                    /*gtk_widget_set_opacity(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(widgets->builder,"box_race")),ch),1);*/
                    gtk_widget_set_sensitive(wid,TRUE);
                    couleur_item(wid,GRIS);

                }
                else
                {
                    gtk_widget_set_sensitive(wid,FALSE);
                    couleur_item(wid,ROUGE);
                }

            }

            /* la classe */
            for (j=0;j<NB_CLASSE;j++)
            {
                coch=FALSE;
                for (i=0;classe[i]!=-1;i++)
                {
                    if (classe[i]==j)
                    {
                        coch=TRUE;
                    }
                    else
                    {
                        /* pas celle-là qui est cochée */
                    }
                }
                sprintf(ch,"classe_%hd",j);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(widgets->builder,"box_classe")),ch);
                if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(wid))!=coch) /* on ne modifie que si c'est différent */
                {
                    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(wid),coch);
                }
                else
                {
                    /* il était déjà activé */
                }
                if (est_compatible(race,j,versi))
                {
                    /*gtk_widget_set_opacity(GTK_WIDGET(wid),1);*/
                    couleur_item(wid,GRIS);
                    gtk_widget_set_sensitive(wid,TRUE);
                }
                else
                {
                    gtk_widget_set_sensitive(wid,FALSE);
                    if(CLASSE[j].add[versi].nom!=NULL)
                    {
                        couleur_item(wid,ORANGE);
                    }
                    else
                    {
                        couleur_item(wid,ROUGE);
                    }
                }



            }
      }

      g_free(classe);
    }

}

void change_choix_pref (GtkComboBox *widget, struct widgets * widgets)
{
    const char * nom_CB;
    unsigned short i;
    char ch[16];

    if (widgets!=NULL && widgets->window!=NULL)
    {
        nom_CB=gtk_widget_get_name (GTK_WIDGET(widget));

        for (i=0;i<6;i++)
        {
            if (nom_CB[3]==Lettre1_caract[i]) /* on a trouvé l'item changé */
            {
                ch[0]=Lettre1_caract[i];
                ch[1]='1'+gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
                ch[2]=0;
                gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM((gtk_builder_get_object(widgets[1].builder,ch))),TRUE);
                i=6; /* on sort de la boucle */
            }
            else
            {
                /* pas le bon item */
            }
        }
    }
}

void change_choix_niv (GtkComboBox *widget, struct widgets * widgets)
{
    unsigned short i;
    char ch[16];

    if (widgets!=NULL && widgets->window!=NULL)
    {
        i=gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
        sprintf(ch,"Niveau%hu",i);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM((gtk_builder_get_object(widgets[1].builder,ch))),TRUE);
    }
}

void change_choix_tirage (GtkRadioButton *widget, struct widgets * widgets)
{
    const char * nom_RB=NULL;
    GtkWidget * menu=NULL;

    if (widgets!=NULL && widgets->window!=NULL)
    {
        nom_RB=gtk_widget_get_name (GTK_WIDGET(widget));
        if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
        {

            menu=GTK_WIDGET(gtk_builder_get_object(widgets[1].builder,nom_RB));

            if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menu)))
            {
                gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu),TRUE);
            }
        }
    }
}

void change_choix_sexe(GtkRadioButton *widget, struct widgets * widgets)
{
    const char * nom_S=NULL;
    GtkWidget * menu=NULL;

    if (widgets!=NULL && widgets->window!=NULL)
    {
        nom_S=gtk_widget_get_name (GTK_WIDGET(widget));
        if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
        {

            menu=GTK_WIDGET(gtk_builder_get_object(widgets[1].builder,nom_S));

            if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menu)))
            {
                gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu),TRUE);
            }
        }
    }
}


void change_choix_pdv(GtkRadioButton *widget, struct widgets * widgets)
{
    const char * nom_S=NULL;
    GtkWidget * menu=NULL;

    if (widgets!=NULL && widgets->window!=NULL)
    {
        nom_S=gtk_widget_get_name (GTK_WIDGET(widget));
        if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
        {

            menu=GTK_WIDGET(gtk_builder_get_object(widgets[1].builder,nom_S));

            if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menu)))
            {
                gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu),TRUE);
            }
        }
    }
}

void change_choix_version(GtkRadioButton *widget, struct widgets * widgets)
{
    const char * nom_S=NULL;
    GtkWidget * menu=NULL;

    if (widgets!=NULL && widgets->window!=NULL)
    {
        nom_S=gtk_widget_get_name (GTK_WIDGET(widget));
        if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
        {
            menu=GTK_WIDGET(gtk_builder_get_object(widgets[1].builder,nom_S));

            if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menu)))
            {
                gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu),TRUE);
            }
            else
            {
            }
        }
    }
}

void Change_Choix_Race (GtkComboBox *widget, struct widgets * widgets)
{
    const char * nom_Race;
    signed short race_choix;

    GtkMenuItem * menu_race;
    signed short j;
    GtkWidget *pMenu ;
    GList * gl, *gl1;


    if (widgets!=NULL && widgets->window!=NULL && gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
    { /* on ne passe que pour activer une race */
        nom_Race=gtk_widget_get_name (GTK_WIDGET(widget));
        sscanf(nom_Race,"race_%hd",&race_choix);



        menu_race = GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"Race"));
        if (menu_race==NULL)
        {
              printf("Entrée Race non trouvée\n");
        }
        else
        {
          pMenu=gtk_menu_item_get_submenu(GTK_MENU_ITEM(menu_race));                                      /* on trouve le sous-menu                             */
          if (pMenu==NULL)
          {
              printf("Erreur : menu absent\n");
          }
          else
          {
              gl1=gtk_container_get_children(GTK_CONTAINER(pMenu));
              for (gl=gl1,j=0; gl != NULL; gl= g_list_next(gl))
              {
                  if (j==race_choix)
                  {
                      if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(gl->data))!=TRUE)
                      {
                          gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(gl->data),TRUE);
                          Change_Race (GTK_MENU_ITEM(gl->data),widgets);
                      }
                  }
                  j++;
              }
              g_list_free(gl1);
          }
        }

    }
}

void Change_Choix_Classe (GtkComboBox *widget, struct widgets * widgets)
{
    const char * nom_Classe=NULL;
    signed short classe_choix;

    GtkMenuItem * menu_classe=NULL;
    signed short j;
    GtkWidget *pMenu=NULL ;
    GList * gl=NULL, *gl1=NULL;


    if (widgets!=NULL && widgets->window!=NULL)
    {
        nom_Classe=gtk_widget_get_name (GTK_WIDGET(widget));
        if (sscanf(nom_Classe,"classe_%hd",&classe_choix)==1)
        {
            menu_classe = GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"Classe"));
            if (menu_classe==NULL)
            {
                  printf("Entrée Classe non trouvée\n");
            }
            else
            {
              pMenu=gtk_menu_item_get_submenu(GTK_MENU_ITEM(menu_classe));                                      /* on trouve le sous-menu                             */
              if (pMenu==NULL)
              {
                  printf("Erreur : menu absent\n");
              }
              else
              {
                  gl1=gtk_container_get_children(GTK_CONTAINER(pMenu));
                  for (gl=gl1,j=0; gl != NULL; gl= g_list_next(gl))
                  {
                      if (j==classe_choix)
                      {
                          if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(gl->data))!=gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)))
                          {
                              gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(gl->data),gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)));
                              Change_Classe (GTK_MENU_ITEM(gl->data),widgets);
                          }
                      }
                      j++;
                  }
                  g_list_free(gl1);
              }
            }
        }
        else
        {
            dialogue("menu.c, Change_Choix_Classe, classe non trouvée",0);
        }
    }
}

void Change_Tirage (GtkMenuItem *menuitem, struct widgets * widgets)
{
    FeuilleDeChoix(NULL,NULL);

    (void)menuitem;
    (void)widgets;
}

void Change_Version (GtkMenuItem *menuitem, struct widgets * widgets)
{

    if (RACE[trouve_race(widgets)].add[trouve_version(widgets)].nom==NULL)
    {
        init_race(widgets);
    }
    else
    {
        Change_Race(menuitem,widgets);
    }
}

void Change_Sexe (GtkMenuItem *menuitem, struct widgets * widgets)
{
    FeuilleDeChoix(NULL,NULL);

    (void)menuitem;
    (void)widgets;
}

void Change_Pdv (GtkMenuItem *menuitem, struct widgets * widgets)
{
    FeuilleDeChoix(NULL,NULL);

    (void)menuitem;
    (void)widgets;
}

void Change_Race (GtkMenuItem *menuitem, struct widgets * widgets)
{
    signed short retour,race, * classe=NULL,version=trouve_version(widgets),i,j,num_classe;
    GtkMenuItem * menu_classe=NULL;
    GtkWidget *pMenu=NULL ;
    GList * gl=NULL, *gl1=NULL;
    const char * ch=NULL;
    static signed short drap=0;

    if (drap==1)
    {
        //drap=2;
        //while (drap!=0);
    }
    else //if (drap==0)
    {
        drap=1;
        race=trouve_race(widgets);
        classe=trouve_classe(widgets);
        retour=est_multi_compatible(race,classe,version);
        if (retour<=0 && classe!=NULL)
        { /* la liste de classe n'est pas compatible => on propose un choix compatible */
            if (retour==0)
            {/* on a une liste à mettre */
                ch=gtk_widget_get_name(GTK_WIDGET(menuitem));
                i=sscanf(ch,"classe_%hd",&num_classe);
                if (i!=0)
                {
                    i=0;
                    for (j=0;classe[j]!=-1;j++)
                    {
                        if(classe[j]==num_classe)
                        {
                            i=1;
                        }
                        else
                        {
                            /* ce n'est pas la bonne classe */
                        }
                    }
                    if (i==0 && est_compatible(race,num_classe,version))
                    {
                        classe=(signed short *)g_realloc(classe,2*sizeof(signed short ));
                        classe[0]=num_classe;
                        classe[1]=-1;
                    }
                    else
                    {
                        /* si la classe n'est pas trouvée, on ne prend que celle-là */
                    }
                }
                else
                {
                    /*printf("menu.c, fonction Change_Race, erreur dans la lecture de %s\n",ch);*/
                }
                menu_classe = GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"Classe"));
                if (menu_classe==NULL)
                {
                      printf("Entrée Classe non trouvée\n");
                }
                else
                {
                  pMenu=gtk_menu_item_get_submenu(GTK_MENU_ITEM(menu_classe));                                      /* on trouve le sous-menu                             */
                  if (pMenu==NULL)
                  {
                      printf("Erreur sous-menu absent\n");
                  }
                  else
                  {
                      gl1=gtk_container_get_children(GTK_CONTAINER(pMenu));
                      for (j=0,gl=gl1;gl != NULL; gl= g_list_next(gl),j++) /* parcours des classes */
                      {
                          if (j<NB_CLASSE)
                          {
                              for(i=0;classe[i]!=-1;i++)
                              {
                                  retour=0;
                                  if (classe[i]==j)
                                  {
                                      retour=1;
                                  }
                                  else
                                  {
                                      /* pas la bonne classe */
                                  }
                              }
                              if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(gl->data))!=(retour==1))
                              { /* mise dans le bon état du menu */
                                  gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(gl->data),retour==1);
                              }
                              else
                              {
                                  /* le menu était dans le bon état => pas besoin de le modifier */
                              }
                          }
                          else
                          {
                              /* j trop grand : plus grand que le nombre de race */
                          }
                      }
                      g_list_free(gl1);
                  }
                }
            }
            else
            {/* aucune classe correspond à cette race */
                  menu_classe = GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"Classe"));
                  pMenu=gtk_menu_item_get_submenu(GTK_MENU_ITEM(menu_classe));                                      /* on trouve le sous-menu                             */
                  if (pMenu==NULL)
                  {
                      printf("Erreur sous-menu absent\n");
                  }
                  else
                  {
                      gl1=gtk_container_get_children(GTK_CONTAINER(pMenu));
                      for (j=0,gl=gl1;gl != NULL; gl= g_list_next(gl),j++) /* parcours des classes */
                      {
                              if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(gl->data))==TRUE)
                              {
                                  gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(gl->data),FALSE);
                              }
                              else
                              {
                                  /* pas besoin de changer l'état */
                              }
                      }
                      g_list_free(gl1);
                  }
            }
        }
        FeuilleDeChoix(NULL,NULL);
        g_free(classe);
        drap=0;
    }
}

void Change_Classe (GtkMenuItem *menuitem, struct widgets * widgets)
{
    Change_Race(menuitem,widgets);
}

void Change_Niv(GtkMenuItem *menuitem, struct widgets * widgets)
{
    char ch[16]="Niveau ";
    unsigned short i;
    GtkWidget * wid;

    for (i=0;i<=20;i++) /* les niveaux */
    {
        sprintf(ch+6,"%hu",i);
        wid=GTK_WIDGET((gtk_builder_get_object(widgets->builder,ch)));
        if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(wid))==TRUE)
        {
            wid=GTK_WIDGET((gtk_builder_get_object(widgets->builder,"Niveau")));
            sprintf(ch+6," %hu",i);
            gtk_menu_item_set_label(GTK_MENU_ITEM(wid),ch);
            i=20; /* accélérer la sortie de la boucle */
        }
    }
    //if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem)))
    {
        FeuilleDeChoix(NULL,NULL);
    }

    (void)menuitem;
}

void callback_ouvre_recent(GtkMenuItem *menuitem, struct widgets * widgets)
{
    if (ouvre_perso((char *)gtk_menu_item_get_label (menuitem)))
    {
        envp_mem_hist((char *)gtk_menu_item_get_label (menuitem));
    }
    else
    {
        envp_mem_hist_supp((char *)gtk_menu_item_get_label (menuitem));
    }

    (void) widgets;
}

void callback_aff_menu_recent(GtkMenuItem *menuitem, struct widgets * widgets)
{
    GtkMenuItem *menuaffichage=NULL;
    GtkWidget *pMenu=NULL ;
    GtkWidget *e_menu=NULL ;
    char aff[LONG],* ch1=NULL;
    unsigned short i=0,j=0,k;
    GList * gl=NULL;
    FILE * fp=NULL;

    menuaffichage = GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"Fichiers_recents"));
    if (menuaffichage==NULL)
    {
        printf("Entrée menu \"Fichiers_recents\" non trouvée\n");
    }
    else
    { /* ouverture correcte */
        pMenu=gtk_menu_item_get_submenu(GTK_MENU_ITEM(menuaffichage));                                      /* on trouve le sous-menu                             */
        if (pMenu==NULL)
        {
            pMenu=gtk_menu_new ();
        }
        else
        {
            gl=gtk_container_get_children(GTK_CONTAINER(pMenu));
            for (j=0; gl != NULL; gl= g_list_next(gl),j++)
            {
                gtk_widget_destroy(GTK_WIDGET(gl->data));
            }
            g_free(gl);
        }
    }

    ch1=getenv(MEM_PROG);
    sprintf(aff,"%s%c.perso.add%chist",ch1,SEPARATEUR,SEPARATEUR);
    fp = g_fopen(aff, "r");
    if (fp!=NULL && menuaffichage!=NULL)
    {
        while ((ch1=fgets(aff,LONG-1,fp)))
        {
                for(k=0;ch1[k]!='\n' && ch1[k]!=0;k++);
                ch1[k]=0; /* on enlève le saut de ligne final */
                if (j==0 && meme_extension(aff,"grp")==1)
                {
                    e_menu =gtk_separator_menu_item_new ();
                    gtk_menu_shell_insert(GTK_MENU_SHELL(pMenu),e_menu,i);
                    i++;
                    j=1;
                }
                else
                { /* on ne vient pas de passer dans l'affichage des groupes */
                }
                e_menu = gtk_menu_item_new_with_mnemonic(aff);
                gtk_menu_item_set_label(GTK_MENU_ITEM(e_menu),aff);
                sprintf(aff,"pers_grp_%hu",i);
                gtk_widget_set_name(e_menu,aff);
                gtk_menu_shell_insert(GTK_MENU_SHELL(pMenu),e_menu,i);
                if (e_menu==NULL) printf("Erreur !");
                gtk_menu_item_set_submenu (menuaffichage, pMenu);
                g_signal_connect(G_OBJECT(e_menu), "activate", G_CALLBACK(callback_ouvre_recent), widgets);
                i++;

        }
        fclose(fp);
    }
    else
    { /* pas de fichiers historique ou pas d'entrée menu */
    }
    gtk_widget_show_all(pMenu);

    (void) menuitem;
}
