#include "include.h"
#include <dirent.h>
#include <string.h>
#include <math.h>


/* place des éléments sur les feuilles */
#define FEUILLE_NOM          "A1"

#define FEUILLE_ORIGINE      "C1"
#define FEUILLE_MIN_CARACT_H "C3"
#define FEUILLE_MIN_CARACT_F "D3"
#define FEUILLE_MAX_CARACT_H "F3"
#define FEUILLE_MAX_CARACT_F "G3"
#define FEUILLE_MOD_CARACT   "I3"
#define FEUILLE_AC           "B16"
#define FEUILLE_VOL_TABLE    "D53"

/* race */
#define FEUILLE_PDV_RACE    "L7"
#define FEUILLE_LIM_NIV     "B10"
#define FEUILLE_CLASSE      "B11"
#define FEUILLE_AGE         "B12"
#define FEUILLE_MULTICLASSE "B13"
#define FEUILLE_TRANCH_AGE1 "B14"
#define FEUILLE_TRANCH_AGE2 "D14"
#define FEUILLE_CA_RACE     "D16"
#define FEUILLE_VISION      "B17"
#define FEUILLE_ECOUTE      "B18"
#define FEUILLE_RESISTANCE  "B19"
#define FEUILLE_BONUS_ARME  "B21"
#define FEUILLE_SAVE_RACE   "B25"

#define FEUILLE_LANGUE      "B27"
#define FEUILLE_DETECTION   "B29"
#define FEUILLE_SURPRENDRE  "B31"
#define FEUILLE_SE_FAIRE_SU "B33"
#define FEUILLE_RACE_COMP   "B35"
#define FEUILLE_RACE_XP     "B37"
#define FEUILLE_H_TAILLE    "C39"
#define FEUILLE_H_TAILLE_V  "D39"
#define FEUILLE_H_POIDS     "E39"
#define FEUILLE_F_TAILLE    "C40"
#define FEUILLE_F_TAILLE_V  "D40"
#define FEUILLE_F_POIDS     "E40"

/* classe */
#define FEUILLE_PDV         "B11"
#define FEUILLE_ARME        "B13"
#define FEUILLE_MAIT_ARME_D "B15"
#define FEUILLE_MAIT_ARM_PE "C15"
#define FEUILLE_MAIT_ARM_PR "D15"
#define FEUILLE_ARMURES     "B17"
#define FEUILLE_BOUCLIER    "B18"
#define FEUILLE_SPECIAL     "B20"
#define FEUILLE_VOLEUR      "B22"
#define FEUILLE_BACK_STAB   "B23"
#define FEUILLE_COMP_C      "B24"
#define FEUILLE_COMP_CP     "B25"
#define FEUILLE_ALIGNEMENT  "B27"
#define FEUILLE_XP          "B29"
#define FEUILLE_SAVE        "E29"
#define FEUILLE_TACO_D      "E31"
#define FEUILLE_TACO_A      "F31"
#define FEUILLE_TACO        "E32"
#define FEUILLE_SORT_M      "E33"
#define FEUILLE_SORT_M_S    "E34"
#define FEUILLE_SORT_C      "E35"
#define FEUILLE_SORT_C_B    "F35"
#define FEUILLE_SORT_C_S    "E36"
#define FEUILLE_SORT_D_B    "F36"
#define FEUILLE_COMP_C1     "E37"
#define FEUILLE_COMP_I      "E38"
#define FEUILLE_COMP_N      "G38"
#define FEUILLE_PSI_CLASSE  "E39"
#define FEUILLE_VADE_RETRO  "N2"
#define FEUILLE_MAX_B_CONST "N7"
#define FEUILLE_B_INIT      "H15"
#define FEUILLE_B_INIT_NIV  "J15"
#define FEUILLE_DIEU        "E40"
#define FEUILLE_PANTHEON    "G40"


#define FEUILLE_LAR_VOL     17

/* save */
#define FEUILLE_DEPART_SAVE "A2"

/* psi */
#define FEUILLE_PSI_CARACT  "B3"
#define FEUILLE_PSI_DB_BASE "D29"
#define FEUILLE_PSI_DB_BONU "D30"
#define FEUILLE_PSI_NV_BASE "D32"
#define FEUILLE_PSI_NV_BONU "D33"
#define FEUILLE_PSI_TALENTS "B38"

void retourne_classe(struct_classe * classe_, char * fichier);
void retourne_classe_version(struct cl_add * classe_add,GMarkupDomNode * node,unsigned short version);
void feuille_pdv(char ** pdv,GMarkupDomNode * node);
unsigned short feuille_lire_alignement(GMarkupDomNode * node);
void feuille_lire_competences(GMarkupDomNode * node, unsigned long * l_c);
struct sv_add * lecture_case_save(char * coordonnees,GMarkupDomNode * ooo,unsigned short version_);
so_add * lecture_case_sort(char * coordonnees,GMarkupDomNode * ooo,unsigned short version_);
struct psi_add2 * lecture_case_psi2(char * coordonnees,GMarkupDomNode * ooo);
struct psi_add1 * lecture_case_psi1(char * coordonnees,GMarkupDomNode * ooo);
unsigned long ecole_id(char ** liste_ecole,unsigned long * spe);
signed short ** feuille_lire_voleur(char * coordonnees,GMarkupDomNode * node);
void retourne_sort(struct_sort * sort_, char * fichier);
void retourne_save(struct_save * save_, char * fichier);
struct_armes * lire_struct_armes(char * fichier);
struct_armure * lire_struct_armures(char * fichier);
struct_origine ** lire_struct_origine(char * fichier);
unsigned short groupe_armes(struct_armes * t_armes,unsigned short pt_gr,char * fichier);
void retourne_psi(struct_psi_c * psi_, char * fichier);
signed short * lire_caract(char ** ch);
void lecture_race(struct ra_add * race_v,GMarkupDomNode * node,unsigned short version);
void retourne_race(struct_race * race_, char * fichier);
liste_classe_race * feuille_lire_classerace(char * coordonnees,GMarkupDomNode * node);
struct bonus_armes * feuille_lire_race_b_arme(char * coordonnees,GMarkupDomNode * node);
void feuille_lire_race_save(char * coordonnees,char ** sortie,GMarkupDomNode * node);
signed short feuille_lire_origine(GMarkupDomNode * node,struct_origine * ori);
liste_classe_race * range_l_cl_r(liste_classe_race * depart, liste_classe_race * nv);
liste_classe * range_l_cl(liste_classe * depart, liste_classe * nv);
signed char compare_liste_classe(liste_classe * depart, liste_classe * nv);
struct_competences * lire_struct_competences(char * chemin,char *** competencesl1,char *** competencesl2);
signed char feuille_lire_multiclasse(char * coordonnees,GMarkupDomNode * node);
void feuille_lire_modif_CA(signed short * modif_CA,char * coordonnees,GMarkupDomNode * node);
char ** feuille_lire_ligne(char * coordonnees,GMarkupDomNode * node);
signed short * feuille_lire_ligne_arme(char * coordonnees,GMarkupDomNode * node);
struct_niv_classe * lecture_lim_racce_add2(GMarkupDomNode * node);
struct_niv_classe * lecture_lim_racce_add1(GMarkupDomNode * node);

/* static pour protéger encore plus : même un "externe struct_classe * classe" ne peut pas lui donner un accès en écriture dans un autre fichier */
static struct_classe * classe=NULL;
static unsigned short nb_classe=0;
static struct_psi_c * psi=NULL;
static unsigned short nb_psi=0;
static struct_save * save=NULL;
static unsigned short nb_save=0;
static struct_sort * sort=NULL;
static unsigned short nb_sort=0;
static struct_armure * armures=NULL;
static unsigned short nb_armures=0;
static struct_armes * armes=NULL;
static unsigned short nb_armes=0;
static struct_race * race=NULL;
static unsigned short nb_race=0;
static struct_competences * competences=NULL;
static unsigned short nb_competences=0;
static unsigned long masque_compentence_primaire=0,masque_compentence_secondaire=0;
static unsigned short nb_groupe_competences_primaires;
static char ** liste_competences_primaires=NULL, ** liste_competences_secondaires=NULL;
static struct_origine ** origine=NULL;
static unsigned short nb_origine_[2];


/* fonctions "getters" pour cacher aux autres fichiers que ce sont des constantes */
/* Elles retournent les tableaux demandés */
unsigned short * nb_origine()
{
    return nb_origine_;
}

struct_origine ** donne_origine()
{
    return origine;
}

struct_competences * donne_competences()
{
    return competences;
}

unsigned short donne_nb_competence()
{
    return nb_competences;
}

unsigned short donne_nb_competence_primaire()
{
    return nb_groupe_competences_primaires;
}

unsigned short donne_masque_compentence_primaire()
{
    return masque_compentence_primaire;
}

unsigned short donne_masque_compentence_secondaire()
{
    return masque_compentence_secondaire;
}

struct_race * donne_race()
{
    return race;
}

unsigned short donne_nb_race()
{
    return nb_race;
}

struct_armes * donne_armes()
{
    return armes;
}

unsigned short donne_nb_arme()
{
    return nb_armes;
}

struct_armure * donne_armure()
{
    return armures;
}

unsigned short donne_nb_armure()
{
    return nb_armures;
}

unsigned short donne_nb_sort()
{
    return 0;
}

struct_sort * donne_sort()
{
    return sort;
}

struct_save * donne_save()
{
    return save;
}

unsigned short donne_nb_save()
{
    return nb_save;
}

unsigned short donne_nb_psi()
{
    return nb_psi;
}

struct_psi_c * donne_psi()
{
    return psi;
}

struct_classe * donne_classe()
{
    return classe;
}

unsigned short donne_nb_classe()
{
    return nb_classe;
}


