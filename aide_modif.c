#define F_COMPETENCE    1
#define F_CLASSE        2
#define F_ARME          3

#include <string.h>
#include "include.h"

void tue_aide_modif(GtkWidget *appel, aide * info_fenetre);
void reveil_aide_modif(GtkWidget *appel, aide * info_fenetre);
void init_aide_modif(GtkWidget *appel, aide * info_fenetre);
void valide_aide_modif(GtkWidget *appel, aide * info_fenetre);

void aide_modif_competence(GtkWidget *appel, aide * info_fenetre);
void tableau_competences(GtkWidget *appel, FenetrePerso * _perso);

void aide_modif_classe(GtkWidget *appel, aide * info_fenetre);
void tableau_classe(GtkWidget *appel, FenetrePerso * _perso);

void aide_modif_arme(GtkWidget *appel, aide * info_fenetre);
void tableau_arme(GtkWidget *appel, FenetrePerso * _perso);

signed short _drap=0; /* permet de bloquer les callback quand il est à 1 */

void tableau_competences(GtkWidget *appel, FenetrePerso * _perso)
{
    unsigned short dimension,i,j;
    gint x,y;
    GtkWidget * grille,* wid, * wid2;
    unsigned short * pt_competence;
    char ch[64];

    aide * info_fenetre=(aide *)g_malloc(sizeof(aide));

    info_fenetre->_perso=_perso;
    gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"window")),FALSE);
    info_fenetre->info=F_COMPETENCE;
    info_fenetre->modif=ouverture_glade_retour("info_modif.glade",GLADE_NON_CONNECT);

    dimension=(NB_COMPETENCE-1)/4+1; /* pour avoir celui juste en dessus avec les arondis entiers */
    grille=gtk_grid_new();

    for(i=0;i<dimension;i++) /* colonne */
    {
        for (j=0;j<4;j++) /*ligne */
        {
            if (i+dimension*j<NB_COMPETENCE)
            { /* on n'a pas dépassé la taille */
                wid=gtk_check_button_new();
                gtk_grid_attach(GTK_GRID(grille),wid,j*4,i,1,1);
                sprintf(ch,"ch_%hu",i+dimension*j);
                gtk_widget_set_name (wid,ch);
                couleur_item(wid,_perso->couleur_competence[i+dimension*j]);
                wid=gtk_spin_button_new_with_range(0,99,1);
                pt_competence=(unsigned short *)g_malloc(sizeof(unsigned short));
                *pt_competence=i+dimension*j;
                g_object_set_data(G_OBJECT(wid), "competence",pt_competence);
                sprintf(ch,"sp_%hu",i+dimension*j);
                gtk_widget_set_name (wid,ch);
                gtk_grid_attach(GTK_GRID(grille),wid,j*4+1,i,1,1);
                g_signal_connect (G_OBJECT (wid), "changed", G_CALLBACK (aide_modif_competence),info_fenetre);
                wid=gtk_label_new_with_mnemonic(COMPETENCE[i+dimension*j].nom);
                gtk_widget_set_halign(wid,GTK_ALIGN_START);
                wid2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                gtk_widget_set_hexpand(wid2,TRUE);
                gtk_box_pack_start (GTK_BOX(wid2),wid,FALSE,FALSE,0);
                couleur_item(wid2,_perso->couleur_competence[i+dimension*j]);
                gtk_grid_attach(GTK_GRID(grille),wid2,j*4+2,i,1,1);
                wid=gtk_separator_new(GTK_ORIENTATION_VERTICAL);
                gtk_grid_attach(GTK_GRID(grille),wid,j*4+3,i,1,1);
            }
            else
            { /* les cases en trop */
            }
        }
    }
    wid=gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(wid) ,GTK_WIDGET(grille));
    gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(info_fenetre->modif->builder,"v_box")),wid,TRUE,TRUE,0);
    init_aide_modif(NULL,info_fenetre);
    gtk_widget_show_all(wid);
    wid=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"window"));
    sprintf(ch,"Compétences de ");
    strncat(ch,_perso->perso.nom,63);
    gtk_window_set_title(GTK_WINDOW(wid),ch);
    gtk_window_get_size (GTK_WINDOW(wid),&x,&y);
    gtk_window_resize (GTK_WINDOW(wid),0+x,500+y);
    gtk_builder_connect_signals(info_fenetre->modif->builder,info_fenetre);

    (void)appel;
}

