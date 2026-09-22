/******************************************************************************
 *    Copyright (C) 2010-2015 by SIAUD Bernard                                *
 *    troumad@siaud.org                                                       *
 *                                                                            *
 *    This program is free software; you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation; either version 2 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program; if not, write to the                           *
 *    Free Software Foundation, Inc.,                                         *
 *    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 ******************************************************************************/

#include "include.h"
#include <string.h>
#include <zip.h>

void enregistre_groupe_sous(GtkWidget *ChildWidget, FenetrePerso * fenetre);
void nouveau_nom_groupe(GtkWidget *ChildWidget, struct callback_nv_nom * c_b);
void ajouter_perso_groupe(GtkWidget *ChildWidget, FenetrePerso * fenetre);
void info_fichier_groupe(GtkFileChooser *chooser, struct callback_nv_nom * c_b);
void callback_add_perso_groupe(GtkWidget *ChildWidget, struct callback_nv_nom * c_b);
void ferme_nv_nom(GtkWidget *ChildWidget , GtkWidget * fenetre);
void feuille_groupe(FenetrePerso * fenetre);
void nouveau_groupe(GtkWidget *ChildWidget, FenetrePerso * _perso_appel);
char * chaine_groupe_armure(GMarkupDomNode * tmp_node, char * ch);

void enregistre_groupe(GtkWidget *ChildWidget , FenetrePerso * fenetre)
{
    FILE * fichier;
    char ch[LONG],aff[LONG],chaine[LONG],chemin_gr[LONG],*tmp,*tmp1,*tmp2;
    GtkWidget *p_dialog = NULL;
    signed short i, j;

    init_liste_sort();
    sprintf(aff,"%s",fenetre->perso.nom_fichier);
    if (compare_sans_casse(aff+strlen(aff)-4,".grp")==0)
    { /* aff a l'extension => on la supprime dans aff et la maintient dans ch*/
        strncpy(ch,aff,LONG-1);
        aff[strlen(aff)-4]=0;
    }
    else
    {/* aff n'a pas l'extension => on la rajoute dans ch*/
        strncpy(ch,aff,LONG-1);
        strncat(ch,".grp",LONG-1);
    }
    strncpy(aff+strlen(aff),"_grp.odt",LONG-1-strlen(aff));
    fichier = g_fopen(aff, "rb");
    if (fichier!=NULL) /* Le fichier existe déjà */
    {
        fclose(fichier);
        for (i=0;aff[i]!=0;i++)
        {
            chaine[i]=aff[i];
        }
        chaine[i]='~';
        chaine[i+1]=0;
        copie_fichier(chaine,aff);
    }
    else
    {
    }

    envp_mem_hist(ch);
    /*envp_init(((gpointer *)param)[8]);*/
    fichier = g_fopen(ch, "rb");
    if (fichier!=NULL) /* Le fichier existe déjà */
    {
        fclose(fichier);
        for (i=0;ch[i]!=0;i++)
        {
            chaine[i]=ch[i];
        }
        chaine[i]='~';
        chaine[i+1]=0;
        copie_fichier(chaine,ch);
    }
    else
    {
    }
    strcpy(chaine,"Fichiers de sauvegarde\n\nSauvegarde pour nouveau travail\n");
    strncat(chaine,ch,LONG-1);
    strncat(chaine,"\n\nRésumé du groupe\n",LONG-1);
    strncat(chaine,aff,LONG-1);

    p_dialog = gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_OTHER,GTK_BUTTONS_OK,"%s",chaine);
    gtk_dialog_run(GTK_DIALOG(p_dialog));
    gtk_widget_destroy(p_dialog);

    feuille_groupe(fenetre);
    fichier = g_fopen(ch, "w");
    if (fichier!=NULL)
    {
        g_fprintf(fichier,"<document type=\"groupe\">\n");
        tmp2=NULL;
        strcpy(chemin_gr,fenetre->perso.nom_fichier);
        for(i=strlen(chemin_gr)-1;i>=0 && chemin_gr[i]!=SEPARATEUR;i--);
        if (i>=0) /* on ne retient que le chemin, le nom est au bout */
        {
            chemin_gr[i]=0;
        }
        else
        {
        }
        if (fenetre->uri!=NULL)
        {
            for(i=0;fenetre->uri[i]!=NULL;i++) /*i */
            {
                strcpy(chaine,fenetre->uri[i]);
                for(j=strlen(chaine)-1;j>=0 && chaine[j]!=SEPARATEUR;j--);
                if (compare_sans_casse(chaine+strlen(chaine)-5,".pers")==0)
                {
                    chaine[strlen(chaine)-5]=0;
                }
                else
                {
                }
                if (j>=0) /* on ne retient que le chemin */
                {
                    chaine[j]=0;
                }
                else
                {
                }
                for (tmp1=chemin_gr,tmp=chaine;*tmp!=0 && *tmp1!=0 && *tmp1==*tmp;tmp++,tmp1++)
                {
                    if (*tmp==SEPARATEUR)
                    {
                        tmp2=tmp;
                    }
                    else
                    {
                    }
                }
                if (*tmp1==0 && *tmp==0)
                { /* même répertoire */
                    g_fprintf(fichier," <perso>.%c",SEPARATEUR);
                    ecrit_xml(fichier,chaine+strlen(chaine)+1); /* juste le nom */
                    g_fprintf(fichier,"</perso>\n");
                }
                else if (tmp2!=NULL)
                {
                    if (*tmp==SEPARATEUR && *tmp1==0)
                    {
                        g_fprintf(fichier," <perso>");
                        ecrit_xml(fichier,tmp+1); /* le chemin pour aller vers le fichier : répertoire dans le répertoire du groupe */
                        g_fprintf(fichier,"%c",SEPARATEUR);
                        ecrit_xml(fichier,chaine+strlen(chaine)+1);
                        g_fprintf(fichier,"</perso>\n");
                    }
                    else
                    { /* le groupe est dans un répertoire où se trouve le perso */
                        if (*tmp==0 && *tmp1==SEPARATEUR)
                        {
                            ch[0]=0;
                        }
                        else
                        {
                            sprintf(ch,"..%c%s",SEPARATEUR,tmp2+1);
                        }
                        while(*tmp1!=0)
                        { /* répertoire différent, mais pas dans l'arborescence de  ((char **)param)[1] */
                            if (*tmp1==SEPARATEUR)
                            {
                                if (*ch==0)
                                {
                                    aff[0]='.';
                                    aff[1]='.';
                                    for (i=0;ch[i]!=0 && i<LONG-2;i++)
                                    {
                                        aff[i+2]=ch[i];
                                    }
                                    if (i<LONG-2)
                                    {
                                        aff[i+2]=0;
                                    }
                                    else
                                    {
                                        aff[LONG-1]=0;
                                    }
                                }
                                else
                                {
                                    aff[0]='.';
                                    aff[1]='.';
                                    aff[2]=SEPARATEUR;
                                    for (j=0;ch[j]!=0 && i<LONG-3;j++)
                                    {
                                        aff[j+3]=ch[j];
                                    }
                                    if (j<LONG-3)
                                    {
                                        aff[j+3]=0;
                                    }
                                    else
                                    {
                                        aff[LONG-1]=0;
                                    }
                                }
                                strcpy(ch,aff);
                            }
                            else
                            {
                            }

                            tmp1++;
                        }
                        g_fprintf(fichier," <perso>");
                        ecrit_xml(fichier,ch);
                        g_fprintf(fichier,"%c",SEPARATEUR);
                        ecrit_xml(fichier,chaine+strlen(chaine)+1);
                        g_fprintf(fichier,"</perso>\n");
                    }
                }
                else /* reprendre à la racine, rien en commun */
                {
                    g_fprintf(fichier," <perso>");
                    ecrit_xml(fichier,fenetre->uri[i]);
                    g_fprintf(fichier,"</perso>\n");
                }
            }
        }
        else
        {
            /* personne dans le groupe */
        }
        g_fprintf(fichier,"</document>\n");

        fclose(fichier);
    }
    else
    {
        strncat(ch," ne peut pas s'ouvrir",LONG-1);
        dialogue(ch,0);
    }
    (void)ChildWidget;
}