void lecture_fichiers(char * ici)
{
 gchar * chemin=NULL, * tmp_ch=NULL;
 DIR * rep=NULL;
 struct dirent * ent=NULL;
 struct_classe tmp_classe;
 struct_race tmp_race;
 signed short drap;
 unsigned short j,k;

 chemin=NULL;
 lire_sorts(ici); /* on récupère les sorts de magicien et de prêtre */

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","origines.ods",NULL);
 origine=lire_struct_origine(chemin);
 if (origine==NULL)
 {
    printf("Surprise : le fichier %s a un problème\n",chemin);
 }
 else
 {
 }
 g_free(chemin);
 chemin=NULL;

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","sorts","tableau_niveau",NULL);
 rep = opendir(chemin);
 if (rep != NULL)
 {
    g_free(chemin);
    while ((ent = readdir(rep)) != NULL)
    { /* http://c.developpez.com/faq/?page=fichiers#FICHIERS_dir_list */
         if (g_str_has_suffix((gchar *)ent->d_name,".ods"))
         {    /* les listes de sorts de différentes classes */
            nb_sort++;
            sort=(struct_sort *)g_realloc(sort,nb_sort*sizeof(struct_sort));
            chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","sorts","tableau_niveau",(char *)ent->d_name,NULL);
            sort[nb_sort-1].type=NULL;
            sort[nb_sort-1].add[ADD].nb=NULL;
            sort[nb_sort-1].add[ADD2].nb=NULL;
            sort[nb_sort-1].add[ADD].nom=NULL;
            sort[nb_sort-1].add[ADD2].nom=NULL;
            retourne_sort (sort+nb_sort-1,chemin);
            g_free(chemin);
        }
        else
        {
        }
    }
    closedir(rep);
    rep=NULL;
 }
 else
 {
     printf("Surprise : le répertoire %s n'existe pas\n",chemin);
     g_free(chemin);
 }
 chemin=NULL;

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","dieux.add",NULL);

 if (genere_pantheon(chemin)==NULL) /* on récupère les panthéons du fichier des dieux */
 {
     printf("Surprise : le fichier %s a un problème\n",chemin);
 }
 else
 {
 }
 g_free(chemin);


 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","saves",NULL);
 rep = opendir(chemin);
 if (rep != NULL)
 {
    g_free(chemin);
    while ((ent = readdir(rep)) != NULL)
    { /* http://c.developpez.com/faq/?page=fichiers#FICHIERS_dir_list */
        if (g_str_has_suffix((gchar *)ent->d_name,".ods"))
        {
            nb_save++;
            save=(struct_save *)g_realloc(save,nb_save*sizeof(struct_save));
            chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","saves",(char *)ent->d_name,NULL);
            save[nb_save-1].add[ADD].save=NULL;
            save[nb_save-1].add[ADD2].save=NULL;
            save[nb_save-1].add[ADD].nom=NULL;
            save[nb_save-1].add[ADD2].nom=NULL;
            retourne_save (save+nb_save-1,chemin);
            g_free(chemin);
        }
        else
        {
        }
    }
    closedir(rep);
    rep=NULL;
 }
 else
 {
     printf("Surprise : le répertoire %s n'existe pas\n",chemin);
     g_free(chemin);
 }
 chemin=NULL;


 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","armes","armes.ods",NULL);
 armes=lire_struct_armes(chemin);
 if (armes==NULL)
 {
    printf("Surprise : le fichier %s a un problème\n",chemin);
 }
 else
 {
 }
 g_free(chemin);
 chemin=NULL;

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","armes","petit_groupe",NULL);
 rep = opendir(chemin);
 if (rep != NULL)
 {
    g_free(chemin);
    chemin=NULL;
    while ((ent = readdir(rep)) != NULL)
    {
        if (g_str_has_suffix((gchar *)ent->d_name,".ods"))
        {
                chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","armes","petit_groupe",(char *)ent->d_name,NULL);
                groupe_armes(armes,2,chemin);
                g_free(chemin);
                chemin=NULL;
        }
        else
        {
            /* ce n'est pas un fichier ods à lire */
        }
    }
    closedir(rep);
    rep=NULL;
 }
 else
 {
     printf("Surprise : le répertoire %s n'existe pas\n",chemin);
     g_free(chemin);
     chemin=NULL;
 }

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","armes","grand_groupe",NULL);
 rep = opendir(chemin);
 if (rep != NULL)
 {
    g_free(chemin);
    chemin=NULL;
    while ((ent = readdir(rep)) != NULL)
    {
        if (g_str_has_suffix((gchar *)ent->d_name,".ods"))
        {
                chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","armes","grand_groupe",(char *)ent->d_name,NULL);
                groupe_armes(armes,3,chemin);
                g_free(chemin);
                chemin=NULL;
        }
        else
        {
            /* ce n'est pas un fichier ods à lire */
        }
    }
    closedir(rep);
    rep=NULL;
 }
 else
 {
     printf("Surprise : le répertoire %s n'existe pas\n",chemin);
     g_free(chemin);
     chemin=NULL;
 }

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","armes","armures.ods",NULL);
 armures=lire_struct_armures(chemin);
 if (armures==NULL)
 {
    printf("Surprise : le fichier %s a un problème\n",chemin);
 }
 else
 {
 }
 g_free(chemin);
 chemin=NULL;


 /* progression des classes dans les psi */
 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","psi","tableau_niveau",NULL);
 rep = opendir(chemin);
 if (rep != NULL)
 {
    g_free(chemin);
    chemin=NULL;
    while ((ent = readdir(rep)) != NULL)
    {
        if (g_str_has_suffix((gchar *)ent->d_name,".ods"))
        {
            nb_psi++;
            chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","psi","tableau_niveau",(char *)ent->d_name,NULL);
            psi=(struct_psi_c *)g_realloc(psi,nb_psi*sizeof(struct_psi_c));
            psi[nb_psi-1].add1.nom=NULL;
            psi[nb_psi-1].add2.nom=NULL;
            retourne_psi (psi+nb_psi-1,chemin);
            g_free(chemin);
            chemin=NULL;
        }
    }
    closedir(rep);
    rep=NULL;
 }
 else
 {
     printf("Surprise : le répertoire %s n'existe pas\n",chemin);
     g_free(chemin);
     chemin=NULL;
 }

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","competences.ods",NULL);
 competences=lire_struct_competences(chemin,&liste_competences_primaires,&liste_competences_secondaires);
 if (competences==NULL)
 {
    printf("Surprise : le fichier %s a un problème\n",chemin);
 }
 else
 {
 }
 g_free(chemin);
 chemin=NULL;

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","classes",NULL);
 rep = opendir(chemin);
 if (rep != NULL)
 {
    g_free(chemin);
    chemin=NULL;
    while ((ent = readdir(rep)) != NULL)
    { /* http://c.developpez.com/faq/?page=fichiers#FICHIERS_dir_list */
        if (g_str_has_suffix((gchar *)ent->d_name,".ods"))
        {
            classe=(struct_classe *)g_realloc(classe,(nb_classe+1)*sizeof(struct_classe));

            tmp_classe.add[ADD].nom=NULL;
            tmp_classe.add[ADD2].nom=NULL;
            tmp_classe.add[ADD2].liste_competences=0;
            tmp_classe.nom=NULL;

            chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","classes",(char *)ent->d_name,NULL);
            retourne_classe (&tmp_classe,chemin);
            g_free(chemin);
            chemin=NULL;

            if (tmp_classe.nom!=NULL)
            {
                j=0;
                drap=1;
                while(j<nb_classe && drap) /* mettre la classe à sa place dans l'ordre alphabétique */
                {
                    if (compare_sans_casse(tmp_classe.nom,classe[j].nom)<0)
                    {
                        drap=0;
                    }
                    else
                    {
                        j++;
                    }
                }
                for(k=nb_classe;k>j;k--)
                {
                    classe[k]=classe[k-1];
                }
                classe[j]=tmp_classe;
                nb_classe++;
            }
            else
            { /* pas de nom à la classe : fichier classe non reconnu */
                printf("Fichier classe %s non reconnu\n",ent->d_name);
            }
        }
        else
        { /* pas un fichier ods */
        }
    }
    closedir(rep);
    rep=NULL;
 }
 else
 {
     printf("Surprise : le répertoire %s n'existe pas\n",chemin);
     g_free(chemin);
     chemin=NULL;
 }

 chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","races",NULL);
 rep = opendir(chemin);
 if (rep != NULL)
 {
    g_free(chemin);
    chemin=NULL;
    while ((ent = readdir(rep)) != NULL)
    { /* http://c.developpez.com/faq/?page=fichiers#FICHIERS_dir_list */
        if (g_str_has_suffix((gchar *)ent->d_name,".ods"))
        {
            race=(struct_race *)g_realloc(race,(nb_race+1)*sizeof(struct_race));
            tmp_race.add[ADD].nom=NULL;
            tmp_race.add[ADD2].nom=NULL;
            chemin=g_build_path (G_DIR_SEPARATOR_S,ici,"fichiers","races",(char *)ent->d_name,NULL);
            retourne_race (&tmp_race,chemin);
            if (tmp_race.add[ADD].nom==NULL)
            {
                 tmp_ch=tmp_race.add[ADD2].nom;
            }
            else
            {   /* on privilégie le nom ADD1 de la race */
                tmp_ch=tmp_race.add[ADD].nom;
            }
            if (tmp_ch!=NULL)
            {
                j=0;
                drap=1;
                while(j<nb_race && drap)
                {
                    if (race[j].add[ADD].nom!=NULL && strcmp(tmp_ch,race[j].add[ADD].nom)<0)
                    {
                        drap=0;
                    }
                    else if (race[j].add[ADD2].nom!=NULL && strcmp(tmp_ch,race[j].add[ADD2].nom)<0)
                    {
                        drap=0;
                    }
                    else
                    {
                        j++;
                    }
                }
                for(k=nb_race;k>j;k--)
                {
                    race[k]=race[k-1];
                }
                race[j]=tmp_race;
                nb_race++;
                tmp_ch=NULL;
            }
            else
            {
                /* pas ne nom de race : anormal */
                printf("le fichier de race %s n'a pas de nom de race\n",chemin);
            }
            g_free(chemin);
            chemin=NULL;
        }
        else
        {
        }
    }
    closedir(rep);
    rep=NULL;
 }
 else
 {
     printf("Surprise : le répertoire %s n'existe pas\n",chemin);
     g_free(chemin);
     chemin=NULL;
 }
}

void retourne_classe(struct_classe * classe_, char * fichier)
{
    GMarkupDomNode * node=NULL,* ooo=NULL;
    char * tmp;

    if ((ooo = g_markup_dom_new (fichier, NULL)))
    {
        if ((node=g_markup_dom_nom (ooo,"ADD1","table:name")))
        { /* sinon, la version ADD1 de la classe_ n'existe pas */
            if ((tmp=lecture_case(FEUILLE_NOM,node)))
            {
                classe_->add[ADD].nom=g_strdup(tmp);
                classe_->nom=classe_->add[ADD].nom;
                retourne_classe_version(&(classe_->add[ADD]),node,ADD1);
            }
            else
            {
                classe_->add[ADD].nom=NULL;
            }
        }
        else
        {
            classe_->add[ADD].nom=NULL;
        }
        if ((node=g_markup_dom_nom (ooo,"ADD2","table:name")))
        {
            if ((tmp=lecture_case(FEUILLE_NOM,node)))
            {
                if (classe_->add[ADD].nom==NULL)
                {
                        classe_->add[ADD2].nom=g_strdup(tmp);
                        classe_->nom=classe_->add[ADD2].nom;
                }
                else
                {
                    classe_->add[ADD2].nom=classe_->add[ADD].nom;
                }
                retourne_classe_version(&(classe_->add[ADD2]),node,ADD2);
            }
            else classe_->add[ADD2].nom=NULL;
        }
        else
        {
            classe_->add[ADD2].nom=NULL;
        }
        g_markup_dom_free(ooo);
    }
    else
    {
        classe_->add[ADD].nom=NULL;
        classe_->add[ADD2].nom=NULL;
        classe_->nom=NULL;
        printf("le fichier de classe %s n'est pas bon\n",fichier);
    }
}

