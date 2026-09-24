#include "include.h"
#include <string.h>
#include <math.h>

/*signed short feuille_lire_case_d(char * coord,GMarkupDomNode * node,signed short defaut);
float        feuille_lire_case_f(char * coord,GMarkupDomNode * node,float        min);
float        feuille_lire_case_f_d(char * coord,GMarkupDomNode * node,float   defaut);
signed short * feuille_lire_ligne_nb(char * coordonnees,GMarkupDomNode * node);
signed long * feuille_lire_colonne_nbl(char * coordonnees,GMarkupDomNode * node);*/

float donne_float_A1(char * coordonnees,GMarkupDomNode * ooo)
{
    unsigned long i,j,c/*,l_nan=0xffffffff*/;
    float fnan=nanf("");
    float retour;

    /*fnan =*((float*)&l_nan);*/


    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
        i=i*26+*(coordonnees+j)-'A'+1;
    }
    if (j==0)
    {
         return fnan; /* aucune lettre devant */
    }
    else
    {
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        {
            retour=fnan;
        }
        else
        {/* passage des coordonnées humaines aux coordonnées informatiques */
            retour=donne_float_ij(j-1,i-1,ooo);
        }
    }

    return retour;
}

float donne_float_ij(unsigned long i, unsigned long j,GMarkupDomNode * ooo)
{ /* ooo pointe sur la feuille à traiter du fichier ods */
    unsigned long type=0,x,y,ii,jj/*,l_nan=0xffffffff*/; /* représentation binaire d'un NaN */
    GMarkupDomNode * node, *tmp;
    float fnan=nanf(""),sortie;

    /*fnan =*((float*)&l_nan);*/ /* astuce pour créer un float NaN */

    /* première ligne pas dans le tableau */
    /* recherche de la colonne */
    for (x=0,ii=0;x<=i && ii<ooo->nb_fils;x++,ii++)
    {
        tmp=ooo->fils+ii;
        if (strcmp(tmp->nom,"table:table-row")!=0)
        {
            i++; /* ce n'est pas une ligne, il faudra donc un fils de plus */
        }
        else
        {
            for (jj=0;jj<tmp->nb_att;jj++)
            {   /* si c'est une ligne, on cherche si elle n'est pas en fait le regroupement de plusieurs lignes */
                if (strcmp(tmp->attributs[jj].nom,"table:number-rows-repeated")==0)
                {   /* prendre en compte le fait que cette ligne se répète plusieurs fois */
                    if (sscanf(tmp->attributs[jj].value,"%lu",&y) == 1)
                    { /* nombre d regroupement */
                        x+=(y-1);
                    }
                    else
                    { /* lecture mauvaise : on ne la prend pas en compte */
                    }
                }
                else
                { /* ligne seule */
                }
            }
        }
    }
    if (x>i)
    {
        ii--;             /* attention, si une case se répète.... */
    }
    else
    { /* n'est pas trop loin */
    }
    if (ii<ooo->nb_fils)       /* la case était vide => on cherche dans une ligne inexistante */
    {
        node=ooo->fils+ii;
        for (x=0,ii=0;x<=j && ii<node->nb_fils;x++,ii++)
        {
            /* parcours des cases de la ligne pour trouver la bonne colonne */
            tmp=node->fils+ii;
            if (strcmp(tmp->nom,"table:table-cell")!=0)
            {
                j++; /* si ce n'est pas une case il faudra en chercher une de plus */
            }
            else
            {
                for (jj=0;jj<tmp->nb_att;jj++)
                {   /* c'est peut-être une case qui se répète plusieurs fois */
                    if (strcmp(tmp->attributs[jj].nom,"table:number-columns-repeated")==0)
                    {   /* si oui, il faut prendre en compte le nombre de répétition */
                        if (sscanf(tmp->attributs[jj].value,"%lu",&y) == 1)
                        {
                            x+=(y-1);
                        }
                        else
                        { /* mauvaise lecture */
                        }
                    }
                    else
                    { /* pas l'attribut recherché */
                    }
                }
            }
        }
        if (x>j) ii--;
        if (ii<node->nb_fils)
        {
            node=node->fils+ii; /* node pointe maintenant sur la case recherchée */
            ii=0;
            if (node->nb_att>0)
            {
                for(i=0;i<node->nb_att;i++)
                {
                    if (compare_sans_casse(node->attributs[i].nom,"office:value")==0) /* on a l'attribut 'valeur' recherché, le type supposé est float            */
                    {                   /* on va pouvoir lire la valeur au format float, séparateur décimal : point */
                        ii=sscanf(node->attributs[i].value,"%f",&sortie);
                        if (ii!=0)
                        { /* on récupère la sortie */
                          fnan=sortie;
                          i=node->nb_att;
                          type=3;
                        }
                        else
                        { /* mauvaise lecture */
                        } /* si on n'a rien lu, il y a eu une erreur : on renvoie nan */
                    }
                    else if (compare_sans_casse(node->attributs[i].nom,"office:value-type")==0) /* on a l'attribut 'valeur' recherché, le type supposé est float            */
                    {
                          if (compare_sans_casse(node->attributs[i].value,"string")==0)
                          {
                            type=2;
                          }
                          else if (compare_sans_casse(node->attributs[i].value,"float")==0)
                          {
                            type=1;
                          }
                          else
                          { /* cas par défaut : 0 */
                          }
                          i=node->nb_att;
                    }
                    else
                    { /* aucun des deux cas cherchés */
                    }
                }
            }
        }
        else
        {
            ii=0;
        }
        if (type==2 || type==1)
        {
            if (ii==0 && node->nb_fils>0 && node->fils->nb_texte>0) /* on n'a pas trouvé de float => on renvoie un float qui pourrait être dans la case  */
            {                                                       /* en début de chaîne de caractères, mais le format dépend alors de la configuration */
                ii=sscanf(node->fils->texte->texte,"%f",&sortie);   /* du PC qui a enregistré le document donc non universel                             */
                if (ii!=0)
                {
                    fnan=sortie;
                }
                else
                {
                    fnan =nanf(""); /* *((float*)&l_nan);*/ /* astuce pour créer un float NaN */
                }
            }
            else
            {
                fnan =nanf(""); /* *((float*)&l_nan);*/ /* astuce pour créer un float NaN */
            }
        }
        else if (type==0)
        {
             fnan =nanf(""); /* *((float*)&l_nan); */ /* astuce pour créer un float NaN */
        }/* le cas non traité est le cas float pris par défaut avant */
        else
        { /* type=3 par exemple : on ne fait rien de plus   */
        }
    }

    return fnan;
}

