#include "include.h"
#include <string.h>


#define ORIGINE   1
#define PERSONNEL 0

unsigned long tabl_sphere(char * liste);
void Enregistre_Dieu_presse(GtkWidget *ChildWidget, struct widgets *widgets);
void Modif_Pan(GtkWidget *ChildWidget, struct widgets *widgets,unsigned short num_dieu);
void Modif_Sphere(GtkWidget *ChildWidget, struct widgets *widgets);
void aucune(GtkWidget *ChildWidget, struct widgets *widgets);
void Majeure(GtkWidget *ChildWidget, struct widgets *widgets);
void mineure(GtkWidget *ChildWidget, struct widgets *widgets);
void ajuste_sphere(struct widgets *widgets,unsigned short etat);
void Suppr_dieu (GtkWidget *ChildWidget , struct widgets * _widgets);
void Appli_dieu (GtkWidget *ChildWidget , struct widgets * _widgets);
void lit_dieu(dieux * dieu,struct widgets * widgets);
void Aff_liste_pan(struct widgets *widgets,unsigned short num_pan);
void init_dieu(dieux * dieu);
void libere_dieu(dieux * dieu);
void annuler_sphere(GtkWidget *ChildWidget, struct widgets *widgets);
void finir_dieu(GtkWidget *ChildWidget, struct widgets *widgets);
signed short dieu_alignement(GMarkupDomNode * node);
void dieu_competence_plus(GtkWidget *ChildWidget, struct widgets *widgets);
void dieu_competence_moins(GtkWidget *ChildWidget, struct widgets *widgets);

pant * pantheons=NULL;
signed short nb_pantheons=0;

pant * donne_pantheons()
{
    return pantheons;
}

