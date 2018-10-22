#include "../Headers/include.h"

int getArgc ( char* tab, char car ) //Retourne le nombre de sous-chaine d'une chaine séparées d'un char
{
    int count = 1 ;
    int i;
    
    for ( i = 0 ; i<strlen(tab);i++)
    {
        if ( tab[i] == car)
        {
            count++;
        }
    }   
    return count;
}

void cmdmkdir (int argc , char* argv[]) //Commande qui permet de créer un répertoire
{
    int i;
    if ( argc == 1 )
    {
        printf("Usage: mkdir <nomrépertoire1>...\n");
    }
    else
    {
        for (i=1;i<argc;i++)
        {
            if ( my_mkdir(argv[i]) < 0) //Si erreur lors de l'utilisation de la primitive my_mkdir
            {
                printf("Erreur lors de la création du réportoire %s\n", argv[i]);
            }
        }
    }
}
void cmdrmdir(int argc, char *argv[]) //Commande pour supprimer un répertoire
{
    
    if (argc <= 1 )
        printf("Usage : rmdir <nomrépertoire>...\n");
    else
    {
        int i;
        for (i=1 ; i < argc ; i++)
        {
            if (my_rmdir(argv[i])==-1) //Appel à la primitive my_rmdir et message d'erreur en cas d'erreur
                 printf("Erreur lors de la suppression du réportoire %s\n", argv[i]);
        }
    }
}

void cmdls(int argc,char *argv[]) //Commande qui affiche les fichies présent dans un répertoire
{
       int i,count;
       char chaine[50],**splitC;
       for (i=0;i<DISQUE.tabinode[PWD].taille;i++)
        {
           chaine[i]=DISQUE.tabblock[DISQUE.tabinode[PWD].block[0]].data[i];
        }
        chaine[DISQUE.tabinode[PWD].taille]='\0';
        splitC=split(chaine,';');
        count=getArgc(chaine,';')-1;
       
       for (i=0;i<count;i++)
       {
           switch(DISQUE.tabinode[atoi(splitC[i])].filetype)
           {
            case 1: printf("[f] ");
                    break;
            case 2: printf("[d] ");
                    break;
            case 3: printf("[l] ");
                    break;
            default: printf("[?] ");
                    break;
           }
           printf("%s %s\n",DISQUE.tabinode[atoi(splitC[i])].nom,DISQUE.tabinode[atoi(splitC[i])].droits);
       }
}
void cmdtouch(int argc, char *argv[]) //Commande pour créer un fichier
{
    if (argc <= 1){
        printf("Usage : touch <fichiers>...\n");
    }
    else 
    {
        int i;
        for (i = 1 ; i < argc ; i++)
        {
            if (getInode(argv[i]) == -1) //On vérifie que le fichier à créer n'existe pas deja
            {
                if (mycreat(argv[i]) == -1) //Si erreur lors de l'appel de mycreat
                 printf("Erreur lors de la création du fichier %s\n", argv[i]);
            }
            else
                printf("Fichier %s deja existant\n",argv[i]);
        }
    }
}

void cmdrm(int argc, char *argv[]) //Commande pour supprimer un fichier
{
    if (argc <= 1){
        printf("Usage : rm <fichiers>...\n");
    }
    else 
    {
        int i;
        for (i=1 ; i < argc ; i++)
        {
            if (mydelete(argv[i])==-1) //Si erreur lors de l'appel de mydelete
                 printf("Erreur lors de la suppression du fichier %s\n", argv[i]);
        }
    }
}
        

        
void cmdcd(int argc, char *argv[]) // Commande pour changer de répertoire courant
{
    int indexInode;
    if (argc != 2)
        printf("Usage : cd <repertoire>\n");
    else if (((indexInode=getInode(argv[1])) == -1) //Si le dossier n'existe pas
    ||(DISQUE.tabinode[indexInode].filetype != 2) // Ou que ce n'est pas un répertoire
    ||((DISQUE.tabinode[indexInode].irepParent != PWD) // Ou que le répertoire n'est pas le fils du répertoire courant
            &&(DISQUE.tabinode[PWD].irepParent != indexInode)) // et qu'il n'est aussi pas le père du répertoire courant
    )
        printf("Erreur lors du changement de repertoire courant\n");
    else 
        PWD=indexInode;
}


void cmddf(int argc, char *argv[]) //Commande qui affiche l'étât du disque
{
    int i,countI=0,countB=0;
    for (i=0;i<NBBLOCK;i++)
    {
        if (DISQUE.tabblock[i].dispo==0)
            countB++;
       
    }
    for (i=0;i<NBINODE;i++)
    {
    if (DISQUE.tabinode[i].filetype==0)
            countI++;
    }
    printf("Nombre de bloc dispo: %d\n",countB);
    printf("Nombre d'inodes dispo: %d\n",countI);
    printf("Soit au total: %ld octets disponibles\n",countB*sizeof(blocks));
}




