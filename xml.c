/******************************************************************************
 *    Copyright (C) 2006 by JOSEPH Nicolas                                    *
 *    gege2061@redaction-developpez.com                                       *
 *    Copyright (C) 2010-2013 by SIAUD Bernard                                *
 *    troumad@siaud.org                                                       *
 *                                                                            *
 *    This program is free software; you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation; either version 2 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program; if not, write to the                           *
 *    Free Software Foundation, Inc.,                                         *
 *    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 ******************************************************************************/

/* http://troumad.developpez.com/C/gtk/gtk_xml/ pour plus d'explication sur ce code */

#include "include.h"

#include <ctype.h>

#include <glib.h>
#include <string.h>
#include <stdio.h>


/* laisser pour sauter des lignes et commenter pour ne pas en sauter */
/* #define PRESENTATION */



void xml_start_element (GMarkupParseContext *context, const gchar *element_name,
                        const gchar **attribute_names,
                        const gchar **attribute_values, gpointer user_data,
                        GError **error);
void xml_end_element (GMarkupParseContext *context,
                             const gchar *element_name, gpointer user_data,
                             GError **error);
void xml_com (GMarkupParseContext *context, const gchar *text,gsize text_len, gpointer user_data, GError **error);
void xml_text (GMarkupParseContext *context, const gchar *text,gsize text_len, gpointer user_data, GError **error);

/*void xml_test (GMarkupParseContext *context, const gchar *text,gsize text_len, gpointer user_data, GError **error);
void xml_start_test (GMarkupParseContext *context, const gchar *element_name,
                        const gchar **attribute_names,
                        const gchar **attribute_values, gpointer user_data,
                        GError **error);
void xml_end_test (GMarkupParseContext *context,
                             const gchar *element_name, gpointer user_data,
                             GError **error);


void xml_test (GMarkupParseContext *context, const gchar *text,gsize text_len, gpointer user_data, GError **error)
{
    printf("test : %s\n",text);
}

void xml_start_test (GMarkupParseContext *context, const gchar *element_name,
                        const gchar **attribute_names,
                        const gchar **attribute_values, gpointer user_data,
                        GError **error)
{
    printf("start : %s\n",element_name);
}

void xml_end_test (GMarkupParseContext *context,
                             const gchar *element_name, gpointer user_data,
                             GError **error)
{
    printf("end : %s\n",element_name);
}*/


/* macros =================================================================== */
/* constants ================================================================ */


/* private variables ======================================================== */
/* private functions ======================================================== */
/*gchar * mintexte()(gchar * s)
{
  gchar * c;

  for (; *s && (*s==' ' || *s=='\t' || *s=='\n'); s++);
  c=s;
  while(*c)
   c++;
  c--;
  while (*c==' ' || *c=='\t' || *c=='\n')
   c--;
  *(c+1)=0;

  return s;

}*/
  //return g_strstrip(s);




void dom_print (GMarkupDomNode *root,FILE * sortie,gint pos)
{
#define INDENT(niv,sortie)          \
  do                                \
  {                                 \
    guint ii = niv-1;               \
                                    \
    for (; ii > 0; ii--)            \
    {                               \
      g_fprintf(sortie,"\t");         \
    }                               \
  } while (0)

  guint texte=0;
  guint fils=0;
  guint com=0;
  guint i;

  if (root != NULL)
  {

    if (root->niveau!=0)
    {
      i = 0;
#ifdef PRESENTATION
      INDENT (root->niveau,sortie);
#endif
      g_fprintf(sortie,"<%s", root->nom);

      while (root->nb_att>i)
      {
        if (root->attributs[i].nom!=NULL && root->attributs[i].value!=NULL)
        g_fprintf(sortie," %s=\"%s\"", root->attributs[i].nom,
                               root->attributs[i].value);
        i++;
      }
    }
    if (root->nb_fils+root->nb_texte+root->nb_com!=0)
    {
      if (root->niveau!=0)
#ifdef PRESENTATION
g_fprintf(sortie,">\n");
#else
g_fprintf(sortie,">");
#endif
      while (texte<root->nb_texte || fils<root->nb_fils || com<root->nb_com)
      {
        if (root->nb_com>com && root->com[com].item==(texte+fils+com))
        {
#ifdef PRESENTATION
            INDENT (root->niveau + 1,sortie);
#endif
            g_fprintf(sortie,"%s\n", root->com[com].texte);
            //g_print("%s\n", root->com[com].texte);
            com++;
        }
        if (root->nb_texte>texte && root->texte[texte].item==(texte+fils+com))
        {
#ifdef PRESENTATION
            INDENT (root->niveau + 1,sortie);
#endif
            ecrit_xml(sortie,root->texte[texte].texte);
            //fputc('\n',sortie);
            texte++;
        }
        if (root->nb_fils>fils && root->fils[fils].item==(fils+texte+com))
        {
            dom_print (root->fils+fils,sortie,pos+1);
            fils++;
        }
      }
      if (root->niveau)
      {
#ifdef PRESENTATION
       INDENT (root->niveau,sortie);
       g_fprintf(sortie,"</%s>\n", root->nom);
#else
       g_fprintf(sortie,"</%s>", root->nom);
#endif
      }
    }
    else
      if (root->niveau)
      {
#ifdef PRESENTATION
        g_fprintf(sortie,"/>\n");
#else
        g_fprintf(sortie,"/>");
#endif
      }
  }

  #undef INDENT
}