pant * genere_pantheon(char * fichier)
{
    /* couple panthéon/dieu si deux dieux même nom dans deux panthéons différents */
    gchar ch[LONG], * s;
    GError * error=NULL;
    GMarkupDomNode * nodeo=NULL, * tmp_node=NULL,*node=NULL,*nodep=NULL,*noded=NULL;
    unsigned short i,j,k,l,pan=0, dieu,nb_pan=0,nb_dieu,ok;
    GtkWidget *p_dialog = NULL;
    unsigned short question_pan, modif=0;

    nodeo=g_markup_dom_new(fichier,&error); /* on récupère les données dans le fichier xml dieux.add */
    if (nodeo!=NULL)
    {
        node=nodeo->fils; /* rentrer dans "document" */

        pantheons=(pant *)g_malloc((node->nb_fils+1)*sizeof(pant));
        for(i=0; i<node->nb_fils; i++) /* parcours des panthéons, le premier niveau du document, dans le fichier xml où tout devrait être trié dans l'ordre alphabétique */
        {
            if (node->fils[i].nb_texte>0)
            {
                pantheons[i].nom=g_strdup(node->fils[i].texte[0].texte);
            }
            else
            { /* ceci est sensé ne pas arriver ! */
              printf("Erreur : panthéon %hu sans nom\n",i+1);
              pantheons[i].nom=g_strdup("ERREUR");
            }
            pantheons[i].dieu=NULL;
            pantheons[i].ori=ORIGINE;
            nodep=node->fils+i;
            pantheons[i].dieu=(dieux *)g_malloc((nodep->nb_fils+1)*sizeof(dieux));
            for(j=0; j<nodep->nb_fils; j++)
            {
                if (nodep->fils[j].nb_texte>0)
                {
                    pantheons[i].dieu[j].nom=g_strdup(nodep->fils[j].texte[0].texte);
                }
                else
                {
                  printf("Erreur au pantheon %s (%hu) dieu %hu sans nom\n",pantheons[i].nom,i+1,j+1);
                  pantheons[i].dieu[j].nom=g_strdup("ERREUR");
                }
                pantheons[i].dieu[j].ori=ORIGINE;
                tmp_node=nodep->fils+j;

                noded=g_markup_dom_node(tmp_node,"min");
                if (noded && noded->nb_texte>0)
                {
                  pantheons[i].dieu[j].mineure=tabl_sphere(noded->texte[0].texte);
                }
                else
                {
                  pantheons[i].dieu[j].mineure=0;
                }

                noded=g_markup_dom_node(tmp_node,"maj");
                if (noded && noded->nb_texte>0)
                  pantheons[i].dieu[j].majeure=tabl_sphere(noded->texte[0].texte);
                else
                  pantheons[i].dieu[j].majeure=0;

                noded=g_markup_dom_node(tmp_node,"symbole");
                if (noded && noded->nb_texte>0)
                  pantheons[i].dieu[j].symbole=g_strdup(noded->texte[0].texte);
                else
                  pantheons[i].dieu[j].symbole=NULL;

                noded=g_markup_dom_node(tmp_node,"commentaire");
                if (noded && noded->nb_texte>0)
                  pantheons[i].dieu[j].commentaire=g_strdup(noded->texte[0].texte);
                else
                  pantheons[i].dieu[j].commentaire=NULL;

                noded=g_markup_dom_node(tmp_node,"vd");
                if (noded && noded->nb_texte>0)
                  pantheons[i].dieu[j].vd=g_strdup(noded->texte[0].texte);
                else
                  pantheons[i].dieu[j].vd=NULL;

                noded=g_markup_dom_node(tmp_node,"vd_delta");
                if (noded && noded->nb_texte>0)
                {
                    if (sscanf(noded->texte[0].texte,"%hd",&pantheons[i].dieu[j].vd_delta)==0)
                        pantheons[i].dieu[j].vd_delta=0;
                }
                else
                  pantheons[i].dieu[j].vd_delta=0;

                pantheons[i].dieu[j].pc_nb=0;
                pantheons[i].dieu[j].pc=NULL;
                pantheons[i].dieu[j].pc_niv=NULL;
                noded=g_markup_dom_node(tmp_node,"pc");
                if (noded!=NULL && noded->nb_fils>0)
                {
                      for (k=0;k<noded->nb_fils;k++)
                      {
                            if (compare_sans_casse(noded->fils[k].nom,"item")==0)
                            {
                                if (noded->fils[k].nb_texte>0 && sscanf(noded->fils[k].texte[0].texte,"%hu",&l)>0)
                                {
                                    pantheons[i].dieu[j].pc_nb++;
                                    pantheons[i].dieu[j].pc_niv=(unsigned short *)g_realloc(pantheons[i].dieu[j].pc_niv,pantheons[i].dieu[j].pc_nb*sizeof(unsigned short));
                                    pantheons[i].dieu[j].pc=(char **)g_realloc(pantheons[i].dieu[j].pc,pantheons[i].dieu[j].pc_nb*sizeof(char *));
                                    pantheons[i].dieu[j].pc_niv[pantheons[i].dieu[j].pc_nb-1]=l;
                                    for (l=0;noded->fils[k].texte[0].texte[l+1]!=' ' && noded->fils[k].texte[0].texte[l+1]!=0;l++); /*aller à la fin du chiffre */
                                    for (;(noded->fils[k].texte[0].texte[l+1]==' '  || noded->fils[k].texte[0].texte[l+1]=='-') && noded->fils[k].texte[0].texte[l+1]!=0;l++); /*aller au début du texte */
                                    pantheons[i].dieu[j].pc[pantheons[i].dieu[j].pc_nb-1]=g_strdup(noded->fils[k].texte[0].texte+l);
                                }
                                else
                                {
                                    printf("Item %hu du dieu %s du panthéon %s suspecte : %s\n",k,pantheons[i].dieu[j].nom,pantheons[i].nom,noded->fils[k].texte[0].texte);
                                }
                            }
                            else
                            {
                                printf("Entrée %hu nommée %s suspecte dans les PC du dieu %s du panthéon %s\n",k,noded->fils[k].nom,pantheons[i].dieu[j].nom,pantheons[i].nom);
                            }
                      }
                }
                else
                {
                    /* pas d'entrée pc */
                }

                pantheons[i].dieu[j].alignement=dieu_alignement(tmp_node);


            }
            init_dieu(pantheons[i].dieu+j);
        }
        pantheons[i].nom=NULL; /* on marque la fin par un nom NULL */
        pantheons[i].dieu=NULL;
        nb_pan=i;
        g_markup_dom_free(nodeo);
        nodeo=NULL;

    }

    s=donne_chemin_dieux_perso();
    nodeo=g_markup_dom_new(s,NULL);
    g_free(s);
    if (nodeo!=NULL && pantheons!=NULL)
    {
        node=nodeo->fils; /* rentrer dans document */
        pan=1; /* le panthéon 0 est à part : c'est le général pour les clerc, paladins, rodeurs, druides dont les sorts ne dépendent pas du dieu */
        if(node!=NULL)
        {
            for(i=0; i<node->nb_fils; i++) /* parcours des panthéons dans le fichier xml où tout devrait être trié dans l'ordre alphabétique */
            {
                while (pantheons[pan].nom!=NULL && compare_sans_casse(pantheons[pan].nom,node->fils[i].texte[0].texte)<0) /* les ordres étant alphabétique, pas besoin de recommencer à pan=1 */
                {
                    pan++; /* on cherche le premier panthéon supérieur dans l'ordre alphabétique */
                }
                if (pantheons[pan].nom==NULL || compare_sans_casse(pantheons[pan].nom,node->fils[i].texte[0].texte)!=0) /* nouveau panthéon */
                {
                  nb_pan++;
                  pantheons=(pant *)g_realloc(pantheons,(nb_pan+1)*sizeof(pant));
                  for (j=nb_pan;j>pan;j--) /* décallage des panthéons */
                    pantheons[j]=pantheons[j-1];
                  /* vider le panthéon pan pour y mettre le nouveau panthéon pas free => on efface alors le suivnt qui a les mêmes pointeurs */
                  /* ne pas faire g_free(pantheons[pan].nom); ne pas vider cette mémoire car elle est pour le panthéon suivant */
                  pantheons[pan].nom=NULL;
                  pantheons[pan].dieu=(dieux *)g_malloc(sizeof(dieux)); /* on crée le premier dieu du panthéon pour dire que c'est le dernier : vide */
                  init_dieu(pantheons[pan].dieu);
                  if (node->fils[i].nb_texte>0)
                  {
                      pantheons[pan].nom=g_strdup(node->fils[i].texte[0].texte);
                  }
                  else
                  { /* ceci est sensé ne pas arriver ! */
                    printf("Erreur : panthéon %hu sans nom\n",i+1);
                    pantheons[pan].nom=g_strdup("ERREUR");
                  }
                }
                nodep=node->fils+i; /* on passe aux dieux */
                nb_dieu=0; /* on commence un panthéon et on va compter combien de dieux il a déjà dans le base de données */
                while (pantheons[pan].dieu[nb_dieu].nom!=NULL) nb_dieu++; /* compter les dieux de ce panthéon */
                dieu=0; /* on commence un panthéon qu'on va parcourir du début */
                question_pan=0; /* savoir si on veut conserver nos modifications sur le panthéon */
                for(j=0; j<nodep->nb_fils; j++)
                {
                    while (pantheons[pan].dieu[dieu].nom!=NULL && compare_sans_casse(pantheons[pan].dieu[dieu].nom,nodep->fils[j].texte[0].texte)<0)
                      dieu++; /* on cherche le premier dieu supérieur dans l'ordre alphabétique */
                    if (pantheons[pan].dieu[dieu].nom==NULL || compare_sans_casse(pantheons[pan].dieu[dieu].nom,nodep->fils[j].texte[0].texte)!=0) /* le dieu n'existe pas encore */
                    {
                      nb_dieu++;
                      pantheons[pan].dieu=g_realloc(pantheons[pan].dieu,(nb_dieu+1)*sizeof(dieux));
                      for (k=nb_dieu;k>dieu;k--) /* décallage des panthéons */
                        pantheons[pan].dieu[k]=pantheons[pan].dieu[k-1];
                      ok=1;
                    }
                    else
                    {
                      if (question_pan==0) /* on n'a pas encore répondu à cette question */
                      {
                        sprintf(ch,"Vous avez au moins un dieu modifié dans le panthéon %s.\n Voulez-vous conserver toutes vos modifications ?\n ATTENTION :\nLes modifications non conservées seront effacées si vous modifiez ou rentrez un dieu.",pantheons[pan].nom);
                        p_dialog = gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
                        switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
                        {
                            case GTK_RESPONSE_NO:
                                gtk_widget_destroy(p_dialog);;
                                sprintf(ch,"Voulez-vous conserver au moins une des modifications que vous avez apporté au panthéon %s?\n ATTENTION :\nLes modifications non conservées seront effacées si vous modifiez ou rentrez un dieu.",pantheons[pan].nom);
                                p_dialog = gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
                                switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
                                {
                                    case GTK_RESPONSE_NO:
                                        question_pan=3;
                                        modif=1;
                                        break;
                                    default :
                                        question_pan=2;
                                        break;
                                }
                                gtk_widget_destroy(p_dialog);
                                break;
                            default :
                                question_pan=1;
                                break;
                        }
                        gtk_widget_destroy(p_dialog);
                      }
                      if (question_pan==2) /* on souhaite pouvoir garder certaines de nos modifications */
                      {
                        sprintf(ch,"Voulez-vous prendre votre version personnelle pour\nle dieu %s du panthéon %s\n ATTENTION :\nLes modifications non conservées seront effacées si vous modifiez ou rentrez un dieu.",pantheons[pan].dieu[dieu].nom,pantheons[pan].nom);
                        p_dialog = gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
                        switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
                        {
                            case GTK_RESPONSE_NO:
                                ok=0;
                                modif=1;
                                break;
                            default :
                                ok=1;
                                break;
                        }
                        gtk_widget_destroy(p_dialog);
                       }
                       else
                        ok=(question_pan==1);
                    }

                    if (ok==1) /* on veut modifier ce dieu */
                    {
                      /* vider le dieu dieu pour y mettre le nouveau dieu */
                      g_free(pantheons[pan].dieu[dieu].nom);
                      pantheons[pan].dieu[dieu].nom=NULL;
                      pantheons[pan].dieu[dieu].alignement=0;
                      g_free(pantheons[pan].dieu[dieu].vd);
                      pantheons[pan].dieu[dieu].vd=NULL;
                      g_free(pantheons[pan].dieu[dieu].symbole);
                      pantheons[pan].dieu[dieu].symbole=NULL;
                      g_free(pantheons[pan].dieu[dieu].commentaire);
                      pantheons[pan].dieu[dieu].commentaire=NULL;

                      if (nodep->fils[j].nb_texte>0)
                      {
                        pantheons[pan].dieu[dieu].nom=g_strdup(nodep->fils[j].texte[0].texte);
                      }
                      else
                      {
                        printf("Erreur au pantheon %s (%hu) dieu %hu sans nom\n",pantheons[pan].nom,i+1,j+1);
                        pantheons[pan].dieu[dieu].nom=g_strdup("ERREUR");
                      }
                      pantheons[pan].dieu[dieu].ori=PERSONNEL;
                      tmp_node=nodep->fils+j;

                      noded=g_markup_dom_node(tmp_node,"min");
                      if (noded!=NULL && noded->nb_texte>0)
                      {
                        pantheons[pan].dieu[dieu].mineure=tabl_sphere(noded->texte[0].texte);
                      }
                      else
                        pantheons[pan].dieu[dieu].mineure=0;

                      noded=g_markup_dom_node(tmp_node,"maj");
                      if (noded!=NULL && noded->nb_texte>0)
                        pantheons[pan].dieu[dieu].majeure=tabl_sphere(noded->texte[0].texte);
                      else
                        pantheons[pan].dieu[dieu].majeure=0;

                      noded=g_markup_dom_node(tmp_node,"symbole");
                      if (noded!=NULL && noded->nb_texte>0)
                        pantheons[pan].dieu[dieu].symbole=g_strdup(noded->texte[0].texte);
                      else
                        pantheons[pan].dieu[dieu].symbole=NULL;

                      noded=g_markup_dom_node(tmp_node,"commentaire");
                      if (noded!=NULL && noded->nb_texte>0)
                        pantheons[pan].dieu[dieu].commentaire=g_strdup(noded->texte[0].texte);
                      else
                        pantheons[pan].dieu[dieu].commentaire=NULL;

                      noded=g_markup_dom_node(tmp_node,"vd");
                      if (noded!=NULL && noded->nb_texte>0)
                        pantheons[pan].dieu[dieu].vd=g_strdup(noded->texte[0].texte);
                      else
                        pantheons[pan].dieu[dieu].vd=NULL;

                      noded=g_markup_dom_node(tmp_node,"vd_delta");
                      if (noded!=NULL && noded->nb_texte>0)
                      {
                        if (sscanf((noded->texte[0].texte),"%hd",&(pantheons[pan].dieu[dieu].vd_delta))!=1)
                        {
                            pantheons[pan].dieu[dieu].vd_delta=0;
                        }
                      }
                      else
                      {
                          pantheons[pan].dieu[dieu].vd_delta=0;
                      }

                      noded=g_markup_dom_node(tmp_node,"pc");
                      pantheons[pan].dieu[dieu].pc_nb=0;
                      pantheons[pan].dieu[dieu].pc=NULL;
                      pantheons[pan].dieu[dieu].pc_niv=NULL;
                      if (noded!=NULL && noded->nb_fils>0)
                      {
                          for (k=0;k<noded->nb_fils;k++)
                          {
                                if (compare_sans_casse(noded->fils[k].nom,"item")==0)
                                {
                                    if (noded->fils[k].nb_texte>0 && sscanf(noded->fils[k].texte[0].texte,"%hu",&l)>0)
                                    {
                                        pantheons[pan].dieu[dieu].pc_nb++;
                                        pantheons[pan].dieu[dieu].pc_niv=(unsigned short *)g_realloc(pantheons[pan].dieu[dieu].pc_niv,pantheons[pan].dieu[dieu].pc_nb*sizeof(unsigned short));
                                        pantheons[pan].dieu[dieu].pc=(char **)g_realloc(pantheons[pan].dieu[dieu].pc,pantheons[pan].dieu[dieu].pc_nb*sizeof(char *));
                                        pantheons[pan].dieu[dieu].pc_niv[pantheons[pan].dieu[dieu].pc_nb-1]=l;
                                        for (l=0;noded->fils[k].texte[0].texte[l+1]!=' ' && noded->fils[k].texte[0].texte[l+1]!=0;l++); /*aller à la fin du chiffre */
                                        for (;(noded->fils[k].texte[0].texte[l+1]==' ' || noded->fils[k].texte[0].texte[l+1]=='-') && noded->fils[k].texte[0].texte[l+1]!=0;l++); /*aller au début du texte */
                                        pantheons[pan].dieu[dieu].pc[pantheons[pan].dieu[dieu].pc_nb-1]=g_strdup(noded->fils[k].texte[0].texte+l);
                                    }
                                    else
                                    {
                                        printf("Item %hu du dieu %s du panthéon %s suspecte : %s\n",k,pantheons[pan].dieu[dieu].nom,pantheons[pan].nom,noded->fils[k].texte[0].texte);
                                    }
                                }
                                else
                                {
                                    printf("Entrée %hu nommée %s suspecte dans les PC du dieu %s du panthéon %s\n",k,noded->fils[k].nom,pantheons[pan].dieu[dieu].nom,pantheons[pan].nom);
                                }
                          }
                      }
                      else
                      {
                      }
                      pantheons[pan].dieu[dieu].alignement=dieu_alignement(tmp_node);
                    }
                }
            }
        }

        if (modif==1)
        {
            sprintf(ch,"Vous n'avez pas pris en compte certains des dieux de votre fichier personnel de dieu.\n Voulez-les vous les supprimer TOUS définitivement.\n ATTENTION : vous ne pourrez plus les récupérer.");
            p_dialog = gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
            switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
            {
                case GTK_RESPONSE_NO:
                    break;
                default :
                    //enregistre_pan(0);
                    break;
            }
            gtk_widget_destroy(p_dialog);
        }

        g_markup_dom_free(nodeo);
    }
    else
    {
        if (pantheons==NULL)
        {
            printf("Incapable de lire le fichier de panthéon. Le programme ne marchera pas.\n");
        }
        else
        {
            printf("Pas de dieux personnels\n");
        }
    }

    for(nb_pantheons=pan;pantheons[nb_pantheons].nom!=NULL;nb_pantheons++);

    return pantheons;
}

