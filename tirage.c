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
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Un grand merci à Denis Léger pour sa liste de sort de magicien que j'ai
    récupéré pour compléter la mienne.
*/

#include "include.h"
#include <string.h>
#include <time.h>

unsigned short tirage_3D6();
unsigned short tirage_4D6();
unsigned short tirage_5D6();
unsigned short tirage_3D6x6();
unsigned short tirage_4D5p3();
unsigned short max_2d(unsigned short des);
unsigned short max_1d(unsigned short des);
unsigned short max_max(unsigned short des);

void init_caract_voleur(FenetrePerso * _perso);


void applique_menu(GtkWidget *wid_appel, struct widgets * widgets)
{
    FenetrePerso * _perso=NULL;
    GtkWidget * wid=NULL;
    char ch[16], * pt_ch=NULL;
    signed short * caract,caract_min[6]={3,3,3,3,3,3},caract_pref[6]={0,3,4,2,1,5},caract_max[7]={18,18,18,18,18,18,100};
    signed char pl_c_min_max[8]={0,1,2,3,4,5,6,7},pl_tr[8]={0,1,2,3,4,5,6,7};
    signed short niv=1,i,j,m,im,t,tmp,nb;
    struct cl_add * classe=NULL;
    signed short * tab_max=NULL, * tab_min=NULL, * tab_race=NULL;
    unsigned short (*pf)()=NULL; /* pointeur sur la fonction de tirage de dès */
    unsigned short (*tirage_pdv)(unsigned short)=NULL; /* pointeur sur la méthode de tirage des pdv */
    unsigned short constit=0,constitg=0, ** pdv,nb_des,de;
    signed short * b_constit_add2=NULL;
    char c;
    signed short * tmp_cl;

    tmp_cl=trouve_classe(widgets);

    if (*tmp_cl==-1)
    {
        donne_info("Tirage impossible :\naucune classe choisie");
        g_free(tmp_cl);
    }
    else
    {
        _perso=gestion_fenetre (widgets, NULL, NULL);
        _perso->perso.classe=tmp_cl;
        wid=GTK_WIDGET(gtk_builder_get_object(widgets->builder,"Masculin"));
        if (_perso->perso.classe[0]!=-1)
        {
            if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(wid)))
            {
                _perso->perso.sexe=MASCULIN;
            }
            else
            {
                _perso->perso.sexe=FEMININ;
            }
            wid=GTK_WIDGET(gtk_builder_get_object(widgets->builder,"ADD"));
            if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(wid)))
            {
                _perso->perso.version=ADD;
            }
            else
            {
                _perso->perso.version=ADD2;
            }
            strcpy(ch,"Niveau");
            for(i=0;i<=20;i++)
            {
                sprintf(ch+6,"%hd",i);
                wid=GTK_WIDGET(gtk_builder_get_object(widgets->builder,ch));
                if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(wid)))
                {
                    niv=i;
                    i=20;
                }
            }
            b_constit_add2=(signed short *)g_malloc(niv*sizeof(signed short));

            _perso->perso.race=trouve_race(widgets);   /* décompte du nombre de classe              */
            for (i=0;_perso->perso.classe[i]!=-1;i++); /* pour réserver la place mémoire nécessaire */
            _perso->perso.niveau=(signed short *)g_malloc(i*sizeof(signed short));
            _perso->perso.XP=(unsigned long *)g_malloc(i*sizeof(signed long));
            pdv=(unsigned short **)g_malloc(i*sizeof(unsigned short *));

            for (i=0;_perso->perso.classe[i]!=-1;i++)
            {
                _perso->perso.niveau[i]=niv;
                _perso->perso.XP[i]=0;
                classe=&(CLASSE[_perso->perso.classe[i]].add[_perso->perso.version]);
                for (j=0;j<6;j++)
                {
                    caract_max[j]=_min(classe->max_caract[j],caract_max[j]);
                    caract_min[j]=max(classe->min_caract[j],caract_min[j]);
                }
            }
            if (_perso->perso.sexe==MASCULIN)
            {
                tab_max=RACE[_perso->perso.race].add[_perso->perso.version].max_caract_h;
                tab_min=RACE[_perso->perso.race].add[_perso->perso.version].min_caract_h;
            }
            else
            {
                tab_max=RACE[_perso->perso.race].add[_perso->perso.version].max_caract_f;
                tab_min=RACE[_perso->perso.race].add[_perso->perso.version].min_caract_f;
            }
            tab_race=RACE[_perso->perso.race].add[_perso->perso.version].modif_caract;
            for (i=0;i<6;i++)
            {
                caract_max[i]=_min(tab_max[i],caract_max[i])-tab_race[i];
                caract_min[i]= max(tab_min[i],caract_min[i])-tab_race[i];
            }

            caract=_perso->perso.caract; /* simplifie le calcul et l'écriture */
            for(i=0;i<6;i++) /* utilisation du tableau caract comme tableau temporaire */
            {                  /* avant sa véritable affectation                         */
                caract[i]=caract_min[i];
            }
            for(i=1;i<6;i++)
            {                   /* on classe les minima dans l'ordre croissant */
                 im=i-1;
                 m=caract[im]; /* l'ordre sera mémorisé dans pl_c_min_max     */
                 for(j=i;j<6;j++)
                 {
                     if (m<caract[j])
                     {
                         im=j;
                         m=caract[j];
                     }
                     else
                     {
                         /* ce n'est pas le max temporaire */
                     }
                 }
                 caract[im]=caract[i-1]; /* on échange le max et le  dernier */
                 caract[i-1]=m;
                 tmp=pl_c_min_max[i-1];
                 pl_c_min_max[i-1]=pl_c_min_max[im];
                 pl_c_min_max[im]=tmp;
            }

            /* repérage de la méthopde de tirage */
            wid=GTK_WIDGET(gtk_builder_get_object(widgets->builder,"3D6_"));
            if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(wid)))
            {
                pf=&tirage_3D6;
            }
            else
            {
                wid=GTK_WIDGET(gtk_builder_get_object(widgets->builder,"4D6_"));
                if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(wid)))
                {
                    pf=&tirage_4D6;
                }
                else
                {
                    wid=GTK_WIDGET(gtk_builder_get_object(widgets->builder,"5D6__"));
                    if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(wid)))
                    {
                        pf=&tirage_5D6;
                    }
                    else
                    {
                        wid=GTK_WIDGET(gtk_builder_get_object(widgets->builder,"6x3D6_"));
                        if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(wid)))
                        {
                            pf=&tirage_3D6x6;
                        }
                        else
                        {
                            pf=&tirage_4D5p3;
                        }
                    }
                }
            }

            for(i=0;i<8;i++)
            {
                caract[i]=(*pf)();
            }

            for (i=0;i<6;i++)
            {
                for (j=1;j<=6;j++)
                {
                    sprintf(ch,"%c%hd",Lettre1_caract[i],j);
                    wid=GTK_WIDGET(gtk_builder_get_object(widgets->builder,ch));
                    if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(wid)))
                    {
                        caract_pref[j-1]=i;
                        j=8; /* pour sortir plus vite */
                    }
                }
            }

            for(i=1;i<6;i++) /* on classe les caractéristiques suivant les préférances */
            {
                 m=caract[caract_pref[i-1]];
                 im=i-1;
                 for(j=i;j<6;j++)
                 {
                     if (m<caract[caract_pref[j]])
                     {
                         im=j;
                         m=caract[caract_pref[j]];
                     }
                 }
                 caract[caract_pref[im]]=caract[caract_pref[i-1]];
                 caract[caract_pref[i-1]]=m;
                 m=pl_tr[caract_pref[im]];
                 pl_tr[caract_pref[im]]=pl_tr[caract_pref[i-1]];
                 pl_tr[caract_pref[i-1]]=m;
            }

            for(i=0;i<6;i++)
            {  /* parcours par ordre d'importance de la caractéristique */
               if (caract[pl_c_min_max[i]]<caract_min[pl_c_min_max[i]])
               { /* si une caractéristique n'est pas suffisante avec l'ordre de préférence */
                   if (caract_pref[0]!=pl_c_min_max[i])
                   { /* si la caractéristique n'est pas la préférée */
                       for(j=0;caract_pref[j]!=pl_c_min_max[i];j++);
                       /* trouver l'ordre de préférence de la caractéristique*/
                       im=j;
                       while ((caract_min[caract_pref[j]]>caract_min[pl_c_min_max[i]] || caract[caract_pref[j]]<caract_min[pl_c_min_max[i]]) && j>=0) /* est-ce que celui d'avant à une meilleur priorité ? */
                       {
                          j--;
                       }
                       if (j>=0) /* il y en a un avant dont la priorité est inférieur */
                       {         /* on les décale tous d'un rang */
                           tmp=caract[caract_pref[im]];
                           caract[caract_pref[im]]=caract[caract_pref[j]];
                           for(;i<im;im--)
                           {
                               if (caract_min[caract_pref[im]]<caract_min[pl_c_min_max[i]])
                               { /* ne prendre en compte que ceux dont la priorité est inférieure */
                                t=tmp;
                                tmp=caract[caract_pref[im]];
                                caract[caract_pref[im]]=t;
                               }
                           }
                       }
                   }
               }
            }

            for(i=0;i<6;i++) /* gérer les max */
            {  /* parcours par ordre d'importance de la caractéristique */
               if (caract[caract_pref[i]]>caract_max[caract_pref[i]])
               { /* si une caractéristique est forte avec l'ordre de préférence */
                   for(j=i+1;j<6 && caract[caract_pref[j]]<caract_max[caract_pref[i]];j++);
                   while((caract[caract_pref[j]]<caract_min[caract_pref[i]]) && j>i) j--;
                   /* décaler les i d'une place et 'mettre j à la place de i */
                   tmp=caract[caract_pref[j]];
                   while (j>i)
                   {
                       caract[caract_pref[j]]=caract[caract_pref[j-1]];
                       j--;
                   }
                   caract[caract_pref[i]]=tmp;
               }
            }

            for (j=0;j<6;j++)
            {
                caract[j]+=tab_race[j];
            }

            init_caract_voleur(_perso);

            wid=GTK_WIDGET(gtk_builder_get_object(widgets->builder,"1D"));
            if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(wid)))
            {
                tirage_pdv=&max_1d ;
            }
            else
            {
                wid=GTK_WIDGET(gtk_builder_get_object(widgets->builder,"2D"));
                if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(wid)))
                {
                    tirage_pdv=&max_2d ;
                }
                else
                {
                    tirage_pdv=&max_max;
                }
            }

            if (caract[CONSTITUTION]>16) /* lire le nombre entre parenthèse après le +2 */
            {
              sscanf(tabl_constit[caract[CONSTITUTION]-1][0],"+2(%hd)",&constitg); /* si pas de sortie alors 0 */
              constit=2;
            }
            else
            {
              sscanf(tabl_constit[caract[CONSTITUTION]-1][0],"%hd",&constit);
              constitg=constit;
            }

            _perso->perso.pdv=(signed short *)g_malloc((_perso->perso.niveau[0]+1)*sizeof(signed short));
            for (i=0;_perso->perso.classe[i]!=-1;i++)
            {
                pdv[i]=(unsigned short *)g_malloc(_perso->perso.niveau[i]*sizeof(unsigned short));
                for (j=0;j<_perso->perso.niveau[i];j++)
                {
                    pt_ch=CLASSE[_perso->perso.classe[i]].add[_perso->perso.version].pdv[_min(j,19)];

                    nb=sscanf(pt_ch,"%huD%hu+%c",&nb_des,&de,&c);
                    if (nb==1)
                    {
                        pdv[i][j]=nb_des;
                        if (i!=0)
                        {   /* b barbare : double les bonus du guerrier, g guerrier, n limite le bo,nus à 2 */
                            b_constit_add2[j]=max(b_constit_add2[j],0);
                        }
                        else
                        {  /* initialisation : premier passage */
                            b_constit_add2[j]=0;
                        }
                    }
                    if (nb>1)
                    {
                        pdv[i][j]=0;
                        for(;nb_des>0;nb_des--)
                            pdv[i][j]+=tirage_pdv(de);
                        if(nb==3)
                        {
                            if (_perso->perso.version==ADD1) /* b : cas du barbare */
                                pdv[i][j]+=max(1,(c=='b'?constitg*2:(c=='n'?constit:constitg)));
                            else
                            {
                                if (i!=0)
                                {   /* b barbare : double les bonus du guerrier, g guerrier, n limite le bo,nus à 2 */
                                    b_constit_add2[j]=max(b_constit_add2[j],(c=='b'?constitg*2:(c=='n'?constit:constitg)));
                                }
                                else
                                {  /* initialisation : premier passage */
                                    b_constit_add2[j]=(c=='b'?constitg*2:(c=='n'?constit:constitg));
                                }
                            }
                        }
                    }
                }
            }
            for (j=0;j<_perso->perso.niveau[0];j++)
            {
                _perso->perso.pdv[j]=pdv[0][j];
                for(m=1;m<i;m++)
                {
                    _perso->perso.pdv[j]+=pdv[m][j];
                }
                _perso->perso.pdv[j]/=m;
                if (_perso->perso.version==ADD2)
                {
                    _perso->perso.pdv[j]=max(1,_perso->perso.pdv[j]+b_constit_add2[j]);
                }
                else
                { /* et ADD1 ???? */
                }
                if (_perso->perso.pdv[j]<=0)
                {
                    _perso->perso.pdv[j]=1;
                }
                else
                {
                }
            }
            for(m=1;m<i;m++)
            {
                g_free(pdv[m]);
            }
            g_free(pdv);
            _perso->perso.pdv[j]=0; /* marquer la fin du tirage des pdv */


            affiche_resume(_perso);
        }
        else
        {
            /* pas de classe sélectionnées */
            gestion_fenetre (NULL,NULL, _perso);
        }
    }
    (void)wid_appel;
}

