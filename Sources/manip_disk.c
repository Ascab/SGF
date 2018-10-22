#include "../Headers/include.h"

int get_firstAB(void) //Renvoit l'index du premier bloc dispo
{
    int i=0, index=-1;
    while (i < NBBLOCK) //Tant que l'on a pas parcouru tout les blocs
    {
        if ((DISQUE.tabblock[i].dispo) == 0) //Si le bloc est dispo
        {
            index=i;
            break; //On sort du while
        }
        i++;
    }
    return index;
}
int get_firstInode() //Renvoit l'index de la premièr inode disponible
{
    int i=0, index=-1;
    
    while (i < NBINODE) //Tant que l'on a pas parcouru tout les blocs
    {
        if (! DISQUE.tabinode[i].filetype) //Si le type de fichier est 0
        {
            index=i;
            break; //On sort de la boucle
        }
        i++;
    }
    return index;   
}

int getInode(char nom[30])//Recupère l'inode contenant le même nom de fichier que celui donné en parametre
{
    int i=0, index = -1;
    while (i < NBINODE) //Tant que l'on a pas parcouru toutes les inodes
    {
        if ((DISQUE.tabinode[i].filetype != 0)&&(strcmp(nom, DISQUE.tabinode[i].nom)==0))
        //Si le fichier contenu dans l'inode est défini et qu'il n'y a pas de différences entre le nom de fichier
        //et le nom donnée en paramètre
        {
            index=i;
            break;
        }
        i++;
    }
    return index;
}

int myReallocBlock(inode *i) //Alloue un bloc supplémentaire à une inode donnée en paramètre
{
    int index ;
    if ((index = DISQUE.sb.firstAvalaibleBlockIndex) != -1) // S'il n'y a pas de bloc disponible
    {
        if ((i->block=(int*)realloc((void*)(i->block),  //Si erreur lors de la réallocation du tableau contenant les index de bloc
                            (i->nbBlockUsed+1)*sizeof(int)))==NULL) 
            return -1; //Erreur
        
        i->nbBlockUsed++;
        i->block[i->nbBlockUsed-1]=index;
        DISQUE.tabblock[index].dispo=1;
    }
    return index;
}