void retourne_classe_version(struct cl_add * classe_add,GMarkupDomNode * node,unsigned short version)
{
    char * tmp1;
    signed short i,j;

    feuille_lire_colonne_char_pourcentage(FEUILLE_MIN_CARACT_H,node,classe_add->min_caract,6,0);
    classe_add->min_caract[BEAUTE]=3;
    classe_add->min_caract[HUMOUR]=3;
    feuille_lire_colonne_char_pourcentage(FEUILLE_MAX_CARACT_H,node,classe_add->max_caract,6,25);
    classe_add->max_caract[BEAUTE]=18;
    classe_add->max_caract[HUMOUR]=18;
    feuille_lire_colonne_char_pourcentage(FEUILLE_MOD_CARACT  ,node,classe_add->dix_caract,6,0);

    feuille_pdv(classe_add->pdv,node);
    classe_add->armes=feuille_lire_ligne_arme(FEUILLE_ARME,node);

    classe_add->nb_arme[0]=feuille_lire_case_m(FEUILLE_MAIT_ARME_D,node,0);
    classe_add->nb_arme[1]=feuille_lire_case_m(FEUILLE_MAIT_ARM_PE,node,-10);
    classe_add->nb_arme[2]=feuille_lire_case_m(FEUILLE_MAIT_ARM_PR,node,0);

    classe_add->ac=feuille_lire_ligne_nb(FEUILLE_AC,node);
    classe_add->armures=feuille_lire_ligne(FEUILLE_ARMURES,node);
    classe_add->bouclier=feuille_lire_ligne(FEUILLE_BOUCLIER,node);
    classe_add->special=feuille_lire_ligne(FEUILLE_SPECIAL,node);
    classe_add->alignement=feuille_lire_alignement(node);
    classe_add->origine=feuille_lire_origine(node,origine[version]);
    switch(classe_add->origine)
    {
    case -1 :
        printf(" Classe %s origine non identifiée\n",classe_add->nom);
        break;
    case -2 :
        printf(" Classe %s origine absente\n",classe_add->nom);
        break;
    case -3 : /* ce serait une erreur dans le code source */
        printf(" Coordonnée %s mauvaises\n",FEUILLE_ORIGINE);
        break;
    default : /* pas de problème : origine bien trouvée */
        break;
    }

    if (version==ADD1)
    {
        classe_add->tacO._1[0]=feuille_lire_ligne_nb(FEUILLE_TACO_D,node);
        classe_add->tacO._1[1]=feuille_lire_ligne_nb(FEUILLE_TACO  ,node);
    }
    else
    {
        classe_add->tacO._2[0]=feuille_lire_case_m(FEUILLE_TACO_D,node,20);
        if ((tmp1=lecture_case(FEUILLE_TACO_A,node)))
        {
            if (sscanf(tmp1,"%hu/%hu",classe_add->tacO._2+1,classe_add->tacO._2+2)!=2)
            {
                printf("Erreur lecture TacO de %s : %s => %hu/%hu\n", classe_add->nom,tmp1,classe_add->tacO._2[1],classe_add->tacO._2[2]);
                classe_add->tacO._2[1]=0;
                classe_add->tacO._2[2]=1;
            }
        }
        else /* en cas de problème de lecture : le tacO ne progresse pas */
        {
            classe_add->tacO._2[1]=0;
            classe_add->tacO._2[2]=1;
            printf("Erreur lecture TacO de %s\n", classe_add->nom);
        }
    }

    classe_add->save=lecture_case_save(FEUILLE_SAVE,node,version);
    classe_add->sort_mago=lecture_case_sort(FEUILLE_SORT_M,node,version);
    classe_add->sort_clerc=lecture_case_sort(FEUILLE_SORT_C,node,version);
    classe_add->vade_retro=feuille_lire_case_m(FEUILLE_VADE_RETRO,node,NON);
    if (lecture_case(FEUILLE_SORT_C_B,node)!=NULL) classe_add->bonus_clerc=*(lecture_case(FEUILLE_SORT_C_B,node));
    else                                           classe_add->bonus_clerc=0;
    if (version==ADD1)
    {
        classe_add->sort_illus=lecture_case_sort(FEUILLE_SORT_M_S,node,ADD1);
        classe_add->sort_druide=lecture_case_sort(FEUILLE_SORT_C_S,node,ADD1);
        if (lecture_case(FEUILLE_SORT_D_B,node)!=NULL)
            classe_add->bonus_druide=*(lecture_case(FEUILLE_SORT_D_B,node));
        else
            classe_add->bonus_druide=0;
        classe_add->psi._1=lecture_case_psi1(FEUILLE_PSI_CLASSE,node);
    }
    else
    {
        classe_add->sort_mago_ecole=ecole_id(feuille_lire_ligne(FEUILLE_SORT_M_S,node),&(classe_add->sort_mago_ecole_spe));
        classe_add->sort_clerc_sphere=feuille_lire_ligne(FEUILLE_SORT_C_S,node);
        classe_add->compt_spe[0]=(signed char **)feuille_lire_ligne(FEUILLE_COMP_C,node); /* compétences particulière donnée à la classe */
        classe_add->compt_spe[1]=(signed char **)feuille_lire_ligne(FEUILLE_COMP_CP,node);
        feuille_lire_competences(node,&(classe_add->liste_competences));
        classe_add->progression_competences[0]=feuille_lire_case_m(FEUILLE_COMP_I ,node,0);
        classe_add->progression_competences[1]=feuille_lire_case_m(FEUILLE_COMP_N ,node,0);
        classe_add->psi._2=lecture_case_psi2(FEUILLE_PSI_CLASSE,node);
    }

    classe_add->voleur=lecture_case_up(FEUILLE_VOLEUR,node);
    classe_add->tab_voleur=feuille_lire_voleur(FEUILLE_VOL_TABLE,node);
    classe_add->backstab=feuille_lire_ligne_nb(FEUILLE_BACK_STAB,node);
    classe_add->XP=feuille_lire_colonne_nbl(FEUILLE_XP,node);
    classe_add->init[0]=(signed short)donne_float_A1(FEUILLE_B_INIT,node);
    classe_add->init[1]=(signed short)donne_float_A1(FEUILLE_B_INIT_NIV,node);
    i=-1;
    if ((tmp1=lecture_case(FEUILLE_PANTHEON,node)))
    {
        if (tmp1!=NULL && tmp1[0]!=0)
        {
                classe_add->pantheon=g_strdup(tmp1);
                i=pantheon_entier(tmp1);
                if (i<0)
                {
                    printf("Le panthéon %s imposé à la classe %s n'est pas dans la base de données\n",tmp1,classe_add->nom);
                }
                else
                {
                    /* panthéon trouvé */
                }
        }
        else
        {
            classe_add->pantheon=NULL;
        }
    }
    else
    {
        classe_add->pantheon=NULL;
    }
    if ((tmp1=lecture_case(FEUILLE_DIEU,node)))
    {
        if (tmp1!=NULL && tmp1[0]!=0)
        {
                classe_add->dieu=g_strdup(tmp1);
                if (i<0)
                {
                    printf("La classe %s a un dieu %s imposé alors qu'elle n'a pas de panthéon valide\n",classe_add->nom,tmp1);
                }
                else
                {
                    /* c'est bon, on a un panthéon et un dieu */
                    j=dieu_entier(tmp1,i);
                    if (j<0)
                    {
                        printf("La classe %s a un dieu %s imposé alors qu'il n'est pas dans le panthéon demandé %s\n",classe_add->nom,tmp1,classe_add->pantheon);
                    }
                    else
                    { /* bon couple panthéon classe */
                    }
                }
        }
        else
        {
            classe_add->dieu=NULL;
        }
    }
    else
    {
        classe_add->dieu=NULL;
    }

}

void feuille_pdv(char ** pdv,GMarkupDomNode * node)
{
    GMarkupDomNode * info;
    unsigned long i,j,c;
    char *coordonnees=FEUILLE_PDV;

    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
         i=i*26+*(coordonnees+j)-'A'+1;
    }
    if (j==0)
    {
        for (c=0;c<20;c++)
        {
            pdv[c]=NULL; /* aucune lettre devant */
        }
    }
    else
    {
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1) for (c=0;c<20;c++) pdv[c]=NULL; /* pas d'entier */
        else
        {
            for (c=0;c<20;c++)
            {
                info=donne_case_ij(j-1,i-1+c,node);
                if (info==NULL)
                {
                    pdv[c]=NULL; /* pas la case */
                }
                else if (info->nb_texte==0)
                {
                    pdv[c]=NULL; /* pas de texte */
                }
                else
                {
                    pdv[c]=g_strdup(info->texte->texte);
                }
            }
        }
    }
}

char ** feuille_lire_ligne(char * coordonnees,GMarkupDomNode * node)
{
    unsigned long i,j,c;
    GMarkupDomNode * info;
    gchar ** sortie=NULL;

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
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        {
            sortie=NULL;
        }
        else
        {
            /* passage des coordonnées humaines aux coordonnées informatiques */

            for(c=0;(info=donne_case_ij(j-1,i-1+c,node)) && info->nb_texte!=0;c++)
            {   /* tant qu'il y a des cases, on lit le contenu */
                sortie=(gchar **)g_realloc(sortie,(c+2)*sizeof(gchar *));
                sortie[c]=g_strdup(info->texte->texte);
            }
            if (c==0)
            {
                sortie=(gchar **)g_malloc(sizeof(gchar *));
                *sortie=NULL;
            }
            else
            { /* place déjà réservée */
                sortie[c]=NULL;
            }
        }
    }

    return sortie;
}

signed short * feuille_lire_ligne_arme(char * coordonnees,GMarkupDomNode * node)
{
    unsigned long i,j,c=0;
    GMarkupDomNode * info;
    signed short * sortie=NULL, * liste_arme;

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
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        {
            sortie=NULL;
        }
        else
        {
            /* passage des coordonnées humaines aux coordonnées informatiques */
            for(c=0;(info=donne_case_ij(j-1,i-1+c,node)) && info->nb_texte!=0;c++)
            {   /* tant qu'il y a des cases, on lit le contenu */
                sortie=( signed short *)g_realloc(sortie,(c+2)*sizeof(signed short));
                if (compare_sans_casse(info->texte->texte,"tout")==0)
                {
                    sortie[c]=-1;
                }
                else
                {
                    sortie[c]=arme_entier(info->texte->texte);
                    if (sortie[c]==NON)
                    {
                        printf("Arme %s non reconnue\n",info->texte->texte);
                    }
                    else
                    { /* l'arme a été reconnue */
                    }
                }
            }
            if (c==0)
            {
                sortie=( signed short *)g_malloc(sizeof(signed short));
                *sortie=FIN;
            }
            else
            {
                sortie[c]=FIN;
            }
            for(i=0;i<c;i++)
            {
                if (sortie[i]==NON)
                { /* on suprimme l'entrée inconnue */
                    for(j=i;j<c;j++)
                    {
                        sortie[j]=sortie[j+1];
                    }
                    c--;
                }
                else
                { /* l'arme est reconnue */
                }
            }
        }
    }
    /* on a maintenant la liste des armes comprenant peut-être des groupes */
    /* on va faire les liste réelle des armes sans les groupes, mais en prenant les armes de ses graoupes */

    liste_arme=(signed short *)g_malloc(nb_armes*sizeof(signed short));
    for (i=0;i<nb_armes;i++)
    {
        liste_arme[i]=0;
    }
    for (i=0;i<c;i++) /* c retient le nombre d'entrées du tableau sortie */
    {
        if (sortie[i]==-1) /* tout */
        {
            for (j=0;j<nb_armes;j++)
            {
                if (armes[j].groupe==1)
                {
                    liste_arme[j]=1;
                }
                else
                {
                    j=nb_armes; /* comme on a fini les armes, on soute les groupes */
                }
            }
            i=c; /* on a déjà tout, pas besoin d'en rajouter : ce serait anormal de passer par là ! */
        }
        else if (armes[sortie[i]].groupe==1)
        {
            liste_arme[sortie[i]]=1;
        }
        else
        {
            if (armes[sortie[i]].pt_groupe!=NULL)
            {
                for (j=0;armes[sortie[i]].pt_groupe[j]!=FIN;j++)
                {
                    if (armes[armes[sortie[i]].pt_groupe[j]].groupe==1)
                    {
                        liste_arme[armes[sortie[i]].pt_groupe[j]]=1;
                    }
                    else
                    { /* c'est un groupe : je ne le prends pas en compte */
                    }
                }
            }
            else
            { /* groupe vide ??? */
            }
        }
    }
    /* décompte du nombre d'armes */
    j=0;
    for (i=0;i<nb_armes;i++)
    {
        j+=liste_arme[i];
    }
    g_free(sortie);
    sortie=( signed short *)g_malloc((j+1)*sizeof(signed short));
    c=0;
    for (i=0;i<j;i++)
    {
        while (liste_arme[c]==0)
        { /* recherche de la première arme */
            c++;
        }
        sortie[i]=c;
        c++; /* il faudra cherche l'arme suivante */
    }
    sortie[j]=FIN;
    g_free(liste_arme);

    return sortie;
}




void feuille_lire_competences(GMarkupDomNode * node, unsigned long * l_c)
{
    signed long i,j,c,k;
    GMarkupDomNode * info;
    char * coordonnees=FEUILLE_COMP_C1;

    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
        i=i*26+*(coordonnees+j)-'A'+1;
    }
    if (j==0)
    {
        /* aucune lettre devant */
    }
    else
    {
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        { /* erreur de lecture : pas de chiffres  après les lettres */
        }
        else
        {/* passage des coordonnées humaines aux coordonnées informatiques */
            *l_c=0;
            for(c=0;(info=donne_case_ij(j-1,i-1+c,node)) && info->nb_texte!=0;c++)
            {   /* tant qu'il y a des cases, on lit le contenu */
                if (liste_competences_primaires!=NULL)
                for (k=0;k>=0 && liste_competences_primaires[k]!=NULL;k++)
                {
                    if (compare_sans_casse(liste_competences_primaires[k],info->texte->texte)==0)
                    { /* on a trouvé */
                        *l_c|=(1<<k);
                        k=-10; /* sortie + témoin */
                    }
                    else
                    { /* pas encore le bon */
                    }
                }
                if (k>0)
                { /* on n'a pas trouvé dans les primaires : on cherche dans les secondaires */
                    if (liste_competences_primaires!=NULL)
                    for (k=0;k>=0 && liste_competences_secondaires[k]!=NULL;k++)
                    {
                        if (compare_sans_casse(liste_competences_secondaires[k],info->texte->texte)==0)
                        { /* on a trouvé */
                            *l_c|=(1<<(k+nb_groupe_competences_primaires));
                            k=-10; /* sortie + témoin */
                        }
                        else
                        { /* pas encore le bon */
                        }
                    }
                    if (k>0)
                    {
                        printf("Le groupe de compétences %s est inconnu\n",info->texte->texte);
                    }
                    else
                    { /* c'est bon, on a trouvé un groupe secondaire */
                    }
                }
                else
                { /* c'est un groupe primaire */
                }
            }
        }
    }
}

