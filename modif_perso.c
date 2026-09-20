#include "include.h"
#include <string.h>

#define TAILLE_MAX_NOM_SORT 80

unsigned short drap=1; /* permet de lancer les fonctions qui gèrent les modifications */

void ferme_modif(GtkWidget *widget, FenetrePerso * _perso);
void init_fichier_modif(GtkWidget *wid, FenetrePerso * _perso);
void valid_fichier_modif(GtkWidget *appel, FenetrePerso * _perso);
void modif_active_dell(GtkWidget *appel,GtkDeleteType type ,gint count , FenetrePerso * _perso);
void modif_active_ins(GtkWidget *appel, gchar * string, FenetrePerso * _perso);
void modif_perso_mag(GtkWidget *appel, FenetrePerso * _perso);
void modif_perso_psi(GtkWidget *appel, FenetrePerso * _perso);
void modif_perso_aff_mag(FenetrePerso * _perso);
void modif_perso_mag_niv(GtkWidget *appel, FenetrePerso * _perso);
void valid_perso_mag(FenetrePerso * _perso);
void init_mag(FenetrePerso * _perso);
void init_psi(FenetrePerso * _perso);



void modif_perso(GtkWidget * appel, FenetrePerso * _perso)
{
    GtkWidget * wid, * wid_util;
    unsigned short j,k;
    GtkComboBox * combo=NULL;
    char ch[LONG];

    if (_perso->modif==NULL)
    { /* ouverture de la fenêtre */
        drap=0;
        _perso->modif=ouverture_glade_retour("modification.glade",GLADE_NON_CONNECT);
        if (_perso->modif!=NULL)
        {
            if (_perso->perso.nom!=NULL && _perso->perso.joueur!=NULL)
            {
                sprintf(ch,"Modification de %s joué%s par %s",_perso->perso.nom,_perso->perso.sexe==FEMININ?"e":"",_perso->perso.joueur);
            }
            else
            { /* au moins un est nul */
                if (_perso->perso.joueur!=NULL)
                {
                    sprintf(ch,"Modification d'un personnage joué par %s",_perso->perso.joueur);
                }
                else if (_perso->perso.nom!=NULL)
                {
                    sprintf(ch,"Modification de %s",_perso->perso.nom);
                }
                else
                {
                    sprintf(ch,"Modification d'un personnage");
                }
            }
            gtk_window_set_title(GTK_WINDOW(gtk_builder_get_object(_perso->modif->builder,"window")),ch);
            /* origine */

            /*_perso->origine=(char **)malloc(2*sizeof(char *));*/
            _perso->ori_add1_add2=_perso->perso.ori_add1_add2;
            wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"Boite_origine_add1"));
            for( k=0;k<2;k++)
            {
                /*_perso->origine[k]=(char *)malloc(NB_ORIGINE[k]*sizeof(char));*/
                 for(j=0;j<NB_ORIGINE[k];j++)
                 {
                    _perso->origine[k][j]=_perso->perso.origine[k][j];
                    wid=gtk_check_button_new_with_label(_ORIGINE[k][j].nom);
                    sprintf(ch,"ori_%hu_%hu",k,j);
                    gtk_widget_set_name (wid,ch);
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),_perso->origine[k][j]);
                    g_signal_connect (G_OBJECT (wid), "toggled", G_CALLBACK (modif_origine),_perso);
                    gtk_box_pack_start(GTK_BOX(wid_util),wid,0,0,0);
                 }
                 _perso->ori_add1_add2=_perso->perso.ori_add1_add2;
                 wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"sources_add1_add2"));
                 gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),_perso->ori_add1_add2);
                 gtk_widget_show_all(wid_util);
                 if (k==0)
                 {   /* pour le second passage, on prépare l'onglet add2 */
                     wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"Boite_origine_add2"));
                 }
                 else
                 {   /* passage pour k=1 : on sort */
                 }
            }

            /* les caractéristiques */
            for(k=0;k<8;k++)
            {
                wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[k]));
                gtk_spin_button_set_range(GTK_SPIN_BUTTON(wid),0,25);
                wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[k]));
                _perso->caract_modif[k]=_perso->perso.caract[k];
            }
            /* le % */
            wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"sexe_race_version"));
            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[8]));
            gtk_spin_button_set_range(GTK_SPIN_BUTTON(wid),0,100);
            _perso->sexe_modif=_perso->perso.sexe;
            wid=gtk_label_new_with_mnemonic("Version : ");
            gtk_box_pack_start(GTK_BOX(wid_util),GTK_WIDGET(wid),1,0,0);
            combo=creation_combo_version();
            gtk_widget_set_name (GTK_WIDGET(combo),"version");
            _perso->version_modif=_perso->perso.version;
            g_signal_connect (G_OBJECT (combo), "changed", G_CALLBACK (modif_version),_perso);
            gtk_box_pack_start(GTK_BOX(wid_util),GTK_WIDGET(combo),1,0,0);
            wid=gtk_label_new_with_mnemonic(" |   Race : ");
            gtk_widget_set_name (wid,"race_label");
            gtk_box_pack_start(GTK_BOX(wid_util),GTK_WIDGET(wid),1,0,0);
            combo=creation_combo_race();
            gtk_widget_set_name (GTK_WIDGET(combo),"race");
            _perso->race_modif=_perso->perso.race;
            g_signal_connect (G_OBJECT (combo), "changed", G_CALLBACK (modif_race),_perso);
            callback_color_race_cellrender (combo,_perso);
            gtk_box_pack_start(GTK_BOX(wid_util),GTK_WIDGET(combo),1,0,0);
            wid=gtk_label_new_with_mnemonic(" |   Sexe : ");
            gtk_box_pack_start(GTK_BOX(wid_util),GTK_WIDGET(wid),1,0,0);
            combo=creation_combo_sexe();
            gtk_widget_set_name (GTK_WIDGET(combo),"sexe");
            g_signal_connect (G_OBJECT (combo), "changed", G_CALLBACK (modif_sexe),_perso);
            gtk_box_pack_start(GTK_BOX(wid_util),GTK_WIDGET(combo),1,0,0);
            gtk_widget_show_all(wid_util);

            /* les classes */

            /* les armures */
            combo=creation_combo_armure();
            callback_color_armure_cellrender (combo,_perso);
            gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(_perso->modif->builder,"bt_armure1")),GTK_WIDGET(combo),1,0,0);
            gtk_widget_set_name (GTK_WIDGET(combo),"armure1");
            g_signal_connect (G_OBJECT (combo), "changed", G_CALLBACK (modif_active),_perso);
            wid=gtk_spin_button_new_with_range(-9,9,1);
            gtk_widget_set_name (wid,"mag_armure1");
            g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_active),_perso);
            gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(_perso->modif->builder,"bt_armure1")),GTK_WIDGET(wid),0,0,0);
            combo=creation_combo_armure();
            callback_color_armure_cellrender (combo,_perso);
            gtk_widget_set_name (GTK_WIDGET(combo),"armure2");
            g_signal_connect (G_OBJECT (combo), "changed", G_CALLBACK (modif_active),_perso);
            gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(_perso->modif->builder,"bt_armure2")),GTK_WIDGET(combo),1,0,0);
            wid=gtk_spin_button_new_with_range(-9,9,1);
            gtk_widget_set_name (wid,"mag_armure2");
            g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_active),_perso);
            gtk_box_pack_start(GTK_BOX(gtk_builder_get_object(_perso->modif->builder,"bt_armure2")),GTK_WIDGET(wid),0,0,0);

            /*bouclier et bague */
            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"bouclier_mag"));
            gtk_spin_button_set_range(GTK_SPIN_BUTTON(wid),-9,9);
            g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_active),_perso);
            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"autre_ca"));
            gtk_spin_button_set_range(GTK_SPIN_BUTTON(wid),-9,9);
            g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_active),_perso);
            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"autre_save"));
            gtk_spin_button_set_range(GTK_SPIN_BUTTON(wid),-9,9);
            g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_active),_perso);
            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"bague_mag"));
            gtk_spin_button_set_range(GTK_SPIN_BUTTON(wid),-9,9);
            g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_active),_perso);
            gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"b_droite_2")));

            /* les dieux */
            wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"bt_pantheon"));
            combo=GTK_COMBO_BOX(creation_combo_pantheons());
            gtk_box_pack_start(GTK_BOX(wid_util),GTK_WIDGET(combo),1,0,0);
            gtk_widget_set_name (GTK_WIDGET(combo),"pantheon");
            g_signal_connect (G_OBJECT (combo), "changed", G_CALLBACK (modif_pantheon),_perso);
            gtk_widget_show_all(wid_util);

            /* voleur */
            for(j=0;j<8;j++)
            {
                wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_fac_vol[j]));
                gtk_spin_button_set_range(GTK_SPIN_BUTTON(wid),-99,999);
            }

            /* sorts */

            /* psi */

            drap=1;
            //_perso->d_m=0; /* remise à zéro dees midifications */
            init_fichier_modif(NULL,_perso);
            gtk_builder_connect_signals(_perso->modif->builder,_perso);

        }
        else
        {
            printf("Ouverture du fichier modification.glade impossible\n");
        }
    }
    else
    { /* mise en premier plan de la fenêtre */
        gtk_window_present(GTK_WINDOW(_perso->modif->window));
    }

    (void)appel;
}

