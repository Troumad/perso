#include "include.h"
#include <string.h>


unsigned short num_fenetre(FenetrePerso ** liste_fenetre,unsigned short nb_fenetre, char * chemin);
void libere_fenetre_perso(FenetrePerso * f_p);
void init_perso(perso * pers);
void init_fenetre_perso(FenetrePerso * _perso);
void libere_perso(perso * pers);
void destroy_fenetre_perso(GtkWidget *wid, FenetrePerso * perso);

FenetrePerso * gestion_fenetre (struct widgets * widgets, char * chemin, FenetrePerso * fermeture)
{ /* chemin ne peut être qu'un .pers ou un .grp */
    static FenetrePerso ** liste_fenetre=NULL;
    FenetrePerso * retour=NULL;
    static unsigned short nb_fenetre=0;
    unsigned short i,j, drap;
    char * pt_n=NULL,  * tmp=NULL;
    FILE * fichier=NULL;

    if (fermeture !=NULL)
    { /* on prend en compte la fermeture de cette fenêtre */
        if (nb_fenetre==0)
        {
            printf("Plus de fenêtres à fermer\n");
        }
        else
        {
            if (chemin==NULL)
            { /* on a bien récupéré un pointeur vers la fenêtre à fermer */
                for (i=0;liste_fenetre[i]!=fermeture && i<nb_fenetre;i++);
            } /* i est le numéro de la fenêtre qu'on ferme */
            else
            {   /* le pointeur "fermeture" est un leure, on a récupéré le nom du fichier */
                if (chemin[strlen(chemin)-1]=='s')
                { /* fermeture d'un perso */

                }
                else if (chemin[strlen(chemin)-1]=='p' && fermeture->uri!=NULL)
                { /* fermeture d'un groupe */
                    for (j=0;fermeture->uri[j]!=NULL;j++);
                    while (j>0) /* comme on fait disparaitre l'entrée supprimée du tableau en décalant, autant commencer par la dernière */
                    {
                        j--;
                        ferme_fenetre_perso(NULL,liste_fenetre[num_fenetre(liste_fenetre,nb_fenetre,fermeture->uri[j])]);
                    }
                }
                else
                {
                    if (chemin[strlen(chemin)-1]=='p')
                    {
                        printf("Le groupe %s est vide\n",chemin);
                    }
                    else
                    {
                        printf("Erreur lors de la fermeture du fichier %s\n",chemin);
                    }
                }
                i=num_fenetre(liste_fenetre,nb_fenetre,chemin); /* à la fin après la fermeture des enfants pour les groupes */
            }
            if (i<nb_fenetre)
            {
                retour=liste_fenetre[i];
                nb_fenetre--;
                /* remplacer la fenêtre supprimmée par la dernière fenêtre */
                liste_fenetre[i]=liste_fenetre[nb_fenetre];
                liste_fenetre[nb_fenetre]=NULL;
                if (retour->modif!=NULL)
                { /* fermeture de la fenêtre de modification */
                    gtk_window_close(GTK_WINDOW(retour->modif->window));
                    retour->modif=NULL;
                }
                else
                {
                }
                if (retour->resume->window!=NULL)
                {
                    gtk_window_close(GTK_WINDOW(retour->resume->window));
                    retour->resume=NULL;
                }
                else
                {
                }
                libere_fenetre_perso(retour);
                retour=NULL;
                liste_fenetre[nb_fenetre]=NULL;
            }
            else
            {
                if (nom[0]==NULL)
                {
                    printf("Fermeture d'une fenêtre qui n'existe pas !\n");
                }
                else
                {
                    printf("La fenêtre %s est déjà fermée\n",chemin);
                }
            }
        }
    }
    else if (widgets!=NULL && chemin!=NULL)
    { /* Est-ce un perso qui fait parti d'un groupe ? */
        for (i=0;i<nb_fenetre;i++)
        {
            if (liste_fenetre[i]->uri!=NULL)
            { /* cette fenêtre est associée à un groupe */
                for (j=0;liste_fenetre[i]->uri[j]!=NULL;j++)
                { /* parcours des perso du groupe */
                    if (strcmp(chemin,liste_fenetre[i]->uri[j])==0)
                    {
                        retour=liste_fenetre[i];
                    }
                    else
                    {
                        /* ce n'est pas la fenêtre de notre perso */
                    }
                }
            }
            else
            {
                /* cette fenêtre est associée à un perso */
            }
        }
    }
    else if (widgets!=NULL)
    { /* création d'un nouveau perso avec tirage aléatoire */
        nb_fenetre++;
        liste_fenetre=(FenetrePerso **)g_realloc(liste_fenetre,(nb_fenetre+1)*sizeof(FenetrePerso *));
        liste_fenetre[nb_fenetre]=NULL;
        retour=(FenetrePerso *)g_malloc(sizeof(FenetrePerso));
        init_fenetre_perso(retour);
        liste_fenetre[nb_fenetre-1]=retour;
        retour->resume=ouverture_glade_retour("resume.glade",GLADE_NON_CONNECT);
        init_perso(&(retour->perso));

        retour->origine=(char **)malloc(2*sizeof(char *));
        retour->ori_add1_add2=1;
        for (i=0;i<2;i++)
        {
            retour->origine[i]=(char *)malloc(NB_ORIGINE[i]*sizeof(char));
            for (j=0;j<NB_ORIGINE[i];j++)
            {
                retour->origine[i][j]=1; /* par défaut, on autorise toutes les origines */
            }
        }

        gtk_builder_connect_signals(retour->resume->builder,retour);
    }
    else if (chemin!=NULL)
    { /* ouverture d'un fichier */
         fichier = g_fopen(chemin, "rb");
         if (fichier!=NULL) /* Le fichier existe déjà */
         {
            i=num_fenetre(liste_fenetre,nb_fenetre,chemin); /* recherche parmi les fichiers déjà ouverts */
            if (i==nb_fenetre)
            { /* un fichier pas encore ouvert */
                nb_fenetre++;
                liste_fenetre=(FenetrePerso **)g_realloc(liste_fenetre,(nb_fenetre+1)*sizeof(FenetrePerso *));
                liste_fenetre[nb_fenetre]=NULL;
                retour=(FenetrePerso *)g_malloc(sizeof(FenetrePerso));
                init_fenetre_perso(retour);
                liste_fenetre[nb_fenetre-1]=retour;
                init_perso(&(retour->perso));
                g_free(retour->perso.nom); /* pour bien montrer qu'il faudra le remplir à partir du fichier */
                retour->perso.nom=NULL;    /* où sont sauvegardées les données du personnages ou du groupe  */
                drap=1;
                if (compare_sans_casse(chemin+strlen(chemin)-5,".pers")==0)
                { /* ouverture d'un perso */
                    retour->resume=ouverture_glade_retour("resume.glade",GLADE_NON_CONNECT);
                }
                else if (compare_sans_casse(chemin+strlen(chemin)-4,".grp")==0)
                {   /* ouverture d'un groupe */
                    retour->resume=ouverture_glade_retour("groupe.glade",GLADE_NON_CONNECT);
                    for (i=0;i<=POURCENTAGE;i++)
                    { /* on marque que c'est un groupe */
                        retour->perso.caract[i]=-1;
                    }
                    retour->d_r=0; /* la fenêtre est ouverte */
                }
                else
                {
                    printf("Le fichier ouvert n'a pas la bonne extention\n");
                    nb_fenetre--;
                    drap=0;
                }
                if (drap==1)
                {
                    retour->perso.nom_fichier=g_strdup(chemin);
                    gtk_builder_connect_signals(retour->resume->builder,retour);
                    pt_n=chemin;
                    for (tmp=chemin;*tmp!=0;tmp++)
                    {
                        if (*tmp==SEPARATEUR)
                        {
                            pt_n=tmp+1;
                        }
                        else
                        {
                        }
                    }
                    gtk_window_set_title(GTK_WINDOW(gtk_builder_get_object(retour->resume->builder,"window")),pt_n);
                }
            }
            else
            { /* ce fichier est déjà  ouvert : mettre retour en premier plan */
                retour=liste_fenetre[i];
                gtk_window_present(GTK_WINDOW(retour->resume->window));
            }
            g_fclose(fichier);
            fichier=NULL;
         }
         else
         {
             printf("Surprise, le fichier %s n'existe pas\n",chemin);
         }
    }
    else
    { /* ouverture d'un nouveau perso vierge */
        nb_fenetre++;
        liste_fenetre=(FenetrePerso **)g_realloc(liste_fenetre,(nb_fenetre+1)*sizeof(FenetrePerso *));
        liste_fenetre[nb_fenetre]=NULL;
        retour=(FenetrePerso *)g_malloc(sizeof(FenetrePerso));
        init_fenetre_perso(retour);
        liste_fenetre[nb_fenetre-1]=retour;
        retour->resume=ouverture_glade_retour("resume.glade",GLADE_NON_CONNECT);
        init_perso(&(retour->perso));
        for (i=0;i<=POURCENTAGE;i++)
        {
            retour->perso.caract[i]=10;
        }
        for (i=0;i<FEUILLE_LAR_VOL;i++)
        {
            retour->perso.tab_voleur[i]=0;
        }
        retour->origine=(char **)malloc(2*sizeof(char *));
        retour->ori_add1_add2=1;
        for (i=0;i<2;i++)
        {
            retour->origine[i]=(char *)malloc(NB_ORIGINE[i]*sizeof(char));
            for (j=0;j<NB_ORIGINE[i];j++)
            {
                retour->origine[i][j]=1; /* par défaut, on autorise toutes les origines */
            }
        }

        gtk_builder_connect_signals(retour->resume->builder,retour);
    }
    if (liste_fenetre!=NULL)
    {
        liste_fenetre[nb_fenetre]=NULL; /* marquage de la fin */
    }
    else
    {
        /* problème au démarrage : lien vers un fichier non compatible */
    }

    return retour;
}

