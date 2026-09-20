#include "include.h"

void callback_enlever_classe(GtkWidget *appel, struct widgets ** fenetre);
void callback_enlever_classe_ferme(GtkWidget *appel, struct widgets ** fenetre);
void callback_enlever_arme(GtkWidget *appel, struct widgets ** fenetre);
void callback_enlever_competence(GtkWidget *appel, struct widgets ** fenetre);
GtkComboBox * creation_combo_classe(void);
GtkComboBox * creation_combo_competence(void);
GtkComboBox * creation_combo_arme(void);
void ajouter_pdv(GtkWidget *appel, FenetrePerso * _perso);
void ajouter_classe(GtkWidget *appel, FenetrePerso * _perso);
void ajouter_competence(GtkWidget *appel, FenetrePerso * _perso);
void ajouter_arme(GtkWidget *appel, FenetrePerso * _perso);
void enlever_pdv(GtkWidget *appel, FenetrePerso * _perso);
void enlever_classe(GtkWidget *appel, FenetrePerso * _perso);
void enlever_competence(GtkWidget *appel, FenetrePerso * _perso);
void enlever_arme(GtkWidget *appel, FenetrePerso * _perso);
void enleve_ligne_competence(struct widgets * fenetre, signed short ligne);

unsigned short classe_nb(unsigned short classe, unsigned short * classe_modif)
{
    unsigned short i,retour=0;

    for(i=0;i<NB_CLASSE;i++)
    {
        if (classe_modif[i]==classe)
        {
            retour=i;
            i=NB_CLASSE;
        }
        else
        {
            /* pas la bonne classe */
        }
    }

    return retour;
}

unsigned short race_nb(unsigned short race, unsigned short * classe_race)
{
    unsigned short i,retour=0;

    for(i=0;i<NB_RACE;i++)
    {
        if (classe_race[i]==race)
        {
            retour=i;
            i=NB_RACE;
        }
        else
        {
            /* pas la bonne classe */
        }
    }

    return retour;
}


GtkComboBox * creation_combo_arme(void)
{
    unsigned short i;
    GtkComboBox * combo;

    combo=GTK_COMBO_BOX(gtk_combo_box_text_new ());
    for(i=0;i<NB_ARME;i++)
    {
        gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), ARME[i].nom, ARME[i].nom);
    }

    return combo;
}

GtkComboBox * creation_combo_classe()
{
    unsigned short i;
    GtkComboBox * combo;

    combo=GTK_COMBO_BOX(gtk_combo_box_text_new ());
    for(i=0;i<NB_CLASSE;i++)
    {
        gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), CLASSE[i].nom, CLASSE[i].nom);
    }

    return combo;
}


GtkComboBox * creation_combo_competence(void)
{
    unsigned short i;
    GtkComboBox * combo;
    char ch[64];

    combo=GTK_COMBO_BOX(gtk_combo_box_text_new ());
    for(i=0;i<NB_COMPETENCE;i++)
    {
        if (COMPETENCE[i].modif==0)
        {
            g_sprintf(ch,"%s, %s, %hu",COMPETENCE[i].nom,nom_abr[COMPETENCE[i].caract],COMPETENCE[i].unite);
        }
        else
        {
            g_sprintf(ch,"%s, %s%+hd, %hu",COMPETENCE[i].nom,nom_abr[COMPETENCE[i].caract],COMPETENCE[i].modif,COMPETENCE[i].unite);
        }
        gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), COMPETENCE[i].nom, ch);
    }

    return combo;
}

