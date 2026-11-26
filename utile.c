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
#include <string.h>

enum
{
    M_FILE_OK,
    M_FILE_ARGUMENT_ERR,
    M_FILE_INPUT_FILE_ERR,
    M_FILE_OUTPUT_FILE_ERR,
    M_FILE_READ_ERR,
    M_FILE_WRITE_ERR,
    M_FILE_NB
};


signed short est_compatible(signed short nb_race,signed short nb_classe,signed short version)
{
    liste_classe * pt_liste_classe;
    liste_classe_race * pt_liste_classe_race;
    signed short retour=0;

    if (nb_classe<0 || nb_race<0 || version<0 || RACE[nb_race].add[version].nom==NULL)
    {
        retour=0;
    }
    else
    {
        pt_liste_classe_race=RACE[nb_race].add[version].liste_classe;
        while(retour==0 && pt_liste_classe_race!=NULL)
        {
            pt_liste_classe=pt_liste_classe_race->classe;
            while(retour==0 && pt_liste_classe!=NULL)
            {
                if (pt_liste_classe->num==nb_classe)
                {
                    retour=1;
                }
                else
                {
                }
                pt_liste_classe=pt_liste_classe->suiv;
            }
            pt_liste_classe_race=pt_liste_classe_race->suiv;
        }
    }

    return retour;
}

signed short est_compatible_armure(signed short nb_classe,signed short nb_armure,signed short version)
{
    char ** pt_liste_armure;
    signed short retour=0; /* 0 non permise et 1 permise */
    unsigned short i;

    if (nb_classe<0 || nb_armure<0 || version<0 || CLASSE[nb_classe].add[version].nom==NULL)
    {
        retour=0;
    }
    else
    {
        pt_liste_armure=CLASSE[nb_classe].add[version].armures;
        if (strncmp(ARMURE[nb_armure].nom,"Bracelets CA",12)==0)
        { /* si l'armure est une paire de bracelets */
            retour=1;
        }
        else if ( pt_liste_armure==NULL || pt_liste_armure[0]==NULL)
        { /* pas de liste d'armures */
            retour=0;
        }
        else if (compare_sans_casse(pt_liste_armure[0],"tout")==0)
        {/* si la classe permet toutes les armures */
            retour=1;
        }
        else
        {
            i=0;
            while(retour==0 && pt_liste_armure[i]!=NULL)
            {
                if (compare_sans_casse(pt_liste_armure[i],ARMURE[nb_armure].nom)==0)
                {
                    retour=1;
                }
                else
                {
                    /* ce n'est pas l'armure cherchée */
                }
                i++;
            }
        }
    }

    return retour;
}