void cmdcat(int argc, char *argv[]) //Commande qui affiche le contenu d'un fichier
{
    if (argc <= 1){
        printf("Usage : cat <fichiers>...\n");
    }
    else
    {
        char buff[10];
        int i,j;
        for (i=1 ; i < argc ; i++)
        {
            if (getInode(argv[i])==-1) //Si le fichier n'existe pas
            {
                printf("Fichier innexistant %s\n",argv[i]);
            }
            else
                {
                int fd;
                if ((fd=myopen(argv[i]))!=-1) //Si l'ouverture du fichier se fait correctement
                {
                    while((j=myread(fd,buff,sizeof(buff))) > 0) //Tant que l'on lit des caractères on les affiches
                        printf("%s",buff);
                    myclose(fd);
                }
            }
            printf("\n");
        }
    
    }
}

void cmdecho(int argc, char *argv[]) 
{
    if (argc <= 1){
        printf("Usage : echo <texte>...\n");
    }
    else 
    {
        int i;
        for (i=1 ; i < argc ; i++)
        {
            printf("%s ",argv[i]);
        }
        printf("\n");
    }
}

void cmdmv(int argc , char *argv[]) //Commande pour modifier le répertoire père d'un fichier
{
    if (argc != 3)
    {
        printf("Usage: mv <src> <dest>\n");
    }
    
    else if ( (retire_fichier(getInode(argv[1]))!=1) || (add(getInode(argv[1]),getInode(argv[2]))!=1)  )
    //S'il y a une erreur lors de l'utilisation de retire_fichier ou add, on affiche un message d'erreur
    {    
     printf("Erreur lors du déplacement du fichier\n");
    }
}

void cmdchmod(int argc,char *argv[]) //Commande pour modifier les droits
{
    int i,index;
    if ( argc != 3 )
    {
        printf("Usage: chmod <droits> <fichier>\n");
    }
    else if(verif_droit(argv[1])!=1) //Si la chaine de droits entrée est incorrecte
    {
        printf("Mauvais écriture des droits\nExemple: rwxrw-r--\n");
    }
    else if ((index=getInode(argv[2]))==-1) // Si le fichier n'existe pas
    {
        printf("Fichier innexistant\n");
    }
    else
    {
        for ( i=0;i<9;i++)
        {
            DISQUE.tabinode[index].droits[i]=argv[1][i];
        }
    }
}


int verif_droit(char *chaine) //Vérifie qu'un chaine est une chaine décrivant des droits sur un fichier
{
    int ret=1,i; 
    if (strlen(chaine)!=9) //Si la taille de la chaine est différente de 9
        ret=0;
    else 
    {
        for (i=0;i<9;i++) //On parcourt la chaine
        {
            if (chaine[i]!='r' && chaine[i]!='w' && chaine[i]!='x' && chaine[i]!='-')
            //Si un caractère est différent de r,w,x ou -
                ret=0;
        }
    }
    return ret;
}

void cmdcp(int argc, char *argv[]) //Commande qui copie un fichier
{
    if (argc != 3)
        printf("Usage : cp <source> <dest>\n");
    else
    {
        char buff[50];
        int j, src, dest;
        if (((src=myopen(argv[1]))!=-1)&&((dest=myopen(argv[2]))!=-1) ) 
        //S'il n'y a pas une errreur lors de l'appel de myopen pour le fichier source ou de destination
        {
            while((j=myread(src,buff,sizeof(buff)))>0) //Tant que l'on lit un caractère dans le fichier sourcer
            {
                mywrite(dest, buff, sizeof(buff)); //On l'écrit dans le fichier de destination
            }
           
            myclose(src);
            myclose(dest);
        }
        else
        {
            printf("Erreur de copie\n");
        }
        
    }
}

void wecho(int argc,char *argv[]) //Commande pour écrire une chaine de caractère donnée en paramètre dans un fichier
{
    int fd,i;
    if (argc < 3)
    {
        printf("Usage: wecho <dest> <texte>\n");
    }
    else
    {
        if ( (fd=myopen(argv[1]))!=-1 ) //Si l'appel à myopen ne renvoit d'erreur
        {
            char str[256];
            for (i = 2 ; i < argc ; i++)
            {
                strcat(str,argv[i]);
                if (i!=argc-1)
                    strcat(str," ");
            }
            mywrite(fd,str,strlen(str));
            myclose(fd);
        }
    }
}