void tableau_arme(GtkWidget *appel, FenetrePerso * _perso)
{
    unsigned short dimension,i,j;
    gint x,y;
    GtkWidget * grille,* wid, * wid2;
    unsigned short * pt_arme;
    char ch[128];

    aide * info_fenetre=(aide *)g_malloc(sizeof(aide));

    info_fenetre->_perso=_perso;
    gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"window")),FALSE);
    info_fenetre->info=F_ARME;
    info_fenetre->modif=ouverture_glade_retour("info_modif.glade",GLADE_NON_CONNECT);

    dimension=(NB_ARME-1)/4+1; /* pour avoir celui juste en dessus avec les arondis entiers */
    grille=gtk_grid_new();
    gtk_widget_set_name (grille,"grille");

    for(i=0;i<dimension;i++) /* colonne */
    {
        for (j=0;j<4;j++) /*ligne */
        {
            if (i+dimension*j<NB_ARME)
            { /* on n'a pas dépassé la taille */
                wid=gtk_check_button_new();
                gtk_grid_attach(GTK_GRID(grille),wid,j*6,i,1,1);
                sprintf(ch,"ch_%hu",i+dimension*j);
                gtk_widget_set_name (wid,ch);
                couleur_item(wid,_perso->couleur_armes[i+dimension*j]);
                wid=gtk_spin_button_new_with_range(0,3,1);
                pt_arme=(unsigned short *)g_malloc(sizeof(unsigned short));
                *pt_arme=i+dimension*j;
                g_object_set_data(G_OBJECT(wid), "arme",pt_arme);
                sprintf(ch,"sp_niv_%hu",i+dimension*j);
                gtk_widget_set_name (wid,ch);
                gtk_grid_attach(GTK_GRID(grille),wid,j*6+2,i,1,1);
                g_signal_connect (G_OBJECT (wid), "changed", G_CALLBACK (aide_modif_arme),info_fenetre);
                wid=gtk_spin_button_new_with_range(-9,9,1);
                pt_arme=(unsigned short *)g_malloc(sizeof(unsigned short));
                *pt_arme=i+dimension*j;
                g_object_set_data(G_OBJECT(wid), "arme",pt_arme);
                sprintf(ch,"sp_mag_%hu",i+dimension*j);
                gtk_widget_set_name (wid,ch);
                gtk_grid_attach(GTK_GRID(grille),wid,j*6+3,i,1,1);
                g_signal_connect (G_OBJECT (wid), "changed", G_CALLBACK (aide_modif_arme),info_fenetre);
                wid=gtk_label_new_with_mnemonic(ARME[i+dimension*j].nom);
                gtk_widget_set_halign(wid,GTK_ALIGN_START);
                wid2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                gtk_widget_set_hexpand(wid2,TRUE);
                gtk_box_pack_start (GTK_BOX(wid2),wid,FALSE,FALSE,0);
                couleur_item(wid2,_perso->couleur_armes[i+dimension*j]);
                gtk_grid_attach(GTK_GRID(grille),wid2,j*6+1,i,1,1);
                wid=gtk_label_new_with_mnemonic(" ");
                gtk_widget_set_halign(wid,GTK_ALIGN_CENTER);
                wid2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                gtk_widget_set_hexpand(wid2,TRUE);
                gtk_box_pack_start (GTK_BOX(wid2),wid,FALSE,FALSE,0);
                couleur_item(wid2,_perso->couleur_armes[i+dimension*j]);
                gtk_grid_attach(GTK_GRID(grille),wid2,j*6+4,i,1,1);
                wid=gtk_separator_new(GTK_ORIENTATION_VERTICAL);
                gtk_grid_attach(GTK_GRID(grille),wid,j*6+5,i,1,1);
            }
            else
            { /* les cases en trop */
            }
        }
    }
    wid=gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(wid) ,GTK_WIDGET(grille));
    gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(info_fenetre->modif->builder,"v_box")),wid,TRUE,TRUE,0);
    init_aide_modif(NULL,info_fenetre);
    gtk_widget_show_all(wid);
    wid=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"window"));
    sprintf(ch,"Armes (nom/niveau/magie) de ");
    strncat(ch,_perso->perso.nom,127);
    gtk_window_set_title(GTK_WINDOW(wid),ch);
    gtk_window_get_size (GTK_WINDOW(wid),&x,&y);
    gtk_window_resize (GTK_WINDOW(wid),0+x,500+y);
    gtk_builder_connect_signals(info_fenetre->modif->builder,info_fenetre);

    (void)appel;
}