signed short est_multi_compatible(signed short nb_race,signed short * nb_classe,signed short version)
{
    liste_classe * pt_liste_classe;
    liste_classe_race * pt_liste_classe_race, * tmp_pt_liste_classe_race;
    signed short retour=0,tmp_retour;
    signed short i=0;

    if (nb_classe==NULL || nb_classe[0]==-1 || nb_race<0 || version<0)
    {
        retour=-1; /* on ne devrait passer par là que pour une race n'acceptant aucune classe ou un choix vierge de classe*/
    }
    else if (RACE[nb_race].add[version].nom==NULL)
    {

    }
    else
    {
        pt_liste_classe_race=RACE[nb_race].add[version].liste_classe;
        for (tmp_pt_liste_classe_race=pt_liste_classe_race;tmp_pt_liste_classe_race!=NULL;tmp_pt_liste_classe_race=tmp_pt_liste_classe_race->suiv)
        {
            tmp_retour=0;
            for(i=0;nb_classe[i]!=-1;i++)
            {
                for(pt_liste_classe=tmp_pt_liste_classe_race->classe;pt_liste_classe!=NULL;pt_liste_classe=pt_liste_classe->suiv)
                {
                    if (pt_liste_classe->num==nb_classe[i])
                    {
                        tmp_retour++;
                    }
                    else
                    {
                    }
                }
            } /* tmp_retour : combien de classe sont égales dans cette liste de multiclasse et la liste à tester */
            retour=max(tmp_retour,retour);
        }
        if (retour!=i) /* i vaut le nombre de classe de la liste à tester */
        { /* il y a un problème => on cherche le plus compatible */
            if (retour==0)
            { /* rien de compatible => on cherche une mono_classe compatible avec la race */
                nb_classe[0]=-1;
                retour=-1; /* -1 signale une race sans classe */
                for (tmp_pt_liste_classe_race=pt_liste_classe_race;tmp_pt_liste_classe_race!=NULL && retour==-1;tmp_pt_liste_classe_race=tmp_pt_liste_classe_race->suiv)
                {
                    for(pt_liste_classe=tmp_pt_liste_classe_race->classe,i=0;pt_liste_classe!=NULL && retour==-1;pt_liste_classe=pt_liste_classe->suiv,i++);
                    if (i==1) /* i : nb de classe dans cette multi-classe */
                    {
                        nb_classe[0]=tmp_pt_liste_classe_race->classe->num;
                        nb_classe[1]=-1;
                        retour=0;
                    }
                    else
                    {
                    }
                }
            }
            else
            {

                for (tmp_pt_liste_classe_race=pt_liste_classe_race;pt_liste_classe_race!=NULL && retour!=0 ;pt_liste_classe_race=pt_liste_classe_race->suiv)
                {
                    tmp_retour=0;
                    for(i=0;nb_classe[i]!=-1;i++)
                    {
                        for(pt_liste_classe=pt_liste_classe_race->classe;pt_liste_classe!=NULL;pt_liste_classe=pt_liste_classe->suiv)
                        {
                            if (pt_liste_classe->num==nb_classe[i])
                            {
                                tmp_retour++;
                            }
                            else
                            {
                            }
                        }
                    } /* tmp_retour : combien de classe sont égales dans cette liste de multiclasse et la liste à tester */
                    if (retour==tmp_retour) /* on trouve un maximum compatible */
                    {
                        for(tmp_retour=0,pt_liste_classe=pt_liste_classe_race->classe;pt_liste_classe!=NULL;pt_liste_classe=pt_liste_classe->suiv,tmp_retour++)
                        {
                            nb_classe[tmp_retour]=pt_liste_classe->num;
                        }
                        nb_classe[tmp_retour]=-1;
                        retour=0;
                    }
                    else
                    {
                    }
                }
                retour=0;
            }
        }
        else
        {
        }
    }

    return retour;
}



 GtkWidget * gtk_get_widget_by_name(GtkContainer * pere,char * _nom)
 {
     GList * gl, *gl1;
     GtkWidget * retour=NULL;

     gl1=gtk_container_get_children(GTK_CONTAINER(pere));
     for (gl=gl1;gl != NULL && retour==NULL; gl= g_list_next(gl))
     {
         if (compare_sans_casse(_nom,(char *)gtk_widget_get_name(GTK_WIDGET(gl->data)))==0)
         {
             retour=GTK_WIDGET(gl->data);
         }
         else
         {
         }
         if (retour==NULL && GTK_IS_CONTAINER(GTK_WIDGET(gl->data)))
         {
             retour=gtk_get_widget_by_name(GTK_CONTAINER(gl->data),_nom);
         }
         else
         {
         }
     }
     g_list_free(gl1);

     return retour;
 }

 char * virgule_point(char * ch)
 { /* passage du séparateur décimal français à celui informatique */
     unsigned short i;

     if (ch!=NULL)
     {
         for (i=0;ch[i]!=0;i++)
         {
             if (ch[i]==',')
             {
                 ch[i]='.';
             }
             else
             {
             }
         }
     }

     return ch;
 }

int copie_fichier(char const *path_dest, char const *path_src)
{
    int err = M_FILE_OK;
    FILE *src, *dest;
    unsigned char buffer;

    /* -tc- Validation des arguments */
    if (path_dest != NULL && path_src != NULL)
    {
        /* -tc- Si path_dest et path_src sont identiques, il n'y a rien a copier. */
        if (strcmp(path_dest, path_src) != 0)
        {
            src = NULL;
            dest = NULL;

            src = g_fopen(path_src, "rb");
            if (src != NULL)
            {
                dest = g_fopen(path_dest, "wb");
                if (dest != NULL)
                {
                    while (err == M_FILE_OK && fread(&buffer, sizeof buffer, 1, src) == 1)
                    {
                        if (fwrite(&buffer, sizeof buffer, 1, dest) != 1)
                        {
                            /* -tc- Erreur lors de l'ecriture */
                            err = M_FILE_WRITE_ERR;
                        }
                    }

                    if (feof(src) == 0)
                    {
                        /* Erreur lors de la lecture */
                        err = M_FILE_READ_ERR;
                    }
                    fclose(dest), dest = NULL;
                }
                else
                {
                    /* -tc- Erreur: impossible d'ouvrir le fichier de destination
                       en ecriture */
                    err = M_FILE_OUTPUT_FILE_ERR;
                }

                fclose(src), src = NULL;
            }
            else
            {
                /* -tc- Erreur: impossible d'ouvrir le fichier en lecture */
                err = M_FILE_INPUT_FILE_ERR;
            }
        }
    }
    else
    {
        /* -tc- Erreur: argument invalide */
        err = M_FILE_ARGUMENT_ERR;
    }

    return err;
}

 GtkWidget * gtk_get_widget_enfants(GtkWidget * pere,unsigned short niveau)
 {
     GList * gl, *gl1;
     GtkWidget * retour=NULL;


     gl1=gtk_container_get_children(GTK_CONTAINER(pere));
     for (gl=gl1;gl != NULL && retour==NULL; gl= g_list_next(gl))
     {
         unsigned short i;

         for (i=0;i<niveau;i++)
         {
             printf(" ");
         }
         printf("%s\n",G_OBJECT_TYPE_NAME(GTK_WIDGET(gl->data)));

         if (GTK_IS_CONTAINER(GTK_WIDGET(gl->data)))
         {
             gtk_get_widget_enfants(GTK_WIDGET(gl->data),niveau+1);
         }
         else
         {

         }
     }
     g_list_free(gl1);

     return retour;
 }