void xml_start_element (GMarkupParseContext *context, const gchar *element_name,
                        const gchar **attribute_names,
                        const gchar **attribute_values, gpointer user_data,
                        GError **error)
{
  guint i,j;
  GMarkupDomContext *dom_context = user_data;
  GMarkupDomNode *node = NULL, * parent=NULL;

  g_return_if_fail (dom_context != NULL); /* déclaration et initialisation du nouveau élément  */
 /* déclaration et initialisation du nouveau élément  */
  if (dom_context->root) /* si ce n'est pas la première node, la node mère du système */
  {
      parent=dom_context->current;
      //for (parent=dom_context->current;dom_context->niveau<parent->niveau;parent=parent->parent);
      parent->nb_fils++;                 /* un for car il est possible qu'on vienne de descendre de niveaux */
      parent->fils=g_realloc(parent->fils,sizeof (*node)*parent->nb_fils); /* on rajoute un fils au père     */
      for (i=0;i<parent->nb_fils-1;i++)  /* ne pas oublié que tous les fils ont changé d'adresse            */
        for (j=0;j<parent->fils[i].nb_fils;j++)
            parent->fils[i].fils[j].parent=parent->fils+i;
      node=parent->fils+parent->nb_fils-1; /* on récupère le lien vers la nouvelle node crée chez le père  */
      node->parent=parent;            /* j'informe sur le parent de cette node                           */
      node->item=dom_context->item;   /* j'informe sur l'ordre de cet élément dans l'ensemble des        */
  }                                   /* éléments (texte, node et commntaires) du père                   */
  else /* si c'est le premier élément */
  {
    node = g_malloc (sizeof (GMarkupDomNode));
    dom_context->root = node;
    node->item=0;
    node->parent=NULL;
  }
  dom_context->current = node;

  /* variable générale de l'état actuel du système  */
  /* la construction d'un nouveau élément est toujours demandé par le père donc on */
  dom_context->niveau++;         /* vient de monter le niveau de 1 . mise dans  la */
  dom_context->item=0;           /* pas encore d'entrée dans cette node            */


  /* initialisation de la node */
  node->nom = g_strdup (element_name);
  node->texte = NULL;
  node->nb_texte   = 0;     /* pas encore de texte        */
  node->com = NULL;
  node->nb_com  = 0;     /* pas encore de commentaires */
  node->fils = NULL;
  node->nb_fils=0;          /* pas encore de fils         */
  node->niveau = dom_context->niveau; /* niveau du noeud */

  /* Copy attributs */
  for (i = 0; attribute_names[i] != NULL; i++); /* on compte le nombre d'attributs */
  if (i!=0 )
  {
      node->attributs = g_malloc (sizeof (xml_attr)*i);
  }
  else
  {
      node->attributs = NULL;
  }

  node->nb_att=i;
  for (i = 0; attribute_names[i] != NULL; i++) /* on parcourt les attributs        */
  {                                         /* on informe leur nom et valeur       */
    node->attributs[i].nom = g_strdup (attribute_names[i]);
    node->attributs[i].value = g_strdup (attribute_values[i]);
  }

  /* Unused parameters */
  (void)context;
  (void)error;
}

void xml_end_element (GMarkupParseContext *context,
                             const gchar *element_name, gpointer user_data,
                             GError **error)
{
  GMarkupDomContext *dom_context = user_data;

  g_return_if_fail (dom_context != NULL);
  g_return_if_fail (dom_context->current != NULL);

  /* je rends la main au père */
  dom_context->item=dom_context->current->item+1;        /* je passe à l'élément suivant du père */
  dom_context->niveau--;                                 /* le père a un niveau de moins         */
  dom_context->current   = dom_context->current->parent; /* la node courante se ra celle du père */

  /* Unused parameters */
  (void)context;
  (void)element_name;
  (void)error;
}