void ferme_fenetre_perso(GtkWidget *wid, FenetrePerso * _perso)
{
    static char drap=0; /* 0 : repos, 1 ferme un perso , 2 ferme un groupe, 3 ferme un perso dans un groupe */
    char modif;
    char ch[LONG];
    GtkWidget *p_dialog = NULL;

    if (_perso!=NULL && (drap==0 || (drap==2 && _perso->perso.caract[0]!=-1) || (drap==1 && _perso->perso.caract[0]==-1))) /* pour éviter un double appel */
    { /* ( on a du travail à faire && ( aucune modif en cours || (modif de groupe && et perso ) || (modif de perso et modif de groupe ) ) ) */
        if (_perso->perso.caract[0]==-1) /* c'est un groupe */
        {
            drap|=2;
            modif=1;
            if (_perso->d_r!=1)
            {
                sprintf(ch,"Voulez-vous enregistrer le groupe %s.\n",_perso->perso.nom_fichier);
                p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->resume->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
                switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
                {
                    case GTK_RESPONSE_NO:
                        gtk_widget_destroy(p_dialog);
                        break ; /* on sort du programme */
                    default :
                        gtk_widget_destroy(p_dialog);
                        enregistre_groupe(NULL,_perso);
                }
                _perso->d_r=1; /* on déclare la fenêtre fermée */
            }
            else
            { /* déjà fermé */
            }
        }
        else /* ce n'est pas un groupe */
        {
            drap|=1;
            modif=2;
            if (_perso->modif!=NULL)
            {
                ferme_modif_perso(NULL,_perso);
            }
            else
            { /* pas de fenêtre de modification ouverte */
            }
            if (_perso->d_r==1 && _perso->resume!=NULL)
            { /* une modification a été prise en compte */
                if (_perso->perso.nom!=NULL)
                {
                    sprintf(ch,"Voulez-vous enregistrer le personnage\n%s.\n",_perso->perso.nom);
                }
                else if (_perso->perso.nom_fichier!=NULL)
                {
                    sprintf(ch,"Voulez-vous enregistrer le personnage\n%s.\n",_perso->perso.nom_fichier);
                }
                else
                {
                    sprintf(ch,"Voulez-vous enregistrer le personnage\nsans nom que vous êtes en train de fermer ?\n");
                }
                p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->resume->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
                switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
                {
                    case GTK_RESPONSE_NO:
                        break ; /* on sort du programme */
                    default :
                        enregistre_perso(NULL,_perso);
                }
                gtk_widget_destroy(p_dialog);
            }
            else
            { /* aucune modification à enregistrer */
            }
        }

        if (_perso->resume==NULL)
        {
            printf("Déjà effacé\n");
        }
        else
        {
            gestion_fenetre(NULL, _perso->perso.nom_fichier ,_perso);
            _perso->resume=NULL;
        }
        drap&=modif; /* on signale que la modif en cours est finie */
    }
    else
    {
    }

    (void)wid;
}