void ajouter_pdv(GtkWidget *appel, FenetrePerso * _perso)
{
     GtkWidget * wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"boite_PDV")), *wid1,*wid;
     unsigned short i,j;
     char ch[32];

     i=0;
     do
     {
         sprintf(ch,"pdv_%hu",i);
         wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch);
         i++;
     }
     while(wid1!=NULL);
     i--; /* if faut commencer à 0 */
     wid1=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);

     sprintf(ch,"nv %hd : ",i+1);
     wid=gtk_label_new_with_mnemonic(ch);
     gtk_box_pack_start(GTK_BOX(wid1),wid,TRUE,TRUE,9);

     wid=gtk_spin_button_new_with_range(1,99,1);
     if (_perso->perso.pdv!=NULL)
     {
        for (j=0;_perso->perso.pdv[j]!=-1;j++);
        if (j<=i)
        {
        }
        else
        { /* place déjà reservée : remettre l'ancienne valeur */
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),_perso->perso.pdv[i]);
        }
     }
     else
     { /* nouvelle valeur : rien à initialiser */
     }

     sprintf(ch,"nb_pdv_%hu",i);
     gtk_widget_set_name (wid,ch);
     g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_pdv),_perso);
     sprintf(ch,"pdv_%hu",i);
     gtk_widget_set_name (wid1,ch);
     gtk_box_pack_start(GTK_BOX(wid1),wid,TRUE,TRUE,9);
     gtk_box_pack_start(GTK_BOX(wid_util),wid1,0,0,0);
     modif_pdv(appel,_perso);
     gtk_widget_show_all(wid1);
}

void enlever_pdv(GtkWidget *appel, FenetrePerso * _perso)
{
     GtkWidget * wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"boite_PDV")), *wid1;
     unsigned short i;
     char ch[32];

     i=0;
     do
     {
         i++;
         sprintf(ch,"pdv_%hu",i);
         wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid),ch);
     }
     while(wid1!=NULL);
     if (i>1)
     {
         i--;
         sprintf(ch,"pdv_%hu",i);
         wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid),ch);
         gtk_widget_destroy(wid1);
     }
     else
     { /* on n'enlève pas la dernière case */
     }

     (void)appel;
}


void ajouter_classe(GtkWidget *appel, FenetrePerso * _perso)
{
     GtkWidget * wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes")), *wid2;
     unsigned short i;

     for (i=1;(wid2=gtk_grid_get_child_at (GTK_GRID(wid),1,i))!=NULL;i++);
     i--;
     ajoute_ligne_classe(_perso,i,0,0,0);
     gtk_widget_show_all(wid);
     donne_classe_modif(_perso); /* -1 pour le dernier */
     donne_niv_classe_modif(_perso);
     modif_classe(NULL,_perso);
     modif_XP(NULL,_perso);

     (void)appel;
}

void enlever_classe(GtkWidget *appel, FenetrePerso * _perso)
{
     GtkWidget * wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes")), *wid1, *wid2;
     struct widgets ** fenetre=(struct widgets **)g_malloc(2*sizeof(struct widgets *));
     char ch[8];
     unsigned short i,j;

     for (i=1;(wid2=gtk_grid_get_child_at (GTK_GRID(wid),1,i))!=NULL;i++);
     if (i>2)
     {
         if ((fenetre[0]=ouverture_glade_retour("boite_supprime.glade",GLADE_NON_CONNECT))!=NULL)
         {
             wid1=GTK_WIDGET(gtk_builder_get_object(fenetre[0]->builder,"boite_ligne"));
             wid2=gtk_label_new_with_mnemonic("Enlever les métiers suivants :");
             gtk_box_pack_start(GTK_BOX(wid1),wid2,1,0,0);
             for (i=1;(wid2=gtk_grid_get_child_at (GTK_GRID(wid),1,i))!=NULL;i++)
             { /* parcours de la seonde colonne de la grille tant qu'il y a un élément */
                 if (GTK_IS_COMBO_BOX_TEXT(wid2))
                 {
                    j=gtk_combo_box_get_active (GTK_COMBO_BOX(wid2));
                    wid2=gtk_check_button_new_with_mnemonic(CLASSE[_perso->combo_classes[j]].nom);
                    sprintf(ch,"%d",j);
                    gtk_widget_set_name (wid2,ch);
                    gtk_box_pack_start(GTK_BOX(wid1),wid2,1,0,0);
                 }
                 else
                 { /* ce n'est pas un combotext => anormal ! */
                 }
             }
             gtk_widget_show_all(wid1);
             fenetre[1]=_perso->modif;
             g_signal_connect (G_OBJECT (gtk_builder_get_object(fenetre[0]->builder,"valide")), "clicked", G_CALLBACK (callback_enlever_classe),fenetre);
             gtk_builder_connect_signals(fenetre[0]->builder,fenetre);
         }
         else
         {
             printf("Erreur lors de l'ouverture du fichier boite_supprime.glade\n");
         }
     }
     else
     { /* on n'enlève pas la seule ligne */
     }

    (void)appel;
}

