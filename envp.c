/*
    Génération de feuille de personnage complète AD&D 1 et 2
    Génération aléatoire de personnage

    Copyright (C) 2011 Bernard SIAUD

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

#include "include.h" /* ici LONG sera défini par windows.h :( */
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void envp_mem_home(char * chemin);

char * donne_chemin_dieux_perso()
{
    char * ch1,aff[LONG];

    ch1=envp_app_data();
    sprintf(aff,"%s%cdieux.add",ch1,SEPARATEUR);
    g_free(ch1);

    return g_strdup(aff);
}

char * chemin_exe()
{
    char * retour;
#ifdef __linux__

 char *tmp,* tmp1=NULL;
 char chemin[1024],ch[1024];

	  sprintf(ch,"/proc/%d/exe",getpid());
	  memset(chemin,0,1024);
      if (readlink(ch,chemin,1023)<0)
      {
        retour = NULL;
      }
      else
      {
          for(tmp=chemin;*tmp!=0;tmp++)
          {
            if (*tmp==SEPARATEUR)
            {
              tmp1=tmp;
            }
            else
            {
            }
          }
          if (tmp1!=NULL)
          {
              *tmp1=0;
          }
          else
          {
          }

        retour=g_strdup(chemin);
      }

#else

 char ch[1024],* tmp,*tmp1=NULL;
 if (!( GetModuleFileName( NULL, ch, 1023) ))
 {
    retour = NULL;
 }
 else
 {
    for (tmp=ch;*tmp!=0;tmp++)
    {
        if (*tmp==SEPARATEUR)
        {
            tmp1=tmp;
        }
    }
    if (tmp1!=NULL)
    {
        *tmp1=0;
    }
    else
    {
    }
    retour=g_strdup(ch);
 }
#endif
 return retour;
}


void envp_init(void)
{
        char * ch1, aff[LONG] /*,ch[1024],*ch2*/ ;
        //unsigned short i,j;
        FILE * fp;
        char chemin[LONG];

        //GList * p_action=((GList **)param)[8];

        getcwd(chemin,LONG-1);
        ch1=envp_app_data();


        if (chdir(ch1)!=0)
        {
            mkdir(ch1,0755);
        }
        else
        {
        }

        sprintf(aff,"%s%chome",ch1,SEPARATEUR);
        fp = g_fopen(aff, "rb");
        if (fp==NULL)
        {
         fp = g_fopen(aff, "w");
         g_fprintf(fp,"%s",getenv(HOME));
         fclose(fp);
        }
        else
        {
         fclose(fp);
        }

        /*sprintf(aff,"%s%chist",ch1,SEPARATEUR);
        fp = g_fopen(aff, "r");
        if (fp==NULL)
        {
         fp = g_fopen(aff, "w");
         fclose(fp);
        }
        else
        {
            for (i=0;i<MEM_FICH_MAX;i++)
            {
                ch2=fgets(aff,1024-1,fp);
                sprintf(ch,"vide%hu",i+1);
                if (aff!=NULL && ch2!=NULL)
                {
                    for(j=0;aff[j]!='\n' && aff[j]!=0;j++);
                    aff[j]=0;
                    if (change_entree_menu(p_action,ch,aff))
                    {
                            //sprintf(ch,"/menubar/FichierMenuAction/hist/vide%hu",i+1);
                            cree_entree_menu(ch,aff,((GtkUIManager **)param)[2],i,"/menubar/FichierMenuAction/hist",((struct_param_nb **)param)[MAIN_PARAM_HIST]+i,menu_va_hist,G_CALLBACK(callback_background_color_menu_hist));
                    }
                }
                else
                {
                    change_entree_menu(p_action,ch,"");
                }
            }
            fclose(fp);
        }*/
        chdir(chemin);

        g_free(ch1);
}

char * envp_donne_home()
{
    char * ch, aff[LONG];
    FILE * fp;

    ch=envp_app_data();

    sprintf(aff,"%s%chome",ch,SEPARATEUR);
    fp = g_fopen(aff, "r");
    fgets(aff,LONG-1,fp); /* 1024 est assez grand pour mémoriser le chemin => sinon plantage pas la suite dans le programme ! */
    fclose(fp);
    g_free(ch);

    return g_strdup(aff);

}

char * envp_app_data()
{
    char * ch, aff[LONG];

    ch=getenv(MEM_PROG);
    sprintf(aff,"%s%c.perso.add",ch,SEPARATEUR);

    return g_strdup(aff);

}


