#include "include.h" /* voir ligne 1039 */
#include <string.h>

struct_sortileges *** lire_struct_sort(char * fichier,unsigned short nature); /* second paramètre : liste des écoles/sphères troisième paramètre : mag ou clerc */
signed short lire_sphere(char * fichier);
struct_psi *** lire_struct_psi(char *fichier);
GMarkupDomNode * nv_tableau_sort(GMarkupDomNode * ooo,unsigned short niveau,char * metier);
void insere_ligne_pretre(GMarkupDomNode * node,struct_sortileges * sort_,unsigned short info);
char * h_m              (unsigned short minute,char * ch); /* transformation minutes => heures/minutes */

struct_sortileges *** sortileges=NULL;
unsigned short ** nb_sort_clerc=NULL;
struct_sphere ** sphere=NULL;
struct_sortileges *** sortileges_mag;
unsigned short ** nb_sort_mag=NULL;
struct_sphere ** ecole=NULL;
unsigned long entropie=0;
unsigned short nb_ecole=0;
unsigned short nb_sphere=0;
struct_sphere * discipline_psi=NULL;
struct_psi *** talents_psi;
unsigned short ** nb_talent_psi;
unsigned short nb_discipline_psi=0;

signed long * liste_mag_id[9]={0,0,0,0,0,0,0,0,0};
char ** liste_mag[9]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
char ** liste_clerc[7]={NULL,NULL,NULL,NULL,NULL,NULL,NULL};
signed long * liste_clerc_id[7]={0,0,0,0,0,0,0};
unsigned short sort_nb_sort_mag_connu[9]={0,0,0,0,0,0,0,0,0};
unsigned short sort_nb_sort_clerc_connu[7]={0,0,0,0,0,0};
unsigned short nb_forme=0; /* je ne prévoie pas lus de 65535 sorts différents par jour aux persos */

#define CLERC 0
#define MAGICIEN 1

struct_sphere * donne_discipline_psi (void)
{
    return discipline_psi;
}

struct_psi *** donne_talents_psi(void)
{
    return talents_psi;
}

unsigned short ** donne_nb_talent_psi(void)
{
    return nb_talent_psi;
}

unsigned short donne_nb_discipline_psi(void)
{
    return nb_discipline_psi;
}

struct_sortileges *** donne_sortileges_clerc(void)
{
    return sortileges;
}

struct_sortileges *** donne_sortileges_mag(void)
{
    return sortileges_mag;
}


unsigned short ** donne_nb_sort_clerc(void)
{
 return nb_sort_clerc;
}

struct_sphere ** donne_sphere(void)
{
    return sphere;
}
unsigned short ** donne_nb_sort_mag(void)
{
    return nb_sort_mag;
}

struct_sphere ** donne_ecole(void)
{
    return ecole;
}

unsigned short donne_nb_ecole(void)
{
    return nb_ecole;
}

unsigned short donne_nb_sphere(void)
{
    return nb_sphere;
}

void lire_sorts(char * ici)
{
 char * chemin;

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","sorts","spheres_ecoles.ods",NULL);
 if (lire_sphere(chemin)==0)
 {
     printf("Surprise : le fichier %s a un problème\n",chemin);
 }
 else
 { /* la lecture s'est bien passée */
 }
 g_free(chemin);

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","sorts","sorts-clerc.ods",NULL);
 sortileges=lire_struct_sort(chemin,CLERC);
 if (sortileges==NULL)
 {
     printf("Surprise : le fichier %s a un problème\n",chemin);
 }
 else
 { /* la lecture s'est bien passée */
 }
 g_free(chemin);

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","sorts","sorts-mag.ods",NULL);
 sortileges_mag=lire_struct_sort(chemin,MAGICIEN);
 if (sortileges==NULL)
 {
     printf("Surprise : le fichier %s a un problème\n",chemin);
 }
 else
 { /* la lecture s'est bien passée */
 }
 g_free(chemin);


 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","psi","psi.ods",NULL);
 talents_psi=lire_struct_psi(chemin);
 if (talents_psi==NULL)
 {
     printf("Surprise : le fichier %s a un problème\n",chemin);
 }
 else
 { /* la lecture s'est bien passée */
 }
 g_free(chemin);
}



signed short lire_sphere(char * fichier)
{
 struct_sphere *** sphere__=NULL, ** sphere_=NULL;
 GMarkupDomNode * node=NULL, *ooo=NULL, *ooo1=NULL;
 ooo1 = g_markup_dom_new (fichier, NULL);
 char passe;
 unsigned short nature,i;
 signed short l;

 ooo1 = g_markup_dom_new (fichier, NULL);
 for (passe=0;passe<2;passe++)
 {
     if (passe==0)
     {
        nature=CLERC;
        sphere__=&sphere;
        ooo=g_markup_dom_nom (ooo1,"Sphères_add2","table:name");
     }
     else
     {
        nature=MAGICIEN;
        sphere__=&ecole;
        ooo=g_markup_dom_nom (ooo1,"Écoles_add2","table:name");
     }
     if (*sphere__!=NULL) /* ça ne devrait pas arriver... */
     {
         if ((*sphere__)[ADD1]!=NULL)
         {
             g_free((*sphere__)[ADD1]);
         }
         else
         {
         }
         if ((*sphere__)[ADD2]!=NULL)
         {
             g_free((*sphere__)[ADD2]);
         }
         else
         {
         }
         g_free(*sphere__);
     }
     else
     {
     }

     *sphere__=(struct_sphere **)g_malloc(2*sizeof(struct_sphere *));
     (*sphere__)[ADD1]=NULL;
     (*sphere__)[ADD2]=NULL;

     if (ooo)
     { /* lecture des sphères ou écoles */

         i=0;
         node=donne_case_ij(1,0,ooo);
         sphere_=(*sphere__)+ADD2; /* on revient comme avant avant de traiter aussi les sorts de ADD1 */
         while(node && node->nb_texte>0)
         {
            *sphere_=(struct_sphere *)g_realloc(*sphere_,(i+1)*sizeof(struct_sphere));
            l=sscanf(node->texte->texte,"%lu",&((*sphere_+i)->clef));
            if (l!=1)
            {
                (*sphere_+i)->clef=0;
            }
            else
            {
            }
            node=donne_case_ij(i+1,1,ooo);
            (*sphere_+i)->nom=g_strdup(node->texte->texte);
            if (compare_sans_casse("Magie Entropique",node->texte->texte)==0)
            {
               entropie=1<<((*sphere_+i)->clef-1); /* l'entropie est un cas trop particulier ! */
            }
            else
            {
            }

            i++;
            node=donne_case_ij(i+1,0,ooo);
         }
         *sphere_=(struct_sphere *)g_realloc(*sphere_,(i+1)*sizeof(struct_sphere));
         (*sphere_+i)->nom=NULL;
     }

     if (nature==MAGICIEN)
     {
       nb_ecole=i;
     }
     else
     {
       nb_sphere=i;
     }
 }

 return nb_ecole+nb_sphere;

}



