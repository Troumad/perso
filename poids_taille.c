#include "include.h"

void tire_taille_poids(GtkWidget *wid,  FenetrePerso * _perso)
{
    struct_race * race=RACE+_perso->race_modif;
    unsigned short sexe=_perso->sexe_modif;
    unsigned short i,t,v;
    char ch[16];
    signed short _version=_perso->version_modif;


    if (race->add[ADD].nom!=NULL)
    { /* seul les humains tirent sur 1d4, les autres races sur 1d3 */
        v=compare_sans_casse(race->add[ADD].nom,"humain")==0?4:3;
    }
    else
    {
        v=3;
    }

    srand((unsigned int)time(0));

    t=rand()%1000;
    for(i=0; i<8 && t>v_poids_taille[i]; i++); /* recherche de la catégorie du tirage en pour 1000 */
    sprintf(ch,"taille_%hu",i+1);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,ch)),TRUE);
    adapte_taille(NULL,_perso);

    if      (i<4)
    {
         gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_taille")),-v*(4-i)+rand()%v);
    }
    else if (i>4)
    {
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_taille")),v*(i-5)+1+rand()%v);
    }
    else
    { /* 4 cas central : pas de modification */
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_taille")),0);
    }

    t=rand()%1000;
    for(i=0; i<8 && t>v_poids_taille[i]; i++);
    sprintf(ch,"poids_%hu",i+1);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,ch)),TRUE);
    adapte_poids(NULL,_perso);

    v*=2;
    if      (i<4)
    {
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_taille")),-v*(4-i)+rand()%v);
    }
    else if (i>4)
    {
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_taille")),v*(i-5)+1+rand()%v);
    }
    else
    { /* tranche médiane sans variation */
    }

    /* modifier la taille de chaque race */
    if ( (_version==ADD && race->add[ADD].nom!=NULL) || race->add[ADD2].nom==NULL)
    {
        t=0;
        if (race->add[ADD].var_taille[sexe]!=NULL && sscanf(race->add[ADD].var_taille[sexe],"%hud%hu",&i,&v)==2)
        {
            for(;i>0;i--)
                t+=rand()%v+1;
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"taille_race")),race->add[ADD].taille[sexe]+v);
    }
    else
    {
        t=0;
        if (race->add[ADD2].var_taille[sexe]!=NULL && sscanf(race->add[ADD2].var_taille[sexe],"%hud%hu",&i,&v)==2)
        {
            for(;i>0;i--)
                t+=rand()%v+1;
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"taille_race")),race->add[ADD2].taille[sexe]+t);
    }
    calc_taille_poids(NULL,_perso);

    (void)wid;
}

void adapte_taille(GtkWidget *wid, FenetrePerso * _perso)
{
    unsigned short i;
    struct_race * race=RACE+_perso->race_modif;
    unsigned short t;
    char ch[32];

    if (race->add[ADD].nom!=NULL)
    { /* seul les humain tirent sur 1d4, les autres races sur 1d3 */
        t=compare_sans_casse(race->add[ADD].nom,"humain")==0?4:3;
    }
    else
    {
        t=3;
    }


    i=0;
    sprintf(ch,"taille_1");
    while(i<8 && !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,ch))))
    {
      i++;
      sprintf(ch,"taille_%hu",i+1);
    }

    if (i<4)
    {
        gtk_spin_button_set_range(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_taille")),-t*(4-i),-t*(3-i)-1);
    }
    else if (i==4)
    {
        gtk_spin_button_set_range(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_taille")),0,0);
    }
    else
    {
        gtk_spin_button_set_range(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_taille")),t*(i-5)+1,t*(i-4));
    }

    (void)wid;
}

void adapte_poids(GtkWidget *wid, FenetrePerso * _perso)
{
    struct_race * race=RACE+_perso->race_modif;
    unsigned short i,t;
    char ch[32];

    if (race->add[ADD].nom!=NULL)
    { /* seul les humain tirent sur 1d8, les autres races sur 1d6 */
        t=compare_sans_casse(race->add[ADD].nom,"humain")==0?8:6;
    }
    else
    {
        t=6;
    }

    i=0;
    sprintf(ch,"poids_1");
    while(i<8 && !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,ch))))
    {
      i++;
      sprintf(ch,"poids_%hu",i+1);
    }

    if (i<4)
    {
        gtk_spin_button_set_range(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_poids")),-t*(4-i),-t*(3-i)-1);
    }
    else if (i==4)
    {
        gtk_spin_button_set_range(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_poids")),0,0);
    }
    else
    {
        gtk_spin_button_set_range(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_poids")),t*(i-5)+1,t*(i-4));
    }

    (void)wid;
}

void verif_taille(GtkWidget *wid, FenetrePerso * _perso)
{
    struct_race * race=RACE+_perso->race_modif;
    unsigned short sexe=_perso->sexe_modif;
    unsigned short i,j,k;
    signed short _version=_perso->version_modif;

    if ( (_version==ADD && race->add[ADD].nom!=NULL) || race->add[ADD2].nom==NULL)
    {
        if (race->add[ADD].var_taille[sexe]!=NULL && sscanf(race->add[ADD].var_taille[sexe],"%hud%hu",&k,&j)==2)
        {
            i=k*j;
        }
        else
        {
            i=0;
            k=0;
        }
        j=race->add[ADD].taille[sexe];
        i+=j;
        j+=k;
    }
    else
    {
        if (race->add[ADD2].var_taille[sexe]!=NULL && sscanf(race->add[ADD2].var_taille[sexe],"%hud%hu",&k,&j)==2)
        {
            i=k*j;
        }
        else
        {
            i=0;
            k=0;
        }
        j=race->add[ADD2].taille[sexe];
        i+=j;
        j+=k;
    }
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"taille_race")),j,i);

    (void)wid;
}


void calc_taille_poids(GtkWidget *wid, FenetrePerso * _perso)
{
    float poids,
          taille=gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"taille_race")))*(1+0.01*gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_taille"))));
    unsigned short force=gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,nom_min[0])));
    unsigned short i;
    char ch[LONG];
    struct_race * race=RACE+_perso->race_modif;
    unsigned short sexe=_perso->sexe_modif;
    short _version=_perso->version_modif;

    if (force<4)
    {
        force=0;
    }
    else
    {
        force-=3;
        if (force>15) force=15;
    }
    taille*=(1+force_taille[force]);

    for (i=0; i<80 && taille<c_poids_taille[i][0]; i++);
    if (i==0)
    {
        poids=c_poids_taille[0][1]/c_poids_taille[0][0]*taille;
    }
    else if (i>=80)
    {
        poids=c_poids_taille[79][1]/c_poids_taille[79][0]*taille;
    }
    else
    {
        poids=c_poids_taille[i][1];
    }

    /* modifier ici le rapport poids/taille de la race */
    if ( (_version==ADD && race->add[ADD].nom!=NULL) || race->add[ADD2].nom==NULL)
    {
        poids*=(race->add[ADD].poids[sexe]*(1+0.01*gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_poids"))))*(1+force_poids[force]));
    }
    else
    {
         poids*=(race->add[ADD2].poids[sexe]*(1+0.01*gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"spin_poids"))))*(1+force_poids[force]));
    }
    sprintf(ch,"Taille = %.2f pouces ou %.2f m\nMasse = %.1f livres ou %.1f kg",taille,taille*0.0254,poids,poids*.42);
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(_perso->modif->builder,"info_poids_taille")),ch);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"taille")),taille*2.54);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,"poids")),poids*.42);

    (void)wid;
}

