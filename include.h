#ifndef INCLUDE_H_INCLUDED
#define INCLUDE_H_INCLUDED

#include <sys/types.h> /* pour le répertoire vers l'exécutable */

#ifdef __linux__
#define HOME     "HOME"
#define MEM_PROG "HOME"
#define TMP      "TMP"
#define SEPARATEUR    '/'
#else
#include <unistd.h>
#include <time.h>
#include <windows.h> /* pour le répertoire vers l'exécutable */
#include <process.h>
#include <direct.h>

#define HOME     "HOMEPATH"
#define MEM_PROG "APPDATA"
#define TMP      "TMP"

#define mkdir(a,b) _mkdir(a)
#define SEPARATEUR    '\\'
/* compatibilité de fonctions */
#define getpid _getpid
#define getcwd _getcwd
#define chdir _chdir

#endif


#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <glib/gstdio.h>


/* mémorisation du nombre maximum de fichier en pers et en grp mémoriser pour le menu */
#define MEM_FICH_MAX   8


#ifndef LONG
#define LONG 2048
#endif
#ifndef max
#define max(i,j) (i>j?i:j)
#endif
#ifndef _min
#define _min(i,j) (i<j?i:j)
#endif

#define ADD1 0
#define ADD  0
#define ADD2 1 /* 0 ou 1 => !version donne l'autre */

#define MASCULIN 0
#define FEMININ  1

#define NON -99
#define FIN -999

/* couleur */
#define GRIS      0
#define ROUGE     1
#define VERT      2
#define VIOLET    3
#define BLEU      4
#define JAUNE     5
#define GRISCLAIR 6
#define GRISFONCE 7
#define ORANGE    8
#define NOIR      9

/* alignement : puissance de 2 */
#define BL 1
#define BN 2
#define BC 4
#define NL 8
#define NN 16
#define NC 32
#define ML 64
#define MN 128
#define MC 256

#define FORCE            0
#define INTELLIGENCE     1
#define SAGESSE          2
#define DEXTERITE        3
#define CONSTITUTION     4
#define CHARISME         5
#define BEAUTE           6
#define HUMOUR           7
#define POURCENTAGE      8

#define FEUILLE_LAR_VOL     17

#define PAS_REGARDE 3
#define COMPRIS     1
#define PAS_COMPRIS 2

#define nom_sexe(i) i==0?"Masculin":"Féminin"
#define maitrise_arme(i) i==0?"Non maitrisée":(i==1?"Maitrisée":(i==2?"Spécialisation":"Double spécialisation"))


struct widgets
{
  GtkWidget * window;
  GtkBuilder *builder;
};

typedef struct
{
  char * nom;
  unsigned long majeure;
  unsigned long mineure;
  char * symbole;
  char * commentaire;
  char * vd;
  signed short vd_delta;
  signed short alignement;
  unsigned short ori;
  unsigned short pc_nb;
  unsigned short * pc_niv;
  char ** pc;
} dieux;

typedef struct
{
  char * nom;
  dieux * dieu;
  unsigned short ori;
} pant;

typedef struct
{
  char * nom; /* nom de la classe */
  signed short ** nb;
} so_add;

typedef struct
{
  char * type; /* clerc, druide, mago ou druide */
  so_add add[2];

} struct_sort;


typedef struct
{
  signed long clef;
  char * nom;
  char * portee;
  char * vsm;
  char * duree;
  char * lance;
  char * effet;
  char * save;
  char * composant;
  unsigned short id_dnd;
  signed char reversible;
  unsigned long sphere;
  unsigned long long source;
} struct_sortileges;

typedef struct
{
    signed long clef;
    char * nom;
} struct_sphere;

struct sv_add
{
  char * nom;
  unsigned short ** save;
};

typedef struct
{
  char * nom;
  struct sv_add add[2];
} struct_save;

struct psi_add1
{
    char * nom;
};

struct psi_add2 /* pour les classes de personnages */
{
    char * nom;
    unsigned short base[26];
    unsigned short bonus[26];

    struct
    {
        signed short * base; /* le n° de la caractéristique ou un nombre en négatif */
        signed short * bonus; /* le n° de la caractéristique ou un nombre en négatif */
    } debut;
    struct
    {
        signed short * base; /* le n° de la caractéristique ou un nombre en négatif */
        signed short * bonus; /* le n° de la caractéristique ou un nombre en négatif */
    } niveau;