void tableau_classe(GtkWidget *appel, FenetrePerso * _perso)
{
    unsigned short dimension,i,j;
    gint x,y;
    GtkWidget * grille,* wid, * wid2;
    unsigned short * pt_classe;
    char ch[128];

    aide * info_fenetre=(aide *)g_malloc(sizeof(aide));

    info_fenetre->_perso=_perso;
    gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"window")),FALSE);
    info_fenetre->info=F_CLASSE;
    info_fenetre->modif=ouverture_glade_retour("info_modif.glade",GLADE_NON_CONNECT);

    dimension=(NB_CLASSE-1)/3+1; /* pour avoir celui juste en dessus avec les arondis entiers */
    grille=gtk_grid_new();

    for(i=0;i<dimension;i++) /* colonne */
    {
        for (j=0;j<3;j++) /*ligne */
        {
            if (i+dimension*j<NB_CLASSE)
            { /* on n'a pas dépassé la taille */
                wid=gtk_check_button_new();
                gtk_grid_attach(GTK_GRID(grille),wid,j*6,i,1,1);
                sprintf(ch,"ch_%hu",i+dimension*j);
                gtk_widget_set_name (wid,ch);
                couleur_item(wid,_perso->couleur_classes[i+dimension*j]);
                wid=gtk_spin_button_new_with_range(0,99,1);
                pt_classe=(unsigned short *)g_malloc(sizeof(unsigned short));
                *pt_classe=i+dimension*j;
                g_object_set_data(G_OBJECT(wid), "classe",pt_classe);
                sprintf(ch,"sp_niv_%hu",i+dimension*j);
                gtk_widget_set_name (wid,ch);
                gtk_grid_attach(GTK_GRID(grille),wid,j*6+2,i,1,1);
                g_signal_connect (G_OBJECT (wid), "changed", G_CALLBACK (aide_modif_classe),info_fenetre);
                wid=gtk_spin_button_new_with_range(0,99999999,1);
                pt_classe=(unsigned short *)g_malloc(sizeof(unsigned short));
                *pt_classe=i+dimension*j;
                g_object_set_data(G_OBJECT(wid), "classe",pt_classe);
                sprintf(ch,"sp_XP_%hu",i+dimension*j);
                gtk_widget_set_name (wid,ch);
                gtk_grid_attach(GTK_GRID(grille),wid,j*6+3,i,1,1);
                g_signal_connect (G_OBJECT (wid), "changed", G_CALLBACK (aide_modif_classe),info_fenetre);
                wid=gtk_label_new_with_mnemonic(CLASSE[i+dimension*j].nom);
                gtk_widget_set_halign(wid,GTK_ALIGN_START);
                wid2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                gtk_widget_set_hexpand(wid2,TRUE);
                gtk_box_pack_start (GTK_BOX(wid2),wid,FALSE,FALSE,0);
                couleur_item(wid2,_perso->couleur_classes[i+dimension*j]);
                gtk_grid_attach(GTK_GRID(grille),wid2,j*6+1,i,1,1);
                wid=gtk_label_new_with_mnemonic(" ");
                gtk_widget_set_halign(wid,GTK_ALIGN_CENTER);
                wid2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                gtk_widget_set_hexpand(wid2,TRUE);
                gtk_box_pack_start (GTK_BOX(wid2),wid,FALSE,FALSE,0);
                couleur_item(wid2,_perso->couleur_classes[i+dimension*j]);
                gtk_grid_attach(GTK_GRID(grille),wid2,j*6+4,i,1,1);
                wid=gtk_separator_new(GTK_ORIENTATION_VERTICAL);
                gtk_grid_attach(GTK_GRID(grille),wid,j*6+5,i,1,1);
            }
            else
            { /* les cases en trop */
            }
        }
    }
    wid=gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(wid) ,GTK_WIDGET(grille));
    gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(info_fenetre->modif->builder,"v_box")),wid,TRUE,TRUE,0);
    init_aide_modif(NULL,info_fenetre);
    gtk_widget_show_all(wid);
    wid=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"window"));
    sprintf(ch,"Classes (nom/niveau/XP) de ");
    strncat(ch,_perso->perso.nom,127);
    gtk_window_set_title(GTK_WINDOW(wid),ch);
    gtk_window_get_size (GTK_WINDOW(wid),&x,&y);
    gtk_window_resize (GTK_WINDOW(wid),0+x,300+y);
    gtk_builder_connect_signals(info_fenetre->modif->builder,info_fenetre);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(info_fenetre->modif->builder,"affiche_info_modif")),"Choix des classes");

    (void)appel;
}