void xml_text (GMarkupParseContext *context, const gchar *text,gsize text_len, gpointer user_data, GError **error)
{

  GMarkupDomContext *dom_context = user_data;
  gchar * ch=g_strdup (text);

  g_return_if_fail (dom_context != NULL);
  g_return_if_fail (dom_context->current != NULL);

  if (!str_isspace (text))/* si le texte n'est pas vide (saut de ligne ou espace ou tabulation */
  {                       /* on rajoute cet élément (voir rajout d'une node : c'est pareil)    */
    dom_context->current->nb_texte++;
    if (dom_context->current->nb_texte==1)
    {
        dom_context->current->texte=g_malloc(dom_context->current->nb_texte*sizeof(contenu));
    }
    else
    {
        dom_context->current->texte=g_realloc(dom_context->current->texte,dom_context->current->nb_texte*sizeof(contenu));
    }
    dom_context->current->texte[dom_context->current->nb_texte-1].item=dom_context->item;
    dom_context->item++;
    dom_context->current->texte[dom_context->current->nb_texte-1].texte = g_strdup (g_strstrip(ch)); /* enlève les espaces avant et après */
  }

  g_free(ch);
  /* Unused parameters */
  (void)context;
  (void)text_len;
  (void)error;
}

void xml_com (GMarkupParseContext *context, const gchar *text,gsize text_len, gpointer user_data, GError **error)
{
  GMarkupDomContext *dom_context = user_data;
  g_return_if_fail (dom_context != NULL);
  g_return_if_fail (dom_context->current != NULL);

  if (!str_isspace (text)) /* si le texte n'est pas vide (saut de ligne ou espace ou tabulation) */
  {                        /* on rajoute cet élément (voir rajout d'une node : c'est pareil)     */
    dom_context->current->nb_com++;
    dom_context->current->com=g_realloc(dom_context->current->com,dom_context->current->nb_com*sizeof(contenu));
    dom_context->current->com[dom_context->current->nb_com-1].item=dom_context->item;
    dom_context->item++;
    dom_context->current->com[dom_context->current->nb_com-1].texte = g_strdup (text);
  }
  else
  {
      /* printf("xml_com : Texte vide\n"); */
  }

  /* Unused parameters */
  (void)context;
  (void)text_len;
  (void)error;
}



/**
 * g_markup_dom_free:
 * @node: a #GMarkupDomNode.
 *
 * Frees a #GMarkupDomNode.
 **/
void g_markup_dom_free (GMarkupDomNode *node)
{
  if (node != NULL)
  {
    guint i;

    g_free (node->nom), node->nom = NULL;
    for (i = 0; node->nb_att>i; i++)
    {
      g_free (node->attributs[i].nom), node->attributs[i].nom = NULL;
      g_free (node->attributs[i].value), node->attributs[i].value = NULL;
    }
    if (node->nb_att>0) g_free (node->attributs), node->attributs = NULL;

    for (i = 0; node->nb_texte>i; i++)
    {
      g_free (node->texte[i].texte);
    }
    if (node->nb_texte>0) g_free(node->texte),node->texte=NULL;

    for (i = 0; node->nb_com>i; i++)
    {
      g_free (node->com[i].texte);
    }
    if (node->nb_com>0) g_free(node->com),node->com=NULL;

    for (i = 0; node->nb_fils>i; i++)
    {
      g_markup_dom_free (node->fils+i);
    }
    if (node->nb_fils>0) g_free(node->fils), node->fils = NULL;

    if (node->niveau==0) g_free(node), node = NULL; /* il faut juste libérer la racine, les autres, sont dans des tableaux */
   }
}

GMarkupDomNode * g_markup_dom_nom (GMarkupDomNode *node,const gchar * nom_val,gchar * attr)
{
  GMarkupDomNode * tmp;

  if (node != NULL)                     /* la node existe t'elle vraiment ?                            */
  {
    guint i;

    for (i = 0; node->nb_att>i; i++)    /* parcourt les attributs de cette node                         */
    {                                   /* à la recherche de l'attribut attr dont la valeur est nom_val */
      if (strcmp((node->attributs[i].value),nom_val)==0 && strcmp(node->attributs[i].nom,attr)==0)
       return node;                     /* si oui renvoie le pointeur sur la node                       */
    }                                   /* si le couple attribut/valeur n'a pas été trouvé              */
    for (i = 0; node->nb_fils>i; i++)   /* parcourt des nodes filles                                    */
    {                                   /* c'est une procédure récursive                                */
      if ((tmp=g_markup_dom_nom (node->fils+i,nom_val,attr)))  /* si le couple a été trouvé             */
       return tmp;                      /* renvoie de la node qui contient ce couple                    */
    }

  }
  return NULL;                          /* si rien n'a été trouvé dans la node ou parmi  ces enfants,   */
}