    signed short * discipline;
    signed short * devotion;
    signed short * science;
    signed short * defense;
};

typedef struct
{
  struct psi_add1 add1;
  struct psi_add2 add2;
} struct_psi_c;

typedef struct _struct_psi struct_psi;

struct _struct_psi /* pour la liste de psi disponible */
{
  unsigned short clef;
  unsigned short defense;
  char * nom;
  char * portee;
  char * duree;
  char * lance;
  char * zone;
  unsigned short caracteristique;
  signed short modif;
  char * cout;
  unsigned short niveau;
  unsigned short * prerequis;
  struct_psi ** prerequis_lien;
};

typedef struct st_ar struct_armes;

struct st_ar
{
  gchar * nom;
  unsigned short longueur;
  signed short vitesse;
  gchar PM[8];
  gchar G[8];
  unsigned short groupe ;
  signed short * pt_groupe;
};

typedef struct
{
  gchar * nom;
  signed short ac;

  signed short add[2][16];
} struct_armure;


typedef struct
{
    gchar * nom;
    signed short clef;
    gchar * court;
} struct_origine;


struct cl_add
{
  char * nom;
  signed short origine;
  signed short min_caract[9];
  signed short max_caract[9];
  signed short dix_caract[9];
  signed char vade_retro;
  signed short * ac;
  signed short * backstab;
  char * pdv[20];
  signed short * armes;
  signed char nb_arme[4];
  char ** armures;
  char ** bouclier;
  char ** special;
  char * voleur;
  signed char ** compt_spe[2];
  signed short ** tab_voleur;
  unsigned short alignement;
  signed long * XP;
  struct sv_add * save;
  union
  {
    signed short * _1[2];
    unsigned short _2[3];
  } tacO;

  so_add * sort_mago;
  so_add * sort_illus;
  unsigned long sort_mago_ecole;
  unsigned long sort_mago_ecole_spe;
  so_add * sort_clerc;
  char bonus_clerc;
  so_add * sort_druide;
  char bonus_druide;
  char ** sort_clerc_sphere;
  unsigned long liste_competences;
  unsigned short progression_competences[2];
  union
  {
   struct psi_add2 * _2;
   struct psi_add1 * _1;
  }	psi;
  signed short init[2];
  gchar * pantheon; /* on ne peut mettre un entier car on peut rajouter des items en cours d'utilisation */
  gchar * dieu;
};

typedef struct
{
  gchar * nom;
  unsigned short unite;
  unsigned short caract;
  signed short modif;
  signed long classe;
} struct_competences;

struct bonus_armes
{
  char * arme;
  signed short bonus;
};

typedef struct
{
  struct cl_add add[2];
  char * nom;
} struct_classe;

typedef struct str_l_cl
{
  char * nom;
  unsigned short num; /* le numéro de la classe */
  struct str_l_cl * suiv;
  unsigned short niveau;
  unsigned long xp;
} liste_classe;

typedef struct str_l_cl_r
{
  liste_classe * classe;
  char * age;
  struct str_l_cl_r * suiv;
} liste_classe_race;

typedef struct
{
  union
  {

    signed char ** _1; /* dimention 1 : un max, dimension 2 le niveau max + les caract correspondantes */
    signed char _2[2]; /* case 1 le max, case 2 : option voir les DD2_NIV_MAX_ */
  };
} struct_niv_classe;
/* cas normal */
#define ADD2_NIV_MAX_MAITRE     0
/* cas humanoïde : manuel complet des humanoides */
#define ADD2_NIV_MAX_HUMANOIDE  1
/* cas 1/2 orc/ogre humanoïde : manuel complet des humanoides */
#define ADD2_NIV_MAX_DEMI_HUM   2
/* pas de limite si mono_classé : manuel complet des humanoides */
#define ADD2_NIV_MAX_MONO       4
/* cas particulier du saurial  : +1 au niv si le saurial à +10% en XP */
#define ADD2_NIV_MAX_SAURIAL    8



struct ra_add
{
  char * nom;
  signed short origine;
  signed short min_caract_h[9];
  signed short max_caract_h[9];
  signed short min_caract_f[9];
  signed short max_caract_f[9];
  signed short modif_caract[9];
  liste_classe_race * liste_classe;
  unsigned char * limite;
  signed short pdv;
  signed char multiclasse;
  signed short * tranche_age;
  char * vision;
  char* ecoute;
  char * resistance;
  signed short CA;
  signed short modif_CA[4];
  struct bonus_armes * bonus_arme;
  char * save[5];
  char ** langue;
  char ** detection;
  char * surprendre;
  char * sefairesurprendre;
  char ** competence;
  signed short ** tab_voleur;
  float xp;
  unsigned short taille[2];
  char * var_taille[2];
  float poids[2];
  struct_niv_classe * niv;
};

