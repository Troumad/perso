#include "include.h"
#include <string.h>

char * pdv_classe(signed short _classe,signed short niv,signed short _version);
signed short classes_race(signed short nb_race,signed short * nb_classe,signed short version);

signed short classes_race(signed short nb_race,signed short * nb_classe,signed short version)
{ /* retour : 1 c'est bon, 0 ça ne va pas */
    liste_classe * pt_liste_classe;
    liste_classe_race * pt_liste_classe_race, * tmp_pt_liste_classe_race;
    signed short retour=0,tmp_retour;
    signed short i=0;


    if (nb_classe==NULL || nb_classe[0]==-1 || nb_race<0 || version<0)
    {
        retour=0; /* on ne devrait passer par là que pour une race n'acceptant aucune classe ou un choix vierge de classe*/
    }
    else if (RACE[nb_race].add[version].multiclasse==1)
    {
        retour=1;
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
        if (retour==i)
        {
            retour=1;
        }
        else
        {
            retour=0;
        }
    }

    return retour;
}


void verif_race(FenetrePerso * _perso)
{
    short race=_perso->race_modif;
    signed short _version=_perso->version_modif;
    char ch[128];
    GtkWidget * widj = NULL;


    if ( RACE[race].add[_version].nom && _perso->origine[_version][RACE[race].add[_version].origine]==0)
    { /* la race existe dans cette version, mais son origine n'est pas sélectionnée : on la sélectionne */
            strncpy(ch,"Activation de la source ",127);
            strncat(ch,_ORIGINE[_version][RACE[race].add[_version].origine].nom,127);
            strncat (ch," pour la race ",127);
            strncat (ch,RACE[race].nom,127);
            _perso->origine[_version][RACE[race].add[_version].origine]=1;
            dialogue(ch,0);
            sprintf(ch,"ori_%hd_%hd",_version,RACE[race].add[_version].origine);
            widj=gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"origine")),ch);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widj),TRUE);
    }
    else
    {
        /* pas besoin de chercher à activer l'origine*/
    }

    couleur_item(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"sexe_race_version")),"race_label"),_perso->couleur_races[_perso->race_modif]);
}

void verif_voleur(FenetrePerso * _perso)
{
    signed short i,j,cl, total=0, tmp;
    signed char dext;
    signed short comp[FEUILLE_LAR_VOL];
    short _version=_perso->version_modif;
    signed short * classe=_perso->classe_modif;
    signed short * niv_classe=_perso->niv_classe_modif;
    GtkWidget * wid;
    char ch[LONG];
    struct_race * race=RACE+_perso->race_modif;

    for(j=0; j<FEUILLE_LAR_VOL; j++)
    {
        comp[j]=0; /* initialisation */
    }

    wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[DEXTERITE]));
    dext=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
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
                        total+=CLASSE[classe[cl]].add[ADD2].tab_voleur[1][0]+CLASSE[classe[cl]].add[ADD2].tab_voleur[1][1]*(niv_classe[cl]-1);
                        j=0;
                        /* compétence du départ + les niveaux suivants + on prendra la liste au niveau 0 */
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
    if ((_perso->perso.version==ADD2 && race->add[ADD2].nom!=NULL) || race->add[ADD].nom!=NULL)
    { /* soit c'est un perso ADD2 avec une race ADD2, soit c'est une race qui n'existe pas à ADD1 */
        for(i=0; i<FEUILLE_LAR_VOL; i++)
        {
            if (comp[i]>0)
            {
                comp[i]+=race->add[ADD2].tab_voleur[0][i];
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
                comp[i]+=race->add[ADD].tab_voleur[0][i];
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
        total+=comp[i];
    }

    /* Après avoir compté, on vérifie si ça correspond aux comptes */
    *ch=0; /* initialiser la chaîne ch */
    for(j=0;j<8;j++)
    {
        wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_fac_vol[j]));
        tmp=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
        if (tmp<comp[j])
        {
            sprintf(ch+strlen(ch),"Le minimum de la compétence %s est %hd.\n",nom_fac_voleur[j],comp[j]);
        }
        else
        { /* le minimum requit est atteint */
        }
        total-=tmp;
    }
    if (total>0)
    { /* tout n'a pas été attribué */
        sprintf(ch+strlen(ch),"Il reste %hd points de compétence à distribuer.\n",total);
    }
    else if (total<0)
    { /* trop de point ont été attribués */
        sprintf(ch+strlen(ch),"%hd points de compétence ont été distribués en trop.\n",-total);
    }
    else
    { /* le bon nombre de point de compétence ont été attribués */
        if (*ch==0)
        { /* tout est parfait */
            strcpy(ch,"Bonne distribution des points de compétences.\n");
        }
        else
        {
            sprintf(ch+strlen(ch),"Mais le bon nombre de points ont été distribués.\n");
        }
    }
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(_perso->modif->builder,"label_voleur")),ch);
}



void verif_alignement(FenetrePerso * _perso)
{
    signed short * classe=_perso->classe_modif;
    signed short _version=_perso->version_modif;
    signed short pantheon=0,dieu=0;
    signed short j,i,moy_b,moy_l,nb_moy;
    GtkComboBox * combo;
    signed short alig;
    GtkWidget *p_dialog = NULL;
    char ch[8];

    combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"bt_pantheon")),"pantheon"));
    pantheon=gtk_combo_box_get_active(combo);
    combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"bt_dieu")),"dieu"));
    dieu=gtk_combo_box_get_active(combo);

    if (pantheon==0 || PANTHEONS[pantheon].dieu[dieu].nom==NULL)
    { /* tous les alignements */
        alig=((signed short)1<<9)-1;
    }
    else
    {
        alig=PANTHEONS[pantheon].dieu[dieu].alignement;
    }
    if (alig==0)
    {/* aucun alignement pour le dieu */
        alig=((signed short)1<<9)-1;
    }
    else
    {
        /* résultat valable */
    }
    for (j=0;classe[j]!=-1;j++)
    {
        if (CLASSE[classe[j]].add[_version].nom!=NULL)
        {
            alig&=CLASSE[classe[j]].add[_version].alignement;
        }
        else
        { /* la classe n'est pas dans cete version : on teste avec l'autre version */
            alig&=CLASSE[classe[j]].add[1-_version].alignement;
        }
    }
    if (alig==0)
    {
        p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_WARNING,GTK_BUTTONS_CLOSE,"Dieu et profession non compatible à cause de l'alignement");
        gtk_dialog_run(GTK_DIALOG(p_dialog));
        gtk_widget_destroy(p_dialog);
    }
    else
    { /* le dieu et la classe sont compatibles */
        for (j=0;j<9;j++)
        {
            sprintf(ch,"Al_%s",algnm[j]);
            if ((alig&(1<<j))==0)
            { /* on déactive */
                gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,ch)),TRUE);
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,ch))))
                {
                    p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Alignement non compatible\n avec le dieu et la profession\nVoulez que ce soit changé\nautomatiquement ?");
                    switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
                    {
                        case GTK_RESPONSE_NO:
                            gtk_widget_destroy(p_dialog);
                            break ; /* on sort du programme */
                        default : /* rajouter une ligne pour le groupe et l'activer */
                            gtk_widget_destroy(p_dialog);
                            moy_b=0;
                            moy_l=0;
                            nb_moy=0;
                            for(i=0;i<9;i++)
                            {
                                if ((alig&(1<<i))==0)
                                { /* alignement non compatible */
                                }
                                else
                                {
                                    moy_b+=i/3;
                                    moy_l+=i%3;
                                    nb_moy++;
                                }
                            }
                            moy_b/=nb_moy;
                            moy_l/=nb_moy;
                            nb_moy=1<<(moy_b*3+moy_l);
                            if ((nb_moy&alig)==0)
                            {   /* une classe que j'aimerai connaître ! */
                                for(i=0;i<9;i++)
                                {
                                    if ((alig&(1<<i))==0)
                                    { /* alignement non compatible */
                                    }
                                    else
                                    {
                                        sprintf(ch,"Al_%s",algnm[i]);
                                        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,ch)),TRUE);
                                        i=10;
                                    }
                                }
                            }
                            else
                            {
                                sprintf(ch,"Al_%s",algnm[moy_b*3+moy_l]);
                                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,ch)),TRUE);
                            }
                    }
                }
                else
                { /* l'alignement est compatible */
                }
            }
            else
            {
                gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,ch)),FALSE);
            }
        }
    }
}