void destroy_fenetre_perso(GtkWidget *wid, FenetrePerso * _perso)
{
    //_perso->resume=NULL;
    ferme_fenetre_perso(wid, _perso);
}


void affiche_resume(FenetrePerso * _perso)
{
    GtkLabel * label;
    char ch[2048];
    signed short i,l,pdv,cl_max=0,ll;

    /*titre*/
    *ch=0;
    if (_perso->perso.nom!=NULL && _perso->perso.nom[0]!=0)
    {
        sprintf(ch,"%s",_perso->perso.nom);
    }
    else
    {
        /* pas de nom de perso */
    }
    if (_perso->perso.joueur!=NULL && _perso->perso.joueur[0]!=0)
    {
        l=strlen(ch);
        sprintf(ch+l," joué%s par %s",_perso->perso.sexe==FEMININ?"e":"",_perso->perso.joueur);
    }
    else
    {
        /* pas le nom du joueur */
    }
    if (*ch!=0)
    {
        gtk_window_set_title(GTK_WINDOW(gtk_builder_get_object(_perso->resume->builder,"window")),ch);
    }
    else
    {
        /* pas de titre */
    }

    /* première colonne : général */
    label=GTK_LABEL(gtk_builder_get_object(_perso->resume->builder,"col1"));
    *ch=0;
    if (_perso->perso.classe!=NULL)
    {
        for (i=0;_perso->perso.classe[i]!=-1;i++)
        {
            l=strlen(ch);
            if (_perso->perso.classe[i]>=0)
            {
                sprintf(ch+l,"%s niveau %hd\n\t%lu XP\n",CLASSE[_perso->perso.classe[i]].nom,_perso->perso.niveau[i],_perso->perso.XP[i]);
            }
            else
            {
                sprintf(ch+l,"classe inconnue niveau %hd\n\t%lu XP\n",_perso->perso.niveau[i],_perso->perso.XP[i]);
            }
            cl_max=max(cl_max,_perso->perso.niveau[i]);
        }
    }
    else
    {
            /* pas de classe */
    }

    l=strlen(ch);
    if (_perso->perso.dieu!=NULL && _perso->perso.dieu[0]!=0)
    {
        sprintf(ch+l,"\nDieu : %s",_perso->perso.dieu);
    }
    else
    {
        /* pas de dieu */
    }
    l=strlen(ch);
    if (_perso->perso.pantheon!=NULL && _perso->perso.pantheon[0]!=0)
    {
        sprintf(ch+l,"\nPantheon : %s",_perso->perso.pantheon);
    }
    else
    {
        /* pas de panthéon */
    }
    l=strlen(ch);
    sprintf(ch+l,"\nVersion : %s\nSexe : %s\n",
                            vers[_perso->perso.version],nom_sexe[_perso->perso.sexe]);
    l=strlen(ch);
    if (_perso->perso.race>0)
    {
        sprintf(ch+l,"Race : %s\n \n",RACE[_perso->perso.race].nom);
    }
    else
    {
        sprintf(ch+l,"\n");
    }

    for(i=0;i<8;i++)
    {   /* écrire à la fin de la chaîne ch */
        sprintf(ch+strlen(ch),"%s : %hd\n",nom[i],_perso->perso.caract[i]);
    }
    if (_perso->perso.pdv!=NULL && _perso->perso.pdv[0]>0)
    {
        l=strlen(ch);
        sprintf(ch+l,"\nPDV\n %hu",_perso->perso.pdv[0]);
        ll=l+9;
        pdv=_perso->perso.pdv[0];
        for (i=1;_perso->perso.pdv[i]>0;i++)
        {
            l=strlen(ch);
            if (l>ll+20)
            {
                sprintf(ch+l,"\n");
                l=strlen(ch);
                ll=l;
            }
            sprintf(ch+l,"+%hu",_perso->perso.pdv[i]);
            pdv+=_perso->perso.pdv[i];
        }
        l=strlen(ch);
        sprintf(ch+l,"\n=%hu",pdv);
    }
    else
    { /* pas de pdv actifs */
    }
    if (*ch!=0)
    {
        gtk_label_set_text (label,ch);
    }
    else
    {
        /* chaine vide */
    }

    /* seconde colonne : voleur */
    label=GTK_LABEL(gtk_builder_get_object(_perso->resume->builder,"col2"));
    *ch=0;
    sprintf(ch,"COMPÉTENCES\nDE VOLEUR\n\n %s\n%hd\n",nom_fac_voleur[0],_perso->perso.tab_voleur[0]);
    for (i=1;i<8;i++)
    {
        l=strlen(ch);
        sprintf(ch+l,"%s\n%hd\n",nom_fac_voleur[i],_perso->perso.tab_voleur[i]);
    }
    if (*ch!=0)
    {
        gtk_label_set_text (label,ch);
    }
    else
    {
        /* chaine vide */
    }

    /* troisième colonne : compétences */
    label=GTK_LABEL(gtk_builder_get_object(_perso->resume->builder,"label"));
    *ch=0;
    if (_perso->perso.competence!=NULL)
    {
        sprintf(ch,"COMPÉTENCES GÉNÉRALES\n\n");
        for (i=0;_perso->perso.competence[i]>=0;i++)
        {
            l=strlen(ch);
            sprintf(ch+l,"%s %hd\n",COMPETENCE[_perso->perso.competence[i]].nom,_perso->perso.niv_competence[i]);
        }
    }
    else
    {
        /* pas de compétence */
    }
    if (_perso->perso.arme!=NULL)
    {
        l=strlen(ch);
        sprintf(ch+l,"\nCOMPÉTENCES MARTIALES\n\n");
        for (i=0;_perso->perso.arme[i]>=0;i++)
        { /* _perso->perso.arme[i] devrait faire -999 (FIN) pour sortir, mais au cours de l'évolution du programme, le valeur à pu passer à -1 ou -99 */
            l=strlen(ch);
            sprintf(ch+l,"%s %hd\n",ARME[_perso->perso.arme[i]].nom,_perso->perso.niv_arme[i]);
        }
    }
    else
    {
        /* pas d'arme */
    }
    if (*ch!=0)
    {
        gtk_label_set_text (label,ch);
    }
    else
    {
        /* chaine vide */
    }
}