signed short feuille_lire_origine(GMarkupDomNode * node,struct_origine * ori)
{
    unsigned long i,j,c;
    GMarkupDomNode * info;
    char * coordonnees=FEUILLE_ORIGINE;

    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
        i=i*26+*(coordonnees+j)-'A'+1;
    }
    if (j==0)
    {
        c=-3; /* aucune lettre devant */
    }
    else
    {
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        {
            c=-3;
        }
        else
        {/* passage des coordonnées humaines aux coordonnées informatiques */
            info=donne_case_ij(j-1,i-1,node);
            if (info && info->nb_texte!=0)
            {
                for(c=0;ori[c].court && compare_sans_casse(info->texte->texte,ori[c].court)!=0;c++);
                if (ori[c].court)
                {
                    /* on est sorti parce qu'on a trouvé l'origine */
                }
                else
                { /* erreur : origine non trouvée */
                  c=-1;
                }
            }
            else
            { /* aucune origine */
                c=-2;
            }
        }
    }

    return c;
}

unsigned short feuille_lire_alignement(GMarkupDomNode * node)
{
    unsigned long i,j,c;
    GMarkupDomNode * info;
    unsigned short sortie=0;
    char * coordonnees=FEUILLE_ALIGNEMENT;

    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
        i=i*26+*(coordonnees+j)-'A'+1;
    }
    if (j==0)
    {
        sortie=0; /* aucune lettre devant */
    }
    else
    {
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        {
            sortie=0;
        }
        else
        {/* passage des coordonnées humaines aux coordonnées informatiques */
            for(c=0;(info=donne_case_ij(j-1,i-1+c,node)) && info->nb_texte!=0;c++)
            {   /* tant qu'il y a des cases, on lit le contenu */
                if      (compare_sans_casse(info->texte->texte,"BL")==0) sortie=sortie|BL;
                else if (compare_sans_casse(info->texte->texte,"BN")==0) sortie=sortie|BN;
                else if (compare_sans_casse(info->texte->texte,"BC")==0) sortie=sortie|BC;
                else if (compare_sans_casse(info->texte->texte,"NL")==0) sortie=sortie|NL;
                else if (compare_sans_casse(info->texte->texte,"NN")==0) sortie=sortie|NN;
                else if (compare_sans_casse(info->texte->texte,"NC")==0) sortie=sortie|NC;
                else if (compare_sans_casse(info->texte->texte,"ML")==0) sortie=sortie|ML;
                else if (compare_sans_casse(info->texte->texte,"MN")==0) sortie=sortie|MN;
                else if (compare_sans_casse(info->texte->texte,"MC")==0) sortie=sortie|MC;
            }
        }
    }

    return sortie;
}

struct sv_add * lecture_case_save(char * coordonnees,GMarkupDomNode * ooo,unsigned short version_)
{
    GMarkupDomNode * info;
    unsigned short i;
    struct sv_add * retour=NULL;

    info=donne_case_A1(coordonnees,ooo);
    if (info==NULL)
    {
        retour = NULL;
    }
    else if (info->nb_texte==0)
    {
        retour = NULL; /* pas de texte */
    }
    else if (version_==ADD1)
    {
        for (i=0;i<nb_save;i++)
        {
            if (save[i].add[ADD].nom!=NULL && compare_sans_casse(save[i].add[ADD].nom,info->texte->texte)==0)
            {
                retour=&(save[i].add[ADD]);
                i=nb_save;
            }
            else
            {
            }
        }
    }
    else
    {
        for (i=0;i<nb_save;i++)
        {
            if (save[i].add[ADD2].nom!=NULL && compare_sans_casse(save[i].add[ADD2].nom,info->texte->texte)==0)
            {
                retour = &(save[i].add[ADD2]);
                i=nb_save;
            }
            else
            {
            }
        }
    }

    return retour;
}

so_add * lecture_case_sort(char * coordonnees,GMarkupDomNode * ooo,unsigned short version_)
{
    GMarkupDomNode * info;
    unsigned short i;
    so_add * retour=NULL;



    info=donne_case_A1(coordonnees,ooo);
    if (info==NULL)
    {
        retour=NULL;
    }
    else if (info->nb_texte==0)
    {
         retour = NULL; /* pas de texte */
    }
    else if (version_==ADD1)
    {
        for (i=0;i<nb_sort;i++)
        {
            if (sort[i].add[ADD].nom!=NULL && compare_sans_casse(sort[i].add[ADD].nom,info->texte->texte)==0)
            {
                retour = &(sort[i].add[ADD]);
                i=nb_sort;
            }
            else
            {
            }
        }
    }
    else
    {
        for (i=0;i<nb_sort;i++)
        {
            if (sort[i].add[ADD2].nom!=NULL && compare_sans_casse(sort[i].add[ADD2].nom,info->texte->texte)==0)
            {
                retour = &(sort[i].add[ADD2]);
                i=nb_sort;
            }
            else
            {
            }
        }
    }

    return retour;
}


/* à revoir */
struct psi_add1 * lecture_case_psi1(char * coordonnees,GMarkupDomNode * ooo)
{
    GMarkupDomNode * info;
    unsigned short i;
    struct psi_add1 * retour=NULL;


    info=donne_case_A1(coordonnees,ooo);
    if (info==NULL || TRUE ) /* pas encore pris en compte */
    {
        retour = NULL;
    }
    else if (info->nb_texte==0)
    {
        retour= NULL; /* pas de texte */
    }
    else
    {
        for (i=0;i<nb_psi;i++)
        {
            if (psi[i].add1.nom!=NULL && compare_sans_casse(psi[i].add1.nom,info->texte->texte)==0)
            {
                retour= &(psi[i].add1);
                i=nb_psi;
            }
        }
    }
    return retour;
}

/* à revoir */
struct psi_add2 * lecture_case_psi2(char * coordonnees,GMarkupDomNode * ooo)
{
    GMarkupDomNode * info;
    unsigned short i;
    struct psi_add2 * retour=NULL;


    info=donne_case_A1(coordonnees,ooo);
    if (info==NULL)
    {
        retour = NULL;
    }
    else if (info->nb_texte==0)
    {
        retour = NULL; /* pas de texte */
    }
    else
    {
        for (i=0;i<nb_psi;i++)
        {
            if (psi[i].add2.nom!=NULL && compare_sans_casse(psi[i].add2.nom,info->texte->texte)==0)
            {
                retour = &(psi[i].add2);
                i=nb_psi;
            }
            else
            { /* on a trouvé le bon : on renvoit un lien vers le fichier psi correspondant */
            }
        }
    }

    return retour;
}

unsigned long ecole_id(gchar ** liste_ecole,unsigned long * spe)
{
    unsigned short i,j;
    unsigned long retour=0;


    *spe=0;
    if (liste_ecole!=NULL)
    {
        for(i=0;liste_ecole[i]!=NULL;i++)
        {
            for (j=0;j<nb_ecole;j++)
            {
             if (compare_sans_casse(ECOLE[ADD2][j].nom,liste_ecole[i])==0)
             {
                 if ((retour&(1<<j))==0)
                 {
                     retour|=(1<<j);
                 }
                 else
                 {
                     *spe|=(1<<j);
                 }
             }
             else
             { /* ce n'est pas la bonne école */
             }
            }
            g_free(liste_ecole[i]);
        }

        g_free(liste_ecole);
    }
    else
    { /* liste vide */
    }

    return retour;
}

signed short ** feuille_lire_voleur(char * coordonnees,GMarkupDomNode * node)
{
    signed long i,j,c,k,l;
    GMarkupDomNode * info;
    signed short ** sortie=NULL;


    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
        i=i*26+*(coordonnees+j)-'A'+1;
    if (j==0)
    {
        sortie= NULL; /* aucune lettre devant */
    }
    else
    {
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        {
            sortie = NULL;
        }
        else
        {
            for (c=0;donne_case_ij(j-1+c,i-1,node) || donne_case_ij(j-1+c,i,node);c++);
            sortie=(signed short **)g_malloc((c+1)*sizeof(signed short *));
            for (k=0;k<c;k++)
            {
                sortie[k]=(signed short *)g_malloc((FEUILLE_LAR_VOL+1)*sizeof(signed short));
                sortie[k][FEUILLE_LAR_VOL]=FIN;
                for(l=0;l<FEUILLE_LAR_VOL;l++)
                {
                    info=donne_case_ij(j-1+k,i+l,node);
                    if (info==NULL || info->nb_texte==0 || sscanf(info->texte->texte,"%hd",sortie[k]+l)==0)
                        sortie[k][l]=0; /* case vide => impossible de le monter mettre -1 si ça existe */
                }
            }
            sortie[c]=NULL;
        }
    }

    return sortie;
}

void retourne_sort(struct_sort * sort_, char * fichier)
{
    GMarkupDomNode * node,* ooo;
    char * tmp;
    signed short i,j,k;
    GMarkupDomNode * info;
    so_add * etude;


    ooo = g_markup_dom_new (fichier, NULL);
    if (sort_->type!=NULL)
    {
        g_free(sort_->type);
        sort_->type=NULL;
    }
    k=0;
    for(j=0;fichier[j]!=0;j++)
        if (fichier[j]==SEPARATEUR || fichier[j]=='/') k=j; /* recherche du dernier séparateur */
    if (fichier[k]==SEPARATEUR || fichier[k]=='/') k++;     /* s'il n'y en pas, on est au début du mot, sinon, il faut aller au suivant */
    for(i=k;fichier[i]!='_' && fichier[i]!=0;i++); /* avant le '_' => type de sort : clerc ou mag, après la classe consernée */
    sort_->type=(char *)g_malloc(i-k+1);
    strncpy(sort_->type,fichier+k,i-k); /* clerc, druide, mago ou druide */
    sort_->type[i-k]=0;

    for (k=0;k<2;k++) /* pour une fois, les structures pour ADD1 et ADD2 sont semblables ! */
    {
        switch(k)
        {
            case ADD1 :
                    etude=&(sort_->add[ADD]);
                    break;
            default :
                    etude=&(sort_->add[ADD2]);
                    break;

        }
        if (etude->nb!=NULL)
        {
            for(i=0;etude->nb[i]!=NULL;i++)
                g_free(etude->nb[i]);
            g_free(etude->nb);
            etude->nb=NULL;
        }
    }
    if (ooo)
    {
        for (k=0;k<2;k++) /* pour une fois, les structures pour ADD1 et ADD2 sont semblables ! */
        {
            switch(k)
            {
                case ADD1 :
                        etude=&(sort_->add[ADD]);
                        node=g_markup_dom_nom (ooo,"ADD1","table:name");
                        break;
                default :
                        etude=&(sort_->add[ADD2]);
                        node=g_markup_dom_nom (ooo,"ADD2","table:name");
                        break;

            }
            if (node)
            {
             if ((tmp=lecture_case(FEUILLE_NOM,node)))
             {
                g_free(etude->nom);
                etude->nom=g_strdup(tmp); /* nom de la classe */

                for(i=0;(info=donne_case_ij(1+i,0,node)) && info->nb_texte!=0;i++);
                etude->nb=(signed short **)g_malloc((i+1)*sizeof(signed short *));
                etude->nb[i]=NULL;
                for(i--;i>=0;i--)
                {   /* tant qu'il y a des cases, on lit le contenu */
                    for(j=0;(info=donne_case_ij(1+i,j,node)) && info->nb_texte!=0;j++);
                    etude->nb[i]=(signed short *)g_malloc((j)*sizeof(signed short));
                    j--;
                    etude->nb[i][j]=-1;
                    for(j--;j>=0;j--)
                    {
                        info=donne_case_ij(1+i,j+1,node);
                        etude->nb[i][j]=0;
                        sscanf(info->texte->texte,"%hu",etude->nb[i]+j);
                    }
                }
             }
            }
        }
        g_markup_dom_free(ooo);
    }
}