GMarkupDomNode * g_markup_dom_node (GMarkupDomNode *node,const gchar * nom_val)
{
  GMarkupDomNode * tmp=NULL;

  if (node != NULL)                     /* marche comme g_markup_dom_nom, mais plus simplement          */
  {
    guint i;

    if (node->nom && strcmp(node->nom,nom_val)==0)
       return node;
    for (i = 0; node->nb_fils>i; i++)
    {
      if ((tmp=g_markup_dom_node (node->fils+i,nom_val)))
       return tmp;
    }

  }
  return NULL;
}

/* public variables ========================================================= */
GMarkupDomNode *g_markup_dom_new (const gchar *filename, GError **error)
{
                  /* sort de la fonction si filename ne pointe pas sur une chaîne de caractères */
  if (filename != NULL)
  {
   GMarkupParser markup_parser; /* http://library.gnome.org/devel/glib/stable/glib-Simple-XML-Subset-Parser.html#GMarkupParser */
   GMarkupParseContext *markup_parse_context=NULL;
   GMarkupDomNode * pere= (GMarkupDomNode *)g_malloc(sizeof(GMarkupDomNode));
   /* pere doit être alloué dynamiquement car il est utilisé par la fonction appelante */
   /* ce n'est pas le cas de context qui n'est utilisé que les fonctions appelées  */
   GMarkupDomContext * context=(GMarkupDomContext *)g_malloc(sizeof(GMarkupDomContext));

   pere->nom=NULL; /* initialisation de la racine du fichier xml */
   pere->niveau=0;
   pere->item=0;
   pere->texte=NULL;
   pere->nb_texte=0;
   pere->attributs=NULL;
   pere->nb_att=0;
   pere->com=NULL;
   pere->nb_com=0;
   pere->parent=NULL;
   pere->fils=NULL;
   pere->nb_fils=0;

   context->item=0;
   context->niveau=0;
   context->current=pere;
   context->root=pere;

           /* création du contexte : fonctions à appeler suivant la nature de l'élément traité */

    markup_parser.start_element = xml_start_element; /* cas : ouverture de balise               */
    markup_parser.end_element = xml_end_element;     /* cas : fermeture de balise               */
    markup_parser.text = xml_text;                   /* cas : texte entre les balises           */
    markup_parser.passthrough = xml_com;             /* cas : commentaires entre les balises    */
    markup_parser.error = NULL;                      /* cas : erreur dans le fichier xml        */

    markup_parse_context = g_markup_parse_context_new (&markup_parser, 0,
                                                       context, NULL);
    /* http://library.gnome.org/devel/glib/stable/glib-Simple-XML-Subset-Parser.html#g-markup-parse-context-new */
    /* maintenant, on sait comment parcourir le fichier */
   { /* ouverture du fichier */
     gchar *text = NULL,*ch=NULL,*point=NULL;
     gsize length = -1;

     /* recherche de fichier libreoffice ou openoffice : touche personelle */
     for (ch=(gchar *)filename;*ch!=0;ch++)
     {
         if (*ch==SEPARATEUR || *ch=='\\') point=NULL;
         else if (*ch=='.') point=ch;
     }

     /* les fichiers repérés sont des .odt, .odc, .odp ou des .ods */
     if (ch-point==4 && strncmp(point,".od",3)==0 && (point[3]=='t' || point[3]=='c' || point[3]=='p' || point[3]=='s') )
     { /* ouverture avec décompression */
        text=content_libo((char *)filename,&length);
     }
     else
     {
        if ( !g_file_get_contents (filename, &text, &length, error))
        {
            g_free(pere);
            pere=NULL;
        }
        else
        {  /* bien passé */
        }
     }

     /* passe tout le fichier filename dans la chaîne de caractères text dont la longueur sera dans lenght */
     if (pere!=NULL && text != NULL) /* si on a récupéré le fichier */
     {
        g_markup_parse_context_parse (markup_parse_context, text, length, error);  /* on lance le parsage du fichier xml */
        g_free (text), text = NULL;
     }

     g_free (markup_parse_context), markup_parse_context = NULL;
   }

    g_free(context);
  }
  else
  {
     pere=NULL;
  }
    return pere;
}


void enregistre_xml(char * nom_fic, GMarkupDomNode * ooo)
{
 FILE * sortie=g_fopen(nom_fic,"w");

 dom_print(ooo,sortie,0);

 fclose(sortie);
}