typedef struct
{
  char * nom;
  struct ra_add add[2];
} struct_race;

typedef struct
{
    signed short caract[9];
    signed short race;
    signed short * classe;
    signed short * niveau;
    unsigned long * XP;
    signed short version;
    signed short sexe;
    unsigned short age;
    gchar * nom;
    gchar * joueur;
    gchar * nom_fichier;
    signed short * pdv; /* pouvoir passer par des nombres négatifs dans les calculs et marquer par 0 ou -1 la fin de la liste */
    signed short tab_voleur[FEUILLE_LAR_VOL];
    signed short * competence;
    signed short * niv_competence;
    signed short * arme;
    signed short * niv_arme;
    signed short * mag_arme;
    signed short * arme_autre_t;
    signed short * arme_autre_d;
    signed short alignement;
    gchar * cheveux;
    gchar * peau;
    gchar * yeux;
    signed short taille;
    signed short poids;
    signed short * bouclier; /* pointe sur NULL si pas de bouclier, sinon pointe sur un entier : mag du bouclier */
    signed short * bague;    /* pointe sur NULL si pas de bague   , sinon pointe sur un entier : mag de la bague */
    signed short autre_ca;
    signed short autre_save;
    signed short armure1;
    signed short mag_armure1;
    signed short armure2;
    signed short mag_armure2;
    gchar * commentaire;
    gchar * pantheon; /* on ne peut mettre un entier car on peut rajouter des items en cours d'utilisation */
    gchar * dieu;
    signed long ** livre_sorts;
    signed short ** sort_su; /* négatif si pas dans le livre de voyage, 0 si pas du tout, 1 pas regardé, 2 compris, 3 pas compris : voir # define */
    signed short * psi;
    signed short * niv_psi;
    signed short * natif_psi;
    char * sociale;
    char cap;
    char ** origine;
    char ori_add1_add2;
} perso;

struct _FenetrePerso
{
    struct widgets * resume;    /* gestion de la fenêtre résumée                */
    struct widgets * modif;     /* gestion de la fenêtre de modifications       */
    perso perso;
    char ** uri;    /* NULL pour un perso, liste des persos pour un groupe      */
    unsigned short d_m; /* memorisation d'une modification dans modif nos sauvegardée dans résume */
    unsigned short d_r; /* mémorisation d'une modification dans resume non sauvegardée */
    unsigned short * couleur_armes;
    unsigned short * couleur_races;
    unsigned short * couleur_classes;
    unsigned short * couleur_armure;
    unsigned short * couleur_competence;
    signed short * classe_modif; /* -1 pour le dernier */
    signed short * armes_modif; /* -1 pour le dernier */
    signed short * competence_modif;
    signed short * niv_classe_modif;
    unsigned short caract_modif[POURCENTAGE+1];
    signed short version_modif;
    unsigned short sexe_modif;
    signed short race_modif;
    signed short mag; /* 0 rien d'affiché, 1 à 9 : le niveau affiché */
    char psi; /* 0 pas affiché, 1 affiché */
    char ** origine;
    char ori_add1_add2;
    unsigned short * combo_races;
    unsigned short * combo_classes;
};

typedef struct _FenetrePerso FenetrePerso;


typedef struct
{
    FenetrePerso * groupe;
    char * nom;
    GtkWidget * wid;
} perso_groupe;

typedef struct
{
    FenetrePerso * _perso;
    signed short info;
    struct widgets * modif;
} aide;

struct callback_nv_nom
{
    FenetrePerso * fenetre_perso;
    struct widgets * widgets_fichier;
};


extern const char Lettre1_caract[];
extern const char * Nom_Caract_Sans_Accent[];
extern const char * Nom_Caract_Avec_Accent[];

  /* définition dans le fichier tableaux.c */