void verif_psi(FenetrePerso * _perso)
{ /* science 1, devotion : 0 */
    signed short i,j,k,l,natif,niv,natif_p,niv_psi=0;
    char _nom[32];
    GtkWidget * wid, * wid_util;
    signed short * classe=_perso->classe_modif;
    signed short * niv_cl=_perso->niv_classe_modif,nb_mem[2],nb_[2]={0,0},nb_discipline=0,nb_discipline_pris=0,nb_defense=0,nb_def_[2]={0,0}, discipline=0, **ratio_psi;
    char * ch=NULL, *ch1, ch_tmp[LONG];

    if (_perso->psi==1)
    {
        for(i=0;classe[i]!=-1;i++)
        { /* je ne prends en compte que ADD2,ADD n'ayant pas de classe de psi */
            if (CLASSE[classe[i]].add[ADD2].psi._2!=NULL)
            {
                niv_psi=max(niv_psi,niv_cl[i]);
                for(k=0;CLASSE[classe[i]].add[ADD2].psi._2->discipline[k]!=FIN && k<niv_cl[i];k++);
                k--;
                nb_[1]+=CLASSE[classe[i]].add[ADD2].psi._2->science[k];
                nb_[0]+=CLASSE[classe[i]].add[ADD2].psi._2->devotion[k];
                nb_discipline+=CLASSE[classe[i]].add[ADD2].psi._2->discipline[k];
                nb_defense+=CLASSE[classe[i]].add[ADD2].psi._2->defense[k];
            }
            else
            { /* pas de psi */
            }
        }
        nb_mem[0]=nb_[0];
        nb_mem[1]=nb_[1];

        ch=g_strdup("Bilan des choix :\n\n\n");
        wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"notebook_psi"));
        ratio_psi=(signed short **)g_malloc(NB_DISCIPLINE_PSI*sizeof(signed short *));
        for(i=0;i<NB_DISCIPLINE_PSI;i++)
        { /* remise à zéro */
            discipline=0;
            ratio_psi[i]=(signed short *)g_malloc(2*NB_DISCIPLINE_PSI*sizeof(signed short));
            wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"notebook_psi"));
            for (j=0;j<2;j++)
            { /* science dévotion */
                ratio_psi[i][j]=0;
                for(k=0;k<NB_TALENTS_PSI[i][j];k++)
                {
                    sprintf(_nom,"psi_natif_%hu",TALENTS_PSI[i][j][k].clef);
                    wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                    natif=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid));
                    sprintf(_nom,"psi_niveau_%hu",TALENTS_PSI[i][j][k].clef);
                    wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                    niv=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
                    if ( natif!=0 || niv!=0)
                    {
                        if (natif!=0)
                        { /* décompte des points psi pour un talent natif */
                            if (sscanf(TALENTS_PSI[i][j][k].cout,"(i:%hu",&l)==1)
                            { /* une fois le coût de lancement */
                            }
                            else
                            { /* pas de coût de lancement */
                            }
                            for (l=0;TALENTS_PSI[i][j][k].cout[l]!=':' && TALENTS_PSI[i][j][k].cout[l]!=0 ; l++)
                            {
                             ; /* recherche du : dans la chaîne */
                            }
                        }
                        else
                        { /* ce n'est pas une compétence native */
                        }
                        if (niv-natif>0)
                        { /* il y a un developpement non natif */
                            ratio_psi[i][j]+=niv-natif;
                            if (TALENTS_PSI[i][j][k].defense)
                            { /* c'est une défence */
                                nb_def_[j]+=niv-natif;
                                if (nb_def_[1]+nb_def_[0]>nb_defense)
                                {   /* on dépasse le nombre de défenses naturelles => il faut compter la discipline */
                                    discipline=1; /* on marque la prise d'une discipline */
                                }
                                else
                                { /* c'est compris dans le nombre de défences naturelles */
                                }
                            }
                            else
                            {
                                nb_[j]-=niv-natif;
                                discipline=1; /* on marque la prise d'une discipline */
                            }
                        }
                        else
                        { /* ce n'est que du natif */
                        }

                        l=0;
                        while (TALENTS_PSI[i][j][k].prerequis[l]!=0)
                        {
                            sprintf(_nom,"psi_natif_%hu",TALENTS_PSI[i][j][k].prerequis_lien[l]->clef);
                            wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                            natif_p=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wid));
                            sprintf(_nom,"psi_niveau_%hu",TALENTS_PSI[i][j][k].prerequis_lien[l]->clef);
                            wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),_nom);
                            niv=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
                            if (natif_p==0 && natif!=0)
                            {
                                sprintf(ch_tmp,"Problème de dépendence native :\n %s natif implique que\n %s est aussi natif\n\n",TALENTS_PSI[i][j][k].nom,TALENTS_PSI[i][j][k].prerequis_lien[l]->nom);
                                ch1=g_strconcat (ch,ch_tmp,NULL);
                                g_free(ch);
                                ch=ch1;
                            }
                            else if (natif_p==0 && niv==0)
                            {
                                sprintf(ch_tmp,"Problème de dépendence non natif :\n %s appris implique\n %s maîtrisé\n\n",TALENTS_PSI[i][j][k].nom,TALENTS_PSI[i][j][k].prerequis_lien[l]->nom);
                                ch1=g_strconcat(ch,ch_tmp,NULL);
                                g_free(ch);
                                ch=ch1;
                            }
                            l++;
                        }
                    }
                }
            }
            nb_discipline_pris+=discipline;
        }


        if ((nb_discipline+nb_defense)!=0)
        {
            if (nb_discipline_pris==nb_discipline)
            {
                sprintf(ch_tmp,"Bon nombre de discliplines\n   %hd\n\n",nb_discipline);
                ch1=g_strconcat(ch,ch_tmp,NULL);
                g_free(ch);
                ch=ch1;
            }
            else if (nb_discipline_pris<nb_discipline)
            {
                sprintf(ch_tmp,"%hd displine(s) disponible(s)\n sur %hd\n\n",nb_discipline-nb_discipline_pris,nb_discipline);
                ch1=g_strconcat(ch,ch_tmp,NULL);
                g_free(ch);
                ch=ch1;
            }
            else
            {
                sprintf(ch_tmp,"%hd displine(s) prise(s) en trop\n sur %hd\n\n",nb_discipline_pris-nb_discipline,nb_discipline);
                ch1=g_strconcat(ch,ch_tmp,NULL);
                g_free(ch);
                ch=ch1;
            }

            if (nb_def_[0]+nb_def_[1]<=nb_defense)
            {
                if (nb_def_[0]+nb_def_[1]!=nb_defense)
                {
                    sprintf(ch_tmp,"Pas assez de défense prises :\n %hd pour %hd permises\n\n",nb_def_[0]+nb_def_[1],nb_defense);
                    ch1=g_strconcat (ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                else
                { /* exactement le nombre minimum de defenses */
                    sprintf(ch_tmp,"Bon nombre de défense : %hd\n\n",nb_defense);
                    ch1=g_strconcat (ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                if (nb_[0]>0)
                {
                    sprintf(ch_tmp,"Pas assez de dévotions prises :\n %hd pour %hd permises\n\n",nb_mem[0]-nb_[0],nb_mem[0]);
                    ch1=g_strconcat (ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                else if (nb_[0]<0)
                {
                    sprintf(ch_tmp,"Trop de dévotions prises :\n %hd pour %hd permises\n\n",nb_mem[0]-nb_[0],nb_mem[0]);
                    ch1=g_strconcat (ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                else
                { /* bon nombre de dévotions */
                    sprintf(ch_tmp,"Bon nombre de dévotion : %hd\n\n",nb_mem[0]);
                    ch1=g_strconcat (ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                if (nb_[1]>0)
                {
                    sprintf(ch_tmp,"Pas assez de sciences prises :\n %hd pour %hd permises\n\n",nb_mem[1]-nb_[1],nb_mem[1]);
                    ch1=g_strconcat (ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                else if (nb_[1]<0)
                {
                    sprintf(ch_tmp,"Trop de sciences prises :\n %hd pour %hd permises\n\n",nb_mem[1]-nb_[1],nb_mem[1]);
                    ch1=g_strconcat(ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                else
                { /* bon nombre de sciences */
                    sprintf(ch_tmp,"Bon nombre de sciences : %hd\n\n",nb_mem[1]);
                    ch1=g_strconcat(ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
            }
            else if (nb_def_[0]+nb_def_[1]-nb_[0]-nb_[1]==nb_defense)
            { /* bonne quantité prise */
                if (((nb_[0]-nb_def_[0])<0) && ((nb_[1]-nb_def_[1])<0))
                {
                    sprintf(ch_tmp,"Bonne répartition");
                    ch1=g_strconcat(ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                else if ((nb_[0]-nb_def_[0])<0)
                {
                    sprintf(ch_tmp,"Le bon nombre de points est réparti,\n mais, il y a trop de sciences :\n %hd pour %hd dont %d en défense\n\n",nb_mem[1]-nb_[1],nb_mem[1],nb_def_[1]);
                    ch1=g_strconcat(ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                else
                {
                    sprintf(ch_tmp,"Le bon nombre de point est réparti,\n mais, il y a trop de dévotion :\n %hd pour %hd dont %d en défense\n\n",nb_mem[0]-nb_[0],nb_mem[0],nb_def_[0]);
                    ch1=g_strconcat(ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
            }
            else if (nb_def_[0]+nb_def_[1]-nb_[0]-nb_[1]>nb_defense)
            {  /* prise trop importante */
                if (nb_def_[0]>0)
                {
                    sprintf(ch_tmp,"Trop de points sont répartis (%hd),\n il y a trop de sciences :\n %hd pour %hd dont %d en défense.\n\n",nb_def_[0]+nb_def_[1]-nb_[0]-nb_[1]>nb_defense,nb_mem[1]-nb_[1],nb_mem[1],nb_def_[1]);
                    ch1=g_strconcat(ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                else if (nb_def_[1]>0)
                {
                    sprintf(ch_tmp,"Trop de points sont répartis (%hd),\n il y a trop de dévotions :\n %hd pour %hd dont %d en défense.\n\n",nb_def_[0]+nb_def_[1]-nb_[0]-nb_[1]>nb_defense,nb_mem[0]-nb_[0],nb_mem[0],nb_def_[0]);
                    ch1=g_strconcat(ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                else
                {
                    sprintf(ch_tmp,"Trop de points sont répartis (%hd),\n il y a trop de dévotions :\n %hd pour %hd dont %d en défense\n il y a trop de sciences :\n %hd pour %hd dont %d en défense.\n\n",nb_def_[0]+nb_def_[1]-nb_[0]-nb_[1]>nb_defense,nb_mem[0]-nb_[0],nb_mem[0],nb_def_[0],nb_mem[1]-nb_[1],nb_mem[1],nb_def_[1]);
                    ch1=g_strconcat(ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
            }

            nb_[0]=0;
            nb_[1]=0;
            niv=0;
            for(i=0;i<NB_DISCIPLINE_PSI;i++)
            {
                if (nb_[0]<=ratio_psi[i][0] && nb_[1]<=ratio_psi[i][1])
                {
                    niv=i;
                    nb_[0]=ratio_psi[i][0];
                    nb_[1]=ratio_psi[i][1];
                }
                if (ratio_psi[i][1]>(ratio_psi[i][0]*2))
                { /* nombre de dévotion n'est pas au moins le double du nombre de science */
                    sprintf(ch_tmp,"Dans %s,\n le nombre de dévotion %hd\n n'est pas au moins le double\n du nombre de science : %hd\n",DISCIPLINE_PSI[niv].nom,ratio_psi[i][0],ratio_psi[i][1]);
                    ch1=g_strconcat(ch,ch_tmp,NULL);
                    g_free(ch);
                    ch=ch1;
                }
                else
                {/* nombre de dévotion au moins le double du nombre de science */
                }
            }
            j=0;
            for(i=0;i<NB_DISCIPLINE_PSI;i++)
            {
                if (i!=niv)
                {
                    if (nb_[1]<=ratio_psi[i][1])
                    {
                        if (j==0)
                        { /* premier problème trouvé */
                            sprintf(ch_tmp,"La discipline primaire semblait\n être %s\n %hd science(s) et\n %hd devotion(s)\n",DISCIPLINE_PSI[niv].nom,nb_[1],nb_[0]);
                            ch1=g_strconcat(ch,ch_tmp,NULL);
                            g_free(ch);
                            ch=ch1;
                            j=1;
                        }
                        else
                        {
                        }
                        sprintf(ch_tmp," mais %s à au moins\n autant de science(s) : %hd\n",DISCIPLINE_PSI[i].nom,ratio_psi[i][1]);
                        ch1=g_strconcat(ch,ch_tmp,NULL);
                        g_free(ch);
                        ch=ch1;
                    }
                    if (nb_[0]<=ratio_psi[i][0])
                    {
                        if (j==0)
                        { /* premier problème trouvé */
                            sprintf(ch_tmp,"La discipline primaire semblait\n être %s\n %hd science(s) et\n %hd devotion(s)\n",DISCIPLINE_PSI[niv].nom,nb_[1],nb_[0]);
                            ch1=g_strconcat(ch,ch_tmp,NULL);
                            g_free(ch);
                            ch=ch1;
                            j=1;
                        }
                        else
                        {
                        }
                        sprintf(ch_tmp," mais %s à au moins\n autant de dévotion(s) : %hd\n",DISCIPLINE_PSI[i].nom,ratio_psi[i][0]);
                        ch1=g_strconcat(ch,ch_tmp,NULL);
                        g_free(ch);
                        ch=ch1;
                    }
                }
                else
                { /* on ne compare pas la discipline primaire à elle-même */
                }
                g_free(ratio_psi[i]);
            }
            g_free(ratio_psi);
            if (j==0)
            {
                sprintf(ch_tmp,"La discipline primaire est %s\n\n",DISCIPLINE_PSI[niv].nom);
                ch1=g_strconcat(ch,ch_tmp,NULL);
                g_free(ch);
                ch=ch1;
            }
            else
            { /* on a repéré une erreur : le message est déjà écrit */
                sprintf(ch_tmp,"\n");
                ch1=g_strconcat(ch,ch_tmp,NULL);
                g_free(ch);
                ch=ch1;
            }
        }
        else if ((nb_[0]+nb_[1]+nb_def_[0]+nb_def_[1])!=0)
        {
            sprintf(ch_tmp,"Vous avez attribué\n des pouvoirs psy\n à votre personnages\n alors qu'il ne devrait pas\n en avoir");
            ch1=g_strconcat(ch,ch_tmp,NULL);
            g_free(ch);
            ch=ch1;
        }
        else
        {
            sprintf(ch_tmp,"Pas de psi attribué\n c'est normal");
            ch1=g_strconcat(ch,ch_tmp,NULL);
            g_free(ch);
            ch=ch1;
        }


        gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(_perso->modif->builder,"info_psi")),ch);
        g_free(ch);
    }
    else
    {
        /* affichage des psi non activé */
    }
}

void verif_classe(FenetrePerso * _perso)
{
    signed short * classe=_perso->classe_modif;
    signed short _version=_perso->version_modif;
    short race=_perso->race_modif;
    unsigned short i,j,k,l,rouge=GRIS;
    GtkLabel * wid=NULL;
    char ch[128];
    struct_niv_classe * niv_race;
    signed short * max_caract=NULL;
    GtkWidget * widj;

    for(i=0;classe[i]!=-1;i++)
    {
        for (j=0;j<i;j++)
        {
            if (classe[i]==classe[j])
            { /* deux fois la même classe dans la liste */
                j=i; /* le doublon sera repéré si en sortie j>i */
            }
            else
            { /* cette classe ne semble pas être un doublon */
            }
        }
        if (i==j) /* pas un doublon */
        { /* on peut oublier j */
            if ( CLASSE[classe[i]].add[_version].nom && _perso->origine[_version][CLASSE[classe[i]].add[_version].origine]==0)
            { /* la classe existe dans cette version, mais son origine n'est pas sélectionnée : on la sélectionne */
                    strncpy(ch,"Activation de la source ",127);
                    strncat(ch,_ORIGINE[_version][CLASSE[classe[i]].add[_version].origine].nom,127);
                    strncat (ch," pour la classe ",127);
                    strncat (ch,CLASSE[classe[i]].nom,127);
                    _perso->origine[_version][CLASSE[classe[i]].add[_version].origine]=1;
                    dialogue(ch,0);
                    sprintf(ch,"ori_%hd_%hd",_version,CLASSE[classe[i]].add[_version].origine);
                    widj=gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"origine")),ch);
                    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widj),TRUE);
            }
            else
            {
                /* pas besoin de chercher à activer l'origine*/
            }
            sprintf(ch,"classe_%hu_l1",i);
            couleur_item(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"grille_classes")),ch),_perso->couleur_classes[classe[i]]);
            sprintf(ch,"classe_%hu_l2",i);
            couleur_item(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"grille_classes")),ch),_perso->couleur_classes[classe[i]]);
            if (_perso->couleur_classes[classe[i]]!=GRIS)
            {
                rouge=ROUGE;
                sprintf(ch,"nv_max_%hu",i);
                wid=GTK_LABEL(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"grille_classes")),ch));
                gtk_label_set_text(wid,"XXX");
            }
            else
            { /* ce n'est pas une classe interdite pour la race */
                niv_race=RACE[race].add[_version].niv+classe[i];
                sprintf(ch,"nv_max_%hu",i);
                wid=GTK_LABEL(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"grille_classes")),ch));
                if (_version==ADD)
                { /* classe ADD1 avec limite */
                    if (niv_race->_1==NULL)
                    { /* pas de limitation connue pour cette classe */
                        gtk_label_set_text(wid,"-");
                    }
                    else
                    {

                        j=0;
                        k=1;
                        while (niv_race->_1[j]!=NULL && k==1)
                        {
                            k=(niv_race->_1[j][0]>_perso->caract_modif[0]);
                            for(l=1;k!=1 && l<6;l++)
                            {
                                k=k||(niv_race->_1[j][l]>_perso->caract_modif[l]);
                            }
                            if ( k==0 && niv_race->_1[j][0]==18 && _perso->caract_modif[0]==18)
                            { /* VÉRIFIER LE % */
                                k=(niv_race->_1[j][6]>_perso->caract_modif[8]); /* % : 6 pour niv_race et 8 pour caract_modif */
                            }
                            j++;
                        }
                        if (j==0)
                        { /* pas prévu, mais bon ...*/
                            sprintf(ch,"--"); /* -- pour repérer ce cas */
                        }
                        else
                        {
                            sprintf(ch,"max=%hd",niv_race->_1[j-1][7]);
                        }
                        gtk_label_set_text(wid,ch);
                    }
                }
                else
                { /* classe ADD2 avec limite */
                    if (niv_race->_2[0]==-1)
                    { /* classe pas permisse */
                        gtk_label_set_text(wid,"XXX");
                    }
                    else if (niv_race->_2[0]==0)
                    { /* pas de limitation pour cette classe */
                        gtk_label_set_text(wid,"-");
                    }
                    else
                    {
                        if (niv_race->_2[1]==0 || niv_race->_2[1] & ADD2_NIV_MAX_SAURIAL)
                        { /* cas normal du manuel du maître */
                            k=0;
                            for(j=0;j<6;j++)
                            {
                                if (CLASSE[classe[i]].add[_version].dix_caract[j]>0)
                                {
                                    k=max(k,_perso->caract_modif[j]);
                                }
                                else
                                {
                                    /* ce n'est pas une caractéristique principale */
                                }
                            }
                            k=_min(k,25);
                            if (niv_race->_2[1] & ADD2_NIV_MAX_SAURIAL)
                            {
                                sprintf(ch,"max : %hd",niv_race->_2[0]+add_niv_max_saurial[k]);
                            }
                            else
                            {
                                sprintf(ch,"max : %hd",niv_race->_2[0]+add_niv_max_maitre[k]);
                            }
                            gtk_label_set_text(wid,ch);
                        }
                        else
                        {
                            for(j=0;classe[j]!=-1;j++); /* recheche du nombre de classes actives */
                            if ((niv_race->_2[1] & ADD2_NIV_MAX_MONO) && j==1)
                            { /* cas : pas de max si une seule classe */
                                gtk_label_set_text(wid,"-");
                            }
                            else
                            {
                                k=0;
                                for(j=0;j<6;j++)
                                {
                                        if (_perso->sexe_modif==MASCULIN)
                                        {
                                            max_caract=RACE[race].add[_version].max_caract_h;
                                        }
                                        else
                                        {
                                            max_caract=RACE[race].add[_version].max_caract_f;
                                        }
                                        if (CLASSE[classe[i]].add[_version].dix_caract[j]>0)
                                        {
                                            k=max(k,_perso->caract_modif[j]-max_caract[j]);
                                        }
                                        k=_min(k,3);
                                }
                                if (niv_race->_2[1] & ADD2_NIV_MAX_DEMI_HUM)
                                { /* cas guerrier 1/2 orc et 1/2 ogre */
                                    sprintf(ch,"max : %hd",niv_race->_2[0]+add_niv_max_demi_hum[k]);
                                    gtk_label_set_text(wid,ch);
                                }
                                else if (niv_race->_2[1] & ADD2_NIV_MAX_HUMANOIDE)
                                {
                                    sprintf(ch,"max : %hd",niv_race->_2[0]+add_niv_max_humanoide[k]);
                                    gtk_label_set_text(wid,ch);
                                }
                                else
                                {
                                    gtk_label_set_text(wid,"Erreur");
                                }
                            }

                        }
                    }
                }
            }
        }
        else
        { /* un doublon de métier */
            sprintf(ch,"classe_%hu_l1",i);
            couleur_item(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"grille_classes")),ch),ORANGE);
            sprintf(ch,"classe_%hu_l2",i);
            couleur_item(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"grille_classes")),ch),ROUGE);
            rouge=ROUGE;
        }
    }

    if (rouge==ROUGE)
    { /* une classe au moins incompatible avec la race */
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes")),ROUGE);

    }
    else if (classes_race(race,classe,_version)==0)
    { /* groupe classe/race non compatible */
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes")),ORANGE);
    }
    else
    { /* groupe classe/race compatible */
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes")),GRIS);
    }
}

void verif_XP_niv(FenetrePerso * _perso)
{
    signed short _version=_perso->version_modif,vers_OK;
    GtkWidget * wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes"));
    signed long xp,xp_demande;
    signed short cl,niveau,i,j;
    signed short * classe=_perso->classe_modif;
    signed short * niv=_perso->niv_classe_modif;

    for (i=0;classe[i]!=-1;i++)
    { /* parcours des classes */
        cl=classe[i];
        xp=gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtk_grid_get_child_at (GTK_GRID(wid),5,i+1)));
        niveau=niv[i];
        if (CLASSE[cl].add[_version].nom==NULL)
        { /* classe non prévue pour cette version : on prend l'autre */
            vers_OK=(_version+1)%2;
        }
        else
        {
            vers_OK=_version;
        }
        niveau--;
        if (niveau<=0)
        {
            xp_demande=0;
            j=niveau;
        }
        else
        {
            for(j=0;j<niveau && CLASSE[cl].add[vers_OK].XP[j]!=FIN;j++);
            if (CLASSE[cl].add[vers_OK].XP[j]==FIN)
            {
                xp_demande=CLASSE[cl].add[vers_OK].XP[j-1]*(niveau-j+1);
            }
            else
            {
                xp_demande=CLASSE[cl].add[vers_OK].XP[j-1];
            }
        }
        if (xp<xp_demande)
        {
            couleur_item(gtk_grid_get_child_at (GTK_GRID(wid),4,i+1),ROUGE);
        }
        else
        {
            if (j<=0 || CLASSE[cl].add[vers_OK].XP[j]!=-1)
            { /* on était encore dans les clous */
                j++;
            }
            else
            { /* on n'est plus dans les clous */
            }
            if (j==0)
            {
                xp_demande=1;
            }
            else if (CLASSE[cl].add[vers_OK].XP[j-1]==FIN)
            {
                xp_demande=CLASSE[cl].add[vers_OK].XP[j-2]*(niveau-j+3);
            }
            else if (CLASSE[cl].add[vers_OK].XP[j]==FIN)
            {
                xp_demande=CLASSE[cl].add[vers_OK].XP[j-1]*(niveau-j+2);
            }
            else
            {
                xp_demande=CLASSE[cl].add[vers_OK].XP[j-1];
            }

            if (xp<xp_demande)
            {
                couleur_item(gtk_grid_get_child_at (GTK_GRID(wid),4,i+1),GRIS);
            }
            else
            {
                couleur_item(gtk_grid_get_child_at (GTK_GRID(wid),4,i+1),VERT);
            }
        }
    }
}