unsigned long tabl_sphere(char * liste)
{
    unsigned long retour=0;
    unsigned long un=1;
    unsigned short i=0,nombre=0,nb=0;

    for(i=0; liste[i]!=0; i++)
    {
        if (nombre==0 && liste[i]>='0' && liste[i]<='9')
        {
            nombre=1;
            nb=liste[i]-'0';
        }
        else if (liste[i]<'0' || liste[i]>'9')
        {
            if (nombre>0)
            { /* on mémorise le nombre */
                nombre=0;
                retour|=un<<nb;
                nb=0;
            }
        }
        else /* liste[i] est un chiffre, pas le premier de la série */
        {
             nb=liste[i]-'0'+nb*10;
        }
    }

    if (nombre>0)
    { /* on mémorise le nombre */
        retour|=un<<nb;
    }

    return retour;
}


void Modif_dieu (GtkWidget *ChildWidget, struct widgets * _widgets)
{
    unsigned short i;
    GtkGrid *grille=NULL;
    static struct widgets * widgets=NULL;
    GtkWidget * label=NULL, * radio0=NULL, *radio=NULL;
    char ch[32];

    if (widgets==NULL || widgets->builder==NULL) /* pour ne pas ouvrir la fenêtre deux fois */
    {                                            /* widgets->builder est mis automatiquement à NULL quand on ferme la fenêtre */
        if (widgets!=NULL)
        {
            g_free(widgets);
        }
        widgets=ouverture_glade_retour("Dieux.glade",1);
        if (widgets!=NULL)
        {
            grille = GTK_GRID((gtk_builder_get_object(widgets->builder,"Grille_Sphere")));
            if (grille!=NULL)
            {
                for(i=0;2*i<NB_SPHERE;i++)
                {
                    gtk_grid_insert_row (grille, 1+i);
                    label=gtk_label_new (SPHERE[ADD2][i].nom);
                    gtk_grid_attach(grille,label,0,i+1,1,1);
                    gtk_widget_show(label);
                    sprintf(ch,"radio_sphere_%hu_0",i);
                    radio0 = gtk_radio_button_new(NULL);
                    gtk_widget_set_name (radio0,ch);
                    gtk_grid_attach(grille,radio0,1,i+1,1,1);
                    gtk_widget_set_halign(radio0,GTK_ALIGN_CENTER);
                    gtk_widget_show(radio0);
                    sprintf(ch,"radio_sphere_%hu_1",i);
                    radio = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio0));
                    gtk_widget_set_name (radio,ch);
                    gtk_widget_set_halign(radio,GTK_ALIGN_CENTER);
                    gtk_grid_attach(grille,radio,2,i+1,1,1);
                    gtk_widget_show(radio);
                    sprintf(ch,"radio_sphere_%hu_2",i);
                    radio = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio0));
                    gtk_widget_set_name (radio,ch);
                    gtk_grid_attach(grille,radio,3,i+1,1,1);
                    gtk_widget_set_halign(radio,GTK_ALIGN_CENTER);
                    gtk_widget_show(radio);

                    if (i+NB_SPHERE/2+1<=NB_SPHERE) /* ne pas prendre en compte la dernière sphère s'il y en a un nombre impair */
                    {
                        label=gtk_label_new (SPHERE[ADD2][i+NB_SPHERE%2+NB_SPHERE/2].nom);
                        gtk_grid_attach(grille,label,4,i+1,1,1);
                        gtk_widget_show(label);
                        sprintf(ch,"radio_sphere_%hu_0",i+NB_SPHERE%2+NB_SPHERE/2);
                        radio0 = gtk_radio_button_new(NULL);
                        gtk_widget_set_name (radio0,ch);
                        gtk_grid_attach(grille,radio0,5,i+1,1,1);
                        gtk_widget_set_halign(radio0,GTK_ALIGN_CENTER);
                        gtk_widget_show(radio0);
                        sprintf(ch,"radio_sphere_%hu_1",i+NB_SPHERE%2+NB_SPHERE/2);
                        radio = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio0));
                        gtk_widget_set_name (radio,ch);
                        gtk_grid_attach(grille,radio,6,i+1,1,1);
                        gtk_widget_set_halign(radio,GTK_ALIGN_CENTER);
                        gtk_widget_show(radio);
                        sprintf(ch,"radio_sphere_%hu_2",i+NB_SPHERE%2+NB_SPHERE/2);
                        radio = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(radio0));
                        gtk_widget_set_name (radio,ch);
                        gtk_grid_attach(grille,radio,7,i+1,1,1);
                        gtk_widget_set_halign(radio,GTK_ALIGN_CENTER);
                        gtk_widget_show(radio);
                    }
                    else
                    {
                    }
                }
                gtk_builder_connect_signals(widgets->builder, widgets);

            }
            else
            {
                printf("Entrée Grille_Sphere inexistante dans Dieux_glade\n");
            }

            Aff_liste_pan(widgets,0);
        }
        else
        {
            printf("Erreur lors de la création de la fenêtre Dieu.glade\n");
        }

    }
    else
    {
printf("Modif_dieu : !( widgets==NULL || widgets->builder==NULL)\n");
        gtk_window_present(GTK_WINDOW(widgets->window));
    }


    (void)ChildWidget;
    (void)_widgets;
}