extern const gchar * vers[];
extern const gchar * init[];
extern const gchar * nom[];
extern const gchar * nom_min[];
extern const gchar * nom_abr[];
extern const gchar algnm[9][4];
extern const gchar * Tirages[];
extern const gchar * nom_sexe[];
extern const signed short niveau_RN[];
extern const gchar * nom_fac_vol[];
extern const gchar * nom_fac_voleur[];
extern const signed short fac_vol_caract[11][8];
extern const gchar vd_add1[13][14][4];
extern const gchar vd_add2[13][14][4];
extern const gchar tabl_constit[25][5][8];
extern const gchar tabl_inte1[25][5][8];
extern const gchar tabl_inte2[25][5][8];
extern const gchar tabl_sagesse[25][11][128];
extern const gchar tabl_dexterite[2][25][3][4];
extern const unsigned short c_poids_taille[80][2];
extern const unsigned short taille_race[7][4];
extern const float poids_race[7];
extern const signed short v_poids_taille[9];
extern const float force_taille[16];
extern const float force_poids[16];
extern const gint hiatus[9][20];
extern const signed short add_niv_max_maitre[26];
extern const signed short add_niv_max_saurial[26];
extern const signed short add_niv_max_humanoide[4];
extern const signed short add_niv_max_demi_hum[4];

extern unsigned long entropie;
extern unsigned short nb_ecole;

/* lecture_fichier.c */

void lecture_fichiers(char * ici);

const struct_classe * donne_classe();
#define CLASSE donne_classe()
unsigned short donne_nb_classe();
#define NB_CLASSE donne_nb_classe()
unsigned short donne_nb_psi(); /* nombre de classe utilisant les psi */
#define NB_PSI donne_nb_psi()
const struct_psi_c * donne_psi(); /* classe utilisant les psi */
#define PSI donne_psi()
const struct_save * donne_save();
#define SAVE donne_save
unsigned short donne_nb_save();
#define NB_SAVE donne_nb_save
const struct_sort * donne_sort();
#define SORT donne_sort()
unsigned short donne_nb_sort(); /* nombre de tables de lanceurs de sorts */
#define NB_SORT donne_nb_sort()
const struct_armes * donne_armes();
#define ARME donne_armes()
unsigned short donne_nb_arme();
#define NB_ARME donne_nb_arme()
const struct_armure * donne_armure();
#define ARMURE donne_armure()
unsigned short donne_nb_armure();
#define NB_ARMURE donne_nb_armure()
unsigned short donne_nb_race();
#define NB_RACE donne_nb_race()
const struct_race * donne_race();
#define RACE donne_race()
const struct_competences * donne_competences();
#define COMPETENCE donne_competences()
unsigned short donne_nb_competence();
#define NB_COMPETENCE donne_nb_competence()
unsigned short donne_nb_competence_primaire();
#define NB_GR_COMPETENCE_PRIMAIRE donne_nb_competence()
unsigned short donne_masque_compentence_secondaire();
#define MASQUE_COMPETENCE_SECONDAIRE donne_masque_compentence_secondaire()
unsigned short donne_masque_compentence_primaire();
#define MASQUE_COMPETENCE_PRIMAIRE donne_masque_compentence_primaire()
const struct_origine ** donne_origine();
#define _ORIGINE donne_origine()
unsigned short * nb_origine();
#define NB_ORIGINE nb_origine()

signed short race_entier(char * nom);
signed short classe_entier(char * nom);
signed short competence_entier(char * nom);
signed short arme_entier(char * nom);
signed short armure_entier(char * nom);


/* gestion_glade.c */
#define GLADE_CONNECT 1
#define GLADE_NON_CONNECT 0
void get_widgets(struct widgets *widgets);
void ouverture_glade(char * nom, unsigned short connect);
struct widgets * ouverture_glade_retour(char * nom, unsigned short connect); /* paramètre GLADE_?  */
void ferme_glade(GtkWidget *ChildWidget , struct widgets * widgets);

/* chaine.c */
signed char compare_sans_casse(char * s1,char * s2);
int str_isspace (const gchar *s);
char * simplifie_uri(char *nom);

/* menu.c */
void init_race(struct widgets * widgets);
void Change_Pref (GtkMenuItem *menuitem, struct widgets *widgets);
signed short trouve_race(struct widgets *widgets);
void complete_menu (GtkMenuItem *menuitem, struct widgets *widgets);
void FeuilleDeChoix(GtkMenuItem *menuitem, struct widgets * _widgets);
signed short * trouve_classe(struct widgets *widgets);
void callback_ouvre_recent(GtkMenuItem *menuitem, struct widgets * widgets);
void callback_aff_menu_recent(GtkMenuItem *menuitem, struct widgets * widgets);