void verif_pdv(FenetrePerso * _perso)
{
    signed short _version=_perso->version_modif;
    signed short constit=0,constitg,niv,pdv_tot=0,pdv_max=0,pdv_min=0,min_tot=0,bonus,pdv;
    signed short * classe=_perso->classe_modif;
    char * ch_pdv,c,bc=0,ch[32];
    signed short a,b,nb,i;
    GtkWidget * wid, * wid_util;

    /* calcul du bonus de constit */
    wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[CONSTITUTION]));
    constit=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
    if (constit>16)
    {
        if (sscanf(tabl_constit[constit-1][0],"+2(%hd)",&constitg)==1) /* si pas de sortie alors 0 */
        { /*lecture accomplie */
        }
        else
        {
            printf("Erreur de lecture des bonus de pdv (%s) pour une consitution de %hd\n",tabl_constit[constit-1][0],constit);
        }
        constit=2;
    }
    else
    {
        //if (tabl_constit[constit-1][0]==NULL || tabl_constit[constit-1][0][0]==0 || sscanf(tabl_constit[constit-1][0],"%hd",&constitg)==0)
        if (tabl_constit[constit-1][0][0]==0 || sscanf(tabl_constit[constit-1][0],"%hd",&constitg)==0)
        {
            constitg=0; /* prendre en compte le cas vide constitution entre 7 et 14 */
        }
        else
        { /* c'est bon : c'est trouvé */
        }
        constit=constitg;
    }

    wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"boite_PDV"));
    niv=0;
    sprintf(ch,"nb_pdv_%hu",niv);
    while ((wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch))!=NULL)
    {
        min_tot=0;
        pdv_tot=0;
        pdv_max=0;
        pdv_min=0;
        bonus=0;
        pdv=gtk_spin_button_get_value(GTK_SPIN_BUTTON(wid));
        for (i=0;classe[i]!=-1;i++)
        {
            ch_pdv=pdv_classe(classe[i],niv,_version);
            nb=sscanf(ch_pdv,"%hu%c%hu+%c",&a,&c,&b,&bc);
            if (nb>2)
            {
                min_tot+=a;
                pdv_tot+=a*b;
                if (nb==4)
                {
                    if (_version==ADD1)
                    {
                        if (bc=='g')
                        {
                            pdv_max=max(pdv_max,a*b+constitg);
                            pdv_min=pdv_min==0?a:_min(pdv_min,a+constitg);
                            min_tot+=constitg;
                            pdv_tot+=constitg;
                        }
                        else if (bc=='b') /* le barbare double les bonus */
                        {
                            pdv_max=max(pdv_max,a*b+constitg*2);
                            pdv_min=pdv_min==0?a:_min(pdv_min,a+constitg*2);
                            min_tot+=constitg*2;
                            pdv_tot+=constitg*2;
                        }
                        else
                        {
                            pdv_max=max(pdv_max,a*b+constit);
                            pdv_min=pdv_min==0?a:_min(pdv_min,a+constit);
                            min_tot+=constit;
                            pdv_tot+=constit;
                        }
                    }
                    else
                    {
                        if (bc=='g')
                        {
                             bonus=2;
                        }
                        else
                        {
                            bonus=max(bonus,1);
                        }
                        pdv_max=max(pdv_max,a*b);
                        pdv_min=pdv_min==0?a:_min(pdv_min,a);
                    }
                }
                else
                {
                    pdv_max=max(pdv_max,a*b);
                    pdv_min=pdv_min==0?a:_min(pdv_min,a);
                }
            }
            else if (nb>0)
            {
                pdv_max=max(pdv_max,a);
                pdv_min=pdv_min==0?a:_min(pdv_min,a);
                min_tot+=a;
                pdv_tot+=a;
            }
            else
            {
            }
        }/* à la sortie i vaut le nombre de classe */
        if (i==0)
        {
            i=0;
        }
        else
        {
            /* c'est pour éviter une division pas 0 */
        }
        min_tot/=i;
        pdv_tot/=i;
	    /* mettre un niveau plus bas et rajouter le décompte du bonus pour ADD2 */
	    if (_version==ADD2)
        { /* add2 : meilleure possiblilité */
            switch(bonus)
            {
                case 1 : pdv-=constit;
                    break;
                case 2 : pdv-=constitg;
                    break;
                default : /* niveau de classe sans bonus */
                    break;
            }
        }
        else
        { /* ADD1 est déjà compter en cours de route */
        } /* chaque classe : son bonus */
        pdv_max=max(1,pdv_max);
        pdv_min=max(1,pdv_min);
        min_tot=max(1,min_tot);
        pdv_tot=max(1,pdv_tot);
        wid=gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch+3); /* 3 après le début : enlever le nb_ */
        if (pdv<pdv_min || pdv>pdv_max)
        {
            couleur_item(wid,ROUGE);
        }
        else if (pdv<min_tot || pdv>pdv_tot)
        {
            couleur_item(wid,ORANGE);
        }
        else
        {
            couleur_item(wid,GRIS);
        }
        niv++;
        sprintf(ch,"nb_pdv_%hu",niv);
    }
}