void callback_enlever_classe(GtkWidget *appel, struct widgets ** fenetre)
{
     GList * gl, *gl1;
     GtkWidget * wid=GTK_WIDGET(gtk_builder_get_object(fenetre[0]->builder,"boite_ligne"));
     GtkWidget * wid_perso=GTK_WIDGET(gtk_builder_get_object(fenetre[1]->builder,"grille_classes"));
     GtkWidget * wid1;
     unsigned short cl,i=1,drapo=0;
     char ch[32];

     gl1=gtk_container_get_children(GTK_CONTAINER(wid));
     for (gl=gl1;gl != NULL; gl= g_list_next(gl))
     {
         if (GTK_IS_TOGGLE_BUTTON(gl->data) && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gl->data)))
         {
             sscanf((char *)gtk_widget_get_name(GTK_WIDGET(gl->data)),"%hu",&cl);
             drapo=0;
             for (i=1;(wid1=gtk_grid_get_child_at (GTK_GRID(wid_perso),1,i))!=NULL && drapo==0;i++)
             { /* parcours de la seonde colonne de la grille tant qu'il y a un élément */
                 if (GTK_IS_COMBO_BOX_TEXT(wid1) && gtk_combo_box_get_active (GTK_COMBO_BOX(wid1))==cl)
                 {
                     gtk_grid_remove_row (GTK_GRID(wid_perso),i);
                     drapo=1;
                 }
                 else
                 {  /* pas la bonne ligne */
                 }
             } /* on vient d'enlever la ligne i+1 : la ligne i est l'ancienne i+1, indice i */
             for (i--;(wid1=gtk_grid_get_child_at (GTK_GRID(wid_perso),1,i))!=NULL;i++)
             { /* ON EN A DÉJÀ ENLEVÉ UN => IL FAUT DÉCALER LES NOMS DES AUTRES */
                sprintf(ch,"classe_%hu_l1",i);
                wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
                sprintf(ch,"classe_%hu_l1",i-1);
                gtk_widget_set_name (wid1,ch);
                sprintf(ch,"classe_%hu_l2",i);
                wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
                sprintf(ch,"classe_%hu_l2",i-1);
                gtk_widget_set_name (wid1,ch);
                sprintf(ch,"classe_%hu",i);
                wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
                sprintf(ch,"classe_%hu",i-1);
                gtk_widget_set_name (wid1,ch);
                sprintf(ch,"niveau_%hu",i);
                wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
                sprintf(ch,"niveau_%hu",i-1);
                gtk_widget_set_name (wid1,ch);
                sprintf(ch,"XP_%hu",i);
                wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
                sprintf(ch,"XP_%hu",i-1);
                gtk_widget_set_name (wid1,ch);
             }
         }
         else
         { /* ce n'est pas une case à cocher */
         }
     }
     g_list_free(gl1);
     ferme_glade(appel,fenetre[0]);
     wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),"classe_0");
     cl=gtk_combo_box_get_active(GTK_COMBO_BOX(wid));
     gtk_combo_box_set_active (GTK_COMBO_BOX(wid),(cl+1)%i);
     gtk_combo_box_set_active (GTK_COMBO_BOX(wid),cl);
     g_free(fenetre);
}