void retourne_save(struct_save * save_, char * fichier)
{
    GMarkupDomNode * node,* ooo,* node_t;
    char * tmp;
    unsigned short i,j;
    signed short dpt;

    if ((ooo = g_markup_dom_new (fichier, NULL)))
    {
        if (save_->add[ADD].save!=NULL)
        {
            g_free(save_->add[ADD].save);
            save_->add[ADD].save=NULL;
        }
        if (save_->add[ADD2].save!=NULL)
        {
            g_free(save_->add[ADD2].save);
            save_->add[ADD2].save=NULL;
        }
        if (save_->add[ADD].nom!=NULL)
        {
            g_free(save_->add[ADD].nom);
            save_->add[ADD].nom=NULL;
        }
        if (save_->add[ADD2].nom!=NULL)
        {
            g_free(save_->add[ADD2].nom);
            save_->add[ADD2].nom=NULL;
        }
        if ((node=g_markup_dom_nom (ooo,"ADD1","table:name")))
        { /* sinon, la version ADD1 de la classe_ n'existe pas */
            if ((tmp=lecture_case(FEUILLE_NOM,node)))
            {
                save_->add[ADD].nom=g_strdup(tmp);
                i=1;
                dpt=feuille_lire_case_m(FEUILLE_DEPART_SAVE ,node,-1);
                if (dpt==-1) /* la case est vide : un gros problème */
                {
                     printf("Gros problème au fichier save %s\n",fichier);
                     g_markup_dom_free(ooo);
                     return;
                }
                else
                {
                }
                if (dpt==0) i++; /* pour le cas du guerrier 0 qui est en fait un perso sans classe_ */
                while(donne_case_ij(i,0,node)!=NULL)
                {
                    save_->add[ADD].save=(unsigned short **)g_realloc(save_->add[ADD].save,(i+dpt-1)*sizeof(unsigned short *));
                    save_->add[ADD].save[i+dpt-2]=(unsigned short *)g_malloc(5*sizeof(unsigned short ));
                    for(j=1;j<=5;j++)
                    {
                        node_t=donne_case_ij(i,j,node);
                        if (node_t!=NULL && node_t->nb_texte!=0 && sscanf(node_t->texte->texte,"%hu",save_->add[ADD].save[i+dpt-2]+j-1)==0)
                          save_->add[ADD].save[i+dpt-2][j-1]=0;
                    }
                    i++;
                }
                save_->add[ADD].save=(unsigned short **)g_realloc(save_->add[ADD].save,(i+dpt-1)*sizeof(unsigned short *));
                save_->add[ADD].save[i+dpt-2]=NULL;
            }
            else
            {
                save_->add[ADD].nom=NULL;
            }
        }
        else save_->add[ADD].nom=NULL;
        if ((node=g_markup_dom_nom (ooo,"ADD2","table:name")))
        { /* sinon, la version ADD1 de la classe_ n'existe pas */
            if ((tmp=lecture_case(FEUILLE_NOM,node)))
            {
                save_->add[ADD2].nom=g_strdup(tmp);
                i=1;
                dpt=feuille_lire_case_m(FEUILLE_DEPART_SAVE ,node,-1);
                if (dpt==-1) /* la case est vide : un gros problème */
                {
                    g_markup_dom_free(ooo);
                    return;
                }
                if (dpt==0) i++; /* pour le cas du guerrier 0 qui est en fait un perso sans classe_ */
                while(donne_case_ij(i,0,node)!=NULL)
                {
                    save_->add[ADD2].save=(unsigned short **)g_realloc(save_->add[ADD2].save,(i+dpt-1)*sizeof(unsigned short *));
                    save_->add[ADD2].save[i+dpt-2]=(unsigned short *)g_malloc(5*sizeof(unsigned short ));
                    for(j=1;j<=5;j++)
                    {
                        node_t=donne_case_ij(i,j,node);
                        if (node_t!=NULL && node_t->nb_texte!=0 && sscanf(node_t->texte->texte,"%hu",save_->add[ADD2].save[i+dpt-2]+j-1)==0)
                          save_->add[ADD2].save[i+dpt-2][j-1]=0;
                    }
                    i++;
                }
                save_->add[ADD2].save=(unsigned short **)g_realloc(save_->add[ADD2].save,(i+dpt-1)*sizeof(unsigned short *));
                save_->add[ADD2].save[i+dpt-2]=NULL;

            }
            else save_->add[ADD2].nom=NULL;
        }
        else save_->add[ADD2].nom=NULL;
        g_markup_dom_free(ooo);
    }
    else
    {
        save_->add[ADD].nom=NULL;
        save_->add[ADD2].nom=NULL;
    }
}

struct_armes * lire_struct_armes(char * fichier)
{
 unsigned short i=0;
 signed short j;
 GMarkupDomNode * node, *ooo, *ooo1;
 struct_armes * retour=NULL;

 ooo1 = g_markup_dom_new (fichier, NULL);
 if (ooo1!=NULL)
 {
     ooo=g_markup_dom_node (ooo1,"table:table"); /* pour la suite, il faut être dans une feuille : la première */
     node=donne_case_ij(3,0,ooo);
     while(node && node->nb_texte>0)
     {
        retour=(struct_armes *)g_realloc(retour,(i+1)*sizeof(struct_armes));
        retour[i].nom=g_strdup(node->texte->texte);

        node=donne_case_ij(i+3,1,ooo);
        if (node && node->nb_texte>0 )
        {
            j=sscanf(node->texte->texte,"%hu",&retour[i].longueur);
            if (j!=1) retour[i].longueur=0;
        }
        else
            retour[i].longueur=0;

        node=donne_case_ij(i+3,2,ooo);
        if (node && node->nb_texte>0 )
        {
            j=sscanf(node->texte->texte,"%hd",&retour[i].vitesse); /* car -1 pour armes non lancées non répertoriées */
            if (j!=1) retour[i].vitesse=0;
        }
        else
            retour[i].vitesse=0;

        node=donne_case_ij(i+3,3,ooo);
        if (node && node->nb_texte>0 )
        {
            for(j=0;node->texte->texte[j]!=0 && j<7;j++)
                retour[i].PM[j]=node->texte->texte[j];
            retour[i].PM[j]=0;
        }
        else
            retour[i].PM[0]=0;

        node=donne_case_ij(i+3,4,ooo);
        if (node && node->nb_texte>0 )
        {
            for(j=0;node->texte->texte[j]!=0 && j<7;j++)
                retour[i].G[j]=node->texte->texte[j];
            retour[i].G[j]=0;
        }
        else
            retour[i].G[0]=0;

        node=donne_case_ij(i+3,9,ooo);
        if (node && node->nb_texte>0 )
        {
            j=sscanf(node->texte->texte,"%hu",&retour[i].groupe);
            if (j!=1) retour[i].groupe=1;
            /*else
                printf(" %s => %hu\n",retour[i].nom,retour[i].groupe);*/
        }
        else
            retour[i].groupe=1;
        retour[i].pt_groupe=NULL;


        i++;
        node=donne_case_ij(i+3,0,ooo);
     }
     retour=(struct_armes *)g_realloc(retour,(i+1)*sizeof(struct_armes));
     retour[i].nom=NULL;
     nb_armes+=i;

     g_markup_dom_free(ooo1);

 }
 return retour;

}

struct_armure * lire_struct_armures(char * fichier)
{
 unsigned short k,i=0;
 signed short j;
 GMarkupDomNode * node, *ooo, *ooo1;
 struct_armure * retour=NULL;

 ooo1 = g_markup_dom_new (fichier, NULL);
 if (ooo1!=NULL)
 {
     ooo=g_markup_dom_node (ooo1,"table:table"); /* pour la suite, il faut être dans une feuille : la première */
     node=donne_case_ij(1,1,ooo);
     while(node && node->nb_texte>0)
     {
        retour=(struct_armure *)g_realloc(retour,(i+1)*sizeof(struct_armure));
        retour[i].nom=g_strdup(node->texte->texte);

        node=donne_case_ij(i+1,2,ooo);
        if (node && node->nb_texte>0 )
        {
            j=sscanf(node->texte->texte,"%hu",&retour[i].ac);
            if (j!=1) retour[i].ac=10;
        }
        else
            retour[i].ac=10;

        for(k=0;k<15;k++)
        {
            node=donne_case_ij(i+1,4+k,ooo);
            if (node && node->nb_texte>0 )
            {
                j=sscanf(node->texte->texte,"%hu",retour[i].add[ADD]+k);
                if (j!=1) retour[i].add[ADD][k]=0;
            }
            else
                retour[i].add[ADD][k]=0;
        }

        for(k=0;k<16;k++)
        {
            node=donne_case_ij(i+1,20+k,ooo);
            if (node && node->nb_texte>0 )
            {
                j=sscanf(node->texte->texte,"%hu",retour[i].add[ADD2]+k);
                if (j!=1) retour[i].add[ADD2][k]=0;
            }
            else
                retour[i].add[ADD2][k]=0;
        }

        i++;
        node=donne_case_ij(i+1,1,ooo);
     }
     retour=(struct_armure *)g_realloc(retour,(i+1)*sizeof(struct_armure));
     retour[i].nom=NULL;
     nb_armures=i;

     g_markup_dom_free(ooo1);
 }

 return retour;
}


struct_origine ** lire_struct_origine(char * fichier)
{
 unsigned short i=0,_version;
 GMarkupDomNode * node=NULL, *ooo1=NULL;
 struct_origine ** retour=NULL;

 retour=(struct_origine **)g_malloc(2*sizeof(struct_origine *));
 retour[0]=NULL;
 retour[1]=NULL;
 ooo1 = g_markup_dom_new (fichier, NULL);
 if (ooo1!=NULL)
 {
     node=g_markup_dom_nom (ooo1,"ADD1","table:name");
     for (_version=0;_version<2;_version++)
     {
         if (node)
         {
            i=0;
            do
            {
                retour[_version]=(struct_origine *)g_realloc(retour[_version],(i+1)*sizeof(struct_origine));
                retour[_version][i].court=lecture_case_up_ij(i,0,node);
                retour[_version][i].nom=lecture_case_up_ij(i,1,node);
                retour[_version][i].clef=donne_float_ij(i,2,node);
                i++;
            }
            while (retour[_version][i-1].court && retour[_version][i-1].nom);
            if (retour[_version][i-1].court || retour[_version][i-1].nom)
            {
                printf("Surprise dans ADD%hd dans origine, ligne %hd, court=%s et nom=%s\n",_version,i,retour[_version][i-1].court,retour[_version][i-1].nom);
                g_free(retour[_version][i-1].court); /* par précaution si un seul des deux est initialisé */
                retour[_version][i-1].court=NULL;
                g_free(retour[_version][i-1].nom);
                retour[_version][i-1].nom=NULL;
            }
            else
            {
                /* c'est bon, les deux sont vierges */
            }
            if (_version==0)
            {
                node=g_markup_dom_nom (ooo1,"ADD2","table:name"); /* préparer pour le tour suivant */
            }
            else
            {
                /* rien à faire : on va sortir */
            }
            nb_origine_[_version]=i-1;
         }
         else
         {
             printf("Surprise pas de feuille ADD%d dans origine\n",_version);
         }
     }

     g_markup_dom_free(ooo1);
 }

 return retour;
}