/* messages.c */
void GPL (GtkMenuItem *menuitem, struct widgets *widgets);
void Explication  (GtkMenuItem *menuitem, struct widgets * _widgets);
void traite_erreur(GError *error);
unsigned short demande_oui_non(char * message);
void donne_info(char * message);
void dialogue(char * ch,signed short i);


/* dieux.c */
pant * genere_pantheon(char * fichier);
#define PANTHEONS donne_pantheons()
pant * donne_pantheons();
void Modif_dieu (GtkWidget *ChildWidget, struct widgets * _widgets);
signed short pantheon_entier(char * nom); /* renvoie le numéro du panthéon ou l'opposé du nombre de panthéon s'il le panthéon n'existe pas */
signed short dieu_entier(char * nom,signed short panth); /* renvoie le numéro du dieu ou l'opposé du nombre de dieux dans le panthéon si le dieu n'est pas trouvé */
GtkComboBoxText * creation_combo_pantheons(void);
GtkComboBoxText * creation_combo_dieux(signed short panth);


/* lecture des fichiers xml.c */
typedef struct _xml_attr xml_attr;
typedef struct _GMarkupDomNode GMarkupDomNode;
typedef struct _GMarkupDomContext GMarkupDomContext;

typedef struct
{
    gchar * texte;
    guint    item;
} contenu;

struct _GMarkupDomNode
{
  gchar *nom;
  guint item; /* numéro de l'item dans la liste des items du père */
  guint niveau;
  contenu * texte;/* tableau d’entrée texte */
  guint nb_texte; /* nombre d'entrées texte */
  xml_attr *attributs; /* tableau d'attributs */
  guint nb_att; /* nombre d'attributs */
  contenu * com; /* tableau de commentaires */
  guint nb_com; /* nombre de commentaires */
  struct _GMarkupDomNode *parent;
  struct _GMarkupDomNode *fils;
  guint nb_fils; /* nombre de nodes fille */
};

struct _GMarkupDomContext
{
  guint niveau;
  guint item;
  GMarkupDomNode *root;
  GMarkupDomNode *current;
};

struct _xml_attr
{
    gchar *nom;
    gchar *value;
};

void g_markup_dom_free (GMarkupDomNode *);
GMarkupDomNode *g_markup_dom_new (const gchar *filename, GError **error);
void dom_print (GMarkupDomNode *root,FILE * sortie,gint pos);
void enregistre_xml(char * nom, GMarkupDomNode * ooo); /* génère le fichier xml */
FILE * enregistre_tmp_xml(GMarkupDomNode * ooo, char ** nom_de_fichier);
GMarkupDomNode * g_markup_dom_nom (GMarkupDomNode *node,const gchar * nom,gchar * attr); /* trouve une node en fonction d'une valeur particulière d'un attribut particulier */
GMarkupDomNode * g_markup_dom_node (GMarkupDomNode *node,const gchar * nom_val); /* trouve une node à partir du nom de la balise */
void copie_node(GMarkupDomNode * arrive,GMarkupDomNode * modele); /* copie une node modèle sur une autre node */
GMarkupDomNode * xml_ajoute_fin(GMarkupDomNode * node,const gchar * texte); /* mettre une node fille en fin d'une node, le nom de la node étant indiqué par le texte */
void xml_ajoute_fin_texte(GMarkupDomNode * node); /* rajouter un élément texte en fin d'une node */
void xml_ecrit_dernier_texte(GMarkupDomNode * node,const gchar * ch); /* remplace le dernier texte d'une node ou crée un élément texte s'il n'y en a pas */
void xml_ajoute_fin_attribut(GMarkupDomNode * node); /* rajouter un élément attribut */
unsigned char xml_ecrit_dernier_attribut(GMarkupDomNode * node,char * nom, char * val); /* remplace le dernier attribut : renvoie le nb d'attributs modifié */
unsigned char xml_ecrit_attribut(GMarkupDomNode * node,char * nom, char * val);  /* remplace la valeur de l'attribut nomm par val */
void decale_node(GMarkupDomNode *node,unsigned short i); /* decale les node pour mettre la dernière node à la place i */
void supprime_node(GMarkupDomNode * pere,guint place); /* supprime la node place de pere */
GMarkupDomNode * ajoute_node(GMarkupDomNode * pere,guint i,char * texte);  /* ajoute la ième node                           */
void xml_sup_node(GMarkupDomNode *node); /* supprime la node passée en paramètre */
void modif_xml(GMarkupDomNode *item,const gchar * ch);
unsigned char echange_node(GMarkupDomNode *node,GMarkupDomNode *node1);
void ecrit_xml(FILE * sortie, char * ch); /* prise en compte des caractères spéciaux du xml */