void ajoute_ligne_classe(FenetrePerso * _perso,unsigned short place,unsigned short classe,unsigned short niveau,unsigned long XP)
{
    GtkWidget * wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes"));
    GtkWidget * wid;
    GtkComboBox * combo;
    char ch[LONG];

    sprintf(ch,"classe_%hu_l1",place);
    wid=gtk_label_new_with_mnemonic(" - ");
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,0,place+1,1,1);

    sprintf(ch,"classe_%hu_l2",place);
    wid=gtk_label_new_with_mnemonic(" : ");
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,2,place+1,1,1);

    combo=creation_combo_classe();
    sprintf(ch,"classe_%hu",place);
    gtk_widget_set_name (GTK_WIDGET(combo),ch);
    gtk_grid_attach (GTK_GRID(wid_util),GTK_WIDGET(combo),1,place+1,1,1);
    calcul_couleur_classe(_perso);
    gtk_combo_box_set_active (combo,classe_nb(classe,_perso->combo_classes));
    callback_color_classe_cellrender (combo,_perso);
    g_signal_connect (G_OBJECT (combo), "changed", G_CALLBACK (modif_classe),_perso);


    wid=gtk_spin_button_new_with_range(0,99,1);
    /*gtk_entry_set_width_chars(GTK_ENTRY(wid),1);*/
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),niveau);
    sprintf(ch,"niveau_%hu",place);
    gtk_widget_set_name (wid,ch);
    g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_niv),_perso);
    gtk_grid_attach (GTK_GRID(wid_util),wid,3,place+1,1,1);

    wid=gtk_spin_button_new_with_range(0,99999999,1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),XP);
    sprintf(ch,"XP_%hu",place);
    gtk_widget_set_name (wid,ch);
    g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_XP),_perso);
    gtk_grid_attach (GTK_GRID(wid_util),wid,5,place+1,1,1);

    wid=gtk_label_new_with_mnemonic("-=-");
    sprintf(ch,":_%hu",place);
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,4,place+1,1,1);

    wid=gtk_label_new_with_mnemonic("X");
    sprintf(ch,"nv_max_%hu",place);
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,6,place+1,1,1);

}

void ajoute_ligne_competence(FenetrePerso * _perso,unsigned short place,signed short competence,unsigned short niveau)
{
    GtkWidget * wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_competence"));
    GtkWidget * wid;
    GtkComboBox * combo;
    char ch[LONG];

    sprintf(ch,"competence_%hu_l1",place);
    wid=gtk_label_new_with_mnemonic(" - ");
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,0,place+1,1,1);
    sprintf(ch,"competence_%hu_l2",place);
    wid=gtk_label_new_with_mnemonic(" : ");
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,2,place+1,1,1);
    combo=creation_combo_competence();
    sprintf(ch,"competence_%hu",place);
    gtk_widget_set_name (GTK_WIDGET(combo),ch);
    callback_color_competence_cellrender (combo,_perso);
    gtk_grid_attach (GTK_GRID(wid_util),GTK_WIDGET(combo),1,1+place,1,1);
    gtk_combo_box_set_active (combo,competence);
    g_signal_connect (G_OBJECT (combo), "changed", G_CALLBACK (modif_competence),_perso);
    wid=gtk_spin_button_new_with_range(0,99,1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),niveau);
    sprintf(ch,"niv_competence_%hu",place);
    gtk_widget_set_name (wid,ch);
    g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_competence),_perso);
    gtk_grid_attach (GTK_GRID(wid_util),wid,3,1+place,1,1);
}