struct_sortileges *** lire_struct_sort(char *fichier,unsigned short nature) /* second paramètre : liste des écoles/sphères troisième paramètre : mag ou clerc */
{
 unsigned short i=0,k=0,l=0;
 signed short j=0;
 GMarkupDomNode * node=NULL, *ooo=NULL, *ooo1=NULL;
 struct_sortileges *** retour=NULL;
 unsigned short * nb_sort_=NULL;
 unsigned short nb_niveau=0,niveau=0;
 char *c=NULL;
 char vsm[6];

 ooo1 = g_markup_dom_new (fichier, NULL);

 if (nature==CLERC)
 {
   nb_sort_clerc=(unsigned short **)g_malloc(2*sizeof(unsigned short *));
   nb_sort_clerc[ADD1]=NULL;
   nb_sort_clerc[ADD2]=NULL;
 }
 else if (nature==MAGICIEN)
 {
   nb_sort_mag=(unsigned short **)g_malloc(2*sizeof(unsigned short *));
   nb_sort_mag[ADD1]=NULL;
   nb_sort_mag[ADD2]=NULL;
 }
 else
 {
 }

 if (nature==CLERC) /* prêtre */
 {
    nb_niveau=7;
 }
 else
 {
    nb_niveau=9;
 }

 retour=(struct_sortileges ***)g_malloc(2*sizeof(struct_sortileges **));
 retour[ADD1]=NULL; /* pas utilisé pour le momment */
 retour[ADD2]=(struct_sortileges **)g_malloc(nb_niveau*sizeof(struct_sortileges *));

 nb_sort_=(unsigned short *)g_malloc(nb_niveau*sizeof(unsigned short));
 for (l=0;l<nb_niveau;l++)
 {
  retour[ADD2][l]=NULL;
  nb_sort_[l]=0;
 }
 if ((ooo=g_markup_dom_nom (ooo1,"Sorts_add2","table:name")))
 { /* lecture des sorts */
     i=0;
     l=0;
     node=donne_case_ij(1,1,ooo);
     while(node && node->nb_texte>0) /* parcours des lignes du fichier */
     {

        c=g_strdup(node->texte->texte);
        node=donne_case_ij(i+1,2,ooo);
        if (node && node->nb_texte>0 )
        {
            j=sscanf(node->texte->texte,"%hu",&niveau);
            if (j!=1)
            {
              niveau=0;
            }
            else
            {
            }
        }
        else
        {
             niveau=0;
        }
        if (niveau==0)
        {
            printf("Erreur pour le fichier sort %s à la ligne %hu\n",c,i+2);
            niveau=1;
        }
        else
        {
        }

        niveau--;
        retour[ADD2][niveau]=(struct_sortileges *)g_realloc(retour[ADD2][niveau],(nb_sort_[niveau]+1)*sizeof(struct_sortileges));
        retour[ADD2][niveau][nb_sort_[niveau]].nom=c;


        node=donne_case_ij(i+1,6,ooo);
        if (node && node->nb_texte>0 )
        {
            retour[ADD2][niveau][nb_sort_[niveau]].portee=g_strdup(node->texte->texte);
        }
        else
        {
            retour[ADD2][niveau][nb_sort_[niveau]].portee=g_strdup("-");
        }

        node=donne_case_ij(i+1,11,ooo);
        vsm[0]=0;
        if (node && node->nb_texte>0 )
        {
            vsm[0]='V';
            vsm[1]=0;
        }
        node=donne_case_ij(i+1,12,ooo);
        if (node && node->nb_texte>0 )
        {
            if (*vsm==0)
            {
                vsm[0]='S';
                vsm[1]=0;
            }
            else
            {
                vsm[1]=',';
                vsm[2]='S';
                vsm[3]=0;
            }
        }
        node=donne_case_ij(i+1,13,ooo);
        if (node && node->nb_texte>0 )
        {
            if (*vsm==0)
            {
                vsm[0]='M';
                vsm[1]=0;
            }
            else if (vsm[1]==0)
            {
                vsm[1]=',';
                vsm[2]='M';
                vsm[3]=0;
            }
            else
            {
                vsm[3]=',';
                vsm[4]='M';
                vsm[5]=0;
            }
        }


        if (*vsm)
        {
            retour[ADD2][niveau][nb_sort_[niveau]].vsm=g_strdup(vsm);
        }
        else
        {
            retour[ADD2][niveau][nb_sort_[niveau]].vsm=g_strdup("-");
        }


        node=donne_case_ij(i+1,7,ooo);
        if (node && node->nb_texte>0 )
        {
            retour[ADD2][niveau][nb_sort_[niveau]].duree=g_strdup(node->texte->texte);
        }
        else
        {
             retour[ADD2][niveau][nb_sort_[niveau]].duree=g_strdup("-");
        }

        node=donne_case_ij(i+1,9,ooo);
        if (node && node->nb_texte>0 )
        {
            retour[ADD2][niveau][nb_sort_[niveau]].lance=g_strdup(node->texte->texte);
        }
        else
        {
             retour[ADD2][niveau][nb_sort_[niveau]].lance=g_strdup("-");
        }

        node=donne_case_ij(i+1,8,ooo);
        if (node && node->nb_texte>0 )
        {
            retour[ADD2][niveau][nb_sort_[niveau]].effet=g_strdup(node->texte->texte);
        }
        else
        {
             retour[ADD2][niveau][nb_sort_[niveau]].effet=g_strdup("-");
        }

        node=donne_case_ij(i+1,10,ooo);
        if (node && node->nb_texte>0 )
        {
            retour[ADD2][niveau][nb_sort_[niveau]].save=g_strdup(node->texte->texte);
        }
        else
        {
             retour[ADD2][niveau][nb_sort_[niveau]].save=g_strdup("-");
        }

        node=donne_case_ij(i+1,14,ooo);
        if (node && node->nb_texte>0 && compare_sans_casse(node->texte->texte,"NULL")!=0)
        {
            retour[ADD2][niveau][nb_sort_[niveau]].composant=g_strdup(node->texte->texte);
        }
        else
        {
            retour[ADD2][niveau][nb_sort_[niveau]].composant=g_strdup("-");
        }

        retour[ADD2][niveau][nb_sort_[niveau]].sphere=0;
        node=donne_case_ij(i+1,4,ooo);
        if (node && node->nb_texte>0 )
        {
            c=node->texte->texte;
            j=sscanf(c,"<%hu>",&k);
            while (*c!=0 && j)
            {
                if (k>32)
                {
                     printf("Revoir le programme, il y a trop d'école de magie ou de sphère de prêtres");
                }
                else
                {
                    if (nature==CLERC)
                    { /* k=0 pour générale */
                        retour[ADD2][niveau][nb_sort_[niveau]].sphere|=1<<k; /* il est prévu que k n'atteigne pas 32 : il est à 28 ! */
                    }
                    else /* magie */
                    {/* k commence à 1, on le fait commencer à 0 */
                        retour[ADD2][niveau][nb_sort_[niveau]].sphere|=1<<(k-1); /* il est prévu que k-1 n'atteigne pas 32 : il est à 27 */
                    }
                }
                while (*c!='>')
                {
                    c++;
                }
                c++;
                j=sscanf(c,"<%hu>",&k);
            }
        }

        node=donne_case_ij(i+1,3,ooo);
        if (node && node->nb_texte>0 )
        {
          retour[ADD2][niveau][nb_sort_[niveau]].reversible=1;
        }
        else
        {
         retour[ADD2][niveau][nb_sort_[niveau]].reversible=0;
        }

        node=donne_case_ij(i+1,5,ooo);
        if (node && node->nb_texte>0)
        {
            c=node->texte->texte;
            retour[ADD2][niveau][nb_sort_[niveau]].source=0;
            j=sscanf(c,"<%hu>",&k);
            while (*c!=0 && j==1)
            {
                if (k<=100)
                {
                    retour[ADD2][niveau][nb_sort_[niveau]].source*=100;
                    retour[ADD2][niveau][nb_sort_[niveau]].source+=k;
                }
                else
                {
                    printf("Numéro de source > 100 : il y a un problème. Chercher %s à la ligne %hd\n",node->texte->texte,i);
                }

                while (*c!='>')
                {
                    c++;
                }
                c++;
                j=sscanf(c,"<%hu>",&k);
            }
        }

        node=donne_case_ij(i+1,0,ooo);
        if (node && node->nb_texte>0 )
        {
            j=sscanf(node->texte->texte,"%lu",&retour[ADD2][niveau][nb_sort_[niveau]].clef);
            if (j!=1)
            {
                retour[ADD2][niveau][nb_sort_[niveau]].clef=0;
            }
            else
            { /* clef bien lue */
            }
        }
        else
        {
             retour[ADD2][niveau][nb_sort_[niveau]].clef=0;
        }
        if (retour[ADD2][niveau][nb_sort_[niveau]].clef==0)
        {
             printf("Problème avec l'identifiant du sort %s de la ligne %hu\n",c,i+3);
        }
        else
        { /* bonne lecture du sort */
        }

        node=donne_case_ij(i+1,18,ooo);
        if (node && node->nb_texte>0 )
        {
            sscanf(node->texte->texte,"%hd",&retour[ADD2][niveau][nb_sort_[niveau]].id_dnd);
        }
        else
        {
            retour[ADD2][niveau][nb_sort_[niveau]].id_dnd=0;
        }

        i++;
        nb_sort_[niveau]++;
        node=donne_case_ij(i+1,1,ooo);
        if (nature==CLERC) /* pretre */
        {
           nb_sort_clerc[ADD2]=nb_sort_;
        }
        else /* magicien */
        {
          nb_sort_mag[ADD2]=nb_sort_;
        }
     }
 }


 g_markup_dom_free(ooo1);

 return retour;
}

signed short sort_entier(char * _nom,signed short niveau,signed short version)
{
    signed short i,retour=NON;

    for (i=0;i<nb_sort_mag[version][niveau-1];i++)
    {   /* recherche dans la première série de non */
        if (compare_sans_casse(_nom,sortileges_mag[version][niveau-1][i].nom)==0)
        {
            retour=sortileges_mag[version][niveau-1][i].clef;
            i=nb_sort_mag[version][niveau-1];
        }
    }
    if (retour==NON)
    { /* ancienne dénomination ? */
            printf("Essayez avec une version antérieure à la version 4.4 ou identifier le sort %s\n",_nom);
    }

    return retour;
}