/* sorts.c */
void lire_sorts(char * ici);
void aff_nb_sort(GMarkupDomNode * node,struct_classe * s_c,signed short * clerc,signed short * mag,signed short sage,signed short inte,unsigned short niv,signed short version_);
void affiche_sort_pretre(char * pantheon,char * dieu,GMarkupDomNode * ooo,unsigned short _cl,char ** origine);

#define DISCIPLINE_PSI donne_discipline_psi()
struct_sphere * donne_discipline_psi (void);
#define TALENTS_PSI donne_talents_psi()
struct_psi *** donne_talents_psi(void);
#define NB_TALENTS_PSI donne_nb_talent_psi()
unsigned short ** donne_nb_talent_psi(void);
#define NB_DISCIPLINE_PSI donne_nb_discipline_psi()
unsigned short donne_nb_discipline_psi(void);
#define SORTILEGE_MAG   donne_sortileges_mag()
struct_sortileges ***   donne_sortileges_mag(void); /* tableau 3 dimensions : */
#define NB_SORT_CLERC donne_nb_sort_clerc()
struct_sortileges ***   donne_sortileges_clerc(void); /* version , niveau, num */
#define SORTILEGE_CLERC donne_sortileges_clerc()
unsigned short ** donne_nb_sort_clerc(void);
#define SPHERE donne_sphere()
struct_sphere ** donne_sphere(void);
#define NB_SORT_MAG donne_nb_sort_mag()
unsigned short ** donne_nb_sort_mag(void);
#define ECOLE donne_ecole()
struct_sphere ** donne_ecole(void);
#define NB_ECOLE donne_nb_ecole()
unsigned short donne_nb_ecole(void);
#define NB_SPHERE donne_nb_sphere()
unsigned short donne_nb_sphere(void);
signed short sort_entier(char * nom,signed short niveau,signed short version);
unsigned short affiche_sort_mag(unsigned short niveau,FenetrePerso * _perso,GMarkupDomNode * ooo, unsigned short * pages_voy, unsigned short * pages_t);
void init_liste_sort();
void init_liste_mag();
signed char dans_source(unsigned long long source,char ** origine);

/* style.c */
void style (GtkWidget *ChildWidget, cairo_t *cr,double rouge, double vert, double bleu);
void bord (GtkWidget *ChildWidget, cairo_t *cr);
gboolean style1(GtkWidget *ChildWidget, cairo_t *cr);
gboolean style2(GtkWidget *ChildWidget, cairo_t *cr);
gboolean style3(GtkWidget *ChildWidget, cairo_t *cr);
gboolean style4(GtkWidget *ChildWidget, cairo_t *cr);
gboolean style5(GtkWidget *ChildWidget, cairo_t *cr);
gboolean style_race(GtkWidget *ChildWidget, cairo_t *cr,struct widgets *widgets);
gboolean style_classe(GtkWidget *ChildWidget, cairo_t *cr,struct widgets *widgets);
void couleur_item(GtkWidget *ChildWidget,signed short couleur);

/* envp.c */
char * donne_chemin_dieux_perso(void);
char * chemin_exe(void);
void envp_init();
char * envp_donne_home(void);
char * envp_app_data();
void envp_mem_home_rep(char * chemin);
void envp_mem_hist(char * chemin);
void envp_mem_hist_supp(char * chemin);
/*char * rep_tmp();*/
char * windows_home(char * chemin);
/* void  menu_va_hist(GtkWidget *wid, gpointer * param); */
GtkWidget * deja_edite(char * nom_de_fichier);
char * chemin_absolu(char * chemin);