unsigned short num_fenetre(FenetrePerso ** liste_fenetre,unsigned short nb_fenetre, char * chemin)
{
    unsigned short i, retour=nb_fenetre;

    if (chemin==NULL)
    {
    }
    else
    {
        for(i=0;i<nb_fenetre;i++)
        {
            if (strcmp(chemin,liste_fenetre[i]->perso.nom_fichier)==0)
            {
                retour=i;
                i=nb_fenetre; /* on sortira du for plus rapidement */
            }
            else
            {
            }
        }
    }

    return retour;
}

void nouveau(GtkWidget *wid, struct widgets * param)
{
    affiche_resume(gestion_fenetre (NULL, NULL, NULL));

    (void)param;
    (void)wid;
}

void init_fenetre_perso(FenetrePerso * _perso)
{
    unsigned short i;

    _perso->resume=NULL;
    _perso->modif=NULL;
    _perso->uri=NULL;
    _perso->d_m=0;
    _perso->d_r=0;
    _perso->classe_modif=(signed short *)g_malloc(sizeof(unsigned short)); /* -1 pour le dernier */
    _perso->classe_modif[0]=-1;
    _perso->armes_modif=(signed short *)g_malloc(sizeof(unsigned short)); /* -1 pour le dernier */
    _perso->armes_modif[0]=-1;
    _perso->competence_modif=(signed short *)g_malloc(sizeof(unsigned short)); /* -1 pour le dernier */
    _perso->competence_modif[0]=-1;
    _perso->niv_classe_modif=(signed short *)g_malloc(sizeof(unsigned short)); /* -1 pour le dernier */
    _perso->niv_classe_modif[0]=-1;
    _perso->couleur_armes     =(unsigned short *)g_malloc(NB_ARME      *sizeof(unsigned short));
    _perso->couleur_classes   =(unsigned short *)g_malloc(NB_CLASSE    *sizeof(unsigned short));
    _perso->combo_classes     =(unsigned short *)g_malloc(NB_CLASSE    *sizeof(unsigned short));
    _perso->couleur_armure    =(unsigned short *)g_malloc(NB_ARMURE    *sizeof(unsigned short));
    _perso->couleur_competence=(unsigned short *)g_malloc(NB_COMPETENCE*sizeof(unsigned short));
    _perso->couleur_races     =(unsigned short *)g_malloc(NB_RACE      *sizeof(unsigned short));
    _perso->combo_races       =(unsigned short *)g_malloc(NB_RACE      *sizeof(unsigned short));
    _perso->psi=0;
    _perso->mag=0;
    _perso->race_modif=0;

    for (i=0;i<NB_CLASSE;i++)
    { /* par précaution */
        _perso->combo_classes[i]=i;
    }
    for (i=0;i<NB_RACE;i++)
    {
        _perso->combo_races[i]=i;
    }
}