struct_psi *** lire_struct_psi(char *fichier)
{ /* travail avec des variables globales qu'il initialise une fois pour toute */
     signed short j,x,y,z;
     unsigned short i=0,k,l=0;
     GMarkupDomNode * node=NULL, *ooo=NULL, *ooo1=NULL;
     char *c=NULL;
     signed short discipline,science;

     if ((ooo1 = g_markup_dom_new (fichier, NULL)))
     {

         if ((ooo=g_markup_dom_nom (ooo1,"discipline","table:name")))
         { /* lecture des sphères ou écoles */
             if (discipline_psi) /* ça ne devrait pas arriver, mais autant prendre ses précautions */
             {
              g_free(discipline_psi);
              discipline_psi=NULL;
             }
             else
             {
             }
             i=0;
             node=donne_case_ij(1,0,ooo);
             while(node && node->nb_texte>0)
             {
                discipline_psi=(struct_sphere *)g_realloc(discipline_psi,(i+1)*sizeof(struct_sphere));
                l=sscanf(node->texte->texte,"%lu",&((discipline_psi+i)->clef));
                if (l!=1)
                {
                    (discipline_psi+i)->clef=0;
                }
                else
                { /* lecture correcte de la clef */
                }
                node=donne_case_ij(i+1,1,ooo);
                (discipline_psi+i)->nom=g_strdup(node->texte->texte);

                i++;
                node=donne_case_ij(i+1,0,ooo);
             }
             discipline_psi=(struct_sphere *)g_realloc(discipline_psi,(i+1)*sizeof(struct_sphere));
             (discipline_psi+i)->nom=NULL;
             talents_psi=(struct_psi ***)g_malloc(i*sizeof(struct_psi **));
             nb_talent_psi=(unsigned short **)g_malloc(i*sizeof(unsigned short *));
             nb_discipline_psi=i;
             while (i>0)
             {
                 i--; /* on commence à max-1 et on finit à 0 */
                 talents_psi[i]=(struct_psi **)g_malloc(2*sizeof(struct_psi *)); /* science / dévotion */
                 talents_psi[i][0]=NULL;
                 talents_psi[i][1]=NULL;
                 nb_talent_psi[i]=(unsigned short *)g_malloc(2*sizeof(unsigned short));
                 nb_talent_psi[i][0]=0;
                 nb_talent_psi[i][1]=0;
             }

             if ((ooo=g_markup_dom_nom (ooo1,"liste","table:name")))
             { /* lecture des psi */
                 i=0;l=0;
                 node=donne_case_ij(2,0,ooo);
                 while(node && node->nb_texte>0)
                 {
                    c=g_strdup(node->texte->texte);

                    node=donne_case_ij(i+2,2,ooo);
                    if (node && node->nb_texte>0 )
                    {
                        j=sscanf(node->texte->texte,"%hu",&discipline);
                        if (j!=1)
                        {
                          discipline=-1;
                        }
                        else
                        { /* bonne lecture de la discipline */
                        }
                    }
                    else
                    { /* rien dans la case discipline */
                         discipline=-2;
                    }
                    node=donne_case_ij(i+2,11,ooo);
                    if (node && node->nb_texte>0 )
                    {
                        j=sscanf(node->texte->texte,"%hu",&science);
                        if (j!=1)
                        {
                            science=-1;
                        }
                        else
                        { /* bonne lecture de la case scinece */
                        }
                    }
                    else
                    { /* rien dans la case science */
                         science=-2;
                    }
                    if (discipline<=0 || science<0 || discipline>nb_discipline_psi || science>1)
                    { /* affichage de l'erreur */
                        printf("Lecture des psis : erreur pour le fichier psi.ods %s à la ligne %hu : discipline=%hu et science=%hu (fichier sorts.c)\n",c,i+3,discipline,science);
                        g_free(c);
                        c=NULL;
                    }
                    else
                    {
                        discipline--; /* les indices du vont de 0 à nb_discipline_psi-1 et celui de la bd de 1 à nb_discipline_psi */
                        talents_psi[discipline][science]=(struct_psi *)g_realloc(talents_psi[discipline][science],(nb_talent_psi[discipline][science]+1)*sizeof(struct_psi));
                        talents_psi[discipline][science][nb_talent_psi[discipline][science]].nom=c;

                        node=donne_case_ij(i+2,1,ooo);
                        if (node && node->nb_texte>0 )
                        {
                            j=sscanf(node->texte->texte,"%hu",&talents_psi[discipline][science][nb_talent_psi[discipline][science]].clef);
                            if (j==0)
                            {
                               printf("Lecture des psi : problème de clef à la discipline %s à la ligne %hu\n",c,i+3);
                            }
                            else
                            { /* bonne lecture de la clef */
                            }
                        }
                        else
                        {
                             printf("Lecture des psi : problème de clef à la discipline %s à la ligne %hu\n",c,i+3);
                        }

                        node=donne_case_ij(i+2,3,ooo);
                        talents_psi[discipline][science][nb_talent_psi[discipline][science]].caracteristique=6;
                        if (node && node->nb_texte>0 )
                        {
                            for(j=0;j<6;j++)
                            {
                                if (compare_sans_casse(node->texte->texte,(char *)nom_abr[j])==0)
                                {
                                     talents_psi[discipline][science][nb_talent_psi[discipline][science]].caracteristique=j;
                                     j=6; /* accélérer la sortie */
                                }
                                else
                                {
                                }
                            }
                            if (talents_psi[discipline][science][nb_talent_psi[discipline][science]].caracteristique==6)
                            {
                               printf("Lecture des psi : problème de caractéristique %s à la discipline %s à la ligne %hu\n",node->texte->texte,c,i+3);
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                               printf("Pas de caractéristique à la discipline %s à la ligne %hu\n",c,i+3);
                        }

                        node=donne_case_ij(i+2,4,ooo);
                        if (node && node->nb_texte>0 )
                        {
                            j=sscanf(node->texte->texte,"%hd",&talents_psi[discipline][science][nb_talent_psi[discipline][science]].modif);
                            if (j==0)
                            {
                               printf("Lecture des psi : problème de modif à la discipline %s à la ligne %hu\n",c,i+3);
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                             talents_psi[discipline][science][nb_talent_psi[discipline][science]].modif=0;
                        }

                        node=donne_case_ij(i+2,5,ooo);
                        if (node && node->nb_texte>0 )
                        {
                            talents_psi[discipline][science][nb_talent_psi[discipline][science]].cout=g_strdup(node->texte->texte);
                        }
                        else
                        {
                             talents_psi[discipline][science][nb_talent_psi[discipline][science]].cout=g_strdup("-");
                        }

                        node=donne_case_ij(i+2,6,ooo);
                        if (node && node->nb_texte>0 )
                        {
                            talents_psi[discipline][science][nb_talent_psi[discipline][science]].portee=g_strdup(node->texte->texte);
                        }
                        else
                        {
                             talents_psi[discipline][science][nb_talent_psi[discipline][science]].portee=g_strdup("-");
                        }

                        node=donne_case_ij(i+2,7,ooo);
                        if (node && node->nb_texte>0 )
                        {
                            talents_psi[discipline][science][nb_talent_psi[discipline][science]].lance=g_strdup(node->texte->texte);
                        }
                        else
                        {
                             talents_psi[discipline][science][nb_talent_psi[discipline][science]].lance=g_strdup("-");
                        }

                        node=donne_case_ij(i+2,8,ooo);
                        if (node && node->nb_texte>0 )
                        {
                            talents_psi[discipline][science][nb_talent_psi[discipline][science]].zone=g_strdup(node->texte->texte);
                        }
                        else
                        {
                            talents_psi[discipline][science][nb_talent_psi[discipline][science]].zone=g_strdup("-");
                        }

                        node=donne_case_ij(i+2,9,ooo);
                        talents_psi[discipline][science][nb_talent_psi[discipline][science]].niveau=0;
                        if (node && node->nb_texte>0 )
                        {
                            j=sscanf(node->texte->texte,"%hu",&talents_psi[discipline][science][nb_talent_psi[discipline][science]].niveau);
                            if (j==0)
                            {
                               printf("Lecture des psi : problème de niveau à la discipline %s à la ligne %hu\n",c,i+3);
                            }
                        }
                        else
                        {
                            printf("Lecture des psi : problème de niveau à la discipline %s à la ligne %hu\n",c,i+3);
                        }

                        node=donne_case_ij(i+2,10,ooo);
                        talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis=(unsigned short *)g_malloc(sizeof(unsigned short ));
                        talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis_lien=(struct_psi **)g_malloc(sizeof(struct_psi *));
                        talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis[0]=0;
                        talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis_lien[0]=NULL;
                        if (node && node->nb_texte>0 )
                        {
                            c=node->texte->texte;
                            k=1;
                            while (*c)
                            {
                                talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis=(unsigned short *)g_realloc(talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis,(k+1)*sizeof(unsigned short));
                                talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis_lien=(struct_psi **)g_realloc(talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis_lien,(k+1)*sizeof(struct_psi *));
                                talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis[k]=0;
                                talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis_lien[k]=NULL;
                                j=sscanf(c,"<%hu>",talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis+k-1);
                                if (j==0) /* on a rempli la première acse avec le numéro dr la discipline */
                                {
                                    talents_psi[discipline][science][nb_talent_psi[discipline][science]].prerequis[k-1]=0;
                                    printf("Lecture des psi : problème de prérequis à la discipline %s à la ligne %hu pour la lecture du prérequis %s en %s\n",talents_psi[discipline][science][nb_talent_psi[discipline][science]].nom,i+3,node->texte->texte,c);
                                }
                                else
                                {
                                    k++;
                                }
                                do
                                {
                                   c++;
                                }
                                while (*c && *c!='<');
                            }
                        }
                        else
                        {
                        }

                        node=donne_case_ij(i+2,12,ooo);
                        talents_psi[discipline][science][nb_talent_psi[discipline][science]].defense=0;
                        if (node && node->nb_texte>0 )
                        {
                            j=sscanf(node->texte->texte,"%hu",&(talents_psi[discipline][science][nb_talent_psi[discipline][science]].defense));
                            if (j==0)
                            {
                                talents_psi[discipline][science][nb_talent_psi[discipline][science]].defense=0;
                                printf("Lecture des psi : problème de défense à la discipline %s à la ligne %hu : %s\n",talents_psi[discipline][science][nb_talent_psi[discipline][science]].nom,i+3,node->texte->texte);
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                        }
                        nb_talent_psi[discipline][science]++;
                    }
                    i++;
                    node=donne_case_ij(2+i,0,ooo);
                 }
                 /* décriptage des dépendences psi : il fallait d'abord remplir tous les psis */
                 for (discipline=0;discipline<nb_discipline_psi;discipline++)
                 {
                     for (science=0;science<2;science++)
                     {
                         for(i=0;i<nb_talent_psi[discipline][science];i++)
                         {
                             l=0;
                             while (talents_psi[discipline][science][i].prerequis[l]!=0)
                             {
                                 for (x=0;x<nb_discipline_psi;x++)
                                 {
                                     for (y=0;y<2;y++)
                                     {
                                         for(z=0;z<nb_talent_psi[x][y];z++)
                                         {
                                             if (talents_psi[x][y][z].clef==talents_psi[discipline][science][i].prerequis[l])
                                             {
                                                 talents_psi[discipline][science][i].prerequis_lien[l]=talents_psi[x][y]+z;
                                                 y=1; /* forcer la sortie des 3 boucles for */
                                                 x=nb_discipline_psi-1;
                                                 z=nb_talent_psi[x][y]-1;
                                             }
                                             else
                                             { /* pas encore le bon */
                                             }
                                         }
                                     }
                                 }
                                 l++;
                             }
                         }
                     }
                 }
             }
             else
             {
                 printf("Pas de table psi dans le fichier %s\n",fichier);
             }
         }
         else
         {
             printf("Pas de discipline dans le fichier psi %s\n",fichier);
             talents_psi=NULL;
             nb_talent_psi=NULL;
         }
         g_markup_dom_free(ooo1);
     }
     else
     {
         printf("Problème avec le fichier %s\n",fichier);
     }

     return talents_psi;
}

void aff_nb_sort(GMarkupDomNode * node,struct_classe * s_c,signed short * clerc,signed short * mag,signed short sage,signed short inte,unsigned short niv,signed short version_)
{
    GMarkupDomNode * tmp1_node=NULL,* tmp2_node=NULL,* tmp3_node=NULL,* tmp4_node=NULL,* tmp5_node=NULL, *node_lien_sorts=NULL;
    gint tmp_sort=0,max_sort=0;
    signed short i,j,l,k,m;
    char aff[LONG];
    char * metier=NULL;
    unsigned char add_;
    so_add * nb_sorts=NULL;
    unsigned short tps_sort,tps_sort_total;

    if ((version_ ==ADD1 && s_c->add[ADD].nom!=NULL) || s_c->add[ADD2].nom==NULL)
    {
        add_=ADD1;
        metier=s_c->add[ADD].nom;
    }
    else if (((version_ ==ADD2 && s_c->add[ADD2].nom!=NULL) || s_c->add[ADD].nom==NULL))
    {
        add_=ADD2;
        metier=s_c->add[ADD2].nom;
    }
    else
    {
        add_=0;
        nb_sorts=NULL;
        metier=NULL;
    }

    if ((add_==ADD  && (s_c->add[ADD ].sort_clerc!=NULL || s_c->add[ADD].sort_druide!=NULL))
     || (add_==ADD2 &&  s_c->add[ADD2].sort_clerc!=NULL))
    {
        if (add_==ADD)
        {
            if (s_c->add[ADD].sort_clerc!=NULL)
            {
                 nb_sorts=s_c->add[ADD].sort_clerc;
            }
            else
            {
                 nb_sorts=s_c->add[ADD].sort_druide;
            }
        }
        else
        {
            nb_sorts=s_c->add[ADD2].sort_clerc;
        }

        for (i=0;i<niv && nb_sorts->nb[i]!=NULL;i++); /* i : recherche du niveau maximum de compétence de la classe */
        i--;
        if (i<0 || nb_sorts->nb[i][1]==0)
        {
            i=-1; /* pas de sorts */
        }
        else
        {   /* s'il y a des sorts */
            if (*clerc>0)
            { /* il y avait des sorts de clerc => on rajoute un tableau de sort de clerc */
                tmp1_node=g_markup_dom_nom(node,"section_sorts_clerc","name");
                xml_ajoute_fin(tmp1_node,"text:p");
                xml_ajoute_fin(tmp1_node,"text:p");
                decale_node(tmp1_node,0);
                decale_node(tmp1_node,0);
                copie_node(tmp1_node->fils,tmp1_node->fils+2); // copie du tableau

                xml_ajoute_fin_attribut(tmp1_node->fils+1); // mise en place du texte
                xml_ecrit_dernier_attribut(tmp1_node->fils+1,"text:style-name","P24");
                xml_ajoute_fin_attribut(tmp1_node->fils+1);
                xml_ecrit_dernier_attribut(tmp1_node->fils+1,"name","suppr_clerc0");

                tmp1_node=tmp1_node->fils; // effacer le tableau précédent
                if (tmp1_node!=NULL)
                {
                    while(tmp1_node->nb_fils>3)
                    { /* on ne garde que 3 lignes :        */
                      /* 1) table:table-column en indice 0 */
                      /* 2) la ligne de titre en indice 1  */
                      /* 3) la dernière ligne : le temps   */
                        xml_sup_node(tmp1_node->fils+2);
                    }
                }
                else
                {
                    printf("Erreur effaçage name=%s\n",aff);
                }
            }
            else
            { /* premier passage avec des sorts de clerc */
            }
            /*tmp1_node=g_markup_dom_nom(node,"clerc","name");
            xml_ecrit_dernier_texte(tmp1_node->fils[0].fils,metier);*/
            (*clerc)++;
            tmp1_node=g_markup_dom_nom(node,"clerc","name");
            tmp2_node=g_markup_dom_nom(node,"clerc_tps","name");
            tps_sort_total=0;
            xml_ecrit_dernier_texte(tmp1_node->fils[0].fils,metier);
            max_sort=0;
            for (j=0;j<niv && nb_sorts->nb[i][j]!=-1;j++); /* j : recherche du nombre max de niveau de sort de la classe */
            if (nb_sorts->nb[i][j]==-1)
            { /* limite du tableau dépassé */
                j--;
            }
            else
            { /* on n'a pas dépassé la limite du tableau */
            }
            if (add_==ADD2 || compare_sans_casse("druide",metier)!=0)
            {
                j=_min(j,tabl_sagesse[sage-1][10][0]-'0');
            }
            else
            {
                /* druide ADD1 : pas de limite en fonction de la sagesse */
            }

            for (l=1;l<=j;l++)
            {
                tmp_sort=0;
                if ((l<5 || sage>17 || (sage==17 && l==5)) && ((add_==ADD1 && (s_c->add[ADD].bonus_clerc=='b' || s_c->add[ADD].bonus_druide=='b') )|| (add_==ADD2 && s_c->add[ADD2].bonus_clerc=='b')))
                {

                    tmp_sort=nb_sorts->nb[i][l];
                    if (tmp_sort==0)
                    {
                        sprintf(aff,"niv %d : 0",l);
                        tps_sort=0;
                    }
                    else if (tabl_sagesse[sage-1][l][0]==0)
                    {
                      sprintf(aff,"%d",tmp_sort);
                      tps_sort=tmp_sort*10*l;
                      max_sort=max(max_sort,tmp_sort);
                    }
                    else
                    {
                        sprintf(aff,"niv %d : %d",l,tmp_sort+tabl_sagesse[sage-1][l][0]-'0');
                        tps_sort=(tmp_sort+tabl_sagesse[sage-1][l][0]-'0')*10*l;
                        max_sort=max(max_sort,tmp_sort+tabl_sagesse[sage-1][l][0]-'0');
                    }
                    xml_ecrit_dernier_texte(tmp1_node->fils[l].fils,aff);
                    xml_ecrit_dernier_texte(tmp2_node->fils[l].fils,h_m(tps_sort,aff));
                    tps_sort_total+=tps_sort;
                }
                else
                {
                    tmp_sort=nb_sorts->nb[i][l];
                    tps_sort=tmp_sort*l;
                    sprintf(aff,"niv %d : %d",l,tmp_sort);
                    tps_sort=tmp_sort*10*l;
                    xml_ecrit_dernier_texte(tmp1_node->fils[l].fils,aff);
                    xml_ecrit_dernier_texte(tmp2_node->fils[l].fils,h_m(tps_sort,aff));
                    tps_sort_total+=tps_sort;
                    max_sort=max(max_sort,tmp_sort);
                }
            }
            if (j<7) /* pas une classe pure de lanceur de sorts ou pas au max */
            {
                xml_ecrit_dernier_texte(tmp1_node->fils[7].fils,metier);
                aff[0]=0;
                sprintf(aff,"c : %hu",nb_sorts->nb[i][0]); /* le niveau de maîtrise */
                xml_ecrit_dernier_texte(tmp1_node->fils[7].fils,aff);
                xml_ecrit_dernier_texte(tmp2_node->fils[7].fils,"-");
                for (l=j+1;l<7;l++)
                {
                    sprintf(aff,"niv %d : 0",l);
                    xml_ecrit_dernier_texte(tmp1_node->fils[l].fils,aff);
                    xml_ecrit_dernier_texte(tmp2_node->fils[l].fils,"-");
                }
            }
            else
            { /* classe pure de lanceur de sorts au maximum */
            }
            xml_ecrit_dernier_texte(tmp2_node->fils[0].fils,h_m(tps_sort_total,aff));

            if (g_markup_dom_nom (node->parent,"formulaire_perso","name")!=NULL)  /* if : ne pas le faire pour le groupe */
            for (l=0;l<max_sort;l++)
            {
                tmp2_node=ajoute_node(g_markup_dom_nom(node,"clerc","name")->parent,2+l,"table:table-row");
                sprintf(aff,"sort n°%hd",l+1);
                tmp3_node=xml_ajoute_fin(tmp2_node,"table:table-cell");
                xml_ajoute_fin(tmp3_node,"text:p");
                xml_ecrit_dernier_texte(tmp3_node->fils,aff);
                xml_ajoute_fin_attribut(tmp3_node->fils);
                xml_ecrit_dernier_attribut(tmp3_node->fils,"text:style-name","P15");
                xml_ajoute_fin_attribut(tmp3_node); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp3_node,"table:style-name","Tableau3.A2"); /* remplace le dernier attribut */
                for (k=1;k<7;k++)
                {
                    tmp3_node=xml_ajoute_fin(tmp2_node,"table:table-cell");
                    xml_ajoute_fin_attribut(tmp3_node); /* rajouter un élément attribut */
                    if (k>j)
                    {
                        tmp_sort=0;
                    }
                    else if ((k<5 || sage>17 || (sage==17 && k==5)) && ((add_==ADD1 && (s_c->add[ADD].bonus_clerc=='b' || s_c->add[ADD].bonus_druide=='b') )|| (add_==ADD2 && s_c->add[ADD2].bonus_clerc=='b')))
                    { /* l => k */
                        tmp_sort=nb_sorts->nb[i][k];
                        if (tmp_sort==0 || tabl_sagesse[sage-1][k][0]==0)
                        { /* pas de bonus si pas de sorts */
                        }
                        else
                        {
                            tmp_sort=tmp_sort+tabl_sagesse[sage-1][k][0]-'0';
                        }
                    }
                    else
                    {
                        tmp_sort=nb_sorts->nb[i][k];
                    }

                    if (l>=tmp_sort)
                    {
                        xml_ecrit_dernier_attribut(tmp3_node,"table:style-name","Tableau1.VD21.n"); /* remplace le dernier attribut */
                    }
                    else
                    { /* il faut inscrire le sort mémorisé pour la journée */
                        xml_ecrit_dernier_attribut(tmp3_node,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
                        tmp3_node=xml_ajoute_fin(tmp3_node,"text:p");
                        //if (sort_nb_sort_clerc_connu[k-1]!=0)
                        if (tmp_sort!=0)
                        {
                            tmp4_node=xml_ajoute_fin(tmp3_node,"draw:control");

                            xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp4_node,"text:anchor-type","as-char");
                            xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp4_node,"draw:z-index","5");

                            xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp4_node,"draw:name","Forme1");
                            xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp4_node,"draw:style-name","gr2");
                            xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp4_node,"draw:text-style-name","P41");
                            xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp4_node,"svg:width","2.5cm");
                            xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp4_node,"svg:height","0.4cm");
                            xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                            sprintf(aff,"control_%hd",nb_forme);
                            nb_forme++;
                            xml_ecrit_dernier_attribut(tmp4_node,"draw:control",aff);
                            xml_ajoute_fin(tmp3_node,"text:s");

                            /* faire la liste de sort pour le niveau k, la ligne l */
                            tmp3_node=g_markup_dom_nom (node->parent,"formulaire_perso","name");
                            node_lien_sorts=g_markup_dom_nom(node,"suppr_clerc0","name");
                            if (tmp3_node!=NULL)
                            {
                                tmp4_node=xml_ajoute_fin(tmp3_node,"form:form");
                                copie_node(tmp4_node,tmp3_node->fils);
                                xml_ecrit_attribut(tmp4_node,"form:name",aff);
                                tmp5_node=g_markup_dom_node(tmp4_node,"form:listbox");
                                xml_ajoute_fin_attribut(tmp5_node); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp5_node,"xml:id",aff);
                                xml_ajoute_fin_attribut(tmp5_node); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp5_node,"form:id",aff);
                                //xml_ajoute_fin_attribut(tmp5_node); /* rajouter un élément attribut */
                                sprintf(aff,"scontrol_%hd",nb_forme-1);
                                xml_ecrit_attribut(tmp5_node,"form:name",aff);
        /*					<form:listbox form:name="Zone de liste 1" form:control-implementation="ooo:com.sun.star.form.component.ListBox" xml:id="control1" form:id="control1" form:dropdown="true" form:printable="true" form:size="10" form:bound-column="1"> */
                                tmp5_node=g_markup_dom_node(tmp4_node,"form:listbox");
                                /* variable disponible : m */

                                if (l==0)
                                {
                                    sprintf(aff,"Niveau %hd : ",k);
                                    tmp4_node=xml_ajoute_fin(node_lien_sorts,"text:span");
                                    xml_ajoute_fin_attribut(tmp4_node);
                                    xml_ecrit_dernier_attribut(tmp4_node,"text:style-name","T13");
                                    xml_ajoute_fin_texte(tmp4_node);
                                    xml_ecrit_dernier_texte(tmp4_node, aff);

                                }
                                for(m=0;m<sort_nb_sort_clerc_connu[k-1];m++)
                                {
                                    tmp4_node=xml_ajoute_fin(tmp5_node,"form:option");
                                    xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                                    xml_ecrit_dernier_attribut(tmp4_node,"form:label",liste_clerc[k-1][m]);
                                    if (l==0)
                                    {
                                        if (m!=0)
                                        {
                                             xml_ajoute_fin_texte(node_lien_sorts);
                                             xml_ecrit_dernier_texte(node_lien_sorts," ; ");
                                        }
                                        tmp4_node=xml_ajoute_fin(node_lien_sorts,"text:a");
                                        xml_ajoute_fin_attribut(tmp4_node);
                                        xml_ecrit_dernier_attribut(tmp4_node,"xlink:type","simple");
                                        sprintf(aff,"https://dnd.siaud.org/pretre.php?sort=%ld",liste_clerc_id[k-1][m]);
                                        xml_ecrit_dernier_attribut(tmp4_node,"xlink:href",aff);
                                        tmp4_node=xml_ajoute_fin(tmp4_node,"text:span");
                                        xml_ajoute_fin_attribut(tmp4_node);
                                        xml_ecrit_dernier_attribut(tmp4_node,"text:style-name","T0");
                                        xml_ajoute_fin_texte(tmp4_node);
                                        xml_ecrit_dernier_texte(tmp4_node,liste_clerc[k-1][m]);
                                    }
                                }
                                if (l==0)
                                {
                                    xml_ajoute_fin(node_lien_sorts,"text:line-break");
                                }
        /*						<form:properties>
                                <form:option form:label="Sort 1"/>
                                <form:option form:label="sort 2"/>*/

                            }
                            else
                            {
                                /* dialogue("Erreur : pas d'entrée nommée \"formulaire_perso\" dans content.xml" ,0); */
                            }
                        }
                    }
                }
                /* k=7, pour fermer le tableau */
                tmp3_node=xml_ajoute_fin(tmp2_node,"table:table-cell");
                xml_ajoute_fin_attribut(tmp3_node); /* rajouter un élément attribut */
                if ((i>0 && l>=nb_sorts->nb[i-1][7]) || j<7)
                {
                    xml_ecrit_dernier_attribut(tmp3_node,"table:style-name","Tableau1.VD31.n"); /* remplace le dernier attribut */
                }
                else
                { /* il faut inscrire le sort mémorisé pour la journée */
                    xml_ecrit_dernier_attribut(tmp3_node,"table:style-name","Tableau1.VD31"); /* remplace le dernier attribut */
                    tmp3_node=xml_ajoute_fin(tmp3_node,"text:p");
                    if (sort_nb_sort_clerc_connu[k-1]!=0)
                    {
                        tmp4_node=xml_ajoute_fin(tmp3_node,"draw:control");

                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"text:anchor-type","as-char");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:z-index","5");

                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:name","Forme1");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:style-name","gr2");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:text-style-name","P41");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"svg:width","2.5cm");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"svg:height","0.4cm");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        sprintf(aff,"control_%hd",nb_forme);
                        nb_forme++;
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:control",aff);
                        xml_ajoute_fin(tmp3_node,"text:s");

                        /* faire la liste de sort pour le niveau k, la ligne l */
                        tmp3_node=g_markup_dom_nom (node->parent,"formulaire_perso","name");
                        if (tmp3_node!=NULL)
                        {
                            tmp4_node=xml_ajoute_fin(tmp3_node,"form:form");
                            copie_node(tmp4_node,tmp3_node->fils);
                            tmp5_node=g_markup_dom_node(tmp4_node,"form:listbox");
                            xml_ajoute_fin_attribut(tmp5_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp5_node,"xml:id",aff);
                            xml_ajoute_fin_attribut(tmp5_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp5_node,"form:id",aff);
    /*					<form:listbox form:name="Zone de liste 1" form:control-implementation="ooo:com.sun.star.form.component.ListBox" xml:id="control1" form:id="control1" form:dropdown="true" form:printable="true" form:size="10" form:bound-column="1"> */
                            tmp5_node=g_markup_dom_node(tmp4_node,"form:listbox");
                            /* variable disponible : m */
                            for(m=0;m<sort_nb_sort_clerc_connu[k-1];m++)
                            {
                                tmp3_node=xml_ajoute_fin(tmp5_node,"form:option");
                                xml_ajoute_fin_attribut(tmp3_node); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp3_node,"form:label",liste_clerc[k-1][m]);
                            }

    /*						<form:properties>
                            <form:option form:label="Sort 1"/>
                            <form:option form:label="sort 2"/>*/

                        }
                        else
                        {
                            /* dialogue("Erreur : pas d'entrée nommée \"formulaire_perso\" dans content.xml" ,0); */
                        }
                    }
                }
            }
        }
    }

    if ((add_==ADD1 && (s_c->add[ADD].sort_mago!=NULL || s_c->add[ADD].sort_illus!=NULL))
           || (add_==ADD2 &&  s_c->add[ADD2].sort_mago!=NULL))
    {
        if (*mag>0)
        {
            tmp1_node=g_markup_dom_nom(node,"section_sorts_mag","name");
            xml_ajoute_fin(tmp1_node,"text:p");
            xml_ajoute_fin(tmp1_node,"text:p");
            decale_node(tmp1_node,0);
            decale_node(tmp1_node,0);
            copie_node(tmp1_node->fils,tmp1_node->fils+2); // copie du tableau

            xml_ajoute_fin_attribut(tmp1_node->fils+1); // mise en place du texte
            xml_ecrit_dernier_attribut(tmp1_node->fils+1,"text:style-name","P24");
            xml_ajoute_fin_attribut(tmp1_node->fils+1);
            xml_ecrit_dernier_attribut(tmp1_node->fils+1,"name","suppr_mag0");

            tmp1_node=tmp1_node->fils; // effacer le tableau précédent
            if (tmp1_node!=NULL)
            {
                while(tmp1_node->nb_fils>3)
                {
                    xml_sup_node(tmp1_node->fils+2);
                }
            }
            else
            {
                printf("Erreur effaçage name=%s\n",aff);
            }
        }
        else
        { /* premier passage avec des sorts de mag */
        }
        if (add_==ADD1 && (s_c->add[ADD].sort_mago!=NULL || s_c->add[ADD].sort_illus!=NULL))
        {
            if (s_c->add[ADD].sort_mago!=NULL)
            {
                 nb_sorts=s_c->add[ADD].sort_mago;
            }
            else
            {
                nb_sorts=s_c->add[ADD].sort_illus;
            }
            if (tabl_inte1[inte-1][1][0]!=0)
            {
                 sscanf(tabl_inte1[inte-1][1],"%hd",&j); /* limite du niveau de sort */
            }
            else
            {
                j=0;
            }
        }
        else
        { /* j : limitation par l'intelligence du nombre de niveaux compréhensibles */
            nb_sorts=s_c->add[ADD2].sort_mago;
            if (tabl_inte2[inte-1][1][0]!=0)
            {
                 sscanf(tabl_inte2[inte-1][1],"%hd",&j); /* limite du niveau de sort */
            }
            else
            {
                j=0;
            }
        }
        for (i=1;i<niv && nb_sorts->nb[i]!=NULL;i++) ; /* i monte au max du niveau ou de la compétence en magie de la classe */
                                                       /* on pourrait avoir un max de 0 ! Si nb_sorts->nb[i]!=NULL : plus de sorts en plus */
        if (nb_sorts->nb[i]==NULL) i--;                /* i si c'est le max de la classe qui a été dépassé, on retourne au max */

        tmp1_node=g_markup_dom_nom(node,"magicien","name");
        tmp2_node=g_markup_dom_nom(node,"mag_tps","name");
        xml_ecrit_dernier_texte(tmp1_node->fils[0].fils,metier);
        tps_sort_total=0;
        max_sort=0;
        for (l=1;l<=j && nb_sorts->nb[i-1][l]!=-1;l++)
        {
            sprintf(aff,"niv %d : %hd",l,nb_sorts->nb[i-1][l]);
            xml_ecrit_dernier_texte(tmp1_node->fils[l].fils,aff);
            tps_sort=nb_sorts->nb[i-1][l]*l*10;
            tps_sort_total+=tps_sort;
            xml_ecrit_dernier_texte(tmp2_node->fils[l].fils,h_m(tps_sort,aff));
            max_sort=max(max_sort,nb_sorts->nb[i-1][l]);
        }
        for(;l<=9;l++)
        {
            sprintf(aff,"niv %d : 0",l);
            xml_ecrit_dernier_texte(tmp1_node->fils[l].fils,aff);
            xml_ecrit_dernier_texte(tmp2_node->fils[l].fils,"-");
        }
        xml_ecrit_dernier_texte(tmp2_node->fils[0].fils,h_m(tps_sort_total,aff));

        /*for(;l<9;l++)
        {
                sprintf(aff,"niv %d : %hd",l,nb_sorts->nb[i-1][l]);
                xml_ecrit_dernier_texte(tmp1_node->fils[l].fils,"-");
        }*/
        for (l=0;l<j && nb_sorts->nb[i][l]!=-1;l++); /* j : recherche du niveau de sort max maitrisé */
        j=l; /* passe du niveau max en fonction de l'intelligence uniquement, à l'int + niveau */
        if (nb_sorts->nb[i][j]==-1) j--;               /* recherche du max de compétance */
        if (j<9) /* pas uns classe pure de lanceur de sorts */
        {
                aff[0]=0;
                sprintf(aff,"m : %hu",nb_sorts->nb[i-1][0]); /* le niveau de maîtrise */
                xml_ecrit_dernier_texte(tmp1_node->fils[9].fils,aff);
                xml_ecrit_dernier_texte(tmp2_node->fils[9].fils,"-");
                for (l=j;l<9;l++)
                {
                    sprintf(aff,"niv %d : 0",l);
                    xml_ecrit_dernier_texte(tmp1_node->fils[l].fils,aff);
                }
        }
        (*mag)++;
        if ( s_c->add[ADD2].nom!=NULL && compare_sans_casse("entropiste",s_c->add[ADD2].nom)==0)
        {
            tmp2_node=tmp1_node->parent;
            xml_ajoute_fin(tmp2_node,"table:table-row");
            tmp1_node=tmp2_node->fils+tmp2_node->nb_fils-2;
            tmp2_node=tmp2_node->fils+tmp2_node->nb_fils-1;
            copie_node(tmp2_node,tmp1_node);
            niv=_min(niv,9);
            niv--;
            for(l=0;l<10;l++)
            {
                sprintf(aff,"%d:%d; %d:%d",2*l+1,hiatus[niv][2*l],2*l+2,hiatus[niv][2*l+1]);
                xml_ecrit_dernier_texte(tmp2_node->fils[l].fils,aff);
            }
        }
        else
        {
            /* fin de gestion du cas entropiste */
        }
        if (g_markup_dom_nom (node->parent,"formulaire_perso","name")!=NULL)  /* if : ne pas le faire pour le groupe */
        for (l=0;l<max_sort;l++)
        {
            tmp2_node=ajoute_node(g_markup_dom_nom(node,"magicien","name")->parent,2+l,"table:table-row");
            sprintf(aff,"sort n°%hd",l+1);
            tmp3_node=xml_ajoute_fin(tmp2_node,"table:table-cell");
            xml_ajoute_fin(tmp3_node,"text:p");
            xml_ecrit_dernier_texte(tmp3_node->fils,aff);
                xml_ajoute_fin_attribut(tmp3_node->fils);
                xml_ecrit_dernier_attribut(tmp3_node->fils,"text:style-name","P15");
            xml_ajoute_fin_attribut(tmp3_node); /* rajouter un élément attribut */
            xml_ecrit_dernier_attribut(tmp3_node,"table:style-name","Tableau3.A2"); /* remplace le dernier attribut */
            for (k=1;k<9;k++)
            {
                tmp3_node=xml_ajoute_fin(tmp2_node,"table:table-cell");
                xml_ajoute_fin_attribut(tmp3_node); /* rajouter un élément attribut */
                if (k>j || l>=nb_sorts->nb[i-1][k])/* niveau sup au max permis ou aucun sort */
                {
                    xml_ecrit_dernier_attribut(tmp3_node,"table:style-name","Tableau1.VD21.n"); /* remplace le dernier attribut */
                }
                else
                { /* il faut inscrire le sort mémorisé pour la journée */
                    xml_ecrit_dernier_attribut(tmp3_node,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
                    tmp3_node=xml_ajoute_fin(tmp3_node,"text:p");
                    if (sort_nb_sort_mag_connu[k-1]!=0)
                    {
                        tmp4_node=xml_ajoute_fin(tmp3_node,"draw:control");

                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"text:anchor-type","as-char");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:z-index","5");

                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:name","Forme1");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:style-name","gr2");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:text-style-name","P41");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"svg:width","2cm");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"svg:height","0.4cm");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        sprintf(aff,"control_%hd",nb_forme);
                        nb_forme++;
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:control",aff);
                        xml_ajoute_fin(tmp3_node,"text:s");

                        /* faire la liste de sort pour le niveau k, la ligne l */
                        tmp3_node=g_markup_dom_nom (node->parent,"formulaire_perso","name");
                        node_lien_sorts=g_markup_dom_nom(node,"suppr_mag0","name");
                        if (tmp3_node!=NULL)
                        {
                            tmp4_node=xml_ajoute_fin(tmp3_node,"form:form");
                            copie_node(tmp4_node,tmp3_node->fils);
                            tmp5_node=g_markup_dom_node(tmp4_node,"form:listbox");
                            xml_ajoute_fin_attribut(tmp5_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp5_node,"xml:id",aff);
                            xml_ajoute_fin_attribut(tmp5_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp5_node,"form:id",aff);
    /*					<form:listbox form:name="Zone de liste 1" form:control-implementation="ooo:com.sun.star.form.component.ListBox" xml:id="control1" form:id="control1" form:dropdown="true" form:printable="true" form:size="10" form:bound-column="1"> */
                            tmp5_node=g_markup_dom_node(tmp4_node,"form:listbox");
                            /* variable disponible : m */
                            if (l==0 && *mag==1)
                            {
                                sprintf(aff,"Niveau %hd : ",k);
                                //tmp3_node = g_markup_dom_nom(node, "suppr_mag0", "name");
                                tmp3_node=xml_ajoute_fin(node_lien_sorts,"text:span");
                                xml_ajoute_fin_attribut(tmp3_node);
                                xml_ecrit_dernier_attribut(tmp3_node,"text:style-name","T13");
                                xml_ajoute_fin_texte(tmp3_node);
                                xml_ecrit_dernier_texte(tmp3_node, aff);
                                //aff[0] = 0;
                            }

                            for(m=0;m<sort_nb_sort_mag_connu[k-1];m++)
                            {
                                tmp3_node=xml_ajoute_fin(tmp5_node,"form:option");
                                xml_ajoute_fin_attribut(tmp3_node); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp3_node,"form:label",liste_mag[k-1][m]);

                                if (l==0 && *mag==1)
                                { /* génère la liste simple de sorts avec lien vers le sort */
                                    if (m!=0)
                                    {
                                         xml_ajoute_fin_texte(node_lien_sorts);
                                         xml_ecrit_dernier_texte(node_lien_sorts," ; ");
                                    }
                                    tmp3_node=xml_ajoute_fin(node_lien_sorts,"text:a");
                                    xml_ajoute_fin_attribut(tmp3_node);
                                    xml_ecrit_dernier_attribut(tmp3_node,"xlink:type","simple");
                                    sprintf(aff,"https://dnd.siaud.org/mago.php?sort=%ld",liste_mag_id[k-1][m]);
                                    xml_ecrit_dernier_attribut(tmp3_node,"xlink:href",aff);
                                    tmp3_node=xml_ajoute_fin(tmp3_node,"text:span");
                                    xml_ajoute_fin_attribut(tmp3_node);
                                    xml_ecrit_dernier_attribut(tmp3_node,"text:style-name","T0");
                                    xml_ajoute_fin_texte(tmp3_node);
                                    xml_ecrit_dernier_texte(tmp3_node,liste_mag[k-1][m]);
                                }
                            }
                            if (l==0 && *mag==1)
                            {
                                xml_ajoute_fin(node_lien_sorts,"text:line-break");
                            }

    /*						<form:properties>
                            <form:option form:label="Sort 1"/>
                            <form:option form:label="sort 2"/>*/

                        }
                        else
                        {
                            /* dialogue("Erreur : pas d'entrée nommée \"formulaire_perso\" dans content.xml" ,0); */
                        }
                    }
                }
            }
            /* k=9, pour fermer le tableau */
            tmp3_node=xml_ajoute_fin(tmp2_node,"table:table-cell");
            xml_ajoute_fin_attribut(tmp3_node); /* rajouter un élément attribut */
            if ( j<9 || l>=nb_sorts->nb[i-1][k])
            {
                xml_ecrit_dernier_attribut(tmp3_node,"table:style-name","Tableau1.VD31.n"); /* remplace le dernier attribut */
            }
            else
            { /* il faut inscrire le sort mémorisé pour la journée */
                xml_ecrit_dernier_attribut(tmp3_node,"table:style-name","Tableau1.VD31"); /* remplace le dernier attribut */
                    tmp3_node=xml_ajoute_fin(tmp3_node,"text:p");
                    if (sort_nb_sort_mag_connu[8]!=0)
                    {
                        tmp4_node=xml_ajoute_fin(tmp3_node,"draw:control");

                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"text:anchor-type","as-char");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:z-index","5");

                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:name","Forme1");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:style-name","gr2");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:text-style-name","P41");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"svg:width","2cm");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp4_node,"svg:height","0.4cm");
                        xml_ajoute_fin_attribut(tmp4_node); /* rajouter un élément attribut */
                        sprintf(aff,"control_%hd",nb_forme);
                        nb_forme++;
                        xml_ecrit_dernier_attribut(tmp4_node,"draw:control",aff);
                        xml_ajoute_fin(tmp3_node,"text:s");

                        /* faire la liste de sort pour le niveau k, la ligne l */
                        tmp3_node=g_markup_dom_nom (node->parent,"formulaire_perso","name");
                        if (tmp3_node!=NULL)
                        {
                            tmp4_node=xml_ajoute_fin(tmp3_node,"form:form");
                            copie_node(tmp4_node,tmp3_node->fils);
                            tmp5_node=g_markup_dom_node(tmp4_node,"form:listbox");
                            xml_ajoute_fin_attribut(tmp5_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp5_node,"xml:id",aff);
                            xml_ajoute_fin_attribut(tmp5_node); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(tmp5_node,"form:id",aff);
    /*					<form:listbox form:name="Zone de liste 1" form:control-implementation="ooo:com.sun.star.form.component.ListBox" xml:id="control1" form:id="control1" form:dropdown="true" form:printable="true" form:size="10" form:bound-column="1"> */
                            tmp5_node=g_markup_dom_node(tmp4_node,"form:listbox");
                            /* variable disponible : m */
                            for(m=0;m<sort_nb_sort_mag_connu[k-1];m++)
                            {
                                tmp3_node=xml_ajoute_fin(tmp5_node,"form:option");
                                xml_ajoute_fin_attribut(tmp3_node); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp3_node,"form:label",liste_mag[k-1][m]);
                            }

    /*						<form:properties>
                            <form:option form:label="Sort 1"/>
                            <form:option form:label="sort 2"/>*/

                        }
                        else
                        {
                            /* dialogue("Erreur : pas d'entrée nommée \"formulaire_perso\" dans content.xml" ,0); */
                        }
                    }
            }
        }
    }

    (void)version_;
}