void verif_caract(FenetrePerso * _perso)
{
    struct_race * race=RACE+_perso->race_modif;
    unsigned short sexe=_perso->sexe_modif;
    signed short _version=_perso->version_modif;
    signed short * classe=_perso->classe_modif;
    unsigned short * caract=NULL,caract_min[POURCENTAGE+1]={0,0,0,0,0,0,0,0,0},caract_max[POURCENTAGE+1]={25,25,25,25,25,25,25,25,100};
    unsigned short i,j;

    if (sexe==MASCULIN)
    {
        for (j=0;j<=POURCENTAGE;j++)
        {
            caract_min[j]=max(caract_min[j],race->add[_version].min_caract_h[j]);
            caract_max[j]=_min(caract_max[j],race->add[_version].max_caract_h[j]);
        }
    }
    else
    {
        for (i=0;i<=POURCENTAGE;i++)
        {
            caract_min[i]=max(caract_min[i],race->add[_version].min_caract_f[i]);
            caract_max[i]=_min(caract_max[i],race->add[_version].max_caract_f[i]);
        }
    }
    for(i=0;classe[i]!=-1;i++)
    {
        for (j=0;j<=POURCENTAGE;j++)
        {
            caract_min[j]=max(caract_min[j],CLASSE[classe[i]].add[_version].min_caract[j]);
        }
    }

    caract=_perso->caract_modif;
    /* la force : cas différent : colorier aussi le % */
    if (caract[0]<caract_min[0])
    {
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[0])),ROUGE);
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[0])),ROUGE);
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[POURCENTAGE])),ROUGE);
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[POURCENTAGE])),ROUGE);
    }
    else if  (caract[0]>caract_max[0])
    {
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[0])),VERT);
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[0])),VERT);
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[POURCENTAGE])),VERT);
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[POURCENTAGE])),VERT);
    }
    else
    {
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[0])),GRIS);
        couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[0])),GRIS);
        if (caract[0]==18)
        {
            if (caract[POURCENTAGE]<caract_min[POURCENTAGE] && caract_min[0]==18) /* cas surprenant où il faudrait une force > 18 */
            {
                couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[POURCENTAGE])),ROUGE);
                couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[POURCENTAGE])),ROUGE);
            }
            else if (caract[POURCENTAGE]>caract_max[POURCENTAGE] && 18==caract_max[0]) /* cas où on peut avoir une force max de 18, mais avec un % plus petit */
            {
                couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[POURCENTAGE])),VERT);
                couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[POURCENTAGE])),VERT);
            }
            else
            {
                couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[POURCENTAGE])),GRIS);
                couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[POURCENTAGE])),GRIS);
            }
        }
        else
        {
            couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[POURCENTAGE])),GRIS);
            couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[POURCENTAGE])),GRIS);
        }
    }
    for (i=1;i<POURCENTAGE;i++) /* i=0 non pris : c'est la force, vérifiée avant */
    {
        if (caract[i]<caract_min[i])
        {
            couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[i])),ROUGE);
            couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[i])),ROUGE);
        }
        else if  (caract[i]>caract_max[i])
        {
            couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[i])),VERT);
            couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[i])),VERT);
        }
        else
        {
            couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_abr[i])),GRIS);
            couleur_item(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[i])),GRIS);
        }
    }
}