FILE * enregistre_tmp_xml(GMarkupDomNode * ooo, char ** nom_)
{

#ifdef __linux__
  FILE * sortie=tmpfile();
#else
  FILE * sortie=NULL;
#endif

 if (sortie==NULL)
 {
    char ch[LONG];
    char * s =  envp_app_data();
    sprintf(ch,"%s%ccontent.xml",s,SEPARATEUR);
    *nom_=g_strdup(ch);
    g_free(s);

    sortie = g_fopen(ch,"w");

    if (sortie==NULL)
    {
        printf("Erreur d'ouverture de tmpfile : fichier temporaire : %s\n",ch);
    }
 }
 else
 {
 }
 dom_print(ooo,sortie,0);

 fseek(sortie,0,SEEK_SET);
 return sortie;
}

void copie_node(GMarkupDomNode * arrive,GMarkupDomNode * modele)
{
  guint i;

printf("Passage par copie_node pour %s\n",modele->nom);
  if (modele != NULL && arrive!=NULL) /* les deux nodes existent */
  {
    if (arrive->nb_att>0)
    { /* on efface ce qui est déjà dans arrive */
        for (i = 0; arrive->nb_att>i; i++)
        {
            g_free(arrive->attributs[i].value);
            g_free(arrive->attributs[i].nom);
        }
        g_free(arrive->attributs),arrive->attributs=NULL;
    }
    arrive->nb_att=modele->nb_att;
    if (arrive->nb_att>0) /* on place dans arrive ce qui est dans modele */
    {
         arrive->attributs=g_malloc(modele->nb_att*sizeof(xml_attr));
    }
    else
    {
         arrive->attributs=NULL;
    }
    for (i = 0; modele->nb_att>i; i++)
    {
        arrive->attributs[i].nom=g_strdup( modele->attributs[i].nom);
        arrive->attributs[i].value=g_strdup( modele->attributs[i].value);
    }

    if (arrive->nb_texte>0)
    { /* on efface ce qui est déjà dans arrive */
        for (i = 0; arrive->nb_texte>i; i++)
        {
            g_free(arrive->texte[i].texte);
        }
        g_free(arrive->texte),arrive->texte=NULL;
    }
    arrive->nb_texte=modele->nb_texte;
    if (arrive->nb_texte>0)
    {
         arrive->texte=(contenu *)g_malloc(modele->nb_texte*sizeof(contenu));
    }
    else
    {
         arrive->texte=NULL;
    }
    for (i = 0; modele->nb_texte>i; i++)
    {
        arrive->texte[i].item=modele->texte[i].item;
        arrive->texte[i].texte=g_strdup(modele->texte[i].texte);
    }

    if (arrive->nb_com>0)
    { /* on efface ce qui est déjà dans arrive */
        for (i = 0; arrive->nb_com>i; i++)
        {
            g_free(arrive->com[i].texte);
        }
        g_free(arrive->com),arrive->com=NULL;
    }
    arrive->nb_com=modele->nb_com;
    if (arrive->nb_com>0)
    {
        arrive->com=(contenu *)g_malloc(modele->nb_com*sizeof(contenu));
    }
    else
    {
        arrive->com=NULL;
    }
    for (i = 0; modele->nb_com>i; i++)
    {
        arrive->com[i].item=modele->com[i].item;
        arrive->com[i].texte=g_strdup(modele->com[i].texte);
    }

    for (i = 0; arrive->nb_fils>i; i++)
    {  /* on efface ce qui est déjà dans arrive */
        g_markup_dom_free(arrive->fils+i);
    }
    arrive->nb_fils=modele->nb_fils;
    if (arrive->nb_fils>0)
    {
        arrive->fils=(GMarkupDomNode *)g_malloc(modele->nb_fils*sizeof(GMarkupDomNode));
    }
    else
    {
         arrive->fils=NULL;
    }
    for (i = 0; modele->nb_fils>i; i++)
    { /* il faut créer un à un tout les nouveaux fils de arrive */
      arrive->fils[i].nb_fils=0;
      arrive->fils[i].nb_att=0;
      arrive->fils[i].nb_com=0;
      arrive->fils[i].nb_texte=0;
      arrive->fils[i].parent=arrive;
      arrive->fils[i].niveau=arrive->niveau+1;
      arrive->fils[i].item=modele->fils[i].item;
      arrive->fils[i].nom=NULL;
      copie_node(arrive->fils+i, modele->fils+i); /* pour y copier les fils de modele */
    }

    if (arrive->nom!=NULL) g_free(arrive->nom);
    arrive->nom=g_strdup(modele->nom);
  }
}

