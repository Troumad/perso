#include "include.h"
#include <string.h>

void mini_vol_modif(GtkWidget * wid,FenetrePerso * _perso);
void efface_mag (GtkWidget * appel,FenetrePerso * _perso);
void callback_couleur_competence (GtkComboBox *combobox,FenetrePerso * _perso);

void donne_caract_modif(FenetrePerso * _perso)
{
    unsigned short i;
    GtkWidget * wid;

    for(i=0;i<=8;i++)
    {
        wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[i]));
        _perso->caract_modif[i]=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
    }
}

void donne_version_modif(FenetrePerso * _perso)
{
    GtkWidget * wid;

    wid=gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"sexe_race_version")),"version");
    _perso->version_modif=gtk_combo_box_get_active (GTK_COMBO_BOX(wid));
}

void donne_race_modif(FenetrePerso * _perso)
{
    GtkWidget * wid;

    wid=gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"sexe_race_version")),"race");
    _perso->race_modif=_perso->combo_races[gtk_combo_box_get_active (GTK_COMBO_BOX(wid))];
}

void donne_sexe_modif(FenetrePerso * _perso)
{
    GtkWidget * wid;

    wid=gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"sexe_race_version")),"sexe");
    _perso->sexe_modif=gtk_combo_box_get_active (GTK_COMBO_BOX(wid));
}

void donne_classe_modif(FenetrePerso * _perso)
{
    char ch[64];
    unsigned short i,j;
    GtkWidget * wid, *pere=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes"));

    g_free(_perso->classe_modif);
    _perso->classe_modif=NULL;

    i=0;
    wid=gtk_get_widget_by_name(GTK_CONTAINER(pere),"classe_0");
    if (wid!=NULL)
    {
        while (wid!=NULL)
        {
            i++;
            _perso->classe_modif=(signed short*)g_realloc(_perso->classe_modif,(i+1)*sizeof(signed short));
            j=gtk_combo_box_get_active (GTK_COMBO_BOX(wid));
            _perso->classe_modif[i-1]=_perso->combo_classes[j];
            sprintf(ch,"classe_%hu",i);
            wid=gtk_get_widget_by_name(GTK_CONTAINER(pere),ch);
        }
    }
    else
    {
        _perso->classe_modif=(signed short*)g_malloc(sizeof(signed short));
    }
    _perso->classe_modif[i]=-1;
}

void donne_niv_classe_modif(FenetrePerso * _perso)
{
    char ch[64];
    unsigned short i;
    GtkWidget * wid, *pere=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes"));

    g_free(_perso->niv_classe_modif);
    _perso->niv_classe_modif=NULL;

    i=0;
    wid=gtk_get_widget_by_name(GTK_CONTAINER(pere),"niveau_0");
    if (wid!=NULL)
    {
        while (wid!=NULL)
        {
            i++;
            _perso->niv_classe_modif=(signed short*)g_realloc(_perso->niv_classe_modif,(i+1)*sizeof(signed short));
            _perso->niv_classe_modif[i-1]=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
            sprintf(ch,"niveau_%hu",i);
            wid=gtk_get_widget_by_name(GTK_CONTAINER(pere),ch);
        }
    }
    else
    {
        _perso->niv_classe_modif=(signed short*)g_malloc(sizeof(signed short));
    }
    _perso->niv_classe_modif[i]=-1;
}

void donne_arme_modif(FenetrePerso * _perso)
{
    char ch[64];
    unsigned short i;
    GtkWidget * wid, *pere=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_armes"));

    g_free(_perso->armes_modif);
    _perso->armes_modif=NULL;

    i=0;
    wid=gtk_get_widget_by_name(GTK_CONTAINER(pere),"arme_0");
    if (wid!=NULL)
    {
        while (wid!=NULL)
        {
            i++;
            _perso->armes_modif=(signed short*)g_realloc(_perso->armes_modif,(i+1)*sizeof(signed short));
            _perso->armes_modif[i-1]=gtk_combo_box_get_active (GTK_COMBO_BOX(wid));
            sprintf(ch,"arme_%hu",i);
            wid=gtk_get_widget_by_name(GTK_CONTAINER(pere),ch);
        }
    }
    else
    {
        _perso->armes_modif=(signed short*)g_malloc(sizeof(signed short));
    }
    _perso->armes_modif[i]=-1;
}

