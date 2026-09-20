#include "include.h"

void classe_active (GtkWidget *ChildWidget, cairo_t *cr,struct widgets *widgets);
void race_active (GtkWidget *ChildWidget, cairo_t *cr,struct widgets *widgets);

gboolean style1(GtkWidget *ChildWidget, cairo_t *cr)
{
     style (ChildWidget,cr,1.*0xf0/255,1.*0xe6/255,1.*0x8c/255);
     bord(ChildWidget,cr);

     return FALSE;
}

gboolean style2(GtkWidget *ChildWidget, cairo_t *cr)
{
     style (ChildWidget,cr,1.*0xff/255,1.*0xf0/255,1.*0xf5/255);
     bord(ChildWidget,cr);

     return FALSE;
}

gboolean style3(GtkWidget *ChildWidget, cairo_t *cr)
{
     style (ChildWidget,cr,1.*0xfd/255,1.*0xd7/255,1.*0x00/255);
     bord(ChildWidget,cr);

     return FALSE;
}

gboolean style4(GtkWidget *ChildWidget, cairo_t *cr)
{
     style (ChildWidget,cr,1.*0xad/255,1.*0xdd/255,1.*0x2f/255);
     bord(ChildWidget,cr);

     return FALSE;
}

gboolean style5(GtkWidget *ChildWidget, cairo_t *cr)
{
     style (ChildWidget,cr,1.*0xe0/255,1.*0xff/255,1.*0xff/255);
     bord(ChildWidget,cr);

     return FALSE;
}

gboolean style_race(GtkWidget *ChildWidget, cairo_t *cr,struct widgets *widgets)
{

     const char * race;
     signed short nb_race=NB_RACE,i;
     GtkMenuItem * entree_menu;
     signed short versi;
     gint width = gtk_widget_get_allocated_width (ChildWidget);
     gint height = gtk_widget_get_allocated_height (ChildWidget);



     if (ChildWidget==NULL)
     { /* réinitialisation forcée au futur passage */
         printf("Virer un passage par style_race\n");
     }
     else
     {
         race = gtk_menu_item_get_label(GTK_MENU_ITEM(ChildWidget));

         for (i=0;i<NB_RACE;i++)
         {
             if (compare_sans_casse((char *)race,RACE[i].nom)==0)
             {
                 nb_race=i;
                 i=NB_RACE; /* on sort du for */
             }
             else
             {
                 /* pas la race recherchée */
             }
         }
         if (nb_race<NB_RACE)
         {
             entree_menu=GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"ADD"));

             if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(entree_menu))==TRUE)
             { /* ADD */
                 versi=ADD;
             }
             else
             { /* ADD2 */
                 versi=ADD2;
             }

             if (RACE[nb_race].add[versi].nom!=NULL)
             {
                  gtk_widget_set_sensitive(ChildWidget,TRUE);
                  cairo_set_source_rgb (cr, 0.9, 0.9, 0.9);
                  cairo_rectangle (cr, 0, 0, width, height);
                  cairo_fill (cr);
             }
             else
             {
                  gtk_widget_set_sensitive(ChildWidget,FALSE);
                  cairo_set_source_rgb (cr, 1, 0, 0);
                  cairo_rectangle (cr, 0, 0, width, height);
                  cairo_fill (cr);
             }
         }
     }

     (void)cr;
     return FALSE;
}

gboolean style_classe(GtkWidget *ChildWidget, cairo_t *cr,struct widgets *widgets)
{
     const char * classe;
     signed short nb_race,nb_classe=NB_CLASSE,i;
     GtkMenuItem * entree_menu;
     signed short versi;
     gint width = gtk_widget_get_allocated_width (ChildWidget);
     gint height = gtk_widget_get_allocated_height (ChildWidget);

     if (ChildWidget==NULL)
     { /* réinitialisation forcée au futur passage */
         printf("Supprimer le passage dans style_classe\n");
     }
     else
     {
         classe = gtk_menu_item_get_label(GTK_MENU_ITEM(ChildWidget));
         for (i=0;i<NB_CLASSE;i++)
         {
             if (compare_sans_casse((char *)classe,CLASSE[i].nom)==0)
             {
                 nb_classe=i;
                 i=NB_CLASSE; /* sortir du for */
             }
             else
             {
             }
         }
         if (nb_classe<NB_CLASSE)
         {
             nb_race=trouve_race(widgets);

             entree_menu=GTK_MENU_ITEM (gtk_builder_get_object(widgets->builder,"ADD"));
             if (gtk_check_menu_item_get_active (GTK_CHECK_MENU_ITEM(entree_menu))==TRUE)
             {
                 versi=ADD;
             }
             else
             {
                 versi=ADD2;
             }

             if (est_compatible(nb_race,nb_classe,versi))
             {
                  gtk_widget_set_sensitive(ChildWidget,TRUE);
                  cairo_set_source_rgb (cr, .9, .9, .9);
                  cairo_rectangle (cr, 0, 0, width, height);
                  cairo_fill (cr);
             }
             else
             {
                gtk_widget_set_sensitive(ChildWidget,FALSE);
                if(CLASSE[nb_classe].add[versi].nom==NULL)
                {
                  cairo_set_source_rgb (cr, 1, 0, 0);
                  cairo_rectangle (cr, 0, 0, width, height);
                  cairo_fill (cr);

                }
                else
                {
                  cairo_set_source_rgb (cr, 1, 0.5, 0);
                  cairo_rectangle (cr, 0, 0, width, height);
                  cairo_fill (cr);
                }
              }
         }
     }

     (void)cr;
     return FALSE;
}