void modif_xml(GMarkupDomNode *item,const gchar * ch) /* remplacer le texte en fin de node */
{
  if (item)
  {
      if (item->nb_texte==0)            /* s'il n'y a pas de texte, rajouter un texte à la fin de la node */
      {
       item->texte=(contenu *)g_malloc(sizeof(contenu));
       item->nb_texte=1;
       item->texte[0].item=item->nb_com+item->nb_fils;
      }
      else                              /* sinon, enlever le texte déjà présent                           */
      {
        g_free(item->texte[0].texte);
      }
      item->texte[0].texte=g_strdup(ch);/* mettre le nouveau texte à la place                             */
  }
}

GMarkupDomNode * xml_ajoute_fin(GMarkupDomNode * node,const gchar * texte)
/* mettre une node fille en fin d'une node, le node de la node étant indiqué par le texte */
{
    guint i,j;

    if (node==NULL)
    {
        printf("Erreur xml_ajoute_fin, ajout de %s impossible ; node NULL\n",texte);
        return NULL;
    }
    else
    {
        node->fils=(GMarkupDomNode *)g_realloc(node->fils,(1+node->nb_fils)*sizeof(GMarkupDomNode));
        node->fils[node->nb_fils].item=node->nb_texte+node->nb_com+node->nb_fils;
        /* place de la nouvelle node dans l'ensemble des enfants de la node mère    */
        node->fils[node->nb_fils].nom=g_strdup(texte);
        /* nom de la nouvelle node                                                  */
        node->fils[node->nb_fils].niveau=node->niveau+1;
        /* noveau de la nouvelle node                                               */
        node->fils[node->nb_fils].texte=NULL;
        /* noveau de la nouvelle node                                               */
        node->fils[node->nb_fils].nb_texte=0;
        node->fils[node->nb_fils].attributs=NULL;
        node->fils[node->nb_fils].nb_att=0;
        node->fils[node->nb_fils].com=NULL;
        node->fils[node->nb_fils].nb_com=0;
        node->fils[node->nb_fils].parent=node;
        node->fils[node->nb_fils].fils=NULL;
        node->fils[node->nb_fils].nb_fils=0;
        for (i=0;i<node->nb_fils;i++)
        {   /* mettre à jour les fils du fils à cause du realloc                    */
            for(j=0;j<node->fils[i].nb_fils;j++)
                node->fils[i].fils[j].parent=node->fils+i;
        }
        node->nb_fils++;
        /* la node mère a un enfant de plus                                         */

        return node->fils+node->nb_fils-1;
    }
}

void xml_ajoute_fin_texte(GMarkupDomNode * node) /* rajouter un élément texte en fin d'une node */
{
    if (node==NULL)
    {
            printf("xml_ajoute_fin_texte erreur : ajout d'une case texte impossible car node=NULL");
    }
    else
    {
        node->texte=(contenu *)g_realloc(node->texte,(1+node->nb_texte)*sizeof(contenu));
        node->texte[node->nb_texte].item=node->nb_texte+node->nb_com+node->nb_fils; /* le 1er est 0 */
        node->texte[node->nb_texte].texte=NULL;      /* le nouveau texte sera vide                  */
        node->nb_texte++;                            /* on incrémente le nombre de texte            */
    }
}

void xml_ecrit_dernier_texte(GMarkupDomNode * node,const char *ch)
{                                                 /* remplace le dernier texte d'une node       */
    if (node!=NULL)
    {
        if (node->nb_texte==0)                        /* vérifier s'il y a déjà un texte            */
        {
          xml_ajoute_fin_texte(node);                  /* pour faire la place si besoin              */
        }
        else if (node->texte[node->nb_texte-1].texte!=NULL)
        {
          g_free(node->texte[node->nb_texte-1].texte); /* ou libérer la place si elle est déjà prise */
        }
        else
        {
        }
        node->texte[node->nb_texte-1].texte=g_strdup(ch); /* copie du texte                         */
    }
    else
    {
        printf("Impossible de rajouter le texte %s dans la node : elle est NULL.\nVoir fonction xml_ecrit_dernier_texte du fichier xml.c\n",ch);
    }
}

void xml_ajoute_fin_attribut(GMarkupDomNode * node) /* rajouter un élément attribut */
{
    node->attributs=(xml_attr *)g_realloc(node->attributs,(1+node->nb_att)*sizeof(xml_attr));
    node->attributs[node->nb_att].nom=g_strdup("troumad");
    node->attributs[node->nb_att].value=g_strdup("1");
    node->nb_att++;
}