void verif_armes_simplifie(FenetrePerso * _perso)
{
    signed short * arme=_perso->armes_modif;
    GtkWidget * wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_armes"));
    signed short nb_arme,* unefois,i,mag, mag1;
    char ch[LONG];

    unefois=(signed short *)g_malloc(NB_ARME*sizeof(signed short));
    for(i=0;i<NB_ARME;i++)
    {
        unefois[i]=-1;
    }
    for(nb_arme=0;arme[nb_arme]!=-1;nb_arme++)
    {
        if (unefois[arme[nb_arme]]==-1)
        { /* première fois qu'on trouve cette arme */
            unefois[arme[nb_arme]]=nb_arme;
        }
        else
        {
            sprintf(ch,"mag_arme_%hd",nb_arme);
            mag=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
            if (unefois[arme[nb_arme]]==-2)
            { /* c'est au moins le troisième item de cette arme qui peut rester */
                for (i=0;i<nb_arme;i++)
                {
                    if (arme[nb_arme]==arme[i])
                    { /* on a trouvé une autre fois où on a la même arme */
                        sprintf(ch,"mag_arme_%hd",i);
                        mag1=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
                        if (mag==mag1)
                        { /* deux fois le même : on enlève */
                            enleve_ligne_arme(_perso->modif, nb_arme+1);
                            donne_arme_modif(_perso);
                            arme=_perso->armes_modif;
                            nb_arme--;
                            i=nb_arme;
                            _perso->d_m=1;
                        }
                        else
                        { /* les deux niveaux de magie sont différents : on conserve */
                        }
                    }
                    else
                    { /* armes différentes */
                    }
                }
            }
            else /* c'est la seconde fois qu'on tombe sur cette arme : ce cas est conservé pour diminuer le temps de calcul */
            { /* ça peut être valable si le personnage utilse l'arme avec deux niveaux de magie */
                sprintf(ch,"mag_arme_%hd",unefois[arme[nb_arme]]); /* le niveau de magie de l'autre arme */
                mag1=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
                if (mag==mag1)
                { /* deux fois le même : on enlève */
                    enleve_ligne_arme(_perso->modif, nb_arme+1);
                    donne_arme_modif(_perso);
                    arme=_perso->armes_modif;
                    nb_arme--;
                    _perso->d_m=1;
                }
                else
                { /* les deux niveaux de magie sont différents : on conserve */
                    unefois[arme[nb_arme]]=-2;
                }
            }
        }
    }
}