void bord (GtkWidget *ChildWidget, cairo_t *cr)
{
   	gdouble dash[2];
	gint width = gtk_widget_get_allocated_width (ChildWidget);
	gint height = gtk_widget_get_allocated_height (ChildWidget);

	dash[0]=5;
	dash[1]=10;
	cairo_set_dash (cr, dash, 1, 0); // Affectation du trait discontinu déclaré dans le tableau dash
	cairo_set_line_width (cr, 1); // Epaisseur du trait
	cairo_set_source_rgb (cr, 0, 0, 0);

	/*Dessin d'un trait autour du widget*/
	cairo_move_to (cr, 0, 0);
	cairo_line_to (cr, width, 0);
	cairo_line_to (cr, width, height);
	cairo_line_to (cr, 0, height);
	cairo_line_to (cr, 0, 0);
	cairo_stroke (cr);
}

void style (GtkWidget *ChildWidget, cairo_t *cr,double rouge, double vert, double bleu)
{ /* on trace dans la cellule du dessus */
	/*cairo_set_source_rgb (cr, rouge, vert, bleu); // Affectation de la couleur récupérée en tant que donnée utilisateur
    cairo_paint(cr);*/
        gdouble dash[2];
        gint width = gtk_widget_get_allocated_width (ChildWidget);
        gint height = gtk_widget_get_allocated_height (ChildWidget);


        dash[0]=5;
        dash[1]=10;
        cairo_set_dash (cr, dash, 1, 0); // Affectation du trait discontinu déclaré dans le tableau dash
        cairo_set_line_width (cr, 2); // épaisseur du trait
        cairo_set_source_rgb (cr, rouge, vert, bleu);

        /*Dessin d'un trait autour du widget*/
        cairo_move_to (cr, 0,0);
        cairo_line_to (cr, width, 0);
        cairo_line_to (cr, width, height);
        cairo_line_to (cr, 0, height);
        cairo_line_to (cr, 0, 0);
        /*cairo_line_to (cr, width+10, -height);*/
        cairo_stroke (cr);

    (void)ChildWidget;
}

void classe_active (GtkWidget *ChildWidget, cairo_t *cr,struct widgets *_widgets)
{
    /*style_classe(NULL,NULL,NULL);*/

    (void)ChildWidget;
    (void)cr;
    (void)_widgets;
}

void race_active (GtkWidget *ChildWidget, cairo_t *cr,struct widgets *_widgets)
{
    /*style_race(NULL,NULL,NULL);*/

    (void)ChildWidget;
    (void)cr;
    (void)_widgets;
}

void couleur_item(GtkWidget *ChildWidget,signed short couleur)
{
/* On mémorise le dernier style dans une donnée attachée au widget.                      */
/* Si aucune donnée n'est actuellement mise : premier passage, on initialise le système. */
/* Si on veut remettre le même que l'actuel, on ne fait rien.                            */

    GtkStyleContext * css;
    unsigned short * pt_couleur;

    pt_couleur = (unsigned short *) g_object_get_data(G_OBJECT(ChildWidget), "couleur" );
    css=gtk_widget_get_style_context (ChildWidget);

    if (pt_couleur && *pt_couleur==couleur)
    { /* même couleur : on ne change pas */
    }
    else
    {
        if (pt_couleur!=NULL)
        {   /* on enlève le style précédent */
            switch (*pt_couleur)
            {
                case GRIS: gtk_style_context_remove_class (css,"gris");
                    break;
                case ROUGE: gtk_style_context_remove_class (css,"rouge");
                    break;
                case ORANGE: gtk_style_context_remove_class (css,"orange");
                    break;
                case VERT: gtk_style_context_remove_class (css,"vert");
                    break;
                case VIOLET: gtk_style_context_remove_class (css,"violet");
                    break;
                case BLEU: gtk_style_context_remove_class (css,"bleu");
                    break;
                case JAUNE: gtk_style_context_remove_class (css,"jaune");
                    break;
                case GRISCLAIR: gtk_style_context_remove_class (css,"grisclair");
                    break;
                case GRISFONCE: gtk_style_context_remove_class (css,"grisfonce");
                    break;
                default:
                    break;
            }
        }
        else
        {   /* système non initialisé : on prépare la place mémoire */
            pt_couleur=(unsigned short *)g_malloc(sizeof(unsigned short));
        }

        switch (couleur)
        {   /* on met le nouveau style */
            case GRIS: gtk_style_context_add_class (css,"gris");
                break;
            case ROUGE: gtk_style_context_add_class (css,"rouge");
                break;
            case ORANGE: gtk_style_context_add_class (css,"orange");
                break;
            case VERT: gtk_style_context_add_class (css,"vert");
                break;
            case VIOLET: gtk_style_context_add_class (css,"violet");
                break;
            case BLEU: gtk_style_context_add_class (css,"bleu");
                break;
            case JAUNE: gtk_style_context_add_class (css,"jaune");
                break;
            case GRISCLAIR: gtk_style_context_add_class (css,"grisclair");
                break;
            case GRISFONCE: gtk_style_context_add_class (css,"grisfonce");
                break;
            default:
                break;
        }
        *pt_couleur=couleur; /* memorisation du style actuel */
        g_object_set_data(G_OBJECT(ChildWidget), "couleur",pt_couleur);
    }
}