unsigned short tirage_4D6()
{
    unsigned short t,min,i,d;


    t= rand() % 6;
    min=t;

    for (i=1;i<4;i++)
    {
        d= rand() % 6;
        t+=d;
        if (d<min)
            min=d;
    }
    t-=min-3;

    return t;
}

unsigned short tirage_4D5p3()
{
    unsigned short t,min,i,d;

    t= rand() % 5;
    min=t;

    for (i=1;i<4;i++)
    {
        d= rand() % 5;
        t+=d;
        if (d<min)
            min=d;
    }
    t-=min-6;

    return t;
}

unsigned short tirage_3D6()
{
    unsigned short t,i;


    t= rand() % 6 +3;

    for (i=1;i<3;i++)
    {
        t+=rand() % 6;
    }

    return t;
}

unsigned short tirage_3D6x6()
{
    unsigned short tmax=0,t,j,i;

    for(j=0;j<6;j++)
    {
        t= rand() % 6+3;
        for (i=1;i<3;i++)
        {
            t+=rand() % 6;
        }
        if (tmax<t)
        {
            tmax=t;
        }
    }

    return tmax;
}

unsigned short tirage_5D6()
{
    unsigned short t,tt,i,d,dd;
    /* t somme tirage, dd dernier, d avant dernier, tt tirage */
    t= rand() % 6;
    d= rand() % 6;
    if (d>t)
    {
        dd=t;
    }
    else
    {
        dd=d;
        d=t;
    }
    t=d+dd+3;
    for (i=0;i<3;i++)
    {

        tt=rand() % 6;
        t+=tt;
        if (tt<d)
        {
            if (tt<dd)
            {
                d=dd;
                dd=tt;
            }
            else
            {
                d=tt;
            }
        }
    }
    t-=(dd+d);

    return t;
}