void donne_competence_modif(FenetrePerso * _perso)
{
    char ch[64];
    unsigned short i;
    GtkWidget * wid, *pere=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_competence"));


    g_free(_perso->competence_modif);
    _perso->competence_modif=NULL;

    i=0;
    wid=gtk_get_widget_by_name(GTK_CONTAINER(pere),"competence_0");
    if (wid!=NULL)
    {
        while (wid!=NULL)
        {
            i++;
            _perso->competence_modif=(signed short*)g_realloc(_perso->competence_modif,(i+1)*sizeof(signed short));
            _perso->competence_modif[i-1]=gtk_combo_box_get_active (GTK_COMBO_BOX(wid));
            sprintf(ch,"competence_%hu",i);
            wid=gtk_get_widget_by_name(GTK_CONTAINER(pere),ch);
        }
    }
    else
    {
        _perso->competence_modif=(signed short*)g_malloc(sizeof(signed short));
    }
    _perso->competence_modif[i]=-1;

}

void calcul_couleur_classe(FenetrePerso * _perso)
{
    signed short i,j,place=0,clas_sortie;
    signed short * clas;
    short race=_perso->race_modif;
    short _version=_perso->version_modif;
    char ch[32];

    GtkWidget * wid,*pere=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes"));;

    for(i=0;i<NB_CLASSE;i++)
    {
        if ((CLASSE[i].add[0].nom==NULL || _perso->origine[0][CLASSE[i].add[0].origine]==0) && ( CLASSE[i].add[1].nom==NULL || _perso->origine[1][CLASSE[i].add[1].origine]==0))
        {
            _perso->couleur_classes[i]=NOIR;
        }
        else if (est_compatible(race,i,_version)==1)
        {
            _perso->couleur_classes[i]=GRIS;
        }
        else
        {
            if (CLASSE[i].add[_version].nom==NULL)
            {
                _perso->couleur_classes[i]=ROUGE;
            }
            else
            {
                _perso->couleur_classes[i]=ORANGE;
            }
        }
    }

    if (_perso->classe_modif!=NULL)
    {
        for(i=0;_perso->classe_modif[i]!=-1;i++);
        clas=(signed short *)g_malloc((i+1)*sizeof(signed short));
        for(i=0;_perso->classe_modif[i]!=-1;i++)
        {
            clas[i]=_perso->classe_modif[i];
        }
        clas[i]=-1;
    }
    else
    {
        clas=NULL;
    }

    wid=gtk_get_widget_by_name(GTK_CONTAINER(pere),"classe_0");
    j=0;
    if (wid!=NULL)
    {
        while (wid!=NULL)
        {
            place=0;
            gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(wid));

            for(i=0;i<NB_CLASSE;i++)
            {
                if (_perso->couleur_classes[i]==NOIR || (_perso->couleur_classes[i]==ROUGE && _perso->ori_add1_add2==0))
                {
                    /* on ne rajoute pas la classe */
                }
                else
                {
                    if (clas && clas[j]==i)
                    {
                        clas_sortie=place;
                    }
                    else
                    {
                        /* pas la classe sélectionnée */
                    }
                    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(wid), CLASSE[i].nom, CLASSE[i].nom);
                    _perso->combo_classes[place]=i;
                    place++;
                }
            }
            if (clas)
            {
                gtk_combo_box_set_active(GTK_COMBO_BOX(wid),clas_sortie);
            }
            else
            {
                /* pas encore initialisé */
            }
            callback_color_classe_cellrender (GTK_COMBO_BOX(wid),_perso);
            g_signal_connect (G_OBJECT(wid),"changed", G_CALLBACK(modif_classe),_perso);
            j++;
            sprintf(ch,"classe_%hu",j);
            wid=gtk_get_widget_by_name(GTK_CONTAINER(pere),ch);
        }
    }
    else
    {
        /* aucune classe */
    }

    g_free(clas);
}

void calcul_couleur_armure(FenetrePerso * _perso)
{
    unsigned short i,k;
    signed short _version=_perso->version_modif;

    _perso->couleur_armure[0]=GRIS;
    for(i=1;i<NB_ARMURE;i++)
    {
        _perso->couleur_armure[i]=GRIS;
        if (_perso->classe_modif)
        {
            for(k=0; _perso->classe_modif[k]!=-1; k++)
            {
    //if (i==1) dialogue("calcul_couleur_armure k=%hd",k);
                if                                                             (est_compatible_armure(_perso->classe_modif[k],i,_version)==1 ||
                    (CLASSE[_perso->classe_modif[k]].add[_version].nom==NULL && est_compatible_armure(_perso->classe_modif[k],i,!_version)==1))
                {
                    /* compatible : on laisse gris */
                }
                else
                {
                    _perso->couleur_armure[i]=ROUGE;
                }
            }
        }
        else
        { /* si classe modif non initialisé : pas normal, on l'initialise vise */
            _perso->classe_modif=(signed short *)g_malloc(sizeof(signed short));
            _perso->classe_modif[0]=-1;
        }
    }
}