void ferme_modif(GtkWidget *widget, FenetrePerso * _perso)
{
  GtkWidget *p_dialog = NULL;
  char ch[LONG];

    if (_perso->d_m==1)
    {
        if (_perso->perso.nom)
        {
            sprintf(ch,"Voulez-vous sauvegarder les modifications du personnage \n%s",_perso->perso.nom);
        }
        else
        {
            sprintf(ch,"Voulez-vous sauvegarder les modifications du personnage \nsans nom");
        }
        p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
        switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
        {
            case GTK_RESPONSE_NO:
                break; /* on sort du programme */
            default :
                while (drap==0); /* on attend son tour */
                valid_fichier_modif(NULL,_perso);
                break;
        }
        gtk_widget_destroy(p_dialog);

    }
    else
    { /* aucune modification depuis le dernier enregistrement */
    }

    g_free(_perso->competence_modif);
    _perso->competence_modif=NULL;
    g_free(_perso->armes_modif);
    _perso->armes_modif=NULL;
    g_free(_perso->classe_modif);
    _perso->classe_modif=NULL;
    g_free(_perso->niv_classe_modif);
    _perso->niv_classe_modif=NULL;
    _perso->modif=NULL;
    _perso->d_m=0; /* on oublie toutes les modifications qui ont été faites */
    _perso->mag=0;
    _perso->psi=0;


    (void)widget;
}

GtkComboBox * creation_combo_race(void)
{
    unsigned short i;
    GtkComboBox * combo;

    combo=GTK_COMBO_BOX(gtk_combo_box_text_new ());

    for(i=0;i<NB_RACE;i++)
    {
        gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), RACE[i].nom, RACE[i].nom);
    }

    return combo;
}

GtkComboBox * creation_combo_armure(void)
{
    unsigned short i;
    GtkComboBox * combo;

    combo=GTK_COMBO_BOX(gtk_combo_box_text_new ());

    for(i=0;i<NB_ARMURE;i++)
    {
        gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), ARMURE[i].nom, ARMURE[i].nom);
    }

    return combo;
}

GtkComboBox * creation_combo_version(void)
{
    GtkComboBox * combo;

    combo=GTK_COMBO_BOX(gtk_combo_box_text_new ());
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), "ADD", "ADD");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), "ADD2", "ADD2");

    return combo;
}

GtkComboBox * creation_combo_sexe(void)
{
    GtkComboBox * combo;

    combo=GTK_COMBO_BOX(gtk_combo_box_text_new ());
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), "Masculin", "Masculin");
    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(combo), "Féminin", "Féminin");

    return combo;
}

void ferme_modif_perso(GtkWidget *wid, FenetrePerso * _perso)
{
    static char _drap=0;

    if (_drap==0) /* pour éviter un double appel */
    {
        _drap=1;
        gtk_window_close(GTK_WINDOW(_perso->modif->window));
        _drap=0;
    }
    else
    {
    }

    (void)wid;
}


void couleur(GtkWidget * wid, unsigned short place)
{
    if (((place%6)/3)==0)
    {
        couleur_item(wid,GRISCLAIR);
    }
    else
    {
        couleur_item(wid,GRISFONCE);
    }
}