void ajoute_ligne_arme(FenetrePerso * _perso,unsigned short place,unsigned short arme,unsigned short niveau,signed short magie,signed short autre_t,signed short autre_d)
{
    GtkWidget * wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_armes"));
    GtkWidget * wid;
    GtkComboBox * combo;
    char ch[LONG];

    sprintf(ch,"arme_%hu_l1",place);
    wid=gtk_label_new_with_mnemonic(" - ");
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,0,place+1,1,1);
    sprintf(ch,"arme_%hu_l2",place);
    wid=gtk_label_new_with_mnemonic(" : ");
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,2,place+1,1,1);
    combo=creation_combo_arme();
    sprintf(ch,"arme_%hu",place);
    gtk_widget_set_name (GTK_WIDGET(combo),ch);
    callback_color_arme_cellrender (combo,_perso);
    gtk_grid_attach (GTK_GRID(wid_util),GTK_WIDGET(combo),1,1+place,1,1);
    gtk_combo_box_set_active (combo,arme);
    g_signal_connect (G_OBJECT (combo), "changed", G_CALLBACK (modif_arme),_perso);
    wid=gtk_spin_button_new_with_range(0,3,1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),niveau);
    sprintf(ch,"niv_arme_%hu",place);
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,3,1+place,1,1);
    g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_arme),_perso);
    sprintf(ch,"niv_arme_%hu_l1",place);
    wid=gtk_label_new_with_mnemonic(" - ");
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,4,1+place,1,1);
    sprintf(ch,"autre_t_arme_%hu_l1",place);
    wid=gtk_label_new_with_mnemonic(" - ");
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,6,1+place,1,1);
    sprintf(ch,"autre_d_arme_%hu_l1",place);
    wid=gtk_label_new_with_mnemonic(" - ");
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,8,1+place,1,1);
    wid=gtk_spin_button_new_with_range(-9,+9,1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),magie);
    g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_active),_perso);
    sprintf(ch,"mag_arme_%hu",place);
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,5,1+place,1,1);

    wid=gtk_spin_button_new_with_range(-9,+9,1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),autre_t);
    g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_active),_perso);
    sprintf(ch,"arme_autre_t_%hu",place);
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,7,1+place,1,1);
    wid=gtk_spin_button_new_with_range(-9,+9,1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),autre_d);
    g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_active),_perso);
    sprintf(ch,"arme_autre_d_%hu",place);
    gtk_widget_set_name (wid,ch);
    gtk_grid_attach (GTK_GRID(wid_util),wid,9,1+place,1,1);
}

void callback_enlever_classe_ferme(GtkWidget *appel, struct widgets ** fenetre)
{
    ferme_glade(appel,fenetre[0]);
}

void ajouter_arme(GtkWidget *appel, FenetrePerso * _perso)
{
     GtkWidget * wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_armes")), *wid2;
     unsigned short i;

     for (i=1;(wid2=gtk_grid_get_child_at (GTK_GRID(wid),1,i))!=NULL;i++);
     i--;
     ajoute_ligne_arme(_perso,i,0,0,0,0,0);
     gtk_widget_show_all(wid);
     modif_arme(NULL,_perso);

    (void)appel;
}