void libere_fenetre_perso(FenetrePerso * f_p)
{
    unsigned short i;

    if (f_p->uri!=NULL)
    {
        for (i=0;f_p->uri[i]!=NULL;i++)
        {
            g_free(f_p->uri[i]);
        }
        g_free(f_p->uri);
        f_p->uri=NULL;
    }
    else
    {
    }
    g_free(f_p->couleur_armes);
    g_free(f_p->couleur_classes);
    g_free(f_p->couleur_competence);
    g_free(f_p->couleur_races);
    libere_perso(&(f_p->perso));
    /*g_free(f_p);*/
}

void libere_perso(perso * pers)
{
    unsigned short i;
    g_free(pers->classe);
    g_free(pers->niveau);
    g_free(pers->XP);
    g_free(pers->nom);
    g_free(pers->joueur);
    g_free(pers->nom_fichier);
    g_free(pers->pdv);
    g_free(pers->competence);
    g_free(pers->niv_competence);
    g_free(pers->arme);
    g_free(pers->niv_arme);
    g_free(pers->mag_arme);
    g_free(pers->arme_autre_t);
    g_free(pers->arme_autre_d);
    g_free(pers->cheveux);
    g_free(pers->peau);
    g_free(pers->yeux);
    g_free(pers->bouclier);
    g_free(pers->bague);
    g_free(pers->commentaire);
    g_free(pers->pantheon);
    g_free(pers->dieu);
    if (pers->livre_sorts!=NULL)
    {
        for (i=0;i<9;i++)
        {
            g_free(pers->livre_sorts[i]);
        }
        g_free(pers->livre_sorts);
        pers->livre_sorts=NULL;
    }
    else
    {
    }
    if (pers->sort_su!=NULL)
    {
        for (i=0;i<9;i++)
        {
            g_free(pers->sort_su[i]);
        }
        g_free(pers->sort_su);
        pers->sort_su=NULL;
    }
    else
    {
    }
    g_free(pers->psi); /* pas enore géré */
    g_free(pers->niv_psi);
    g_free(pers->sociale);
    g_free(pers->natif_psi);
    g_free(pers->origine[0]);
    g_free(pers->origine[1]);
    g_free(pers->origine);
    pers->origine=NULL;
}