char * h_m              (unsigned short minute,char * ch)
{
    if (minute<60)
    {
         sprintf(ch,"%hu min",minute);
    }
    else
    {
        if (minute%60==0)
        {
            sprintf(ch,"%huh",minute/60);
        }
        else
        {
            sprintf(ch,"%huh%humin",minute/60,minute%60);
        }
    }

    return ch;
}

signed char dans_source(unsigned long long source,char ** origine) /* source du sort, origine permise au perso */
{
    unsigned short i;
    signed char present=0;

    while (source>0)
    {
        i=source%100-1;
        if (origine[1][i]==1)
        {
            present=1;
        }

        source/=100;
    }

    return present;
}


GMarkupDomNode * nv_tableau_sort(GMarkupDomNode * ooo,unsigned short niveau,char * metier)
{
    GMarkupDomNode * tmp_node, * node;
    char ch[LONG];
    unsigned short i;
    unsigned short mago=!compare_sans_casse(metier,"mago");

    /* rajouter un tableau à la fin */
    node=g_markup_dom_node(ooo,"office:text");
    xml_ajoute_fin(node,"text:p");
    xml_ajoute_fin_attribut(node->fils+node->nb_fils-1); /* rajouter un élément attribut */
    //xml_ecrit_dernier_attribut(node->fils+node->nb_fils-1,"text:style-name","SautePage"); /* remplace le dernier attribut */
    xml_ajoute_fin(node,"table:table");
    node=node->fils+node->nb_fils-1; /* on arrive sur la dernière node */
    for (i=0; i<9+mago; i++) xml_ajoute_fin(node,"table:table-column");
    for (i=0; i<9+mago; i++)
    {
        xml_ajoute_fin_attribut(node->fils+i); /* rajouter un élément attribut */
        xml_ecrit_dernier_attribut(node->fils+i,"table:style-name","Tableau1.VDA"); /* remplace le dernier attribut */
    }
    xml_ecrit_dernier_attribut(node->fils,"table:style-name","Tableau1.VDL"); /* remplace le dernier attribut */
    xml_ecrit_dernier_attribut(node->fils+2,"table:style-name","Tableau1.VDAE"); /* remplace le dernier attribut */
    xml_ecrit_dernier_attribut(node->fils+4,"table:style-name","Tableau1.C"); /* remplace le dernier attribut */
    xml_ecrit_dernier_attribut(node->fils+6,"table:style-name","Tableau1.C"); /* remplace le dernier attribut */
    xml_ecrit_dernier_attribut(node->fils+7,"table:style-name","Tableau1.CL"); /* remplace le dernier attribut */
    xml_ecrit_dernier_attribut(node->fils+8,"table:style-name","Tableau1.VDB"); /* remplace le dernier attribut */
    xml_ajoute_fin(node,"table:table-row"); /* node poinetra sur le tableau du niveau */
    tmp_node=node->fils+node->nb_fils-1; /* on arrive sur la dernière node */
    /* rajouter la ligne de titre */
    /* première case : le nom du sort_ */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    sprintf(ch,"Sorts %s niv %hu",metier,niveau);
    xml_ajoute_fin_attribut(tmp_node->fils); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils,"table:style-name","Tableau1.VD1"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils,"office:value-type","string"); /* remplace le dernier attribut */
    xml_ajoute_fin(tmp_node->fils,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils->fils);
    xml_ecrit_dernier_texte(tmp_node->fils->fils,ch);
    /* deuxième case : portee */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+1,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[1].fils);
    xml_ecrit_dernier_texte(tmp_node->fils[1].fils,"Portée");
    xml_ajoute_fin_attribut(tmp_node->fils+1); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+1,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils+1); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+1,"office:value-type","string"); /* remplace le dernier attribut */
    /* vsm */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */


    xml_ajoute_fin(tmp_node->fils+2,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[2].fils);
    xml_ecrit_dernier_texte(tmp_node->fils[2].fils,"vsm");
    xml_ajoute_fin_attribut(tmp_node->fils+2); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+2,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils+2); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+2,"office:value-type","string"); /* remplace le dernier attribut */
    /* duree */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+3,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[3].fils);
    xml_ecrit_dernier_texte(tmp_node->fils[3].fils,"Durée");
    xml_ajoute_fin_attribut(tmp_node->fils+3); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+3,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils+3); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+3,"office:value-type","string"); /* remplace le dernier attribut */
    /* lance */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+4,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[4].fils);
    xml_ecrit_dernier_texte(tmp_node->fils[4].fils,"T lancé");
    xml_ajoute_fin_attribut(tmp_node->fils+4); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+4,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils+4); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+4,"office:value-type","string"); /* remplace le dernier attribut */
    /* effet */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+5,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[5].fils);
    xml_ecrit_dernier_texte(tmp_node->fils[5].fils,"Z effet");
    xml_ajoute_fin_attribut(tmp_node->fils+5); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+5,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils+5); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+5,"office:value-type","string"); /* remplace le dernier attribut */
    /* save */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+6,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[6].fils);
    xml_ecrit_dernier_texte(tmp_node->fils[6].fils,"Save");
    xml_ajoute_fin_attribut(tmp_node->fils+6); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+6,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils+6); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+6,"office:value-type","string"); /* remplace le dernier attribut */
    /* composant */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+7,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[7].fils);
    xml_ecrit_dernier_texte(tmp_node->fils[7].fils,"Composant");
    xml_ajoute_fin_attribut(tmp_node->fils+7); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+7,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils+7); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+7,"office:value-type","string"); /* remplace le dernier attribut */
    /* sphère */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+8,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[8].fils);
    if (strcmp(metier,"mago")==0)
    {
         xml_ecrit_dernier_texte(tmp_node->fils[8].fils,"École");
    }
    else
    {
         xml_ecrit_dernier_texte(tmp_node->fils[8].fils,"Sphère");
    }
    xml_ajoute_fin_attribut(tmp_node->fils+8); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+8,"table:style-name","Tableau1.VD3"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils+8); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+8,"office:value-type","string"); /* remplace le dernier attribut */
    if (mago)
    {
      xml_ecrit_dernier_attribut(node->fils+9,"table:style-name","Tableau1.VDAE"); /* remplace le dernier attribut */
      xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
      xml_ajoute_fin(tmp_node->fils+9,"text:p");
      xml_ajoute_fin_texte(tmp_node->fils[9].fils);
      xml_ecrit_dernier_texte(tmp_node->fils[9].fils,"Voy");
      xml_ajoute_fin_attribut(tmp_node->fils+9); /* rajouter un élément attribut */
      xml_ecrit_dernier_attribut(tmp_node->fils+9,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
      xml_ajoute_fin_attribut(tmp_node->fils+9); /* rajouter un élément attribut */
      xml_ecrit_dernier_attribut(tmp_node->fils+9,"office:value-type","string"); /* remplace le dernier attribut */
    }
    return node;
}