unsigned char xml_ecrit_dernier_attribut(GMarkupDomNode * node,char * nomm, char * val) /* remplace le dernier attribut : renvoie le nb d'attributs modifié */
{
    if (node==NULL || nomm==NULL || val==NULL || node->nb_att<1) return 0; /* protection : au moins une valeur incohérente */
    if (node->attributs[node->nb_att-1].nom!=NULL) g_free(node->attributs[node->nb_att-1].nom);     /* libère la place pouvant être occupée par le précédent nom                    */
    if (node->attributs[node->nb_att-1].value!=NULL) g_free(node->attributs[node->nb_att-1].value);   /* libère la place pouvant être occupée par la valeur de l'attribut précédent   */
    node->attributs[node->nb_att-1].nom=g_strdup(nomm);
    node->attributs[node->nb_att-1].value=g_strdup(val);
    return 1;
}

unsigned char xml_ecrit_attribut(GMarkupDomNode * node,char * nomm, char * val) /* remplace la valeur de l'attribut nomm par val */
{
    unsigned short i;
    unsigned short retour=0;

    if (node==NULL|| nomm==NULL || val==NULL)
    {
        retour=2;
    }
    else
    {
        for (i = 0; node->nb_att>i; i++)    /* parcourt les attributs de cette node                         */
        {                                   /* à la recherche de l'attribut attr dont la valeur est nom_val */
          if (strcmp(node->attributs[i].nom,nomm)==0)
          {
              if (node->attributs[i].value!=NULL) g_free(node->attributs[i].value);     /* libère la place pouvant être occupée */
              node->attributs[i].value=g_strdup(val);
              i=node->nb_att; /* forcer la sortie */
              retour=1;
          }
          else
          {
              /* ce n'est pas l'attribut recherché */
          }
        }
    }

  return retour; /* 0 attribut non trouvé, 1 OK, 2 node nulle */
}



void decale_node(GMarkupDomNode *node,unsigned short i)    /* decale les node pour mettre la dernière node à la place i */
{                                                          /* ATTENTION : laisse les textes inchangés */
    gint tmp,tmp1;
    signed short j;                                        /* signed short : pour passer négatif si i=0 */
    GMarkupDomNode tmp_node;


    tmp=node->fils[node->nb_fils-1].item;                  /* place actuelle dans la hiérarchie de la dernière node        */
    tmp_node=node->fils[node->nb_fils-1];                  /* place actuelle dans la hiérarchie de la dernière node        */

    for(j=node->nb_fils-2;j>=i;j--)                        /* parcourt des node de la future avant dernière à la première  */
    {                                                      /* à être déplacée                                              */
       tmp1=node->fils[j].item;                            /* On mémorise la place de la node actuelle                     */
       node->fils[j].item=tmp;                             /* On mémorise la place de la node actuelle                     */
       tmp=tmp1;                                           /* on mémorise l'ancienne place de la node actuelle             */
       node->fils[j+1]=node->fils[j];                      /* On modifie le tableau des nodes filles                       */
    }

    node->fils[i]=tmp_node;                                /* la dernière node prend la place de la node i                 */
    node->fils[i].item=tmp;                                /* numéro de la node dans la liste des enfants des enfants de   */
}                                                          /*                                                la node mère  */


void supprime_node(GMarkupDomNode * pere,guint i)          /* supprime la ième node                                        */
{                                                          /* Attention : non testé avec les textes et les commentaires    */
    unsigned short j;
    guint tmp=pere->fils[i].item;                          /* tmp est l'indice de la node laissée vide                     */

    g_markup_dom_free(pere->fils+i);                       /* libère la placé réservée pour la node                        */
    pere->nb_fils--;                                       /* la node mère aura un enfant de moins                         */
    for(j=i;j<pere->nb_fils;j++)
    {
        pere->fils[j]=pere->fils[j+1];                     /* rappratrie le noeud suivant                                  */
        pere->fils[j].item--;                              /* décrémenter sa place                                         */
    }
    for (i=0;i<pere->nb_texte;i++)                         /* parcourt des textes                                          */
        if (pere->texte[i].item>tmp)                       /* si le texte est après la place laissée libre                 */
            pere->texte[i].item--;                         /* décrémenter sa place                                         */
    for (i=0;i<pere->nb_com;i++)                           /* parcourt des commentaires                                    */
        if (pere->com[i].item>tmp)                         /* si le commentaire est après la place laissée libre           */
            pere->com[i].item--;                           /* décrémenter sa place                                         */
}