void enregistre_groupe_sous(GtkWidget *ChildWidget, FenetrePerso * fenetre)
{
    struct callback_nv_nom * c_b=(struct callback_nv_nom *)g_malloc(sizeof(struct callback_nv_nom));

    c_b->widgets_fichier=ouverture_glade_retour("nom_groupe.glade",GLADE_NON_CONNECT);
    if (c_b->widgets_fichier!=NULL)
    {
        c_b->fenetre_perso=fenetre;
        gtk_builder_connect_signals(c_b->widgets_fichier->builder,c_b);
        gtk_window_set_title(GTK_WINDOW(gtk_builder_get_object(c_b->widgets_fichier->builder,"window")),"Choix d'un nouveau nom de groupe");
    }
    else
    {
        printf("Erreur à l'ouverture de nom_groupe.glade\n");
        g_free(c_b);
    }

    (void)ChildWidget;
}

void  nouveau_nom_groupe(GtkWidget *ChildWidget, struct callback_nv_nom * c_b)
{
    char * _nom, * pt_n,  * tmp;

    _nom=gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(gtk_builder_get_object(c_b->widgets_fichier->builder,"window")));
    if (_nom!=NULL)
    {
        g_free(c_b->fenetre_perso->perso.nom_fichier);
        if (meme_extension(_nom,".grp")==1)
        {
            c_b->fenetre_perso->perso.nom_fichier=g_strdup(_nom);
        }
        else
        {
            c_b->fenetre_perso->perso.nom_fichier=(char *)g_malloc(5+strlen(_nom));
            sprintf(c_b->fenetre_perso->perso.nom_fichier,"%s.grp",_nom);
        }
        pt_n=_nom;
        for (tmp=_nom;*tmp!=0;tmp++)
        {
            if (*tmp==SEPARATEUR)
            {
                pt_n=tmp+1;
            }
            else
            {
            }
        }
        gtk_window_set_title(GTK_WINDOW(gtk_builder_get_object(c_b->fenetre_perso->resume->builder,"window")),pt_n);
        enregistre_groupe(NULL , c_b->fenetre_perso );
        g_free(_nom);
    }
    else
    {
        printf("Aucun nom choisi\n");
    }
    gtk_widget_destroy(GTK_WIDGET(c_b->widgets_fichier->window));
    g_free(c_b);

    (void)ChildWidget;

}

void ajouter_perso_groupe(GtkWidget *ChildWidget, FenetrePerso * fenetre)
{
    struct callback_nv_nom * c_b=(struct callback_nv_nom *)g_malloc(sizeof(struct callback_nv_nom));

    c_b->widgets_fichier=ouverture_glade_retour("nom_perso.glade",GLADE_NON_CONNECT);
    if (c_b->widgets_fichier!=NULL)
    {
        c_b->fenetre_perso=fenetre;
        g_signal_connect(gtk_builder_get_object(c_b->widgets_fichier->builder,"validation"), "clicked", G_CALLBACK (callback_add_perso_groupe),c_b);
        gtk_builder_connect_signals(c_b->widgets_fichier->builder,c_b);
        gtk_window_set_title(GTK_WINDOW(gtk_builder_get_object(c_b->widgets_fichier->builder,"window")),"Ajout d'un nouveau personnage dans le groupe");
    }
    else
    {
        printf("Erreur à l'ouverture de nom_perso.glade\n");
        g_free(c_b);
    }

    (void)ChildWidget;
}

void info_fichier_groupe(GtkFileChooser *chooser, struct callback_nv_nom * c_b)
{
    info_fichier(chooser, c_b->widgets_fichier);
}

