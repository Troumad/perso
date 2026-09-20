/*
    Génération de feuille de personnage complète AD&D 1 et 2
    Génération aléatoire de personnage

    Copyright (C) 2011 Bernard SIAUD

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

/* fichier de lecture/écriture et interprétation des feuilles de classe et de race */

#include "include.h"
#include <string.h>
#include <zip.h>
#include <errno.h>


#define ZIPZAP_SUCCESS 1

char * content_libo(char * fichierZip,size_t * longueur)
{

    /* Code d'origine : */
    /* http://forum.ubuntu-fr.org/viewtopic.php?pid=2596893#p2596893 Visité le 02/01/2011 */
    /* modifié pour ajouter la gestion des erreurs et quelques fonctionnalités */

    int err = 0;
    struct zip *f_zip=NULL;
    struct zip_stat file_stat;
    struct zip_file *file_zip=NULL;
    int id=-1;
    int i = 0;
    char * retour=NULL;
    zip_error_t error;

    if(fichierZip == NULL)
    {
        retour=NULL;
    }
    else
    {
        f_zip = zip_open(fichierZip, ZIP_CHECKCONS, &err); /* on ouvre l'archive zip */

        /* s'il y a des erreurs */
        if(err != ZIP_ER_OK)
        {
            zip_error_init_with_code(&error, err);
            printf("Error %d : %s\n",err,zip_error_strerror(&error));
            zip_error_fini(&error);
            retour=NULL;
        }
        else if(f_zip==NULL)
        { /* si le fichier zip n'est pas ouvert */
            printf("Erreur à l'ouverture du fichier %s\n", fichierZip);
            retour= NULL;
        }
        else
        {
            /* on récupère le nombre de fichier dans l'archive zip */
            int count =  zip_get_num_entries(f_zip,ZIP_FL_UNCHANGED);
            if(count==-1)
            {
                printf("Erreur à la lecture du fichier %s\n", fichierZip);
                zip_close(f_zip);
                f_zip = NULL;
                retour = NULL;
            }
            else
            {
                id=-1;
                for(i=0; i<count; i++)
                {
                    /* on utilise la position "i" pour récupérer le nom des fichiers */
                    if (strcmp("content.xml",zip_get_name(f_zip, i, ZIP_FL_UNCHANGED))==0) id=i;
                }
                if(id==-1)
                {

                    printf("pas de content.xml dans le fichier %s\n", fichierZip);
                    zip_close(f_zip);
                    f_zip = NULL;
                    retour = NULL;
                }
                else
                {
                    zip_stat_index(f_zip, id, 0, &file_stat);

                    /* 4. pour connaître la taille du fichier et ainsi pouvoir le lire en entier*/
                    if(zip_stat(f_zip, file_stat.name, 0, &file_stat) == -1)
                    {
                        printf("%s\n", zip_strerror(f_zip));
                        retour = NULL;
                    }
                    else
                    {
                        /* 5. on ouvre le fichier archivé */
                        file_zip=zip_fopen(f_zip, file_stat.name, ZIP_FL_UNCHANGED);
                        if(!file_zip)
                        {
                            printf("%s\n", zip_strerror(f_zip));
                            retour = NULL;
                        }
                        else
                        {
                            char *str=NULL;
                            str = g_malloc((size_t)(file_stat.size+1));
                            *longueur=file_stat.size;
                            memset(str, 0, (size_t)(file_stat.size+1));
                            if(str == NULL)
                            {
                                printf("Erreur d'allocation mémoire\n");
                                retour = NULL;
                            }
                            else if(zip_fread(file_zip, str, (size_t)(file_stat.size)) != (int)file_stat.size)
                            { /* 6. on lit le fichier archivé */
                                printf("%s\n", zip_strerror(f_zip));
                                g_free(str);
                                retour = NULL;
                            }
                            else
                            {
                                retour = str;
                            }
                            zip_close(f_zip);
                            f_zip = NULL;
                            zip_fclose(file_zip);
                            file_zip = NULL;
                        }
                    }
                }
            }
        }
    }

    return retour;
}