void init_fichier_modif(GtkWidget *appel, FenetrePerso * _perso)
{
    signed short i;
    char ch[LONG];
    GtkWidget * wid, * wid1, * wid_util;
    GtkComboBox * combo;
    GtkWidget * p_dialog;



    if (drap==1) /* on ne peut pas vérifier */
    {
        if (_perso->d_m==1)
        { /* confirmer la mise à zéro */
            if (_perso->perso.nom)
            {
                sprintf(ch,"Voulez-vous annuler et perdre\n les modifications du personnage \n%s",_perso->perso.nom);
            }
            else
            {
                sprintf(ch,"Voulez-vous annuler et perdre\n les modifications du personnage \nsans nom");
            }
            p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
            switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
            {
                case GTK_RESPONSE_NO:
                    while (drap==0); /* on attend son tour */
                    valid_fichier_modif(NULL,_perso);
                    break; /* on sort du programme */
                default :
                    break;
            }
            gtk_widget_destroy(p_dialog);
        }
        else
        { /* mise à zéro inutile sauf au départ */
        }
        _perso->d_m=0; /* on sera comme au départ */
        drap=0; /* on ne vérifie rien ... */
        /* les caractéristiques */
        for(i=0;i<=8;i++)
        {
            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[i]));
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),_perso->perso.caract[i]);
        }

        /* info de bases */
        wid1=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"sexe_race_version"));
        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid1),"version");
        gtk_combo_box_set_active (GTK_COMBO_BOX(wid),_perso->perso.version);
        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid1),"race");
        gtk_combo_box_set_active (GTK_COMBO_BOX(wid),race_nb(_perso->perso.race,_perso->combo_races));
        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid1),"sexe");
        gtk_combo_box_set_active (GTK_COMBO_BOX(wid),_perso->perso.sexe);

        /* les métiers */
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes"));
        vide_widget(wid_util);
        wid=gtk_label_new_with_mnemonic("Classe");
        gtk_grid_attach (GTK_GRID(wid_util),wid,1,0,1,1);
        wid=gtk_label_new_with_mnemonic("Niveau");
        gtk_grid_attach (GTK_GRID(wid_util),wid,3,0,1,1);
        wid=gtk_label_new_with_mnemonic("point d'expérience");
        gtk_grid_attach (GTK_GRID(wid_util),wid,5,0,1,1);
        wid=gtk_label_new_with_mnemonic("niveau max");
        gtk_grid_attach (GTK_GRID(wid_util),wid,6,0,1,1);
        if (_perso->perso.classe!=NULL)
        {

            for (i=0;_perso->perso.classe[i]!=-1;i++)
            {
                ajoute_ligne_classe(_perso,i,_perso->perso.classe[i],_perso->perso.niveau[i],_perso->perso.XP[i]);
            }
            g_free(_perso->classe_modif);
            _perso->classe_modif=(signed short *)malloc((i+1)*sizeof(signed short));
            _perso->classe_modif[i]=-1;
            while (i>0)
            {
                i--;
                _perso->classe_modif[i]=_perso->perso.classe[i];
                sprintf(ch,"classe_%hu",i);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch);
                /*if (i==0) gtk_combo_box_set_active (GTK_COMBO_BOX(wid),classe_nb(_perso->perso.classe[i],_perso->combo_classes));*/
            }
            i=-1;
        }
        else
        {
            i=0;
        }
        if (i==0)
        { /* cas anormal ou début vierge : pas de classe sélectionnée */
            ajoute_ligne_classe(_perso,0,0,0,0);
        }
        else
        {
        }
        gtk_widget_show_all(wid_util);

        /* pdv */
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"boite_PDV"));
        vide_widget(wid_util);
        if (_perso->perso.pdv!=NULL)
        {
            for (i=0;_perso->perso.pdv[i]>0;i++)
            {
                wid1=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                 sprintf(ch,"nv %hd : ",i+1);
                 wid=gtk_label_new_with_mnemonic(ch);
                 gtk_box_pack_start(GTK_BOX(wid1),wid,TRUE,TRUE,9);
                wid=gtk_spin_button_new_with_range(1,99,1);
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),_perso->perso.pdv[i]);
                sprintf(ch,"nb_pdv_%hu",i);
                gtk_widget_set_name (wid,ch);
                g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_pdv),_perso);
                sprintf(ch,"pdv_%hu",i);
                gtk_widget_set_name (wid1,ch);
                gtk_box_pack_start(GTK_BOX(wid1),wid,TRUE,TRUE,9);

                gtk_box_pack_start(GTK_BOX(wid_util),wid1,0,0,0);
            }
            gtk_widget_show_all(wid_util);
        }
        else
        {
            /* pas de pdv enregistrés */
        }

        /* les armes */
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_armes"));
        vide_widget(wid_util);
        wid=gtk_label_new_with_mnemonic("Arme");
        gtk_grid_attach (GTK_GRID(wid_util),wid,1,0,1,1);
        wid=gtk_label_new_with_mnemonic("Maîtrise");
        gtk_grid_attach (GTK_GRID(wid_util),wid,3,0,1,1);
        wid=gtk_label_new_with_mnemonic("Magie");
        gtk_grid_attach (GTK_GRID(wid_util),wid,5,0,1,1);
        wid=gtk_label_new_with_mnemonic("Autre toucher");
        gtk_grid_attach (GTK_GRID(wid_util),wid,7,0,1,1);
        wid=gtk_label_new_with_mnemonic("Autre dégâts");
        gtk_grid_attach (GTK_GRID(wid_util),wid,9,0,1,1);
        if (_perso->perso.arme!=NULL)
        {
            for (i=0;_perso->perso.arme[i]!=-1;i++)
            {
                ajoute_ligne_arme(_perso,i,_perso->perso.arme[i],_perso->perso.niv_arme[i],_perso->perso.mag_arme[i],_perso->perso.arme_autre_t[i],_perso->perso.arme_autre_d[i]);
            }
            gtk_widget_show_all(wid_util);
        }
        else
        {
            /* pas d'armes enregistrées */
        }


        /* les armures */
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"b_droite_2"));
        combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),"armure1"));
        gtk_combo_box_set_active (combo,_perso->perso.armure1);
        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),"mag_armure1");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),_perso->perso.mag_armure1);
        combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),"armure2"));
        gtk_combo_box_set_active (combo,_perso->perso.armure2);
        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),"mag_armure2");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),_perso->perso.mag_armure2);
        if (_perso->perso.cap!='o')
        {
            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"Vit/Long"));
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),TRUE);
        }
        else
        {
            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"CAP/CR"));
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),TRUE);
        }

        /*bouclier et bague */
        wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"bouclier_mag"));
        if (_perso->perso.bouclier!=NULL)
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,"bouclier")),TRUE);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),_perso->perso.bouclier[0]);
        }
        else
        {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,"bouclier")),FALSE);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),0);
        }
        wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"bague_mag"));
        if (_perso->perso.bague!=NULL)
        {
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),_perso->perso.bague[0]);
        }
        else
        {
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),0);
        }
        wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"autre_ca"));
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),_perso->perso.autre_ca);
        wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"autre_save"));
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),_perso->perso.autre_save);

        /* les dieux */
        combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"bt_pantheon")),"pantheon"));
        i=pantheon_entier(_perso->perso.pantheon);
        if (i<0)
        {
            i=0;
        }
        else
        { /* le pantheon a été trouvé */
        }
        gtk_combo_box_set_active (combo,i);
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"bt_dieu"));
        combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),"dieu"));
        if (combo)
        {
            gtk_widget_destroy(GTK_WIDGET(combo));
        }
        else
        { /* le menu n'est pas installé : c'est l'ouverture de la fenêtre */
        }
        combo=GTK_COMBO_BOX(creation_combo_dieux(i));
        gtk_box_pack_start(GTK_BOX(wid_util),GTK_WIDGET(combo),1,0,0);
        gtk_combo_box_set_active (combo,ABS(dieu_entier(_perso->perso.dieu,i)));
        g_signal_connect (G_OBJECT (combo), "changed", G_CALLBACK (modif_dieu),_perso);
        gtk_widget_set_name (GTK_WIDGET(combo),"dieu");
        gtk_widget_show_all(wid_util);

        /* compétences */
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_competence"));
        vide_widget(wid_util);
        wid=gtk_label_new_with_mnemonic("Compétence");
        gtk_grid_attach (GTK_GRID(wid_util),wid,1,0,1,1);
        wid=gtk_label_new_with_mnemonic("Niveau");
        gtk_grid_attach (GTK_GRID(wid_util),wid,3,0,1,1);
        if (_perso->perso.competence!=NULL)
        {
            for (i=0;_perso->perso.competence[i]!=-1;i++)
            {
                ajoute_ligne_competence(_perso,i,_perso->perso.competence[i],_perso->perso.niv_competence[i]);
            }
            gtk_widget_show_all(wid_util);
        }
        else
        { /* pas de compétence */
        }

        /* voleur */
        for(i=0;i<8;i++)
        {
            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_fac_vol[i]));
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),_perso->perso.tab_voleur[i]);
        }

        /* alignement */
        sprintf(ch,"Al_%s",algnm[_perso->perso.alignement]);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,ch)),TRUE);

        /* divers */
        if (_perso->perso.nom)
        {
            gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"Nom_perso")),_perso->perso.nom);
        }
        else
        {
        }
        if (_perso->perso.joueur)
        {
            gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"nom_joueur")),_perso->perso.joueur);
        }
        else
        {
        }
        sprintf(ch,"%hu",_perso->perso.age);
        gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"age")),ch);
        if (_perso->perso.cheveux)
        {
            gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"cheveux")),_perso->perso.cheveux);
        }
        else
        {
        }
        if (_perso->perso.yeux)
        {
            gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"yeux")),_perso->perso.yeux);
        }
        else
        {
        }
        if (_perso->perso.peau)
        {
            gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"peau")),_perso->perso.peau);
        }
        else
        {
        }
        if (_perso->perso.sociale)
        {
            gtk_entry_set_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"sociale")),_perso->perso.sociale);
        }
        else
        {
        }

        /* magie */
        if (_perso->mag==1)
        {
            init_mag(_perso);
        }
        else
        {
            /* onglets 'magie' non ouverts */
        }

        /* taille - poids */
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,"poids_5")),TRUE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,"taille_5")),TRUE);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"taille")),_perso->perso.taille);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"poids")),_perso->perso.poids);
        sprintf(ch,"\n\nTaille = %hd cm\nMasse = %hd kg",_perso->perso.taille,_perso->perso.poids);
        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(_perso->modif->builder,"info_poids_taille")),ch);

        /* psi */
        if (_perso->psi==1)
        {
            init_psi(_perso);
        }
        else
        {
            /* psi non ouvert */
        }
        /* note perso */
        GtkTextBuffer * buf=gtk_text_buffer_new (NULL);
        if (_perso->perso.commentaire!=NULL)
        {
            gtk_text_buffer_set_text(buf,_perso->perso.commentaire,-1);
            gtk_text_view_set_buffer (GTK_TEXT_VIEW(gtk_builder_get_object(_perso->modif->builder,"note_perso")),buf);
        }
        else
        {
        }

        donne_caract_modif(_perso);
        donne_version_modif( _perso);
        donne_sexe_modif(_perso);
        donne_race_modif(_perso);
        donne_arme_modif(_perso);
        donne_competence_modif(_perso);
        donne_niv_classe_modif(_perso);

        drap=1;
        modif_version(NULL,_perso);
        modif_competence(NULL,_perso);
        modif_psi(NULL,_perso);
        verif_taille(NULL,_perso);
        _perso->d_m=0;
    }
    else
    {
        /* le drapeau est activé */
    }

    (void)appel;
}

void modif_caract(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        donne_caract_modif(_perso);
        verif_caract(_perso);
        verif_competence(_perso); /* le nombre varie en fonction de l'intelligence */
        verif_voleur(_perso);
        verif_classe(_perso); /* pour vérifier les niveaux maximum */
        verif_pdv(_perso);
        verif_psi(_perso);
        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }


    (void)appel;
}

void modif_sexe(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        donne_sexe_modif(_perso);
        verif_caract(_perso);
        verif_taille(NULL,_perso);
        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }


    (void)appel;
}

void modif_race(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        donne_race_modif(_perso);
        calcul_couleur_classe(_perso);
        calcul_couleur_armure(_perso);
        verif_caract(_perso);
        verif_race(_perso);
        verif_classe(_perso);
        verif_pdv(_perso);
        verif_voleur(_perso);
        verif_taille(NULL,_perso);

        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }


    (void)appel;
}

void modif_classe(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        donne_classe_modif(_perso);
        calcul_couleur_armure(_perso);
        calcul_couleur_competence(_perso);
        calcul_couleur_arme(_perso);
        verif_caract(_perso);
        verif_classe(_perso);
        verif_pdv(_perso);
        verif_XP_niv(_perso);
        verif_arme(_perso);
        verif_alignement(_perso);
        verif_competence(_perso);
        verif_voleur(_perso);
        verif_psi(_perso);
        verif_dieu(_perso);

        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }


    (void)appel;
}