void enlever_arme(GtkWidget *appel, FenetrePerso * _perso)
{
     GtkWidget * wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_armes")), *wid1, *wid2;
     struct widgets ** fenetre=(struct widgets **)g_malloc(2*sizeof(struct widgets *));
     char ch[8];
     unsigned short i,j;

     for (i=1;(wid2=gtk_grid_get_child_at (GTK_GRID(wid),1,i))!=NULL;i++);
     if (i>2)
     {
         fenetre[0]=ouverture_glade_retour("boite_supprime.glade",GLADE_NON_CONNECT);
         wid1=GTK_WIDGET(gtk_builder_get_object(fenetre[0]->builder,"boite_ligne"));
         wid2=gtk_label_new_with_mnemonic("Enlever les armes suivantes :");
         gtk_box_pack_start(GTK_BOX(wid1),wid2,1,0,0);
         for (i=1;(wid2=gtk_grid_get_child_at (GTK_GRID(wid),1,i))!=NULL;i++)
         { /* parcours de la seonde colonne de la grille tant qu'il y a un élément */
             if (GTK_IS_COMBO_BOX_TEXT(wid2))
             {
                j=gtk_combo_box_get_active (GTK_COMBO_BOX(wid2));
                wid2=gtk_check_button_new_with_mnemonic(ARME[j].nom);
                sprintf(ch,"%d",j);
                gtk_widget_set_name (wid2,ch);
                gtk_box_pack_start(GTK_BOX(wid1),wid2,1,0,0);
             }
             else
             { /* ce n'est pas un combotext => anormal ! */
                printf("La ligne %hu est %s au lieu d'être un COMBO_TEXT_BOX\n",i,G_OBJECT_TYPE_NAME(G_OBJECT(wid2)));
             }
         }
         gtk_widget_show_all(wid1);
         fenetre[1]=_perso->modif;
         g_signal_connect (G_OBJECT (gtk_builder_get_object(fenetre[0]->builder,"valide")), "clicked", G_CALLBACK (callback_enlever_arme),fenetre);
         gtk_builder_connect_signals(fenetre[0]->builder,fenetre);
     }
     else
     { /* on n'enlève pas la seule ligne */
     }

    (void)appel;
}

void callback_enlever_arme(GtkWidget *appel, struct widgets ** fenetre)
{
     GList * gl, *gl1;
     GtkWidget * wid=GTK_WIDGET(gtk_builder_get_object(fenetre[0]->builder,"boite_ligne"));
     GtkWidget * wid_perso=GTK_WIDGET(gtk_builder_get_object(fenetre[1]->builder,"grille_armes"));
     GtkWidget * wid1;
     unsigned short cl,i=1,drapo=0;

     gl1=gtk_container_get_children(GTK_CONTAINER(wid));
     for (gl=gl1;gl != NULL; gl= g_list_next(gl))
     {
         if (GTK_IS_TOGGLE_BUTTON(gl->data) && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gl->data)))
         {
             sscanf((char *)gtk_widget_get_name(GTK_WIDGET(gl->data)),"%hu",&cl);
             drapo=0;
             for (i=1;(wid1=gtk_grid_get_child_at (GTK_GRID(wid_perso),1,i))!=NULL && drapo==0;i++)
             { /* parcours de la seconde colonne de la grille tant qu'il y a un élément */
                 if (GTK_IS_COMBO_BOX_TEXT(wid1) && gtk_combo_box_get_active (GTK_COMBO_BOX(wid1))==cl)
                 {
                     enleve_ligne_arme(fenetre[1],i);
                     drapo=1;
                 }
                 else
                 {  /* pas la bonne ligne */
                 }
             }
         }
         else
         { /* ce n'est pas une case à cocher */
         }
     }
     g_list_free(gl1);
     ferme_glade(appel,fenetre[0]);
     wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),"arme_0");
     cl=gtk_combo_box_get_active(GTK_COMBO_BOX(wid));
     gtk_combo_box_set_active (GTK_COMBO_BOX(wid),(cl+1)%i);
     gtk_combo_box_set_active (GTK_COMBO_BOX(wid),cl);
     g_free(fenetre);
}