void calcul_couleur_competence(FenetrePerso * _perso)
{
    unsigned short i,k;
    signed short * classe=_perso->classe_modif;
    unsigned long drap_compt=0,masque_primaire;


    for(k=0; classe[k]!=-1; k++)
    {
        if (CLASSE[classe[k]].add[ADD2].nom!=NULL)
        {
            drap_compt|=CLASSE[classe[k]].add[ADD2].liste_competences;
        }
        else
        { /* ce n'est qu'une classe ADD1 : sans compétences */
        }
    }
    k=NB_COMPETENCE; /* deux lignes pour éviter multiples apples */
    masque_primaire=MASQUE_COMPETENCE_PRIMAIRE;
    for(i=0;i<k;i++)
    {
        if ((drap_compt&(COMPETENCE[i].classe))==0)
        {   /* les compétences vraiment particulières sont marquées en rouges si elles sont interdites à la race */
            if ((COMPETENCE[i].classe&masque_primaire)==0)
            {
                _perso->couleur_competence[i]=ROUGE;
            }
            else
            {
                _perso->couleur_competence[i]=ORANGE;
            }
        }
        else
        {
            _perso->couleur_competence[i]=GRIS;
        }
    }
}

void calcul_couleur_arme(FenetrePerso * _perso)
{
    unsigned short i,k;
    short version_=_perso->version_modif;
    signed short * classe=_perso->classe_modif;


    for(i=0;ARME[i].groupe==1;i++)
    {
        _perso->couleur_armes[i]=ROUGE;
    }
    for(;ARME[i].groupe==2;i++)
    {
        _perso->couleur_armes[i]=JAUNE;
    }
    for(;i<NB_ARME;i++)
    {
        _perso->couleur_armes[i]=VERT;
    }
    for(k=0; classe[k]!=-1; k++)
    {
        signed short * pt;
        if (CLASSE[classe[k]].add[version_].nom!=NULL)
        {/* décompte du nombre d'armes permis */
            for(pt=CLASSE[classe[k]].add[version_].armes; *pt!=FIN; pt++)
            {
                if (*pt==-1)
                { /* toutes les armes sont permises */
                    for(i=0;i<NB_ARME;i++)
                    {
                        _perso->couleur_armes[i]=GRIS;
                    }
                }
                else
                {
                    _perso->couleur_armes[*pt]=GRIS;
                }
            }
        }
        else  /* MÊME SI C'EST OFFICIELLEMENT une classe d'une version uniquement, je la prends en compte pour l'autre */
        {
            for(pt=CLASSE[classe[k]].add[1-version_].armes; *pt!=FIN; pt++)
            {
                if (*pt==-1)
                { /* toutes les armes sont permises */
                    for(i=0;i<NB_ARME;i++)
                    {
                        if (_perso->couleur_armes[i]==ROUGE)
                        { /* on signale que ce n'est permet que avec la classe interdite */
                            _perso->couleur_armes[i]=ORANGE;
                        }
                        else
                        { /* on a déjà au moins aussi bien */
                        }
                    }
                }
                else
                {
                    if (_perso->couleur_armes[*pt]==ROUGE)
                    {/* on signale que ce n'est permet que avec la classe interdite */
                        _perso->couleur_armes[*pt]=ORANGE;
                    }
                    else
                    { /* on a déjà au moins aussi bien */
                    }
                }
            }
        }
    }
}