/* lire_odc.c */
GMarkupDomNode * donne_case_ij(unsigned long i, unsigned long j,GMarkupDomNode * ooo); /* la première case est la case (0,0) */
char * lecture_case(char * coordonnees,GMarkupDomNode * ooo);
GMarkupDomNode * donne_case_A1(char * coordonnees,GMarkupDomNode * ooo);
signed short feuille_lire_case_m(char * coord,GMarkupDomNode * node,signed short min);
signed short feuille_lire_case_d(char * coord,GMarkupDomNode * node,signed short defaut);
signed short feuille_lire_case_M(char * coord,GMarkupDomNode * node,signed short max);
float        feuille_lire_case_f(char * coord,GMarkupDomNode * node,float        min);
float        feuille_lire_case_f_d(char * coord,GMarkupDomNode * node,float   defaut);
signed short * feuille_lire_ligne_nb(char * coordonnees,GMarkupDomNode * node);
float donne_float_ij(unsigned long i, unsigned long j,GMarkupDomNode * ooo);
float donne_float_A1(char * coordonnees,GMarkupDomNode * ooo);
signed long * feuille_lire_colonne_nbl(char * coordonnees,GMarkupDomNode * node);
void feuille_lire_colonne_char(char * coordonnees,GMarkupDomNode * node,signed short * tab,unsigned short longueur,signed char defaut);
void feuille_lire_colonne_char_pourcentage(char * coordonnees,GMarkupDomNode * node,signed short * tab,unsigned short longueur,signed char defaut);
char * lecture_case_up(char * coordonnees,GMarkupDomNode * ooo); /* lecture case avec réservation de la mémoire */
char * lecture_case_up_ij(unsigned long i, unsigned long j,GMarkupDomNode * ooo); /* lecture case avec réservation de la mémoire */

/*utile.c */
signed short est_compatible(signed short nb_race,signed short nb_classe,signed short version);
signed short est_compatible_armure(signed short nb_classe,signed short nb_armure,signed short version);
signed short est_multi_compatible(signed short nb_race,signed short * nb_classe,signed short version);
GtkWidget * gtk_get_widget_by_name(GtkContainer * pere,char * nom);
char * virgule_point(char * ch);
int copie_fichier(char const *path_dest, char const *path_src);
GtkWidget * gtk_get_widget_enfants(GtkWidget * pere,unsigned short niveau);
void vide_widget(GtkWidget *appel);
gchar * nb_texte(gchar * ch,unsigned short nb);
unsigned short place_max(unsigned short * tab, unsigned short taille);
void g_fclose(FILE * ptf);
signed short meme_extension(char * _nom,char * ext);

/* zip.c */
char * content_libo(char * fichierZIP,size_t * longueur);

/* tirage.c */
void applique_menu(GtkWidget *wid, struct widgets * param);

/* ouverture.c */
void ouverture(GtkWidget *wid_appel, struct widgets * widgets);
void callback_ouvre_perso(GtkButton *button, char * ch);
FenetrePerso * ouvre_perso(char * nom);
void info_fichier(GtkFileChooser *chooser,  struct widgets * _widgets);

/* groupe.c*/
void enregistre_groupe(GtkWidget *ChildWidget, FenetrePerso * fenetre);
void callback_suppr_perso_groupe(GtkButton *button, perso_groupe * p_gr);
signed short nouveau_membre_groupe(FenetrePerso * fenetre_perso, char * nom,unsigned short nb);

/* gestion_fenetre_perso.c */
FenetrePerso * gestion_fenetre (struct widgets * widgets, char * chemin,FenetrePerso * fermeture);
void affiche_resume(FenetrePerso * perso);
void ferme_fenetre_perso(GtkWidget *wid, FenetrePerso * widgets);
void nouveau(GtkWidget *wid, struct widgets * param);


/* modif_perso */
void modif_perso(GtkWidget *wid, FenetrePerso * perso);
void modif_caract(GtkWidget *appel, FenetrePerso * _perso);
void modif_race(GtkWidget *appel, FenetrePerso * _perso);
void modif_version(GtkWidget *appel, FenetrePerso * _perso);
void modif_sexe(GtkWidget *appel, FenetrePerso * _perso);
void modif_classe(GtkWidget *appel, FenetrePerso * _perso);
void modif_pdv(GtkWidget *appel, FenetrePerso * _perso);
void modif_XP(GtkWidget *appel, FenetrePerso * _perso);
void modif_niv(GtkWidget *appel, FenetrePerso * _perso);
void modif_arme(GtkWidget *appel, FenetrePerso * _perso);
void modif_dieu(GtkWidget *appel, FenetrePerso * _perso);
void modif_pantheon(GtkWidget *appel, FenetrePerso * _perso);
void modif_voleur(GtkWidget *appel, FenetrePerso * _perso);
void modif_active(GtkWidget *appel, FenetrePerso * _perso);
void modif_psi(GtkWidget *appel, FenetrePerso * _perso);
void modif_origine(GtkWidget *appel, FenetrePerso * _perso);
void simplifier_arme(GtkWidget *appel, FenetrePerso * _perso);
GtkComboBox * creation_combo_armure(void);
GtkComboBox * creation_combo_race(void);
GtkComboBox * creation_combo_version(void);
GtkComboBox * creation_combo_sexe(void);
void couleur(GtkWidget * wid, unsigned short place);
void modif_competence(GtkWidget *appel, FenetrePerso * _perso);
void ferme_modif_perso(GtkWidget *wid, FenetrePerso * _perso);