unsigned short groupe_armes(struct_armes * t_armes,unsigned short pt_gr,char * fichier)
{
 signed short i=1,j,a;
 GMarkupDomNode * node, *ooo, *ooo1;
 unsigned short retour=0; /* nombre d'armes lues dans le groupe */

 if ((ooo1 = g_markup_dom_new (fichier, NULL)))
 {
    if((ooo=g_markup_dom_node (ooo1,"table:table"))) /* pour la suite, il faut être dans une feuille : la première */
    {
         for(j=0;t_armes[j].nom!=NULL && t_armes[j].groupe!=pt_gr;j++);

         node=donne_case_ij(0,1,ooo);
         if (t_armes[j].nom!=NULL && node && node->nb_texte>0)
         {
             for(;t_armes[j].nom!=NULL && compare_sans_casse(node->texte->texte,t_armes[j].nom)!=0 && t_armes[j].groupe==pt_gr;j++);

             if (t_armes[j].groupe==pt_gr) /* un petit ou un grand groupe correspondant a été trouvé */
             {
                 node=donne_case_ij(1,1,ooo);
                 i=1;
                 while(node && node->nb_texte>0)
                 {

                    a=arme_entier(node->texte->texte);
                    if (a!=NON)
                    {
                        retour++;
                        t_armes[j].pt_groupe=(signed short *)g_realloc(t_armes[j].pt_groupe,retour*sizeof(signed short));
                        t_armes[j].pt_groupe[retour-1]=a;
                    }
                    else
                    {
                        printf("Erreur : arme ou petit groupe %s inconnue dans le fichier %s\n",node->texte->texte,fichier);
                    }
                    i++;
                    node=donne_case_ij(i,1,ooo);
                 }
                 if (retour>0) /* on marque la fin de la liste */
                 {
                    t_armes[j].pt_groupe=(signed short *)g_realloc(t_armes[j].pt_groupe,(retour+1)*sizeof(signed short));
                    t_armes[j].pt_groupe[retour]=FIN;
                 }
                 else
                 { /* c'est un groupe vide ! */
                     printf("Erreur : groupe du fichier %s vide\n",fichier);
                 }
             }
             else
             {
                 printf("Erreur : groupe inconnu %s dans le fichier %s\n",node->texte->texte,fichier);
             }

         }
         else
         {
             printf("Erreur : pas de groupe %hu dans la liste\nou fichier %s incorrect\n",pt_gr,fichier);
         }
    }
    else
    {
        printf("Erreur dans la lecture du fichier %s : pas de table\n",fichier);
    }

    g_markup_dom_free(ooo1);

 }
 else
 {
     printf("Erreur dans la lecture du fichier %s, mauvaise structure xml\n",fichier);
 }

 return retour;

}

void retourne_psi(struct_psi_c * psi_, char * fichier)
{
    GMarkupDomNode * ooo, *ooo1;
    signed short i;
    char * ch;
    float reel;
    char ** ligne;

    if ((ooo1 = g_markup_dom_new (fichier, NULL)))
    {
        psi_->add1.nom=NULL; /* pour le moment ADD1 est prévu sans psi */

        if ((ooo=g_markup_dom_nom (ooo1,"ADD2","table:name")))
        {
                 if ((ch=lecture_case(FEUILLE_NOM,ooo)))
                 {
                    psi_->add2.nom=g_strdup(ch);
                    psi_->add2.base[0]=0;
                    psi_->add2.bonus[0]=0;
                    for (i=1;i<=25;i++)
                    {
                        psi_->add2.base[i]=donne_float_ij(1+i,2,ooo);
                        psi_->add2.bonus[i]=donne_float_ij(1+i,3,ooo);
                    }
                    ligne=feuille_lire_ligne(FEUILLE_PSI_DB_BASE,ooo);
                    psi_->add2.debut.base=lire_caract(ligne);
                    g_strfreev(ligne);
                    ligne=feuille_lire_ligne(FEUILLE_PSI_DB_BONU,ooo);
                    psi_->add2.debut.bonus=lire_caract(ligne);
                    g_strfreev(ligne);
                    ligne=feuille_lire_ligne(FEUILLE_PSI_NV_BASE,ooo);
                    psi_->add2.niveau.base=lire_caract(ligne);
                    g_strfreev(ligne);
                    ligne=feuille_lire_ligne(FEUILLE_PSI_NV_BONU,ooo);
                    psi_->add2.niveau.bonus=lire_caract(ligne);
                    g_strfreev(ligne);
                    reel=donne_float_ij(37,2,ooo);
                    i=0;
                    while (!isnan(reel))
                    {
                       i++;

                       reel=donne_float_ij(37+i,2,ooo);
                    }
                    i++;
                    psi_->add2.discipline=(signed short *)g_malloc(i*sizeof(unsigned short));
                    psi_->add2.devotion=(signed short *)g_malloc(i*sizeof(unsigned short));
                    psi_->add2.science=(signed short *)g_malloc(i*sizeof(unsigned short));
                    psi_->add2.defense=(signed short *)g_malloc(i*sizeof(unsigned short));
                    reel=donne_float_ij(37,2,ooo);

                    i=0;
                    while (!isnan(reel))
                    {
                       psi_->add2.discipline[i]=reel;
                       psi_->add2.science[i]=donne_float_ij(37+i,3,ooo);
                       psi_->add2.devotion[i]=donne_float_ij(37+i,4,ooo);
                       psi_->add2.defense[i]=donne_float_ij(37+i,5,ooo);
                       i++;
                       reel=donne_float_ij(37+i,2,ooo);
                    }
                    psi_->add2.discipline[i]=FIN; /* négatif pour montrer la fin */
                    psi_->add2.devotion[i]=FIN;
                    psi_->add2.science[i]=FIN;
                    psi_->add2.defense[i]=FIN;
                 }
                 else
                 {
                 }
        }
        else
        {
            psi_->add2.nom=NULL;
            printf("Erreur dans la lecture du fichier %s : pas de table:name dans ADD2\n",fichier);
        }
        g_markup_dom_free(ooo1);
    }
    else
    {
        printf("Erreur dans la lecture du fichier %s, mauvaise structure xml\n",fichier);
    }
}

signed short * lire_caract(char ** ch)
{
    signed short i,j;
    signed short * tab=NULL;

    for (j=0;ch[j];j++)
    {
        tab=(signed short *)g_realloc(tab,(j+2)*sizeof(signed short));
        tab[j]=FIN; /* une valeur de -1 peut être interprêté comme une erreur */
        for (i=0;i<6;i++)
        {
            if (compare_sans_casse(ch[j],(char *)nom_abr[i])==0)
            {
                tab[j]=i;
            }
            else
            {
            }
        }

        if (tab[j]==FIN && sscanf(ch[j],"%hd",&i)==1) /* le cas d'un bonus fixe positif : il sera stocké négativement pour faire la différence avec une caractéristique */
        {
          tab[j]=-i;
        }
        else
        {
        }
    }
    tab[j]=FIN;

    return tab;
}

void retourne_race(struct_race * race_, char * fichier)
{
    GMarkupDomNode * node=NULL,* ooo=NULL;
    char * tmp;

   race_->nom=NULL;
   race_->add[ADD].liste_classe=NULL;
   race_->add[ADD2].liste_classe=NULL;

   if ((ooo = g_markup_dom_new (fichier, NULL)))
    {
        if ((node=g_markup_dom_nom (ooo,"ADD1","table:name")))
        { /* sinon, la version ADD1 de la classe_ n'existe pas */
            if ((tmp=lecture_case(FEUILLE_NOM,node)))
            {
                race_->add[ADD].nom=g_strdup(tmp);
                race_->nom=race_->add[ADD].nom;
                lecture_race(&(race_->add[ADD]),node,ADD1);
            }
            else
            {
                race_->add[ADD].nom=NULL;
            }
        }
        else
        {
            race_->add[ADD].nom=NULL;
        }
        if ((node=g_markup_dom_nom (ooo,"ADD2","table:name")))
        { /* sinon, la version ADD1 de la classe_ n'existe pas */
            if ((tmp=lecture_case(FEUILLE_NOM,node)))
            {
                if (race_->add[ADD].nom==NULL)
                {
                    race_->add[ADD2].nom=g_strdup(tmp);
                    race_->nom=race_->add[ADD2].nom;
                }
                else
                {
                    race_->add[ADD2].nom=race_->add[ADD].nom;
                }
                lecture_race(&(race_->add[ADD2]),node,ADD2);
            }
            else
            {
                race_->add[ADD2].nom=NULL;
            }
        }
        else
        {
            race_->add[ADD2].nom=NULL;
        }
        g_markup_dom_free(ooo);
    }
    else
    {
        race_->add[ADD].nom=NULL;
        race_->add[ADD2].nom=NULL;
    }
}

void lecture_race(struct ra_add * race_v,GMarkupDomNode * node,unsigned short version)
{
    char * tmp;

    feuille_lire_colonne_char_pourcentage(FEUILLE_MIN_CARACT_H,node,race_v->min_caract_h,6,0);
    feuille_lire_colonne_char_pourcentage(FEUILLE_MAX_CARACT_H,node,race_v->max_caract_h,6,25);
    feuille_lire_colonne_char_pourcentage(FEUILLE_MIN_CARACT_F,node,race_v->min_caract_f,6,0);
    feuille_lire_colonne_char_pourcentage(FEUILLE_MAX_CARACT_F,node,race_v->max_caract_f,6,25);
    race_v->max_caract_f[BEAUTE]=18;
    race_v->max_caract_h[BEAUTE]=18;
    race_v->min_caract_f[BEAUTE]=3;
    race_v->min_caract_h[BEAUTE]=3;
    race_v->max_caract_f[HUMOUR]=18;
    race_v->max_caract_h[HUMOUR]=18;
    race_v->min_caract_f[HUMOUR]=3;
    race_v->min_caract_h[HUMOUR]=3;
    feuille_lire_colonne_char(FEUILLE_MOD_CARACT  ,node,race_v->modif_caract,6,0);
    race_v->liste_classe=feuille_lire_classerace(FEUILLE_CLASSE,node);
    race_v->multiclasse=feuille_lire_multiclasse(FEUILLE_MULTICLASSE,node);
    race_v->origine=feuille_lire_origine(node,origine[version]);
    switch(race_v->origine)
    {
    case -1 :
        printf(" Race %s origine non identifiée\n",race_v->nom);
        break;
    case -2 :
        printf(" Race %s origine absente\n",race_v->nom);
        break;
    case -3 : /* ce serait une erreur dans le code source */
        printf(" Coordonnées %s mauvaises\n",FEUILLE_ORIGINE);
        break;
    default : /* pas de problème : origine bien trouvée */
        break;
    }

    if (version==ADD)
    {
        race_v->niv=lecture_lim_racce_add1(node->parent);
        if (race_v->niv==0)
        {
            printf("Pas de limite de niveau ADD1 pour %s\n",race_v->nom);
        }
        else
        {
            /* C'est bon, on a reçu la limite */
        }
    }
    else
    {
        race_v->niv=lecture_lim_racce_add2(node);
    }
    race_v->tranche_age=feuille_lire_ligne_nb(FEUILLE_TRANCH_AGE1,node);
    race_v->CA=feuille_lire_case_d(FEUILLE_AC,node,10);
    feuille_lire_modif_CA(race_v->modif_CA,FEUILLE_CA_RACE,node);
    race_v->pdv=feuille_lire_case_d(FEUILLE_PDV_RACE,node,0);
    if ((tmp=lecture_case(FEUILLE_VISION,node)))        race_v->vision=g_strdup(tmp);
    else                                                race_v->vision=NULL;
    if ((tmp=lecture_case(FEUILLE_ECOUTE,node)))        race_v->ecoute=g_strdup(tmp);
    else                                                race_v->ecoute=NULL;
    if ((tmp=lecture_case(FEUILLE_RESISTANCE,node)))    race_v->resistance=g_strdup(tmp);
    else                                                race_v->resistance=NULL;
    race_v->bonus_arme=feuille_lire_race_b_arme(FEUILLE_BONUS_ARME,node);
    feuille_lire_race_save(FEUILLE_SAVE_RACE,race_v->save,node);
    race_v->langue=feuille_lire_ligne(FEUILLE_LANGUE,node);
    race_v->detection=feuille_lire_ligne(FEUILLE_DETECTION,node);
    race_v->tab_voleur=feuille_lire_voleur(FEUILLE_VOL_TABLE,node);
    if ((tmp=lecture_case(FEUILLE_SURPRENDRE,node)))        race_v->surprendre=g_strdup(tmp);
    else                                                    race_v->surprendre=NULL;
    if ((tmp=lecture_case(FEUILLE_SE_FAIRE_SU,node)))       race_v->sefairesurprendre=g_strdup(tmp);
    else                                                    race_v->sefairesurprendre=NULL;
    if (version==ADD2)
    {
        race_v->competence=feuille_lire_ligne(FEUILLE_RACE_COMP,node);
    }
    else
    {
    }
    race_v->xp=1+feuille_lire_case_m(FEUILLE_RACE_XP,node,0)/100.;
    race_v->taille[0]=feuille_lire_case_m(FEUILLE_H_TAILLE,node,10);
    race_v->taille[1]=feuille_lire_case_m(FEUILLE_F_TAILLE,node,10);
    race_v->var_taille[0]=lecture_case_up(FEUILLE_H_TAILLE_V,node);
    race_v->var_taille[1]=lecture_case_up(FEUILLE_F_TAILLE_V,node);
    race_v->poids[0]=feuille_lire_case_f(FEUILLE_H_POIDS,node,0);
    race_v->poids[1]=feuille_lire_case_f(FEUILLE_F_POIDS,node,0);
}