void calcul_couleur_race(FenetrePerso * _perso)
{
    signed short i,place=0,ras,ras_sortie=0;
    short _version=_perso->version_modif;

    GtkWidget * wid=NULL;

    for(i=0;i<NB_RACE;i++)
    {
        if ((RACE[i].add[0].nom==NULL || _perso->origine[0][RACE[i].add[0].origine]==0) && ( RACE[i].add[1].nom==NULL || _perso->origine[1][RACE[i].add[1].origine]==0))
        {
            _perso->couleur_races[i]=NOIR;
        }
        else if (RACE[i].add[_version].nom==NULL || _perso->origine[_version][RACE[i].add[_version].origine]==0)
        {
            _perso->couleur_races[i]=ROUGE;
        }
        else
        {
            _perso->couleur_races[i]=GRIS;
        }
    }

    wid=gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"sexe_race_version")),"race");
    if (wid!=NULL)
    {
            place=0;
            ras=_perso->combo_races[gtk_combo_box_get_active (GTK_COMBO_BOX(wid))];
            gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(wid));

            for(i=0;i<NB_RACE;i++)
            {
                if (_perso->couleur_races[i]==NOIR || (_perso->couleur_races[i]==ROUGE && _perso->ori_add1_add2==0))
                {
                    /* on ne rajoute pas la classe */
                }
                else
                {
                    if (ras==i)
                    {
                        ras_sortie=place;
                    }
                    else
                    {
                        /* pas la classe sélectionnée */
                    }
                    gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(wid), RACE[i].nom, RACE[i].nom);
                    _perso->combo_races[place]=i;
                    place++;
                }
            }
            gtk_combo_box_set_active(GTK_COMBO_BOX(wid),ras_sortie);
            callback_color_race_cellrender (GTK_COMBO_BOX(wid),_perso);
            g_signal_connect (G_OBJECT(wid),"changed", G_CALLBACK(modif_race),_perso);
    }
    else
    {
        /* aucune race : pas normal du tout */
    }



}

gboolean couleur_combo(GtkWidget *widget, cairo_t *cr, unsigned short * _couleur)
{
   couleur_item(widget,*_couleur);
   GtkAllocation allocation;

   /* Récupération de la taille du widget */
   gtk_widget_get_allocation (widget, &allocation);

   /* Affectation de la couleur de fond */
   switch (*_couleur)
    {
        case JAUNE   : cairo_set_source_rgba (cr, 1, 0.85, 0,1);
         break;
        case BLEU   : cairo_set_source_rgba (cr, 0.3, 0.3, 1,1);
         break;
        case VERT   : cairo_set_source_rgba (cr, 0.1, 1, 0.1,1);
         break;
        case NOIR   : cairo_set_source_rgba (cr, 1, 0.0, 0.0,.5);
         break;
        case ROUGE  : cairo_set_source_rgba (cr, 1, 0.1, 0.1,1);
         break;
        case GRIS   :  cairo_set_source_rgba (cr, .90, .90, .90,1);
         break;
        case ORANGE :  cairo_set_source_rgba (cr, 1, .6, 0,2);
         break;
        default :
         break;
    }
   /* Remplissage de la surface du widget */
   cairo_rectangle (cr, 0, 0, allocation.width, allocation.height);
   cairo_fill(cr);

   return FALSE;
}

void callback_color_arme_cellrender (GtkComboBox *combobox,FenetrePerso * _perso)
{
  gint i;
  AtkObject *atk;
  GtkWidget *widget = NULL;

  for (i=0; i < atk_object_get_n_accessible_children (gtk_combo_box_get_popup_accessible (combobox)); i++)
  {
      atk =  atk_object_ref_accessible_child (gtk_combo_box_get_popup_accessible (combobox), i);
      widget = gtk_accessible_get_widget ((GtkAccessible*)atk);

      g_signal_connect(G_OBJECT(widget), "draw",G_CALLBACK(couleur_combo) ,_perso->couleur_armes+i);
  }
}

void callback_couleur_competence (GtkComboBox *combobox,FenetrePerso * _perso)
{
  gint i;
  AtkObject *atk=NULL;
  GtkWidget *widget = NULL;
  //cairo_t cr;
  GtkAllocation allocation;

  for (i=0; i < atk_object_get_n_accessible_children (gtk_combo_box_get_popup_accessible (combobox)); i++)
  {
      atk =  atk_object_ref_accessible_child (gtk_combo_box_get_popup_accessible (combobox), i);
      widget = gtk_accessible_get_widget ((GtkAccessible*)atk);
   /* Récupération de la taille du widget */
      gtk_widget_get_allocation (widget, &allocation);

      g_signal_connect(G_OBJECT(widget), "draw",G_CALLBACK(couleur_combo) ,_perso->couleur_competence+i);

      /*switch (_perso->couleur_competence[i])
      {
            case JAUNE  : cairo_set_source_rgba (&cr, 1, 0.85, 0,1);
             break;
            case BLEU   : cairo_set_source_rgba (&cr, 0.3, 0.3, 1,1);
             break;
            case VERT   : cairo_set_source_rgba (&cr, 0.1, 1, 0.1,1);
             break;
            case ROUGE  : cairo_set_source_rgba (&cr, 1, 0.1, 0.1,1);
             break;
            case GRIS   : cairo_set_source_rgba (&cr, .90, .90, .90,1);
             break;
            case ORANGE :  cairo_set_source_rgba (&cr, 1, .6, 0,2);
             break;
            default :
             break;
      }*/
   /* Affectation de la couleur de fond */
   /* Remplissage de la surface du widget */
     /* cairo_rectangle (&cr, 0, 0, allocation.width, allocation.height);
      cairo_fill(&cr);

      gtk_widget_draw (widget,&cr);*/
  }
}


