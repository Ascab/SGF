#include "../Headers/include.h"

char** split(char *chaine, char car) //Sépare une chaine en sous chaine séparées par un caractère
{   
    char** tab;
    tab=NULL;
    
    int i=0,j=0,k=0,count=0;
    tab=(char**)malloc(sizeof(char*));
    tab[0]=(char*)malloc(sizeof(char)*64);
    while ( chaine[count] != '\0')
    {
        count++;
    }
   
   for ( k=0 ; k< count ; k++)
    {
        if ( chaine[k] == car)
        {
            tab[i][j]='\0';
            i++;
            j=0; 
            tab=(char**)realloc(tab,sizeof(char*)*(i+1));
            tab[i]=(char*)malloc(sizeof(char)*64);
        }
        else
        {
            tab[i][j]=chaine[k];
            j++;
        }
    }
    tab[i][j]='\0';
    return tab;
}



int str_rem(char *s, char const *srem) /* Source https://www.developpez.net/forums/d133547/c-cpp/c/enlever-d-chaine/ */
//Retire une sous chaine d'une chaine 
{
  int n = 0;
  char *p;
 
  if (s && srem)
  {
    size_t const len = strlen(srem);
 
    while((p = strstr(s, srem)) != NULL)
    {
      memmove(p, p + len, strlen(p + len) + 1);
      n++;
    }
  }
  return n;
}