void envp_mem_home_rep(char * chemin)
{
    unsigned short i=strlen(chemin);

    while (i && chemin[i]!=SEPARATEUR)
    {
        i--;
    }
    if (i!=0)
    {
        chemin[i]=0;
        envp_mem_home(chemin);
        chemin[i]=SEPARATEUR;
    }
    else
    { /* c'est la racine ! */
    }
}


void envp_mem_home(char * chemin)
{
    char * ch, aff[LONG];
    FILE * fp;

    ch=envp_app_data();

    sprintf(aff,"%s%chome",ch,SEPARATEUR);
    g_free(ch);
    fp = g_fopen(aff, "w");
    g_fprintf(fp,"%s",chemin);
    fclose(fp);
}


void envp_mem_hist(char * chemin)
{
        char * ch_grp[MEM_FICH_MAX],* ch_pers[MEM_FICH_MAX], aff[LONG],* ch1;
        FILE * fp;
        unsigned short i,j,nb_pers=0,nb_grp=0;
        /* pour la gestion de l'affichage du menu */

        if (chemin !=NULL)
        {
            ch1=envp_app_data();
            sprintf(aff,"%s%chist",ch1,SEPARATEUR);
            g_free(ch1);
            fp = g_fopen(aff, "r");
            if (fp!=NULL)
            {   /* lecture du fichier historique : une entrée par ligne */
                for (i=0;i<MEM_FICH_MAX*2;i++)
                {
                    ch1=fgets(aff,LONG-1,fp);
                    //if (aff!=NULL && ch1!=NULL)
                    if (aff[0]!=0 && ch1!=NULL)
                    {
                        for(j=0;aff[j]!='\n' && aff[j]!=0;j++);
                        aff[j]=0; /* on enlève le saut de ligne final */
                        if (strcmp(aff,chemin)==0)
                        { /* on ne rajoute pas le chemin actuel : il sera mis en premier */
                            i--;
                        }
                        else
                        {
                            if (meme_extension(aff,"pers")==1)
                            {
                                ch_pers[nb_pers]=g_strdup(aff);
                                nb_pers++;
                            }
                            else if (meme_extension(aff,"grp")==1)
                            {
                                ch_grp[nb_grp]=g_strdup(aff);
                                nb_grp++;
                            }
                            else
                            {
                                printf("La ligne %s du fichier hist n'est ni un groupe, ni un personnage\n",aff);
                            }
                        }
                    }
                    else
                    {
                        i=MEM_FICH_MAX*2;
                    }

                }
                fclose(fp);
            }
            else
            {
                /* pas encore d'entrée */
            }

            /* lecture du fichier de mémorisation et placement du nouveau fichier à sa place */
            /* en premier les .pers => les personnages, après les .grp => les groupes        */

            ch1=envp_app_data();
            sprintf(aff,"%s%chist",ch1,SEPARATEUR);
            g_free(ch1);
            fp = g_fopen(aff, "w");
            /* ce n'est pas un personnage : on mettra la nouvelle entrée plus tard */
            if (meme_extension(chemin,"pers")==1)
            {
                g_fprintf(fp,"%s\n",chemin);
                if (nb_pers==MEM_FICH_MAX)
                {
                    nb_pers--;
                }
                else
                {
                    /* on n'était pas encore au max */
                }
            }
            else
            {
            }
            for (i=0;i<nb_pers;i++)
            {
                g_fprintf(fp,"%s\n",ch_pers[i]);
                g_free(ch_pers[i]);
            }
            if (meme_extension(chemin,"grp")==1)
            {
                g_fprintf(fp,"%s\n",chemin);
                if (nb_grp==MEM_FICH_MAX)
                {
                    nb_grp--;
                }
                else
                {
                    /* on n'était pas encore au max */
                }
            }
            else
            {
                /* ce n'est pas un groupe : on l'a rajouté pour les personnage, ou ce n'était aucun des deux */
            }
            for (i=0;i<nb_grp;i++)
            {
                g_fprintf(fp,"%s\n",ch_grp[i]);
                g_free(ch_grp[i]);
            }
            fclose(fp);
        }
        else
        {
            printf("envp_mem_hist : pas normal : entrée NULL\n");
        }
}