void Modif_Pan(GtkWidget *ChildWidget, struct widgets *widgets,unsigned short num_dieu)
{
    signed short panth,i;
    GtkComboBox * combo=NULL;

    panth=gtk_combo_box_get_active(GTK_COMBO_BOX((gtk_builder_get_object(widgets->builder,"Liste_Pantheon"))));

    if(panth!=-1)
    {
        combo = GTK_COMBO_BOX((gtk_builder_get_object(widgets->builder,"Liste_Dieu")));
        gtk_combo_box_text_remove_all (GTK_COMBO_BOX_TEXT(combo));
        if (combo!=NULL)
        {
            for(i=0;pantheons[panth].dieu[i].nom!=NULL;i++)
            {
              gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), pantheons[panth].dieu[i].nom, pantheons[panth].dieu[i].nom);
            }
            if (i==0)
            { /* il n'y a pas de dieu dans ce panthéon */
                 gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo),"", "");
                 gtk_combo_box_set_active (GTK_COMBO_BOX (combo),num_dieu);
                 gtk_combo_box_text_remove_all (GTK_COMBO_BOX_TEXT(combo));
            }
            else
            {
                gtk_combo_box_set_active (GTK_COMBO_BOX (combo),0);
            }
        }
        else
        {
            printf("Entrée Liste_Dieu inexistante dans Dieux_glade\n");
        }
        Modif_Sphere(ChildWidget,widgets);
    }
    else
    {
        /* c'est un nouveau panthéon : on ne peut pas trouver de dieu correspondant */
    }
}


void Appli_dieu (GtkWidget *ChildWidget , struct widgets * widgets)
{
    signed short panth,dieu,j;
    char * _nom=NULL;

    panth=gtk_combo_box_get_active(GTK_COMBO_BOX((gtk_builder_get_object(widgets->builder,"Liste_Pantheon"))));
    dieu=gtk_combo_box_get_active(GTK_COMBO_BOX((gtk_builder_get_object(widgets->builder,"Liste_Dieu"))));

    if (panth==0)
    {
        if (demande_oui_non("Vous voulez modifier des informations\nsur des classes de base.\nCe n'est pas permis.\nVoulez-vous vraiment le faire ?")==1)
        {
            donne_info("Dans ce cas, modifiez manuellement le fichier dieux.add\nJe ne souhaite pas en être responsable.\nPar contre, si c'est pour corriger une erreur par rapport\naux règles, prévenez-moi.");
        }
    }
    else
    {
        if (panth==-1) /* nouveau panthéon */
        {
            _nom=g_strdup(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT((gtk_builder_get_object(widgets->builder,"Liste_Pantheon")))));
            for (j=1;pantheons[j].nom!=NULL;j++);                       /* chercher le dernier panthéon dans la liste */
            pantheons=(pant *)g_realloc(pantheons,(j+2)*sizeof(pant));  /* on commence la recherche à 1 car pantheon[0] est réservé pour le général */
            pantheons[j+1]=pantheons[j];                                /* général : clerc, paladin... */
            while (j>1 && compare_sans_casse(_nom,pantheons[j-1].nom)<0)
            {
                j--;
                pantheons[j+1]=pantheons[j];
            }
            pantheons[j].nom=_nom;
            pantheons[j].dieu=(dieux *)g_malloc(2*sizeof(dieux));
            pantheons[j].ori=PERSONNEL;
            init_dieu(pantheons[j].dieu);
            init_dieu(pantheons[j].dieu+1);
            lit_dieu(pantheons[j].dieu,widgets);
            Aff_liste_pan(widgets,j);
            Modif_Pan(ChildWidget,widgets,0);
        }
        else if (dieu==-1)
        {
            _nom=g_strdup(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT((gtk_builder_get_object(widgets->builder,"Liste_Dieu")))));
            for (j=1;pantheons[panth].dieu[j].nom!=NULL;j++); /* chercher le dernier dieu du panthéon dans la liste */
            pantheons[panth].dieu=(dieux *)g_realloc(pantheons[panth].dieu,(j+2)*sizeof(dieux));
            pantheons[panth].dieu[j+1]=pantheons[panth].dieu[j];
            while (j>0 && compare_sans_casse(_nom,pantheons[panth].dieu[j-1].nom)<0)
            {
                j--;
                pantheons[panth].dieu[j+1]=pantheons[panth].dieu[j];
            }
            init_dieu(pantheons[panth].dieu+j);
            lit_dieu(pantheons[panth].dieu+j,widgets);
            Modif_Pan(ChildWidget,widgets,j);
            g_free(_nom);
        }
        else /* modification d'un dieu existant */
        {
            if (pantheons[panth].dieu[dieu].ori==ORIGINE)
            {   /* protection d'un dieu original */
                donne_info("Vous souhaitez modifier un dieu original.\nCe n'est pas faisable par cette interface graphique.\nSi vous penser qu'il y a une erreur dans le programme\nprévenez-moi. Merci.");
            }
            else
            {
                /*if (demande_oui_non("Vous voulez modifier des informations\nsur un dieu de votre base de donnée.\nVoulez-vous vraiment le corriger ?")==1)*/
                {
                    lit_dieu(pantheons[panth].dieu+dieu,widgets);
                }
            }
        }

        if (ChildWidget!=NULL)
        {
            donne_info("Ceci est une modification momentanée.\nPour qu'elle soit enregistrée,\ncliquez sur [Enregistrer].");
        }
        else
        { /* pour éviter le message si on arrive de l'enregistrement */
        }
    }
}