void init_caract_voleur(FenetrePerso * _perso)
{
    signed short i,j,cl, total, manque;
    signed char dext;
    signed short * comp;

    comp=_perso->perso.tab_voleur;

    for(j=0; j<FEUILLE_LAR_VOL; j++)
    {
        comp[j]=0; /* initialisation */
    }

    dext=_min(_perso->perso.caract[DEXTERITE],19);
    dext=(max(dext,9))-9;

    if (_perso->perso.version==ADD1)
    {
        for (cl=0;_perso->perso.classe[cl]!=-1;cl++)
        {
            if (CLASSE[_perso->perso.classe[cl]].add[ADD].nom!=NULL)
            {
                for (j=0; j<_perso->perso.niveau[cl] && CLASSE[_perso->perso.classe[cl]].add[ADD].tab_voleur[j]!=NULL; j++);
                if (j>0) j--;
                if (CLASSE[_perso->perso.classe[cl]].add[ADD].tab_voleur[j]!=NULL)
                {
                    for(i=0; i<FEUILLE_LAR_VOL; i++)
                    {
                        comp[i]=max(comp[i],CLASSE[_perso->perso.classe[cl]].add[ADD].tab_voleur[j][i]);
                    }
                }
            }

        }
    }
    else
    {
        for (cl=0;_perso->perso.classe[cl]!=-1;cl++)
        {
            if (CLASSE[_perso->perso.classe[cl]].add[ADD2].nom!=NULL)
            {
                if (CLASSE[_perso->perso.classe[cl]].add[ADD2].voleur!=NULL && (strcmp("OK",CLASSE[_perso->perso.classe[cl]].add[ADD2].voleur)==0 || strcmp("perso",CLASSE[_perso->perso.classe[cl]].add[ADD2].voleur)==0))
                {
                    if (strcmp("perso",CLASSE[_perso->perso.classe[cl]].add[ADD2].voleur)==0)
                    {
                        total=CLASSE[_perso->perso.classe[cl]].add[ADD2].tab_voleur[1][0];
                        total+=CLASSE[_perso->perso.classe[cl]].add[ADD2].tab_voleur[1][1]*(_perso->perso.niveau[cl]-1);
                        manque=total%7;
                        total/=7;
                        j=0;
                    }
                    else
                    {
                        total=0;
                        manque=0;
                        for (j=0; j<_perso->perso.niveau[cl] && CLASSE[_perso->perso.classe[cl]].add[ADD2].tab_voleur[j]!=NULL; j++);
                        if (j>0) j--;
                    }
                    if (CLASSE[_perso->perso.classe[cl]].add[ADD2].tab_voleur[j]!=NULL)
                    {
                        for(i=0; i<FEUILLE_LAR_VOL; i++)
                        {
                            if (i<7)
                            {
                                if (i<manque)
                                {
                                    comp[i]=max(comp[i],CLASSE[_perso->perso.classe[cl]].add[ADD2].tab_voleur[j][i]+total+1);
                                }
                                else
                                {
                                    comp[i]=max(comp[i],CLASSE[_perso->perso.classe[cl]].add[ADD2].tab_voleur[j][i]+total);
                                }
                            }
                            else
                            {
                                comp[i]=max(comp[i],CLASSE[_perso->perso.classe[cl]].add[ADD2].tab_voleur[j][i]);
                            }
                        }
                    }
                }
            }
        }
    }

    /* gestion de la race */
    if (_perso->perso.version==ADD2)
    {
        if (RACE[_perso->perso.race].add[ADD2].nom!=NULL)
            for(i=0; i<FEUILLE_LAR_VOL; i++)
            {
                if (comp[i]>0)
                {
                    comp[i]+=RACE[_perso->perso.race].add[ADD2].tab_voleur[0][i];
                }
            }
    }
    else
    {
        if (RACE[_perso->perso.race].add[ADD].nom!=NULL)
            for(i=0; i<FEUILLE_LAR_VOL; i++)
                if (comp[i]>0)
                    comp[i]+=RACE[_perso->perso.race].add[ADD].tab_voleur[0][i];
    }

    /* gestion de la dextérité */
    for (i=0; i<8; i++)
    {
        if (comp[i]>0)
        {
            comp[i]+=fac_vol_caract[dext][i];
        }
    }
}

unsigned short max_2d(unsigned short des)
{
    unsigned short i=rand()%des,j=rand()%des;
    if (i>j)
    {
        i++;
    }
    else
    {
        i=j+1;
    }
    return i;
}

unsigned short max_1d(unsigned short des)
{
    return rand()%des+1;
}

unsigned short max_max(unsigned short des)
{
    return des;
}