signed short feuille_lire_case_m(char * coord,GMarkupDomNode * node,signed short min)
{
    return (signed short)(feuille_lire_case_f(coord,node,min)); /* calc ne connaît pas le type entier */
}                                                               /* mais uniquement le type float      */

signed short feuille_lire_case_M(char * coord,GMarkupDomNode * node,signed short max)
{
    float retour;
    signed short sortie;


    retour=feuille_lire_case_f_d(coord,node,max); /* calc ne connaît pas le type entier */
    if (retour>max || isnan(retour) /*retour!=retour*/) /* est-ce un NaN ? */
    {
        sortie=max;
    }         /* mais uniquement le type float      */
    else
    {
        sortie=(signed short)retour;
    }

    return sortie;
}

signed short feuille_lire_case_d(char * coord,GMarkupDomNode * node,signed short defaut)
{
    return (signed short)(feuille_lire_case_f_d(coord,node,defaut)); /* calc ne connaît pas le type entier */
}                                                                  /* mais uniquement le type float      */


float feuille_lire_case_f(char * coord,GMarkupDomNode * node,float min)
{
    float reel=0;

    reel=donne_float_A1(coord,node);

    if (reel<min || isnan(reel)/*reel!=reel*/) /* compare à NaN */
    {
         reel=min;
    }
    else
    { /* on garde la valeur lue */
    }

    return reel;
}

float feuille_lire_case_f_d(char * coord,GMarkupDomNode * node,float defaut)
{
    float reel=0;

    reel=donne_float_A1(coord,node);

    if (isnan(reel)/*reel!=reel*/) /* compare à NaN */
    {
         reel=defaut;
    }
    else
    { /* on garde la valeur lue */
    }

    return reel;
}

char * lecture_case_up(char * coordonnees,GMarkupDomNode * ooo) /* lecture case avec réservation de la mémoire */
{
    GMarkupDomNode * info;
    char * retour;

    info=donne_case_A1(coordonnees,ooo);
    if (info==NULL)
    {
        retour = NULL;
    }
    else if (info->nb_texte==0)
    {
        retour= NULL; /* pas de texte */
    }
    else
    {
        retour=g_strdup(info->texte->texte);
    }

    return retour;
}