void modif_niv(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        donne_niv_classe_modif(_perso);
        verif_XP_niv(_perso);
        verif_arme(_perso);
        verif_competence(_perso);
        verif_voleur(_perso);
        verif_psi(_perso);

        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }


    (void)appel;
}

void modif_voleur(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        verif_voleur(_perso);

        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }


    (void)appel;
}

void modif_XP(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        verif_XP_niv(_perso);
        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }


    (void)appel;
}
void modif_version(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        donne_version_modif(_perso);
        verif_caract(_perso);
        verif_race(_perso);
        verif_classe(_perso);
        verif_pdv(_perso);
        verif_XP_niv(_perso);
        verif_arme(_perso);
        verif_alignement(_perso);
        verif_voleur(_perso);
        calcul_couleur_classe(_perso);
        calcul_couleur_race(_perso);
        calcul_couleur_arme(_perso);
        calcul_couleur_competence(_perso);
        calcul_couleur_armure(_perso);
        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }


    (void)appel;
}

void modif_pdv(GtkWidget *appel,FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        verif_pdv(_perso);
        _perso->d_m=1;

        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }


    (void)appel;
}

void modif_arme(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        donne_arme_modif(_perso);
        verif_arme(_perso);
        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }


    (void)appel;
}

void modif_dieu(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        verif_alignement(_perso);
        verif_dieu(_perso);
        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }

    (void)appel;
}

void modif_competence(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        donne_competence_modif(_perso);
        verif_competence(_perso);
        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }

    (void)appel;
}

void modif_active(GtkWidget *appel, FenetrePerso * _perso)
{ /* on enregistre juste une modification */
    if (_perso==NULL)
    {
        printf("Surprise : données personnelles nulles à modif_active (modif_perso.c)\n");
    }
    else if (drap==1)
    {
        _perso->d_m=1;
    }
    else
    { /* le drapeau de non modification est activé */
    }

    (void)appel;
}

void modif_active_dell(GtkWidget *appel,GtkDeleteType type ,gint count , FenetrePerso * _perso)
{ /* on enregistre juste une modification */

    modif_active(appel,_perso);
    (void)type;
    (void) count;
}

void modif_active_ins(GtkWidget *appel, gchar * string, FenetrePerso * _perso)
{ /* on enregistre juste une modification */

    modif_active(appel,_perso);
    (void)string;
}


void modif_psi(GtkWidget *appel, FenetrePerso * _perso)
{ /* on enregistre juste une modification */
    if (drap==1 && _perso->psi==1)
    {
        drap=0;
        verif_psi(_perso);
        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }

    (void)appel;
}

void modif_pantheon(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        GtkComboBoxText * combo;
        signed short i;
        GtkWidget * wid_util;

        combo=GTK_COMBO_BOX_TEXT(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"bt_pantheon")),"pantheon"));
        i=pantheon_entier(gtk_combo_box_text_get_active_text (combo));
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"bt_dieu"));
        combo=GTK_COMBO_BOX_TEXT(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),"dieu"));
        gtk_widget_destroy(GTK_WIDGET(combo));
        combo=creation_combo_dieux(i);
        gtk_box_pack_start(GTK_BOX(wid_util),GTK_WIDGET(combo),1,0,0);
        gtk_combo_box_set_active (GTK_COMBO_BOX(combo),ABS(dieu_entier(_perso->perso.dieu,i)));
        gtk_widget_set_name (GTK_WIDGET(combo),"dieu");
        g_signal_connect (G_OBJECT (combo), "changed", G_CALLBACK (modif_dieu),_perso);
        gtk_widget_show_all(wid_util);
        _perso->d_m=1;
        drap=1;
        verif_dieu(_perso);
    }
    else
    {
        /* le drapeau est activé */
    }

    (void)appel;
}


void modif_origine(GtkWidget *appel, FenetrePerso * _perso)
{
    unsigned short i,j,k,ok=1;
    signed short * classe=_perso->classe_modif;
    gchar ch[LONG];
    unsigned short _version = _perso->version_modif;

    if (drap==1)
    {
        drap=0;
        if (sscanf(gtk_widget_get_name (appel),"ori_%hu_%hu",&i,&j)==2)
        {
            _perso->origine[i][j]=!_perso->origine[i][j];
             if (_perso->origine[i][j]==0)
             { /* disparition d'une origine possible */
                for(k=0;classe[k]!=-1;k++)
                { /* parcours des classes : est-ce compatible */
                    if (CLASSE[classe[k]].add[i].nom) /* si la classe existe dans la version supprimée */
                    {
                        if (i==_version )  /* si c'est la version actuelle */
                        {
                             if (CLASSE[classe[k]].add[_version].origine==j)
                             { /* on veut supprimer la version d'où vient cette classe */
                                if (ok==0)
                                {
                                    strncat(ch,"\nproblème avec la classe ",LONG-1);
                                    strncat(ch,CLASSE[classe[k]].nom,LONG-1);
                                }
                                else
                                {
                                    strncpy(ch, "Problème avec la classe ",LONG-1);
                                    strncat(ch,CLASSE[classe[k]].nom,LONG-1);
                                    ok=0;
                                }
                             }
                             else
                             {
                                 /* pas de problème */
                             }
                        }
                        else if (CLASSE[classe[k]].add[_version].nom==NULL) /* si la classe n'existe que dans l'autre version */
                        { /* i est l'autre version */
                             if (CLASSE[classe[k]].add[i].origine==j)
                             { /* on veut supprimer la version d'où vient cette classe */
                                if (ok==0)
                                {
                                    strncat(ch,"\nproblème avec la classe ",LONG-1);
                                    strncat(ch,CLASSE[classe[k]].nom,LONG-1);
                                }
                                else
                                {
                                    strncpy(ch, "Problème avec la classe ",LONG-1);
                                    strncat(ch,CLASSE[classe[k]].nom,LONG-1);
                                    ok=0;
                                }
                                strncat(ch," qui n'existe pas dans la version AD&D choisie",LONG-1);
                             }
                        }
                        else
                        { /* i est la version de AD&D non utilisée et c'est bon dans la version utilisée */
                        }
                    }
                    else
                    {
                        /* la classe n'existe pas dans la version supprimée => rien a gérer */
                    }
                }

                if (RACE[_perso->race_modif].add[i].nom) /* si la classe existe dans la version supprimée */
                {
                    if (i==_version )  /* si c'est la version actuelle */
                    {
                         if (RACE[_perso->race_modif].add[_version].origine==j)
                         { /* on veut supprimer la version d'où vient cette classe */
                            if (ok==0)
                            {
                                strncat(ch,"\nproblème avec la race ",LONG-1);
                                strncat(ch,RACE[_perso->race_modif].nom,LONG-1);
                            }
                            else
                            {
                                strncpy(ch, "Problème avec la race ",LONG-1);
                                strncat(ch,RACE[_perso->race_modif].nom,LONG-1);
                                ok=0;
                            }
                         }
                         else
                         {
                             /* pas de problème */
                         }
                    }
                    else if (RACE[_perso->race_modif].add[_version].nom==NULL) /* si la classe n'existe que dans l'autre version */
                    { /* i est l'autre version */
                         if (RACE[_perso->race_modif].add[i].origine==j)
                         { /* on veut supprimer la version d'où vient cette classe */
                            if (ok==0)
                            {
                                strncat(ch,"\nproblème avec la race ",LONG-1);
                                strncat(ch,RACE[_perso->race_modif].nom,LONG-1);
                            }
                            else
                            {
                                strncpy(ch, "Problème avec la race ",LONG-1);
                                strncat(ch,RACE[_perso->race_modif].nom,LONG-1);
                                ok=0;
                            }
                            strncat(ch," qui n'existe pas dans la version AD&D choisie",LONG-1);
                         }
                    }
                    else
                    { /* i est la version de AD&D non utilisée et c'est bon dans la version utilisée */
                    }
                }
                else
                {
                    /* la classe n'existe pas dans la version supprimée => rien a gérer */
                }


                if (ok==0)
                { /* on ne peut pas désactiver cette origine */
                    _perso->origine[i][j]=1;
                    dialogue(ch,0);
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(appel),TRUE);
                }
                else
                { /* mettre à jour les listes de classe et de races */

                }

             }
             else
             {
                 /* nouvelle origine : pas de problème */
             }

        }
        else
        {
            _perso->ori_add1_add2=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(appel));
        }
        calcul_couleur_classe(_perso);
        calcul_couleur_race(_perso);

        drap=1;
    }
    else
    {
        /* drapeau activé : on laisse tomber */
    }
}


