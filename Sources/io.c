#include "../Headers/include.h"

int mywrite(int fd,char *chaine,int count) //Primitive qui écrit dans un fichier
{
    int i,j=TABFD[fd].pos_cursor/TAILLEBLOCK;
    if ( fd < 0 || TABFD[fd].inode==-1 || DISQUE.tabinode[TABFD[fd].inode].filetype!=1 )
    //Si le descripteur de fichier n'existe pas ou que l'inode qu'il ne contient pas d'inode ou que le fichier n'est pas un fichier texte
    {
        return -1; //Erreur
    }
    else
    {
        for ( i=0;i<count;i++)
        {
            if (((TABFD[fd].pos_cursor%TAILLEBLOCK)==0)&&TABFD[fd].pos_cursor!=0) //Si on est en bout de bloc
            {
                j++; 
                if (j>DISQUE.tabinode[TABFD[fd].inode].nbBlockUsed) //Si tout les blocs alloués au fichier sont remplis, on réalloue un nouveau bloc
                {
                    if(myReallocBlock(&DISQUE.tabinode[TABFD[fd].inode])==-1) //Si la réallocation d'un nouveau bloc a échouée
                    {
                        break;
                    }
                }
            }
        DISQUE.tabblock[DISQUE.tabinode[TABFD[fd].inode].block[j]].data[TABFD[fd].pos_cursor%TAILLEBLOCK]=chaine[i];
        TABFD[fd].pos_cursor++;
            
        }
        DISQUE.tabblock[DISQUE.tabinode[TABFD[fd].inode].block[j]].data[TABFD[fd].pos_cursor%TAILLEBLOCK]='\0';
        return i;
    }
    
}

int myread(int fd, char *buff, int count) //Primitive pour lire dans un fichier
{
    int i;
    for (i=1;i<count;i++)
    {
        buff[i]='\0';
    }
    if (fd < 0 || TABFD[fd].inode==-1 || DISQUE.tabinode[TABFD[fd].inode].filetype!=1)
    //Si le descripteur de fichier n'existe pas ou que l'inode qu'il ne contient pas d'inode ou que le fichier n'est pas un fichier texte
    {
        return -1; //Erreur
    }
    else
    {
        int j=(TABFD[fd].pos_cursor/TAILLEBLOCK),c=0;
        while ((c<count) && (DISQUE.tabblock[DISQUE.tabinode[TABFD[fd].inode].block[j]].data[TABFD[fd].pos_cursor%TAILLEBLOCK]!='\0')) 
        //Tant que le buffer n'est pas rempli et que l'on est pas à la fin du fichier
        {
            if((TABFD[fd].pos_cursor%TAILLEBLOCK==0)&&(c!=0)) //Si on est à la fin d'un bloc
                j++; //On change de bloc
            buff[c]=DISQUE.tabblock[DISQUE.tabinode[TABFD[fd].inode].block[j]].data[TABFD[fd].pos_cursor%TAILLEBLOCK];
            TABFD[fd].pos_cursor++;
            c++;
        }
        return c;
    }
}