void vide_widget(GtkWidget *appel)
{
     GList * gl, *gl1;

     gl1=gtk_container_get_children(GTK_CONTAINER(appel));
     for (gl=gl1;gl != NULL; gl= g_list_next(gl))
     {
         gtk_widget_destroy(GTK_WIDGET(gl->data));
     }
     g_list_free(gl1);
}

gchar * nb_texte(gchar * ch,unsigned short nb)
{
    if (nb>99)
    {
        ch[0]='0'+nb/100;
        ch[1]='0'+nb/10%10;
        ch[2]='0'+nb%10;
        ch[3]=0;

    }
    else if (nb>9)
    {
        ch[0]='0'+nb/10;
        ch[1]='0'+nb%10;
        ch[2]=0;
    }
    else
    {
        ch[0]='0'+nb;
        ch[1]=0;
    }

    return ch;
}

unsigned short place_max(unsigned short * tab, unsigned short taille)
{
    unsigned short max,i,i_max;

    i_max=0;
    max=tab[0];
    for(i=1;i<taille;i++)
    {
        if(max<tab[i])
        {
            max=tab[i];
            i_max=i;
        }
        else
        {
            /* ce n'est pas le max */
        }
    }

    return i_max;
}

void g_fclose(FILE * ptf)
{
    if (ptf!=NULL)
    {
        fclose(ptf);
    }
    else
    {
        /* pointeur de fichier nul */
    }
}

signed short meme_extension(char * _nom,char * ext)
{
    signed short i;
    signed short drap=1;
    char * pt;

    if (strlen(ext)<strlen(_nom))
    {
        pt=_nom+strlen(_nom)-strlen(ext);
        for(i=strlen(ext)-1;i>=0;i--)
        {
            if (pt[i]!=ext[i])
            {
                drap=0;
                i=0; /* sortir de la boucle */
            }
            else
            { /* même valeur */
            }
        }
    }
    else
    {
        drap=0; /* le nom est plus court que l'extension : il n'a donc pas l'extension */
    }
    return drap;
}

char * chemin_odt(char * chemin_pers, signed short version)
{
    char * retour=(char *)g_malloc(LONG);
    FILE * f=NULL;
    unsigned short i;
    signed short v;

    for (v=0;v<2;v++)
    { /* d'abord la variante de la version demandée, puis l'autre */
        g_strlcpy(retour,chemin_pers,LONG);
        i=strlen(retour);
        if (i>5 && compare_sans_casse(retour+i-5,".pers")==0)
        {
            retour[i-5]=0; /* retire .pers */
        }
        else if (i>4 && compare_sans_casse(retour+i-4,".grp")==0)
        {
            retour[i-4]=0; /* retire .grp */
            g_strlcat(retour,"_grp",LONG);
        }
        else
        {
        }
        g_strlcat(retour,version==ADD2?"_add2.odt":".odt",LONG);
        f=g_fopen(retour,"rb");
        if (f!=NULL)
        {
            fclose(f);
            v=2; /* variante trouvée : je n'aime pas voir un break et on peut savoir pourquoi on est sorti de la boucle : v=3 => fichier trouvé  */
        }
        else
        {
            version=!version;   /* inversion de ADD et ADD2 pour essayer l'autre : l'une fait 0 et 1, c'est volontairement binaire */
        }                       /* les autres versions de donjons et dragons sont trop différentes pour être aussi traitées ici */
    }
    return retour; /* aucune variante : on renvoie la dernière essayée (dialogue "n'existe pas" la nommera) */
}