void simplifier_arme(GtkWidget *appel, FenetrePerso * _perso)
{
    if (drap==1)
    {
        drap=0;
        verif_armes_simplifie(_perso);
        _perso->d_m=1;
        drap=1;
    }
    else
    {
        /* le drapeau est activé */
    }

    (void)appel;
}

void valid_fichier_modif(GtkWidget *appel, FenetrePerso * _perso)
{
    signed short niv,i,j,k,l,nb;
    char ch[LONG],_nom[LONG];
    GtkWidget * wid, * wid_util;
    GtkComboBox * combo;
    GtkTextIter debut, fin;
    GtkTextBuffer * texte;


    if (drap==1) /* système momentanément figé */
    {
        _perso->d_m=0; /* on sera comme au départ */
        drap=0; /* on ne vérifie rien ... */
        /* les caractéristiques */
        for(i=0;i<=8;i++)
        {
            _perso->perso.caract[i]=_perso->caract_modif[i];
        }

        /* info de bases */
        _perso->perso.version=_perso->version_modif;
        _perso->perso.race=_perso->race_modif;
        _perso->perso.sexe=_perso->sexe_modif;

        /* les métiers */
        g_free(_perso->perso.classe);
        g_free(_perso->perso.niveau);
        g_free(_perso->perso.XP);
        for(i=0;_perso->classe_modif[i]!=-1;i++);
        i++;
        _perso->perso.classe=(signed short *)g_malloc(i*sizeof(signed short));
        _perso->perso.niveau=(signed short *)g_malloc(i*sizeof(signed short));
        _perso->perso.XP=(unsigned long *)g_malloc(i*sizeof(unsigned long));
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes"));
        for(i=0;_perso->classe_modif[i]!=-1;i++)
        {
            _perso->perso.classe[i]=_perso->classe_modif[i];
            _perso->perso.niveau[i]=_perso->niv_classe_modif[i];
            _perso->perso.XP[i]=gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_grid_get_child_at (GTK_GRID(wid_util),5,i+1)));
        }
        _perso->perso.classe[i]=-1;
        _perso->perso.niveau[i]=-1;
        _perso->perso.XP[i]=-1;

        /* pdv */
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"boite_PDV"));
        g_free(_perso->perso.pdv);
        _perso->perso.pdv=NULL;
        i=0;
        sprintf(ch,"nb_pdv_%hu",i);
        while ((wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch))!=NULL)
        {
            wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch);
            _perso->perso.pdv=(signed short *)g_realloc(_perso->perso.pdv,(i+2)*sizeof(signed short));
            _perso->perso.pdv[i]=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
            i++;
            sprintf(ch,"nb_pdv_%hu",i);
        }
        if (i==0)
        {
            _perso->perso.pdv=(signed short *)g_malloc(sizeof(signed short));
        }
        else
        {
            /* mémoire déjà réservée */
        }
        _perso->perso.pdv[i]=-1;

        /* les armes */
        g_free(_perso->perso.arme);
        g_free(_perso->perso.niv_arme);
        g_free(_perso->perso.mag_arme);
        g_free(_perso->perso.arme_autre_t);
        g_free(_perso->perso.arme_autre_d);
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_armes"));
        for(i=0;_perso->armes_modif[i]!=-1;i++);
        i++; /* on compte le nombre d'arme pour faire un seul malloc et éviter de faire plein de realloc */
        _perso->perso.arme=(signed short *)g_malloc(i*sizeof(signed short));
        _perso->perso.niv_arme=(signed short *)g_malloc(i*sizeof(signed short));
        _perso->perso.mag_arme=(signed short *)g_malloc(i*sizeof(signed short));
        _perso->perso.arme_autre_t=(signed short *)g_malloc(i*sizeof(signed short));
        _perso->perso.arme_autre_d=(signed short *)g_malloc(i*sizeof(signed short));
        for(i=0;_perso->armes_modif[i]!=-1;i++)
        {
            _perso->perso.arme[i]=_perso->armes_modif[i];
            sprintf(ch,"niv_arme_%hd",i);
            _perso->perso.niv_arme[i]=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
            sprintf(ch,"mag_arme_%hd",i);
            _perso->perso.mag_arme[i]=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
            sprintf(ch,"arme_autre_t_%hd",i);
            _perso->perso.arme_autre_t[i]=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
            sprintf(ch,"arme_autre_d_%hd",i);
            _perso->perso.arme_autre_d[i]=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
        }
        _perso->perso.arme[i]=-1;
        _perso->perso.niv_arme[i]=-1;
        _perso->perso.mag_arme[i]=-1;
        _perso->perso.arme_autre_t[i]=0;
        _perso->perso.arme_autre_d[i]=0;
        wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"CAP/CR"));
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid)))
        {
            _perso->perso.cap='o';
        }
        else
        {
            _perso->perso.cap='n';
        }

        /* les armures */
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"b_droite_2"));
        combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),"armure1"));
        _perso->perso.armure1=gtk_combo_box_get_active (combo);
        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),"mag_armure1");
        _perso->perso.mag_armure1=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
        combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),"armure2"));
        _perso->perso.armure2=gtk_combo_box_get_active (combo);
        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),"mag_armure2");
        _perso->perso.mag_armure2=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));

        /*bouclier et bague */
        g_free(_perso->perso.bouclier);
        wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"bouclier_mag"));
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,"bouclier"))))
        {
            _perso->perso.bouclier=(signed short *)g_malloc(sizeof(signed short));
            _perso->perso.bouclier[0]=gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"bouclier_mag")));
        }
        else
        {
            _perso->perso.bouclier=NULL;
        }
        g_free(_perso->perso.bague);
        wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"bague_mag"));
        if ((i=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid)))!=0)
        {
            _perso->perso.bague=(signed short *)g_malloc(sizeof(signed short));
            _perso->perso.bague[0]=i;
        }
        else
        {
            _perso->perso.bague=NULL;
        }
        wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"autre_ca"));
        _perso->perso.autre_ca=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
        wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"autre_save"));
        _perso->perso.autre_save=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));

        /* les dieux */
        g_free(_perso->perso.pantheon);
        combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"bt_pantheon")),"pantheon"));
        _perso->perso.pantheon=g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo)));
        g_free(_perso->perso.dieu);
        combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"bt_dieu")),"dieu"));
        _perso->perso.dieu=g_strdup(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo)));

        /* alignement */
        for (i=0;i<9;i++)
        {
            sprintf(ch,"Al_%s",algnm[i]);
            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,ch)))==TRUE)
            {
                _perso->perso.alignement=i;
                i=9;
            }
            else
            { /* pas encore le bon alignement */
            }
        }

        /* divers */
        g_free(_perso->perso.nom);
        _perso->perso.nom=g_strdup(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"Nom_perso"))));
        g_free(_perso->perso.joueur);
        _perso->perso.joueur=g_strdup(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"nom_joueur"))));
        sscanf(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"age"))),"%hu",&(_perso->perso.age));
        g_free(_perso->perso.cheveux);
        _perso->perso.cheveux=g_strdup(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"cheveux"))));
        g_free(_perso->perso.yeux);
        _perso->perso.yeux=g_strdup(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"yeux"))));
        g_free(_perso->perso.peau);
        _perso->perso.peau=g_strdup(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"peau"))));
        g_free(_perso->perso.sociale);
        _perso->perso.sociale=g_strdup(gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(_perso->modif->builder,"sociale"))));

        /* compétences */
        g_free(_perso->perso.competence);
        g_free(_perso->perso.niv_competence);
        for(i=0;_perso->competence_modif[i]!=-1;i++);
        i++; /* on compte les compétences pour faire un malloc et éviter les realloc */
        _perso->perso.competence=(signed short *)g_malloc(i*sizeof(signed short));
        _perso->perso.niv_competence=(signed short *)g_malloc(i*sizeof(signed short));
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_competence"));
        for(i=0;_perso->competence_modif[i]!=-1;i++)
        {
            _perso->perso.competence[i]=_perso->competence_modif[i];
            sprintf(ch,"niv_competence_%hd",i);
            _perso->perso.niv_competence[i]=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
        }
        _perso->perso.competence[i]=-1;
        _perso->perso.niv_competence[i]=-1;

        /* voleur */
        for(i=0;i<8;i++)
        {
            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_fac_vol[i]));
            _perso->perso.tab_voleur[i]=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
        }

        /* magie */
        if (_perso->mag!=0)
        { /* perso->mag est le niveau de magie de l'onglet ouvert */
            valid_perso_mag(_perso);
        }
        else
        {
            /* onglet magie fermé => on ne regarde pas les modifications */
        }

        /* taille */
        _perso->perso.taille=gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"taille")));
        _perso->perso.poids =gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"poids" )));

        /*psi*/
        if (_perso->psi==1)
        {
            if (_perso->perso.psi!=NULL)
            { /* effacer */
                g_free(_perso->perso.psi);
                g_free(_perso->perso.natif_psi);
                g_free(_perso->perso.niv_psi);
                _perso->perso.psi=NULL;
                _perso->perso.natif_psi=NULL;
                _perso->perso.niv_psi=NULL;
            }
            else
            {
            }
            nb=0;
            wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"notebook_psi"));
            for(i=0;i<NB_DISCIPLINE_PSI;i++)
            {
                for (j=0;j<2;j++)
                { /* science dévotion */
                    for(k=0;k<NB_TALENTS_PSI[i][j];k++)
                    {
                        sprintf(_nom,"psi_natif_%hu",TALENTS_PSI[i][j][k].clef);
                        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                        l=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid));
                        sprintf(_nom,"psi_niveau_%hu",TALENTS_PSI[i][j][k].clef);
                        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                        niv=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
                        if (niv!=0 || l!=0)
                        {
                             _perso->perso.psi=(signed short *)g_realloc(_perso->perso.psi,(nb+2)*sizeof(signed short));
                             _perso->perso.natif_psi=(signed short *)g_realloc(_perso->perso.natif_psi,(nb+2)*sizeof(signed short));
                             _perso->perso.niv_psi=(signed short *)g_realloc(_perso->perso.niv_psi,(nb+2)*sizeof(signed short));
                             _perso->perso.niv_psi[nb]=niv;
                             _perso->perso.psi[nb]=TALENTS_PSI[i][j][k].clef;
                             _perso->perso.natif_psi[nb]=l;
                             nb++;
                        }
                        else
                        { /* talent non sélectionné */
                        }
                    }
                }
            }
            if (nb!=0)
            {
                _perso->perso.psi[nb]=-1;
                _perso->perso.niv_psi[nb]=-1;
                _perso->perso.natif_psi[nb]=-1;
            }
            else
            { /* non initialisé */
            }
        }
        else
        {
            /* onglet psi  fermé : on ne prend pas en compte les modifications */
        }

        /* note perso */
        g_free(_perso->perso.commentaire);
        texte=gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(_perso->modif->builder,"note_perso")));
        gtk_text_buffer_get_start_iter(texte,&debut);
        gtk_text_buffer_get_end_iter(texte,&fin);
        _perso->perso.commentaire=gtk_text_buffer_get_text(texte,&debut,&fin,TRUE);

        /* origine */
        for( k=0;k<2;k++)
        {
             for(j=0;j<NB_ORIGINE[k];j++)
             {
                _perso->perso.origine[k][j]=_perso->origine[k][j];
             }
        }
        _perso->perso.ori_add1_add2=_perso->ori_add1_add2;

        drap=1;
        affiche_resume(_perso);
        _perso->d_r=1;
    }
    else
    {
        /* le drapeau est activé */
    }

    (void)appel;
}