void Suppr_dieu (GtkWidget *ChildWidget , struct widgets * widgets)
{
    signed short panth,dieu,j;
    char ch[256];
    GtkComboBox * combo;

    panth=gtk_combo_box_get_active(GTK_COMBO_BOX((gtk_builder_get_object(widgets->builder,"Liste_Pantheon"))));
    dieu=gtk_combo_box_get_active(GTK_COMBO_BOX((gtk_builder_get_object(widgets->builder,"Liste_Dieu"))));
    if (panth!=-1 && dieu!=-1)
    {
        if (panth==0)
        {
            donne_info("Vous voulez supprimer des informations\nsur des classes de base.\nCe n'est pas permis.");
        }
        else
        {
            sprintf(ch,"Vous allez supprimer\nle dieu %s\ndu panthéon %s.\nÊtes-bien d'accord ?",pantheons[panth].dieu[dieu].nom,pantheons[panth].nom);
            if (demande_oui_non(ch)==1)
            {
                for (j=dieu;pantheons[panth].dieu[j].nom!=NULL;j++)
                {
                    pantheons[panth].dieu[j]=pantheons[panth].dieu[j+1];
                }
                pantheons[panth].dieu=(dieux *)g_realloc(pantheons[panth].dieu,j*sizeof(dieux));
                Modif_Pan(ChildWidget, widgets,0);
                if (j==1)
                {
                    Suppr_dieu (ChildWidget, widgets); /* on fait à nouveau appel à cette fonction si par hasard, on voulait aussi supprimer le panthéon qui se trouve vide */
                }
            }
            else
            {

            }
        }
    }
    else
    { /* il n'y a pas de dieu à supprimer à supprimer */
        if (panth!=-1)
        { /* mais il peut il avoir un dieu */
            sprintf(ch,"Voulez-vous supprimer\nle panthéon %s qui est vide ?",pantheons[panth].nom);
            if (demande_oui_non(ch)==1)
            {
                for (j=panth;pantheons[j].nom!=NULL;j++)
                {
                    pantheons[j]=pantheons[j+1];
                }
                pantheons=(pant *)g_realloc(pantheons,j*sizeof(pant));
                combo = GTK_COMBO_BOX((gtk_builder_get_object(widgets->builder,"Liste_Pantheon")));
                gtk_combo_box_text_remove_all (GTK_COMBO_BOX_TEXT(combo));
                if (combo!=NULL)
                {
                    for(j=0;pantheons[j].nom!=NULL;j++)
                    {
                      gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), pantheons[j].nom, pantheons[j].nom);
                    }
                    gtk_combo_box_set_active (GTK_COMBO_BOX (combo),0);
                }
            }
        }
    }
}

void Modif_Sphere(GtkWidget *ChildWidget, struct widgets *widgets)
{
    signed short panth,dieu,i,j;
    GtkGrid *grille=NULL;
    char * maj_min=NULL;
    char ch[32];
    GtkWidget * wid;
    GtkEntryBuffer *buffer;

    panth=gtk_combo_box_get_active(GTK_COMBO_BOX((gtk_builder_get_object(widgets->builder,"Liste_Pantheon"))));
    dieu=gtk_combo_box_get_active(GTK_COMBO_BOX((gtk_builder_get_object(widgets->builder,"Liste_Dieu"))));
    if (panth!=-1 && dieu!=-1)
    {
        GList * gl1, * gl;

        maj_min=(char *)g_malloc(sizeof(char)*NB_SPHERE);
        for (i=0;i<NB_SPHERE;i++)
        {
            if ((pantheons[panth].dieu[dieu].majeure&(1<<i))!=0)
            {
                maj_min[i]=1;
            }
            else if ((pantheons[panth].dieu[dieu].mineure&(1<<i))!=0)
            {
                maj_min[i]=2;
            }
            else
            {
                maj_min[i]=0;
            }
        }

        grille = GTK_GRID(gtk_builder_get_object(widgets->builder,"Grille_Sphere"));
        gl1=gtk_container_get_children (GTK_CONTAINER(grille));
        for (gl=gl1; gl != NULL; gl = g_list_next(gl))
        {
               if (strncmp(gtk_widget_get_name (GTK_WIDGET(gl->data)),"radio_sphere_",13)==0)
               {
                        sscanf(gtk_widget_get_name (GTK_WIDGET(gl->data)),"radio_sphere_%hd_%hd",&i,&j);
                        if (maj_min[i]==j)
                        {
                             gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(gl->data),1);
                        }
               }
        }
        g_list_free(gl1);

        strcpy(ch,"Al_");
        for(j=0;j<9;j++)
        {
            strcat(ch,algnm[j]);
            if ((((1<<j) & pantheons[panth].dieu[dieu].alignement))==0)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(widgets->builder,ch)),FALSE);
            }
            else
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(widgets->builder,ch)),TRUE);
            }
            ch[3]=0; /* on remet à nouveau ch="AL_" */
        }

        if (pantheons[panth].dieu[dieu].symbole && pantheons[panth].dieu[dieu].symbole[0])
        {
            gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(widgets->builder,"Entree_Symbole")),pantheons[panth].dieu[dieu].symbole);
        }
        else
        {
            gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(widgets->builder,"Entree_Symbole")),"");
        }
        j=0;
        if (pantheons[panth].dieu[dieu].commentaire) if (pantheons[panth].dieu[dieu].commentaire[0])
        {
            gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(widgets->builder,"Boite_Commentair")),pantheons[panth].dieu[dieu].commentaire);
            j=1;
        }
        if (j==0)
        {
            gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(widgets->builder,"Boite_Commentair")),"");
        }

        if (pantheons[panth].dieu[dieu].vd && pantheons[panth].dieu[dieu].vd[0])
        {
            if (compare_sans_casse(pantheons[panth].dieu[dieu].vd,"repousse")==0)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(widgets->builder,"MV_Repousse")),1);
            }
            else if (compare_sans_casse(pantheons[panth].dieu[dieu].vd,"attire")==0)
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(widgets->builder,"MV_Attire")),1);
            }
            else
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(widgets->builder,"MV_Neutre")),1);
            }
        }
        else
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(widgets->builder,"MV_Neutre")),1);
        }

        gtk_combo_box_set_active(GTK_COMBO_BOX(gtk_builder_get_object(widgets->builder,"MV_Combo_Variation")),pantheons[panth].dieu[dieu].vd_delta+5);

        g_free(maj_min);

        grille=GTK_GRID(gtk_builder_get_object(widgets->builder,"grille_competences"));

        for(i=0;i<pantheons[panth].dieu[dieu].pc_nb;i++)
        {
            wid=gtk_grid_get_child_at (grille,0,i+1);
            sprintf(ch,"%hu",pantheons[panth].dieu[dieu].pc_niv[i]);
            buffer=gtk_entry_buffer_new(ch,-1);
            gtk_entry_buffer_set_max_length (buffer, 2);
            if (wid==NULL)
            { /* la case n'existe pas */
                wid=gtk_entry_new_with_buffer(buffer);
                gtk_entry_set_width_chars (GTK_ENTRY(wid),2);
                gtk_grid_attach(grille,wid,0,i+1,1,1);
            }
            else
            { /* la case existe déjà */
                gtk_entry_set_buffer (GTK_ENTRY(wid),buffer);
            }
            wid=gtk_grid_get_child_at (grille,1,i+1);
            buffer=gtk_entry_buffer_new(pantheons[panth].dieu[dieu].pc[i],-1);
            gtk_entry_buffer_set_max_length (buffer, 200);
            if (wid==NULL)
            { /* la case n'existe pas */
                gtk_grid_attach(grille,gtk_entry_new_with_buffer(buffer),1,i+1,1,1);
            }
            else
            { /* la case existe déjà */
                gtk_entry_set_buffer (GTK_ENTRY(wid),buffer);
            }
        }
        if (i>0)
        {
                /* rien ne bouge */
        }
        else
        {   /* i=0 */
            wid=gtk_grid_get_child_at (grille,0,1);
            buffer=gtk_entry_buffer_new("",-1);
            gtk_entry_buffer_set_max_length (buffer, 2);
            gtk_entry_set_buffer (GTK_ENTRY(wid),buffer);

            wid=gtk_grid_get_child_at (grille,1,1);
            buffer=gtk_entry_buffer_new("",-1);
            gtk_entry_buffer_set_max_length (buffer, 0);
            gtk_entry_set_buffer (GTK_ENTRY(wid),buffer);

            i=1;
            /* on laisse au moins une case vide */
        }
        while ((wid=gtk_grid_get_child_at (grille,0,i+1))!=NULL)
        {
           gtk_grid_remove_row (grille,i+1);
        }
        gtk_widget_show_all(GTK_WIDGET(grille));

    }

    (void)ChildWidget;
}