void verif_competence(FenetrePerso * _perso)
{
    signed short * classe=_perso->classe_modif;
    signed short * competence = _perso->competence_modif;
    signed short * niveau=_perso->niv_classe_modif;
    GtkWidget * wid_util;
    char ch[LONG];
    signed short nb_comp=0, nb,k;

    for (k=0;classe[k]!=-1;k++)
    {
        if (CLASSE[classe[k]].add[ADD2].nom!=NULL)
        {
            nb=CLASSE[classe[k]].add[ADD2].progression_competences[0]+niveau[k]/CLASSE[classe[k]].add[ADD2].progression_competences[1];
            nb_comp=max(nb_comp,nb);
        }
        else
        { /* classe uniquement ADD1 => pas de compét"ences */
        }
    }
    nb=gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,nom_min[INTELLIGENCE]))));
    sscanf(tabl_inte2[nb-1][0],"%hd",&k);
    nb_comp+=k;
    nb=nb_comp;

    wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_competence"));
    for(k=0;competence[k]!=-1;k++)
    {
        sprintf(ch,"niv_competence_%hd",k);
        if (_perso->couleur_competence[competence[k]]==GRIS)
        {  /* competences permises pour l'ensemble de classe */
            nb-=(COMPETENCE[competence[k]].unite)*gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
        }
        else if (_perso->couleur_competence[competence[k]]==ORANGE)
        {  /* competences non permises pour l'ensemble de classe, mais, c'est une compétence normale */
            nb-=(COMPETENCE[competence[k]].unite)*gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)))*2;
        }
        else
        {  /* competences non permises pour l'ensemble de classe, et, c'est une compétence anormale */
            nb-=(COMPETENCE[competence[k]].unite)*gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)))*3;
        }
        sprintf(ch,"competence_%hd_l1",k);
        couleur_item(GTK_WIDGET(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)),_perso->couleur_competence[competence[k]]);
        sprintf(ch,"competence_%hd_l2",k);
        couleur_item(GTK_WIDGET(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)),_perso->couleur_competence[competence[k]]);
    }

    if (nb==0)
    { /* bon nombre de compétences */
        sprintf(ch,"Le bon nombre d'unité de compétences a été attribué");
    }
    else if (nb==-1)
    { /* trop de compétences choisies */
        sprintf(ch,"1 slot de compétences en trop a été attribué pour un maximum de %hd",nb_comp);
    }
    else if (nb<0)
    { /* trop de compétences choisies */
        sprintf(ch,"%hd slots de compétences en trop ont été attribués pour un maximum de %hd",-nb,nb_comp);
    }
    else if (nb==1)
    { /* il reste des compétences à prendre */
        sprintf(ch,"1 slot de competences peuvent être encore attribués pour un maximum de %hd\n",nb_comp);
    }
    else
    { /* il reste des compétences à prendre */
        sprintf(ch,"%hd slots de competences peuvent être encore attribués pour un maximum de %hd\n",nb,nb_comp);
    }
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(_perso->modif->builder,"nb_competences")),ch);

}