void init_perso(perso * pers)
{
    unsigned short i,j;

	pers->nom_fichier=NULL;
	pers->classe=NULL;
	pers->niveau=NULL;
	pers->XP=NULL;
	pers->pdv=NULL;
	pers->joueur=NULL;
	pers->age=0;
	pers->sexe=0;
	pers->race=0;
	pers->version=1;
	pers->nom=g_strdup(""); /* ne pas avoir NULL pour faire la différence avec un perso qui arrive d'un fichier à lire */
	pers->competence=NULL;
	pers->niv_competence=NULL;
	pers->arme=NULL;
	pers->niv_arme=NULL;
	pers->mag_arme=NULL;
	pers->arme_autre_t=NULL;
	pers->arme_autre_d=NULL;
	pers->alignement=4;
	pers->cheveux=NULL;
	pers->peau=NULL;
	pers->yeux=NULL;
	pers->poids=0;
	pers->taille=0;
	pers->bouclier=NULL;
	pers->bague=NULL;
	pers->autre_ca=0;
	pers->autre_save=0;
	pers->armure1=0;
	pers->armure1=0;
	pers->mag_armure1=0;
	pers->armure2=0;
	pers->mag_armure2=0;
	pers->commentaire=NULL;
	pers->pantheon=NULL;
	pers->dieu=NULL;
	pers->livre_sorts=NULL;
	pers->sort_su=NULL;
	pers->psi=NULL;
	pers->niv_psi=NULL;
	pers->natif_psi=NULL;
	pers->sociale=NULL;
	pers->cap='n';
	for(i=0;i<9;i++)
    {
        pers->caract[i]=0;
    }
    for (i=0;i<FEUILLE_LAR_VOL;i++)
    {
        pers->tab_voleur[i]=0;
    }
    pers->origine=(char **)malloc(2*sizeof(char*));
    pers->ori_add1_add2=1;
    for (j=0;j<2;j++)
    {
        pers->origine[j]=(char *)malloc(sizeof(char)*NB_ORIGINE[j]);
        for (i=0;i<NB_ORIGINE[j];i++)
        {
            pers->origine[j][i]=1; /* par défaut, on active toutes les origines */
        }
    }
}