void finir_dieu(GtkWidget *ChildWidget, struct widgets *widgets)
{
    widgets->builder=NULL;

    (void)ChildWidget;
}


void annuler_sphere(GtkWidget *ChildWidget, struct widgets *widgets)
{
    signed short panth,dieu;

    panth=gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_builder_get_object(widgets->builder,"Liste_Pantheon")));
    if (panth==-1)
    {
        gtk_combo_box_set_active (GTK_COMBO_BOX(gtk_builder_get_object(widgets->builder,"Liste_Pantheon")),0);
    }
    else
    {
        dieu=gtk_combo_box_get_active(GTK_COMBO_BOX((gtk_builder_get_object(widgets->builder,"Liste_Dieu"))));
        if (dieu==-1)
        {
            Modif_Pan(ChildWidget,widgets,0);
        }
        else
        {
            Modif_Sphere(ChildWidget,widgets);
        }
    }
}


void aucune(GtkWidget *ChildWidget, struct widgets *widgets)
{
    ajuste_sphere(widgets,0);

    (void)ChildWidget;
}

void Majeure(GtkWidget *ChildWidget, struct widgets *widgets)
{
    ajuste_sphere(widgets,1);

    (void)ChildWidget;
}

void mineure(GtkWidget *ChildWidget, struct widgets *widgets)
{
    ajuste_sphere(widgets,2);

    (void)ChildWidget;
}

void dieu_competence_plus(GtkWidget *ChildWidget, struct widgets *widgets)
{
    unsigned short i;
    GtkWidget * wid;
    GtkEntryBuffer * buffer;
    GtkGrid * grille;

    grille=GTK_GRID(gtk_builder_get_object(widgets->builder,"grille_competences"));
    i=0;
    do
    {
        i++;
        wid=gtk_grid_get_child_at (grille,0,i+1);
    }
    while (wid!=NULL);

    buffer=gtk_entry_buffer_new("",-1);
    gtk_entry_buffer_set_max_length (buffer, 2);
    wid=gtk_entry_new_with_buffer(buffer);
    gtk_entry_set_width_chars (GTK_ENTRY(wid),2);
    gtk_grid_attach(grille,wid,0,i+1,1,1);

    buffer=gtk_entry_buffer_new("",-1);
    gtk_entry_buffer_set_max_length (buffer, 200);
    wid=gtk_entry_new_with_buffer(buffer);
    gtk_grid_attach(grille,wid,1,i+1,1,1);

    gtk_widget_show_all(GTK_WIDGET(grille));

    (void)ChildWidget;
}

void dieu_competence_moins(GtkWidget *ChildWidget, struct widgets *widgets)
{
    unsigned short i;
    GtkWidget * wid;
    GtkGrid * grille;

    grille=GTK_GRID(gtk_builder_get_object(widgets->builder,"grille_competences"));
    i=0;
    do
    {
        i++;
        wid=gtk_grid_get_child_at (grille,0,i+1);
    }
    while (wid!=NULL);

    if (i>1)
    {
        gtk_grid_remove_row (grille,i);
    }
    else
    {
        /* je conserve la première ligne même vide */
    }

    (void)ChildWidget;
}


void ajuste_sphere(struct widgets *widgets,unsigned short etat)
{
    GtkGrid *grille=NULL;
    signed short i,j;
    GList * gl, *gl1=NULL;

    grille = GTK_GRID((gtk_builder_get_object(widgets->builder,"Grille_Sphere")));
    gl1=gtk_container_get_children (GTK_CONTAINER(grille));
    for (gl=gl1; gl != NULL; gl = g_list_next(gl))
    {
           if (strncmp(gtk_widget_get_name (GTK_WIDGET(gl->data)),"radio_sphere_",13)==0)
           {
               sscanf(gtk_widget_get_name (GTK_WIDGET(gl->data)),"radio_sphere_%hd_%hd",&i,&j);
               if (j==etat)
               {
                   gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(gl->data),1);
               }
           }
    }
    g_list_free(gl1);
}

void init_dieu(dieux * dieu)
{
    dieu->nom=NULL;
    dieu->majeure=0;
    dieu->mineure=0;
    dieu->symbole=NULL;
    dieu->commentaire=NULL;
    dieu->vd=NULL;
    dieu->vd_delta=0;
    dieu->alignement=0;
    dieu->ori=PERSONNEL;
    dieu->pc_nb=0;
    dieu->pc=NULL;
    dieu->pc_niv=NULL;
}

void libere_dieu(dieux * dieu)
{
    unsigned short i;
    for(i=0;i<dieu->pc_nb;i++)
    {
        g_free(dieu->pc[i]);
    }
    g_free(dieu->pc);
    g_free(dieu->pc_niv);
    g_free(dieu->nom);
    g_free(dieu->vd);
    g_free(dieu->symbole);
    g_free(dieu->commentaire);
    init_dieu(dieu);
}