GMarkupDomNode * ajoute_node(GMarkupDomNode * pere,guint i,char * texte)  /* ajoute la ième node                           */
{                                                          /* Attention : non testé avec les textes et les commentaires    */
    if (pere->nb_fils==0)
    {
        i=0;
        xml_ajoute_fin(pere,texte);                        /* si c'est le premier, on prend la fonction qui met à la fin   */
    }
    else
    {
        unsigned short j,k;
        guint tmp;

        if (i>pere->nb_fils)
        {                                                      /* cas anormal, mais je mets un garde fou                      */
            i=pere->nb_fils;                                   /* ce sera le dernier élément                                  */
        }
        else
        {
        }
        tmp=pere->fils[i].item;                                /* tmp est l'indice de la nouvelle node                         */
        pere->nb_fils++;                                       /* la node mère aura un enfant de plus                          */
        pere->fils=(GMarkupDomNode *)g_realloc(pere->fils,pere->nb_fils*sizeof(GMarkupDomNode)); /* place pour le nouveau        */
        for(j=pere->nb_fils-1;j>i;j--)
        {
            pere->fils[j]=pere->fils[j-1];                     /* rapatrie le noeud suivant                                    */
            pere->fils[j].item++;                              /* incrémenter sa place                                         */
        }
        for (j=0;j<pere->nb_texte;j++)                         /* parcours des textes                                          */
        {
            if (pere->texte[j].item>tmp)                       /* si le texte est après la nouvelle place                      */
            {
                 pere->texte[j].item++;                        /* incrémenter sa place                                         */
            }
            else
            {
                /* le texte est avant la nouvelle node : pas besoin de changer sa place */
            }
        }
        for (j=0;j<pere->nb_com;j++)                           /* parcours des commentaires                                    */
        {
             if (pere->com[j].item>tmp)                        /* si le commentaire est après la nouvelle place                */
             {
                  pere->com[j].item++;                         /* incrémenter sa place                                         */
             }
             else
             {
                /* le commentaire est avant la nouvelle node : pas besoin de changer sa place */
             }
        }
        pere->fils[i].nom=g_strdup(texte);                     /* nom de la nouvelle node                                      */
        pere->fils[i].niveau=pere->niveau+1;                   /* niveau de la nouvelle node                                   */
        pere->fils[i].texte=NULL;
        pere->fils[i].nb_texte=0;
        pere->fils[i].attributs=NULL;
        pere->fils[i].nb_att=0;
        pere->fils[i].com=NULL;
        pere->fils[i].nb_com=0;
        pere->fils[i].parent=pere;
        pere->fils[i].fils=NULL;
        pere->fils[i].nb_fils=0;
        for (k=0;k<pere->nb_fils;k++)
        {   /* mettre à jour les fils du fils à cause du realloc                    */
            for(j=0;j<pere->fils[k].nb_fils;j++)
            {
                 pere->fils[k].fils[j].parent=pere->fils+k;
            }
        }
    }

    return pere->fils + i;
}

void xml_sup_node(GMarkupDomNode *node) /* supprime la node passée en paramètre */
{
    if (node==NULL || node->parent==NULL)
    {
    }
    else
    {
        supprime_node(node->parent,node-node->parent->fils);
    }
}

unsigned char echange_node(GMarkupDomNode * node,GMarkupDomNode * node1)
{                                                       /* ATTENTION : laisse les textes inchangés */
    if (node==NULL || node1==NULL) return FALSE;

    GMarkupDomNode * parent=node->parent, * parent1=node1->parent,tmp_node;
    gint place=node-parent->fils, place1=node1-parent1->fils;
    guint item=node->item; /* place et item : même chose */

    node->item=node1->item;
    node1->item=item;

    node->parent=parent1;
    node1->parent=parent;

    tmp_node=*node;
    parent->fils[place]=*node1;
    parent1->fils[place1]=tmp_node;

    return TRUE;
}

void ecrit_xml(FILE * sortie, char * ch) /* prise en compte des caractères spéciaux du xml */
{
    unsigned long i;

    if (ch!=0)
    {
        for (i=0;ch[i];i++)
        {
                    switch(ch[i])
                    { /* compatibilité avec les restrictions de la bibliothèque glib */
                      case '&'  : g_fprintf(sortie,"&amp;"); break;
                      case '<'  : g_fprintf(sortie,"&lt;");  break;
                      case '>'  : g_fprintf(sortie,"&gt;");  break;
                      case '\'' : g_fprintf(sortie,"&apos;");break;
                      case '\"' : g_fprintf(sortie,"&quot;");break;
                      default   : fputc(ch[i],sortie);
                    }
        }
    }
    else
    {
        printf("Erreur appel de ecrit_xml avec un pointeur nul\n");
    }
}