void insere_ligne_pretre(GMarkupDomNode * node,struct_sortileges * sort_,unsigned short info)
{
    GMarkupDomNode * tmp_node=NULL;
    char ch[LONG];
    unsigned short /*i=0,*/j=0;


    xml_ajoute_fin(node,"table:table-row"); /* node pointera sur le tableau du niveau */
    tmp_node=node->fils+node->nb_fils-1; /* on arrive sur la dernière node */
    /* première case : le nom du sort_ */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils->fils);
    xml_ecrit_dernier_texte(tmp_node->fils->fils,sort_->nom);
    xml_ajoute_fin_attribut(tmp_node->fils); /* rajouter un élément attribut */
    if (node->nb_fils%2==0)
    {
      xml_ecrit_dernier_attribut(tmp_node->fils,"table:style-name","Tableau1.VD11"); /* remplace le dernier attribut */
    }
    else
    {
      xml_ecrit_dernier_attribut(tmp_node->fils,"table:style-name","Tableau1.VD11.n"); /* remplace le dernier attribut */
    }
    xml_ajoute_fin_attribut(tmp_node->fils); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils,"office:value-type","string"); /* remplace le dernier attribut */
    if (info)
    {
      switch(info%10)
      {
        case 2 :
          xml_ajoute_fin_attribut(tmp_node->fils->fils); /* rajouter un élément attribut */
          xml_ecrit_dernier_attribut(tmp_node->fils->fils,"text:style-name","pas_compris"); /* remplace le dernier attribut */
          break;
        case 3 :
          xml_ajoute_fin_attribut(tmp_node->fils->fils); /* rajouter un élément attribut */
          xml_ecrit_dernier_attribut(tmp_node->fils->fils,"text:style-name","pas_lu"); /* remplace le dernier attribut */
          break;
        case 4 :
          xml_ajoute_fin_attribut(tmp_node->fils->fils); /* rajouter un élément attribut */
          xml_ecrit_dernier_attribut(tmp_node->fils->fils,"text:style-name","pas_ecole"); /* remplace le dernier attribut */
          break;
        case 5 :
          xml_ajoute_fin_attribut(tmp_node->fils->fils); /* rajouter un élément attribut */
          xml_ecrit_dernier_attribut(tmp_node->fils->fils,"text:style-name","ecole_OK"); /* remplace le dernier attribut */
          break;
        default :
          xml_ajoute_fin_attribut(tmp_node->fils->fils); /* rajouter un élément attribut */
          xml_ecrit_dernier_attribut(tmp_node->fils->fils,"text:style-name","sort_"); /* remplace le dernier attribut */
          break;
      }
    }
    else
    {
    }



    /* deuxième case : portee */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+1,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[1].fils);
    xml_ecrit_dernier_texte(tmp_node->fils[1].fils,sort_->portee);
    xml_ajoute_fin_attribut(tmp_node->fils+1); /* rajouter un élément attribut */
    if (node->nb_fils%2==0)
    {
      xml_ecrit_dernier_attribut(tmp_node->fils+1,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
    }
    else
    {
      xml_ecrit_dernier_attribut(tmp_node->fils+1,"table:style-name","Tableau1.VD21.n"); /* remplace le dernier attribut */
    }
    xml_ajoute_fin_attribut(tmp_node->fils+1); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+1,"office:value-type","string"); /* remplace le dernier attribut */
    /* vsm */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+2,"text:p");
    xml_ajoute_fin(tmp_node->fils[2].fils,"text:span");
    xml_ajoute_fin_texte(tmp_node->fils[2].fils->fils);
    xml_ecrit_dernier_texte(tmp_node->fils[2].fils->fils,sort_->vsm);
    xml_ajoute_fin_attribut(tmp_node->fils+2); /* rajouter un élément attribut */
    if (node->nb_fils%2==0)
    {
      xml_ecrit_dernier_attribut(tmp_node->fils+2,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
    }
    else
    {
      xml_ecrit_dernier_attribut(tmp_node->fils+2,"table:style-name","Tableau1.VD21.n"); /* remplace le dernier attribut */
    }
    xml_ajoute_fin_attribut(tmp_node->fils+2); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+2,"office:value-type","string"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils[2].fils->fils); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils[2].fils->fils,"text:style-name","T6b"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils[2].fils); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils[2].fils,"text:style-name","PVD"); /* remplace le dernier attribut */
    /* duree */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+3,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[3].fils);
    xml_ecrit_dernier_texte(tmp_node->fils[3].fils,sort_->duree);
    xml_ajoute_fin_attribut(tmp_node->fils+3); /* rajouter un élément attribut */
    if (node->nb_fils%2==0)
    {
      xml_ecrit_dernier_attribut(tmp_node->fils+3,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
    }
    else
    {
      xml_ecrit_dernier_attribut(tmp_node->fils+3,"table:style-name","Tableau1.VD21.n"); /* remplace le dernier attribut */
    }
    xml_ajoute_fin_attribut(tmp_node->fils+3); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+3,"office:value-type","string"); /* remplace le dernier attribut */
    /* lance */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+4,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[4].fils);
    xml_ecrit_dernier_texte(tmp_node->fils[4].fils,sort_->lance);
    xml_ajoute_fin_attribut(tmp_node->fils+4); /* rajouter un élément attribut */
    if (node->nb_fils%2==0)
    {
      xml_ecrit_dernier_attribut(tmp_node->fils+4,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
    }
    else
    {
      xml_ecrit_dernier_attribut(tmp_node->fils+4,"table:style-name","Tableau1.VD21.n"); /* remplace le dernier attribut */
    }
    xml_ajoute_fin_attribut(tmp_node->fils+4); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+4,"office:value-type","string"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils[4].fils); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils[4].fils,"text:style-name","PVD"); /* remplace le dernier attribut */
    /* effet */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+5,"text:p");
    xml_ajoute_fin(tmp_node->fils[5].fils,"text:span");
    xml_ajoute_fin_texte(tmp_node->fils[5].fils->fils);
    xml_ecrit_dernier_texte(tmp_node->fils[5].fils->fils,sort_->effet);
    xml_ajoute_fin_attribut(tmp_node->fils+5); /* rajouter un élément attribut */
    if (node->nb_fils%2==0)
    {
      xml_ecrit_dernier_attribut(tmp_node->fils+5,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
    }
    else
    {
     xml_ecrit_dernier_attribut(tmp_node->fils+5,"table:style-name","Tableau1.VD21.n"); /* remplace le dernier attribut */
    }
    xml_ajoute_fin_attribut(tmp_node->fils+5); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+5,"office:value-type","string"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils[5].fils->fils); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils[5].fils->fils,"text:style-name","T6"); /* remplace le dernier attribut */
    /* save */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+6,"text:p");
    xml_ajoute_fin_texte(tmp_node->fils[6].fils);
    xml_ecrit_dernier_texte(tmp_node->fils[6].fils,sort_->save);
    xml_ajoute_fin_attribut(tmp_node->fils+6); /* rajouter un élément attribut */
    if (node->nb_fils%2==0)
    {
     xml_ecrit_dernier_attribut(tmp_node->fils+6,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
    }
    else
    {
     xml_ecrit_dernier_attribut(tmp_node->fils+6,"table:style-name","Tableau1.VD21.n"); /* remplace le dernier attribut */
    }
    xml_ajoute_fin_attribut(tmp_node->fils+6); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+6,"office:value-type","string"); /* remplace le dernier attribut */
    /* composant */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+7,"text:p");
    xml_ajoute_fin(tmp_node->fils[7].fils,"text:span");
    xml_ajoute_fin_texte(tmp_node->fils[7].fils->fils);
    xml_ecrit_dernier_texte(tmp_node->fils[7].fils->fils,sort_->composant);
    xml_ajoute_fin_attribut(tmp_node->fils+7); /* rajouter un élément attribut */
    if (node->nb_fils%2==0)
    {
     xml_ecrit_dernier_attribut(tmp_node->fils+7,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
    }
    else
    {
     xml_ecrit_dernier_attribut(tmp_node->fils+7,"table:style-name","Tableau1.VD21.n"); /* remplace le dernier attribut */
    }
    xml_ajoute_fin_attribut(tmp_node->fils+7); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+7,"office:value-type","string"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils[7].fils->fils); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils[7].fils->fils,"text:style-name","T6b"); /* remplace le dernier attribut */
    /* sphère */
    xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
    xml_ajoute_fin(tmp_node->fils+8,"text:p");
    ch[0]=0;
    if (info==0)
    {
        for(j=0; sphere[ADD2][j].nom!=NULL; j++)
        {
            if (sort_->sphere&(1<<j))
            {
                /*if (j==14)
                { // général passe de 0 à 15
                    i=0;
                }
                else if (j<=13 && j>=9)
                { // décalage entre 11 et 15
                    i=j+2;
                }
                else
                {
                    i=j+1;
                }*/

                if (ch[0]!=0)
                {
                    strncat(ch," ",LONG-1);
                    strncat(ch,sphere[ADD2][j].nom,LONG-1);
                }
                else
                {
                    strncpy(ch,sphere[ADD2][j].nom,LONG-1);
                }
            }
        }
    }
    else
    {
        for(j=0; j<nb_ecole; j++)
        {
            if (sort_->sphere&(1<<(ecole[ADD2][j].clef-1)))
            {
                if (ch[0]!=0)
                { if (j<8 || j==20 || j==23)
                  {
                    strncat(ch," ",LONG-1);
                    strncat(ch,ecole[ADD2][j].nom,LONG-1);
                  }
                }
                else
                { if (j<8 || j==20 || j==23)
                  {
                     strncpy(ch,ecole[ADD2][j].nom,LONG-1);
                  }
                }
            }
        }
    }
    xml_ajoute_fin(tmp_node->fils[8].fils,"text:span");
    xml_ajoute_fin_texte(tmp_node->fils[8].fils->fils);
    xml_ecrit_dernier_texte(tmp_node->fils[8].fils->fils,ch);
    xml_ajoute_fin_attribut(tmp_node->fils+8); /* rajouter un élément attribut */
    if (node->nb_fils%2==0)
    {
      xml_ecrit_dernier_attribut(tmp_node->fils+8,"table:style-name","Tableau1.VD31"); /* remplace le dernier attribut */
    }
    else
    {
      xml_ecrit_dernier_attribut(tmp_node->fils+8,"table:style-name","Tableau1.VD31.n"); /* remplace le dernier attribut */
    }
    xml_ajoute_fin_attribut(tmp_node->fils+8); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils+8,"office:value-type","string"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils[8].fils->fils); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils[8].fils->fils,"text:style-name","T6b"); /* remplace le dernier attribut */
    xml_ajoute_fin_attribut(tmp_node->fils[8].fils); /* rajouter un élément attribut */
    xml_ecrit_dernier_attribut(tmp_node->fils[8].fils,"text:style-name","PVD"); /* remplace le dernier attribut */

    if (info)
    {
      xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
      xml_ajoute_fin(tmp_node->fils+9,"text:p");
      xml_ajoute_fin(tmp_node->fils[9].fils,"text:span");
      xml_ajoute_fin_texte(tmp_node->fils[9].fils->fils);
      xml_ajoute_fin_attribut(tmp_node->fils+9); /* rajouter un élément attribut */
      if (node->nb_fils%2==0)
      {
          xml_ecrit_dernier_attribut(tmp_node->fils+9,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
      }
      else
      {
          xml_ecrit_dernier_attribut(tmp_node->fils+9,"table:style-name","Tableau1.VD21.n"); /* remplace le dernier attribut */
      }
      if (info>9)
      {
        xml_ecrit_dernier_texte(tmp_node->fils[9].fils->fils,"0");
      }
      else
      {
        xml_ecrit_dernier_texte(tmp_node->fils[9].fils->fils,"x");
      }
    }
}