char * lecture_case_up_ij(unsigned long i, unsigned long j,GMarkupDomNode * ooo) /* lecture case avec réservation de la mémoire */
{
    GMarkupDomNode * info;
    char * retour;

    info=donne_case_ij(i,j,ooo);
    if (info==NULL)
    {
        retour = NULL;
    }
    else if (info->nb_texte==0)
    {
        retour= NULL; /* pas de texte */
    }
    else
    {
        retour=g_strdup(info->texte->texte);
    }

    return retour;
}

char * lecture_case(char * coordonnees,GMarkupDomNode * ooo)
{
    GMarkupDomNode * info;
    char * retour;

    info=donne_case_A1(coordonnees,ooo);
    if (info==NULL)
    {
        retour=NULL;
    }
    else if (info->nb_texte==0)
    {
        retour=NULL; /* pas de texte */
    }
    else
    {
        retour=info->texte->texte;
    }

    return retour;

}

GMarkupDomNode * donne_case_A1(char * coordonnees,GMarkupDomNode * ooo)
{
    unsigned long i,j,c;
    GMarkupDomNode * retour;

    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
         i=i*26+*(coordonnees+j)-'A'+1; /* base 26 pour les coordonnées à 2 lettres */
    }
    if (j==0)
    {
        retour= NULL; /* aucune lettre devant */
    }
    else
    {
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        {
            retour= NULL;
        }
        else     /* passage des coordonnées humaines aux coordonnées informatiques */
        {
            retour = donne_case_ij(j-1,i-1,ooo); /* (1,1) => (0,0)              */
        }
    }

    return retour;
}

GMarkupDomNode * donne_case_ij(unsigned long i, unsigned long j,GMarkupDomNode * ooo)
{
    unsigned long x,y,ii,jj;
    GMarkupDomNode * node, *tmp, * retour;

    if (ooo==NULL)
    {
        retour=NULL;
    }
    else
    {
        /* première ligne pas dans le tableau */
        /* recherche de la colonne */
        for (x=0,ii=0;x<=i && ii<ooo->nb_fils;x++,ii++)
        {
            tmp=ooo->fils+ii;
            if (strcmp(tmp->nom,"table:table-row")!=0)
             i++;
            for (jj=0;jj<tmp->nb_att;jj++)
            {
                if (strcmp(tmp->attributs[jj].nom,"table:number-rows-repeated")==0)
                {
                    if (sscanf(tmp->attributs[jj].value,"%lu",&y) == 1) x+=(y-1);
                }
            }
        }
        if (x>i) ii--;              /* attention, si une case se répète.... */
        if (ii>=ooo->nb_fils)
        {
            retour= NULL; /* la case était vide => on est trop loin ! */
        }
        else
        {
            node=ooo->fils+ii;
            for (x=0,ii=0;x<=j && ii<node->nb_fils;x++,ii++)
            {
                tmp=node->fils+ii;
                if (strcmp(tmp->nom,"table:table-cell")!=0)
                 j++;
                for (jj=0;jj<tmp->nb_att;jj++)
                {
                    if (strcmp(tmp->attributs[jj].nom,"table:number-columns-repeated")==0)
                    {
                        if (sscanf(tmp->attributs[jj].value,"%lu",&y) == 1) x+=(y-1);
                    }
                }
            }
            if (x>j) ii--;
            if (ii>=node->nb_fils) retour = NULL; /* la case était vide => on est trop loin ! */
            else
            {
                node=node->fils+ii;
                if (node->nb_fils==0) retour = NULL; /* case vide */
                else                  retour = node->fils; /* donne texte dans la case */
            }
        }
    }
    return retour;
}

signed long * feuille_lire_colonne_nbl(char * coordonnees,GMarkupDomNode * node)
{
    signed short i,j,c;
    GMarkupDomNode * info;
    signed long * sortie=NULL;

    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
        i=i*26+*(coordonnees+j)-'A'+1;
    }
    if (j==0)
    {
        sortie=NULL; /* aucune lettre devant */
    }
    else
    {
        c=sscanf(coordonnees+j,"%hd",&j);
        if (c!=1) /* AUCUN CHIFFRE APRÈS */
        {
            sortie=NULL;
        }
        else
        {/* passage des coordonnées humaines aux coordonnées informatiques */

            for(c=0;(info=donne_case_ij(j-1+c,i-1,node)) && info->nb_texte!=0;c++)
            {   /* tant qu'il y a des cases, on lit le contenu */
                sortie=(signed long *)g_realloc(sortie,(c+1)*sizeof(signed long));
                sortie[c]=0;
                sscanf(info->texte->texte,"%ld",sortie+c);
            }
            sortie=(signed long *)g_realloc(sortie,(c+1)*sizeof(signed long));
            sortie[c]=FIN;
        }
    }

    return sortie;
}