void callback_add_perso_groupe(GtkWidget *ChildWidget, struct callback_nv_nom * c_b)
{
    unsigned short i,nb;
    GSList * liste, * tmp=NULL;

    liste=gtk_file_chooser_get_filenames (GTK_FILE_CHOOSER(gtk_builder_get_object(c_b->widgets_fichier->builder,"window")));

    if (c_b->fenetre_perso->uri==NULL)
    {   /* premier d'un nouveau groupe */
        nb=0;
    }
    else
    {
        for (nb=0;c_b->fenetre_perso->uri[nb]!=NULL;nb++); /* trouver le dernier groupe */
    }
    for (tmp=liste;tmp!=NULL;tmp=tmp->next)
    {
        c_b->fenetre_perso->uri=(char **)g_realloc(c_b->fenetre_perso->uri,(nb+2)*sizeof(char *));
        /* vérifier s'il n'est pas déjà dans la liste */
        for (i=0;i<nb && compare_sans_casse((char *)tmp->data,c_b->fenetre_perso->uri[i])!=0;i++)
        {
        }
        if (i==nb)
        { /* nouveau nom dans la liste */
            nouveau_membre_groupe(c_b->fenetre_perso,(char *)tmp->data,nb);
        }
        else
        {
            ouvre_perso((char *)tmp->data);/* un nom déjà dans la liste */
        }
        nb++; /* avant de passer au suivant, on incrémente */
    }
    c_b->fenetre_perso->uri[nb]=NULL;

    g_slist_free(liste);
    ferme_glade(ChildWidget , c_b->widgets_fichier);
}

void ferme_nv_nom(GtkWidget *ChildWidget , GtkWidget * fenetre)
{
    gtk_widget_destroy(fenetre);

    (void)ChildWidget;
}