/* modif_perso_mag_niv afficher un autre niveau             */
/* modif_perso_mag : ouverture du premier niveau            */
/* init_mag : mettre ce qui est mémorisé au niveau affiché  */
/* modif_perso_aff_mag : afficher un niveau                 */
/* void valid_perso_mag : enregistrer les modif d'un niveau */

void modif_perso_mag_niv(GtkWidget *appel, FenetrePerso * _perso)
{
    char _nom[16]="niv_mag1";
    signed short niv=15,i;
    GtkWidget * p_dialog, * wid_util;

    /* rechercher le nom du bouton qui appelle */
    for(i=1;i<=9;i++)
    {
        _nom[7]='0'+i;
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,_nom));
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid_util)))
        {
            niv=i;
            i=20;
        }
        else
        {
            /* l'entrée n'est pas la bonne */
        }
    }
    if (niv<10) /* on a trouvé une entrée validée */
    {
        if (_perso->mag!=niv)
        {
            /* sauvegarder le précédent niveau si on le souhaite */
            p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Enregistrer les modifications\nde ce niveau avant de passer au suivant ?");
            if (gtk_dialog_run (GTK_DIALOG (p_dialog)) != GTK_RESPONSE_NO)
            { /* enregistrement du niveau */
                valid_perso_mag(_perso);
            }
            else
            {
                /* on n'enregistre pas les modifications du niveau précédent */
            }
            gtk_widget_destroy(p_dialog);
            /* effacer le précédent niveau */
            wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"mag_grille"));
            for(i=1;i<=NB_SORT_MAG[1][_perso->mag-1];i++)
            {
                    gtk_grid_remove_row (GTK_GRID(wid_util),1);
            }

            /* afficher le nouveau niveau */
            _perso->mag=niv;
            modif_perso_aff_mag(_perso);
        }
        else
        {
            /* même niveau => on peut demander si on réinitialise le niveau */
        }

    }
    else
    {
        printf("Le nom du bouton n'est pas au bon format\n");
    }

    (void) appel;
}

void modif_perso_aff_mag(FenetrePerso * _perso)
{ /* niv de 0 à 8 : niveau rel : niv+1 */
    GtkWidget * wid=NULL, * wid1=NULL, * wid_util=NULL, * wid2=NULL;
    unsigned short i=0,j,niv=_perso->mag-1,ii;
    char _nom[LONG],url[LONG];

                    wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"mag_grille"));
                    for(ii=0;ii<NB_SORT_MAG[1][niv];ii++)
                    { /* couper les noms trop longs */
                        sprintf(url,"https://dnd.siaud.org/mago.php?sort=%lu",SORTILEGE_MAG[ADD2][niv][ii].clef);
                        if (dans_source(SORTILEGE_MAG[ADD2][niv][ii].source,_perso->perso.origine))
                        {
                            if (strlen(SORTILEGE_MAG[ADD2][niv][ii].nom)>TAILLE_MAX_NOM_SORT)
                            {
                                strcpy(_nom,SORTILEGE_MAG[ADD2][niv][ii].nom);
                                for(j=TAILLE_MAX_NOM_SORT/3;j<strlen(_nom);j++)
                                {
                                    if (_nom[j]=='/')
                                    {
                                        _nom[j]='\n';
                                        j=strlen(_nom);
                                    }
                                    else
                                    {
                                    }
                                }
                                if (j==strlen(_nom))
                                {
                                    for(j=strlen(_nom)/2;j<strlen(_nom);j++)
                                    {
                                        if (_nom[j]==' ')
                                        {
                                            _nom[j]='\n';
                                            j=strlen(_nom);
                                        }
                                        else
                                        {
                                        }
                                    }
                                }
                                //wid=gtk_label_new_with_mnemonic(_nom);
                                wid=gtk_link_button_new_with_label(url,_nom);
                            }
                            else
                            {
                                //wid=gtk_label_new_with_mnemonic(SORTILEGE_MAG[ADD2][niv][ii].nom);
                                wid=gtk_link_button_new_with_label(url,SORTILEGE_MAG[ADD2][niv][ii].nom);
                            }

                            gtk_grid_attach(GTK_GRID(wid_util),wid,0,1+i,1,1);
                            couleur(wid,i);
                            wid1=gtk_radio_button_new(NULL);
                            sprintf(_nom,"compris_%hu_%lu",niv,SORTILEGE_MAG[ADD2][niv][ii].clef);
                            gtk_widget_set_name (GTK_WIDGET(wid1),_nom);
                            g_signal_connect (G_OBJECT (wid1), "clicked", G_CALLBACK (modif_active),_perso);
                            gtk_widget_set_halign (wid1,GTK_ALIGN_CENTER);
                            wid2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                            gtk_widget_set_hexpand(wid1,TRUE);
                            gtk_box_pack_start (GTK_BOX(wid2),wid1,FALSE,FALSE,0);
                            gtk_grid_attach(GTK_GRID(wid_util),wid2,1,1+i,1,1);
                            couleur(wid2,i);

                            wid=gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(wid1));
                            wid2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                            gtk_widget_set_hexpand(wid,TRUE);
                            gtk_widget_set_halign (wid,GTK_ALIGN_CENTER);
                            gtk_box_pack_start (GTK_BOX(wid2),wid,FALSE,FALSE,0);
                            sprintf(_nom,"pas_compris_%hu_%lu",niv,SORTILEGE_MAG[ADD2][niv][ii].clef);
                            gtk_widget_set_name (GTK_WIDGET(wid),_nom);
                            g_signal_connect (G_OBJECT (wid), "clicked", G_CALLBACK (modif_active),_perso);
                            gtk_grid_attach(GTK_GRID(wid_util),wid2,2,1+i,1,1);
                            couleur(wid2,i);

                            wid=gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(wid1));
                            wid2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                            gtk_widget_set_hexpand(wid,TRUE);
                            gtk_widget_set_halign (wid,GTK_ALIGN_CENTER);
                            gtk_box_pack_start (GTK_BOX(wid2),wid,FALSE,FALSE,0);
                            sprintf(_nom,"pas_regarde_%hu_%lu",niv,SORTILEGE_MAG[ADD2][niv][ii].clef);
                            gtk_widget_set_name (GTK_WIDGET(wid),_nom);
                            g_signal_connect (G_OBJECT (wid), "clicked", G_CALLBACK (modif_active),_perso);
                            gtk_grid_attach(GTK_GRID(wid_util),wid2,3,1+i,1,1);
                            couleur(wid2,i);

                            wid=gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(wid1));
                            wid2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                            gtk_widget_set_hexpand(wid,TRUE);
                            gtk_widget_set_halign (wid,GTK_ALIGN_CENTER);
                            gtk_box_pack_start (GTK_BOX(wid2),wid,FALSE,FALSE,0);
                            //gtk_widget_set_valign (wid,GTK_ALIGN_CENTER);
                            sprintf(_nom,"pas_livre_%hu_%lu",niv,SORTILEGE_MAG[ADD2][niv][ii].clef);
                            gtk_widget_set_name (GTK_WIDGET(wid),_nom);
                            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),TRUE);
                            g_signal_connect (G_OBJECT (wid), "clicked", G_CALLBACK (modif_active),_perso);
                            gtk_grid_attach(GTK_GRID(wid_util),wid2,4,1+i,1,1);
                            couleur(wid2,i);

                            wid=gtk_check_button_new();
                            wid2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                            gtk_widget_set_hexpand(wid,TRUE);
                            gtk_widget_set_halign (wid,GTK_ALIGN_CENTER);
                            gtk_box_pack_start (GTK_BOX(wid2),wid,FALSE,FALSE,0);
                            sprintf(_nom,"voyage_%hu_%lu",niv,SORTILEGE_MAG[ADD2][niv][ii].clef);
                            g_signal_connect (G_OBJECT (wid), "clicked", G_CALLBACK (modif_active),_perso);
                            gtk_widget_set_name (GTK_WIDGET(wid),_nom);
                            gtk_grid_attach(GTK_GRID(wid_util),wid2,5,1+i,1,1);
                            couleur(wid2,i);
                            //gtk_widget_set_valign (wid,GTK_ALIGN_CENTER);
                            gtk_widget_show_all(wid_util);
                            i++;
                        }
                    }
                    init_mag(_perso);
}