void tue_aide_modif(GtkWidget *appel, aide * info_fenetre)
{
    gtk_window_close(GTK_WINDOW(info_fenetre->modif->window));
    (void)appel;
}

void reveil_aide_modif(GtkWidget *appel, aide * info_fenetre)
{
    gtk_widget_set_sensitive (GTK_WIDGET(gtk_builder_get_object(info_fenetre->_perso->modif->builder,"window")),TRUE);
    g_free(info_fenetre);
    (void)appel;
}

void init_aide_modif(GtkWidget *appel, aide * info_fenetre)
{
    GtkWidget * wid_util, * wid_util1, *wid, * p_dialog;
    char ch[LONG];
    signed short i,niv,l;
    unsigned long j;

    _drap=1;

    switch( info_fenetre->info )
    {
        case F_COMPETENCE :
            wid_util=GTK_WIDGET(gtk_builder_get_object(info_fenetre->_perso->modif->builder,"grille_competence"));
            wid_util1=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"v_box"));
            for (i=0;i<NB_COMPETENCE;i++)
            { /* REMISE À ZÉRO */
                sprintf(ch,"ch_%hd",i);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),FALSE);
                sprintf(ch,"sp_%hd",i);
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),0);
            }
            for(i=0;info_fenetre->_perso->competence_modif[i]!=-1;i++)
            {
                sprintf(ch,"ch_%hd",info_fenetre->_perso->competence_modif[i]);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),TRUE);
                sprintf(ch,"niv_competence_%hd",i);
                j=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
                sprintf(ch,"sp_%hd",info_fenetre->_perso->competence_modif[i]);
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),j);
            }
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(info_fenetre->modif->builder,"affiche_info_modif")),gtk_label_get_text(GTK_LABEL(gtk_builder_get_object(info_fenetre->_perso->modif->builder,"nb_competences"))));
            break;
        case F_CLASSE :
            wid_util=GTK_WIDGET(gtk_builder_get_object(info_fenetre->_perso->modif->builder,"grille_classes"));
            wid_util1=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"v_box"));
            for(i=0;i<NB_CLASSE;i++)
            { /* REMISE À ZÉRO */
                sprintf(ch,"ch_%hd",i);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),FALSE);
                sprintf(ch,"sp_niv_%hu",i);
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),0);
                sprintf(ch,"sp_XP_%hu",i);
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),0);
            }
            for(i=0;info_fenetre->_perso->classe_modif[i]!=-1;i++)
            {
                sprintf(ch,"ch_%hd",info_fenetre->_perso->classe_modif[i]);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),TRUE);
                sprintf(ch,"sp_niv_%hu",info_fenetre->_perso->classe_modif[i]);
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),info_fenetre->_perso->niv_classe_modif[i]);
                sprintf(ch,"sp_XP_%hu",info_fenetre->_perso->classe_modif[i]);
                j=gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_grid_get_child_at (GTK_GRID(wid_util),5,i+1)));
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),j);
            }
            break;
        case F_ARME :
            wid_util=GTK_WIDGET(gtk_builder_get_object(info_fenetre->_perso->modif->builder,"grille_armes"));
            wid_util1=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"v_box"));
            for(i=0;i<NB_ARME;i++)
            { /* REMISE À ZÉRO */
                sprintf(ch,"ch_%hd",i);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch);
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),FALSE);
                sprintf(ch,"sp_niv_%hu",i);
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),0);
                sprintf(ch,"sp_mag_%hu",i);
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),0);
            }
            for(i=0;info_fenetre->_perso->armes_modif[i]!=-1;i++)
            {
                sprintf(ch,"ch_%hd",info_fenetre->_perso->armes_modif[i]);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch);
                sprintf(ch,"mag_arme_%hd",i);
                niv=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid)))
                { /* bouton déjà activé */
                    sprintf(ch,"Gestion du doublon %s mag +%hd",ARME[info_fenetre->_perso->armes_modif[i]].nom,niv);
                    p_dialog = gtk_dialog_new_with_buttons (ch,
                                                              GTK_WINDOW(info_fenetre->modif->window),
                                                              GTK_DIALOG_MODAL,
                                                              "Annuler l'ouverture de\nla fenêtre de choix d'armes", 4,
                                                              "Prendre en compte\ncette arme-ci",1,
                                                              "Prendre en compte\nla meilleure arme",2,
                                                              "Abandonner\ncette entrée",3,
                                                              NULL);
                    l=gtk_dialog_run(GTK_DIALOG(p_dialog));
                    gtk_widget_destroy (p_dialog);
                } /* 0 : première fois, 1 seconde fois et 2 on prend en compte la magie la plus élevé, 3 on abandonne cette entrée, 4 on ne passe pas par là pour ne pas perdre la configuration actuelle */
                else
                { /* le bouton n'était pas activé */
                    l=0;
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),TRUE);
                }
                if (l==0 || l==1 || l==2)
                { /* prise en compte de l'état actuel pour la magie */
                    sprintf(ch,"sp_mag_%hu",info_fenetre->_perso->armes_modif[i]);
                    wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch);
                    if (l==2) /* uniquement si c'est le plus élevé */
                    {
                        niv=max(niv,gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid)));
                    }
                    else
                    { /* déjà pris en compte */
                    }
                    gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),niv);
                }
                else if (l==3)
                { /* on continue comme si cette entrée n'existait pas */
                }
                else /* l==4 */
                { /* on n'affiche pas cette fenêtre de choix d'armes*/
                    tue_aide_modif(NULL,info_fenetre);
                }
                if (l==0) /* en dernier pour préserver le niv au niveau de magie */
                {   /* activation de l'arme */
                    /*mettre le niveau de maîtrise qui est obligatoirement le même s'il y en a plusieurs */
                    sprintf(ch,"niv_arme_%hd",i);
                    niv=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
                    sprintf(ch,"sp_niv_%hu",info_fenetre->_perso->armes_modif[i]);
                    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),niv);
                }
                else
                { /* arme déjà rentrée */
                }

            }
            break;
        default :
            printf("init_aide_modif : cas non pris en compte\n");
    }
    _drap=0;

    (void) appel;
}