liste_classe_race * feuille_lire_classerace(char * coordonnees,GMarkupDomNode * node)
{
    unsigned long i,j,c,num_classe;
    GMarkupDomNode * info;
    liste_classe_race * sortie=NULL,* tmp_cr;
    liste_classe * tmp_c;
    char * ch, *pt,*tmp;

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
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        {
            sortie = NULL;
        } /* passage des coordonnées humaines aux coordonnées informatiques */
        else
        {
            for(c=0;(info=donne_case_ij(j-1,i-1+c,node)) && info->nb_texte!=0;c++)
            {   /* tant qu'il y a des cases, on lit le contenu */
                tmp_cr=(liste_classe_race *)g_malloc(sizeof(liste_classe_race));
                tmp_cr->suiv=NULL;
                tmp_cr->classe=NULL;
                ch=info->texte->texte;
                for (pt=ch;*pt!=0;pt++)
                {
                    if (*pt=='/')
                    {
                        *pt=0;
                        tmp_c=(liste_classe *)g_malloc(sizeof(liste_classe));
                        tmp_c->suiv=NULL;
                        tmp=g_strdup(ch);
                        tmp_c->nom=g_strdup(g_strstrip(tmp));
                        tmp_c->num=-1;
                        for(num_classe=0;num_classe<nb_classe;num_classe++)
                        {
                          if (compare_sans_casse(classe[num_classe].nom,tmp_c->nom)==0)
                          {
                              tmp_c->num=num_classe;
                              num_classe=nb_classe;
                          }
                        }
                        g_free(tmp);
                        ch=pt+1;
                        *pt='/';
                        tmp_cr->classe=range_l_cl(tmp_cr->classe,tmp_c);
                    }
                }
                tmp_c=(liste_classe *)g_malloc(sizeof(liste_classe));
                tmp_c->suiv=NULL;
                tmp_c->nom=g_strdup(ch);
                if ((info=donne_case_ij(j,i-1+c,node)))
                {
                     tmp_cr->age=g_strdup(info->texte->texte);
                }
                else
                {
                    tmp_cr->age=NULL;
                }
                tmp_c->num=-1;
                for(num_classe=0;num_classe<nb_classe;num_classe++)
                {
                  if (compare_sans_casse(classe[num_classe].nom,tmp_c->nom)==0)
                  {
                      tmp_c->num=num_classe;
                      num_classe=nb_classe;
                  }
                  else
                  {
                  }
                }
                tmp_cr->classe=range_l_cl(tmp_cr->classe,tmp_c);
                sortie=range_l_cl_r(sortie,tmp_cr);
            }
        }
    }

    return sortie;
}

struct bonus_armes * feuille_lire_race_b_arme(char * coordonnees,GMarkupDomNode * node)
{
    unsigned long i,j,c;
    GMarkupDomNode * info;
    struct bonus_armes * sortie=NULL;

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
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        {
            sortie = NULL;
        } /* passage des coordonnées humaines aux coordonnées informatiques */
        else
        {
            for(c=0;(info=donne_case_ij(j-1,i-1+c,node)) && info->nb_texte!=0;c++)
            {   /* tant qu'il y a des cases, on lit le contenu */
                sortie=(struct bonus_armes *)g_realloc(sortie,(c+1)*sizeof(struct bonus_armes));
                sortie[c].arme=g_strdup(info->texte->texte);
                if ((info=donne_case_ij(j,i-1+c,node))!=NULL && info->nb_texte>0)
                {
                     sscanf(info->texte->texte,"%hd",&(sortie[c].bonus));
                }
                else
                {
                }
            }
            sortie=(struct bonus_armes *)g_realloc(sortie,(c+1)*sizeof(struct bonus_armes));
            sortie[c].arme=NULL;
        }
    }

    return sortie;
}

void feuille_lire_race_save(char * coordonnees,char ** sortie,GMarkupDomNode * node)
{
    unsigned long i,j,c;
    GMarkupDomNode * info;

    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
         i=i*26+*(coordonnees+j)-'A'+1;
    }
    if (j==0)
    {
        for(i=0;i<5;i++)
        {
             sortie[i]=NULL; /* aucune lettre devant */
        }
    }
    else
    {
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        {
            for(i=0;i<5;i++)
            {
                sortie[i]=NULL; /* passage des coordonnées humaines aux coordonnées informatiques */
            }
        }
        else
        {
            for(c=0;c<5;c++)
            {   /* tant qu'il y a des cases, on lit le contenu */
                if ((info=donne_case_ij(j-1,i-1+c,node)) && info->nb_texte!=0)
                {
                    sortie[c]=virgule_point(g_strdup(info->texte->texte));
                }
                else
                {
                    sortie[c]=NULL;
                }
            }
        }
    }
}

liste_classe_race * range_l_cl_r(liste_classe_race * depart, liste_classe_race * nv)
{
     liste_classe_race * tmp;
     liste_classe_race * retour;

     if (depart==nv || depart==NULL)
     {
         retour=nv;
     }
     else
     {

         if (compare_liste_classe(depart->classe,nv->classe)<0)
         {
           tmp=depart;
           while (tmp->suiv!=NULL && compare_liste_classe(tmp->suiv->classe,nv->classe)<0)
           {
               tmp=tmp->suiv;
           }
           nv->suiv=tmp->suiv;
           tmp->suiv=nv;
           retour= depart;
         }
         else
         {
             nv->suiv=depart;
             retour=nv;
         }
     }
     return retour;
}

liste_classe * range_l_cl(liste_classe * depart, liste_classe * nv)
{
     liste_classe * tmp;

     if (nv==NULL)
     {
       tmp=depart;
     }
     else
     {
         if (depart==nv || depart==NULL)
            tmp=nv;
         else
         {
             if (compare_sans_casse(depart->nom,nv->nom)<0)
             {
               tmp=depart;
               while (tmp->suiv!=NULL && compare_sans_casse(tmp->suiv->nom,nv->nom)<0)
               {
                   tmp=tmp->suiv;
               }
               nv->suiv=tmp->suiv;
               tmp->suiv=nv;
               tmp=depart;
             }
             else
             {
                nv->suiv=depart;
                tmp=nv;
             }
         }
     }
     return tmp;
}

signed char compare_liste_classe(liste_classe * depart, liste_classe * nv)
{
    signed char retour;

    if (depart==NULL && nv==NULL)
    {
        retour=0;             /* s'il n'y a pas de différence avant le dernier : retour : 0 */
    }
    else
    {
        if (depart==NULL)
        {
            retour=-1;                        /* s'il n'en reste plus qu'un retour 1 ou -1 */
        }
        else
        {
            if (nv==NULL)
            {
                retour= 1;
            }
            else
            {
                if (compare_sans_casse(depart->nom,nv->nom)!=0)     /* si les deux sont différents  */
                {
                    retour=compare_sans_casse(depart->nom,nv->nom); /* pn renvoie la différence     */
                }
                else
                {
                    retour=compare_liste_classe(depart->suiv,nv->suiv); /* sinon, on regarde au suivant */
                }
            }
        }
    }

    return retour;
}

struct_competences * lire_struct_competences(char * chemin,char *** competencesl1,char *** competencesl2)
{
 char ** liste=NULL;
 struct_competences * retour=NULL;
 unsigned short i=0,y,x;
 signed short j;
 GMarkupDomNode * node, *ooo, *ooo1;

 if ((ooo1 = g_markup_dom_new (chemin, NULL)))
 {
     if ((ooo=g_markup_dom_node (ooo1,"table:table"))) /* pour la suite, il faut être dans une feuille : la première */
     {
         node=donne_case_ij(0,6,ooo);
         nb_groupe_competences_primaires=0;
         masque_compentence_primaire=0;
         liste=(char **)g_malloc(sizeof(char *));
         while(node && node->nb_texte>0)
         {
             masque_compentence_primaire=masque_compentence_primaire<<1;
             masque_compentence_primaire++;
             liste=(char **)g_realloc(liste,(nb_groupe_competences_primaires+2)*sizeof(char *));
             liste[nb_groupe_competences_primaires]=g_strdup(node->texte->texte);
             nb_groupe_competences_primaires++;
             node=donne_case_ij(0,6+nb_groupe_competences_primaires,ooo);
         }
         liste[nb_groupe_competences_primaires]=NULL;
         *competencesl1=liste;
         node=donne_case_ij(0,7+nb_groupe_competences_primaires,ooo);
         liste=(char **)g_malloc(sizeof(char *));
         i=0;
         masque_compentence_secondaire=0;
         while(node && node->nb_texte>0)
         {
             masque_compentence_secondaire=masque_compentence_secondaire<<1;
             masque_compentence_secondaire++;
             liste=(char **)g_realloc(liste,(i+2)*sizeof(char *));
             liste[i]=g_strdup(node->texte->texte);
             i++;
             node=donne_case_ij(0,7+nb_groupe_competences_primaires+i,ooo);
         }
         masque_compentence_secondaire=masque_compentence_secondaire<<nb_groupe_competences_primaires;
         liste=(char **)g_realloc(liste,(i+1)*sizeof(char *));
         liste[i]=NULL;
         *competencesl2=liste;
         /* k nb de style primaire de compétences et i nb de secondaires */

         node=donne_case_ij(1,1,ooo);
         x=0;
         while(node && node->nb_texte>0)
         {
            retour=(struct_competences *)g_realloc(retour,(x+1)*sizeof(struct_competences));
            retour[x].nom=g_strdup(node->texte->texte);

            node=donne_case_ij(x+1,2,ooo);
            if (node && node->nb_texte>0 )
            {
                j=sscanf(node->texte->texte,"%hu",&retour[x].unite);
                if (j!=1) retour[x].unite=1;
            }
            else
            {
                 retour[x].unite=10;
            }

            node=donne_case_ij(x+1,3,ooo);
            retour[x].caract=POURCENTAGE;
            if (node && node->nb_texte>0 )
            {
                for (j=0;j<POURCENTAGE;j++)
                {
                     if (compare_sans_casse(node->texte->texte,(char *)nom_min[j])==0)
                     {
                          retour[x].caract=j;
                     }
                     else
                     {
                     }
                }
            }

            node=donne_case_ij(x+1,4,ooo);
            if (node && node->nb_texte>0 )
            {
                j=sscanf(node->texte->texte,"%hu",&retour[x].modif);
                if (j!=1)
                {
                    retour[x].modif=0;
                }
                else
                {
                }
            }
            else
            {
                 retour[x].modif=0;
            }
            retour[x].classe=0;

            for (y=0;y<nb_groupe_competences_primaires;y++)
            {
                node=donne_case_ij(x+1,6+y,ooo);
                if (node && node->nb_texte>0 && node->texte->texte[0]=='1')
                {
                    retour[x].classe|=(1<<y);
                }
                else
                {
                }
            }

            for (y=0;y<i;y++)
            {
                node=donne_case_ij(x+1,7+y+nb_groupe_competences_primaires,ooo);
                if (node && node->nb_texte>0 && node->texte->texte[0]=='1')
                {
                    retour[x].classe|=(1<<(y+nb_groupe_competences_primaires));
                }
                else
                {
                }
            }

            x++;
            node=donne_case_ij(x+1,1,ooo);
         }
         retour=(struct_competences *)g_realloc(retour,(x+1)*sizeof(struct_competences));
         retour[x].nom=NULL;
         nb_competences=x;
     }
     else
     {
         printf("Erreur dans le fichier %s : pas de table:table\n",chemin);
     }

     g_markup_dom_free(ooo1);
 }
 else
 {
     printf("Erreur xml dans le fichier %s\n",chemin);
 }

 return retour;
}

