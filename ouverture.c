/*
    Génération de feuille de personnage complète AD&D 1 et 2
    Génération aléatoire de personnage

    Copyright (C) 2015 Bernard SIAUD

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "include.h"
#include <string.h>

void ouverture_liste(GtkWidget *ChildWidget , struct widgets * widgets);
void ouvre_recent (GtkRecentChooser *chooser, gpointer          user_data);
void nom_perso_repertoire(GtkFileChooser *chooser,  struct widgets * _widgets);
unsigned short trouve_origine(GMarkupDomNode * node,char * tab,signed short version);


void ouverture(GtkWidget *ChildWidget, struct widgets * _widgets)
{
    ouverture_glade("ouverture.glade",1);

    (void)_widgets;
    (void)ChildWidget;
}

void nom_perso_repertoire(GtkFileChooser *chooser,  struct widgets * _widgets)
{
    char * ch=envp_donne_home();


    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(chooser),ch);
    g_free(ch);

    (void)_widgets;
}



void  info_fichier(GtkFileChooser *chooser,  struct widgets * _widgets)
{
    gchar * _nom,ch[LONG];
    GMarkupDomNode * nodeo=NULL, *noded=NULL;
    unsigned short nb, i, j;

    _nom=gtk_file_chooser_get_filename (chooser);

    if (_nom!=NULL)
    {
        nodeo=g_markup_dom_new(_nom,NULL); /* on récupère les données dans le fichier xml dieux.add */
        if (nodeo!=NULL)
        {
            ch[0]=0;
            if (*(_nom+strlen(_nom)-1)=='s') /* c'est un .pers) */
            {
                noded=g_markup_dom_node(nodeo,"perso");
                if (noded!=NULL && noded->nb_texte>0)
                {
                    strcat(ch,noded->texte[0].texte);
                }
                else
                {
                }
                noded=g_markup_dom_node(nodeo,"race");
                if (noded!=NULL && noded->nb_texte>0)
                {
                    strcat(ch,", ");
                    strcat(ch,noded->texte[0].texte);
                }
                else
                {
                }
                noded=g_markup_dom_node(nodeo,"profession");
                if (noded!=NULL)
                {
                    nb=noded->nb_fils;
                    for (i=0;i<nb;i++)
                    {

                        strcat(ch,", ");
                        strcat(ch,noded->fils[i].texte[0].texte);
                        for (j=0;j<noded->fils[i].nb_att;j++)
                        {
                            if (compare_sans_casse(noded->fils[i].attributs[j].nom,"niv")==0)
                            {
                                strcat(ch," nv ");
                                strcat(ch,noded->fils[i].attributs[j].value);
                            }
                            else
                            {
                            }
                        }
                    }
                }
                else
                {
                }
            }
            else /* c'est un .grp */
            {
                noded=g_markup_dom_node(nodeo,"document");
                if (noded)
                {
                    nb=noded->nb_fils;
                    if (noded!=NULL)
                    {
                        for (i=0;i<nb;i++)
                        {
                            if (noded->fils[i].nb_texte>0)
                            {
                                if (ch[0]!=0)
                                {
                                    strcat(ch,", ");
                                }
                                else
                                {
                                }
                                strcat(ch,noded->fils[i].texte[0].texte);
                            }
                            else
                            { /* pas de texte => pas de nom de fichiers */
                            }
                        }
                    }
                    else
                    { /* fichier sans fils */
                    }
                }
                else
                {      /* fichier vide */
                    noded=NULL;
                }
            }
            gtk_label_set_text(GTK_LABEL((gtk_builder_get_object(_widgets->builder,"info"))),ch);
        }
        g_markup_dom_free(nodeo);
    }
    g_free(_nom);

    (void)_widgets;
}