void valide_aide_modif(GtkWidget *appel, aide * info_fenetre)
{
    GtkWidget * wid_util, * wid_util1, *wid;
    char ch[32];
    unsigned short i,k,niv;


    switch( info_fenetre->info )
    {
        case F_COMPETENCE :
            wid_util=GTK_WIDGET(gtk_builder_get_object(info_fenetre->_perso->modif->builder,"grille_competence"));
            /* mise à zéro des compétences */
            vide_widget(wid_util);
            wid_util1=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"v_box"));
            k=0;
            for(i=0;i<NB_COMPETENCE;i++) /* colonne */
            {
                sprintf(ch,"ch_%hu",i);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch);
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid)))
                {
                    sprintf(ch,"sp_%hu",i);
                    ajoute_ligne_competence(info_fenetre->_perso,k,i,gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch))));
                    k++;
                }
                else
                { /* case non activée */
                }
            }
            gtk_widget_show_all(wid_util);
            donne_competence_modif(info_fenetre->_perso);
            verif_competence(info_fenetre->_perso);
            gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(info_fenetre->modif->builder,"affiche_info_modif")),gtk_label_get_text(GTK_LABEL(gtk_builder_get_object(info_fenetre->_perso->modif->builder,"nb_competences"))));
            break;
        case F_CLASSE :
            wid_util=GTK_WIDGET(gtk_builder_get_object(info_fenetre->_perso->modif->builder,"grille_classes"));
            wid_util1=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"v_box"));
            vide_widget(wid_util);
            wid=gtk_label_new_with_mnemonic("Classe");
            gtk_grid_attach (GTK_GRID(wid_util),wid,1,0,1,1);
            wid=gtk_label_new_with_mnemonic("Niveau");
            gtk_grid_attach (GTK_GRID(wid_util),wid,3,0,1,1);
            wid=gtk_label_new_with_mnemonic("point d'expérience");
            gtk_grid_attach (GTK_GRID(wid_util),wid,5,0,1,1);
            i=0; /* décompte du nombre de classes affichées */
            for(k=0;k<NB_CLASSE;k++)
            {
                sprintf(ch,"ch_%hu",k);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch);
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid)))
                {
                    sprintf(ch,"sp_niv_%hu",k);
                    niv=gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)));
                    sprintf(ch,"sp_XP_%hu",k);
                    ajoute_ligne_classe(info_fenetre->_perso,i,k,niv,gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch))));
                    i++;
                }
                else
                { /* classe non sélectionnée */
                }
            }
            if (i==0)
            { /* cas anormal ou début vierge : pas de classe sélectionnée */
                ajoute_ligne_classe(info_fenetre->_perso,0,0,0,0);
            }
            else
            { /* il y a déjà au moins une ligne => pas besoin d'en rajouter */
            }
            gtk_widget_show_all(wid_util);
            donne_classe_modif(info_fenetre->_perso);
            donne_niv_classe_modif(info_fenetre->_perso);
            break;
        case F_ARME :
            wid_util=GTK_WIDGET(gtk_builder_get_object(info_fenetre->_perso->modif->builder,"grille_armes"));
            wid_util1=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"v_box"));
            vide_widget(wid_util);
            wid=gtk_label_new_with_mnemonic("Arme");
            gtk_grid_attach (GTK_GRID(wid_util),wid,1,0,1,1);
            wid=gtk_label_new_with_mnemonic("Maîtrise");
            gtk_grid_attach (GTK_GRID(wid_util),wid,3,0,1,1);
            wid=gtk_label_new_with_mnemonic("Magie");
            gtk_grid_attach (GTK_GRID(wid_util),wid,5,0,1,1);
            i=0; /* décompte du nombre de classes affichées */
            for(k=0;k<NB_ARME;k++)
            {
                sprintf(ch,"ch_%hu",k);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch);
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid)))
                {
                    sprintf(ch,"sp_niv_%hu",k);
                    niv=gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)));
                    sprintf(ch,"sp_mag_%hu",k);
                    ajoute_ligne_arme(info_fenetre->_perso,i,k,niv,gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch))),0,0);
                    i++;
                }
                else
                { /* arme non sélectionnée */
                }
            }
            gtk_widget_show_all(wid_util);
            donne_arme_modif(info_fenetre->_perso);
            break;
        default :
            printf("init_aide_modif : cas non pris en compte\n");
    }

    (void) appel;
}