signed char feuille_lire_multiclasse(char * coordonnees,GMarkupDomNode * node)
{
    signed char retour;
    if (compare_sans_casse(lecture_case(coordonnees,node),"oui")==0)
    {
        retour=1;
    }
    else
    {
        retour=0;
    }

    return retour;
}

void feuille_lire_modif_CA(signed short * modif_CA,char * coordonnees,GMarkupDomNode * node)
{

    signed long i,j,c;
    GMarkupDomNode * info;

    modif_CA[0]=0;
    modif_CA[1]=0;
    modif_CA[2]=0;
    modif_CA[3]=0;

    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
        i=i*26+*(coordonnees+j)-'A'+1;
    if (j!=0)
    {
        c=sscanf(coordonnees+j,"%ld",&j);
        if (c==1)
        {
            j--;

            info=donne_case_ij(j,i-1,node);
            if (info!=NULL)
            {
                sscanf(info->texte->texte,"%hd",modif_CA);
            }
            else
            {
            }
            info=donne_case_ij(j,i,node);
            if (info!=NULL)
            {
                sscanf(info->texte->texte,"%hd",modif_CA+1);
            }
            else
            {
            }
            info=donne_case_ij(j,i+1,node);
            if (info!=NULL)
            {
                sscanf(info->texte->texte,"%hd",modif_CA+2);
            }
            else
            {
            }
            info=donne_case_ij(j,i+2,node);
            if (info!=NULL)
            {
                sscanf(info->texte->texte,"%hd",modif_CA+3);
            }
            else
            {
            }
        }
    }
}

/* utilisation des variables globales du fichier */
signed short classe_entier(char * _nom)
{
    unsigned short i, retour=NON;

    if (_nom!=NULL)
    {
        for (i=0;i<nb_classe;i++)
        {
            if (compare_sans_casse(_nom,classe[i].nom)==0)
            {
                retour=i;
                i=nb_classe;
            }
            else
            {
            }
        }
    }
    else
    {
    }

    return retour;
}


signed short race_entier(char * _nom)
{
    unsigned short i, retour=NON;

    if (_nom!=NULL)
    {
        for (i=0;i<nb_race;i++)
        {
            if (compare_sans_casse(_nom,race[i].nom)==0)
            {
                retour=i;
                i=nb_race;
            }
            else
            {
            }
        }
    }
    else
    {
    }

    return retour;
}

signed short competence_entier(char * _nom)
{
    unsigned short i, retour=NON;

    if (_nom!=NULL)
    {
        for (i=0;i<nb_competences;i++)
        {
            if (compare_sans_casse(_nom,competences[i].nom)==0)
            {
                retour=i;
                i=nb_competences;
            }
            else
            {
            }
        }
    }
    else
    {
    }

    return retour;
}

signed short arme_entier(char * _nom)
{
    signed short i, retour=NON;

    if (_nom!=NULL)
    {
        for (i=0;i<nb_armes;i++)
        {
            if (compare_sans_casse(_nom,armes[i].nom)==0)
            {
                retour=i;
                i=nb_armes;
            }
            else
            {
            }
        }
    }
    else
    {
    }

    return retour;
}

signed short armure_entier(char * _nom)
{
    signed short i, retour=NON;

    if (_nom!=NULL)
    {
        for (i=0;i<nb_armures;i++)
        {
            if (compare_sans_casse(_nom,armures[i].nom)==0)
            {
                retour=i;
                i=nb_armures;
            }
            else
            {
            }
        }
    }
    else
    {
    }
    if (retour==NON)
    { /* correction suite à erreur dans 1er fichier armure */
        if (compare_sans_casse(_nom,"C, de mailles silencieuse")==0)
        {
            retour=armure_entier("C. de mailles silencieuse");
        }
        else if (compare_sans_casse(_nom,"C, de mailles Elfique")==0)
        {
            retour=armure_entier("C. de mailles Elfique");
        }
    }

    return retour;
}

struct_niv_classe * lecture_lim_racce_add2(GMarkupDomNode * node)
{ /* lecture pour ADD2 */
    unsigned long i,j,c,num_classe;
    GMarkupDomNode * info;
    struct_niv_classe * sortie=(struct_niv_classe *)g_malloc(nb_classe*sizeof(struct_niv_classe));
    char * ch, *pt;
    char * coordonnees=FEUILLE_CLASSE;
    unsigned char ok=1;
    unsigned short tmp,nb;

    for (i=0;i<nb_classe;i++)
    {
        sortie[i]._2[0]=-1;
        sortie[i]._2[1]=-1;
    }
    for (i=0,j=0;*(coordonnees+j)>='A' && *(coordonnees+j)<='Z';j++)
    {
         i=i*26+*(coordonnees+j)-'A'+1;
    }
    if (j==0)
    {
        printf("Surprise à la lecture de LECTURE_CLASSE : %s : pas de vraies coordonnées\n",FEUILLE_CLASSE);
    }
    else
    {
        c=sscanf(coordonnees+j,"%lu",&j);
        if (c!=1)
        {
            printf("Surprise à la lecture de LECTURE_CLASSE : %s : pas de vraies coordonnées\n",FEUILLE_CLASSE);
        } /* passage des coordonnées humaines aux coordonnées informatiques */
        else
        {
            for(c=0;(info=donne_case_ij(j-1,i-1+c,node)) && info->nb_texte!=0;c++)
            {   /* tant qu'il y a des cases, on lit le contenu */
                ch=info->texte->texte;
                ok=1;
                for (pt=ch;*pt!=0;pt++)
                {
                    if (*pt=='/')
                    {
                        ok=0; /*c'est une classe multiple */
                    }
                    else
                    {
                    }
                }
                if (ok==1)
                {
                    for(num_classe=0;num_classe<nb_classe;num_classe++)
                    {
                      if (compare_sans_casse(classe[num_classe].nom,ch)==0)
                      { /* on a trouvé la classe */
                          info=donne_case_ij(j-2,i-1+c,node);
                          if (info==NULL || info->nb_texte==0)
                          { /* rien d'écrit => pas de limite */
                            sortie[num_classe]._2[0]=0;
                          }
                          else
                          {
                              ch=info->texte->texte;
                              if (sscanf(ch,"%hu",&tmp))
                              { /* c'est une classe avec limite */
                                  sortie[num_classe]._2[0]=tmp;
                                  sortie[num_classe]._2[1]=0;
                                  for (nb=0;ch[nb]!=0;nb++)
                                  {
                                      switch (ch[nb])
                                      {
                                          case '0' :
                                          case '1' :
                                          case '2' :
                                          case '3' :
                                          case '4' :
                                          case '5' :
                                          case '6' :
                                          case '7' :
                                          case '8' : /* chiffres déjà pris en compte */
                                          case '9' : break;
                                          case 'm' : sortie[num_classe]._2[1]|=ADD2_NIV_MAX_DEMI_HUM;
                                            break;
                                          case 'h' : sortie[num_classe]._2[1]|=ADD2_NIV_MAX_HUMANOIDE;
                                            break;
                                          case '*' : sortie[num_classe]._2[1]|=ADD2_NIV_MAX_MONO;
                                            break;
                                          case 's' : sortie[num_classe]._2[1]|=ADD2_NIV_MAX_SAURIAL;
                                            break;
                                          default  : printf("La chaîne de niveau max %s comprend un caractère non pris en compte\n",ch);
                                      }
                                  }
                              }
                              else
                              {
                                  /* chaine nulle => pas de limite */
                                  sortie[num_classe]._2[0]=0;
                                  sortie[num_classe]._2[1]=0; /* aussi par précaution */
                              }
                          }
                          num_classe=nb_classe; /* on force la sortie de la boucle */
                      }
                      else
                      {
                      }
                    }

                }
                else
                {
                    /* on ne prend pas en compte les classes multiples */
                }
            }
        }
    }

    return sortie;
}

struct_niv_classe * lecture_lim_racce_add1(GMarkupDomNode * node)
{ /* lecture pour ADD1 */
    unsigned long i,j,c,num_classe;
    GMarkupDomNode * info,* node_lim=g_markup_dom_nom (node,"lim_ADD1","table:name");
    struct_niv_classe * sortie=(struct_niv_classe *)g_malloc(nb_classe*sizeof(struct_niv_classe));
    char * ch,car;
    unsigned short tmp,nb,tmp1;


    for (i=0;i<nb_classe;i++)
    {
        sortie[i]._1=NULL;
    }
    if (node_lim==NULL)
    {
        node_lim=g_markup_dom_nom (node,"lim ADD1","table:name");
    }
    else
    {
        /* c'est la seconde chance */
    }
    if (node_lim==NULL)
    {
        g_free(sortie);
        sortie=NULL; /* aucune lettre devant */
    }
    else
    {
        j=1;
        info=donne_case_ij(0,0,node_lim);
        while (info!=NULL && info->nb_texte!=0)
        {
            ch=info->texte->texte;
            for(num_classe=0;num_classe<nb_classe;num_classe++)
            {
                if (compare_sans_casse(classe[num_classe].nom,ch)==0)
                { /* on a trouvé la classe */
                    i=2;
                    info=donne_case_ij(2,j-1,node_lim);
                    while (info!=NULL && info->nb_texte!=0)
                    {
                        i++;
                        info=donne_case_ij(i,j-1,node_lim);
                    } /* retourne la dernière case avec un max */
                    if (i==2)
                    {
                        /*if (j==1) return NULL;*/ /* retour NULL pour la phase de test et repérer les fichiers à problème */
                    }
                    else
                    {
                        c=i;
                        sortie[num_classe]._1=(signed char **)g_malloc((c-1)*sizeof(char *));
                        sortie[num_classe]._1[c-2]=NULL; /* on marque le dernier */
                        while (i>2)
                        {
                            info=donne_case_ij(i-1,j-1,node_lim); /* récupération du niveau */
                            sortie[num_classe]._1[c-i]=(signed char *)g_malloc(8*sizeof(char)); /* 0 à 5 => carart, 6% et 7 niv */
                            sscanf(info->texte->texte,"%hd",&tmp); /* info->texte->texte a été testé lors de la bouclz précédente */
                            sortie[num_classe]._1[c-i][7]=(char)tmp; /* on passe par tmp car sortie[num_classe]._1[c-i][7] est un char */
                            info=donne_case_ij(i-1,j,node_lim); /* de la force */
                            if (info!=NULL && info->nb_texte!=0)
                            {
                                nb =sscanf(info->texte->texte,"%hd%c%hd",&tmp,&car,&tmp1); /* on passe par cet intermédiaire car les %hd ont des problèmes avec les char */
                                if (nb<3 || (car!='-' && car!='/')) /* normalement, car fait - ou / */
                                {
                                    sortie[num_classe]._1[c-i][6]=0;
                                }
                                else
                                {   /* l'écriture 18/00 signifie 18 100%*/
                                    sortie[num_classe]._1[c-i][6]=tmp1;
                                }
                                if (nb>0)
                                {
                                    sortie[num_classe]._1[c-i][0]=tmp;
                                }
                                else
                                {
                                    sortie[num_classe]._1[c-i][0]=0;
                                }
                            }
                            else
                            {
                                sortie[num_classe]._1[c-i][0]=0;
                                sortie[num_classe]._1[c-i][6]=0;
                            }
                            for (tmp1=1;tmp1<6;tmp1++)
                            { /* de l'int au ch */
                                info=donne_case_ij(i-1,j+tmp1,node_lim);
                                if (info!=NULL && info->nb_texte!=0)
                                {
                                   if (sscanf(info->texte->texte,"%hd",&tmp)==1)
                                   {
                                       sortie[num_classe]._1[c-i][tmp1]=tmp;
                                   }
                                   else
                                   {
                                       sortie[num_classe]._1[c-i][tmp1]=0;
                                   }
                                }
                                else
                                {
                                    sortie[num_classe]._1[c-i][tmp1]=0;
                                }
                            }
                            i--;
                        }
                    }
                    num_classe=nb_classe; /* forcer la sortie */
                }
                else
                {
                    /* pas la bonne classe : on passe à la suivante */
                }
            }

            j+=7; /* on passe à la classe suivante */
            info=donne_case_ij(0,j-1,node_lim);
        }
    }

    return sortie;
}