void lit_dieu(dieux * dieu,struct widgets * widgets)
{
    GtkGrid *grille=NULL;
    GtkWidget * wid+NULL;
    signed short i,j,k;
    GList * gl1=NULL, *gl=NULL;
    char ch[LONG], *s=NULL;

    dieu->majeure=0;
    dieu->mineure=0;
    grille = GTK_GRID((gtk_builder_get_object(widgets->builder,"Grille_Sphere")));
    gl1=gtk_container_get_children (GTK_CONTAINER(grille));
    for (gl=gl1; gl != NULL; gl = g_list_next(gl))
    {
           if (strncmp(gtk_widget_get_name (GTK_WIDGET(gl->data)),"radio_sphere_",13)==0)
           {
                	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(gl->data)))
                    {
                        sscanf(gtk_widget_get_name (GTK_WIDGET(gl->data)),"radio_sphere_%hd_%hd",&i,&j);
                        switch (j)
                        {
                            case 1 : dieu->majeure|=(1<<i);
                                break;
                            case 2 : dieu->mineure|=(1<<i);
                                break;
                            default :
                                break;
                        }
                    }
           }
    }
    g_list_free(gl1);
    dieu->nom=g_strdup(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT((gtk_builder_get_object(widgets->builder,"Liste_Dieu")))));
    dieu->ori=PERSONNEL;

    strcpy(ch,"Al_");
    dieu->alignement=0;
    for(i=0;i<9;i++)
    {
        strcat(ch,algnm[i]);
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(widgets->builder,ch))))
        {
            dieu->alignement|=(1<<i);
        }
        else
        { /* alignement non sélectionné */
        }
        ch[3]=0; /* on remet à nouveau ch="AL_" */
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(widgets->builder,"MV_Repousse"))))
    {
        dieu->vd=g_strdup("repousse");
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(widgets->builder,"MV_Attire"))))
    {
        dieu->vd=g_strdup("attire");
    }
    else
    {
        dieu->vd=g_strdup("neutre");
    }
    dieu->vd_delta=gtk_combo_box_get_active(GTK_COMBO_BOX(gtk_builder_get_object(widgets->builder,"MV_Combo_Variation")))-5;
    dieu->symbole=g_strdup(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(widgets->builder,"Entree_Symbole"))));
    dieu->commentaire=g_strdup(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(widgets->builder,"Boite_Commentair"))));
    grille=GTK_GRID(gtk_builder_get_object(widgets->builder,"grille_competences"));
    i=0; /* place dans la grille */
    j=0; /* indice dans le tableau */
    wid=gtk_grid_get_child_at (grille,0,i+1);
    while(wid!=NULL)
    {
        if (sscanf(gtk_entry_get_text (GTK_ENTRY(wid)),"%hd",&k)==1)
        {
            if (j<dieu->pc_nb)
            { /* pas de nouvelles cases à mémoriser */
                g_free(dieu->pc[j]);
            }
            else
            { /* il faut étendre le tableau */
                dieu->pc_niv=(unsigned short *)g_realloc(dieu->pc_niv,(j+1)*sizeof(unsigned short));
                dieu->pc=(char **)g_realloc(dieu->pc,(j+1)*sizeof(char *));
            }
            dieu->pc_niv[j]=k;
            wid=gtk_grid_get_child_at (grille,1,i+1);
            dieu->pc[j]=g_strdup(gtk_entry_get_text (GTK_ENTRY(wid)));
            j++;
        }
        else
        {
            /* pas de niveau => on oublie */
        }
        i++;
        wid=gtk_grid_get_child_at (grille,0,i+1);
    }


    while (dieu->pc_nb>j)
    { /* vider les cases en trop */
        dieu->pc_nb--;
        g_free(dieu->pc[dieu->pc_nb]);
    }
    dieu->pc_nb=j;
    /* diminution possible de la taille des tableaux : beaucoup de calcul pour presque rien */
    /*dieu->pc_niv=(unsigned short *)g_realloc(dieu->pc_niv,dieu->pc_nb*sizeof(unsigned short));
    dieu->pc=(char **)g_realloc(dieu->pc,dieu->pc_nb*sizeof(char *)); */
    /* mise dans l'ordre des niveaux */
    for(i=dieu->pc_nb;i>1;i--)
    {
        j=place_max(dieu->pc_niv,i);
        k=dieu->pc_niv[j]; /* le max */
        s=dieu->pc[j];
        dieu->pc_niv[j]=dieu->pc_niv[i-1]; /* le dernier */
        dieu->pc[j]=dieu->pc[i-1];
        dieu->pc_niv[i-1]=k; /* le max est mis au bout */
        dieu->pc[i-1]=s;
    }

    Modif_Sphere(NULL,widgets); /* pour afficher ce qu'on a enregistrer */

}

void Aff_liste_pan(struct widgets *widgets,unsigned short num_pan)
{
    signed short i;
    GtkComboBox * combo=NULL;

        combo = GTK_COMBO_BOX((gtk_builder_get_object(widgets->builder,"Liste_Pantheon")));
        gtk_combo_box_text_remove_all (GTK_COMBO_BOX_TEXT(combo));
        if (combo!=NULL)
        {
            for(i=0;pantheons[i].nom!=NULL;i++)
            {
              gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), pantheons[i].nom, pantheons[i].nom);
            }
            gtk_combo_box_set_active (GTK_COMBO_BOX (combo),num_pan);
            Modif_Pan(NULL, widgets,0);
        }
        else
        {
            printf("Entrée Liste_Pantheon inexistante dans Dieux_glade\n");
        }

}