void feuille_lire_colonne_char_pourcentage(char * coordonnees,GMarkupDomNode * node,signed short * tab,unsigned short longueur,signed char defaut)
{
    unsigned short i,j,ca,nb;
    char c;
    GMarkupDomNode * info;

    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
        i=i*26+*(coordonnees+j)-'A'+1;
    }
    if (j==0)
    {
        /* aucune lettre devant => sortie directe */
    }
    else
    {
        ca=sscanf(coordonnees+j,"%hd",&j);
        if (ca!=1) /* AUCUN CHIFFRE APRÈS */
        {
            /* sortie directe */
        }
        else
        {/* passage des coordonnées humaines aux coordonnées informatiques */

            {   /* tant qu'il y a des cases, on lit le contenu */
                if ((info=donne_case_ij(j-1,i-1,node)) && info->nb_texte!=0)
                {
                    nb =sscanf(info->texte->texte,"%hd%c%hd",&ca,&c,tab+POURCENTAGE); /* on passe par cet intermédiaire car les %hd ont des problèmes avec les char */
                    if (nb<3 || (c!='-' && c!='/')) /* normalement, c fait - ou / */
                    {
                        tab[POURCENTAGE]=0;
                    }
                    else if (tab[POURCENTAGE]==0)
                    {   /* l'écriture 18/00 signifie 18 100%*/
                        tab[POURCENTAGE]=100;
                    }

                }
                else
                {
                    tab[POURCENTAGE]=0;
                }
            }
        }
    }

    feuille_lire_colonne_char(coordonnees,node,tab,longueur,defaut);
}



void feuille_lire_colonne_char(char * coordonnees,GMarkupDomNode * node,signed short * tab,unsigned short longueur,signed char defaut)
{
    unsigned short i,j,c;
    GMarkupDomNode * info;

    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
        i=i*26+*(coordonnees+j)-'A'+1;
    }
    if (j==0)
    {
        /* aucune lettre devant => sortie directe */
    }
    else
    {
        c=sscanf(coordonnees+j,"%hd",&j);
        if (c!=1) /* AUCUN CHIFFRE APRÈS */
        {
            /* sortie directe */
        }
        else
        {/* passage des coordonnées humaines aux coordonnées informatiques */

            for(c=0;c<longueur;c++)
            {   /* tant qu'il y a des cases, on lit le contenu */
                if ((info=donne_case_ij(j-1+c,i-1,node)) && info->nb_texte!=0)
                {
                    sscanf(info->texte->texte,"%hd",tab+c); /* on passe par cet intermédiaire car les %hd ont des problèmes avec les char */
                }
                else
                {
                    tab[c]=defaut;
                }
            }
        }
    }
}

signed short * feuille_lire_ligne_nb(char * coordonnees,GMarkupDomNode * node)
{
    signed long i,j,c;
    GMarkupDomNode * info;
    signed short * sortie=NULL;

    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
         i=i*26+*(coordonnees+j)-'A'+1;
    }
    if (j==0)
    {
        sortie=NULL; /* aucune lettre devant */
    }
    else
    {
        c=sscanf(coordonnees+j,"%ld",&j);
        if (c!=1)
        {
            sortie=NULL;
        }
        else
        {
            /* passage des coordonnées humaines aux coordonnées informatiques */

            for(c=0;(info=donne_case_ij(j-1,i-1+c,node)) && info->nb_texte!=0;c++)
            {   /* tant qu'il y a des cases, on lit le contenu */
                sortie=(signed short *)g_realloc(sortie,(c+2)*sizeof(signed short));
                sortie[c]=0;
                sscanf(info->texte->texte,"%hu",sortie+c);
            }
            if (c==0)
            {  /* rien d'écrit encore */
                sortie=(signed short *)malloc(sizeof(signed short));
                *sortie=FIN;
            }
            else
            { /* place déjà réservée */
                sortie[c]=FIN;
            }
        }
    }

    return sortie;
}