void envp_mem_hist_supp(char * chemin)
{
        char * ch[2*MEM_FICH_MAX], aff[LONG],* ch1;
        FILE * fp;
        unsigned short i,j;
        /* pour la gestion de l'affichage du menu */

        ch1=envp_app_data();
        sprintf(aff,"%s%chist",ch1,SEPARATEUR);
        g_free(ch1);
        fp = g_fopen(aff, "r");
        if (fp!=NULL)
        {   /* lecture du fichier historique : une entrée par ligne */
            i=0;
            while ((ch1=fgets(aff,LONG-1,fp)))
            {
                //if (aff!=NULL)
                if (aff[0]!=0)
                {
                    for(j=0;aff[j]!='\n' && aff[j]!=0;j++);
                    aff[j]=0; /* on enlève le saut de ligne final */
                    if (j!=0 && chemin!=NULL && strcmp(aff,chemin)==0)
                    {
                    }
                    else
                    {
                        /* ligne non vide et fichier différent : on le prend en compte */
                        ch[i]=g_strdup(aff);
                        i++;
                    }
                }
                else
                {
                    /* ligne vide */
                }

            }
            fclose(fp);
        }
        else
        {
            /* pas encore d'entrée */
        }

        /* lecture du fichier de mémorisation et placement du nouveau fichier à sa place */
        /* en premier les .pers => les personnages, après les .grp => les groupes        */

        ch1=envp_app_data();
        sprintf(aff,"%s%chist",ch1,SEPARATEUR);
        g_free(ch1);

        fp = g_fopen(aff, "w");
        if (fp==NULL)
        {
            printf("n'ouvre pas %s i=%hu\n",aff,i);
        }
        else
        {
            /* ce n'est pas un personnage : on mettra la nouvelle entrée plus tard */
            for (j=0;j<i;j++)
            {
                g_fprintf(fp,"%s\n",ch[j]);
                g_free(ch[j]);
            }
            fclose(fp);
        }
}

char * windows_home(char * chemin) /* uniquement pour windows */
{
    char ch[1024];

    if (chemin[0]==SEPARATEUR)
    {
        sprintf(ch,"%s%s",getenv("HOMEDRIVE"),chemin);
        g_free(chemin);
        chemin=g_strdup(ch);
    }
    else
    {
    }

    return chemin;
}


GtkWidget * deja_edite(char * nom_de_fichier)
{
    char ch[1024];
    FILE * fp;
    signed long pid;
    GtkWidget * win=NULL;
    GtkWidget * retour;

    sprintf(ch,"%s.lock",nom_de_fichier);
    fp = g_fopen(ch, "rb");
    if (fp == NULL)
    {
    /* L'ouverture du fichier "fichier.txt" a echoue => le fichier "fichier.txt" n'existe pas. */
        retour= NULL;
    }
    else if (fscanf(fp,"%ld %p",&pid,&win)!=2)
    {
        fclose(fp);
        retour = NULL;
    }
    else  if (pid!=getpid())
    {
        fclose(fp);
        retour= NULL;
    }
    else
    {
        fclose(fp);
        gtk_window_present (GTK_WINDOW(win));
        retour = win;
    }

    return retour;
}

char * chemin_absolu(char * chemin)
{
    unsigned char i;
    char * tmpo, tmp1[LONG];

      if (chemin!=NULL && chemin[0]=='.')
      { /* on passe en chemin absolu */
          getcwd(tmp1,LONG-1); /* tmp1 : chemin actuel */
          for(tmpo=tmp1;*tmpo!=0;tmpo++); /* tmpo : fin de la chaine de caractère du chemin actuel */
          i=0;
          while ( chemin[i]=='.' && chemin[i+1]=='.' && (chemin[i+2]==SEPARATEUR || (chemin[i+2]==0)) && tmpo>tmp1)
          {
              i+=3;
              for(;*tmpo!=SEPARATEUR && tmpo>=tmp1;tmpo--);
              tmpo--; /* retour avant le séparatateur */
          }
          if (tmpo>tmp1) *(tmpo+1)=0; /* s'arrêter au séparateur */
          if (chemin[i]==SEPARATEUR)
            i++;
          tmpo=(char *)g_malloc(strlen(tmp1)+strlen(chemin+i)+2);
          sprintf(tmpo,"%s%c%s",tmp1,SEPARATEUR,chemin+i);
          g_free(chemin);
          chemin = tmpo;
      }
      else
      {
      }
      return chemin;
}