void Enregistre_Dieu_presse(GtkWidget *ChildWidget, struct widgets *widgets)
{
    char * s=NULL;
    FILE * fichier=NULL;
    unsigned short i,j,l,enregistre_pan;



    if (demande_oui_non("Voulez-aussi enregistrer le dieu affiché ?")==1)
    {
        Appli_dieu (NULL,widgets);
    }
    else
    {
    }

    for (i=1;pantheons[i].nom!=NULL;i++)
    {
        enregistre_pan=0; /* drapeau pour dire que ce panthéon n'est pas enregistré dans le fichier */
        for (j=0;pantheons[i].dieu[j].nom!=NULL;j++)
        {
            if (pantheons[i].dieu[j].ori==PERSONNEL)
            {
                if (enregistre_pan==0)
                {   /* mise en place du nouveau panhéon dans le fichier de sauvegarde */
                    enregistre_pan=1;
                    if (fichier==NULL) /* fichier pas encore ouvert => on l'ouvre */
                    {
                        s=donne_chemin_dieux_perso();
                        fichier=g_fopen(s,"w");
                        g_free(s);
                        g_fprintf(fichier,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<document>\n<pan>%s\n",pantheons[i].nom);
                    }
                    else
                    {
                        g_fprintf(fichier,"</pan>\n<pan>%s\n",pantheons[i].nom);
                    }

                }
                g_fprintf(fichier," <die>%s\n",pantheons[i].dieu[j].nom);
                if (pantheons[i].dieu[j].majeure!=0)
                {
                    g_fprintf(fichier,"  <maj>\n ");
                    for(l=0;l<32;l++)
                    {
                        if ((pantheons[i].dieu[j].majeure&(1<<l))!=0)
                        {
                            g_fprintf(fichier,"  %hu",l);
                        }
                    }
                    g_fprintf(fichier,"\n  </maj>\n");
                }
                else
                {
                    g_fprintf(fichier,"  <maj />\n");
                }
                if (pantheons[i].dieu[j].mineure!=0)
                {
                    g_fprintf(fichier,"  <min>\n ");
                    for(l=0;l<32;l++)
                    {
                        if ((pantheons[i].dieu[j].mineure&(1<<l))!=0)
                        {
                            g_fprintf(fichier,"  %hu",l);
                        }
                    }
                    g_fprintf(fichier,"\n  </min>\n");
                }
                else
                {
                    g_fprintf(fichier,"  <min />\n");
                }
                if (pantheons[i].dieu[j].symbole!=NULL && pantheons[i].dieu[j].symbole[0]!=0)
                {
                    g_fprintf(fichier,"  <symbole>\n   %s\n  </symbole>\n",pantheons[i].dieu[j].symbole);
                }
                else
                {
                    g_fprintf(fichier,"  <symbole />\n");
                }
                if (pantheons[i].dieu[j].commentaire!=NULL && pantheons[i].dieu[j].commentaire[0]!=0)
                {
                    g_fprintf(fichier,"  <commentaire>\n   %s\n  </commentaire>\n",pantheons[i].dieu[j].commentaire);
                }
                else
                {
                    g_fprintf(fichier,"  <commentaire />\n");
                }
                if (pantheons[i].dieu[j].vd!=NULL && pantheons[i].dieu[j].vd[0]!=0)
                {
                    g_fprintf(fichier,"  <vd>\n   %s\n  </vd>\n",pantheons[i].dieu[j].vd);
                }
                else
                {
                    g_fprintf(fichier,"  <vd />\n");
                }
                g_fprintf(fichier,"  <vd_delta>\n   %hd\n  </vd_delta>\n",pantheons[i].dieu[j].vd_delta);
                if (pantheons[i].dieu[j].alignement!=0)
                {
                    g_fprintf(fichier,"  <alignement>\n ");
                    for (l=0;l<9;l++)
                    {
                        if (((1<<l) & pantheons[i].dieu[j].alignement)!=0)
                        {
                            g_fprintf(fichier,"  %s",algnm[l]);
                        }
                        else
                        { /* alignement non correct pour ce dieu */
                        }
                    }
                    g_fprintf(fichier,"\n  </alignement>\n");
                }
                else
                {
                    g_fprintf(fichier,"  <alignement />\n");
                }

                if (pantheons[i].dieu[j].pc_nb==0)
                {
                    g_fprintf(fichier,"  <pc />\n");
                }
                else
                {
                    g_fprintf(fichier,"  <pc>\n");
                    for (l=0;l<pantheons[i].dieu[j].pc_nb;l++)
                    {
                        g_fprintf(fichier,"   <item>%hu - %s</item>\n",pantheons[i].dieu[j].pc_niv[l],pantheons[i].dieu[j].pc[l]);
                    }
                    g_fprintf(fichier,"  </pc>\n");
                }
                g_fprintf(fichier," </die>\n");
            }
        }
    }

    if (fichier!=NULL)
    {
        g_fprintf(fichier,"</pan>\n</document>");
        fclose(fichier);
    }
    else
    { /* on ne ferme pas un fichier qui n'a pas été ouvert */
    }

    (void)ChildWidget;
}

signed short pantheon_entier(char * _nom)
{
    signed short i, retour=-1;

    retour=0;
    if (_nom!=NULL)
    {
        for (i=0;i<nb_pantheons;i++)
        {
            if (compare_sans_casse(_nom,pantheons[i].nom)==0)
            {
                retour=i;
                i=nb_pantheons;
            }
        }
    }
    else
    {
    }

    return retour;
}

signed short dieu_entier(char * _nom,signed short panth)
{
    signed short i, retour=-1;


    if (_nom!=NULL)
    {
        if (panth<0 || panth>=nb_pantheons)
        {   /* protection */
            if (panth>=nb_pantheons)
            {
                printf("Numéro du panthéon %hd anormal\n",panth);
            }
            else
            {
            }
            panth=0;
        }
        else
        {
        }
        for (i=0;pantheons[panth].dieu[i].nom!=NULL;i++)
        {
            if (compare_sans_casse(_nom,pantheons[panth].dieu[i].nom)==0)
            {
                retour=i;
            }
        }
        if (retour==-1)
        {
            retour=-i;
        }
    }
    else
    {
        for (i=0;pantheons[panth].dieu[i].nom!=NULL;i++);
        retour=-i;
        if (retour==0) retour=-1;  /* renvoyer au minimum -1 : si on passe là, c'est qu'on a un panthéon sans dieu => anormal */
    }

    return retour;
}

GtkComboBoxText * creation_combo_pantheons(void)
{
    unsigned short i;
    GtkComboBoxText * combo;

    combo=GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new ());

    for(i=0;i<nb_pantheons;i++)
    {
        gtk_combo_box_text_append (combo, pantheons[i].nom, pantheons[i].nom);
    }
    gtk_combo_box_text_append (combo, "Aucun", "Aucun");

    return combo;
}

GtkComboBoxText * creation_combo_dieux(signed short panth)
{
    unsigned short i;
    GtkComboBoxText * combo;

    if (panth<0 || panth>=nb_pantheons)
    {   /* protection */
        if (panth>=nb_pantheons)
        {
            printf("Numéro du panthéon %hd anormal\n",panth);
        }
        else
        {
        }
        panth=0;
    }
    else
    { /* cas normal */
    }
    combo=GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new ());

    for (i=0;pantheons[panth].dieu[i].nom!=NULL;i++)
    {
        gtk_combo_box_text_append (combo, pantheons[panth].dieu[i].nom, pantheons[panth].dieu[i].nom);
    }
    gtk_combo_box_text_append (combo, "Aucun", "Aucun");

    return combo;
}

signed short dieu_alignement(GMarkupDomNode * node)
{
    GMarkupDomNode * noded=NULL;
    char ch[LONG], *s;
    signed short retour=0;

    noded=g_markup_dom_node(node,"alignement");
    if (noded && noded->nb_texte>0)
    {
        strcpy(ch,noded->texte[0].texte);
        for(s=ch;*s!=0;s++); /* s pointe sur la fin de la chaine */
        while (s>ch)
        {
            do
            { /* supprimer espace ou , en fin de chaîne */
                *s=0;
                s--;
            }
            while ((*s==' ' || *s==',') && s>ch);
            while (*(s-1)!=' ' && *(s-1)!=',' && s>ch)
            {
                s--;
            }
            if      (compare_sans_casse(s,"BL")==0)
              retour|=BL;
            else if (compare_sans_casse(s,"BN")==0)
             retour|=BN;
            else if (compare_sans_casse(s,"BC")==0)
             retour|=BC;
            else if (compare_sans_casse(s,"NL")==0)
             retour|=NL;
            else if (compare_sans_casse(s,"NN")==0)
             retour|=NN;
            else if (compare_sans_casse(s,"NC")==0)
             retour|=NC;
            else if (compare_sans_casse(s,"ML")==0)
             retour|=ML;
            else if (compare_sans_casse(s,"MN")==0)
             retour|=MN;
            else if (compare_sans_casse(s,"MC")==0)
             retour|=MC;
            else if (compare_sans_casse(s,"N")==0)
              retour|=NN; /* compatibilité avec ancien fichier */
            else
             printf("Mauvaise chaîne alignment en |%s| pour un dieu : %s avec %hx\n",s,noded->texte[0].texte,retour);
        }
    }
    else
    { /* pas d'alignement prévu */
    }

    return retour;
}
