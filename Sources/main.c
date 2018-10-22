/*
+------------------------------------------------------------------------------+
| SGF et mini BASH                                                             |
| Auteurs : Apete William, Barotin Alexis, Blot Nicolas, Cabaleiro Andréas,    |
|          Gabiou Alexis, Gaillard Robin, Lenormand Olivia                     |
+------------------------------------------------------------------------------+
*/

#include "../Headers/include.h"

int init_superBloc(void);
int init_block();
int init_inode();
int init_fd();
void interpreteur(char *chaine);

int main(void)
{
    char c[512];
    init_superBloc();
    init_inode();
    init_block();
    init_fd();
    
    my_mkdir("/");
    PWD=getInode("/");
    printf("%s ~ $ ",DISQUE.tabinode[PWD].nom);
	while(1)
	{
	    c[0]='\0';
	    fgets(c,sizeof(c),stdin);
	    str_rem(c,"\n");
	    if (!strcmp(c,"exit")) break;
	    interpreteur(c);
	    printf("%s ~ $ ",DISQUE.tabinode[PWD].nom);
	}
	return 0;
}


int init_superBloc() //Inialise les valeurs du superbloc
{
    DISQUE.sb.partitionSize=TAILLEBLOCK*NBBLOCK + NBINODE * sizeof(inode) + sizeof(superBlock);
    DISQUE.sb.firstAvalaibleBlockIndex=get_firstAB();
    DISQUE.sb.firstAvalaibleInodeIndex=get_firstInode();
    return 0;
}

int init_block() // Initialise tout les blocs en les rendant disponibles et en initilisant la data comme vide
{
    int i;
    for (i=0;i<NBBLOCK;i++)
    {
        DISQUE.tabblock[i].dispo=0;
        DISQUE.tabblock[i].data[0]='\0';
    }
    return 0;
}

int init_inode() //Initialise les champs de toutes les inodes
{
    int i,j;
    for (i=0;i<NBINODE;i++)
    {
        for ( j = 0; j<9;j++)
        {
            DISQUE.tabinode[i].droits[j]='-';
        }
        DISQUE.tabinode[i].block=NULL;
        DISQUE.tabinode[i].filetype=0;
        for (j=0;j<3;j++)
        {
            DISQUE.tabinode[i].datecreation[j]=0;
        }
        DISQUE.tabinode[i].taille=0;
        DISQUE.tabinode[i].nbBlockUsed=0;
        DISQUE.tabinode[i].irepParent=-1;
    }
    return 0;
}
int init_fd() //Initialise le tableau des fichier ouverts en mettant les index d'inodes à -1 et la position du curseur à 0
{
    int i; 
    for (i = 0 ; i < TAILLETABFD ; i++)
    {
        TABFD[i].inode=-1;
        TABFD[i].pos_cursor=0;
    }
    return 0;
}

void interpreteur(char *chaine) //Interpréteur de commande
{
    char **argv=NULL;
    argv=split(chaine,' ');
    int argc=getArgc(chaine,' ');
    if ((argv[0] != NULL) && (strcmp(chaine,"\0")!=0) )
    {
        if ( strcmp(argv[0],"cp") == 0 )
        {
            cmdcp(argc, argv);
        }
        else if ( strcmp(argv[0],"rm") == 0 )
        {
            cmdrm(argc,argv);
        }
        else if ( strcmp(argv[0],"mv") == 0 )
        {
             cmdmv(argc,argv);
        }
        else if ( strcmp(argv[0],"cat") == 0 )
        {
            cmdcat(argc,argv);
        }
        else if ( strcmp(argv[0],"ls") == 0 )
        {
            cmdls(argc,argv);
        }
        else if ( strcmp(argv[0],"mkdir") == 0 )
        {
            cmdmkdir(argc,argv);
        }
        else if ( strcmp(argv[0],"rmdir") == 0 )
        {
            cmdrmdir(argc,argv);
        }
        else if ( strcmp(argv[0],"cd") == 0 )
        {
            cmdcd(argc,argv);
        }
        else if ( strcmp(argv[0],"df") == 0 )
        {
            cmddf(argc,argv);
        }
        else if ( strcmp(argv[0],"chmod") == 0 )
        {
            cmdchmod(argc,argv);
        }
        else if ( strcmp(argv[0],"touch") == 0 )
        {
            cmdtouch(argc,argv);
        }
       
        else if ( strcmp(argv[0],"echo") == 0 )
        {
            cmdecho(argc,argv);
        } 
        else if ( strcmp(argv[0],"wecho") == 0 )
        {
            wecho(argc,argv);
        } 
        else
        {
            printf("Erreur, commande inconnue %s \n",argv[0]);
        }
    }
    
}