void verif_arme(FenetrePerso * _perso)
{
    signed short version_=_perso->version_modif;
    signed short * classe=_perso->classe_modif;
    signed short * arme=_perso->armes_modif;
    signed short * niveau=_perso->niv_classe_modif;
    signed short nb_arme,* unefois,i,compt_tot=0,compt_max=0,compt=0,drap=0,k;
    GtkWidget * wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_classes")), *wid;
    char ch[LONG];
    unsigned short coul;
    GtkWidget *p_dialog = NULL;
    signed short a,ga,drap_arme=0,arme_seule,groupe_1,simplification=0;

    for(nb_arme=0;arme[nb_arme]!=-1;nb_arme++);
    /* proposition d'élimination d'un item à 0 de niveau à rajouter ? */
    unefois=(signed short *)g_malloc(NB_ARME*sizeof(signed short));
    /* on initialise toutes les armes à non prises */
    for(arme_seule=0;ARME[arme_seule].groupe==1;arme_seule++)
    {
        unefois[arme_seule]=NON;
    } /* arme_seule : nb d'arme dans le tableau */
    for(groupe_1=arme_seule;ARME[groupe_1].groupe==2;groupe_1++)
    {
        unefois[groupe_1]=NON;
    } /* dans le tableau des armes, de arme_seule à groupe_1 : petit groupe */
    for(i=groupe_1;i<NB_ARME;i++)
    {
        unefois[i]=NON;
    } /* dans tableau arme, de groupe_1 à NB_ARME : grand groupe */

    /* il a droit à combien d'arme */
    for(k=0; classe[k]!=-1; k++)
    {
        if (CLASSE[classe[k]].add[version_].nom!=NULL)
        {/* décompte du nombre d'armes permis */
            compt=CLASSE[classe[k]].add[version_].nb_arme[0]+niveau[k]/CLASSE[classe[k]].add[version_].nb_arme[2];
            compt_tot=max(compt_tot,compt);
            compt_max+=compt;
            if (compare_sans_casse("guerrier",CLASSE[classe[k]].add[version_].nom)==0)
            { /* drapeau pour la spécialisation */
                drap=1;
            }
            else
            {
            }
        }
        else  /* MÊME SI C'EST OFFICIELLEMENT une classe d'une version uniquement, je la prends en compte pour l'autre */
        {
            compt=CLASSE[classe[k]].add[(version_+1)%2].nb_arme[0]+niveau[k]/CLASSE[classe[k]].add[(version_+1)%2].nb_arme[2];
            compt_tot=max(compt_tot,compt);
            compt_max+=compt;
        }
    }
    if (k>1)
    { /* pour être spécialisé, il faut être guerrier monoclassé => impossible d'être spécialisé pour un biclassé */
        drap=0;
    }
    else
    { /* on laisse la spécialisation si c'est la mono classe de guerrier */
    }

    compt=0;
    wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_armes"));
    for(k=0;k<nb_arme;k++)
    {
        sprintf(ch,"niv_arme_%hd",k);
        if (ARME[arme[k]].groupe==1) /* groupe 2 => petit groupe d'armes, groupe 3 => grand groupe d'armes */
        {   /* c'est une arme */
            if (unefois[arme[k]]!=NON) /* on ne retient que la première fois non nulle */
            {
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)),unefois[arme[k]]); /* on remet la même maîtrise de l'arme que la première fois */
            }
            else
            {
                unefois[arme[k]]=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
                compt+=unefois[arme[k]];
            }
            if ((i=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch))))>1)
            { /* niveau de maitrise = spécialisation */
                if (drap) /* il peut se spécialiser */
                {
                    if (i>2 && version_==ADD2) /* seul les guerriers purs peuvent se spécialiser */
                    {                                                                                                                           /* la double spé n'est que pour ADD1 */
                        coul=ORANGE;
                    }
                    else
                    {                                                                                                                           /* la double spé n'est que pour ADD1 */
                        coul=GRIS;
                    }
                }
                else /* il ne peut pas se précialiser */
                {                                                                                                                           /* la double spé n'est que pour ADD1 */
                    if (i==3)
                    {
                        coul=ROUGE;
                    }
                    else
                    {
                        coul=ORANGE;
                    }
                }
            }
            else
            {       /* pas de spécialisation choisie */                                                                                                                     /* la double spé n'est que pour ADD1 */
                coul=GRIS;
            }
        }
        else /* c'est un groupe */
        {
            coul=GRIS;
            if (unefois[arme[k]]!=NON) /* on ne retient que la première fois non nulle */
            {
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)),unefois[arme[k]]); /* on remet la même maîtrise de l'arme que la première fois */
            }
            else
            {
                unefois[arme[k]]=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)));
            }
            if (unefois[arme[k]]>0)
            { /* si le groupe est connu */
                compt+=ARME[arme[k]].groupe; /* 2 petit groupe et 3 grand groupe */
                if (unefois[arme[k]]>1) /* pas de spécialisation dans un groupe */
                {
                    gtk_spin_button_set_value(GTK_SPIN_BUTTON(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch))),1);
                    unefois[arme[k]]=1;
                }
                else
                { /* le groupe est connu avec un niveau de 1 */
                }
            }
            else
            { /* le groupe est affiché, mais non connu */
            }
            sprintf(ch,"mag_arme_%hd",k);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)),0); /* pas de groupes magiques */
            sprintf(ch,"arme_autre_t_%hd",k);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)),0); /* pas de autre aux groupes */
            sprintf(ch,"arme_autre_d_%hd",k);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)),0); /* pas de autre aux groupes */
        }
        sprintf(ch,"niv_arme_%hd_l1",k);
        couleur_item(GTK_WIDGET(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)),coul);

        /* vérifier la validité de l'arme */
        sprintf(ch,"arme_%hd_l1",k);
        couleur_item(GTK_WIDGET(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)),_perso->couleur_armes[arme[k]]);
        sprintf(ch,"arme_%hd_l2",k);
        couleur_item(GTK_WIDGET(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),ch)),_perso->couleur_armes[arme[k]]);
    }

    /* vérification si des armes sont dans des groupes appris */
    for (ga=arme_seule;ga<NB_ARME;ga++) /* parcours des groupes */
    {
        if (unefois[ga]>0 && ARME[ga].pt_groupe!=NULL) /* rechercher des groupes connus uniquement */
        {                               /* + groupe non vide ne devrait pas arriver si les fichiers ods sont bien faits */
            for (a=0;a<groupe_1;a++) /* parcours des armes et des petits groupes */
            {
                if (unefois[a]>0 ) /* l'arme est connue ou pas encore repérée comme étant dans un groupe */
                {
                    for (i=0;ARME[ga].pt_groupe[i]!=FIN && a!=ARME[ga].pt_groupe[i];i++);
                    if (ARME[ga].pt_groupe[i]!=FIN) /* on sort à cause d'une égalité : arme trouvée dans le groupe  */
                    {
                        if (ARME[a].groupe==2)
                        { /* c'est un petit groupe dans un grand groupe */
                            compt-=2;
                            unefois[a]=NON;
                            simplification=1; /* on pourra simplifier par la suite */
                        }
                        else
                        { /* pas besoin de compter la connaissance de l'arme */
                            compt--;
                            unefois[a]*=-1; /* arme déjà comptée dans un groupe */
                        }
                    }
                    else
                    {/* l'arme n'est pas trouvée dans le groupe */
                    }
                }
                else
                { /* l'arme n'est pas connue */
                }
            }
       }
       else
       { /* ce n'est pas un groupe connu */
       }
    }
    if  (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(_perso->modif->builder,"active_groupe"))))
    {  /* vérification si on ne trouve pas plusieurs armes dans un groupe alors qu'il n'est pas considéré comme su */
        for (ga=groupe_1;ga<NB_ARME;ga++)
        { /* parcours des grands groupes */
            if (unefois[ga]==NON && ARME[ga].pt_groupe!=NULL) /* c'est un grand groupe non su */
            {                         /* et non vide : ne devrait pas arriver si les fichiers ods sont bien faits */
                k=0;
                for (a=0;a<groupe_1;a++) /* parcours des armes et des petits groupes */
                {
                    if (unefois[a]>0) /* l'arme est connue et pas dans un groupe */
                    {
                        for (i=0;ARME[ga].pt_groupe[i]!=FIN && a!=ARME[ga].pt_groupe[i];i++);
                        if (ARME[ga].pt_groupe[i]!=FIN)
                        { /* on est sorti parce qu'on a trouvé l'arme */
                            k+=ARME[a].groupe; /* arme ou petit groupe */
                        }
                        else
                        { /* l'arme n'est pas dans le groupe */
                        }
                    }
                    else
                    { /* arme non connue ou déjà dans un groupe */
                    }
                }
                if (k>=3)
                {
                    sprintf(ch,"Voulez-vous valider le groupe %s.\nIl contient assez d'armes connu pour être rentable.",ARME[ga].nom);
                    p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
                    switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
                    {
                        case GTK_RESPONSE_NO:
                            gtk_widget_destroy(p_dialog);
                            break ; /* on sort du programme */
                        default : /* rajouter une ligne pour le groupe et l'activer */
                            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_armes"));
                            for (i=1;gtk_grid_get_child_at (GTK_GRID(wid),1,i)!=NULL;i++);
                            i--;
                            ajoute_ligne_arme(_perso,i,ga,1,0,0,0);
                            gtk_widget_destroy(p_dialog);
                            gtk_widget_show_all(wid);
                            donne_arme_modif(_perso);
                            arme=_perso->armes_modif;
                            compt+=3;
                            for (a=0;a<groupe_1;a++) /* parcours des armes et des petits groupes */
                            {
                                if (unefois[a]>0) /* l'arme est connue */
                                {
                                    for (i=0;ARME[ga].pt_groupe[i]!=FIN && a!=ARME[ga].pt_groupe[i];i++);
                                    if (ARME[ga].pt_groupe[i]!=FIN) /* on sort à cause d'une égalité */
                                    {
                                        if (ARME[a].groupe==1)
                                        { /* pas besoin de compter la connaissance de l'arme */
                                            compt--;
                                            unefois[a]*=-1; /* armes déjà comptée dans un groupe */
                                        }
                                        else
                                        {
                                            compt-=2;
                                            unefois[a]=NON;
                                        } /* ne pas proposer ici la simplification car on passera de nouveau dans cette fonction après la modification */
                                    }
                                    else
                                    { /* l'arme n'est pas dans le groupe */
                                    }
                                }
                            }
                            drap_arme=2;
                            /* ga=NB_ARME; forcer la sortie rapide */
                            break;
                    }
                }
            }
            else
            { /* ce n'est pas un groupe de niveau 3 connu non vide */
            }
        }
        for (ga=arme_seule;ga<groupe_1;ga++)
        { /* parcours des petits gruopes */
            if (unefois[ga]==NON && ARME[ga].pt_groupe!=NULL ) /* c'est un petit groupe non su */
            {                          /* et non vide : ne devrait pas arriver si les fichiers ods sont bien faits */
                k=0;
                for (a=0;ARME[a].groupe==1;a++) /* parcours des armes */
                {
                    if (unefois[a]>0) /* l'arme est connue */
                    {
                            for (i=0;ARME[ga].pt_groupe[i]!=FIN && a!=ARME[ga].pt_groupe[i];i++);
                            if (ARME[ga].pt_groupe[i]!=FIN) /* on sort à cause d'une égalité */
                            {
                                k++;
                            }
                    }
                    else
                    { /* arme connue */
                    }
                }
                if (k>=2)
                {
                    sprintf(ch,"Voulez-vous valider le groupe %s.\nIl contient assez d'armes connu pour être rentable.",ARME[ga].nom);
                    p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
                    switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
                    {
                        case GTK_RESPONSE_NO:
                            gtk_widget_destroy(p_dialog);
                            break ; /* on sort du programme */
                        default : /* rajouter une ligne pour le groupe et l'activer */
                            wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_armes"));
                            for (i=1;gtk_grid_get_child_at (GTK_GRID(wid),1,i)!=NULL;i++);
                            i--;
                            ajoute_ligne_arme(_perso,i,ga,1,0,0,0);
                            gtk_widget_destroy(p_dialog);
                            gtk_widget_show_all(wid);
                            donne_arme_modif(_perso);
                            arme=_perso->armes_modif;
                            compt+=2;
                            for (a=0;a<arme_seule;a++) /* parcours des armes */
                            {
                                if (unefois[a]>0) /* l'arme est connue */
                                {
                                    for (i=0;ARME[ga].pt_groupe[i]!=FIN && a!=ARME[ga].pt_groupe[i];i++);
                                    if (ARME[ga].pt_groupe[i]!=FIN) /* on sort à cause d'une égalité */
                                    { /* pas besoin de compter la connaissance de l'arme */
                                        compt--; /* o nn'enlève que des armes simples */
                                        unefois[a]*=-1; /* armes déjà comptée dans un groupe */
                                    }
                                    else
                                    { /* l'arme n'est pas dans le groupe */
                                    }
                                }
                            }
                            drap_arme=2;
                            /* ga=NB_ARME; forcer la sortie rapide */
                            break;
                    }
                }
            }
        }
    }

    if (compt>compt_max && compt_tot!=compt_max)
    {
        sprintf(ch,"Il y a %hu armes en trop.\nEn enlever %hu semblerait indispensable.",compt-compt_tot,compt-compt_max);
    }
    else if (compt>compt_tot)
    {
        sprintf(ch,"\nIl y a %hu armes en trop.",compt-compt_tot);
    }
    else if (compt==compt_tot)
    {
        sprintf(ch,"\nLe bon nombre d'armes est attribué");
    }
    else
    {
        sprintf(ch,"\nIl reste %hu armes à répartir",compt_tot-compt);
    }
    gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(_perso->modif->builder,"nb_armes")),ch);

    if (simplification==1)
    {
        p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Vous pouvez simplifier votre feuille en enlevant au moins un petit groupe\n déjà inclu dans un grand groupe");
        switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
        {
            case GTK_RESPONSE_NO:
                gtk_widget_destroy(p_dialog);
                break ; /* on sort du programme */
            default : /* rajouter une ligne pour le groupe et l'activer */
                wid=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"grille_armes"));
                for (i=nb_arme-1;i>=0;i--)
                {
                    if (unefois[arme[i]]==NON && ARME[arme[i]].groupe==2)
                    { /* petit groupe inutile */
                        enleve_ligne_arme(_perso->modif,i+1);
                    }
                    else
                    { /* ce n'est pas un petit groupe désectionné */
                    }
                }
                gtk_widget_destroy(p_dialog);
                break;
        }
    }
    else
    { /* pas de simplification de petit groupe possible */
    }

    g_free(unefois);
    if (drap_arme==2)
    { /* on a changer quelque chose : tout reprendre en compte */
        drap_arme=0;
        verif_arme(_perso);
    }
    else
    {
         drap_arme=0;
    }
}