void modif_perso_mag(GtkWidget *appel, FenetrePerso * _perso)
{

    if (_perso->mag==0)
    { /* on affiche l'onglet magie => on demande si on veut afficher la magie au cas ou ce n'est pas le cas */

        GtkWidget *p_dialog = NULL;

        p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Activer les onglets magie ?");
        if (gtk_dialog_run (GTK_DIALOG (p_dialog)) != GTK_RESPONSE_NO)
        {
            gtk_widget_destroy(p_dialog);
            p_dialog=NULL;
            p_dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
            gtk_window_set_default_size(GTK_WINDOW(p_dialog), 320, 200);
            gtk_container_set_border_width(GTK_CONTAINER(p_dialog), 4);
            gtk_grab_add(p_dialog);
            gtk_widget_show_all(p_dialog);
            _perso->mag=1;

            modif_perso_aff_mag(_perso);

            gtk_widget_destroy(p_dialog);
        }
        else
        {
            gtk_widget_destroy(p_dialog);
            /* on laisse fermé */
        }
    }
    else
    {
        /* onglets déjà ouverts */
    }

    (void) appel;
}

void valid_perso_mag(FenetrePerso * _perso)
{
    signed short niv=_perso->mag-1,i,k,l;
    GtkWidget * wid_util=NULL, * wid=NULL;
    char _nom[LONG];

            if (_perso->perso.livre_sorts==NULL)
            {
                _perso->perso.livre_sorts=(signed long **)g_malloc(9*sizeof(signed long *));
                _perso->perso.sort_su=(signed short **)g_malloc(9*sizeof(signed short *));
                for (i=0;i<9;i++)
                {
                    _perso->perso.livre_sorts[i]=NULL;
                    _perso->perso.sort_su[i]=NULL;
                }
            }
            else
            {
                if (_perso->perso.livre_sorts[niv]!=NULL)
                {
                    g_free(_perso->perso.sort_su[niv]);
                    g_free(_perso->perso.livre_sorts[niv]);
                    _perso->perso.sort_su[niv]=NULL;
                    _perso->perso.livre_sorts[niv]=NULL;
                }
                else
                { /* livre de sort non initialisé : pas besoin de le vider */
                }
            }
            wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"mag_grille"));
            k=0;
            for(i=0;i<NB_SORT_MAG[ADD2][niv];i++)
            if (dans_source(SORTILEGE_MAG[ADD2][niv][i].source,_perso->perso.origine))
            {
                sprintf(_nom,"pas_livre_%hu_%lu",niv,SORTILEGE_MAG[ADD2][niv][i].clef);
                wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid)))
                { /* sort pas dans le livre */
                }
                else
                {
                    if (_perso->perso.livre_sorts==NULL)
                    { /* il faut initialiser le livre de sorts */
                        _perso->perso.livre_sorts=(signed long **)g_malloc(9*sizeof(signed long *));
                        _perso->perso.sort_su=(signed short **)g_malloc(9*sizeof(signed short *));
                        for(l=0;l<9;l++)
                        {
                            _perso->perso.sort_su[l]=NULL;
                            _perso->perso.livre_sorts[l]=NULL;
                        }
                    }
                    else
                    { /* livre de sorts déjà initialisé */
                    }
                    _perso->perso.sort_su[niv]=(signed short *)g_realloc(_perso->perso.sort_su[niv],(k+2)*sizeof(unsigned short));
                    _perso->perso.livre_sorts[niv]=(signed long *)g_realloc(_perso->perso.livre_sorts[niv],(k+2)*sizeof(unsigned long));
                    sprintf(_nom,"compris_%hd_%ld",niv,SORTILEGE_MAG[ADD2][niv][i].clef);
                    _perso->perso.livre_sorts[niv][k]=SORTILEGE_MAG[ADD2][niv][i].clef;
                    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom)))) /* revoir */
                    {
                        _perso->perso.sort_su[niv][k]=COMPRIS;
                    }
                    else
                    {
                        sprintf(_nom,"pas_compris_%hd_%ld",niv,SORTILEGE_MAG[ADD2][niv][i].clef);
                        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom))))
                        {
                            _perso->perso.sort_su[niv][k]=PAS_COMPRIS;
                        }
                        else
                        {
                            _perso->perso.sort_su[niv][k]=PAS_REGARDE;
                        }
                    }
                    sprintf(_nom,"voyage_%hd_%lu",niv,SORTILEGE_MAG[ADD2][niv][i].clef);
                    wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid)))
                    { /* le sort est dans le livre de voyage */
                    }
                    else
                    {
                        _perso->perso.sort_su[niv][k]*=-1;
                    }
                    k++;
                }
            }
            if (_perso->perso.livre_sorts!=NULL  && _perso->perso.sort_su[niv]!=NULL)
            {
                _perso->perso.sort_su[niv][k]=0;
                _perso->perso.livre_sorts[niv][k]=-1;
            }
            else
            { /* pas encore de sorts ni de sort de ce niveau */
            }

}