void aide_modif_competence(GtkWidget *appel, aide * info_fenetre)
{
    unsigned short val_competence;
    char ch[32];
    GtkWidget * wid_util1;

    if (_drap==0)
    {
        wid_util1=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"v_box"));
        val_competence= *((unsigned short *) g_object_get_data(G_OBJECT(appel), "competence" ));
        sprintf(ch,"sp_%hd",val_competence); /* activation/déactivation de la compétence si son niveau n'est pas/est 0. */
        if (gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)))!=0)
        { /* entré inutile */
            sprintf(ch,"ch_%hu",val_competence);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),TRUE);
        }
        else
        {
            sprintf(ch,"ch_%hu",val_competence);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),FALSE);
        }
    }
    else
    { /* appel bloqué : init en cours */
    }
}

void aide_modif_classe(GtkWidget *appel, aide * info_fenetre)
{
    unsigned short val_classe;
    char ch[32];
    signed short niv;
    unsigned long XP;
    GtkWidget * wid_util1;

    if (_drap==0)
    {
        wid_util1=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"v_box"));
        val_classe= *((unsigned short *) g_object_get_data(G_OBJECT(appel), "classe" ));
        sprintf(ch,"sp_niv_%hd",val_classe); /* activation/déactivation de la compétence si son niveau n'est pas/est 0. */
        niv=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)));
        sprintf(ch,"sp_XP_%hd",val_classe); /* activation/déactivation de la compétence si son niveau n'est pas/est 0. */
        XP=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)));
        sprintf(ch,"ch_%hu",val_classe);
        if ((niv+XP)>0)
        {  /* entrée inutile */
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),TRUE);
        }
        else
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),FALSE);
        }
    }
    else
    { /* appel bloqué : init en cours */
    }
}

void aide_modif_arme(GtkWidget *appel, aide * info_fenetre)
{
    unsigned short val_arme;
    char ch[32];
    signed short niv;
    signed short mag;
    GtkWidget * wid_util1;

    if (_drap==0)
    {
        wid_util1=GTK_WIDGET(gtk_builder_get_object(info_fenetre->modif->builder,"v_box"));
        val_arme= *((unsigned short *) g_object_get_data(G_OBJECT(appel), "arme" ));
        sprintf(ch,"sp_niv_%hd",val_arme); /* activation/déactivation de la compétence si son niveau n'est pas/est 0. */
        niv=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)));
        sprintf(ch,"sp_mag_%hd",val_arme); /* activation/déactivation de la compétence si son niveau n'est pas/est 0. */
        mag=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)));
        sprintf(ch,"ch_%hu",val_arme);
        if (niv==0 && mag==0)
        { /* entrée inutile
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(info_fenetre->modif->builder,"v_box")),ch)),FALSE); */
        }
        else
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util1),ch)),TRUE);
        }
    }
    else
    { /* appel bloqué : init en cours */
    }
}