void enleve_ligne_arme(struct widgets * fenetre, signed short ligne)
{
    GtkWidget * wid_perso=GTK_WIDGET(gtk_builder_get_object(fenetre->builder,"grille_armes"));
    GtkWidget * wid1;
    char ch[32];

    gtk_grid_remove_row (GTK_GRID(wid_perso),ligne);
    /* on vient d'enlever la ligne i+1 : la ligne i est l'ancienne i+1, indice i */
    for (;(wid1=gtk_grid_get_child_at (GTK_GRID(wid_perso),1,ligne))!=NULL;ligne++)
    { /* ON EN A DÉJÀ ENLEVÉ UN => IL FAUT DÉCALER LES NOMS DES AUTRES */
        sprintf(ch,"arme_%hu_l1",ligne);
        wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
        sprintf(ch,"arme_%hu_l1",ligne-1);
        gtk_widget_set_name (wid1,ch);
        sprintf(ch,"arme_%hu_l2",ligne);
        wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
        sprintf(ch,"arme_%hu_l2",ligne-1);
        gtk_widget_set_name (wid1,ch);
        sprintf(ch,"arme_%hu",ligne);
        wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
        sprintf(ch,"arme_%hu",ligne-1);
        gtk_widget_set_name (wid1,ch);
        sprintf(ch,"niv_arme_%hu",ligne);
        wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
        sprintf(ch,"niv_arme_%hu",ligne-1);
        gtk_widget_set_name (wid1,ch);
        sprintf(ch,"mag_arme_%hu",ligne);
        wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
        sprintf(ch,"mag_arme_%hu",ligne-1);
        gtk_widget_set_name (wid1,ch);

        sprintf(ch,"arme_autre_t_%hu",ligne);
        wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
        sprintf(ch,"arme_autre_t_%hu",ligne-1);
        gtk_widget_set_name (wid1,ch);
        sprintf(ch,"arme_autre_d_%hu",ligne);
        wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
        sprintf(ch,"arme_autre_d_%hu",ligne-1);
        gtk_widget_set_name (wid1,ch);
    }

}

void ajouter_competence(GtkWidget *appel, FenetrePerso * _perso)
{
     GtkWidget * wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_competence")), *wid2;
     unsigned short i;

     for (i=1;(wid2=gtk_grid_get_child_at (GTK_GRID(wid),1,i))!=NULL;i++);
     i--;
     ajoute_ligne_competence(_perso,i,0,0);
     gtk_widget_show_all(wid);
     modif_competence(NULL,_perso);

    (void)appel;
}

void enlever_competence(GtkWidget *appel, FenetrePerso * _perso)
{
     GtkWidget * wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_competence")), *wid1, *wid2;
     struct widgets ** fenetre=(struct widgets **)g_malloc(2*sizeof(struct widgets *));
     char ch[8];
     unsigned short i,j;

     for (i=0;(wid2=gtk_grid_get_child_at (GTK_GRID(wid),1,i))!=NULL;i++);
     if (i>1)
     {
         fenetre[0]=ouverture_glade_retour("boite_supprime.glade",GLADE_NON_CONNECT);
         wid1=GTK_WIDGET(gtk_builder_get_object(fenetre[0]->builder,"boite_ligne"));
         wid2=gtk_label_new_with_mnemonic("Enlever les competences suivantes :");
         gtk_box_pack_start(GTK_BOX(wid1),wid2,1,0,0);
         for (i=1;(wid2=gtk_grid_get_child_at (GTK_GRID(wid),1,i))!=NULL;i++)
         { /* parcours de la seconde colonne de la grille tant qu'il y a un élément */
             if (GTK_IS_COMBO_BOX_TEXT(wid2))
             {
                j=gtk_combo_box_get_active (GTK_COMBO_BOX(wid2));
                wid2=gtk_check_button_new_with_mnemonic(COMPETENCE[j].nom);
                sprintf(ch,"%d",j);
                gtk_widget_set_name (wid2,ch);
                gtk_box_pack_start(GTK_BOX(wid1),wid2,1,0,0);
             }
             else
             { /* ce n'est pas un combotext => anormal ! */
                printf("La ligne %hu est %s au lieu d'être un COMBO_TEXT_BOX\n",i,G_OBJECT_TYPE_NAME(G_OBJECT(wid2)));
             }
         }
         gtk_widget_show_all(wid1);
         fenetre[1]=_perso->modif;
         g_signal_connect (G_OBJECT (gtk_builder_get_object(fenetre[0]->builder,"valide")), "clicked", G_CALLBACK (callback_enlever_competence),fenetre);
         gtk_builder_connect_signals(fenetre[0]->builder,fenetre);
     }
     else
     { /* pas de ligne ! */
     }

    (void)appel;
}