void callback_color_competence_cellrender (GtkComboBox *combobox,FenetrePerso * _perso)
{
  gint i;
  AtkObject *atk;
  GtkWidget *widget = NULL;
/*g_signal_connect(G_OBJECT(combobox), "show",G_CALLBACK(callback_couleur_competence) ,_perso);*/

  for (i=0; i < atk_object_get_n_accessible_children (gtk_combo_box_get_popup_accessible (combobox)); i++)
  {
      atk =  atk_object_ref_accessible_child (gtk_combo_box_get_popup_accessible (combobox), i);
      widget = gtk_accessible_get_widget ((GtkAccessible*)atk);

      g_signal_connect(G_OBJECT(widget), "draw",G_CALLBACK(couleur_combo) ,_perso->couleur_competence+i);
  }
}

void callback_color_classe_cellrender (GtkComboBox *combobox,FenetrePerso * _perso)
{
  gint i;
  AtkObject *atk;
  GtkWidget *widget = NULL;

  for (i=0; i < atk_object_get_n_accessible_children (gtk_combo_box_get_popup_accessible (combobox)); i++)
  {
      atk =  atk_object_ref_accessible_child (gtk_combo_box_get_popup_accessible (combobox), i);
      widget = gtk_accessible_get_widget ((GtkAccessible*)atk);

      g_signal_connect(G_OBJECT(widget), "draw",G_CALLBACK(couleur_combo) ,_perso->couleur_classes+_perso->combo_classes[i]);
  }
}

void callback_color_armure_cellrender (GtkComboBox *combobox,FenetrePerso * _perso)
{
  gint i;
  AtkObject *atk;
  GtkWidget *widget = NULL;

  for (i=0; i < atk_object_get_n_accessible_children (gtk_combo_box_get_popup_accessible (combobox)); i++)
  {
      atk =  atk_object_ref_accessible_child (gtk_combo_box_get_popup_accessible (combobox), i);
      widget = gtk_accessible_get_widget ((GtkAccessible*)atk);

      g_signal_connect(G_OBJECT(widget), "draw",G_CALLBACK(couleur_combo) ,_perso->couleur_armure+i);
  }
}

void callback_color_race_cellrender (GtkComboBox *combobox,FenetrePerso * _perso)
{
  gint i;
  AtkObject *atk;
  GtkWidget *widget = NULL;

  for (i=0; i < atk_object_get_n_accessible_children (gtk_combo_box_get_popup_accessible (combobox)); i++)
  {
      atk =  atk_object_ref_accessible_child (gtk_combo_box_get_popup_accessible (combobox), i);
      widget = gtk_accessible_get_widget ((GtkAccessible*)atk);

      g_signal_connect(G_OBJECT(widget), "draw",G_CALLBACK(couleur_combo) ,_perso->couleur_races+_perso->combo_races[i]);
  }
}