void affiche_sort_pretre(char * pantheon,char * dieu,GMarkupDomNode * ooo,unsigned short cl,char ** source)
{
    /* couple panthéon/dieu si deux dieux même nom dans deux panthéons différents */
    signed short niveau,sort_,i,j;
    GMarkupDomNode * node;

    if (pantheon==NULL || dieu==NULL )
    { /* aucun dieu ou panthéon : anormal */
    }
    else
    {
        i=pantheon_entier(pantheon);
        if (i>=0) /* c'est le bon panthéon */
        {
          j=dieu_entier(dieu,i);
          if (j>=0 || compare_sans_casse(CLASSE[cl].nom,"Druide")==0) /* c'est le bon dieu ou n druide peut ne pas avoir de dieu */
          {
                for (niveau=1; niveau<=3; niveau++)
                {
                    node=nv_tableau_sort(ooo,niveau,CLASSE[cl].nom);

                    /* trouver les sorts de ce niveau */
                     for(sort_=0; sort_<nb_sort_clerc[ADD2][niveau-1]; sort_++)
                     {
                        if (( sortileges[ADD2][niveau-1][sort_].sphere & (PANTHEONS[i].dieu[j].mineure | PANTHEONS[i].dieu[j].majeure)) && dans_source(sortileges[ADD2][niveau-1][sort_].source,source))
                        {
                            insere_ligne_pretre(node,sortileges[ADD2][niveau-1]+sort_,0);
                            sort_nb_sort_clerc_connu[niveau-1]++;
                            liste_clerc[niveau-1]=(char **)g_realloc(liste_clerc[niveau-1],sort_nb_sort_clerc_connu[niveau-1]*sizeof(char *));
                            liste_clerc[niveau-1][sort_nb_sort_clerc_connu[niveau-1]-1]=sortileges[ADD2][niveau-1][sort_].nom;
                            liste_clerc_id[niveau-1]=(signed long *)g_realloc(liste_clerc_id[niveau-1],sort_nb_sort_clerc_connu[niveau-1]*sizeof(signed long));
                            liste_clerc_id[niveau-1][sort_nb_sort_clerc_connu[niveau-1]-1]=sortileges[ADD2][niveau-1][sort_].clef;
                        }
                        else
                        {
/*printf("%lu&(%lu | %lu) => %s\n",sortileges[ADD2][niveau-1][sort_].sphere , PANTHEONS[i].dieu[j].mineure , PANTHEONS[i].dieu[j].majeure,sortileges[ADD2][niveau-1][sort_].nom);*/
                        }
                     }
                }
                if (PANTHEONS[i].dieu[j].majeure>0) for (niveau=4; niveau<=7; niveau++)
                {
                    node=nv_tableau_sort(ooo,niveau,CLASSE[cl].nom);

                     for(sort_=0; sort_<nb_sort_clerc[ADD2][niveau-1]; sort_++)
                     {
                             if (sortileges[ADD2][niveau-1][sort_].sphere&PANTHEONS[i].dieu[j].majeure)
                             {
                                insere_ligne_pretre(node,sortileges[ADD2][niveau-1]+sort_,0);
                                sort_nb_sort_clerc_connu[niveau-1]++;
                                liste_clerc[niveau-1]=(char **)g_realloc(liste_clerc[niveau-1],sort_nb_sort_clerc_connu[niveau-1]*sizeof(char *));
                                liste_clerc[niveau-1][sort_nb_sort_clerc_connu[niveau-1]-1]=sortileges[ADD2][niveau-1][sort_].nom;
                                liste_clerc_id[niveau-1]=(signed long *)g_realloc(liste_clerc_id[niveau-1],sort_nb_sort_clerc_connu[niveau-1]*sizeof(signed long));
                                liste_clerc_id[niveau-1][sort_nb_sort_clerc_connu[niveau-1]-1]=sortileges[ADD2][niveau-1][sort_].clef;
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
            else
            {
                printf("Surprise : le dieu %s du panthéon %s n'exite pas !\n",dieu,pantheon);
            }
        }
        else
        {
            printf("Surprise : le panthéon %s n'exite pas !\n",pantheon);
        }
    }
}

unsigned short affiche_sort_mag(unsigned short niveau,FenetrePerso * _perso,GMarkupDomNode * ooo, unsigned short * pages_voy, unsigned short * pages_t)
{ /* niveau actuel, pointeur général , node fichier, nombre de page sur le livre de voyage */
    unsigned short i,j,k,pages=0;
    GMarkupDomNode * node;
    char ch[LONG];
    unsigned long ecole_OK=0,ecole_=0;


    node=nv_tableau_sort(ooo,niveau+1,"mago");
    for (k=0;_perso->perso.classe[k]>=0;k++)
    { /* parcours des classes */
        if (CLASSE[_perso->perso.classe[k]].add[ADD2].nom!=NULL)
        { /* c'est une classe ADD2 */
            ecole_OK|=CLASSE[_perso->perso.classe[k]].add[ADD2].sort_mago_ecole;
            ecole_|=CLASSE[_perso->perso.classe[k]].add[ADD2].sort_mago_ecole_spe;
        }
        else
        { /* classe ADD1 : livre sort prévu pour ADD2 */
        }
    }

    for (j=0;_perso->perso.sort_su[niveau][j]!=0;j++)
    {
            for(i=0;i<nb_sort_mag[ADD2][niveau];i++)
            {
                if (sortileges_mag[ADD2][niveau][i].clef==_perso->perso.livre_sorts[niveau][j])
                {
                    if (_perso->perso.sort_su[niveau][j]>0)
                    {
                        *pages_voy+=(niveau+1);
                    }
                    else
                    { /* pas dans le livre de voyage */
                    }
                    if (((ecole_OK&sortileges_mag[ADD2][niveau][i].sphere)==0 )|| ((entropie&sortileges_mag[ADD2][niveau][i].sphere)!=0 && (entropie&ecole_OK)==0 ))
                    { /* non mago ou sort dans une sphere non comprise + supprimer l'entropie si le magicien n'est pas entropiste */
                        if (_perso->perso.sort_su[niveau][j]>0)
                        { /* dans le livre de voyage */
                            insere_ligne_pretre(node,sortileges_mag[ADD2][niveau]+i,4);
                        }
                        else
                        {
                            insere_ligne_pretre(node,sortileges_mag[ADD2][niveau]+i,14);
                        }
                    }
                    else
                    {
                         if ((_perso->perso.sort_su[niveau][j]==1 || _perso->perso.sort_su[niveau][j]==-1) && (ecole_&sortileges_mag[ADD2][niveau][i].sphere)!=0) /* 1 => lu, 2 => Pas compris, 3 pas lu */
                         {	/* si le sort est entropique, alors le magicien est entropiste                                   ---                                                                                     ecole_ l'école est sitée 2 fois => spécialité */
                              if (_perso->perso.sort_su[niveau][j]>0)
                              {
                                  insere_ligne_pretre(node,sortileges_mag[ADD2][niveau]+i,5);
                              }
                              else
                              {
                                  insere_ligne_pretre(node,sortileges_mag[ADD2][niveau]+i,15);
                              }
                         }
                         else
                         {
                             if (_perso->perso.sort_su[niveau][j]>0)
                             {
                              insere_ligne_pretre(node,sortileges_mag[ADD2][niveau]+i,_perso->perso.sort_su[niveau][j]);
                             }
                             else
                             {
                              insere_ligne_pretre(node,sortileges_mag[ADD2][niveau]+i,10-_perso->perso.sort_su[niveau][j]);
                             }
                         }
                         if (_perso->perso.sort_su[niveau][j]==1 || _perso->perso.sort_su[niveau][j]==-1)
                         { /* page sue */
                           pages++;
                           sort_nb_sort_mag_connu[niveau]++;
                           liste_mag[niveau]=(char **)g_realloc(liste_mag[niveau],sort_nb_sort_mag_connu[niveau]*sizeof(char *));
                           liste_mag[niveau][sort_nb_sort_mag_connu[niveau]-1]=sortileges_mag[ADD2][niveau][i].nom;
                           liste_mag_id[niveau]=(signed long *)g_realloc(liste_mag_id[niveau],sort_nb_sort_mag_connu[niveau]*sizeof(signed long));
                           liste_mag_id[niveau][sort_nb_sort_mag_connu[niveau]-1]=sortileges_mag[ADD2][niveau][i].clef;
                         }
                         else
                         { /* page non sue */
                         }
                     }
                     for (;i<nb_sort_mag[ADD2][niveau];i++);  /* sortie de la boucle       */
                }                                       /* dès qu'on a trouvé le bon */
            }
    }
    node=g_markup_dom_node(ooo,"office:text");
    node=xml_ajoute_fin(node,"text:p");
    *pages_t+=j*(niveau+1);
    if (pages==0)
    {
        sprintf(ch,"Bien qu'aucun sort de niveau %hu ne soit compris, le livre contient %hu page%s de sorts de ce niveau.",niveau+1,j*(niveau+1),niveau==0?"":"s");
    }
    else
    {
        if (pages==1)
        {
            if (j==1)
            {
                 sprintf(ch,"Le livre contient 1 seul sort de niveau %hu, il est compris et occupe %hu page%s.",pages*(niveau+1),j*(niveau+1),niveau==0?"":"s");
            }
            else
            {
                if (niveau==0)
                {
                    sprintf(ch,"Le livre contient %hu pages de sorts de niveau 1 dont 1 seule contient un sort compris.",j);
                }
                else
                {
                    sprintf(ch,"Le livre contient %hu pages de sorts de niveau %hu dont %hu contiennent un sort compris.",j*(niveau+1),(niveau+1),(niveau+1));
                }

            }
        }
        else if (pages>=1)
        {
            if (pages==j)
            {
                 sprintf(ch,"Le livre contient %hu pages de sorts de niveau %hu. Toutes les pages sont comprises.",pages*(niveau+1),niveau+1);
            }
            else
            {
                 sprintf(ch,"Le livre contient %hu pages de sorts de niveau %hu. %hu pages sont comprises.",j*(niveau+1),niveau+1,pages*(niveau+1));
            }
        }
    }

    xml_ecrit_dernier_texte(node,ch);

    pages*=(niveau+1);
    return pages;
}

void init_liste_sort()
{ /* on passe pour chaque classe qui utilise les sorts de clerc */
    unsigned short i;

    for (i=0;i<7;i++)
    {
        g_free(liste_clerc[i]);
        liste_clerc[i]=NULL;
        g_free(liste_clerc_id[i]);
        liste_clerc_id[i]=NULL;
        sort_nb_sort_clerc_connu[i]=0;
    }
}

void init_liste_mag()
{ /* on ne passe qu'une fois au début de l'affichage du perso */
    unsigned short i;

    nb_forme=0;
    for (i=0;i<9;i++)
    {
        g_free(liste_mag[i]);
        g_free(liste_mag_id[i]);
        liste_mag[i]=NULL;
        liste_mag_id[i]=NULL;
        sort_nb_sort_mag_connu[i]=0;
    }
}