void callback_enlever_competence(GtkWidget *appel, struct widgets ** fenetre)
{
     GList * gl, *gl1;
     GtkWidget * wid=GTK_WIDGET(gtk_builder_get_object(fenetre[0]->builder,"boite_ligne"));
     GtkWidget * wid_perso=GTK_WIDGET(gtk_builder_get_object(fenetre[1]->builder,"grille_competence"));
     GtkWidget * wid1;
     unsigned short cl,i=1,drapo=0;

     gl1=gtk_container_get_children(GTK_CONTAINER(wid));
     for (gl=gl1;gl != NULL; gl= g_list_next(gl))
     {
         if (GTK_IS_TOGGLE_BUTTON(gl->data) && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gl->data)))
         {
             sscanf((char *)gtk_widget_get_name(GTK_WIDGET(gl->data)),"%hu",&cl);
             drapo=0;
             for (i=1;(wid1=gtk_grid_get_child_at (GTK_GRID(wid_perso),1,i))!=NULL && drapo==0;i++)
             { /* parcours de la seconde colonne de la grille tant qu'il y a un élément */
                 if (GTK_IS_COMBO_BOX_TEXT(wid1) && gtk_combo_box_get_active (GTK_COMBO_BOX(wid1))==cl)
                 {
                     enleve_ligne_competence(fenetre[1],i);
                     drapo=1;
                 }
                 else
                 {  /* pas la bonne ligne */
                 }
             }
         }
         else
         { /* ce n'est pas une case à cocher */
         }
     }
     g_list_free(gl1);
     ferme_glade(appel,fenetre[0]);
     wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),"competence_0");
     if (wid!=NULL)
     {
         cl=gtk_combo_box_get_active(GTK_COMBO_BOX(wid));
         gtk_combo_box_set_active (GTK_COMBO_BOX(wid),(cl+1)%i);
         gtk_combo_box_set_active (GTK_COMBO_BOX(wid),cl);
     }
     else
     {
         /* wid n'existe pas => pas d'entrée compétence */
     }
     g_free(fenetre);
}

void enleve_ligne_competence(struct widgets * fenetre, signed short ligne)
{
    GtkWidget * wid_perso=GTK_WIDGET(gtk_builder_get_object(fenetre->builder,"grille_competence"));
    GtkWidget * wid1;
    char ch[32];

    gtk_grid_remove_row (GTK_GRID(wid_perso),ligne);
    /* on vient d'enlever la ligne i+1 : la ligne i est l'ancienne i+1, indice i */
    for (;(wid1=gtk_grid_get_child_at (GTK_GRID(wid_perso),1,ligne))!=NULL;ligne++)
    { /* ON EN A DÉJÀ ENLEVÉ UN => IL FAUT DÉCALER LES NOMS DES AUTRES */
        sprintf(ch,"competence_%hu_l1",ligne);
        wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
        sprintf(ch,"competence_%hu_l1",ligne-1);
        gtk_widget_set_name (wid1,ch);
        sprintf(ch,"competence_%hu_l2",ligne);
        wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
        sprintf(ch,"competence_%hu_l2",ligne-1);
        gtk_widget_set_name (wid1,ch);
        sprintf(ch,"competence_%hu",ligne);
        wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
        sprintf(ch,"competence_%hu",ligne-1);
        gtk_widget_set_name (wid1,ch);
        sprintf(ch,"niv_competence_%hu",ligne);
        wid1=gtk_get_widget_by_name(GTK_CONTAINER(wid_perso),ch);
        sprintf(ch,"niv_competence_%hu",ligne-1);
        gtk_widget_set_name (wid1,ch);
    }

}
