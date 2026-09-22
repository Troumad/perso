/*
    Génération de feuille de personnage complète AD&D 1 et 2
    Génération aléatoire de personnage

    Copyright (C) 2011-2016 Bernard SIAUD

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
*/

#include "include.h"
#include <string.h>
#include <zip.h>
#include <math.h>

void ecrit_ch(FILE * sortie, char * ch);
void force_xml(GMarkupDomNode * ooo,gchar * ch2,gchar * ch3,gchar * ch4,gchar * ch5,gchar * ch6);
void intelligence_xml(GMarkupDomNode * ooo,unsigned short inte,unsigned short version_);
void sagesse_xml(GMarkupDomNode * ooo,unsigned short sage);
void dexterite_xml(GMarkupDomNode * ooo,unsigned short dexterite,signed short vers_);
void constitution_xml(GMarkupDomNode * ooo,unsigned short constit);
void charisme_xml(GMarkupDomNode * ooo,gchar * ch2,gchar * ch3,gchar * ch4);
void vade_retro(signed short niv, signed short vers_, GMarkupDomNode * ooo );
signed long but_xp_classe(struct_classe * s_c,struct_race * s_r,unsigned short niv,unsigned short version_t);
void trouve_modif(char * ch,unsigned short * car ,float * coef);
void enregistre_perso_sous(GtkWidget *ChildWidget, FenetrePerso * _perso);
void xml_ecrit_dernier_texte_f(GMarkupDomNode * node,gchar * ch); /* Gestion des sauts de ligne dans un texte */

void xml_ecrit_dernier_texte_f(GMarkupDomNode * node,gchar * ch1)
{     /* avec gestion des sauts de ligne dans un texte */
    char * ch0=g_strdup(ch1); /* mémoriser pour effacer à la fin */
    char * ch; /* le début de ce qu'il reste à afficher */
    char * pt=ch; /* pointe sur le caractère traité */

    if (!(*ch0))
    { /* vide */
    }
    else
    {
        ch=ch0;
        while (* pt)
        {
            if (*pt=='\n')
            {
                *pt=0;
                /* insertion du saut de ligne avant le nouveau paragraphe */
                xml_ajoute_fin(node,"text:line-break");
                /* ajout d'une node texte à la fin                        */
                xml_ajoute_fin_texte(node);
                /* on écrit dans cette dernière node le texte souhaité    */
                xml_ecrit_dernier_texte(node,ch);
                ch=pt+1;
            }
            pt++;
        }
        /* tout copier depuis le dernier saut de ligne */
        if (ch!=pt) /* reste t’il des choses à afficher ? */
        {
                xml_ajoute_fin(node,"text:line-break");
                xml_ajoute_fin_texte(node);
                xml_ecrit_dernier_texte(node,ch);
        }
        g_free(ch0);
    }
}

char * verif_ext(char * _nom,char * ext)
{
    signed short i;
    char * pt=NULL;
    signed short drap=1;

    if (strlen(ext)<strlen(_nom))
    {
        pt=_nom+strlen(_nom)-strlen(ext);
        for(i=strlen(ext)-1;i>=0;i--)
        {
            if (pt[i]!=ext[i])
            {
                drap=0;
                i=0; /* sortir de la boucle */
            }
            else
            { /* même valeur */
            }
        }
        if (drap==0)
        {
            pt=(char *)g_malloc((strlen(ext)+strlen(_nom)+1)*sizeof(char));
            sprintf(pt,"%s%s",_nom,ext);
            /*g_free(_nom);*/
        }
        else
        { /* c'est bon, on a l'extention */
            pt=g_strdup(_nom);
            /*pt=_nom;*/
        }
    }
    else
    {
        pt=(char *)g_malloc((strlen(ext)+strlen(_nom)+1)*sizeof(char));
        sprintf(pt,"%s%s",_nom,ext);
        /*g_free(_nom);*/
    }
    return pt;
}
void force_xml(GMarkupDomNode * ooo,gchar * ch2,gchar * ch3,gchar * ch4,gchar * ch5,gchar * ch6)
{
    modif_xml(g_markup_dom_nom (ooo,"force2","name"),ch2);
    modif_xml(g_markup_dom_nom (ooo,"force3","name"),ch3);
    modif_xml(g_markup_dom_nom (ooo,"force4","name"),ch4);
    modif_xml(g_markup_dom_nom (ooo,"force5","name"),ch5);
    modif_xml(g_markup_dom_nom (ooo,"force6","name"),ch6);
}

void intelligence_xml(GMarkupDomNode * ooo,unsigned short inte,unsigned short version_)
{
    inte--;
    if (version_==ADD1)
    {
        /*if (tabl_inte1[inte][0][1]==0 && tabl_inte1[inte][0][0]>'1')
            modif_xml(g_markup_dom_nom (ooo,"nb_lang_int","name"),"Nombre de langues");*/
        modif_xml(g_markup_dom_nom (ooo,"intelligence2","name"),tabl_inte1[inte][0]);
        modif_xml(g_markup_dom_nom (ooo,"intelligence3","name"),tabl_inte1[inte][1]);
        modif_xml(g_markup_dom_nom (ooo,"intelligence4","name"),tabl_inte1[inte][2]);
        modif_xml(g_markup_dom_nom (ooo,"intelligence5","name"),tabl_inte1[inte][3]);
        modif_xml(g_markup_dom_nom (ooo,"intelligence6","name"),tabl_inte1[inte][4]);
        modif_xml(g_markup_dom_nom (ooo,"intelligence6_com","name"),"nb min sort");
    }
    else
    {
        /*if (tabl_inte1[inte][0][1]==1 && tabl_inte2[inte][0][0]>'1')
            modif_xml(g_markup_dom_nom (ooo,"nb_lang_int","name"),"Nombre de langues");*/
        modif_xml(g_markup_dom_nom (ooo,"intelligence2","name"),tabl_inte2[inte][0]);
        modif_xml(g_markup_dom_nom (ooo,"intelligence3","name"),tabl_inte2[inte][1]);
        modif_xml(g_markup_dom_nom (ooo,"intelligence4","name"),tabl_inte2[inte][2]);
        modif_xml(g_markup_dom_nom (ooo,"intelligence5","name"),tabl_inte2[inte][3]);
        modif_xml(g_markup_dom_nom (ooo,"intelligence6","name"),tabl_inte2[inte][4]);
    }
}

void sagesse_xml(GMarkupDomNode * ooo,unsigned short sage)
{
    char ch[LONG];
    signed short i;

    sage--;
    modif_xml(g_markup_dom_nom (ooo,"sagesse2","name"),tabl_sagesse[sage][0]);
    sscanf(tabl_sagesse[sage][0],"%hd",&i);
    if (i<0)
    {
        sprintf(ch,"%hd (malus)",i);
        modif_xml(g_markup_dom_nom (ooo,"ssag","name"),ch);
    }
    else
    {
        modif_xml(g_markup_dom_nom (ooo,"ssag","name"),tabl_sagesse[sage][0]);
    }
    modif_xml(g_markup_dom_nom (ooo,"sagesse3","name"),tabl_sagesse[sage][1]);
    modif_xml(g_markup_dom_nom (ooo,"sagesse4","name"),tabl_sagesse[sage][2]);
    modif_xml(g_markup_dom_nom (ooo,"sagesse5","name"),tabl_sagesse[sage][3]);
    if (tabl_sagesse[sage][5][0]==0 || tabl_sagesse[sage][5][0]=='0')
    {
        modif_xml(g_markup_dom_nom (ooo,"sagesse6","name"),tabl_sagesse[sage][4]);
    }
    else
    {
        if (tabl_sagesse[sage][6][0]==0 || tabl_sagesse[sage][6][0]=='0')
        {
             sprintf(ch,"%s+%s",tabl_sagesse[sage][4],tabl_sagesse[sage][5]);
        }
        else if (tabl_sagesse[sage][7][0]==0 || tabl_sagesse[sage][7][0]=='0')
        {
             sprintf(ch,"%s+%s+%s",tabl_sagesse[sage][4],tabl_sagesse[sage][5],tabl_sagesse[sage][6]);
        }
        else
        {
            sprintf(ch,"%s+%s+%s+%s",tabl_sagesse[sage][4],tabl_sagesse[sage][5],tabl_sagesse[sage][6],tabl_sagesse[sage][7]);
        }
        modif_xml(g_markup_dom_nom (ooo,"sagesse6","name"),ch);
    }
    modif_xml(g_markup_dom_nom (ooo,"sagesse7","name"),tabl_sagesse[sage][8]);
    modif_xml(g_markup_dom_nom (ooo,"sagesse8","name"),tabl_sagesse[sage][10]);
}

void dexterite_xml(GMarkupDomNode * ooo,unsigned short dexterite, signed short vers_)
{
    signed short i;
    gchar ch[16];

        dexterite--;
        modif_xml(g_markup_dom_nom (ooo,"dexterite2","name"),tabl_dexterite[vers_][dexterite][0]);
        modif_xml(g_markup_dom_nom (ooo,"dexterite3","name"),tabl_dexterite[vers_][dexterite][1]);
        modif_xml(g_markup_dom_nom (ooo,"dexterite4","name"),tabl_dexterite[vers_][dexterite][2]);
        sscanf(tabl_dexterite[vers_][dexterite][2],"%hd",&i);
        if (i>0)
        {
            sprintf(ch,"%hd (malus)",-i);
        }
        else
        {
            sprintf(ch,"%+hd",-i);
        }
        modif_xml(g_markup_dom_nom (ooo,"sdext","name"),ch);
        modif_xml(g_markup_dom_nom (ooo,"dext1","name"),tabl_dexterite[vers_][dexterite][2]);
        modif_xml(g_markup_dom_nom (ooo,"dext2","name"),tabl_dexterite[vers_][dexterite][2]);
}

void constitution_xml(GMarkupDomNode * ooo,unsigned short constit)
{
    constit--;
    modif_xml(g_markup_dom_nom (ooo,"constitution2","name"),tabl_constit[constit][0]);
    modif_xml(g_markup_dom_nom (ooo,"constitution3","name"),tabl_constit[constit][1]);
    modif_xml(g_markup_dom_nom (ooo,"constitution4","name"),tabl_constit[constit][2]);
    modif_xml(g_markup_dom_nom (ooo,"constitution5","name"),tabl_constit[constit][3]);
    modif_xml(g_markup_dom_nom (ooo,"constitution6","name"),tabl_constit[constit][4]);
}

void charisme_xml(GMarkupDomNode * ooo,gchar * ch2,gchar * ch3,gchar * ch4)
{
    modif_xml(g_markup_dom_nom (ooo,"charisme2","name"),ch2);
    modif_xml(g_markup_dom_nom (ooo,"charisme3","name"),ch3);
    modif_xml(g_markup_dom_nom (ooo,"charisme4","name"),ch4);
}

void vade_retro(signed short niv, signed short vers_, GMarkupDomNode * ooo )
{
    gchar ch[8];
    unsigned short i,j=40;
    signed short retour=0;

    if (niv<=0)
    {
    }
    else
    {
        GMarkupDomNode * node=g_markup_dom_nom (ooo,"vd1","name");

        niv=_min(13,niv);

        if (node->nb_texte>0)
        {
            if (sscanf(node->texte[0].texte,"%hu",&i)==1)
            { /* en cas de multiples classes avec vade_retro, on ne garde que la meilleure */
                if (vers_==ADD1)
                {
                    if (sscanf(vd_add1[0][niv-1],"%hu",&j)==0)
                    {
                        j=0;
                    }
                    else
                    { /* la lecture c'est bien faite */
                    }
                } /* 0 car c'est une lettre donc mieux que le chiffre de i */
                else
                {
                    if (sscanf(vd_add2[0][niv-1],"%hu",&j)==0)
                    {
                        j=0;
                    }
                    else
                    {/* la lecture c'est bien faite */
                    }
                }
                if (j>=i)
                {
                    retour=1;
                }
                else
                {
                }
            }
            else
            {
                node=g_markup_dom_nom (ooo,"vd10","name");
                if (node->nb_texte>0) /* comme i était une lettre on regarde les chiffres de la fin */
                {
                    if (sscanf(node->texte[0].texte,"%hu",&i)==1)
                    { /* en cas de multiples classes avec vade_retro, on ne garde que la meilleure */
                        if (vers_==ADD1)
                        {
                            if (sscanf(vd_add1[9][niv-1],"%hu",&j)==0)
                            {
                                j=-1;
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                            if (sscanf(vd_add2[9][niv-1],"%hu",&j)==0)
                            {
                                j=-1;
                            }
                            else
                            {
                            }
                        }
                        if (j>=i)
                        {
                            retour=1;
                        }
                        else
                        {
                        }
                    }
                }
            }
        }

        if (retour!=1)
        {
            /* Vade retro */
            if (vers_==ADD1)
            {
                for (i=0;i<13;i++)
                {
                    sprintf(ch,"vd%hu",i+1);
                    modif_xml(g_markup_dom_nom (ooo,ch,"name"),vd_add1[i][niv-1]);
                }
            }
            else
            {
                for (i=0;i<13;i++)
                {
                    sprintf(ch,"vd%hu",i+1);
                    modif_xml(g_markup_dom_nom (ooo,ch,"name"),vd_add2[i][niv-1]);
                }
            }
        }
        else
        {
        }
    }
}

void ecrit_ch(FILE * sortie, char * ch) /* prise en compte des caractères spéciaux du xml */
{
    unsigned long i;

    if (ch!=NULL)
    {
        for (i=0;ch[i];i++)
        {
            if (ch[i]=='&')
            {
              g_fprintf(sortie,"&amp;");
            }
            else if (ch[i]=='<')
            {
              g_fprintf(sortie,"&lt;");
            }
            else if (ch[i]=='>')
            {
              g_fprintf(sortie,"&gt;");
            }
            else
            {
                fputc(ch[i],sortie);
            }
        }
    }
    else
    {
        printf("Erreur : chaine vide (fonction ecrit_ch)\n");
    }
}

signed long but_xp_classe(struct_classe * s_c,struct_race * s_r,unsigned short niv,unsigned short version_t)
{
    unsigned short i;
    signed long * s_l;
    float m_race;
    signed long retour;

    if (s_r->add[ADD2].nom==NULL || version_t==ADD1)
    {
        m_race=s_r->add[ADD].xp;
    }
    else
    {
        m_race=s_r->add[ADD2].xp;
    }

    if (s_c->add[ADD2].nom==NULL || version_t==ADD1)
    {
        s_l=s_c->add[ADD].XP;
        if (niv<20 && s_c->add[ADD].pdv[niv-1]==NULL)
        {
            retour=0;
        }  /* niveau supérieur non ateignable */
        else
        {
            for (i=0;i<niv-1 && s_l[i]!=FIN;i++);
            if (s_l[i]!=FIN)
            {
                retour=s_l[i]*m_race; /* CAS DANS LA TABLE */
            }
            else
            {
                /* maintenant, on est dans la progression arithmétique */
                retour= (niv-i+1)*s_l[i-1]*m_race;
            }
        }
    }
    else
    {
        s_l=s_c->add[ADD2].XP;
        if (niv<20 && s_c->add[ADD2].pdv[niv-1]==NULL)
        {
            retour=0;
        } /* niveau suppérieur non ateignable */
        else
        {
            for (i=0;i<niv-1 && s_l[i]!=FIN;i++);
            if (s_l[i]!=FIN)
            {
                retour=s_l[i]*m_race; /* CAS DANS LA TABLE */
            }
            else
            {
                /* maintenant, on est dans la progression arithmétique */
                retour= (niv-i+1)*s_l[i-1]*m_race;
            }
        }
    }

    return retour;
}

void trouve_modif(char * ch,unsigned short * car ,float * coef)
{
    signed short i,j=0,k;

    for (i=0;ch[i]!='/' && ch[i]!=0;i++);

    *car=10; /* cas pas de stat avant */
    j=ch[i];
    ch[i]=0;
    for (k=0;k<8;k++)
    {
        if (compare_sans_casse((char *)nom[k],ch)==0)
        {
            *car=k;
        }
        else
        {
        }
    }
    ch[i]=j;
    if (*car==10)
    {
         i=0;
    }
    else
    {
        i++;
    }

    if (ch[i]!=0)
    {
        j=sscanf(ch+i,"%f",coef);
    }
    if (j==0)
    {
        if (*car!=10)
        {
         *coef=1;
        }
        else
        {
         *coef=0;
        }
    }
}

void enregistre_perso_sous(GtkWidget *ChildWidget, FenetrePerso * _perso)
{   /* si ChildWidget vaut NULL ça veut dire qu'on arrive de enregistre perso avec un nom vierge */
  GtkWidget *p_dialog = NULL;
  char * nom_de_fichier=NULL, *tmp=NULL, ch[LONG];
  GtkFileFilter * filtre= gtk_file_filter_new();
  FILE * fp;

  if (ChildWidget==NULL)
  {
      tmp="Sauvegarder le nouveau fichier";
  }
  else
  {
      tmp="Sauvegarder dans le fichier";
  }

  p_dialog = gtk_file_chooser_dialog_new (tmp, NULL,
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          "Annuler", GTK_RESPONSE_CANCEL,
                                          "Enregistrer sous",GTK_RESPONSE_ACCEPT,
                                          NULL);
  if (_perso->perso.nom_fichier!=NULL && _perso->perso.nom_fichier[0]!=0)
  {
       for (tmp=_perso->perso.nom_fichier+strlen(_perso->perso.nom_fichier)-1;*tmp!=SEPARATEUR && *tmp!=0;tmp--);
       tmp++;
       gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(p_dialog),tmp);
  }
  else
  {
  }
  tmp=envp_donne_home();
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER(p_dialog),tmp);
  g_free(tmp);

  gtk_file_filter_add_pattern (filtre,"*.pers");
  gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(p_dialog),filtre);

  if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
  {
    nom_de_fichier = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
    gtk_widget_destroy(p_dialog);
    fp = g_fopen(nom_de_fichier, "rb");
    if (fp!=NULL) /* Le fichier existe déjà */
    {
        fclose(fp);
        sprintf(ch,"Écraser le fichier\n%s",nom_de_fichier);
        p_dialog = gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_YES_NO,"%s",ch);
        switch(gtk_dialog_run(GTK_DIALOG(p_dialog)))
        {
            case GTK_RESPONSE_NO:
                gtk_widget_destroy(p_dialog);
                break; /* on sort du programme */
            default :
                gtk_widget_destroy(p_dialog);
                if (_perso->perso.nom_fichier!=NULL)
                {
                    g_free(_perso->perso.nom_fichier);
                }
                else
                { /* pas besoin de libérer la mémoire */
                }
                _perso->perso.nom_fichier=verif_ext(nom_de_fichier,".pers");
                envp_mem_home_rep(_perso->perso.nom_fichier);
                enregistre_perso(NULL,_perso);
                break;
        }
    }
    else
    {
        if (_perso->perso.nom_fichier!=NULL)
        {
            g_free(_perso->perso.nom_fichier);
        }
        else
        { /* pas besoin de libérer la mémoire */
        }
        _perso->perso.nom_fichier=verif_ext(nom_de_fichier,".pers");
        envp_mem_home_rep(_perso->perso.nom_fichier);
        enregistre_perso(NULL,_perso);
    }
  }
  else
  {
     gtk_widget_destroy (p_dialog);
  }
  g_free(nom_de_fichier);
}