void verif_dieu(FenetrePerso * _perso)
{
    signed short version_=_perso->version_modif;
    signed short * classe=_perso->classe_modif;
    signed short i,j,k;
    GtkComboBox * combo;
    GtkWidget *p_dialog = NULL, * wid_util;

    for(k=0; classe[k]!=-1; k++)
    {
        if (CLASSE[classe[k]].add[version_].pantheon!=NULL) /* si on imose un dieu, c'est quon a imposé un panthéon */
        {
            combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"bt_pantheon")),"pantheon"));
            i=pantheon_entier(CLASSE[classe[k]].add[version_].pantheon);
            if (i<0)
            {
                i=0;
            }
            else
            { /* le pantheon a été trouvé */
                if (CLASSE[classe[k]].add[version_].dieu!=NULL)
                {
                    j=dieu_entier(CLASSE[classe[k]].add[version_].dieu,i);
                }
                else
                {
                    j=-1; /* pâs de dieu imposé */
                }
                if (CLASSE[classe[k]].add[version_].pantheon!=NULL)
                {
                    combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"bt_pantheon")),"pantheon"));
                    if (compare_sans_casse(gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo)),CLASSE[classe[k]].add[version_].pantheon)==0)
                    {
                        /* bon pantheon */
                        if (CLASSE[classe[k]].add[version_].dieu!=NULL)
                        { /* dieu imposé ? */
                            combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"bt_dieu")),"dieu"));
                            if (compare_sans_casse(CLASSE[classe[k]].add[version_].dieu,gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo)))==0)
                            {
                                /* bon dieu */
                            }
                            else
                            {
                                p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Dieu non compatible avec la profession\nVoulez-vous en changer automatiquement ?");
                                switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
                                {
                                    case GTK_RESPONSE_NO:
                                        gtk_widget_destroy(p_dialog);
                                        break ; /* on sort du programme */
                                    default : /* rajouter une ligne pour le groupe et l'activer */
                                        gtk_widget_destroy(p_dialog);
                                        gtk_combo_box_set_active (combo,j);
                                }
                            }
                        }
                        else
                        {
                            /* pas de dieu imposé */
                        }
                    }
                    else
                    {
                        p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->modif->window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"Panthéon non compatible avec la profession\nVoulez-vous adapter le panthéon et le dieu automatiquement ?");
                        switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
                        {
                            case GTK_RESPONSE_NO:
                                gtk_widget_destroy(p_dialog);
                                break ; /* on sort du programme */
                            default : /* rajouter une ligne pour le groupe et l'activer */
                                gtk_widget_destroy(p_dialog);
                                combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(gtk_builder_get_object(_perso->modif->builder,"bt_pantheon")),"pantheon"));
                                gtk_combo_box_set_active (combo,i);
                                if (j>0)
                                {
                                    wid_util=GTK_WIDGET(gtk_builder_get_object(_perso->modif->builder,"bt_dieu"));
                                    combo=GTK_COMBO_BOX(gtk_get_widget_by_name(GTK_CONTAINER(wid_util),"dieu"));
                                    if (combo)
                                    {
                                        gtk_widget_destroy(GTK_WIDGET(combo));
                                    }
                                    else
                                    { /* le menu n'est pas installé : c'est l'ouverture de la fenêtre */
                                    }
                                    combo=GTK_COMBO_BOX(creation_combo_dieux(i));
                                    gtk_box_pack_start(GTK_BOX(wid_util),GTK_WIDGET(combo),1,0,0);
                                    gtk_combo_box_set_active (combo,j);
                                }
                                else
                                {
                                    /* pas de dieu à adapter */
                                }
                        }
                    }
                }
                else
                {
                    /* on  n'impose pas de panthéon, donc pas de dieu */
                }
            }
        }
        else
        {
            /* ni dieu ni panthéon imposé */
        }
    }

}


char * pdv_classe(signed short _classe,signed short niv,signed short _version)
{
    char * retour;

    niv=_min(niv,19); /* en fait le niveau est niv+1 */
    if (CLASSE[_classe].add[_version].nom!=NULL)
    {
        retour=CLASSE[_classe].add[_version].pdv[niv];
    }
    else
    {   /* si la classe n'existe pas dans cette version, on prend l'autre */
        retour=CLASSE[_classe].add[(_version+1)%2].pdv[niv];
    }

    return retour;
}