void feuille_groupe(FenetrePerso * fenetre)
{
    struct zip * f_zip=NULL;
    struct zip_source * n_zip=NULL;
    char * chemin=NULL, ch[LONG],affichage[LONG], aff[LONG],*tmp1=NULL, *nom_de_fichier=NULL, *tmp=NULL;
    chemin=chemin_exe();
    sprintf(ch,"%s%cLibO%cgroupe.xml",chemin,SEPARATEUR,SEPARATEUR);
    g_free(chemin);
    GMarkupDomNode * ooo=g_markup_dom_new(ch,NULL),* tmp_node=NULL,* tmp1_node=NULL,* tmp2_node=NULL ,* node=NULL, *pers=NULL;
    unsigned short i,k,caract;
    signed short vol,j,vd=-1,version_=ADD2,mag=0,clerc=0,sage,inte,num_classe,nb;
    FILE * fichier;
    signed short pan=0,die=0; /* mémorisation du dieu et du panthéon */

    node=g_markup_dom_node (ooo,"office:text");
    if (node->nb_fils < 3)
    { /* Fichier groupe invalide : node->nb_fils doit être >= 3 pour accéder à node->fils+node->nb_fils-3 */
        printf("Erreur dans le fichier %s\n",ch);
    }
    else
    { /* À partir d'ici, node->nb_fils >= 3 est garanti → tous les accès à node->fils+node->nb_fils-3 sont sûrs */
        strcpy(ch,fenetre->perso.nom_fichier);
        tmp1=ch-1;
        for (tmp=ch;*tmp!=0;tmp++)
        { /* isolé le nom du fichier de la chaîne complète */
            if (*tmp==SEPARATEUR)
            {
                tmp1=tmp;
            }
            else
            {
            }
        }
        modif_xml(g_markup_dom_nom (ooo,"nom","name"),tmp1+1);

        if (fenetre->uri!=NULL)
        {
            for(nb=0;fenetre->uri[nb]!=NULL;nb++)
            {
                strncpy(ch,fenetre->uri[nb],LONG-1);
                vol=0;
                vd=-1;
                clerc=0;
                mag=0;
                xml_ajoute_fin(node,"text:section");
                copie_node(node->fils+node->nb_fils-1,node->fils+node->nb_fils-2);
                copie_node(node->fils+node->nb_fils-2,node->fils+node->nb_fils-3);
                pers=g_markup_dom_new(ch,NULL);
                if (pers!=NULL && pers->nb_fils!=0)
                {
                    tmp_node=g_markup_dom_node(pers,"version");
                    if (tmp_node!=NULL && tmp_node->nb_texte!=0)
                    {
                        for (i=0;i<3;i++)
                        {
                            if (strcmp(tmp_node->texte->texte,vers[i])==0)
                            {
                                version_=i;
                            }
                            else
                            {
                            }
                        }
                    }
                    else
                    {
                        strncpy(aff,"Fichier ",LONG-1);
                        strncat(aff,fenetre->uri[nb],LONG-1);
                        strncat(aff," sans version",LONG-1);
                        dialogue(aff,0);
                    }
                    for (i=0;i<8;i++)
                    {
                        tmp_node=g_markup_dom_nom(pers,nom_min[i],"name");
                        if (tmp_node && tmp_node->nb_texte!=0)
                        {
                            modif_xml(g_markup_dom_nom(node->fils+node->nb_fils-3,nom_min[i],"name"),tmp_node->texte->texte);
                            switch(i)
                            {
                                case 0 : /* force */
                                    if(strcmp(tmp_node->texte->texte,"18")==0)
                                    {
                                        tmp_node=g_markup_dom_nom(pers,"pourcentage","name");
                                        if(strcmp(tmp_node->texte->texte,"00")!=0)
                                        {
                                            modif_xml(g_markup_dom_nom(node->fils+node->nb_fils-3,"pourcentage","name"),tmp_node->texte->texte);
                                        }
                                    }
                                    break;
                                case 2 : /*sagesse */
                                     sscanf(tmp_node->texte->texte,"%hu",&caract); /* bonus save */
                                     modif_xml(g_markup_dom_nom (node->fils+node->nb_fils-3,"ssag","name"),tabl_sagesse[caract-1][0]);
                                     break;
                                case 3 : /* dexterité */
                                     sscanf(tmp_node->texte->texte,"%hu",&caract); /* bonus save */
                                     modif_xml(g_markup_dom_nom (node->fils+node->nb_fils-3,"sdext","name"),tabl_dexterite[version_][caract-1][0]);
                                     break;
                                default :
                                     break;
                            }
                        }
                        else
                        {
                            strncpy(aff,"Fichier ",LONG-1);
                            strncat(aff,fenetre->uri[nb],LONG-1);
                            strncat(aff," sans ",LONG-1);
                            strncat(aff,nom_min[i],LONG-1);
                            dialogue(aff,0);
                        }

                        tmp_node=g_markup_dom_nom(pers,nom_fac_vol[i],"name");
                        if (tmp_node!=NULL)
                        {
                            modif_xml(g_markup_dom_nom(node->fils+node->nb_fils-3,nom_fac_vol[i],"name"),tmp_node->texte->texte);
                            j=0;
                            sscanf(tmp_node->texte->texte,"%hd",&j);
                            vol+=j;
                        }
                        else
                        {
                            printf("Erreur pour la compétence de voleur : %s du fichier%s\n",nom_fac_vol[i],fenetre->uri[nb]);
                        }
                    }
                    if (vol==0)
                    {
                        xml_sup_node(g_markup_dom_nom(node->fils+node->nb_fils-3,"voleur","name")); /* pas de talents de voleurs */
                    }
                    else
                    { /* compétence de voleur : on ne supprime pas la ligne */
                    }
                    sscanf(g_markup_dom_nom(node->fils+node->nb_fils-3,"sagesse","name")->texte->texte,"%hd",&sage);
                    sscanf(g_markup_dom_nom(node->fils+node->nb_fils-3,"intelligence","name")->texte->texte,"%hd",&inte);

                    tmp_node=g_markup_dom_node(pers,"perso");
                    if (tmp_node!=NULL)
                    {
                        ch[0]=0;
                        if (tmp_node->nb_texte>0 && tmp_node->texte->texte!=0)
                        {
                            strcpy(ch,tmp_node->texte->texte);
                        }
                        else
                        { /* Pas de nom de personnage */
                        }
                        j=0;
                        while(ch[j]!=0)
                        {
                             j++;
                        }
                        ch[j]=' ';
                        j++;
                        ch[j]=':';
                        j++;
                        ch[j]=' ';
                        j++;
                        ch[j]=0;
                    }
                    tmp_node=g_markup_dom_node(pers,"profession");
                    if (tmp_node!=NULL)
                    {
                        for (i=0;i<tmp_node->nb_fils;i++)
                        {
                            if (strcmp("p",tmp_node->fils[i].nom)==0)
                            {
                                num_classe=classe_entier(tmp_node->fils[i].texte->texte);
                                while(ch[j]!=0) j++;
                                ch[j]=' ';
                                j++;
                                ch[j]=0;
                                strcpy(ch+j,tmp_node->fils[i].texte->texte);
                                while(ch[j]!=0) j++;
                                ch[j]=':';
                                j++;
                                ch[j]=0;
                                for(k=0;k<tmp_node->fils[i].nb_att;k++)
                                {
                                    if (strcmp(tmp_node->fils[i].attributs[k].nom,"niv")==0)
                                    {
                                        strcpy(ch+j,tmp_node->fils[i].attributs[k].value);
                                        k=0;
                                        sscanf(tmp_node->fils[i].attributs[k].value,"%hd",&k);


                                        if (((CLASSE[num_classe].add[ADD2].nom==NULL || version_==ADD1) && CLASSE[num_classe].add[ADD].vade_retro!=NON) || ((CLASSE[num_classe].add[ADD].nom==NULL || version_==ADD2) && CLASSE[num_classe].add[ADD2].vade_retro!=NON))
                                        {
                                            if (CLASSE[num_classe].add[ADD2].nom==NULL || version_==ADD1)
                                            {
                                                vd=max(vd,k+CLASSE[num_classe].add[ADD].vade_retro);
                                            }
                                            else
                                            {
                                                if   ( CLASSE[num_classe].add[ADD2].vade_retro!=99)
                                                {
                                                    vd=max(vd,k+ CLASSE[num_classe].add[ADD2].vade_retro);
                                                }
                                                else
                                                {
                                                    tmp1_node=g_markup_dom_node(pers,"pantheon");
                                                    if (tmp1_node!=NULL && tmp1_node->nb_texte>0)
                                                    {
                                                        pan=pantheon_entier(tmp1_node->texte[0].texte);
                                                        tmp1_node=g_markup_dom_node(tmp1_node,"dieu");
                                                        if (pan!=0 && tmp1_node!=NULL && tmp1_node->nb_texte>0)
                                                        {
                                                            die=dieu_entier(tmp1_node->texte[0].texte,pan);
                                                            if (compare_sans_casse(PANTHEONS[pan].dieu[die].vd,"repousse")==0 || compare_sans_casse(PANTHEONS[pan].dieu[die].vd,"attire")==0 )
                                                            {
                                                                vd=max(vd,PANTHEONS[pan].dieu[die].vd_delta+k);
                                                            }
                                                        }
                                                        else
                                                        {
                                                            printf("Dieu inconnu pour un prêtre\n");
                                                        }
                                                    }
                                                    else
                                                    {
                                                        printf("Surprise : erreur de panthéon pour un prêtre\n");
                                                    }
                                                }
                                            }
                                        }
                                        aff_nb_sort(node->fils+node->nb_fils-3,CLASSE+num_classe,&clerc,&mag,sage,inte,k,version_);
                                        k=tmp_node->fils[i].nb_att; /* sortie de la boucle : on a trouvé le niveau */
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        printf("Erreur pour les professions du fichier%s\n",fenetre->uri[nb]);
                    }
                    if (mag==0 && clerc==0)
                    {
                        xml_sup_node(g_markup_dom_nom(node->fils+node->nb_fils-3,"section_sorts","name"));
                    }
                    else if (mag==0)
                    {
                        xml_sup_node(g_markup_dom_nom(node->fils+node->nb_fils-3,"suppr_mag0","name"));
                        xml_sup_node(g_markup_dom_nom(node->fils+node->nb_fils-3,"suppr_mag1","name"));
                    }
                    else if (clerc==0)
                    {
                        xml_sup_node(g_markup_dom_nom(node->fils+node->nb_fils-3,"suppr_clerc0","name"));
                        xml_sup_node(g_markup_dom_nom(node->fils+node->nb_fils-3,"suppr_clerc1","name"));
                    }
                    else
                    {
                    }
                    while(ch[j]!=0) j++;
                    ch[j]=' ';
                    j++;
                    ch[j]='-';
                    j++;
                    ch[j]=' ';
                    j++;
                    ch[j]=0;
                    tmp_node=g_markup_dom_node(pers,"race");
                    if (tmp_node!=NULL)
                    {
                        strcpy(ch+j,tmp_node->texte[0].texte);
                        while(ch[j]!=0) j++;
                        ch[j]=' ';
                        j++;
                        ch[j]='-';
                        j++;
                        ch[j]=' ';
                        j++;
                        ch[j]=0;
                    }
                    else
                    {
                        printf("Erreur pour la race du fichier%s\n",fenetre->uri[nb]);
                    }
                    tmp_node=g_markup_dom_node(pers,"sexe");
                    if (tmp_node!=NULL)
                    {
                        strcpy(ch+j,tmp_node->texte[0].texte);
                        while(ch[j]!=0) j++;
                        ch[j]=' ';
                        j++;
                        ch[j]='-';
                        j++;
                        ch[j]=' ';
                        j++;
                        ch[j]=0;
                    }
                    else
                    {
                        printf("Erreur pour le sexe du fichier%s\n",fenetre->uri[nb]);
                    }
                    tmp_node=g_markup_dom_node(pers,"poids");
                    if (tmp_node!=NULL)
                    {
                        strcpy(ch+j,tmp_node->texte[0].texte);
                        while(ch[j]!=0) j++;
                        ch[j]='k';
                        j++;
                        ch[j]='g';
                        j++;
                        ch[j]=' ';
                        j++;
                        ch[j]='-';
                        j++;
                        ch[j]=' ';
                        j++;
                        ch[j]=0;
                    }
                    else
                    {
                        printf("Erreur pour le poids du fichier%s\n",fenetre->uri[nb]);
                    }
                    tmp_node=g_markup_dom_node(pers,"taille");
                    if (tmp_node!=NULL)
                    {
                        strcpy(ch+j,tmp_node->texte[0].texte);
                        while(ch[j]!=0) j++;
                        ch[j]='c';
                        j++;
                        ch[j]='m';
                        j++;
                        ch[j]=' ';
                        j++;
                        ch[j]='-';
                        j++;
                        ch[j]=' ';
                        j++;
                        ch[j]=0;
                    }
                    else
                    {
                        printf("Erreur pour la taille du fichier%s\n",fenetre->uri[nb]);
                    }
                    tmp_node=g_markup_dom_node(pers,"alignement");
                    if (tmp_node!=NULL)
                    {
                        strcpy(ch+j,tmp_node->texte[0].texte);
                    }
                    else
                    {
                        printf("Erreur pour l'alignement du fichier%s\n",fenetre->uri[nb]);
                    }
                    modif_xml(g_markup_dom_nom(node->fils+node->nb_fils-3,"nom","name"),ch); /* copie de la ligne complète */

                    tmp_node=g_markup_dom_node(pers,"pdv");
                    if (tmp_node!=NULL)
                    {
                        ch[0]=0;
                        j=0;
                        vol=0;
                        for (i=0;i<tmp_node->nb_fils;i++)
                        {
                            if (i!=0)
                            {
                                ch[j]='+';
                                j++;
                                ch[j]=0;
                            }
                            strcpy(ch+j,tmp_node->fils[i].texte->texte);
                            k=0;sscanf(tmp_node->fils[i].texte->texte,"%hd",&k);
                            vol+=k;
                            while(ch[j]!=0) j++;
                        }
                        ch[j]='=';
                        j++;
                        sprintf(ch+j,"%hd",vol);
                        tmp_node=g_markup_dom_nom(node->fils+node->nb_fils-3,"pdv","name");
                        modif_xml(tmp_node,ch);
                    }
                    else
                    {
                        printf("Erreur pour les PDV du fichier%s\n",fenetre->uri[nb]);
                    }


                    tmp_node=g_markup_dom_node(pers,"armes");
                    if (tmp_node!=NULL)
                    {
                        tmp1_node=g_markup_dom_nom(node->fils+node->nb_fils-3,"armes","name");
                        if (tmp_node->nb_fils>0)
                        {
                            tmp2_node=tmp1_node->fils+3;
                            xml_ecrit_dernier_texte(tmp2_node->fils[0].fils,tmp_node->fils[0].texte->texte);
                            xml_ecrit_dernier_texte(tmp2_node->fils[1].fils,tmp_node->fils[0].attributs[3].value);
                            xml_ecrit_dernier_texte(tmp2_node->fils[2].fils,tmp_node->fils[0].attributs[2].value);
                            if (tmp_node->nb_fils>1)
                            {
                                tmp2_node=tmp1_node->fils+4;
                                xml_ecrit_dernier_texte(tmp2_node->fils[0].fils,tmp_node->fils[1].texte->texte);
                                xml_ecrit_dernier_texte(tmp2_node->fils[1].fils,tmp_node->fils[1].attributs[3].value);
                                xml_ecrit_dernier_texte(tmp2_node->fils[2].fils,tmp_node->fils[1].attributs[2].value);
                                if (tmp_node->nb_fils>2)
                                {
                                    for(i=2;i<tmp_node->nb_fils;i++)
                                    {
                                        xml_ajoute_fin(tmp1_node,"node");
                                        tmp2_node=tmp1_node->fils+tmp1_node->nb_fils-2;
                                        copie_node(tmp1_node->fils+tmp1_node->nb_fils-1,tmp2_node);
                                        copie_node(tmp2_node,tmp1_node->fils+tmp1_node->nb_fils-3);
                                        xml_ecrit_dernier_texte(tmp2_node->fils[0].fils,tmp_node->fils[i].texte->texte);
                                        xml_ecrit_dernier_texte(tmp2_node->fils[1].fils,tmp_node->fils[i].attributs[3].value);
                                        xml_ecrit_dernier_texte(tmp2_node->fils[2].fils,tmp_node->fils[i].attributs[2].value);
                                    }
                                }
                                else
                                { /* pas besoin de rajouter une ligne pour les armes */
                                }
                            }
                            else if (tmp_node->nb_fils==1)
                            {
                                 supprime_node(tmp1_node,tmp1_node->nb_fils-2);
                            }
                            else
                            { /* pas d'arme */
                            }
                        }
                        else if (tmp_node->nb_fils==1)
                        {
                             supprime_node(tmp1_node,tmp1_node->nb_fils-2);
                        }
                        else
                        {
                        }
                    }
                    else
                    {
                        printf("Erreur pour les armes du fichier%s\n",fenetre->uri[nb]);
                    }

                    tmp_node=g_markup_dom_node(pers,"armure1");
                    chaine_groupe_armure(tmp_node,ch);
                    if (*ch==0)
                    {
                        printf("Erreur pour l'armure1 du fichier%s\n",fenetre->uri[nb]);
                    }
                    else
                    {
                        xml_ecrit_dernier_texte(g_markup_dom_nom(node->fils+node->nb_fils-3,"armure1","name"),ch);
                    }

                    tmp_node=g_markup_dom_node(pers,"armure2");
                    chaine_groupe_armure(tmp_node,ch);
                    if (*ch==0)
                    {
                        printf("Erreur pour l'armure2 du fichier%s\n",fenetre->uri[nb]);
                    }
                    else
                    {
                        xml_ecrit_dernier_texte(g_markup_dom_nom(node->fils+node->nb_fils-3,"armure2","name"),ch);
                    }

                    tmp_node=g_markup_dom_node(pers,"save");
                    if (tmp_node!=NULL)
                    {
                        /* très mauvaise méthode non xml : les attributs sont obligatoirement dans cet ordre :( */
                        xml_ecrit_dernier_texte(g_markup_dom_nom(node->fils+node->nb_fils-3,"spmp","name"),tmp_node->attributs[0].value);
                        xml_ecrit_dernier_texte(g_markup_dom_nom(node->fils+node->nb_fils-3,"sbb","name"),tmp_node->attributs[1].value);
                        xml_ecrit_dernier_texte(g_markup_dom_nom(node->fils+node->nb_fils-3,"spp","name"),tmp_node->attributs[2].value);
                        xml_ecrit_dernier_texte(g_markup_dom_nom(node->fils+node->nb_fils-3,"ssor","name"),tmp_node->attributs[3].value);
                        xml_ecrit_dernier_texte(g_markup_dom_nom(node->fils+node->nb_fils-3,"ssou","name"),tmp_node->attributs[4].value);
                    }
                    else
                    {
                        printf("Erreur pour les saves du fichier%s\n",fenetre->uri[nb]);
                    }

                    if (vd>0)
                    {
                        vd=_min(vd,13);
                        if (version_==ADD1)
                        {
                            tmp_node=g_markup_dom_nom(node->fils+node->nb_fils-3,"liste_vd","name");
                            for(i=0;i<13;i++)
                                xml_ecrit_dernier_texte(tmp_node->fils[i].fils,vd_add1[i][vd-1]);
                        }
                        else if (version_==ADD2)
                        {
                            tmp_node=g_markup_dom_nom(node->fils+node->nb_fils-3,"liste_vd","name");
                            for(i=0;i<13;i++)
                                xml_ecrit_dernier_texte(tmp_node->fils[i].fils,vd_add2[i][vd-1]);
                        }
                        else
                        { /* ni ADD1 ni ADD2 ??? */
                            vd=0;
                        }
                    }
                    if (vd<=0)
                    {
                        xml_sup_node(g_markup_dom_nom(node->fils+node->nb_fils-3,"vd0","name"));
                        xml_sup_node(g_markup_dom_nom(node->fils+node->nb_fils-3,"vd1","name"));
                        xml_sup_node(g_markup_dom_nom(node->fils+node->nb_fils-3,"VD","name"));
                    }
                    else
                    { /* on avait rempli avant la table des vade-rétros */
                    }
                    g_markup_dom_free(pers);
                }
                else
                {
                    strncpy(aff,"Fichier ",LONG-1);
                    strncat(aff,ch,LONG-1);
                    strncat(aff," vide",LONG-1);
                    dialogue(aff,0);
                }
            }
            xml_sup_node(node->fils+node->nb_fils-2); /* suppression de la node modèle */
        }
        else
        {
            /* personne dans le groupe ! */
        }
        nom_de_fichier=NULL;
        fichier=enregistre_tmp_xml(ooo,&nom_de_fichier);
        tmp1=chemin_exe();
        sprintf(affichage,"%s%cLibO%cgroupe.zip",tmp1,SEPARATEUR,SEPARATEUR);
        strcpy(aff,fenetre->perso.nom_fichier);
        if (compare_sans_casse(aff+strlen(aff)-4,".grp")==0)
        {
            i=strlen(aff)-4;
        }
        else
        {
            i=strlen(aff);
        }
        strcpy(aff+i,"_grp.odt");
        copie_fichier(aff,affichage);
        if (nom_de_fichier)
        { /* cas Linux */
            fclose(fichier);
            fichier=g_fopen(nom_de_fichier,"r");
        }
        else
        { /* cas windows */
        }
        f_zip=zip_open(aff,ZIP_CREATE,NULL);
        n_zip=zip_source_filep(f_zip,fichier,0,0); /* récupération du fichier sous forme FILE * sans avoir à le copier sur le disque */
        if (nom_de_fichier)
        { /* cas linux */
            fclose(fichier);
            g_free(nom_de_fichier);
            nom_de_fichier=NULL;
        }
        else
        { /* cas windows */
        }
        zip_file_replace(f_zip,zip_name_locate(f_zip,"content.xml",ZIP_FL_NOCASE) ,n_zip,ZIP_FL_OVERWRITE);
        zip_close(f_zip);
        //fclose(fichier);

        while (echange_node(g_markup_dom_nom (ooo,"case_dexterite","name"),g_markup_dom_nom (ooo,"case_intelligence","name")))
        { /* passage à l'ordre de ADD2 */
            echange_node(g_markup_dom_nom (ooo,"case_constitution","name"),g_markup_dom_nom (ooo,"case_sagesse","name"));
            echange_node(g_markup_dom_nom (ooo,"case_petrification","name"),g_markup_dom_nom (ooo,"case_baguette","name"));
            xml_ecrit_dernier_attribut(g_markup_dom_nom (ooo,"case_dexterite","name"),"name","f_case_dexterite");
            xml_ecrit_dernier_attribut(g_markup_dom_nom (ooo,"case_intelligence","name"),"name","f_case_intelligence");
            xml_ecrit_dernier_attribut(g_markup_dom_nom (ooo,"case_constitution","name"),"name","f_case_constitution");
            xml_ecrit_dernier_attribut(g_markup_dom_nom (ooo,"case_sagesse","name"),"name","f_case_sagesse");
            xml_ecrit_dernier_attribut(g_markup_dom_nom (ooo,"case_petrification","name"),"name","f_case_petrification");
            xml_ecrit_dernier_attribut(g_markup_dom_nom (ooo,"case_baguette","name"),"name","f_case_baguette");
        }
        fichier=enregistre_tmp_xml(ooo,&nom_de_fichier);
        strcpy(aff+i,"_grp_add2.odt");
        copie_fichier(aff,affichage);
        if (nom_de_fichier)
        { /* cas linux */
            fclose(fichier);
            fichier=g_fopen(nom_de_fichier,"r");
        }
        else
        { /* cas windows */
        }
        f_zip=zip_open(aff,ZIP_CREATE,NULL);
        n_zip=zip_source_filep(f_zip,fichier,0,0); /* récupération du fichier sous forme FILE * sans avoir à le copier sur le disque */
        if (nom_de_fichier)
        {   /* cas linux */
            fclose(fichier);
            g_free(nom_de_fichier);
            nom_de_fichier=NULL;
        }
        else
        { /* cas windows*/
        }
        zip_file_replace(f_zip,zip_name_locate(f_zip,"content.xml",ZIP_FL_NOCASE) ,n_zip,ZIP_FL_OVERWRITE);
        zip_close(f_zip);

        g_free(tmp1);
    } /* fin du else : le fichier est le bon */
    g_markup_dom_free(ooo);
}

void callback_suppr_perso_groupe(GtkButton *button, perso_groupe * p_gr)
{
    unsigned short i;

    for (i=0;p_gr->groupe->uri[i]!=NULL && compare_sans_casse(p_gr->groupe->uri[i],p_gr->nom)!=0;i++); /* suprimmer ce nom du groupe */
    if (p_gr->groupe->uri[i]!=NULL)
    {
        g_free(p_gr->groupe->uri[i]);
        for (;p_gr->groupe->uri[i]!=NULL;i++)
        {
            p_gr->groupe->uri[i]=p_gr->groupe->uri[i+1];
        }
        gtk_widget_destroy(p_gr->wid);
        gestion_fenetre (NULL, p_gr->nom,p_gr->groupe); /* fermeture de la fenêtre du perso si elle existe */
        g_free(p_gr->nom);
        g_free(p_gr);
    }
    else
    {
        printf("Anormal : %s n'est pas dans le groupe\n",p_gr->nom);
    }

    (void)button;
}

signed short nouveau_membre_groupe(FenetrePerso * fenetre_perso, char * _nom,unsigned short nb)
{
    GtkWidget * wid=NULL, *wid1=NULL;
    perso_groupe * p_gr=NULL;
    FenetrePerso * local=NULL;
    GdkRectangle rr;
    unsigned short hauteur=600, largeur=800;
    signed short retour;
    char ch[LONG]="  ";
    signed short i;


    local=ouvre_perso(_nom); /* ouverture de la fenêtre perso */
    if (local!=NULL)
    {
        /* placement de la fenêtre sur l'écran pour éviter que les fenêtres d'un groupe se chevauchent */
#if GTK_CHECK_VERSION(3, 22, 0)
        if (gdk_display_get_default() && gdk_display_get_primary_monitor (gdk_display_get_default()))
        {
            gdk_monitor_get_geometry(gdk_display_get_primary_monitor (gdk_display_get_default()), &rr);
            hauteur=rr.height-100;
            largeur=rr.width-100;
        }
        else
        {
            hauteur=600;
            largeur=800;
        }
#else
        hauteur=gdk_screen_get_height(gdk_screen_get_default ())-100;
        largeur=gdk_screen_get_width(gdk_screen_get_default ())-100;
#endif // GTK_CHECK_VERSION
        gtk_window_move (GTK_WINDOW(local->resume->window),(20+13*nb)%largeur,(60+37*nb)%hauteur);
        /* mise en place de la chaîne des persos (paramètre uri) d'un groupe */
        fenetre_perso->uri=(char **)g_realloc(fenetre_perso->uri,(nb+2)*sizeof(char *));
        fenetre_perso->uri[nb+1]=NULL;
        fenetre_perso->uri[nb]=g_strdup(local->perso.nom_fichier);  /* on mémorise le nouveau nom */

        /* mise en place des boutons dans la fenetre groupe */
        wid=gtk_box_new (GTK_ORIENTATION_HORIZONTAL,0); /* le conteneur horizontal */
        p_gr=(perso_groupe *)g_malloc(sizeof(perso_groupe)); /* pour passer l'info par callback */
        p_gr->wid=wid; /* le nom et les 3 boutons dans la fenêtre de groupe */
        p_gr->nom=g_strdup(local->perso.nom_fichier);
        p_gr->groupe=fenetre_perso;
        //wid1=gtk_button_new_with_label("Enlever du groupe");
        wid1=gtk_button_new_from_icon_name("edit-delete",GTK_ICON_SIZE_BUTTON);
        gtk_button_set_label (GTK_BUTTON(wid1),"Enlever du groupe");
        gtk_box_pack_end (GTK_BOX(wid),wid1,FALSE,FALSE,0);
        g_signal_connect(wid1, "clicked", G_CALLBACK (callback_suppr_perso_groupe),p_gr);
        if (local->perso.nom!=NULL && local->perso.nom[0]!=0)
        {
            strcat(strcat(ch,local->perso.nom),"  ");
        }
        else
        {
            strcat(strcat(ch,local->perso.nom_fichier),"  ");
        }
        if (local->perso.classe[0]!=-1)
        {
            strcat(ch,CLASSE[local->perso.classe[0]].nom);
            i=1;
            while(local->perso.classe[i]!=-1)
            {
                strcat(strcat(ch,"/"),CLASSE[local->perso.classe[i]].nom);
                i++;
            }
            strcat(ch,"  ");
        }
        wid1=gtk_label_new(ch);
        gtk_box_pack_end (GTK_BOX(wid),wid1,TRUE,FALSE,0);
        //wid1=gtk_button_new_with_label("Éditer le personnage");
        wid1=gtk_button_new_from_icon_name("document-page-setup",GTK_ICON_SIZE_BUTTON);
        gtk_button_set_label (GTK_BUTTON(wid1),"Éditer le personnage");
        g_signal_connect(wid1, "clicked", G_CALLBACK (callback_ouvre_perso),fenetre_perso->uri[nb]);
        gtk_box_pack_end (GTK_BOX(wid),wid1,FALSE,FALSE,0); /* le dernier bouton dans wid */
        gtk_box_pack_end (GTK_BOX(gtk_builder_get_object(fenetre_perso->resume->builder,"v_box_groupe")),wid,FALSE,FALSE,0); /* wid dans le fenêtre */
        gtk_widget_show_all(wid);
        retour=1;
    }
    else
    {
        retour=0;
    }

    return retour;
}

void nouveau_groupe(GtkWidget *ChildWidget, FenetrePerso * _perso_appel)
{   /* si ChildWidget vaut NULL ça veut dire qu'on arrive de enregistre perso avec un nom vierge */
  GtkWidget *p_dialog = NULL;
  char * nom_de_fichier=NULL, *tmp=NULL, ch[LONG];
  GtkFileFilter * filtre= gtk_file_filter_new();
  FILE * fp=NULL;
  FenetrePerso * _perso=NULL;

  if (ChildWidget==NULL)
  {
      tmp="Sauvegarder le nouveau fichier";
      donne_info("Fichier groupe.c, fonction nouveau_groupe, passage dans ChildWidget==NULL\n"); /* je ne sais plus pourquoi il y a ce cas ni à quoi sert ce test ! */
  }
  else
  {
      tmp="Sauvegarder dans le fichier";
  }

  p_dialog = gtk_file_chooser_dialog_new ("Sauvegarder le groupe dans le fichier", NULL,
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          "Annuler", GTK_RESPONSE_CANCEL,
                                          "Enregistrer sous",GTK_RESPONSE_ACCEPT,
                                          NULL);
  tmp=envp_donne_home();
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(p_dialog),tmp);
  g_free(tmp);
  tmp=NULL;

  gtk_file_filter_add_pattern (filtre,"*.grp");
  gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(p_dialog),filtre);

  if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
  {
    nom_de_fichier = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
    gtk_widget_destroy(p_dialog);
    fp = g_fopen(nom_de_fichier, "rb");
    if (fp!=NULL) /* Le fichier existe déjà */
    {
        fclose(fp);
        sprintf(ch,"Écraser le fichier\n%s",nom_de_fichier);
        p_dialog = gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
        switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
        {
            case GTK_RESPONSE_NO:
                tmp=NULL;
                break; /* on sort du programme */
            default :
                tmp=verif_ext(nom_de_fichier,".grp");
                break;
        }
        gtk_widget_destroy(p_dialog);
    }
    else
    {
        tmp=verif_ext(nom_de_fichier,".grp");
        g_fclose(g_fopen(tmp,"w"));
    }
    g_free(nom_de_fichier);
    nom_de_fichier=NULL;
  }
  else
  {
     gtk_widget_destroy (p_dialog);
  }

  if (tmp!=NULL)
  {
    envp_mem_home_rep(tmp);
    _perso=gestion_fenetre(NULL, tmp, NULL);
    if (_perso!=NULL)
    {
        gtk_window_move (GTK_WINDOW(_perso->resume->window),5,15);
        _perso->perso.classe=NULL;
        _perso->perso.niveau=NULL;
        _perso->perso.XP=NULL;
        _perso->perso.nom=NULL;
        _perso->perso.joueur=NULL;
        _perso->perso.competence=NULL;
        _perso->perso.commentaire=NULL;
        _perso->perso.pdv=NULL;
        _perso->perso.niv_arme=NULL;
        _perso->perso.niv_competence=NULL;
        _perso->perso.niv_psi=NULL;
        _perso->perso.psi=NULL;
        _perso->perso.arme=NULL;
        _perso->perso.mag_arme=NULL;
        _perso->perso.arme_autre_t=NULL;
        _perso->perso.arme_autre_d=NULL;
        _perso->perso.bouclier=NULL;
        _perso->perso.bague=NULL;
        _perso->perso.cheveux=NULL;
        _perso->perso.yeux=NULL;
        _perso->perso.livre_sorts=NULL;
        _perso->perso.sort_su=NULL;
        _perso->perso.natif_psi=NULL;
        _perso->perso.peau=NULL;
        _perso->perso.psi=NULL;
        _perso->perso.pantheon=NULL;
        _perso->perso.dieu=NULL;
        _perso->perso.sociale=NULL;
        _perso->uri=NULL;
    }
    else
    {
        strcpy(ch,"Impossible 'd'ouvrir le fichier ");
        strncat(ch,tmp,LONG-1);
        dialogue(ch,0);
    }
  }
  else
  {
      /* aucun nom => on ferme */
  }

  (void)_perso_appel;
}

char * chaine_groupe_armure(GMarkupDomNode * tmp_node, char * ch)
{
        unsigned short i;
        signed short j=FIN;

        ch[0]=0;
        if (tmp_node!=NULL)
        {
            if (tmp_node->nb_texte>0)
            {
                strncpy(ch,tmp_node->texte->texte,1023);
            }
            else
            {
                strncpy(ch,"armure sans nom",1023);
            }
            for (i=0;i<tmp_node->nb_att;i++)
            {
                if (compare_sans_casse(tmp_node->attributs[i].nom,"mag")==0)
                {
                    if (sscanf(tmp_node->attributs[i].value,"%hd",&j)==1)
                    {
                        if (j!=0)
                        {
                            sprintf(ch+strlen(ch)," %+hd",j);
                        }
                        else
                        {
                            /* on n'affiche pas un bonus de +-0 */
                        }
                    }
                    else
                    {
                        printf("Erreur de lecture mag armure %s, enregistrement groupe\n",ch);
                        j=FIN;
                    }
                }
            }
            for (i=0;i<tmp_node->nb_att;i++)
            {
                if (compare_sans_casse(tmp_node->attributs[i].nom,"ac")==0)
                {
                    if (sscanf(tmp_node->attributs[i].value,"%hd",&j)==1)
                    {
                            sprintf(ch+strlen(ch)," : %+hd",j);
                    }
                    else
                    {
                        printf("Erreur de lecture ac armure %s, enregistrement groupe\n",ch);
                        j=FIN;
                    }
                }
            }

        }
        else
        {
            /* pas de champ armure correspondant */
        }

        return ch;
}