FenetrePerso * ouvre_perso(char * _nom)
{
    GMarkupDomNode * nodeo=NULL, *noded=NULL;
    signed short nb, nv, voy;
    unsigned short i, j ,k;
    FenetrePerso * _perso=NULL;
    char ch[LONG];
    if (_nom!=NULL)
    {
        if (simplifie_uri(_nom)!=NULL)
        {
            _perso=gestion_fenetre (NULL, _nom, NULL);
            if (_perso!=NULL)
            {
                if (_perso->perso.nom==NULL) /* si ce n'est pas un perso déjà ouvert */
                {
                    nodeo=g_markup_dom_new(_nom,NULL);
                    if (nodeo!=NULL)
                    {
                        if (compare_sans_casse(_nom+strlen(_nom)-5,".pers")==0) /* c'est un .pers) */
                        {
                            noded=g_markup_dom_node(nodeo,"perso");
                            if (noded==NULL)
                            {
                                _perso->perso.nom=g_strdup("");
                                _perso->perso.joueur=g_strdup("");
                                _perso->perso.classe=(signed short *)g_malloc(sizeof(signed short));
                                _perso->niv_classe_modif=NULL;
                                _perso->perso.niveau=(signed short *)g_malloc(sizeof(signed short));
                                _perso->perso.XP=(unsigned long *)g_malloc(sizeof(unsigned long));
                                _perso->perso.classe[0]=-1;
                                _perso->perso.niveau[0]=0;
                                _perso->perso.XP[0]=0;
                                _perso->perso.sexe=1;
                                _perso->perso.sociale=g_strdup("");
                                _perso->perso.version=0;

                                _perso->perso.race=-1;
                                _perso->perso.age=0;
                                for(k=0;k<9;k++)
                                {
                                        _perso->perso.caract[k]=0;
                                }
                                        _perso->perso.pdv=NULL;
                                for (i=0;i<FEUILLE_LAR_VOL;i++)
                                {
                                        _perso->perso.tab_voleur[i]=0;
                                }
                                _perso->perso.competence=(signed short *)g_malloc(sizeof(signed short));
                                _perso->perso.niv_competence=(signed short *)g_malloc(sizeof(signed short));
                                _perso->perso.competence[0]=-1;
                                _perso->perso.niv_competence[0]=0;
                                _perso->competence_modif=NULL;

                                _perso->perso.arme=(signed short *)g_malloc(sizeof(signed short));
                                _perso->perso.niv_arme=(signed short *)g_malloc(sizeof(signed short));
                                _perso->perso.mag_arme=(signed short *)g_malloc(sizeof(signed short));
                                _perso->perso.arme_autre_t=(signed short *)g_malloc(sizeof(signed short));
                                _perso->perso.arme_autre_d=(signed short *)g_malloc(sizeof(signed short));
                                _perso->perso.arme[0]=-1;
                                _perso->armes_modif=NULL;
                                _perso->perso.niv_arme[0]=0;
                                _perso->perso.mag_arme[0]=0;
                                _perso->perso.arme_autre_t[0]=0;
                                _perso->perso.arme_autre_d[0]=0;
                                _perso->perso.alignement=5;

                                _perso->perso.cheveux=g_strdup("");
                                _perso->perso.peau=g_strdup("");
                                _perso->perso.yeux=g_strdup("");
                                _perso->perso.poids=0;
                                _perso->perso.taille=0;
                                _perso->perso.bouclier=NULL;
                                _perso->perso.bague=NULL;
                                _perso->perso.autre_ca=0;
                                _perso->perso.autre_save=0;
                                _perso->perso.armure1=0;
                                _perso->perso.mag_armure1=0;
                                _perso->perso.armure2=0;
                                _perso->perso.mag_armure2=0;
                                _perso->perso.livre_sorts=NULL;
                                _perso->perso.sort_su=NULL;
                                _perso->perso.psi=NULL;
                                _perso->perso.niv_psi=NULL;
                                _perso->perso.natif_psi=NULL;
                            }
                            else
                            {
                                if (noded!=NULL && noded->nb_texte>0)
                                {
                                    _perso->perso.nom=g_strdup(noded->texte[0].texte);
                                }
                                else
                                {
                                    _perso->perso.nom=g_strdup("");
                                }
                                noded=g_markup_dom_node(nodeo,"joueur");
                                if (noded!=NULL && noded->nb_texte>0)
                                {
                                    _perso->perso.joueur=g_strdup(noded->texte[0].texte);
                                }
                                else
                                {
                                    _perso->perso.joueur=g_strdup("");
                                }
                                noded=g_markup_dom_node(nodeo,"profession");
                                if (noded!=NULL)
                                {
                                    nb=noded->nb_fils;
                                    _perso->perso.classe=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    _perso->perso.niveau=(signed short *)g_malloc(nb    *sizeof(signed short));
                                    _perso->perso.XP=   (unsigned long *)g_malloc(nb   *sizeof(unsigned long));
                                    for (i=0;i<nb;i++)
                                    {
                                        _perso->perso.classe[i]=classe_entier(noded->fils[i].texte[0].texte);
                                        _perso->perso.niveau[i]=0;
                                        _perso->perso.XP[i]=0;
                                        for (j=0;j<noded->fils[i].nb_att;j++)
                                        {
                                            if (compare_sans_casse(noded->fils[i].attributs[j].nom,"niv")==0)
                                            {
                                                sscanf(noded->fils[i].attributs[j].value,"%hd",_perso->perso.niveau+i);
                                            }
                                            else if (compare_sans_casse(noded->fils[i].attributs[j].nom,"xp")==0)
                                            {
                                                sscanf(noded->fils[i].attributs[j].value,"%lu",_perso->perso.XP+i);
                                            }
                                            else
                                            {

                                            }
                                        }
                                    }
                                    _perso->perso.classe[nb]=-1;
                                }
                                else
                                {
                                    _perso->perso.classe=(signed short *)g_malloc(sizeof(signed short));
                                    _perso->perso.niveau=(signed short *)g_malloc(sizeof(signed short));
                                    _perso->perso.XP=(unsigned long *)g_malloc(sizeof(unsigned long));
                                    _perso->perso.classe[0]=-1;
                                    _perso->perso.niveau[0]=0;
                                    _perso->perso.XP[0]=0;
                                }
                                _perso->niv_classe_modif=NULL;
                                noded=g_markup_dom_node(nodeo,"sexe");
                                if (noded!=NULL && noded->nb_texte>0 && compare_sans_casse((char *)nom_sexe[0],noded->texte[0].texte)==0)
                                {
                                    _perso->perso.sexe=0;
                                }
                                else
                                {
                                    _perso->perso.sexe=1;
                                }
                                noded=g_markup_dom_node(nodeo,"classoc");
                                if (noded!=NULL && noded->nb_texte>0)
                                {
                                    _perso->perso.sociale=g_strdup(noded->texte[0].texte);
                                }
                                else
                                {
                                    _perso->perso.sociale=g_strdup("");
                                }
                                noded=g_markup_dom_node(nodeo,"version");
                                if (noded!=NULL && noded->nb_texte>0 && compare_sans_casse((char *)vers[1],noded->texte[0].texte)==0)
                                {
                                    _perso->perso.version=1;
                                }
                                else
                                {
                                    _perso->perso.version=0;
                                }
                                noded=g_markup_dom_node(nodeo,"race");
                                if (noded!=NULL  && noded->nb_texte>0)
                                {
                                    _perso->perso.race=race_entier(noded->texte[0].texte);
                                }
                                else
                                {
                                    _perso->perso.race=-1;
                                }
                                noded=g_markup_dom_node(nodeo,"age");
                                if (noded!=NULL && noded->nb_texte>0 )
                                {
                                    sscanf(noded->texte[0].texte,"%hu",&_perso->perso.age);
                                }
                                else
                                {
                                    _perso->perso.age=0;
                                }
                                noded=g_markup_dom_node(nodeo,"cap");
                                if (noded!=NULL && noded->nb_texte>0 )
                                {
                                    sscanf(noded->texte[0].texte,"%c",&_perso->perso.cap);
                                    if (_perso->perso.cap!='n' && _perso->perso.cap!='N')
                                    { /* o par défaut car je joue comme ça */
                                        _perso->perso.cap='o';
                                    }
                                    else
                                    {
                                        _perso->perso.cap='n';
                                    } /* deux valeurs à tester o ou n dans le programme, rien d'autre */
                                }
                                else
                                {
                                    _perso->perso.cap='o';
                                }
                                for(k=0;k<9;k++)
                                {
                                    _perso->perso.caract[k]=0;
                                }
                                noded=g_markup_dom_node(nodeo,"caracteristique");
                                if (noded!=NULL)
                                {
                                    nb=noded->nb_fils;
                                    for (i=0;i<nb;i++)
                                    {
                                        for (j=0;j<noded->fils[i].nb_att;j++)
                                        {
                                            if (compare_sans_casse(noded->fils[i].attributs[j].nom,"name")==0)
                                            {   /* on trouve l'attribut dont le nom est "name" */
                                                for(k=0;k<9;k++)
                                                {   /* on cherche si l'attribut est force, intelligence,... */
                                                    if (compare_sans_casse(noded->fils[i].attributs[j].value,(char *)nom_min[k])==0)
                                                    {
                                                        if (noded->fils[i].nb_texte>0)
                                                        {
                                                            sscanf(noded->fils[i].texte[0].texte,"%hd",_perso->perso.caract+k);
                                                        }
                                                        else
                                                        {
                                                        }
                                                        k=9;j=noded->fils[i].nb_att; /* sortir de la boucle */
                                                    }
                                                }
                                            }
                                            else
                                            {
                                            }
                                        }
                                    }
                                }
                                else
                                { /* pas de caractéristique enregistrée */
                                }
                                noded=g_markup_dom_node(nodeo,"pdv");
                                if (noded!=NULL)
                                {
                                    nb=noded->nb_fils;
                                    _perso->perso.pdv=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    for (i=0;i<=nb;i++)
                                    { /* la fin de la série des pdv est marquée par un 0 on un -1 */
                                        _perso->perso.pdv[i]=0;
                                    }
                                    for (i=0;i<nb;i++)
                                    {
                                        if (noded->fils[i].nb_texte>0)
                                        {

                                            for (j=0;j<noded->fils[i].nb_att;j++)
                                            {
                                                if (compare_sans_casse(noded->fils[i].attributs[j].nom,"niv")==0)
                                                {
                                                    sscanf(noded->fils[i].attributs[j].value,"%hd",&k);
                                                    if (k>nb)
                                                    {  /* une protection su le fichier est mal foutu, normalement k=i+1, mais on n'en sait rien... */
                                                        k=nb;
                                                    }
                                                    j=nb;
                                                }
                                                else
                                                {
                                                }
                                            }
                                            sscanf(noded->fils[i].texte[0].texte,"%hd",_perso->perso.pdv+k-1);
                                        }
                                        else
                                        {
                                        }
                                    }
                                }
                                else
                                {
                                    _perso->perso.pdv=NULL;
                                }
                                for (i=0;i<FEUILLE_LAR_VOL;i++)
                                {
                                    _perso->perso.tab_voleur[i]=0;
                                }
                                noded=g_markup_dom_node(nodeo,"voleur");
                                if (noded!=NULL)
                                {
                                    nb=noded->nb_fils;
                                    for (i=0;i<nb;i++)
                                    {
                                        if (noded->fils[i].nb_texte>0)
                                        {
                                            for (j=0;j<noded->fils[i].nb_att;j++)
                                            {
                                                if (compare_sans_casse(noded->fils[i].attributs[j].nom,"name")==0)
                                                {
                                                    for (k=0;k<FEUILLE_LAR_VOL;k++)
                                                    {
                                                        if (compare_sans_casse(noded->fils[i].attributs[j].value,(char *)nom_fac_vol[k])==0)
                                                        {
                                                            sscanf(noded->fils[i].texte[0].texte,"%hd",_perso->perso.tab_voleur+k);
                                                            j=nb;
                                                            k=FEUILLE_LAR_VOL;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                }
                                            }
                                        }
                                        else
                                        {
                                        }
                                    }
                                }
                                else
                                { /* pas de caractéristiques de voleur */
                                    for (k=0;k<FEUILLE_LAR_VOL;k++)
                                    {
                                        _perso->perso.tab_voleur[k]=0;
                                    }
                                }
                                noded=g_markup_dom_node(nodeo,"competence");
                                if (noded!=NULL)
                                {
                                    nb=noded->nb_fils;
                                    _perso->perso.competence=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    _perso->perso.niv_competence=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    for (i=0;i<nb;i++)
                                    {
                                        _perso->perso.competence[i]=competence_entier(noded->fils[i].texte[0].texte);
                                        _perso->perso.niv_competence[i]=0;
                                        for (j=0;j<noded->fils[i].nb_att;j++)
                                        {
                                            if (compare_sans_casse(noded->fils[i].attributs[j].nom,"niv")==0)
                                            {
                                                sscanf(noded->fils[i].attributs[j].value,"%hd",_perso->perso.niv_competence+i);
                                            }
                                            else
                                            {
                                            }
                                        }
                                    }
                                    _perso->perso.competence[nb]=-1; /* marquage de la fin de liste */
                                    _perso->perso.niv_competence[nb]=0;
                                }
                                else
                                {
                                    _perso->perso.competence=(signed short *)g_malloc(sizeof(signed short));
                                    _perso->perso.niv_competence=(signed short *)g_malloc(sizeof(signed short));
                                    _perso->perso.competence[0]=-1;
                                    _perso->perso.niv_competence[0]=0;
                                }
                                _perso->competence_modif=NULL;
                                noded=g_markup_dom_node(nodeo,"armes");
                                if (noded!=NULL)
                                {
                                    nb=noded->nb_fils;
                                    _perso->perso.arme=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    _perso->perso.niv_arme=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    _perso->perso.mag_arme=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    _perso->perso.arme_autre_t=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    _perso->perso.arme_autre_d=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    for (i=0;i<nb;i++)
                                    {
                                        _perso->perso.arme[i]=arme_entier(noded->fils[i].texte[0].texte);
                                        _perso->perso.niv_arme[i]=0; /* mise à 0 au départ : protection */
                                        _perso->perso.mag_arme[i]=0;
                                        _perso->perso.arme_autre_t[i]=0;
                                        _perso->perso.arme_autre_d[i]=0;
                                        for (j=0;j<noded->fils[i].nb_att;j++)
                                        {
                                            if (compare_sans_casse(noded->fils[i].attributs[j].nom,"niv")==0)
                                            {
                                                sscanf(noded->fils[i].attributs[j].value,"%hd",_perso->perso.niv_arme+i);
                                            }
                                            else if (compare_sans_casse(noded->fils[i].attributs[j].nom,"mag")==0)
                                            {
                                                sscanf(noded->fils[i].attributs[j].value,"%hd",_perso->perso.mag_arme+i);
                                            }
                                            else if (compare_sans_casse(noded->fils[i].attributs[j].nom,"autre_t")==0)
                                            {
                                                sscanf(noded->fils[i].attributs[j].value,"%hd",_perso->perso.arme_autre_t+i);
                                            }
                                            else if (compare_sans_casse(noded->fils[i].attributs[j].nom,"autre_d")==0)
                                            {
                                                sscanf(noded->fils[i].attributs[j].value,"%hd",_perso->perso.arme_autre_d+i);
                                            }
                                            else
                                            {
                                            }
                                        }
                                    }
                                    _perso->perso.arme[nb]=-1; /* marquage de la fin de liste */
                                    _perso->perso.niv_arme[nb]=0;
                                    _perso->perso.mag_arme[nb]=0;
                                    _perso->perso.arme_autre_t[nb]=0;
                                    _perso->perso.arme_autre_d[nb]=0;
                                }
                                else
                                {
                                    _perso->perso.arme=(signed short *)g_malloc(sizeof(signed short));
                                    _perso->perso.niv_arme=(signed short *)g_malloc(sizeof(signed short));
                                    _perso->perso.mag_arme=(signed short *)g_malloc(sizeof(signed short));
                                    _perso->perso.arme_autre_t=(signed short *)g_malloc(sizeof(signed short));
                                    _perso->perso.arme_autre_d=(signed short *)g_malloc(sizeof(signed short));
                                    _perso->perso.arme[0]=-1;
                                    _perso->perso.niv_arme[0]=0;
                                    _perso->perso.mag_arme[0]=0;
                                    _perso->perso.arme_autre_t[0]=0;
                                    _perso->perso.arme_autre_d[0]=0;
                                }
                                _perso->armes_modif=NULL;
                                noded=g_markup_dom_node(nodeo,"alignement");
                                if (noded!=NULL)
                                {
                                    if (noded->nb_texte>0)
                                    {
                                        for (i=0;i<9;i++)
                                        {
                                            if (compare_sans_casse((char *)algnm[i],noded->texte[0].texte)==0)
                                            {
                                                _perso->perso.alignement=i;
                                                i=9;
                                            }
                                            else
                                            {
                                            }
                                        }
                                    }
                                    else
                                    {
                                        _perso->perso.alignement=5;
                                    }
                                }
                                else
                                {
                                    _perso->perso.alignement=5;
                                }
                                noded=g_markup_dom_node(nodeo,"cheveux");
                                if (noded!=NULL && noded->nb_texte>0)
                                {
                                    _perso->perso.cheveux=g_strdup(noded->texte[0].texte);
                                }
                                else
                                {
                                    _perso->perso.cheveux=g_strdup("");
                                }
                                noded=g_markup_dom_node(nodeo,"peau");
                                if (noded!=NULL && noded->nb_texte>0)
                                {
                                    _perso->perso.peau=g_strdup(noded->texte[0].texte);
                                }
                                else
                                {
                                    _perso->perso.peau=g_strdup("");
                                }
                                noded=g_markup_dom_node(nodeo,"yeux");
                                if (noded!=NULL && noded->nb_texte>0)
                                {
                                    _perso->perso.yeux=g_strdup(noded->texte[0].texte);
                                }
                                else
                                {
                                    _perso->perso.yeux=g_strdup("");
                                }
                                noded=g_markup_dom_node(nodeo,"com");
                                if (noded!=NULL && noded->nb_texte>0)
                                {
                                    _perso->perso.commentaire=g_strdup(noded->texte[0].texte);
                                }
                                else
                                {
                                    _perso->perso.commentaire=g_strdup("");
                                }
                                noded=g_markup_dom_node(nodeo,"poids");
                                if (noded!=NULL)
                                {
                                    if (noded->nb_texte>0)
                                    {
                                        for (i=0;i<9;i++)
                                        {
                                            sscanf(noded->texte[0].texte,"%hd",&_perso->perso.poids);
                                        }
                                    }
                                    else
                                    {
                                        _perso->perso.poids=0;
                                    }
                                }
                                else
                                {
                                    _perso->perso.poids=0;
                                }
                                noded=g_markup_dom_node(nodeo,"taille");
                                if (noded!=NULL)
                                {
                                    if (noded->nb_texte>0)
                                    {
                                        for (i=0;i<9;i++)
                                        {
                                            sscanf(noded->texte[0].texte,"%hd",&_perso->perso.taille);
                                        }
                                    }
                                    else
                                    {
                                        _perso->perso.taille=0;
                                    }
                                }
                                else
                                {
                                    _perso->perso.taille=0;
                                }
                                noded=g_markup_dom_node(nodeo,"bouclier");
                                if (noded!=NULL)
                                {
                                    _perso->perso.bouclier=(signed short *)g_malloc(sizeof(signed short));
                                    _perso->perso.bouclier[0]=0; /* par précaution si aucun champ ne convient */
                                    for(i=0;i<noded->nb_att;i++)
                                    {
                                        if ( compare_sans_casse(noded->attributs[i].nom,"mag")==0)
                                        {
                                            sscanf(noded->attributs[i].value,"%hd",_perso->perso.bouclier);
                                            i=noded->nb_att;
                                        }
                                    }
                                }
                                else
                                {
                                    _perso->perso.bouclier=NULL;
                                }
                                noded=g_markup_dom_node(nodeo,"bague");
                                if (noded!=NULL)
                                {
                                    _perso->perso.bague=(signed short *)g_malloc(sizeof(signed short));
                                    _perso->perso.bague[0]=0; /* par précaution si aucun champ ne convient */
                                    for(i=0;i<noded->nb_att;i++)
                                    {
                                        if ( compare_sans_casse(noded->attributs[i].nom,"mag")==0)
                                        {
                                            sscanf(noded->attributs[i].value,"%hd",_perso->perso.bague);
                                            i=noded->nb_att;
                                        }
                                    }
                                }
                                else
                                {
                                    _perso->perso.bague=NULL;
                                }

                                noded=g_markup_dom_node(nodeo,"autre_ca");
                                if (noded!=NULL)
                                {
                                    for(i=0;i<noded->nb_att;i++)
                                    {
                                        if ( compare_sans_casse(noded->attributs[i].nom,"value")==0)
                                        {
                                            sscanf(noded->attributs[i].value,"%hd",&(_perso->perso.autre_ca));
                                            i=noded->nb_att;
                                        }
                                    }
                                }
                                else
                                {
                                    _perso->perso.autre_ca=0;
                                }

                                noded=g_markup_dom_node(nodeo,"autre_save");
                                if (noded!=NULL)
                                {
                                    for(i=0;i<noded->nb_att;i++)
                                    {
                                        if ( compare_sans_casse(noded->attributs[i].nom,"value")==0)
                                        {
                                            sscanf(noded->attributs[i].value,"%hd",&(_perso->perso.autre_save));
                                            i=noded->nb_att;
                                        }
                                    }
                                }
                                else
                                {
                                    _perso->perso.autre_save=0;
                                }

                                noded=g_markup_dom_node(nodeo,"armure1");
                                if (noded!=NULL && noded->nb_texte>0)
                                {
                                        _perso->perso.armure1=armure_entier(noded->texte[0].texte);
                                        _perso->perso.mag_armure1=0;
                                        for (j=0;j<noded->nb_att;j++)
                                        {
                                            if (compare_sans_casse(noded->attributs[j].nom,"mag")==0)
                                            {
                                                sscanf(noded->attributs[j].value,"%hd",&_perso->perso.mag_armure1);
                                            }
                                            else
                                            {

                                            }
                                        }
                                }
                                else
                                {
                                        _perso->perso.armure1=0;
                                        _perso->perso.mag_armure1=0;
                                }
                                noded=g_markup_dom_node(nodeo,"armure2");
                                if (noded!=NULL && noded->nb_texte>0)
                                {
                                    _perso->perso.armure2=armure_entier(noded->texte[0].texte);
                                    _perso->perso.mag_armure2=0;
                                    for (j=0;j<noded->nb_att;j++)
                                    {
                                        if (compare_sans_casse(noded->attributs[j].nom,"mag")==0)
                                        {
                                            sscanf(noded->attributs[j].value,"%hd",&_perso->perso.mag_armure2);
                                        }
                                        else
                                        {
                                        }
                                    }
                                }
                                else
                                {
                                    _perso->perso.armure2=0;
                                    _perso->perso.mag_armure2=0;
                                }
                                noded=g_markup_dom_node(nodeo,"pantheon");
                                if (noded!=NULL && noded->nb_texte>0)
                                {
                                    _perso->perso.pantheon=g_strdup(noded->texte[0].texte);
                                    noded=g_markup_dom_node(noded,"dieu");
                                    if (noded!=NULL && noded->nb_texte>0)
                                    {
                                        _perso->perso.dieu=g_strdup(noded->texte[0].texte);
                                    }
                                    else
                                    {
                                        _perso->perso.dieu=g_strdup("");
                                    }
                                }
                                else
                                {
                                    _perso->perso.pantheon=g_strdup("");
                                    _perso->perso.dieu=g_strdup("");
                                }
                                noded=g_markup_dom_node(nodeo,"mag");
                                if (noded!=NULL)
                                {
                                    _perso->perso.livre_sorts=(signed long **)g_malloc(9*sizeof(signed long *));
                                    _perso->perso.sort_su=(signed short **)g_malloc(9*sizeof(signed short *));
                                    for (i=0;i<9;i++)
                                    {
                                        _perso->perso.livre_sorts[i]=NULL;
                                        _perso->perso.sort_su[i]=NULL;
                                    }
                                    for (i=0;i<noded->nb_fils;i++)
                                    {
                                        if (compare_sans_casse("niv",noded->fils[i].nom)==0)
                                        { /* c'est bien une entrée niveau de sorts */
                                            nv=-1; /* marqueur pour le niveau des sorts */
                                            for (j=0;j<noded->fils[i].nb_att;j++)
                                            {
                                                if (compare_sans_casse("niv",noded->fils[i].attributs[j].nom)==0)
                                                { /* c'est bien une entrée sort */
                                                    sscanf(noded->fils[i].attributs[j].value,"%hd",&nv);
                                                    j=noded->fils[i].nb_att;
                                                }
                                                else
                                                {
                                                }
                                            }
                                            if (nv>0 && nv<=9 && _perso->perso.livre_sorts[nv]==NULL)
                                            { /* parcours des sorts de niveau nv, premier passage : un second szerait une erreur */
                                                nb=0;
                                                for (j=0;j<noded->fils[i].nb_fils;j++)
                                                {
                                                    if (compare_sans_casse("sort",noded->fils[i].fils[j].nom)==0)
                                                    { /* c'est bien une entrée sort */
                                                        nb++;
                                                        _perso->perso.livre_sorts[nv-1]=(signed long *)realloc(_perso->perso.livre_sorts[nv-1],(nb+1)*sizeof(signed long));
                                                        _perso->perso.sort_su[nv-1]=(signed short *)realloc(_perso->perso.sort_su[nv-1],(nb+1)*sizeof(signed short));
                                                        _perso->perso.livre_sorts[nv-1][nb-1]=NON;
                                                        _perso->perso.sort_su[nv-1][nb-1]=NON;
                                                        for (k=0;k<noded->fils[i].fils[j].nb_att;k++)
                                                        {
                                                            if (compare_sans_casse("niv",noded->fils[i].fils[j].attributs[k].nom)==0)
                                                            { /* niveau de connaissance */
                                                                sscanf(noded->fils[i].fils[j].attributs[k].value,"%hd",_perso->perso.sort_su[nv-1]+nb-1);
                                                            }
                                                            else if (compare_sans_casse("clef",noded->fils[i].fils[j].attributs[k].nom)==0)
                                                            { /* niveau de comprehension */
                                                                sscanf(noded->fils[i].fils[j].attributs[k].value,"%ld",_perso->perso.livre_sorts[nv-1]+nb-1);
                                                            }
                                                            else if (compare_sans_casse("voyage",noded->fils[i].fils[j].attributs[k].nom)==0)
                                                            { /* livre de voyage */
                                                                voy=0;
                                                                sscanf(noded->fils[i].fils[j].attributs[k].value,"%hd",&voy);
                                                            }
                                                            else
                                                            {
                                                            }
                                                        }
                                                        if (_perso->perso.livre_sorts[nv-1][nb-1]==NON)
                                                        {/* sort non compris */
                                                            if (noded->fils[i].fils[j].nb_texte>0)
                                                            {
                                                                _perso->perso.livre_sorts[nv-1][nb-1]=sort_entier(noded->fils[i].fils[j].texte[0].texte,nv,_perso->perso.version);
                                                            }
                                                            else
                                                            {
                                                            }
                                                        }
                                                        if (_perso->perso.livre_sorts[nv-1][nb-1]==NON)
                                                        {
                                                            nb--;
                                                            printf("sort niveau %hd non compris : <sort",nv);
                                                            for (k=0;k<noded->fils[i].fils[j].nb_att;k++)
                                                            {
                                                                printf(" %s=\"%s\"",noded->fils[i].fils[j].attributs[k].nom,noded->fils[i].fils[j].attributs[k].value);
                                                            }
                                                            printf(">");
                                                            for (k=0;k<noded->fils[i].fils[j].nb_texte;k++)
                                                            {
                                                                printf("%s",noded->fils[i].fils[j].texte[k].texte);
                                                            }
                                                            printf("</sort>\n");
                                                        }
                                                        else
                                                        {
                                                            if (_perso->perso.sort_su[nv-1][nb-1]==NON)
                                                            {
                                                                _perso->perso.sort_su[nv-1][nb-1]=PAS_REGARDE;
                                                            }
                                                            else
                                                            {
                                                            }
                                                            if (voy==0)
                                                            { /* pas dans le livre de voyage */
                                                                _perso->perso.sort_su[nv-1][nb-1]*=-1;
                                                            }
                                                            else
                                                            {
                                                            }
                                                        }
                                                    }
                                                    else
                                                    { /* entrée autre que sort : pas normal */
                                                    }
                                                }
                                                _perso->perso.sort_su[nv-1][nb]=0; /* on marque la fin du niveau */
                                            }
                                            else
                                            {
                                            }
                                        }
                                        else
                                        {
                                        }
                                    }
                                }
                                else
                                {
                                    _perso->perso.livre_sorts=NULL;
                                    _perso->perso.sort_su=NULL;
                                }
                                noded=g_markup_dom_node(nodeo,"psi");
                                if (noded!=NULL && noded->nb_fils>0)
                                {
                                    nb=noded->nb_fils;
                                    _perso->perso.psi=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    _perso->perso.niv_psi=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    _perso->perso.natif_psi=(signed short *)g_malloc((nb+1)*sizeof(signed short));
                                    for(i=0;i<=nb;i++)
                                    {
                                        _perso->perso.psi[i]=-1;
                                        _perso->perso.niv_psi[i]=0;
                                        _perso->perso.natif_psi[i]=-1;
                                    }
                                    j=0;
                                    for(i=0;i<nb;i++)
                                    {
                                        if (compare_sans_casse("talent",noded->fils[i].nom)==0)
                                        {
                                            for (k=0;k<noded->fils[i].nb_att;k++)
                                            {
                                                if (compare_sans_casse("niv",noded->fils[i].attributs[k].nom)==0)
                                                { /* niveau de connaissance */
                                                    sscanf(noded->fils[i].attributs[k].value,"%hd",_perso->perso.niv_psi+j);
                                                }
                                                else if (compare_sans_casse("clef",noded->fils[i].attributs[k].nom)==0)
                                                { /* niveau de comprehension */
                                                    sscanf(noded->fils[i].attributs[k].value,"%hd",_perso->perso.psi+j);
                                                }
                                                else if (compare_sans_casse("natif",noded->fils[i].attributs[k].nom)==0)
                                                { /* livre de voyage */
                                                    voy=0;
                                                    sscanf(noded->fils[i].attributs[k].value,"%hd",_perso->perso.natif_psi+j);
                                                }
                                                else
                                                {
                                                }
                                            }
                                            if (_perso->perso.psi[j]==-1) /* erreur dans la lecture du talent */
                                            {
                                                if (noded->nb_texte>0)
                                                { /* recherche du nom du talent */
                                                    /*_perso->perso.psi[j]=psi_entier(noded->fils[i].texte[0].texte);*/
                                                }
                                                else
                                                {
                                                }
                                            }
                                            else
                                            {
                                            }
                                            if (_perso->perso.psi[j]==-1) /* erreur persistante dans la lecture du talent */
                                            {
                                                _perso->perso.niv_psi[i]=0;
                                                _perso->perso.natif_psi[i]=-1;
                                                j--;
                                            }
                                            else
                                            {
                                            }
                                            j++;
                                        }
                                    }
                                }
                                else
                                {
                                    _perso->perso.psi=NULL;
                                    _perso->perso.niv_psi=NULL;
                                    _perso->perso.natif_psi=NULL;
                                }

                                /* origine : on initialise à tout, s'il y aurait un problème, on gère */
                                _perso->origine=(char **)malloc(2*sizeof(char *));
                                for (i=0;i<2;i++)
                                {
                                    _perso->origine[i]=(char *)malloc(NB_ORIGINE[i]*sizeof(char));
                                    _perso->origine[i][0]=1; /* OK : manuel du joueur */
                                    for (k=1;k<NB_ORIGINE[i];k++)
                                    {
                                        _perso->origine[i][k]=0; /* par défaut, on interdit toutes les origines */
                                    }
                                }
                                _perso->origine[0][2]=1; /* OK arcana */
                                _perso->origine[1][22]=1; /* OK recueil de magie */
                                _perso->ori_add1_add2=1;
                                noded=g_markup_dom_node(nodeo,"origines");
                                if (noded!=NULL && noded->nb_fils>0)
                                {
                                    nb=noded->nb_fils;
                                    j=0;
                                    for (i=0;i<nb;i++)
                                    {
                                        if (compare_sans_casse(noded->fils[i].nom,"version")==0)
                                        { /* la valeur est 0 ou 1 */
                                            _perso->perso.ori_add1_add2=noded->fils[i].texte[0].texte[0]-'0';
                                        }
                                        else if (compare_sans_casse(noded->fils[i].nom,"add1")==0)
                                        {
                                            j+=trouve_origine(noded->fils+i,_perso->perso.origine[0],0);
                                        }
                                        else if (compare_sans_casse(noded->fils[i].nom,"add2")==0)
                                        {
                                            j+=trouve_origine(noded->fils+i,_perso->perso.origine[1],1);
                                        }
                                        else
                                        {
                                                printf("Erreur origine dans %s : entrée : %s\n",_nom,noded->fils[i].nom);
                                        }
                                    }
                                    if (j!=0)
                                    {
                                        /* c'est bon ! */
                                    }
                                    else
                                    {
                                        printf("Surprise origine présente dans %s, mais aucune sélectionnée\n",_nom);
                                    }
                                }
                                else
                                { /* pas l'entrée */
                                }
                            }
                            affiche_resume(_perso);
                        }
                        /* gestion d'un groupe de persos : un .grp */
                        else if (compare_sans_casse(_nom+strlen(_nom)-4,".grp")==0)  /* c'est un .grp */
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
                            _perso->perso.arme_autre_t=NULL;
                            _perso->perso.arme_autre_d=NULL;
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
                            noded=g_markup_dom_node(nodeo,"document");
                            if (noded!=NULL)
                            {
                                nb=noded->nb_fils;
                                k=0;
                                for (i=0;i<nb;i++)
                                {
                                    if (noded->fils[i].nb_texte>0)
                                    {
                                        for(j=strlen(_nom);j>0 && _nom[j]!='/' && _nom[j]!='\\'; j--);
                                        if (j>0)
                                        {
                                            _nom[j+1]=0;
                                            sprintf(ch,"%s%s",_nom,noded->fils[i].texte[0].texte);
                                        }
                                        else
                                        {
                                            sprintf(ch,"%s",noded->fils[i].texte[0].texte);
                                        }
                                        if (compare_sans_casse(ch+strlen(ch)-5,".pers")==0)
                                        { /* on a le .pers */
                                        }
                                        else
                                        {
                                            strcat(ch,".pers");
                                        }
                                        k+=nouveau_membre_groupe(_perso,ch,k);
                                    }
                                    else
                                    {
                                    }
                                }
                                if (_perso->uri!=NULL)
                                {
                                    _perso->uri[k]=NULL;
                                }
                                else
                                {
                                    /* c'est un groupe vide */
                                }
                            }
                        }
                        else
                        {
                            printf("Le fichier %s n'a pas la bonne extension\n",_nom);
                        }
                        g_markup_dom_free(nodeo);
                    }
                    else
                    {
                        printf("Le fichier %s n'a pas une structure de fichier xml\n",_nom);
                        _perso->d_r=1;
                        gestion_fenetre (NULL, _nom, _perso);
                        _perso=NULL;
                    }
                }
                else
                {
                    printf("Erreur lors de l'ouverture du fichier %s\n",_nom);
                }

            }
            else
            {   /* passer devant le perso déjà ouvert   */
                /* c'est géré par gestion fenêtre       */
            }
        }
        else
        {
            printf("Problème avec le perso %s\n",_nom);
        }
    }
    else
    {
        /* arrivée avec un nom nul : anormal ! */
    }

    return _perso;
}

unsigned short trouve_origine(GMarkupDomNode * node,char * tab,signed short version)
{
    unsigned short i,c,drap=0;
    struct_origine * ori=_ORIGINE[version];


    for(c=0;c<NB_ORIGINE[version];c++)
    {
        tab[c]=0; /* on interdit toutes les origines */
    }

    for (i=0;i<node->nb_fils;i++)
    {
        if (node->fils[i].nb_texte>0 && node->fils[i].texte)
        {
            for(c=0;ori[c].court && compare_sans_casse(node->fils[i].texte->texte,ori[c].court)!=0;c++);

            if (ori[c].court)
            {             /* sauf celles qu'on trouve dans le fichier du perso */
                tab[c]=1;
                drap=1;
            }
            else
            { /* erreur : origine non trouvée */
            }
        }
        else
        {
            /* erreur pas de texte */
        }
    }

    return drap;
}

void  ouverture_liste(GtkWidget *ChildWidget , struct widgets * widgets)
{  /* ouverture valable même si on a sélectionné plusieurs fichiers */
    GSList * liste, * tmp;

    liste=gtk_file_chooser_get_filenames (GTK_FILE_CHOOSER(gtk_builder_get_object(widgets->builder,"window")));

    for (tmp=liste;tmp!=NULL;tmp=tmp->next)
    {
        envp_mem_hist((char *)tmp->data);
        ouvre_perso((char *)tmp->data);
    }

    g_slist_free(liste);
    ferme_glade(ChildWidget , widgets);
}

void ouvre_recent (GtkRecentChooser *chooser, gpointer          user_data)
{
    if (strncmp("file://",gtk_recent_chooser_get_current_uri (chooser),7)==0)
    {
        ouvre_perso(gtk_recent_chooser_get_current_uri (chooser)+7);
    }
    else
    {
        printf("Problème avec l'URI :\n%s\n",gtk_recent_chooser_get_current_uri (chooser));
    }

    (void)user_data;
}

void callback_ouvre_perso(GtkButton *button, char * ch)
{
    ouvre_perso(ch);

    (void)button;
}