void enregistre_perso(GtkWidget *ChildWidget, FenetrePerso * _perso)
{
    signed short i,j,k,l,m,n,pt_psi=0,niv_psi=0;
    signed short pan=0,die=0; /* mémorisation du dieu et du panthéon */
    signed short * classe=_perso->perso.classe;
    signed short * niv_cl=_perso->perso.niveau;
    signed short * caract=_perso->perso.caract;
    signed short _version=_perso->perso.version,vers_race;
    signed short sexe=_perso->perso.sexe;
    signed short niv,mag=0,clerc=0,bonus_init=0;
    struct_race * race=RACE+_perso->perso.race;
    signed short * arme=_perso->perso.arme;
    signed short * competence=_perso->perso.competence;
    struct_classe * _classe=CLASSE;
    char ch[LONG],aff[LONG],affichage[LONG], * chemin=NULL, * tmp1=NULL, * nom_de_fichier=NULL;
    FILE * fichier;
    GMarkupDomNode * ooo,* tmp_node, *tmp1_node,* node;
    signed short bonus=0,taco=20,taco_lan,spmp=20,spp=20,sbb=20,ssou=20,ssor=20,nm=-5,bt=1,taco_tmp=20,bt_tmp=1;
    signed short ca1=0,ca2=0,bonus_ca=0;
    char but_xp[LONG]={0};
    char actu_xp[LONG]={0};
    char bonus_xp[LONG]={0};
    char ch_tmp[16];
    char ** save_race=NULL;
    unsigned short car;
    float coef;
    signed short vdr=0;
    signed short vd_delta=0;
    signed short arm_nat;
    unsigned short page_t=0,page=0;
    unsigned short pages_voy=0;
    unsigned short ca_bonus_race=0;
    struct zip * f_zip=NULL;
    struct zip_source * n_zip=NULL;
    GtkWidget *p_dialog = NULL;
    signed short * taco_a=NULL, *degat_a=NULL;


    if (_perso->perso.nom_fichier!=NULL)
    {
         if (gestion_fenetre ((struct widgets *)1,_perso->perso.nom_fichier, NULL)==NULL)
         {
            envp_mem_hist(_perso->perso.nom_fichier);
         }
         else
         {
             /* le perso fait parti d'un groupe ouvert */
         }
         /* début de la construction du corps du fichier odt */
         chemin=chemin_exe();
         sprintf(ch,"%s%cLibO%csortie.xml",chemin,SEPARATEUR,SEPARATEUR);
         g_free(chemin);
         ooo=g_markup_dom_new(ch,NULL);

          if (g_markup_dom_nom (ooo,"actu_xp","name")==NULL)
          {
             p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->resume->window),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,"Erreur système :\n fichier sortie.xml corrompu");
             gtk_dialog_run(GTK_DIALOG(p_dialog));
             gtk_widget_destroy(p_dialog);
          }  /* dernier nom => trouver une erreur dans le fichier xml */
          else
          {
            sprintf(ch,"%s",_perso->perso.nom_fichier);
            i=strlen(ch);
            ch[i-4]='o';
            ch[i-3]='d';
            ch[i-2]='t';
            ch[i-1]=0;
            p_dialog = gtk_message_dialog_new (GTK_WINDOW(_perso->resume->window),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,"Fichier xml : %s\nFichier LibO : %s",_perso->perso.nom_fichier,ch);
            gtk_dialog_run(GTK_DIALOG(p_dialog));
            gtk_widget_destroy(p_dialog);

            if (!str_isspace (_perso->perso.nom))
            {
                modif_xml(g_markup_dom_nom (ooo,"nom","name"),_perso->perso.nom);
            }
            else
            {
                modif_xml(g_markup_dom_nom (ooo,"nom","name"),_perso->perso.nom_fichier);
            }
            if (!str_isspace (_perso->perso.joueur))
            {
                modif_xml(g_markup_dom_nom (ooo,"joueur","name"),_perso->perso.joueur);
            }
            else
            {
                modif_xml(g_markup_dom_nom (ooo,"joueur","name")," - ");
            }
            if (caract[FORCE]!=18 || caract[POURCENTAGE]==00)
            {
                modif_xml(g_markup_dom_nom (ooo,"force1","name"),nb_texte(ch,caract[FORCE]));
            }
            else
            {
                node=g_markup_dom_nom (ooo,"force1","name");
                modif_xml(node,"18 ");

                xml_ajoute_fin(node,"text:span"); /* ajout du pourcentage         */
                node=node->fils+node->nb_fils-1;  /* accès au dernier fils        */
                xml_ajoute_fin_attribut(node);    /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(node,"text:style-name","T12");
                sprintf(ch,"%02hd %%",caract[POURCENTAGE]);
                xml_ajoute_fin_texte(node);
                xml_ecrit_dernier_texte(node,ch);
            }

            sprintf(affichage,"Sexe : %s",nom_sexe[sexe]);
            modif_xml(g_markup_dom_nom (ooo,"sexe","name"),affichage);
            if (_version==ADD || race->add[ADD2].nom==NULL)
            { /* choix de la version pour afficher la race */
                vers_race=ADD;
            }
            else
            {
                vers_race=ADD2;
            }
            sprintf(affichage,"Race : %s",race->add[vers_race].nom);
            modif_xml(g_markup_dom_nom (ooo,"race","name"),affichage);

            ch[0]=0;
            if (caract[SAGESSE]>18 && _version!=ADD1)
            {
                strcat(ch,tabl_sagesse[18][9]);
                for(i=19;i<caract[SAGESSE];i++)
                {
                    strcat(ch,", ");
                    strcat(ch,tabl_sagesse[i][9]);
                }
            }

            if (race->add[vers_race].resistance!=NULL)
            {
                 strcat(ch," ,");
                 strcat(ch,race->add[vers_race].resistance);
                 modif_xml(g_markup_dom_nom (ooo,"resistance","name"),ch);
            }
            else
            { /* pas de résistance raciale */
                if (ch[0]==0)
                {
                    modif_xml(g_markup_dom_nom (ooo,"resistance","name")," - ");
                }
                else
                {
                    modif_xml(g_markup_dom_nom (ooo,"resistance","name"),ch);
                }
            }
            if (race->add[vers_race].vision!=NULL)
            {
                 modif_xml(g_markup_dom_nom (ooo,"vision","name"),race->add[vers_race].vision);
            }
            else
            { /* pas de vision */
                 modif_xml(g_markup_dom_nom (ooo,"vision","name")," - ");
            }
            aff[0]=0;
            for(i=0;race->add[vers_race].detection[i]!=NULL;i++)
            {
                if (i!=0)
                {
                    strcpy(aff,race->add[vers_race].detection[0]);
                }
                else
                {
                    strcat(aff,race->add[vers_race].detection[i]);
                }
            }
            if (aff[0]==0)
            {
                strcpy(aff," - ");
            }
            else
            { /* La chaîne est déjà écrite */
            }
            modif_xml(g_markup_dom_nom (ooo,"detection","name"),aff);
            if (race->add[vers_race].ecoute!=NULL)
            {
                modif_xml(g_markup_dom_nom (ooo,"ecoute","name"),race->add[vers_race].ecoute);
            }
            else
            { /* pas d'écoute */
                modif_xml(g_markup_dom_nom (ooo,"ecoute","name")," - ");
            }
            sprintf(ch,"%hd cm",_perso->perso.taille);
            modif_xml(g_markup_dom_nom (ooo,"taille","name"),ch);
            sprintf(ch,"%hd kg",_perso->perso.poids);
            modif_xml(g_markup_dom_nom (ooo,"poids","name"),ch);
            if (!str_isspace (_perso->perso.cheveux))
            {
                modif_xml(g_markup_dom_nom (ooo,"cheveux","name"),_perso->perso.cheveux);
            }
            else
            {
                modif_xml(g_markup_dom_nom (ooo,"cheveux","name")," - ");
            }
            if (!str_isspace (_perso->perso.peau))
            {
                modif_xml(g_markup_dom_nom (ooo,"peau","name"),_perso->perso.peau);
            }
            else
            {
                modif_xml(g_markup_dom_nom (ooo,"peau","name")," - ");
            }
            if (!str_isspace (_perso->perso.yeux))
            {
                modif_xml(g_markup_dom_nom (ooo,"yeux","name"),_perso->perso.yeux);
            }
            else
            {
                modif_xml(g_markup_dom_nom (ooo,"yeux","name")," - ");
            }
            if (!str_isspace (_perso->perso.sociale))
            {
                modif_xml(g_markup_dom_nom (ooo,"classe sociale","name"),_perso->perso.sociale);
            }
            else
            {
                modif_xml(g_markup_dom_nom (ooo,"classe sociale","name")," - ");
            }
            modif_xml(g_markup_dom_nom (ooo,"age","name"),nb_texte(ch,_perso->perso.age));
            modif_xml(g_markup_dom_nom (ooo,"alignement","name"),algnm[_perso->perso.alignement]);
            if (_perso->perso.dieu!=NULL && _perso->perso.dieu[0]!=0)
            {
                sprintf(ch,"Dieu : %s",_perso->perso.dieu);
                modif_xml(g_markup_dom_nom (ooo,"dieu","name"),ch);
                pan=pantheon_entier(_perso->perso.pantheon);
                die=dieu_entier(_perso->perso.dieu,pan);
                if (die>=0)
                {
                    sprintf(ch,"Symbole de %s : %s",_perso->perso.dieu,PANTHEONS[pan].dieu[die].symbole);
                    modif_xml(g_markup_dom_nom (ooo,"symbole_dieu","name"),ch);
                    modif_xml(g_markup_dom_nom (ooo,"commentaire_dieu","name"),PANTHEONS[pan].dieu[die].commentaire);
                }
                else
                { /* Pas de dieu */
                }
            }
            else
            {
                modif_xml(g_markup_dom_nom (ooo,"dieu","name"),"Dieu : - ");
                modif_xml(g_markup_dom_nom (ooo,"symbole_dieu","name"),"Symbole : -");
                modif_xml(g_markup_dom_nom (ooo,"commentaire_dieu","name")," - ");
            }
            if (_perso->perso.pantheon!=NULL && _perso->perso.pantheon[0]!=0)
            {
                sprintf(ch,"Panthéon : %s",_perso->perso.pantheon);
                modif_xml(g_markup_dom_nom (ooo,"pantheon","name"),ch);
            }
            else
            {
                modif_xml(g_markup_dom_nom (ooo,"pantheon","name"),"Panthéon : - ");
            }
         /* magie */
         init_liste_mag();
         if (_perso->perso.livre_sorts!=NULL)
         {
            page=0;
            l=0;
            for (i=0;i<9;i++)
            {
                if (_perso->perso.livre_sorts[i]!=NULL)
                {
                        page+=affiche_sort_mag(i,_perso,ooo,&pages_voy,&page_t);

                        node=g_markup_dom_node(ooo,"office:text");
                        /* affichage des codes : sorts compris, non compris, dans le livre */
                        xml_ajoute_fin(node,"text:p");
                        xml_ajoute_fin(node,"table:table");
                        node=node->fils+node->nb_fils-1; /* on arrive sur la dernière node */
                        for (j=0;j<6;j++) xml_ajoute_fin(node,"table:table-column");
                        xml_ajoute_fin_attribut(node->fils); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(node->fils,"table:style-name","Tableau1.VDA_L"); /* remplace le dernier attribut */
                        for (j=1;j<6;j++)
                        {
                            xml_ajoute_fin_attribut(node->fils+j); /* rajouter un élément attribut */
                            xml_ecrit_dernier_attribut(node->fils+j,"table:style-name","Tableau1.VDA"); /* remplace le dernier attribut */
                        }
                        xml_ajoute_fin(node,"table:table-row"); /* node pointera sur le tableau du niveau */
                        tmp_node=node->fils+node->nb_fils-1; /* on arrive sur la dernière node */
                        xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                        xml_ajoute_fin_attribut(tmp_node->fils); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils,"table:style-name","Tableau1.VD1"); /* remplace le dernier attribut */
                        xml_ajoute_fin_attribut(tmp_node->fils); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils,"office:value-type","string"); /* remplace le dernier attribut */
                        xml_ajoute_fin(tmp_node->fils,"text:p");
                        xml_ajoute_fin_texte(tmp_node->fils->fils);
                        xml_ecrit_dernier_texte(tmp_node->fils->fils,"Légende : couleur des sorts");
                        /* compris */
                        xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                        xml_ajoute_fin(tmp_node->fils+1,"text:p");
                        xml_ajoute_fin_texte(tmp_node->fils[1].fils);
                        xml_ecrit_dernier_texte(tmp_node->fils[1].fils,"Compris");
                        xml_ajoute_fin_attribut(tmp_node->fils+1); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils+1,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
                        xml_ajoute_fin_attribut(tmp_node->fils+1); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils+1,"office:value-type","string"); /* remplace le dernier attribut */
                        xml_ajoute_fin_attribut(tmp_node->fils[1].fils); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils[1].fils,"text:style-name","sort"); /* remplace le dernier attribut */
                        /* pas_compris */
                        xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                        xml_ajoute_fin(tmp_node->fils+2,"text:p");
                        xml_ajoute_fin_texte(tmp_node->fils[2].fils);
                        xml_ecrit_dernier_texte(tmp_node->fils[2].fils,"Pas compris");
                        xml_ajoute_fin_attribut(tmp_node->fils+2); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils+2,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
                        xml_ajoute_fin_attribut(tmp_node->fils+2); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils+2,"office:value-type","string"); /* remplace le dernier attribut */
                        xml_ajoute_fin_attribut(tmp_node->fils[2].fils); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils[2].fils,"text:style-name","pas_compris"); /* remplace le dernier attribut */
                        /* pas_lu */
                        xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                        xml_ajoute_fin(tmp_node->fils+3,"text:p");
                        xml_ajoute_fin_texte(tmp_node->fils[3].fils);
                        xml_ecrit_dernier_texte(tmp_node->fils[3].fils,"Pas regardé");
                        xml_ajoute_fin_attribut(tmp_node->fils+3); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils+3,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
                        xml_ajoute_fin_attribut(tmp_node->fils+3); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils+3,"office:value-type","string"); /* remplace le dernier attribut */
                        xml_ajoute_fin_attribut(tmp_node->fils[3].fils); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils[3].fils,"text:style-name","pas_lu"); /* remplace le dernier attribut */
                        /* mauvaise école */
                        xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                        xml_ajoute_fin(tmp_node->fils+4,"text:p");
                        xml_ajoute_fin_texte(tmp_node->fils[4].fils);
                        xml_ecrit_dernier_texte(tmp_node->fils[4].fils,"Mauvaise École");
                        xml_ajoute_fin_attribut(tmp_node->fils+4); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils+4,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
                        xml_ajoute_fin_attribut(tmp_node->fils+4); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils+4,"office:value-type","string"); /* remplace le dernier attribut */
                        xml_ajoute_fin_attribut(tmp_node->fils[4].fils); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils[4].fils,"text:style-name","pas_ecole"); /* remplace le dernier attribut */
                        /* École spécialiste */
                        xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                        xml_ajoute_fin(tmp_node->fils+5,"text:p");
                        xml_ajoute_fin_texte(tmp_node->fils[5].fils);
                        xml_ecrit_dernier_texte(tmp_node->fils[5].fils,"École Spécialiste");
                        xml_ajoute_fin_attribut(tmp_node->fils+5); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils+5,"table:style-name","Tableau1.VD3"); /* remplace le dernier attribut */
                        xml_ajoute_fin_attribut(tmp_node->fils+5); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils+5,"office:value-type","string"); /* remplace le dernier attribut */
                        xml_ajoute_fin_attribut(tmp_node->fils[5].fils); /* rajouter un élément attribut */
                        xml_ecrit_dernier_attribut(tmp_node->fils[5].fils,"text:style-name","ecole_OK"); /* remplace le dernier attribut */
                }
            }
            if (page_t>0)
            {
                if (page_t==1)
                {
                    if (page==1)
                    {
                         sprintf(ch,"Le livre de sort contient une page qui contient un sort compris.");
                    }
                    else
                    {
                         sprintf(ch,"Le livre de sort contient une page mais le personnage ne comprend aucun sort.");
                    }
                }
                else
                {
                    if (page==0)
                    {
                         sprintf(ch,"Le livre de sort contient %hu page%s mais le personnage ne comprend aucun sort.",page_t,pages_voy==1?"":"s");
                    }
                    else if (page==1)
                    {
                        sprintf(ch,"Le livre de sort contient %hu pages mais le personnage ne comprend qu'une page de sort.",page_t);
                    }
                    else if (page==page_t)
                    {
                         sprintf(ch,"Le livre de sort contient %hu pages et le personnage comprend tout.",page_t);
                    }
                    else
                    {
                         sprintf(ch,"Le livre de sort contient %hu pages mais le personnage ne comprend que %hu pages de sort.",page_t,page);
                    }
                }
                if (pages_voy && page!=pages_voy)
                {
                  sprintf(ch+strlen(ch)," Son livre de voyage contient %hu page%s.",pages_voy,pages_voy==1?"":"s");
                }
                else
                {
                }
                xml_ecrit_dernier_texte(g_markup_dom_nom (ooo,"pages_mag","name"),ch);
            }
         }

            affichage[0]=0;
            aff[0]=0;
            if (classe!=NULL)
            {
                for (i=0;classe[i]>=0;i++)
                { /* parcours des classes */
                    init_liste_sort();
                    if (affichage[0]!=0)
                    {
                        sprintf(ch,", %s",_classe[classe[i]].nom);
                        strcat(affichage,ch);
                        sprintf(ch,", %hu",niv_cl[i]);
                        strcat(aff,ch);
                    }
                    else
                    {
                        strcpy(affichage,"Classe : ");
                        sprintf(aff,"Niveau : ");
                        strcat(affichage,_classe[classe[i]].nom);
                        sprintf(ch,"%hu",niv_cl[i]);
                        strcat(aff,ch);
                    }
                    if (i!=0)       /* clerc et mag commence à 0 et à chaque classe, ils s'incrémentent si la classe utilisent des sorts de clerc ou de mag */
                    {
                        but_xp[strlen(but_xp)+1]=0;
                        but_xp[strlen(but_xp)]='-';
                        actu_xp[strlen(actu_xp)+1]=0;
                        actu_xp[strlen(actu_xp)]='-';
                    }
                    else
                    {
                    }
                    sprintf(ch_tmp, "%ld",but_xp_classe(_classe+classe[i],race,niv_cl[i],_version));
                    strncat(but_xp,ch_tmp,LONG-1);
                    sprintf(ch_tmp,"%lu",_perso->perso.XP[i]);
                    strncat(actu_xp,ch_tmp,LONG-1);

                     /* calcul des save et taco */
                    if (_classe[classe[i]].add[ADD2].nom==NULL || _version==ADD1)
                    {
                        bonus_init=max(bonus_init,(niv_cl[i]-1)*_classe[classe[i]].add[ADD].init[0]/_classe[classe[i]].add[ADD].init[1]);
                        for(niv=0;niv<niv_cl[i] && _classe[classe[i]].add[ADD].save->save[niv]!=NULL;niv++);
                        if (niv>0)
                        {/* pour ceux qui ont des niveaux trop élevés */
                            niv--;
                        }
                        else
                        { /* on ne va pas passer à -1 ! */
                        }
                        spmp=_min(spmp,_classe[classe[i]].add[ADD].save->save[niv][0]);
                        sbb=_min(sbb,_classe[classe[i]].add[ADD].save->save[niv][2]); /* */
                        ssou=_min(ssou,_classe[classe[i]].add[ADD].save->save[niv][3]);
                        ssor=_min(ssor,_classe[classe[i]].add[ADD].save->save[niv][4]);
                        spp=_min(spp,_classe[classe[i]].add[ADD].save->save[niv][1]); /* */
                        nm=max(nm,_classe[classe[i]].add[ADD].nb_arme[1]);
                        for(niv=0;_classe[classe[i]].add[ADD].tacO._1[0][niv]<=niv_cl[i] && _classe[classe[i]].add[ADD].tacO._1[0][niv]!=FIN;niv++);
                        if (niv>0)
                        {/* pour ceux qui ont des niveaux trop élevés */
                            niv--;
                        }
                        else
                        { /* on ne va pas passer à -1 ! */
                        }
                        taco=_min(taco,_classe[classe[i]].add[ADD].tacO._1[1][niv]);
                        j=0; /* j=0 : rien repéré, j=-1 : impossible, j=1 possible */
                        for (niv=0;niv<6;niv++)
                        {
                            if (j!=-1 && _classe[classe[i]].add[ADD].dix_caract[niv]!=0)
                            {
                                if (_classe[classe[i]].add[ADD].dix_caract[niv]>caract[niv])
                                {
                                    j=-1;
                                }
                                else
                                {
                                    j=1;
                                }
                            }
                        }
                        if (j==1)
                        {
                            strncat(bonus_xp," 10 % -",LONG-1);
                        }
                        else
                        {
                            strncat(bonus_xp," 0 % -",LONG-1);
                        }
                        for(niv=0;niv<niv_cl[i] && _classe[classe[i]].add[ADD].backstab[niv]!=FIN;niv++);
                        if (niv>0)
                        {/* pour ceux qui ont des niveaux trop élevés */
                            niv--;
                        }
                        else
                        { /* on ne va pas passer à -1 ! */
                        }
                        bt_tmp=_classe[classe[i]].add[ADD].backstab[niv];
                        /* recherche d'un éventuel bonus d'XP */

                    }
                    else
                    {
                        bonus_init=max(bonus_init,(niv_cl[i]-1)*_classe[classe[i]].add[ADD2].init[0]/_classe[classe[i]].add[ADD2].init[1]);
                        for(niv=0;niv<niv_cl[i] && _classe[classe[i]].add[ADD2].save->save[niv]!=NULL;niv++);
                        if (niv>0)
                        {
                            niv--;
                        }
                        else
                        { /* on ne va pas passer à -1 ! */
                        }
                        spmp=_min(spmp,_classe[classe[i]].add[ADD2].save->save[niv][0]);
                        sbb=_min(sbb,_classe[classe[i]].add[ADD2].save->save[niv][2]); /* */
                        ssou=_min(ssou,_classe[classe[i]].add[ADD2].save->save[niv][3]);
                        ssor=_min(ssor,_classe[classe[i]].add[ADD2].save->save[niv][4]);
                        spp=_min(spp,_classe[classe[i]].add[ADD2].save->save[niv][1]); /* */
                        nm=max(nm,_classe[classe[i]].add[ADD2].nb_arme[1]);
                        taco=_min(taco,(_classe[classe[i]].add[ADD2].tacO._2[0]-(niv_cl[i]-1)*_classe[classe[i]].add[ADD2].tacO._2[1]/_classe[classe[i]].add[ADD2].tacO._2[2]));
                        j=0; /* j=0 : rien repéré, j=-1 : impossible, j=1 possible */
                        for (niv=0;niv<6;niv++)
                        {
                            if (j!=-1 && _classe[classe[i]].add[ADD2].dix_caract[niv]!=0)
                            {
                                if (_classe[classe[i]].add[ADD2].dix_caract[niv]>caract[niv])
                                {
                                    j=-1;
                                }
                                else
                                {
                                    j=1;
                                }
                            }
                        }
                        if (j==1)
                        {
                            strncat(bonus_xp," 10 % -",LONG-1);
                        }
                        else
                        {
                            strncat(bonus_xp," 0 % -",LONG-1);
                        }
                        for(niv=0;niv<niv_cl[i] && _classe[classe[i]].add[ADD2].backstab[niv]>0;niv++);
                        if (niv>0)
                        {
                            niv--;
                        }
                        else
                        { /* on ne va pas passer à -1 ! */
                        }
                        bt_tmp=_classe[classe[i]].add[ADD2].backstab[niv];
                        /* affichage des sorts de prêtre si besoin */
                        if (_classe[classe[i]].add[ADD2].sort_clerc!=NULL)
                        {
                            if (_classe[classe[i]].add[ADD2].sort_clerc_sphere[0][0]=='*' || (_perso->perso.dieu!=NULL && _perso->perso.dieu[0]!=0 && compare_sans_casse(_classe[classe[i]].nom,"druide")==0)) /* cas du prêtre ou du druide affilié à un dieu*/
                            {
                               affiche_sort_pretre(_perso->perso.pantheon,_perso->perso.dieu,ooo,classe[i],_perso->perso.origine);
                            }
                            else
                            {
                              affiche_sort_pretre(_classe[classe[i]].add[ADD2].sort_clerc_sphere[0],_classe[classe[i]].add[ADD2].sort_clerc_sphere[1],ooo,classe[i],_perso->perso.origine);
                            }
                            /* rajoute les compétences données par le dieu */
                            if (_perso->perso.dieu!=NULL && _perso->perso.dieu[0]!=0 && (compare_sans_casse(_classe[classe[i]].nom,"prêtre")==0 || compare_sans_casse(_classe[classe[i]].nom,"druide")==0))
                            {
                                if (die>=0)
                                {
                                    for (j=0;j<PANTHEONS[pan].dieu[die].pc_nb && PANTHEONS[pan].dieu[die].pc_niv[j]<=niv_cl[i];j++)
                                    {
                                        sprintf(ch,"niveau %hu : %s",PANTHEONS[pan].dieu[die].pc_niv[j],PANTHEONS[pan].dieu[die].pc[j]);
                                        xml_ecrit_dernier_texte_f(g_markup_dom_nom (ooo,"commentaire_dieu","name"),ch);
                                    }
                                }
                                else
                                { /* Pas de dieu */
                                }
                            }
                            else
                            {
                            }


                        }
                        if (_classe[classe[i]].add[ADD2].psi._2!=NULL)
                        {
                            niv=niv_cl[i];
                            for (j=0;_classe[classe[i]].add[ADD2].psi._2->debut.base[j]!=FIN;j++) /* départ */
                            {
                                pt_psi+=_classe[classe[i]].add[ADD2].psi._2->base[caract[_classe[classe[i]].add[ADD2].psi._2->debut.base[j]]];
                            }
                            for (j=0;_classe[classe[i]].add[ADD2].psi._2->debut.bonus[j]!=FIN;j++)
                            {
                                pt_psi+=_classe[classe[i]].add[ADD2].psi._2->bonus[caract[_classe[classe[i]].add[ADD2].psi._2->debut.bonus[j]]];
                            }
                            for (j=0;_classe[classe[i]].add[ADD2].psi._2->niveau.base[j]!=FIN;j++) /* départ */
                            {
                                if (_classe[classe[i]].add[ADD2].psi._2->niveau.base[j]<0)
                                {
                                  pt_psi-=_classe[classe[i]].add[ADD2].psi._2->niveau.base[j]*(niv-1);
                                }
                                else
                                {
                                  pt_psi+=_classe[classe[i]].add[ADD2].psi._2->base[caract[_classe[classe[i]].add[ADD2].psi._2->niveau.base[j]]]*(niv-1);
                                }
                            }
                            for (j=0;_classe[classe[i]].add[ADD2].psi._2->niveau.bonus[j]!=FIN;j++)
                            {
                                pt_psi+=_classe[classe[i]].add[ADD2].psi._2->bonus[caract[_classe[classe[i]].add[ADD2].psi._2->niveau.bonus[j]]]*(niv-1);
                            }
                        }
                    }
                    aff_nb_sort(g_markup_dom_nom (ooo,"section_sorts","name"),_classe+classe[i],&clerc,&mag,caract[SAGESSE],caract[INTELLIGENCE],niv_cl[i],_version);
                    bt=max(bt,bt_tmp); /* backstab */
                    /* CALCUL DE VADE-RETRO */
                    if (((_classe[classe[i]].add[ADD2].nom==NULL || _version==ADD1) && _classe[classe[i]].add[ADD].vade_retro!=NON) || ((_classe[classe[i]].add[ADD].nom==NULL || _version==ADD2) && _classe[classe[i]].add[ADD2].vade_retro!=NON))
                    {
                        if (_classe[classe[i]].add[ADD2].nom==NULL || _version==ADD1)
                        {
                            vade_retro(niv_cl[i]+_classe[classe[i]].add[ADD].vade_retro,ADD1,ooo);
                        }
                        else
                        {
                            if   (_classe[classe[i]].add[ADD2].vade_retro!=99)
                            {
                                vd_delta=niv_cl[i]+_classe[classe[i]].add[ADD2].vade_retro;
                            }
                            else
                            {
                                if (pan!=0)
                                {
                                    if (compare_sans_casse(PANTHEONS[pan].dieu[die].vd,"repousse")==0 || compare_sans_casse(PANTHEONS[pan].dieu[die].vd,"attire")==0 )
                                    {
                                        vd_delta=PANTHEONS[pan].dieu[die].vd_delta+niv_cl[i];
                                    }
                                    else
                                    {
                                        vd_delta=-1; /* Pas de vade retro */
                                    }
                                }
                                else
                                {
                                    printf("Surprise : erreur de panthéon pour un prêtre\n");
                                }
                            }
                            if (vd_delta>0)
                            {
                                vade_retro(vd_delta,ADD2,ooo);
                            }
                            else
                            {
                                vdr--;
                            }
                        }
                        vdr++;
                    }
                    else
                    { /* pas de vade retro */
                    }
                    /* calcul bonus CA race */
                    if (_version==ADD2 || race->add[ADD].nom==NULL)
                    {
                        if (race->add[ADD2].modif_CA[1]!=0 && race->add[ADD2].modif_CA[0]<niv_cl[i])
                        {
                            ca_bonus_race=max(ca_bonus_race,(niv_cl[i]-race->add[ADD2].modif_CA[0])*race->add[ADD2].modif_CA[1]/race->add[ADD2].modif_CA[2]);
                        }
                        else
                        {
                        }
                    }
                    else
                    {
                    }

                } /* fin de la boucle sur les classes */
            }
            else
            { /* pas de classe choisie */
            }
            modif_xml(g_markup_dom_nom (ooo,"classe","name"),affichage);
            modif_xml(g_markup_dom_nom (ooo,"niveau","name"),aff);
            modif_xml(g_markup_dom_nom (ooo,"actu_xp","name"),actu_xp);
            modif_xml(g_markup_dom_nom (ooo,"but_xp","name"),but_xp);
            modif_xml(g_markup_dom_nom (ooo,"bonus_xp","name"),bonus_xp);
            /* on repère si on a affiché des sorts */
            if (mag==0 && clerc==0)
            {
                xml_sup_node(g_markup_dom_nom(ooo,"section_sorts","name"));
            }
            else if (mag==0)
            {
                xml_sup_node(g_markup_dom_nom(ooo,"suppr_mag0","name"));
                xml_sup_node(g_markup_dom_nom(ooo,"suppr_mag1","name"));
            }
            else if (clerc==0)
            {
                xml_sup_node(g_markup_dom_nom(ooo,"suppr_clerc0","name"));
                xml_sup_node(g_markup_dom_nom(ooo,"suppr_clerc1","name"));
            }
            else
            { /* rien à enlever : le perso lance des sorts de clerc et mago */
            }

            taco_tmp=taco;
            if (_version==ADD1)
            {
               switch(caract[FORCE])
               {
                    case 1  : force_xml(ooo,"-5","-4","-450","1","0%");taco_tmp=taco+5;bonus=-4;break;
                    case 2  : force_xml(ooo,"-3","-2","-400","1","0%");taco_tmp=taco+4;bonus=-2;break;
                    case 3  : force_xml(ooo,"-3","-1","-350","2","0%");taco_tmp=taco+3;bonus=-1;break;
                    case 4  :
                    case 5  : force_xml(ooo,"-2","-1","-250","2","0%");taco_tmp=taco+2;bonus=-1;break;
                    case 6  :
                    case 7  : force_xml(ooo,"-1","","-150","2","0%");taco_tmp=taco+1;break;
                    case 8  :
                    case 9  : force_xml(ooo,"","","","2","1%");break;
                    case 10 :
                    case 11 : force_xml(ooo,"","","","2","2%");break;
                    case 12 :
                    case 13 : force_xml(ooo,"","","+100","2","4%");break;
                    case 14 :
                    case 15 : force_xml(ooo,"","","+200","2","7%");break;
                    case 16 : force_xml(ooo,"","+1","+350","3","10%");bonus=+1;break;
                    case 17 : force_xml(ooo,"+1","+1","+500","3","13%");taco_tmp=taco-1;bonus=+1;break;
                    case 18 : if (caract[POURCENTAGE]==0)
                                       {force_xml(ooo,"+1","+2","+750","3","16%");taco_tmp=taco-1;bonus=+2;}
                              else if (caract[POURCENTAGE]<51)
                                       {force_xml(ooo,"+1","+3","1000","3","20%");taco_tmp=taco-1;bonus=+3;}
                              else if (caract[POURCENTAGE]<76)
                                       {force_xml(ooo,"+2","+3","+1250","4","25%");taco_tmp=taco-2;bonus=+3;}
                              else if (caract[POURCENTAGE]<91)
                                       {force_xml(ooo,"+2","+4","+1500","4","30%");taco_tmp=taco-2;bonus=+4;}
                              else if (caract[POURCENTAGE]<100)
                                       {force_xml(ooo,"+2","+5","+2000","4","35%");taco_tmp=taco-2;bonus=+5;}
                              else
                                       {force_xml(ooo,"+3","+6","+3000","5","40%");taco_tmp=taco-3;bonus=+6;}
                              break;
                    case 19 : force_xml(ooo,"+3","+7","4500","7","50%");taco_tmp=taco-3;bonus=+7;break;
                    case 20 : force_xml(ooo,"+3","+8","5000","7","60%");taco_tmp=taco-3;bonus=+8;break;
                    case 21 : force_xml(ooo,"+4","+9","6000","9","70%");taco_tmp=taco-4;bonus=+9;break;
                    case 22 : force_xml(ooo,"+4","+10","7500","11","80%");taco_tmp=taco-4;bonus=+10;break;
                    case 23 : force_xml(ooo,"+5","+11","9000","11","90%");taco_tmp=taco-5;bonus=+11;break;
                    case 24 : force_xml(ooo,"+6","+12","12000","19","100%");taco_tmp=taco-6;bonus=+12;break;
                    case 25 : force_xml(ooo,"+7","+14","15000","19","100%");taco_tmp=taco-7;bonus=+14;break;
                    default : break;
                }
            }
            else
            {
                switch(caract[FORCE])
                {
                    case 1  : force_xml(ooo,"-5","-4","1/3","1","0%");taco_tmp=taco+5;bonus=-4;break;
                    case 2  : force_xml(ooo,"-3","-2","1/5","1","0%");taco_tmp=taco+4;bonus=-2;break;
                    case 3  : force_xml(ooo,"-3","-1","5/10","2","0%");taco_tmp=taco+3;bonus=-1;break;
                    case 4  :
                    case 5  : force_xml(ooo,"-2","-1","10/25","3","0%");taco_tmp=taco+2;bonus=-1;break;
                    case 6  :
                    case 7  : force_xml(ooo,"-1","","20/55","4","0%");taco_tmp=taco+1;break;
                    case 8  :
                    case 9  : force_xml(ooo,"","","35/90","5","1%");break;
                    case 10 :
                    case 11 : force_xml(ooo,"","","40/115","6","2%");break;
                    case 12 :
                    case 13 : force_xml(ooo,"","","45/140","7","4%");break;
                    case 14 :
                    case 15 : force_xml(ooo,"","","55/170","8","7%");break;
                    case 16 : force_xml(ooo,"","+1","70/195","9","10%");bonus=+1;break;
                    case 17 : force_xml(ooo,"+1","+1","85/220","10","13%");taco_tmp=taco-1;bonus=+1;break;
                    case 18 : if (caract[POURCENTAGE]==0)
                                       {force_xml(ooo,"+1","+2","110/235","11","16%");taco_tmp=taco-1;bonus=+2;}
                              else if (caract[POURCENTAGE]<51)
                                       {force_xml(ooo,"+1","+3","135/280","12","20%");taco_tmp=taco-1;bonus=+3;}
                              else if (caract[POURCENTAGE]<76)
                                       {force_xml(ooo,"+2","+3","160/305","13","25%");taco_tmp=taco-2;bonus=+3;}
                              else if (caract[POURCENTAGE]<91)
                                       {force_xml(ooo,"+2","+4","185/330","14","30%");taco_tmp=taco-2;bonus=+4;}
                              else if (caract[POURCENTAGE]<100)
                                       {force_xml(ooo,"+2","+5","235/380","15(3)","35%");taco_tmp=taco-2;bonus=+5;}
                              else /* 100 */
                                       {force_xml(ooo,"+3","+6","335/340","16(6)","40%");taco_tmp=taco-3;bonus=+6;}
                              break;
                    case 19 : force_xml(ooo,"+3","+7","485/640","16(8)","50%");taco_tmp=taco-3;bonus=+7;break;
                    case 20 : force_xml(ooo,"+3","+8","535/700","17(10)","60%");taco_tmp=taco-3;bonus=+8;break;
                    case 21 : force_xml(ooo,"+4","+9","635/810","17(12)","70%");taco_tmp=taco-4;bonus=+9;break;
                    case 22 : force_xml(ooo,"+4","+10","785/970","18(14)","80%");taco_tmp=taco-4;bonus=+10;break;
                    case 23 : force_xml(ooo,"+5","+11","935/1130","18(16)","90%");taco_tmp=taco-5;bonus=+11;break;
                    case 24 : force_xml(ooo,"+6","+12","1235/1440","19(17)","95%");taco_tmp=taco-6;bonus=+12;break;
                    case 25 : force_xml(ooo,"+7","+14","1535/1750","19(18)","99%");taco_tmp=taco-7;bonus=+14;break;
                    default : break;
                }
            }
            modif_xml(g_markup_dom_nom (ooo,"intelligence1","name"),nb_texte(ch,caract[INTELLIGENCE]));
            intelligence_xml(ooo,caract[INTELLIGENCE],_version);
            modif_xml(g_markup_dom_nom (ooo,"sagesse1","name"),nb_texte(ch,caract[SAGESSE]));
            sagesse_xml(ooo,caract[SAGESSE]);
            modif_xml(g_markup_dom_nom (ooo,"dexterite1","name"),nb_texte(ch,caract[DEXTERITE]));

            dexterite_xml(ooo,caract[DEXTERITE],_version);
            if (sscanf(tabl_dexterite[_version][caract[DEXTERITE]-1][1],"%hd",&taco_lan)!=1)
            {/* pas de bonus au taco projectile */
                 taco_lan=0;
            }
            else
            {  /* bonus projectile trouvé dans tabl_dexterite[_version][caract[DEXTERITE]-1][1] */
            }
            taco_lan=taco-taco_lan; /* calcul du taco au lancé : le bonus réduis le taco pour l'améliorer */
            if (sscanf(tabl_dexterite[_version][caract[DEXTERITE]-1][2],"%hd",&bonus_ca)!=1) /* ca nu : juste bonus de dext */
            {
                bonus_ca=0;
            }
            else
            {
            }
            modif_xml(g_markup_dom_nom (ooo,"constitution1","name"),nb_texte(ch,caract[CONSTITUTION]));
            constitution_xml(ooo,caract[CONSTITUTION]);
            modif_xml(g_markup_dom_nom (ooo,"charisme1","name"),nb_texte(ch,caract[CHARISME]));
            if (_version==ADD1)
            {
                switch(caract[CHARISME])
                {
                    case 1  : charisme_xml(ooo,"0","-40%","-35%");break;
                    case 2  : charisme_xml(ooo,"1","-35%","-30%");break;
                    case 3  : charisme_xml(ooo,"1","-30%","-25%");break;
                    case 4  : charisme_xml(ooo,"1","-25%","-20%");break;
                    case 5  : charisme_xml(ooo,"2","-20%","-15%");break;
                    case 6  : charisme_xml(ooo,"2","-15%","-10%");break;
                    case 7  : charisme_xml(ooo,"3","-10%","-5%");break;
                    case 8  : charisme_xml(ooo,"3","-5%","");break;
                    case 9  :
                    case 10 :
                    case 11 : charisme_xml(ooo,"4","","");break;
                    case 12 : charisme_xml(ooo,"5","","");break;
                    case 13 : charisme_xml(ooo,"5","","+5%");break;
                    case 14 : charisme_xml(ooo,"6","+5%","+10%");break;
                    case 15 : charisme_xml(ooo,"7","+15%","+15%");break;
                    case 16 : charisme_xml(ooo,"8","+20%","+25%");break;
                    case 17 : charisme_xml(ooo,"10","+30%","+30%");break;
                    case 18 : charisme_xml(ooo,"15","+40%","+35%");break;
                    case 19 : charisme_xml(ooo,"20","+50%","+40%");break;
                    case 20 : charisme_xml(ooo,"25","+60%","+45%");break;
                    case 21 : charisme_xml(ooo,"30","+70%","+50%");break;
                    case 22 : charisme_xml(ooo,"35","+80%","+55%");break;
                    case 23 : charisme_xml(ooo,"40","+90%","+60%");break;
                    case 24 : charisme_xml(ooo,"45","+100%","+70%");break;
                    case 25 : charisme_xml(ooo,"50","+100%","+75%");break;;
                    default : break;
                }
            }
            else
            {
                switch(caract[CHARISME])
                {
                    case 1  : charisme_xml(ooo,"0","-8","-7");break;
                    case 2  : charisme_xml(ooo,"1","-7","-6");break;
                    case 3  : charisme_xml(ooo,"1","-6","-5");break;
                    case 4  : charisme_xml(ooo,"1","-5","-4");break;
                    case 5  : charisme_xml(ooo,"2","-4","-3");break;
                    case 6  : charisme_xml(ooo,"2","-3","-2");break;
                    case 7  : charisme_xml(ooo,"3","-2","-1");break;
                    case 8  : charisme_xml(ooo,"3","-1","");break;
                    case 9  :
                    case 10 :
                    case 11 : charisme_xml(ooo,"4","","");break;
                    case 12 : charisme_xml(ooo,"5","","");break;
                    case 13 : charisme_xml(ooo,"5","","+1");break;
                    case 14 : charisme_xml(ooo,"6","+1","+2");break;
                    case 15 : charisme_xml(ooo,"7","+3","+3");break;
                    case 16 : charisme_xml(ooo,"8","+4","+5");break;
                    case 17 : charisme_xml(ooo,"10","+6","+6");break;
                    case 18 : charisme_xml(ooo,"15","+8","+7");break;
                    case 19 : charisme_xml(ooo,"20","+10","+8");break;
                    case 20 : charisme_xml(ooo,"25","+12","+9");break;
                    case 21 : charisme_xml(ooo,"30","+14","+10");break;
                    case 22 : charisme_xml(ooo,"35","+16","+11");break;
                    case 23 : charisme_xml(ooo,"40","+18","+12");break;
                    case 24 : charisme_xml(ooo,"45","+20","+13");break;
                    case 25 : charisme_xml(ooo,"50","+20","+14");break;
                    default : break;
                }
            }
            modif_xml(g_markup_dom_nom (ooo,"humour1","name"),nb_texte(ch,caract[HUMOUR]));
            modif_xml(g_markup_dom_nom (ooo,"beaute1","name"),nb_texte(ch,caract[BEAUTE]));
            save_race=race->add[vers_race].save;
            // ordre feuille  : pmp	pp	bbm	sou	sort
            if (save_race[0]!=0)
            {
                trouve_modif(save_race[0],&car,&coef);
                if (car!=10)
                {
                    spmp-=floor(caract[car]/coef);
                }
                else
                {
                    spmp-=coef;
                }
            }
            else
            {
            }
            if (save_race[1]!=0)
            {
                trouve_modif(save_race[1],&car,&coef);
                if (car!=10)
                {
                    spp-=floor(caract[car]/coef);
                }
                else
                {
                    spp-=coef;
                }
            }
            else
            {
            }
            if (save_race[2]!=0)
            {
                trouve_modif(save_race[2],&car,&coef);
                if (car!=10)
                {
                    sbb-=floor(caract[car]/coef);
                }
                else
                {
                    sbb-=coef;
                }
            }
            else
            {
            }
            if (save_race[3]!=0)
            {
                trouve_modif(save_race[3],&car,&coef);
                if (car!=10)
                {
                    ssou-=floor(caract[car]/coef);
                }
                else
                {
                    ssou-=coef;
                }
            }
            else
            {
            }
            if (save_race[4]!=0)
            {
                trouve_modif(save_race[4],&car,&coef);
                if (car!=10)
                {
                    ssor-=floor(caract[car]/coef);
                }
                else
                {
                    ssor-=coef;
                }
            }
            else
            {
            }
            if (bt!=0)
            {
                sprintf(affichage,"+4 , x%hd",bt);
                modif_xml(g_markup_dom_nom (ooo,"backstab","name"),affichage);
            }
            else
            { /* pas de back stab */
                modif_xml(g_markup_dom_nom (ooo,"backstab","name")," - -");
            }

            if (classe!=NULL)
            {
                for(i=0;classe[i]!=-1;i++)
                { /* je ne prends en compte que ADD2,ADD n'ayant pas de classe de psi */
                    if (classe[i]>=0 && _classe[classe[i]].add[ADD2].psi._2!=NULL)
                    {
                        niv_psi=max(niv_psi,niv_cl[i]);
                        if (_classe[classe[i]].add[ADD2].psi._2->debut.base!=NULL)
                        {
                            for(j=0;_classe[classe[i]].add[ADD2].psi._2->debut.base[j]!=FIN;j++)
                            {
                                if (_classe[classe[i]].add[ADD2].psi._2->debut.base[j]>=0)
                                {
                                    pt_psi+=_classe[classe[i]].add[ADD2].psi._2->base[caract[_classe[classe[i]].add[ADD2].psi._2->debut.base[j]]];
                                }
                                else
                                {
                                    pt_psi-=_classe[classe[i]].add[ADD2].psi._2->debut.base[j];
                                }
                            }
                        }
                        else
                        { /* une protection, mais ça devrait toujours être le cas */
                        }
                        if (_classe[classe[i]].add[ADD2].psi._2->debut.bonus!=NULL)
                        {
                            for(j=0;_classe[classe[i]].add[ADD2].psi._2->debut.bonus[j]!=FIN;j++)
                            {
                                pt_psi+=_classe[classe[i]].add[ADD2].psi._2->bonus[caract[_classe[classe[i]].add[ADD2].psi._2->debut.bonus[j]]];
                            }
                        }
                        else
                        { /* une protection, mais ça devrait toujours être le cas */
                        }

                        if (_classe[classe[i]].add[ADD2].psi._2->niveau.base!=NULL)
                        {
                            for(j=0;_classe[classe[i]].add[ADD2].psi._2->niveau.base[j]!=FIN;j++)
                            {
                                if (_classe[classe[i]].add[ADD2].psi._2->niveau.base[j]>=0)
                                {
                                    pt_psi+=_classe[classe[i]].add[ADD2].psi._2->base[caract[_classe[classe[i]].add[ADD2].psi._2->niveau.base[j]]]*(niv_cl[i]-1);
                                }
                                else
                                {
                                    pt_psi-=_classe[classe[i]].add[ADD2].psi._2->niveau.base[j]*(niv_cl[i]-1);
                                }
                            }
                        }
                        else
                        { /* une protection, mais ça devrait toujours être le cas */
                        }
                        if (_classe[classe[i]].add[ADD2].psi._2->niveau.bonus!=NULL)
                        {
                            for(j=0;_classe[classe[i]].add[ADD2].psi._2->niveau.bonus[j]!=FIN;j++)
                            {
                                pt_psi+=_classe[classe[i]].add[ADD2].psi._2->bonus[caract[_classe[classe[i]].add[ADD2].psi._2->niveau.bonus[j]]]*(niv_cl[i]-1);
                            }
                        }
                        else
                        { /* une protection, mais ça devrait toujours être le cas */
                        }
                    }
                    else
                    { /* classe sans psi */
                    }
                }
            }
            else
            { /* pas de classe choisie */
            }
            if (vdr==0)
            {/* pas de vaderetro affiché */
                niv=_min(niv,14)-1;
                tmp_node=g_markup_dom_nom (ooo,"dern_sect","name");
                tmp1_node=g_markup_dom_nom (ooo,"traiVD","name");
                for(i=0;i<(signed short)(tmp_node->nb_fils);i++)
                {
                    if (tmp_node->fils+i==tmp1_node)
                    {
                        supprime_node(tmp_node,i);
                        i=tmp_node->nb_fils;
                    }
                    else
                    {
                    }
                }
                tmp1_node=g_markup_dom_nom (ooo,"texteVD","name");
                for(i=0;i<(signed short)(tmp_node->nb_fils);i++)
                {
                    if (tmp_node->fils+i==tmp1_node)
                    {
                        supprime_node(tmp_node,i);
                        i=tmp_node->nb_fils;
                    }
                    else
                    {
                    }
                }
                tmp1_node=g_markup_dom_nom (ooo,"tableVD","name");
                for(i=0;i<(signed short)tmp_node->nb_fils;i++)
                {
                    if (tmp_node->fils+i==tmp1_node)
                    {
                        supprime_node(tmp_node,i);
                        i=tmp_node->nb_fils;
                    }
                    else
                    {
                    }
                }
            }
            else
            { /* on a affiché un vade rétro */
            }

            if (_perso->perso.bague!=NULL)
            {
                spmp-=_perso->perso.bague[0];/* bonus bague sur save */
                sbb-=_perso->perso.bague[0];
                ssou-=_perso->perso.bague[0];
                ssor-=_perso->perso.bague[0];
                spp-=_perso->perso.bague[0];
                sprintf(ch,"bague %+hd",_perso->perso.bague[0]);
                modif_xml(g_markup_dom_nom (ooo,"bague_CA","name"),ch);
            }
            else
            { /* pas de bague de protection */
                modif_xml(g_markup_dom_nom (ooo,"bague_CA","name"),"-");
            }
            spmp-=_perso->perso.autre_save;/* bonus autre sur save */
            sbb-=_perso->perso.autre_save;
            ssou-=_perso->perso.autre_save;
            ssor-=_perso->perso.autre_save;
            spp-=_perso->perso.autre_save;

            sprintf(affichage,"%hd",spmp);
            modif_xml(g_markup_dom_nom (ooo,"spmp","name"),affichage);
            sprintf(affichage,"%hd",sbb);
            modif_xml(g_markup_dom_nom (ooo,"sbb","name"),affichage);
            sprintf(affichage,"%hd",spp);
            modif_xml(g_markup_dom_nom (ooo,"spp","name"),affichage);
            sprintf(affichage,"%hd",ssor);
            modif_xml(g_markup_dom_nom (ooo,"ssor","name"),affichage);
            sprintf(affichage,"%hd",ssou);
            modif_xml(g_markup_dom_nom (ooo,"ssou","name"),affichage);
            sprintf(affichage,"%hd",taco);
            modif_xml(g_markup_dom_nom (ooo,"taco","name"),affichage);

            arm_nat=race->add[vers_race].CA;
            ca1=ARMURE[_perso->perso.armure1].ac-_perso->perso.mag_armure1;
            ca2=ARMURE[_perso->perso.armure2].ac-_perso->perso.mag_armure2;
            ca1+=bonus_ca;
            ca2+=bonus_ca;
            bonus_ca=0;

            /*if (_perso->perso.bague!=NULL)
            {
                sprintf(aff,"%+hd",-_perso->perso.bague[0]);
            }
            else
            { // pas de bague => pas de bonus
                sprintf(aff,"-");
            }
            sprintf(aff,"azerty");  si un jour, ça apparait, je serais heureux de le savoir  : attente de savoir */

            if (_perso->perso.bouclier!=NULL)
            {
                if (_perso->perso.bouclier[0]==0)
                { /* bouclier non magique */
                    modif_xml(g_markup_dom_nom (ooo,"bouclier","name"),"Bouclier normal");
                    sprintf(aff,"-1");
                    ca1-=1;
                    ca2-=1;
                }
                else
                {
                    bonus_ca=1; /* on a pris en compte le bonus du bouclier => disparition du bonus de la bague... */
                    ca1-=1+_perso->perso.bouclier[0];
                    ca2-=1+_perso->perso.bouclier[0];
                    sprintf(ch,"Bouclier %+hd",_perso->perso.bouclier[0]);
                    modif_xml(g_markup_dom_nom (ooo,"bouclier","name"),ch);
                    sprintf(aff,"%+hd",-1-_perso->perso.bouclier[0]);
                }
                modif_xml(g_markup_dom_nom (ooo,"bouclier1","name"),aff);
                modif_xml(g_markup_dom_nom (ooo,"bouclier2","name"),aff);
            }
            else
            { /* pas de bouclier */
            }

            *ch=0; /* initialisation du nom de l'armure */
            if (strncmp(ARMURE[_perso->perso.armure1].nom,"Bracelet",8)==0)
            {
                sprintf(affichage,"%+hd",ARMURE[_perso->perso.armure1].ac);
                sprintf(ch,"%s ",ARMURE[_perso->perso.armure1].nom);
                if (_perso->perso.bague!=NULL && bonus_ca==0)
                {
                    ca1-=_perso->perso.bague[0]; /* bague bonus CA + save */
                    sprintf(aff,"%+hd",-_perso->perso.bague[0]);
                }
                else
                { /* pas de bague => pas de bonus */
                    sprintf(aff,"-");
                }
                _perso->perso.mag_armure1=0;
            }
            else
            {
                if (strncmp(ARMURE[_perso->perso.armure1].nom,"Aucune",6)==0)
                {
                     sprintf(affichage,"%+hd",race->add[vers_race].CA);
                     if (_perso->perso.bague && bonus_ca==0)
                     {
                        ca1-=_perso->perso.bague[0]; /* bague bonus CA + save */
                        sprintf(aff,"%+hd",-_perso->perso.bague[0]);
                     }
                     else
                     {
                         sprintf(aff,"-");
                     }
                     strcpy(ch,"Aucune");
                }
                else if (_perso->perso.mag_armure1!=0)
                { /* armure magique */
                    /*ca1-=_perso->perso.mag_armure1; : bonus déjà pris en compte */
                    sprintf(affichage,"%+hd",ARMURE[_perso->perso.armure1].ac-_perso->perso.mag_armure1);
                    sprintf(ch,"%s %+hd",ARMURE[_perso->perso.armure1].nom,_perso->perso.mag_armure1);
                    sprintf(aff,"x");
                }
                else
                { /* armure non magique : on compte la bague */
                    sprintf(affichage,"%+hd",ARMURE[_perso->perso.armure1].ac);
                    if (_perso->perso.bague!=NULL && bonus_ca==0) /* mais pas si bouclier magique */
                    {
                        ca1-=_perso->perso.bague[0]; /* bague bonus CA + save */
                        sprintf(aff,"%+hd",-_perso->perso.bague[0]);
                    }
                    else
                    { /* pas de bague => pas de bonus */
                        sprintf(aff,"-");
                    }
                     sprintf(ch,"%s",ARMURE[_perso->perso.armure1].nom);
                }
            }
            modif_xml(g_markup_dom_nom (ooo,"autre_CA1","name"),aff);
            modif_xml(g_markup_dom_nom (ooo,"armure1","name"),affichage);

            sprintf(affichage,"%s",ARMURE[_perso->perso.armure2].nom);
            sprintf(aff,"%+hd",ca2);
            modif_xml(g_markup_dom_nom (ooo,"ca2","name"),aff);

            if (strncmp(ARMURE[_perso->perso.armure2].nom,"Bracelet",8)==0)
            {
                sprintf(affichage,"%+hd",ARMURE[_perso->perso.armure2].ac);
                sprintf(ch+strlen(ch),"/%s ",ARMURE[_perso->perso.armure2].nom);
                if (_perso->perso.bague!=NULL && bonus_ca==0)
                {
                    ca2-=_perso->perso.bague[0]; /* bague bonus CA + save */
                    sprintf(aff,"%+hd",-_perso->perso.bague[0]);
                }
                else
                { /* pas de bague => pas de bonus */
                    sprintf(aff,"-");
                }
                _perso->perso.mag_armure2=0;
            }
            else
            {
                if (strncmp(ARMURE[_perso->perso.armure2].nom,"Aucune",6)==0)
                {
                     sprintf(affichage,"%+hd",race->add[vers_race].CA);
                     if (_perso->perso.bague && bonus_ca==0)
                     {
                        ca2-=_perso->perso.bague[0]; /* bague bonus CA + save */
                        sprintf(aff,"%+hd",-_perso->perso.bague[0]);
                     }
                     else
                     {
                         sprintf(aff,"-");
                     }
                     strcpy(ch+strlen(ch),"/Aucune");
                }
                else if (_perso->perso.mag_armure2!=0)
                { /* armure magique */
                    /* ca2-=_perso->perso.mag_armure2; bonus déjà pris en compte */
                    sprintf(affichage,"%+hd",ARMURE[_perso->perso.armure2].ac-_perso->perso.mag_armure2);
                    sprintf(ch+strlen(ch),"/%s %+hd",ARMURE[_perso->perso.armure2].nom,_perso->perso.mag_armure2);
                    sprintf(aff,"x");
                }
                else
                { /* armure non magique : on compte la bague */
                    sprintf(affichage,"%+hd",ARMURE[_perso->perso.armure2].ac);
                    if (_perso->perso.bague!=NULL && bonus_ca==0)
                    {
                        ca2-=_perso->perso.bague[0]; /* bague bonus CA + save */
                        sprintf(aff,"%+hd",-_perso->perso.bague[0]);
                    }
                    else
                    { /* pas de bague => pas de bonus */
                        sprintf(aff,"-");
                    }
                    sprintf(ch+strlen(ch),"/%s",ARMURE[_perso->perso.armure2].nom);
                }
            }
            modif_xml(g_markup_dom_nom (ooo,"autre_CA2","name"),aff);
            modif_xml(g_markup_dom_nom (ooo,"armure2","name"),affichage);
            modif_xml(g_markup_dom_nom (ooo,"armure","name"),ch); /* les deux armures */
            sprintf(affichage,"%s",ARMURE[_perso->perso.armure2].nom);

            if (ca_bonus_race!=0 && (vers_race==ADD2))
            { /* on vérifie qu'on ne peux pas passer en dessous la CA min permise par le bonus */
                arm_nat-=ca_bonus_race;
                i=ca1; /* pour l'affichage final */
                ca1-=_perso->perso.autre_ca;
                j=ca1; /* pour le test de maximum racial */
                ca1-=ca_bonus_race;
                if (race->add[vers_race].CA!=10 && arm_nat<=ca1) /* premier test : pas d'armure naturelle */
                {
                    ca1=arm_nat-1;
                }
                else
                { /* on ne prend pas en compte l'armure naturelle */
                } /* l'armure partée est mieux                    */
                /* on prend le meilleur de l'armure ou de la limite entre armure avec bonus */
                ca1=_min(max(race->add[ADD2].modif_CA[3],ca1),j);
                if (_perso->perso.autre_ca==0)
                {
                    modif_xml(g_markup_dom_nom (ooo,"prot_spe","name"),"Bonus race :");
                }
                else
                {
                    modif_xml(g_markup_dom_nom (ooo,"prot_spe","name"),"Bonus race + autre :");
                }
                sprintf(aff,"%+hd",ca1-i);
                modif_xml(g_markup_dom_nom (ooo,"prot_spe1","name"),aff);

                i=ca2; /* pour l'affichage final */
                ca2-=_perso->perso.autre_ca;
                j=ca2; /* pour le test de maximum racial */
                ca2-=ca_bonus_race;
                if (race->add[vers_race].CA!=10 && arm_nat<=ca2)
                {
                    ca2=arm_nat-1;
                }
                else
                { /* on ne prend pas en compte l'armure naturelle */
                } /* l'armure portée est mieux                    */
                /* on prend le meilleur de l'armure ou de la limite entre armure avec bonus */
                ca2=_min(max(race->add[ADD2].modif_CA[3],ca2),j);
                sprintf(aff,"%+hd",ca2-i);
                modif_xml(g_markup_dom_nom (ooo,"prot_spe2","name"),aff);
            }
            else
            { /* pas de limite à prendre en compte avec les bonus de race pour la ca */
                    if (_perso->perso.autre_ca==0)
                    {
                    }
                    else
                    {
                        modif_xml(g_markup_dom_nom (ooo,"prot_spe","name"),"autre :");
                        sprintf(aff,"%+hd",-_perso->perso.autre_ca);
                        modif_xml(g_markup_dom_nom (ooo,"prot_spe1","name"),aff);
                        modif_xml(g_markup_dom_nom (ooo,"prot_spe2","name"),aff);
                        ca1-=_perso->perso.autre_ca;
                        ca2-=_perso->perso.autre_ca;
                    }
            }

            sprintf(aff,"%+hd",ca1);
            modif_xml(g_markup_dom_nom (ooo,"ca1","name"),aff);
            strcpy(affichage,"Classe d’armure : ");
            strcat(affichage,aff);
            strcat(affichage," / ");
            sprintf(aff,"%+hd",ca2);
            modif_xml(g_markup_dom_nom (ooo,"ca2","name"),aff);
            strcat(affichage,aff);
            modif_xml(g_markup_dom_nom (ooo,"ca2","name"),aff);
            modif_xml(g_markup_dom_nom (ooo,"bilan_ca","name"),affichage);


            if (arme!=NULL)
            {
                for (m=0;arme[m]>=0;m++);
            }
            else
            {
                m=0;
            }
            node=g_markup_dom_nom (ooo,"tableau_arme","name");
            for(i=4;i<m;i++)
            {
                xml_ajoute_fin(node,"table:table-row");         /* ajout d'une node table:table-row à la fin    */
                copie_node((node->fils)+node->nb_fils-1,(node->fils)+node->nb_fils-2);    /* remet à la fin la dernière node              */
                copie_node((node->fils)+node->nb_fils-2,(node->fils)+node->nb_fils-4);    /* remet à la fin la dernière node              */
                //copie_node((node->fils)+node->nb_fils-2,(node->fils)+j);    /* met la node modèle à l'avant dernière place  */
            }
            tmp_node=g_markup_dom_nom (ooo,"attaque_dos","name");
            sprintf(aff,"%hd",i);
            xml_ajoute_fin_attribut(tmp_node);    /* rajouter un élément attribut */
            xml_ecrit_dernier_attribut(tmp_node,"table:number-rows-spanned",aff);

            for(i=9;(guint)i<node->nb_fils;i++)
            {

                for(k=0;k<(signed short)(node->fils[i].nb_fils);k++)
                {
                    for(l=0;l<(signed short)node->fils[i].fils[k].fils[0].nb_att;l++)
                    {
                        if (strcmp("nom_arme3",node->fils[i].fils[k].fils[0].attributs[l].value)==0)
                        {
                            g_free(node->fils[i].fils[k].fils[0].attributs[l].value);
                            sprintf(aff,"nom_arme%hu",i-5);
                            node->fils[i].fils[k].fils[0].attributs[l].value=g_strdup(aff);
                        }
                        else if (strcmp("bt_f_a3",node->fils[i].fils[k].fils[0].attributs[l].value)==0)
                        {
                            g_free(node->fils[i].fils[k].fils[0].attributs[l].value);
                            sprintf(aff,"bt_f_a%hu",i-5);
                            node->fils[i].fils[k].fils[0].attributs[l].value=g_strdup(aff);
                        }
                        else if (strcmp("bt_m_a3",node->fils[i].fils[k].fils[0].attributs[l].value)==0)
                        {
                            g_free(node->fils[i].fils[k].fils[0].attributs[l].value);
                            sprintf(aff,"bt_m_a%hu",i-5);
                            node->fils[i].fils[k].fils[0].attributs[l].value=g_strdup(aff);
                        }
                        else if (strcmp("bd_f_a3",node->fils[i].fils[k].fils[0].attributs[l].value)==0)
                        {
                            g_free(node->fils[i].fils[k].fils[0].attributs[l].value);
                            sprintf(aff,"bd_f_a%hu",i-5);
                            node->fils[i].fils[k].fils[0].attributs[l].value=g_strdup(aff);
                        }
                        else if (strcmp("bd_m_a3",node->fils[i].fils[k].fils[0].attributs[l].value)==0)
                        {
                            g_free(node->fils[i].fils[k].fils[0].attributs[l].value);
                            sprintf(aff,"bd_m_a%hu",i-5);
                            node->fils[i].fils[k].fils[0].attributs[l].value=g_strdup(aff);
                        }
                        else if (strcmp("d_p/m_a3",node->fils[i].fils[k].fils[0].attributs[l].value)==0)
                        {
                            g_free(node->fils[i].fils[k].fils[0].attributs[l].value);
                            sprintf(aff,"d_p/m_a%hu",i-5);
                            node->fils[i].fils[k].fils[0].attributs[l].value=g_strdup(aff);
                        }
                        else if (strcmp("d_g_a3",node->fils[i].fils[k].fils[0].attributs[l].value)==0)
                        {
                            g_free(node->fils[i].fils[k].fils[0].attributs[l].value);
                            sprintf(aff,"d_g_a%hu",i-5);
                            node->fils[i].fils[k].fils[0].attributs[l].value=g_strdup(aff);
                        }
                        else if (strcmp("bd_t_a3",node->fils[i].fils[k].fils[0].attributs[l].value)==0)
                        {
                            g_free(node->fils[i].fils[k].fils[0].attributs[l].value);
                            sprintf(aff,"bd_t_a%hu",i-5);
                            node->fils[i].fils[k].fils[0].attributs[l].value=g_strdup(aff);
                        }
                        else if (strcmp("taco_a3",node->fils[i].fils[k].fils[0].attributs[l].value)==0)
                        {
                            g_free(node->fils[i].fils[k].fils[0].attributs[l].value);
                            sprintf(aff,"taco_a%hu",i-5);
                            node->fils[i].fils[k].fils[0].attributs[l].value=g_strdup(aff);
                        }
                        else if (strcmp("cap_a3",node->fils[i].fils[k].fils[0].attributs[l].value)==0)
                        {
                            g_free(node->fils[i].fils[k].fils[0].attributs[l].value);
                            sprintf(aff,"cap_a%hu",i-5);
                            node->fils[i].fils[k].fils[0].attributs[l].value=g_strdup(aff);
                        }
                        else if (strcmp("cr_a3",node->fils[i].fils[k].fils[0].attributs[l].value)==0)
                        {
                            g_free(node->fils[i].fils[k].fils[0].attributs[l].value);
                            sprintf(aff,"cr_a%hu",i-5);
                            node->fils[i].fils[k].fils[0].attributs[l].value=g_strdup(aff);
                        }
                        else
                        {
                        }
                    }
                }
            }


            if (arme!=NULL)
            {
                node=g_markup_dom_nom (ooo,"init","name");
                if (_perso->perso.cap=='o')
                {
                    tmp_node=g_markup_dom_nom (node,"loin","name");
                    modif_xml(tmp_node,"Loin");
                    tmp_node=g_markup_dom_nom (node,"pres","name");
                    modif_xml(tmp_node,"Près");
                }
                else
                {
                    tmp_node=g_markup_dom_nom (node,"loin","name");
                    modif_xml(tmp_node,"FV");
                    tmp_node=g_markup_dom_nom (node,"pres","name");
                    modif_xml(tmp_node,"Long.");
                }
                for (i=0;arme[i]>=0;i++)
                {
                    sprintf(aff,"nom_arme%hu",i+1);
                    if (ARME[arme[i]].groupe==1)
                    {
                        j=(ARME[arme[i]].vitesse==0?taco_lan:taco_tmp)-taco;
                        switch (_perso->perso.niv_arme[i])
                        {
                            case 0 : sprintf(affichage,"%+hd%+hd",-j,nm);j-=nm;
                                     sprintf(ch,"%s n-m",ARME[arme[i]].nom);
                                     break;
                            case 2 : sprintf(affichage,"%+hd+1",-j);j-=1;
                                     sprintf(ch,"%s (SP)",ARME[arme[i]].nom);
                                     break;
                            case 3 : sprintf(affichage,"%+hd+3",-j);j-=3;
                                     sprintf(ch,"%s (2SP)",ARME[arme[i]].nom);
                                     break;
                            default: sprintf(affichage,"%+hd",-j);
                                     sprintf(ch,"%s",ARME[arme[i]].nom);
                                     break;
                        }
                        modif_xml(g_markup_dom_nom (ooo,aff,"name"),ch);

                        sprintf(aff,"bt_f_a%hu",i+1);
                        modif_xml(g_markup_dom_nom (ooo,aff,"name"),affichage);
                        j+=(taco-_perso->perso.mag_arme[i]-_perso->perso.arme_autre_t[i]);

                        if (_perso->perso.arme_autre_t[i]==0)
                        {
                            sprintf(affichage,"%+hd",_perso->perso.mag_arme[i]);
                        }
                        else
                        {
                            sprintf(affichage,"%+hd%+hd",_perso->perso.mag_arme[i],_perso->perso.arme_autre_t[i]);
                        }
                        sprintf(aff,"bt_m_a%hu",i+1);
                        modif_xml(g_markup_dom_nom (ooo,aff,"name"),affichage);
                        if (_perso->perso.arme_autre_d[i]==0)
                        {
                            sprintf(affichage,"%+hd",_perso->perso.mag_arme[i]);
                        }
                        else
                        {
                            sprintf(affichage,"%+hd%+hd",_perso->perso.mag_arme[i],_perso->perso.arme_autre_d[i]);
                        }
                        sprintf(aff,"bd_m_a%hu",i+1);
                        modif_xml(g_markup_dom_nom (ooo,aff,"name"),affichage);

                        for (l=0;race->add[vers_race].bonus_arme[l].arme!=NULL;l++)
                        {
                            if (compare_sans_casse(ARME[arme[i]].nom,race->add[vers_race].bonus_arme[l].arme)==0)
                            {
                                j-=race->add[vers_race].bonus_arme[l].bonus;
                            }
                            else
                            {
                            }
                        }

                        sprintf(affichage,"%hd",j);
                        sprintf(aff,"taco_a%hu",i+1);
                        modif_xml(g_markup_dom_nom (ooo,aff,"name"),affichage);
                        taco_a=g_realloc(taco_a,(i+1)*sizeof(signed short));
                        taco_a[i]=j;

                        sprintf(aff,"bd_f_a%hu",i+1);
                        if (ARME[arme[i]].vitesse==0)
                        {
                            k=0;
                        }
                        else
                        {
                            k=bonus; /* seules les armes de jets doivent avoir 0 ou rien en vitesse */
                        }
                        switch (_perso->perso.niv_arme[i])
                        {
                            case 2 : sprintf(affichage,"%+hd+2",k);k+=2;break;
                            case 3 : sprintf(affichage,"%+hd+3",k);k+=3;break;
                            default: sprintf(affichage,"%+hd",k);break;
                        }
                        modif_xml(g_markup_dom_nom (ooo,aff,"name"),affichage);

                        sprintf(aff,"bd_t_a%hu",i+1);
                        sprintf(affichage,"%+hd",k+_perso->perso.mag_arme[i]+_perso->perso.arme_autre_d[i]);
                        modif_xml(g_markup_dom_nom (ooo,aff,"name"),affichage);
                        degat_a=g_realloc(degat_a,(i+1)*sizeof(signed short));
                        degat_a[i]=k+_perso->perso.mag_arme[i];

                        /* CAP-CR : éviter les calculs avec les armes non complètement remplies ou les armes de jet */
                        if (_perso->perso.cap=='o')
                        {
                            sprintf(aff,"cap_a%hu",i+1);
                            if (ARME[arme[i]].vitesse>0 && ARME[arme[i]].longueur>0)
                            {
                                j=ARME[arme[i]].vitesse;
                                if (_version==ADD2)
                                {
                                     j=max(j-_perso->perso.mag_arme[i],1);
                                }
                                else
                                {
                                    /* pour ADD1, on ne prend pas en compte la magie dans le facteur de rapidité de l'arme */
                                }
                                if (caract[FORCE]<=5)
                                {
                                 j*=1.2;
                                }
                                else if  (caract[FORCE]<=8)
                                {
                                 j*=1.1;
                                }
                                else if  (caract[FORCE]==25)
                                {
                                 j*=.2;
                                }
                                else if  (caract[FORCE]==24)
                                {
                                 j*=.3;
                                }
                                else if  (caract[FORCE]==23)
                                {
                                 j*=.4;
                                }
                                else if  (caract[FORCE]>=21)
                                {
                                 j*=.5;
                                }
                                else if  (caract[FORCE]>=19)
                                {
                                 j*=.6;
                                }
                                else if  (caract[FORCE]==18)
                                {
                                 if (caract[POURCENTAGE]>=76)
                                 {
                                   j*=.7;
                                 }
                                 else
                                 {
                                   j*=.8;
                                 }
                                }
                                else if  (caract[FORCE]==17)
                                {
                                 j*=.8;
                                }
                                else if  (caract[FORCE]>=14)
                                {
                                 j*=.9;
                                }
                                else
                                { /* fin traitement force */
                                }
                                if (j<2)
                                {
                                  j--;
                                }
                                else
                                {
                                  j++;
                                }
                                j=3-j/2;
                                if (caract[DEXTERITE]>=24)
                                {
                                  j+=3;
                                }
                                else if (caract[DEXTERITE]>=21)
                                {
                                  j+=2;
                                }
                                else if (caract[DEXTERITE]>=18)
                                {
                                  j++;
                                }
                                else if (caract[DEXTERITE]<=4)
                                {
                                  j-=2;
                                }
                                else if (caract[DEXTERITE]<=14)
                                {
                                  j--;
                                }
                                else
                                { /* fin traiment dextérité */
                                }
                                switch (_perso->perso.niv_arme[i])
                                {
                                    case 2 : /*j+=2;break;*/
                                    case 3 : j+=3;break; /* spé => comme niveau 7 => +3 en init */
                                    default: break;
                                }
                                sprintf(affichage,"%+hd",j+bonus_init);
                                modif_xml(g_markup_dom_nom (ooo,aff,"name"),affichage);

                                j=(ARME[arme[i]].longueur-1)/30-3;
                                if (j>0)
                                {
                                    j--;
                                }
                                else
                                {
                                }
                                j=_min(j,3);
                                j=max(j,-3);
                                if (caract[DEXTERITE]>=18)
                                {
                                 j+=2;
                                }
                                else if (caract[DEXTERITE]>=15)
                                {
                                 j++;
                                }
                                else if (caract[DEXTERITE]<=4)
                                {
                                 j-=2;
                                }
                                else if (caract[DEXTERITE]<=7)
                                {
                                 j--;
                                }
                                else
                                { /* fin traitement dextérité */
                                }
                                switch (_perso->perso.niv_arme[i])
                                {
                                    case 2 : /*j+=2;break;*/
                                    case 3 : j+=3;break;
                                    default: break;
                                }

                                sprintf(affichage,"%+hd",j+bonus_init);
                                sprintf(aff,"cr_a%hu",i+1);
                                modif_xml(g_markup_dom_nom (ooo,aff,"name"),affichage);
                            }
                            else
                            {
                                    modif_xml(g_markup_dom_nom (ooo,aff,"name"),tabl_dexterite[_version][caract[DEXTERITE]-1][0]);
                            }
                        }
                        else
                        { /* on ne joue pas les CAP/CR : affichage du facteur de rapidité et de la longueur */
                            sprintf(affichage,"%hd",ARME[arme[i]].vitesse);
                            sprintf(aff,"cr_a%hu",i+1);
                            modif_xml(g_markup_dom_nom (ooo,aff,"name"),affichage);
                            sprintf(affichage,"%hd",ARME[arme[i]].longueur);
                            sprintf(aff,"cap_a%hu",i+1);
                            modif_xml(g_markup_dom_nom (ooo,aff,"name"),affichage);
                        }

                        sprintf(aff,"d_p/m_a%hu",i+1);
                        modif_xml(g_markup_dom_nom (ooo,aff,"name"),ARME[arme[i]].PM);
                        sprintf(aff,"d_g_a%hu",i+1);
                        modif_xml(g_markup_dom_nom (ooo,aff,"name"),ARME[arme[i]].G);
                    }
                    else
                    {
                        degat_a=g_realloc(degat_a,(i+1)*sizeof(signed short));
                        degat_a[i]=0;
                        taco_a=g_realloc(taco_a,(i+1)*sizeof(signed short));
                        taco_a[i]=0;
                        if (ARME[arme[i]].groupe==2)
                        {
                             sprintf(ch,"%s, petit groupe",ARME[arme[i]].nom);
                        }
                        else
                        {
                            sprintf(ch,"%s, grand groupe",ARME[arme[i]].nom);
                        }
                        modif_xml(g_markup_dom_nom (ooo,aff,"name"),ch);
                    }

                }
            }
            else
            { /* pas d'arme */
            }
            sprintf(affichage,"n.m : %hd",nm);
            modif_xml(g_markup_dom_nom (ooo,"nonmaitrise","name"),affichage);

            /* les psi avant les pdv : on recopie la zone vierge */
            if (_perso->perso.psi!=NULL && _perso->perso.psi[0]>=0)
            {
                node=g_markup_dom_node(ooo,"office:text");
                xml_ajoute_fin(node,"text:p");
                xml_ajoute_fin(node,"table:table");
                node=node->fils+node->nb_fils-1; /* on arrive sur la dernière node */
                for (j=0;j<8;j++) xml_ajoute_fin(node,"table:table-column");
                for (j=0;j<8;j++)
                {
                    xml_ajoute_fin_attribut(node->fils+j); /* rajouter un élément attribut */
                    xml_ecrit_dernier_attribut(node->fils+j,"table:style-name","Tableau1.VDA"); /* remplace le dernier attribut */
                }
                xml_ecrit_dernier_attribut(node->fils+2,"table:style-name","Tableau1.C"); /* remplace le dernier attribut */
                xml_ecrit_dernier_attribut(node->fils+5,"table:style-name","Tableau1.C"); /* remplace le dernier attribut */
                xml_ecrit_dernier_attribut(node->fils+6,"table:style-name","Tableau1.C"); /* remplace le dernier attribut */
                xml_ecrit_dernier_attribut(node->fils+7,"table:style-name","Tableau1.C"); /* remplace le dernier attribut */

                xml_ajoute_fin(node,"table:table-row"); /* node pointera sur le tableau du niveau */
                tmp_node=node->fils+node->nb_fils-1; /* on arrive sur la dernière node */
                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                xml_ajoute_fin_attribut(tmp_node->fils); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils,"table:style-name","Tableau1.VD1"); /* remplace le dernier attribut */
                xml_ajoute_fin_attribut(tmp_node->fils); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils,"office:value-type","string"); /* remplace le dernier attribut */
                xml_ajoute_fin(tmp_node->fils,"text:p");
                xml_ajoute_fin_texte(tmp_node->fils->fils);
                xml_ecrit_dernier_texte(tmp_node->fils->fils,"Nom");
                /* Discipline */
                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                xml_ajoute_fin(tmp_node->fils+1,"text:p");
                xml_ajoute_fin_texte(tmp_node->fils[1].fils);
                xml_ecrit_dernier_texte(tmp_node->fils[1].fils,"Discipline");
                xml_ajoute_fin_attribut(tmp_node->fils+1); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+1,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
                xml_ajoute_fin_attribut(tmp_node->fils+1); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+1,"office:value-type","string"); /* remplace le dernier attribut */
                /* Science */
                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                xml_ajoute_fin(tmp_node->fils+2,"text:p");
                xml_ajoute_fin_texte(tmp_node->fils[2].fils);
                xml_ecrit_dernier_texte(tmp_node->fils[2].fils,"Science");
                xml_ajoute_fin_attribut(tmp_node->fils+2); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+2,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
                xml_ajoute_fin_attribut(tmp_node->fils+2); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+2,"office:value-type","string"); /* remplace le dernier attribut */
                /* Score */
                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                xml_ajoute_fin(tmp_node->fils+3,"text:p");
                xml_ajoute_fin_texte(tmp_node->fils[3].fils);
                xml_ecrit_dernier_texte(tmp_node->fils[3].fils,"Score");
                xml_ajoute_fin_attribut(tmp_node->fils+3); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+3,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
                xml_ajoute_fin_attribut(tmp_node->fils+3); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+3,"office:value-type","string"); /* remplace le dernier attribut */
                /* Coût+maintient */
                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                xml_ajoute_fin(tmp_node->fils+4,"text:p");
                xml_ajoute_fin_texte(tmp_node->fils[4].fils);
                xml_ecrit_dernier_texte(tmp_node->fils[4].fils,"Coût+maintient");
                xml_ajoute_fin_attribut(tmp_node->fils+4); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+4,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
                xml_ajoute_fin_attribut(tmp_node->fils+4); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+4,"office:value-type","string"); /* remplace le dernier attribut */
                /* portée */
                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                xml_ajoute_fin(tmp_node->fils+5,"text:p");
                xml_ajoute_fin_texte(tmp_node->fils[5].fils);
                xml_ecrit_dernier_texte(tmp_node->fils[5].fils,"Portée");
                xml_ajoute_fin_attribut(tmp_node->fils+5); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+5,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
                xml_ajoute_fin_attribut(tmp_node->fils+5); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+5,"office:value-type","string"); /* remplace le dernier attribut */
                /* Temps de préparation */
                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                xml_ajoute_fin(tmp_node->fils+6,"text:p");
                xml_ajoute_fin_texte(tmp_node->fils[6].fils);
                xml_ecrit_dernier_texte(tmp_node->fils[6].fils,"Tps prép");
                xml_ajoute_fin_attribut(tmp_node->fils+6); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+6,"table:style-name","Tableau1.VD2"); /* remplace le dernier attribut */
                xml_ajoute_fin_attribut(tmp_node->fils+6); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+6,"office:value-type","string"); /* remplace le dernier attribut */
                /* zone */
                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                xml_ajoute_fin(tmp_node->fils+7,"text:p");
                xml_ajoute_fin_texte(tmp_node->fils[7].fils);
                xml_ecrit_dernier_texte(tmp_node->fils[7].fils,"Zone effet");
                xml_ajoute_fin_attribut(tmp_node->fils+7); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+7,"table:style-name","Tableau1.VD3"); /* remplace le dernier attribut */
                xml_ajoute_fin_attribut(tmp_node->fils+7); /* rajouter un élément attribut */
                xml_ecrit_dernier_attribut(tmp_node->fils+7,"office:value-type","string"); /* remplace le dernier attribut */


                for (i=1;_perso->perso.psi[i]>0;i++)
                {
                  for (j=0;j<NB_DISCIPLINE_PSI;j++)
                  {
                    for (k=0;k<2;k++)
                    {
                        for (l=NB_TALENTS_PSI[j][k]-1;l>=0;l--)
                        {
                            if (TALENTS_PSI[j][k][l].clef==_perso->perso.psi[i])
                            {
                                xml_ajoute_fin(node,"table:table-row"); /* node pointera sur le tableau du niveau */
                                tmp_node=node->fils+node->nb_fils-1; /* on arrive sur la dernière node */
                                /* première case : le nom du sort */
                                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                                xml_ajoute_fin(tmp_node->fils,"text:p");
                                xml_ajoute_fin_texte(tmp_node->fils->fils);
                                xml_ecrit_dernier_texte(tmp_node->fils->fils,TALENTS_PSI[j][k][l].nom);
                                xml_ajoute_fin_attribut(tmp_node->fils); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils,"table:style-name","Tableau1.VD11"); /* remplace le dernier attribut */
                                xml_ajoute_fin_attribut(tmp_node->fils); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils,"office:value-type","string"); /* remplace le dernier attribut */

                /* Discipline */
                                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                                xml_ajoute_fin(tmp_node->fils+1,"text:p");
                                xml_ajoute_fin_texte(tmp_node->fils[1].fils);
                                xml_ecrit_dernier_texte(tmp_node->fils[1].fils,DISCIPLINE_PSI[j].nom);
                                xml_ajoute_fin_attribut(tmp_node->fils+1); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+1,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
                                xml_ajoute_fin_attribut(tmp_node->fils+1); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+1,"office:value-type","string"); /* remplace le dernier attribut */
                /* Science */
                               xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                                xml_ajoute_fin(tmp_node->fils+2,"text:p");
                                xml_ajoute_fin(tmp_node->fils[2].fils,"text:span");
                                xml_ajoute_fin_texte(tmp_node->fils[2].fils->fils);
                                xml_ecrit_dernier_texte(tmp_node->fils[2].fils->fils,k?"Science":"Dévotion");
                                xml_ajoute_fin_attribut(tmp_node->fils+2); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+2,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
                                xml_ajoute_fin_attribut(tmp_node->fils+2); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+2,"office:value-type","string"); /* remplace le dernier attribut */
                /* Score */
                                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                                xml_ajoute_fin(tmp_node->fils+3,"text:p");
                                xml_ajoute_fin_texte(tmp_node->fils[3].fils);
                                sprintf(ch,"%s%+hd+%hu(niv)=%hd",
                                        nom_abr[TALENTS_PSI[j][k][l].caracteristique],
                                        TALENTS_PSI[j][k][l].modif,
                                        _perso->perso.niv_psi[i]-1,
                                        caract[TALENTS_PSI[j][k][l].caracteristique]+TALENTS_PSI[j][k][l].modif+_perso->perso.niv_psi[i]-1);
                                xml_ecrit_dernier_texte(tmp_node->fils[3].fils,ch);
                                xml_ajoute_fin_attribut(tmp_node->fils+3); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+3,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
                                xml_ajoute_fin_attribut(tmp_node->fils+3); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+3,"office:value-type","string"); /* remplace le dernier attribut */
                /* Coût+maintient */
                                /* Coût+Maintient */
                                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                                xml_ajoute_fin(tmp_node->fils+4,"text:p");
                                xml_ajoute_fin_texte(tmp_node->fils[4].fils);
                                xml_ecrit_dernier_texte(tmp_node->fils[4].fils,TALENTS_PSI[j][k][l].cout);
                                xml_ajoute_fin_attribut(tmp_node->fils+4); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+4,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
                                xml_ajoute_fin_attribut(tmp_node->fils+4); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+4,"office:value-type","string"); /* remplace le dernier attribut */
                /* portée */
                                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                                xml_ajoute_fin(tmp_node->fils+5,"text:p");
                                xml_ajoute_fin(tmp_node->fils[5].fils,"text:span");
                                xml_ajoute_fin_texte(tmp_node->fils[5].fils->fils);
                                xml_ecrit_dernier_texte(tmp_node->fils[5].fils->fils,TALENTS_PSI[j][k][l].portee);
                                xml_ajoute_fin_attribut(tmp_node->fils+5); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+5,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
                                xml_ajoute_fin_attribut(tmp_node->fils+5); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+5,"office:value-type","string"); /* remplace le dernier attribut */
                /* Temps de préparation */
                                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                                xml_ajoute_fin(tmp_node->fils+6,"text:p");
                                xml_ajoute_fin_texte(tmp_node->fils[6].fils);
                                xml_ecrit_dernier_texte(tmp_node->fils[6].fils,TALENTS_PSI[j][k][l].lance);
                                xml_ajoute_fin_attribut(tmp_node->fils+6); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+6,"table:style-name","Tableau1.VD21"); /* remplace le dernier attribut */
                                xml_ajoute_fin_attribut(tmp_node->fils+6); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+6,"office:value-type","string"); /* remplace le dernier attribut */
                /* zone */
                                xml_ajoute_fin(tmp_node,"table:table-cell"); /* création de la case */
                                xml_ajoute_fin(tmp_node->fils+7,"text:p");
                                xml_ajoute_fin_texte(tmp_node->fils[7].fils);
                                xml_ecrit_dernier_texte(tmp_node->fils[7].fils,TALENTS_PSI[j][k][l].zone);
                                xml_ajoute_fin_attribut(tmp_node->fils+7); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+7,"table:style-name","Tableau1.VD31"); /* remplace le dernier attribut */
                                xml_ajoute_fin_attribut(tmp_node->fils+7); /* rajouter un élément attribut */
                                xml_ecrit_dernier_attribut(tmp_node->fils+7,"office:value-type","string"); /* remplace le dernier attribut */

                                if (_perso->perso.natif_psi[i]==1)
                                {
                                     if (sscanf(TALENTS_PSI[j][k][l].cout,"(i:%hu",&l)==1)
                                     {
                                        pt_psi+=l;
                                     }
                                     else
                                     {
                                     }
                                     for (l=0;TALENTS_PSI[j][k][l].cout[l]!=':' && TALENTS_PSI[j][k][l].cout[l]!=0 ; l++);
                                     if (TALENTS_PSI[j][k][l].cout[l]==':') /* cout du maintient */
                                     {
                                        if (sscanf(TALENTS_PSI[j][k][l].cout+l+1,"%hu",&l)==1)
                                        {
                                             pt_psi+=4*l;
                                        }
                                        else
                                        {
                                        }
                                     }
                                     else
                                     {
                                     }
                                }
                            }
                            else
                            {
                                /* pas le bon talent */
                            }
                        }
                    }
                   }
                }
                node=g_markup_dom_node(ooo,"office:text");

                xml_ajoute_fin(node,"text:p");
                node=node->fils+node->nb_fils-1;
                copie_node(node,g_markup_dom_nom (ooo,"pdv","name")); /* on récupère le tableau des pdv */
                tmp_node=node->fils;
                for (i=0;(unsigned short)i<tmp_node->nb_att;i++)
                {
                    if (compare_sans_casse("svg:width",tmp_node->attributs[i].nom)==0)
                    { /* on augmente la largeur à la page */
                        g_free(tmp_node->attributs[i].value);
                        tmp_node->attributs[i].value=g_strdup("20cm");
                    }
                    else if (compare_sans_casse("svg:x",tmp_node->attributs[i].nom)==0)
                    { /* on place l'origine du tableau sur la marge */
                        g_free(tmp_node->attributs[i].value);
                        tmp_node->attributs[i].value=g_strdup("0cm");
                    }
                }
                tmp_node=tmp_node->fils->fils;
                supprime_node(tmp_node,0); /* on supprime les nb <0 des pdv */
                supprime_node(tmp_node,0);
                sprintf(ch,"PFP : %hu",pt_psi);
                xml_ecrit_dernier_texte(node,ch);

                if (pt_psi>60) /* extention de la case qui décompte les pts psi */
                {
                    pt_psi+=30;
                    ch[0]=0;
                    for (i=1;i<=pt_psi;i++)
                    {
                        sprintf(aff," %hu",i);
                        strncat(ch,aff,LONG-1);
                    }
                    xml_ecrit_dernier_texte(tmp_node->fils+1,ch);
                }
                else
                { /* pas besoin d'en rajouter */
                }
            }

            /* PDV après les psi car on copie la boite vierge des PDV dans les psi */
            sprintf(affichage,"Points de vie : ");
            j=0;
            if (_perso->perso.pdv!=NULL)
            {
                for(i=0;_perso->perso.pdv[i]>0;i++)
                {
                    j+=_perso->perso.pdv[i];
                    if (i!=0)
                    {
                        strcat(affichage,"+");
                    }
                    else
                    {
                    }
                    sprintf(ch,"%hd",_perso->perso.pdv[i]);
                    strcat(affichage,ch);
                }
            }
            else
            { /* pas de points de vie */
            }
            sprintf(ch,"=%hd",j);
            strcat(affichage,ch);
            modif_xml(g_markup_dom_nom (ooo,"pdv","name"),affichage);
            if (j>85) /* il y a 90 pdv max sur la feuille */
            {
                tmp1=g_strdup(" 91");
                for (i=92;i<j+11;i++)
                {
                 sprintf(ch, " %hu",i);
                 chemin=g_strconcat(tmp1,ch,NULL);
                 g_free(tmp1);
                 tmp1=chemin;
                }
                tmp_node=g_markup_dom_nom (ooo,"ligne_pdv","name");
                chemin=g_strconcat(tmp_node->texte[0].texte,tmp1,NULL);
                g_free(tmp_node->texte[0].texte);
                g_free(tmp1);
                tmp_node->texte[0].texte=chemin;
            }
            else
            { /* pas besoin de compléter */
            }

            /* les compétences */
            tmp_node=g_markup_dom_nom (ooo,"Connaissance","name");
            if (competence!=NULL)
            {

                for(i=0;competence[i]>=0;i++) /* on parcourt les compétences du perso */
                {
                    xml_ajoute_fin(tmp_node,"text:p");
                    copie_node(tmp_node->fils+tmp_node->nb_fils-1,tmp_node->fils+1);
                    xml_ajoute_fin_texte(tmp_node->fils+tmp_node->nb_fils-1);
                    xml_ecrit_dernier_texte(tmp_node->fils+tmp_node->nb_fils-1,COMPETENCE[competence[i]].nom);
                    xml_ajoute_fin(tmp_node->fils+tmp_node->nb_fils-1,"text:tab");
                    xml_ajoute_fin_texte(tmp_node->fils+tmp_node->nb_fils-1);
                    k=0;
                    for (l=0;classe[l]>=0;l++) /* on vérifie si la compétence est aussi une compétence de classe */
                    { /* bonus du à la classe */
                        if (_classe[classe[l]].add[ADD2].nom!=NULL)
                        {
                            k=0;
                            for (j=0;_classe[classe[l]].add[ADD2].compt_spe[0][j]!=NULL;j++)
                            {
                                if (compare_sans_casse((char *)_classe[classe[l]].add[ADD2].compt_spe[0][j],COMPETENCE[competence[i]].nom)==0)
                                { /* competence spéciale de la classe trouvée */
                                             k=_classe[classe[l]].add[ADD2].compt_spe[1][j][0]
                                                *(_perso->perso.niveau[l]-1)/_classe[classe[l]].add[ADD2].compt_spe[1][j][1]
                                                +1 /* +1 car la présence seule de la compétance augmente de 1 la maîtrise */
                                                +_classe[classe[l]].add[ADD2].compt_spe[1][j][2];
                                }
                                else
                                {
                                    /* pas la bonne compétance */
                                }
                            }
                        }
                        else
                        { /* les compétences sont une histoire add2, ce n'est une classe que add1 */
                        }
                    }
                    if (COMPETENCE[competence[i]].caract!=POURCENTAGE) /* % pour le cas NA */
                    {
                        sprintf(ch,"%hd",caract[COMPETENCE[competence[i]].caract]+COMPETENCE[competence[i]].modif+_perso->perso.niv_competence[i]-1+k);
                        xml_ecrit_dernier_texte(tmp_node->fils+tmp_node->nb_fils-1,ch);
                    }
                    else
                    {
                         xml_ecrit_dernier_texte(tmp_node->fils+tmp_node->nb_fils-1,"NA");
                    }
                }
                vdr=i; /*mémorisation du nombre de compétences sues */
            }
            else
            { /* pas de competence */
                vdr=0;
            }
            if (classe!=NULL)
            {
                for (n=0;classe[n]>=0;n++)
                {
                    for (j=0;_classe[classe[n]].add[ADD2].compt_spe[0][j]!=NULL;j++)
                    {
                        for(i=0;i<vdr;i++)
                        {
                            if (compare_sans_casse((char *)_classe[classe[n]].add[ADD2].compt_spe[0][j],COMPETENCE[competence[i]].nom)==0)
                            {
                                 i=vdr; /* compétence déjà comptée : à la sotie, i ferra vdr+1 */
                            }
                            else
                            { /* pas encore le bon */
                            }
                        }
                        if (i==vdr) /* pas sorti à cause d'une égalité => _classe[classe[n]].add[ADD2].compt_spe[0][j] n'a pas été pris en compte */
                        {
                            for (i=0;i<NB_COMPETENCE;i++)
                            { /* recherche de la compétence */
                                if (compare_sans_casse(COMPETENCE[i].nom,(char *)_classe[classe[n]].add[ADD2].compt_spe[0][j])==0)
                                {
                                    signed short num=0,denom=1,_bonus=0;
                                    if (sscanf((char *)_classe[classe[n]].add[ADD2].compt_spe[1][j],"'%hd/%hd%hd",&num,&denom,&_bonus)!=3)
                                    {
                                        printf("Erreur lecture compétence %s de la classe %s : %s\n",COMPETENCE[i].nom,_classe[classe[n]].add[ADD2].nom,_classe[classe[n]].add[ADD2].compt_spe[1][j]);
                                    }
                                    if (denom==0)
                                    {
                                        printf("Erreur lecture dénominateur compétence %s de la classe %s : %s\n",COMPETENCE[i].nom,_classe[classe[n]].add[ADD2].nom,_classe[classe[n]].add[ADD2].compt_spe[1][j]);
                                        denom=1;
                                    }
                                    xml_ajoute_fin(tmp_node,"text:p");
                                    copie_node(tmp_node->fils+tmp_node->nb_fils-1,tmp_node->fils+1);
                                    xml_ajoute_fin_texte(tmp_node->fils+tmp_node->nb_fils-1);
                                    xml_ecrit_dernier_texte(tmp_node->fils+tmp_node->nb_fils-1,COMPETENCE[i].nom);
                                    xml_ajoute_fin(tmp_node->fils+tmp_node->nb_fils-1,"text:tab");
                                    xml_ajoute_fin_texte(tmp_node->fils+tmp_node->nb_fils-1);
                                    if (COMPETENCE[i].caract!=POURCENTAGE) /* % pour le cas NA */
                                    {
                                        sprintf(ch,"%hd",caract[COMPETENCE[i].caract]+COMPETENCE[i].modif+num*(_perso->perso.niveau[n]-1)/denom+_bonus);
                                        xml_ecrit_dernier_texte(tmp_node->fils+tmp_node->nb_fils-1,ch);
                                        //xml_ecrit_dernier_texte(tmp_node,ch);
                                    }
                                    else
                                    {
                                         //xml_ecrit_dernier_texte(tmp_node,"NA");
                                         xml_ecrit_dernier_texte(tmp_node->fils+tmp_node->nb_fils-1,"NA");
                                    }
                                    for (;COMPETENCE[i].nom!=NULL;i++);
                                    i--;
                                }
                                else
                                {
                                }
                            }
                        }
                        else
                        { /* i fait vdr+1 => la compétence est déjà comptée */
                        }
                    }
                }
            }
            else
            { /* pas de classe */
            }
            supprime_node(tmp_node,1);

         /* voleur */
         for (j=0;j<8;j++)
         {
             sprintf(ch,"%hu",_perso->perso.tab_voleur[j]);
             modif_xml(g_markup_dom_nom (ooo,nom_fac_vol[j],"name"),ch);
             if (_perso->perso.tab_voleur[j]>0  && j!=7) /* on affiche les modificateurs de l'armure si la compétence est connue */
             {
                 if (_perso->perso.armure1>=0) /* modification armure 1 pour toutes les compétences sauf lecture */
                 {
                    sprintf(affichage,"%s_ar1",nom_fac_vol[j]);
                    sprintf(ch,"%s : %hd | ",ARMURE[_perso->perso.armure1].nom,_perso->perso.tab_voleur[j]+ARMURE[_perso->perso.armure1].add[_version][j]);
                    modif_xml(g_markup_dom_nom (ooo,affichage,"name"),ch);
                 }
                 else
                 { /* armure inconnue */
                 }
                 if (_perso->perso.armure2>=0) /* modification armure 2 pour toutes les compétences sauf lecture */
                 {
                    sprintf(affichage,"%s_ar2",nom_fac_vol[j]);
                    sprintf(ch,"%s : %hd",ARMURE[_perso->perso.armure2].nom,_perso->perso.tab_voleur[j]+ARMURE[_perso->perso.armure2].add[_version][j]);
                    modif_xml(g_markup_dom_nom (ooo,affichage,"name"),ch);
                 }
                 else
                 { /* armure inconnue */
                 }
             }
             else
             { /* compétence où l'armure n'intervient pas */
             }
         }
         if (_perso->perso.commentaire!=NULL)
         {
            tmp_node=g_markup_dom_nom (ooo,"dern_sect","name");
            xml_ecrit_dernier_texte_f(g_markup_dom_nom (ooo,"commentaires","name"),_perso->perso.commentaire);
         }



         sprintf(ch,"%s",_perso->perso.nom_fichier);
         i=strlen(ch);
         ch[i-4]='o';
         ch[i-3]='d';
         ch[i-2]='t';
         ch[i-1]=0;
         fichier = g_fopen(ch, "rb");
         if (fichier!=NULL) /* Le fichier existe déjà */
         {
            fclose(fichier);
            for (i=0;ch[i]!=0;i++)
            {
                aff[i]=ch[i];
            }
            aff[i]='~';
            aff[i+1]=0;
            copie_fichier(aff,ch);
         }
         else
         {
             /* le fichier n'exite pas */
         }
         tmp1=chemin_exe();
         nom_de_fichier=NULL;
         fichier=enregistre_tmp_xml(ooo,&nom_de_fichier);
         sprintf(affichage,"%s%cLibO%cfeuille.zip",tmp1,SEPARATEUR,SEPARATEUR);
         copie_fichier(ch,affichage);
         if (nom_de_fichier)
         {
            fclose(fichier);
            fichier=g_fopen(nom_de_fichier,"r");
         }
         else
         { /* le vrai fichier temporaire est actif */
         }
        f_zip=zip_open(ch,ZIP_CREATE,NULL);
        if (f_zip==NULL)
        {
            printf("Erreur lors de l'ouverture de %s : %s\n",ch,zip_strerror(f_zip));
        }
        else
        { /* l'ouverture de xml bien passée */
        }
        n_zip=zip_source_filep(f_zip,fichier,0,0); /* récupération du fichier sous forme FILE * sans avoir à le copier sur le disque */
        if (n_zip==NULL)
        {
            printf("Erreur lors de l'ouverture de content.xml : %s\n",zip_strerror(f_zip));
        }
        else
        { /* l'ouverture de xml bien passée */
        }
        if (zip_file_replace(f_zip,zip_name_locate(f_zip,"content.xml",ZIP_FL_NOCASE) ,n_zip,ZIP_FL_OVERWRITE)!=0) /* on l'injecte dans l'archive en lui donnant un nom */
        {
            printf("Erreur lors de l'insertion de content.xml dans %s : %s\n",ch,zip_strerror(f_zip));
        }
        else
        { /* l'insertion c'est bien faite */
        }
        if (zip_close(f_zip)!=0)   /* on ferme l'archive */
        {
            printf("Erreur lors de la fermeture du fichier %s : %s\n",ch,zip_strerror(f_zip));
        }
        else
        { /* l'enregistrement c'est bien fait */
        }
        if (nom_de_fichier)
        {
            fclose(fichier);
            g_free(nom_de_fichier);
            nom_de_fichier=NULL;
        }
        else
        { /* le vrai fichier temporaire est actif */
        }

        /* échange de ligne pour passer la feuille au format ADD2 */
        echange_node(g_markup_dom_nom (ooo,"ligne_dexterite","name"),g_markup_dom_nom (ooo,"ligne_intelligence","name"));
        echange_node(g_markup_dom_nom (ooo,"ligne_constitution","name"),g_markup_dom_nom (ooo,"ligne_sagesse","name"));
        echange_node(g_markup_dom_nom (ooo,"ligne_petrification","name"),g_markup_dom_nom (ooo,"ligne_baguette","name"));

        strcpy(aff,ch);
        i=strlen(ch)+1;
        ch[i-5]='_';
        ch[i-4]='a';
        ch[i-3]='d';
        ch[i-2]='d';
        ch[i-1]='2';
        ch[i]='.';
        ch[i+1]='o';
        ch[i+2]='d';
        ch[i+3]='t';
        ch[i+4]=0;
        fichier = g_fopen(_perso->perso.nom_fichier, "rb");
        if (fichier!=NULL) /* Le fichier existe déjà */
        {
            fclose(fichier);
            for (i=0;ch[i]!=0;i++)
            {
                affichage[i]=ch[i];
            }
            affichage[i]='~';
            affichage[i+1]=0;
            copie_fichier(affichage,ch);
        }
        else
        { /* le vrai fichier temporaire est actif */
        }
        fichier=enregistre_tmp_xml(ooo,&nom_de_fichier);
        copie_fichier(ch,aff);
        if (nom_de_fichier)
        {
            fclose(fichier);
            fichier=g_fopen(nom_de_fichier,"r");
        }
        else
        { /* le vrai fichier temporaire est actif */
        }
        f_zip=zip_open(ch,ZIP_CREATE,NULL);
        if (f_zip==NULL)
        {
            printf("Erreur lors de l'ouverture de %s : %s\n",ch,zip_strerror(f_zip));
        }
        else
        { /* l'ouverture de xml bien passée */
        }
        n_zip=zip_source_filep(f_zip,fichier,0,0); /* récupération du fichier sous forme FILE * sans avoir à le copier sur le disque */
        if (n_zip==NULL)
        {
            printf("Erreur lors de l'ouverture de content.xml : %s\n",zip_strerror(f_zip));
        }
        else
        { /* l'ouverture de xml bien passée */
        }
        if (zip_file_replace(f_zip,zip_name_locate(f_zip,"content.xml",ZIP_FL_NOCASE) ,n_zip,ZIP_FL_OVERWRITE)!=0) /* on l'injecte dans l'archive en lui donnant un nom */
        {
            printf("Erreur lors de l'insertion de content.xml dans %s : %s\n",ch,zip_strerror(f_zip));
        }
        else
        { /* l'insertion c'est bien faite */
        }
        if (zip_close(f_zip)!=0)   /* on ferme l'archive */
        {
            printf("Erreur lors de la fermeture du fichier %s : %s\n",ch,zip_strerror(f_zip));
        }
        else
        { /* l'enregistrement c'est bien fait */
        }
        if (nom_de_fichier)
        {
            fclose(fichier);
            g_free(nom_de_fichier);
            nom_de_fichier=NULL;
        }
        g_free(tmp1);
    }
    g_markup_dom_free(ooo);
    /* fin enregistement .odt */

    /* début enregistrement .pers */
    fichier = g_fopen(_perso->perso.nom_fichier, "rb");
    if (fichier!=NULL) /* Le fichier existe déjà */
    {
        fclose(fichier);
        sprintf(ch,"%s~",_perso->perso.nom_fichier);
        copie_fichier(ch,_perso->perso.nom_fichier);
    }

    fichier = g_fopen(_perso->perso.nom_fichier, "w");
    if (fichier!=NULL)
    {
        g_fprintf(fichier,"<document type=\"perso\">\n <caracteristique>\n");
        for(i=0;i<8;i++)
        {
            g_fprintf(fichier,"  <c name=\"%s\">%hd</c>\n",nom_min[i],caract[i]);
        }
        if (caract[8]>9)
        {
            g_fprintf(fichier,"  <c name=\"%s\">%hd</c>\n",nom_min[8],caract[8]);
        }
        else
        {
            g_fprintf(fichier,"  <c name=\"%s\">0%hd</c>\n",nom_min[8],caract[8]);
        }
        g_fprintf(fichier," </caracteristique>\n");
        g_fprintf(fichier," <race>%s</race>\n",race->nom);
        g_fprintf(fichier," <sexe>%s</sexe>\n",nom_sexe[sexe]);
        g_fprintf(fichier," <version>%s</version>\n <profession>\n",vers[_version]);
        if (classe!=NULL)
        {
            for (i=0;classe[i]>=0;i++)
            {
                g_fprintf(fichier," <p niv=\"%hd\" xp=\"%lu\">%s</p>\n",niv_cl[i],_perso->perso.XP[i],_classe[classe[i]].nom);
            }
        }
        else
        { /* pas de classes choisie */
        }
        g_fprintf(fichier," </profession>\n");
        g_fprintf(fichier," <armure>\n");
        if (_perso->perso.bague!=NULL)
        {
            g_fprintf(fichier,"   <bague mag='%hd' />\n",_perso->perso.bague[0]);
        }
        else
        { /* pas de bague */
        }
        if (_perso->perso.bouclier!=NULL)
        {
            g_fprintf(fichier,"   <bouclier mag='%hd' />\n",_perso->perso.bouclier[0]);
        }
        else
        { /* pas de bouclier */
        }
        if (_perso->perso.autre_ca!=0)
        {
            g_fprintf(fichier,"   <autre_ca value='%hd' />\n",_perso->perso.autre_ca);
        }
        else
        { /* autre_ca nulle */
        }
        if (_perso->perso.autre_save!=0)
        {
            g_fprintf(fichier,"   <autre_save value='%hd' />\n",_perso->perso.autre_save);
        }
        else
        { /* autre_save nulle */
        }
        g_fprintf(fichier,"  <armure1 mag='%hd' ac='%hd'>%s</armure1>\n  <armure2 mag='%hd' ac='%hd'>%s</armure2>\n </armure>\n",_perso->perso.mag_armure1,ca1,ARMURE[_perso->perso.armure1].nom,_perso->perso.mag_armure2,ca2,ARMURE[_perso->perso.armure2].nom);
        g_fprintf(fichier," <save spmp='%hd' sbb='%hd' spp='%hd' ssor='%hd' ssoo='%hd' />\n",spmp,sbb,spp,ssor,ssou);
        g_fprintf(fichier," <armes>\n");
        if (arme!=NULL)
        {
            for (i=0;arme[i]>=0;i++)
            {
                g_fprintf(fichier,"  <arme niv='%hd' num='%hd' taco='%hd' degat='%hu' mag='%hd' autre_t='%hd' autre_d='%hd'>%s</arme>\n",_perso->perso.niv_arme[i],arme[i],taco_a[i],degat_a[i],_perso->perso.mag_arme[i],_perso->perso.arme_autre_t[i],_perso->perso.arme_autre_d[i],ARME[arme[i]].nom);
            }
        }
        else
        { /* pas d'armes */
        }
        g_fprintf(fichier," </armes>\n <pdv>\n");
        if (_perso->perso.pdv!=NULL)
        {
            for (i=0;_perso->perso.pdv[i]>0;i++)
            {
                g_fprintf(fichier,"  <pdv_n niv='%hd'>%hd</pdv_n>\n",i+1,_perso->perso.pdv[i]);
            }
        }
        else
        { /* pas de pdv */
        }
        g_fprintf(fichier," </pdv>\n <competence>\n");
        if (competence!=NULL)
        {
            for(i=0;competence[i]>=0;i++)
            {
                g_fprintf(fichier,"  <comp niv='%hd'>%s</comp>\n",_perso->perso.niv_competence[i],COMPETENCE[competence[i]].nom);
            }
        }
        else
        { /* pas de compétence */
        }
        g_fprintf(fichier," </competence>\n <voleur>\n");
        for (i=0;i<FEUILLE_LAR_VOL;i++)
        {
            g_fprintf(fichier,"  <vol name='%s'>%hd</vol>\n",nom_fac_vol[i],_perso->perso.tab_voleur[i]);
        }
        g_fprintf(fichier," </voleur>\n <pantheon>\n  <dieu>  ");
        ecrit_ch(fichier,_perso->perso.dieu);
        g_fprintf(fichier,"</dieu>\n ");
        ecrit_ch(fichier,_perso->perso.pantheon);
        g_fprintf(fichier,"\n </pantheon>\n <com>\n");
        ecrit_ch(fichier,_perso->perso.commentaire);
        g_fprintf(fichier,"\n </com>\n");

         if (_perso->perso.livre_sorts!=NULL)
         {
            g_fprintf(fichier," <mag>\n");
            for(i=0;i<9;i++)
            {
                if (_perso->perso.livre_sorts[i]!=NULL)
                {
                    g_fprintf(fichier,"  <niv niv='%hd'>\n",i+1);
                    for (k=0;_perso->perso.sort_su[i][k]!=0;k++)
                    {
                        for (j=0;j<NB_SORT_MAG[ADD2][i];j++)
                        {
                            if (SORTILEGE_MAG[ADD2][i][j].clef==_perso->perso.livre_sorts[i][k])
                            { /* on a trouvé le bon sort */
                                g_fprintf(fichier,"  <sort niv=\"%hd\" clef=\"%ld\" voyage=\"%hd\">",abs(_perso->perso.sort_su[i][k]),_perso->perso.livre_sorts[i][k],_perso->perso.sort_su[i][k]>0?1:0);
                                ecrit_ch(fichier,SORTILEGE_MAG[ADD2][i][j].nom);
                                g_fprintf(fichier,"</sort>\n");
                                j=NB_SORT_MAG[ADD2][i]; /* on sort du fort */
                            }
                            else
                            { /* on n'a pas encore trouvé le bon */
                            }
                        }
                    }
                    g_fprintf(fichier,"  </niv>\n");
                }
                else
                { /* pas de livres de sorts de niveau i */
                }
            }
            g_fprintf(fichier," </mag>\n");
         }
         else
         { /* pas de livre de sorts */
         }
         g_fprintf(fichier," <taille>%hd</taille>\n",_perso->perso.taille);
         g_fprintf(fichier," <poids>%hd</poids>\n",_perso->perso.poids);
         g_fprintf(fichier," <nom>\n  <perso>");
         ecrit_ch(fichier,_perso->perso.nom);
         g_fprintf(fichier,"  </perso>\n  <joueur>");
         ecrit_ch(fichier,_perso->perso.joueur);
         g_fprintf(fichier,"  </joueur>\n </nom>\n <couleur>\n  <cheveux>");
         ecrit_ch(fichier,_perso->perso.cheveux);
         g_fprintf(fichier,"</cheveux>\n  <peau>");
         ecrit_ch(fichier,_perso->perso.peau);
         g_fprintf(fichier,"</peau>\n  <yeux>");
         ecrit_ch(fichier,_perso->perso.yeux);
         g_fprintf(fichier,"</yeux>\n </couleur>\n <classoc>");
         ecrit_ch(fichier,_perso->perso.sociale);
         g_fprintf(fichier,"</classoc>\n <age>%hd ans</age>\n",_perso->perso.age);
         g_fprintf(fichier," <alignement>%s</alignement>\n",algnm[_perso->perso.alignement]);
         if (_perso->perso.psi!=NULL)
         {
             g_fprintf(fichier," <psi>\n");
             for (i=0;_perso->perso.psi[i]>0;i++)
             {
                 g_fprintf(fichier,"  <talent clef='%hd' niv='%hd' natif='%hd'>",_perso->perso.psi[i],_perso->perso.niv_psi[i],_perso->perso.natif_psi[i]);
                 for (j=0;j<NB_DISCIPLINE_PSI;j++)
                 {
                     for (k=0;k<2;k++)
                     {
                         for(l=NB_TALENTS_PSI[j][k]-1;l>=0;l--)
                         {
                             if (TALENTS_PSI[j][k][l].clef==_perso->perso.psi[i])
                             {
                                 ecrit_ch(fichier,TALENTS_PSI[j][k][l].nom);
                                 l=0;
                                 k=2; /* onaccélère la sortie */
                                 j=NB_DISCIPLINE_PSI;
                             }
                             else
                             { /* ce n'est pas le bon */
                             }
                         }
                     }
                 }
                 g_fprintf(fichier,"</talent>\n");
             }
             g_fprintf(fichier," </psi>\n");
         }
         else
         { /* perso sans pouvoir psi */
         }
         g_fprintf(fichier," <cap>%c</cap>\n",_perso->perso.cap);
         g_fprintf(fichier," <origines>\n  <version>%hd</version>\n",_perso->perso.ori_add1_add2);
         for(i=0;i<2;i++)
         {
             g_fprintf(fichier,"  <add%hd>\n",i+1);
             for(j=0;j<NB_ORIGINE[i];j++)
             {
                 if (_perso->perso.origine[i][j])
                 {
                    g_fprintf(fichier,"   <ori>%s</ori>\n",_ORIGINE[i][j].court);
                 }
                 else
                 {
                     /* origine non prise en compte */
                 }
             }
             g_fprintf(fichier,"  </add%hd>\n",i+1);
         }
         g_fprintf(fichier," </origines>\n");
         g_fprintf(fichier,"</document>\n");
         g_fclose(fichier);
        _perso->d_r=0;
     }
     else
     {
            sprintf(ch,"erreur lors de la sauvegarde du fichier ");
            strncat(ch , _perso->perso.nom_fichier,LONG-1);
            dialogue(ch,0);
     }
     g_free(taco_a);
     g_free(degat_a);
     /* FIN DE LA SAUVEGARDE DU FICHIER .PERS */
    }
    else
    {
        enregistre_perso_sous(NULL,_perso);
    }


    (void)ChildWidget;
}