void mini_vol_modif(GtkWidget * wid,FenetrePerso * _perso)
{
    signed short i,j,cl;
    signed char dext;
    signed short comp[FEUILLE_LAR_VOL];
    short _version=_perso->version_modif;
    signed short * classe=_perso->classe_modif;
    signed short * niv_classe=_perso->niv_classe_modif;

    for(j=0; j<FEUILLE_LAR_VOL; j++)
    {
        comp[j]=0; /* initialisation */
    }

    dext=gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,nom_min[DEXTERITE])));
    dext=(max(_min(dext,19),9))-9;

    if (_version==ADD1)
    {
        for (cl=0;classe[cl]!=-1;cl++)
        {
            if (CLASSE[classe[cl]].add[ADD].nom!=NULL)
            {
                for (j=0; j<niv_classe[cl] && CLASSE[classe[cl]].add[ADD].tab_voleur[j]!=NULL; j++);
                if (j>0)
                { /* si on est sorti, c'est qu'on est allé un cran trop loin */
                    j--;
                }
                else
                {
                }
                if (CLASSE[classe[cl]].add[ADD].tab_voleur[j]!=NULL)
                {
                    for(i=0; i<FEUILLE_LAR_VOL; i++)
                    {
                        comp[i]=max(comp[i],CLASSE[classe[cl]].add[ADD].tab_voleur[j][i]);
                    }
                }
                else
                { /* s'il y a une compétence */
                }
            }
            else
            { /* si c'est une classe ADD1 */
            }
        }
    }
    else
    {
        for (cl=0;classe[cl]!=-1;cl++)
        {
            if (CLASSE[classe[cl]].add[ADD2].nom!=NULL)
            {
                if (CLASSE[classe[cl]].add[ADD2].voleur!=NULL && (strcmp("OK",CLASSE[classe[cl]].add[ADD2].voleur)==0 || strcmp("perso",CLASSE[classe[cl]].add[ADD2].voleur)==0))
                {
                    if (strcmp("perso",CLASSE[classe[cl]].add[ADD2].voleur)==0)
                    {
                        j=0;
                    }
                    else
                    { /* c'est une classe à compétence fixe */
                        for (j=0; j<niv_classe[cl] && CLASSE[classe[cl]].add[ADD2].tab_voleur[j]!=NULL; j++);
                        if (j>0)
                        { /* on est sorti un cran trop loin */
                            j--;
                        }
                        else
                        { /* précaution ... */
                        }
                    }
                    if (CLASSE[classe[cl]].add[ADD2].tab_voleur[j]!=NULL)
                    {
                        for(i=0; i<FEUILLE_LAR_VOL; i++)
                        {
                            comp[i]=max(comp[i],CLASSE[classe[cl]].add[ADD2].tab_voleur[j][i]);
                        }
                    }
                }
            }
            else
            { /* ce n'est pas une classe ADD2 */
            }
        }
    }

    /* gestion de la race */
    if ((_perso->perso.version==ADD2 && RACE[_perso->race_modif].add[ADD2].nom!=NULL) || RACE[_perso->race_modif].add[ADD].nom!=NULL)
    { /* soit c'est un perso ADD2 avec une race ADD2, soit c'est une race qui n'existe pas à ADD1 */
        for(i=0; i<FEUILLE_LAR_VOL; i++)
        {
            if (comp[i]>0)
            {
                comp[i]+=RACE[_perso->race_modif].add[ADD2].tab_voleur[0][i];
            }
            else
            { /* on ne met les bonus qu'à des compétences non nulles */
            }
        }
    }
    else
    {
        for(i=0; i<FEUILLE_LAR_VOL; i++)
        {
            if (comp[i]>0)
            {
                comp[i]+=RACE[_perso->race_modif].add[ADD].tab_voleur[0][i];
            }
            else
            { /* on ne met les bonus qu'à des compétences non nulles */
            }
        }
    }

    /* gestion de la dextérité */
    for (i=0; i<8; i++)
    {
        if (comp[i]>0)
        {
            comp[i]+=fac_vol_caract[dext][i];
        }
        else
        { /* on ne met les bonus qu'à des compétences non nulles */
        }
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(_perso->modif->builder,nom_fac_vol[i])),comp[i]);
    }


    (void)wid;
}

void efface_mag (GtkWidget * appel,FenetrePerso * _perso)
{
    unsigned short niv=10,i;
    char _nom[32];
    GtkWidget * wid_util, * wid;

    for(i=1;i<=9;i++)
    {
        sprintf(_nom,"effacer_%hu",i);
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,_nom));
        if (wid_util==appel)
        {
            niv=i-1;
            i=10;
        }
        else
        { /* pas le bon niveau */
        }
    }

    if (niv!=10)
    {
        sprintf(_nom,"mag_%hu",niv+1);
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,_nom));
        for(i=0;i<NB_SORT_MAG[ADD2][niv];i++)
        {
            sprintf(_nom,"pas_livre_%hu_%ld",niv,SORTILEGE_MAG[ADD2][niv][i].clef);
            wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),TRUE);

            sprintf(_nom,"voyage_%hu_%ld",niv,SORTILEGE_MAG[ADD2][niv][i].clef);
            wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wid),FALSE);
        }
    }
    else
    { /* surprise : niveau non trouvé */
    }
}