/* ajouter_enlever_modif.c */
void ajoute_ligne_classe(FenetrePerso * _perso,unsigned short place,unsigned short classe,unsigned short niveau,unsigned long XP);
void ajoute_ligne_arme(FenetrePerso * _perso,unsigned short place,unsigned short arme,unsigned short niveau,signed short magie,signed short autre_t,signed short autre_d);
void ajoute_ligne_competence(FenetrePerso * _perso,unsigned short place,signed short competence,unsigned short niveau);
void enleve_ligne_arme(struct widgets * fenetre, signed short ligne);
unsigned short classe_nb(unsigned short classe,unsigned short * classe_modif);
unsigned short race_nb(unsigned short race, unsigned short * classe_race);


/* verif_modif.c */
void verif_caract(FenetrePerso * _perso);
void verif_race(FenetrePerso * _perso);
void verif_classe(FenetrePerso * _perso);
void verif_pdv(FenetrePerso * _perso);
void verif_XP_niv(FenetrePerso * _perso);
void verif_arme(FenetrePerso * _perso);
void verif_armes_simplifie(FenetrePerso * _perso);
void verif_alignement(FenetrePerso * _perso);
void verif_competence(FenetrePerso * _perso);
void verif_voleur(FenetrePerso * _perso);
void verif_psi(FenetrePerso * _perso);
void verif_dieu(FenetrePerso * _perso);

/* change_modif.c */
void calcul_couleur_classe(FenetrePerso * _perso);
void calcul_couleur_armure(FenetrePerso * _perso);
void calcul_couleur_race(FenetrePerso * _perso);
void calcul_couleur_arme(FenetrePerso * _perso);
void calcul_couleur_competence(FenetrePerso * _perso);
gboolean couleur_combo(GtkWidget *widget, cairo_t *cr, unsigned short * _couleur);
void callback_color_race_cellrender (GtkComboBox *combobox,FenetrePerso * _perso);
void callback_color_classe_cellrender (GtkComboBox *combobox,FenetrePerso * _perso);
void callback_color_armure_cellrender (GtkComboBox *combobox,FenetrePerso * _perso);
void callback_color_arme_cellrender (GtkComboBox *combobox,FenetrePerso * _perso);
void callback_color_competence_cellrender (GtkComboBox *combobox,FenetrePerso * _perso);
void donne_caract_modif(FenetrePerso * _perso);
void donne_version_modif(FenetrePerso * _perso);
void donne_sexe_modif(FenetrePerso * _perso);
void donne_race_modif(FenetrePerso * _perso);
void donne_classe_modif(FenetrePerso * _perso); /* -1 pour le dernier */
void donne_arme_modif(FenetrePerso * _perso); /* -1 pour le dernier */
void donne_competence_modif(FenetrePerso * _perso);
void donne_niv_classe_modif(FenetrePerso * _perso);
void couleur_item_rouge_gris_vert(GtkWidget * wid,unsigned short _couleur);
void couleur_item_rouge_gris_orange(GtkWidget * wid,unsigned short _couleur);

/* poids_taille.c */
void tire_taille_poids(GtkWidget *wid,  FenetrePerso * _perso);
void verif_taille(GtkWidget *wid, FenetrePerso * _perso);
void adapte_taille(GtkWidget *wid, FenetrePerso * _perso);
void adapte_poids(GtkWidget *wid, FenetrePerso * _perso);
void calc_taille_poids(GtkWidget *wid, FenetrePerso * _perso);

/* enregistre.c */
char * verif_ext(char * nom,char * ext); /* rajoute l'extension si elle n'est pas là, affecte la mémoire pour la sortie : à effacer après */
void enregistre_perso(GtkWidget *ChildWidget, FenetrePerso * _perso);

#endif // INCLUDE
