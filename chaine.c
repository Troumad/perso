/*
    Génération de feuille de personnage complète AD&D 1 et 2
    Génération aléatoire de personnage

    Copyright (C) 2011-2026 Bernard SIAUD

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    a1024 with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "include.h"
#include <ctype.h>

const gchar * accent     ="ÀÁÂÃÄÅÇÈÉÊËÌÍÎÏÒÓÔÕÖÙÚÛÜÝàáâãäåçèéêëìíîïðòóôõöùúûüýÿÑñ";
const gchar * sans_accent="AAAAAACEEEEIIIIOOOOOUUUUYaaaaaaceeeeiiiioooooouuuuyyNn";

signed char compare_sans_casse(char * s1,char * s2)
{
    char c1=0,c2=0;
    unsigned short i;
    signed short renvoi;


    if ((s1==NULL || *s1==0) && (s2==NULL || *s2==0))
    {
        renvoi=0;
    }
    else if (s1==NULL || *s1==0)
    {
        renvoi=-1;
    }
    else if (s2==NULL || *s2==0)
    {
        renvoi=1;
    }
    else
    {
        s1--;
        s2--;
        do
        {
          s1++;
          s2++;
          if (*s1==-61 && *(s1+1)!=0)
          {
            s1++;
            c1=g_ascii_toupper(*s1); /* éviter un retour comment le précédent */
            for(i=0;sans_accent[i]!=0;i++)
              if (*s1==accent[2*i+1])
                c1=g_ascii_toupper(sans_accent[i]);
          }
          else
            c1=g_ascii_toupper(*s1);

          if (*s2==-61 && *(s2+1)!=0)
          {
            s2++;
            c2=g_ascii_toupper(*s2);
            for(i=0;sans_accent[i]!=0;i++)
              if (*s2==accent[2*i+1])
                c2=g_ascii_toupper(sans_accent[i]);
          }
          else
            c2=g_ascii_toupper(*s2);

        }
        while(*s1!=0 && *s2!=0 && c1==c2);

        if (*s1==0 && *s2==0)
        {
            renvoi=0;
        }
        else if (*s1==0)
        {
            renvoi=-1;
        }
        else if (*s2==0)
        {
            renvoi=1;
        }
        else
        {
            renvoi=c1-c2;
        }

    }

    return renvoi;
}

int str_isspace (const gchar *s) /* est-ce une chaine vide ? */
{
  int retour;

  if (!s)
  {
      retour=1;
  }
  else
  {
    for (; *s && (*s==' ' || *s=='\t' || *s=='\n'); s++);
    retour=!*s;
  }

  return retour;
}

char * simplifie_uri(char * _nom)
{
 unsigned short i;
 char *s, * s1;
 char * retour;

 retour=_nom;
 g_strstrip(_nom);
 while((s=g_strstr_len (_nom,-1,"/./"))) /* tant qu'on trouve cette occurrence */
 {
     for (i=1;s[i+2]!=0;i++)
     {
         s[i]=s[i+2];
     }
     s[i]=s[i+2];
 }

 while((s=g_strstr_len (_nom,-1,"/../"))) /* tant qu'on trouve cette occurrence */
 {
     *s=0;
     s1=g_strrstr(_nom,"/");
     if (s1!=NULL)
     {
         for (i=1;s[i+3]!=0;i++)
         {
             s1[i]=s[i+3];
         }
         s1[i]=s[i+3];
     }
     else
     {
         retour=NULL;
         *s='\\';
     }
 }

 return retour;
}