void init_mag(FenetrePerso * _perso)
{
    signed short i,niv=_perso->mag-1;
    signed short su;
    char _nom[LONG];
    GtkWidget * wid, * wid_util;

    if (_perso->mag!=0)
    {
            {   /* mise à 0 */
                wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"mag_grille"));
                for(i=0;i<NB_SORT_MAG[ADD2][niv];i++)
                if (dans_source(SORTILEGE_MAG[ADD2][niv][i].source,_perso->perso.origine))
                {
                    sprintf(_nom,"pas_livre_%hu_%ld",niv,SORTILEGE_MAG[ADD2][niv][i].clef);
                    wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                    if (wid!=NULL)
                    {
                        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),TRUE);
                    }
                    else
                    {
                        printf("init_mag : Erreur avec le bouton %s\n",_nom);
                    }

                    sprintf(_nom,"voyage_%hu_%ld",niv,SORTILEGE_MAG[ADD2][niv][i].clef);
                    wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                    if (wid!=NULL)
                    {
                        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),FALSE);
                    }
                    else
                    {
                        printf("init_mag : Erreur avec le bouton %s\n",_nom);
                    }
                }
            }
            if (_perso->perso.livre_sorts!=NULL)
            {   /* prise en compte du personnage */
                {
                    if (_perso->perso.livre_sorts[niv]!=NULL)
                    {
                        for(i=0;_perso->perso.sort_su[niv][i]!=0;i++)
                        {
                            sprintf(_nom,"voyage_%hd_%ld",niv,labs(_perso->perso.livre_sorts[niv][i]));
                            wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                            if (_perso->perso.sort_su[niv][i]<0)
                            {
                                su=-_perso->perso.sort_su[niv][i];
                                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),FALSE);
                            }
                            else
                            {
                                su=_perso->perso.sort_su[niv][i];
                                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),TRUE);
                            }
                            switch(su)
                            {
                                case COMPRIS :
                                    sprintf(_nom,"compris_%hd_%ld",niv,labs(_perso->perso.livre_sorts[niv][i]));
                                    break;
                                case PAS_COMPRIS :
                                    sprintf(_nom,"pas_compris_%hd_%ld",niv,labs(_perso->perso.livre_sorts[niv][i]));
                                    break;
                                case PAS_REGARDE :
                                    sprintf(_nom,"pas_regarde_%hd_%ld",niv,labs(_perso->perso.livre_sorts[niv][i]));
                                    break;
                                default :
                                    sprintf(_nom,"pas_livre_%hd_%ld",niv,labs(_perso->perso.livre_sorts[niv][i]));
                            }
                            wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),TRUE);
                        }

                    }
                    else
                    {
                        /* pas de sort de ce niveau */
                    }
                }
            }
            else
            { /* printf("Pas de sort en sa possession\n"); */
            }
    }
    else
    {
        printf("On ne devrait pas initiliser la magie si les onglets sont fermés\n");
    }

}

void modif_perso_psi(GtkWidget *appel, FenetrePerso * _perso)
{
    GtkWidget *p_dialog = NULL;
    GtkWidget * wid=NULL, * wid1=NULL, * wid2=NULL, * grille=NULL;
    unsigned short i,j,niv,k;
    char _nom[LONG],ch[LONG];

    if (_perso->psi==0)
    {
        p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Activer les onglets 'psi' ?");
        if (gtk_dialog_run (GTK_DIALOG (p_dialog)) != GTK_RESPONSE_NO)
        {
            _perso->psi=1;
            for(i=0;i<NB_DISCIPLINE_PSI;i++)
            {
                if (i==0)
                {
                    wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"scroll_psi_0"));
                    gtk_label_set_text_with_mnemonic(GTK_LABEL(gtk_builder_get_object(_perso->modif->builder,"label_psi_0")),DISCIPLINE_PSI[0].nom);
                }
                else
                {
                    wid=gtk_scrolled_window_new(NULL,NULL);
                    wid1=gtk_label_new_with_mnemonic(DISCIPLINE_PSI[i].nom);
                    gtk_notebook_insert_page (GTK_NOTEBOOK(gtk_builder_get_object(_perso->modif->builder,"notebook_psi")),wid,wid1,i+1);
                }
                grille=gtk_grid_new ();
                gtk_grid_set_column_spacing(GTK_GRID(grille),2);
                gtk_container_add(GTK_CONTAINER(wid) ,GTK_WIDGET(grille));
                gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (wid), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

                wid1=gtk_button_new_with_label("Effacer");
                gtk_grid_attach(GTK_GRID(grille),wid1,0,0,3,1);
                for (j=0;j<2;j++)
                { /* science dévotion */
                    if(j==0)
                    {
                          wid=gtk_label_new(NULL);
                          gtk_label_set_markup (GTK_LABEL(wid),"<span size=\"15000\">Dévotions</span>");
                          gtk_label_set_justify(GTK_LABEL(wid),GTK_JUSTIFY_CENTER);
                          niv=0;
                    }
                    else
                    {
                        wid=gtk_label_new(NULL);
                        gtk_label_set_markup (GTK_LABEL(wid),"<span size=\"15000\">Science</span>");
                        gtk_label_set_justify(GTK_LABEL(wid),GTK_JUSTIFY_CENTER);
                        niv=NB_TALENTS_PSI[i][0]+1;
                    }
                    gtk_grid_attach(GTK_GRID(grille),wid,0,1+niv,1,1);
                    wid=gtk_label_new_with_mnemonic("Natif");
                    gtk_grid_attach(GTK_GRID(grille),wid,1,1+niv,1,1);
                    wid=gtk_label_new_with_mnemonic("Niveau");
                    gtk_grid_attach(GTK_GRID(grille),wid,2,1+niv,1,1);
                    for(k=0;k<NB_TALENTS_PSI[i][j];k++)
                    {
                        if (TALENTS_PSI[i][j][k].defense)
                        {
                            sprintf(ch,"<span style=\"italic\">%s</span>",TALENTS_PSI[i][j][k].nom);
                            wid=gtk_label_new(NULL);
                            gtk_widget_set_tooltip_markup (wid,"C'est une défense");
                            gtk_label_set_markup (GTK_LABEL(wid),ch);
                        }
                        else
                        {
                            wid=gtk_label_new(TALENTS_PSI[i][j][k].nom);
                        }
                        gtk_grid_attach(GTK_GRID(grille),wid,0,2+niv+k,1,1);
                        couleur(wid,k);
                        wid=gtk_check_button_new();
                        wid2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
                        gtk_widget_set_hexpand(wid,TRUE);
                        gtk_widget_set_halign (wid,GTK_ALIGN_CENTER);
                        gtk_box_pack_start (GTK_BOX(wid2),wid,FALSE,FALSE,0);
                        sprintf(_nom,"psi_natif_%hu",TALENTS_PSI[i][j][k].clef);
                        gtk_widget_set_name (GTK_WIDGET(wid),_nom);
                        g_signal_connect (G_OBJECT (wid), "clicked", G_CALLBACK (modif_psi),_perso);
                        gtk_grid_attach(GTK_GRID(grille),wid2,1,2+niv+k,1,1);
                        couleur(wid2,k);
                        wid=gtk_spin_button_new_with_range(0,99,1);
                        sprintf(_nom,"psi_niveau_%hu",TALENTS_PSI[i][j][k].clef);
                        gtk_widget_set_name (GTK_WIDGET(wid),_nom);
                        g_signal_connect (G_OBJECT (wid), "value_changed", G_CALLBACK (modif_psi),_perso);
                        gtk_grid_attach(GTK_GRID(grille),wid,2,2+niv+k,1,1);
                        couleur(wid,k);
                    }
                }
            }
            gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"notebook_psi")));
            init_psi(_perso);
        }
        else
        {
            /* on laisse fermé */
        }
        gtk_widget_destroy(p_dialog);
    }
    else
    {
        /* onglets déjà ouverts */
    }

    (void) appel;
}

void init_psi(FenetrePerso * _perso)
{
    signed short i;
    char _nom[LONG];
    GtkWidget * wid, * wid_util;

    if (_perso->psi==1)
    {
            wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"notebook_psi"));
            for(i=0;i<NB_DISCIPLINE_PSI;i++)
            { /* remise à zéro */
                unsigned short j;
                for (j=0;j<2;j++)
                { /* science dévotion */
                    unsigned short k;
                    for(k=0;k<NB_TALENTS_PSI[i][j];k++)
                    {
                        sprintf(_nom,"psi_natif_%hu",TALENTS_PSI[i][j][k].clef);
                        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),FALSE);
                        sprintf(_nom,"psi_niveau_%hu",TALENTS_PSI[i][j][k].clef);
                        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                        gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),0);
                    }
                }
            }
            if (_perso->perso.psi!=NULL)
            { /* prise en compte frd psi du perso */
                for(i=0;_perso->perso.psi[i]!=-1;i++)
                {
                    if (_perso->perso.natif_psi[i]!=0)
                    {
                        sprintf(_nom,"psi_natif_%hu",_perso->perso.psi[i]);
                        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),TRUE);
                    }
                    else
                    {
                        /* pas natif */
                    }
                    sprintf(_nom,"psi_niveau_%hu",_perso->perso.psi[i]);
                    wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                    gtk_spin_button_set_value(GTK_SPIN_BUTTON(wid),_perso->perso.niv_psi[i]);
                }
            }
            else
            { /*  printf("Connait pas les psi\n"); */
            }
    }
    else
    {
        printf("On ne devrait pas initiliser les psis i les onglets sont fermés\n");
    }

}
