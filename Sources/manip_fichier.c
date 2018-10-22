#include "../Headers/include.h"

int mycreat(char nom[30]) //Primitive qui crée un fichier
{
    if  (DISQUE.sb.firstAvalaibleBlockIndex==-1 || (DISQUE.sb).firstAvalaibleInodeIndex==-1)
    //S'il n'y a pas une inode ou un bloc disponible
    {
        return -1;
    }
    else //Sinon on modifie les informations de la première inode disponible
    {
        int indexInode;
        time_t t;
        struct tm *timeComp;
        t=time(NULL);
        timeComp = localtime(&t);
        indexInode=DISQUE.sb.firstAvalaibleInodeIndex;
        DISQUE.tabinode[indexInode].filetype=1;
        DISQUE.tabinode[indexInode].droits[0]='r';
        DISQUE.tabinode[indexInode].droits[1]='w';
        DISQUE.tabinode[indexInode].datecreation[0]=timeComp->tm_mday;
        DISQUE.tabinode[indexInode].datecreation[1]=timeComp->tm_mon+1;
        DISQUE.tabinode[indexInode].datecreation[2]=timeComp->tm_year+1900;
        DISQUE.tabinode[indexInode].nbBlockUsed++;
        strcpy(DISQUE.tabinode[indexInode].nom,nom);
        DISQUE.tabinode[indexInode].block=(int*)malloc(sizeof(int));
        DISQUE.tabinode[indexInode].block[0]= DISQUE.sb.firstAvalaibleBlockIndex;
        DISQUE.tabblock[DISQUE.tabinode[indexInode].block[0]].dispo++;
        add(indexInode,PWD);
        DISQUE.sb.firstAvalaibleBlockIndex=get_firstAB();
        DISQUE.sb.firstAvalaibleInodeIndex=get_firstInode();
        return indexInode;
    }
}

int mydelete (char nom[30]) //Primitive qui supprime un fichier texte
{
    int indexInode;
    int i,j;
    if ( ((indexInode=getInode(nom)) == -1 ) || (DISQUE.tabinode[indexInode].filetype != 1 ) )
    //Si le fichier n'existe pas ou que le fichier n'est pas un fichier texte
    { 
        return -1; //Erreur
    }
    else
    {
        retire_fichier(indexInode); //On retire le fichier du répertoire dans lequel il est contenu
        for (i=0;i<DISQUE.tabinode[indexInode].nbBlockUsed;i++) //On réinitialise les blocs
        {
            DISQUE.tabblock[i].data[0]='\0';
            DISQUE.tabblock[DISQUE.tabinode[indexInode].block[i]].dispo=0;
        }
        //On réinitialise les informations de l'inode
        free(DISQUE.tabinode[indexInode].block);
        DISQUE.tabinode[indexInode].block=NULL;
        for ( j = 0; j<9;j++)
        {
            DISQUE.tabinode[indexInode].droits[j]='-';
        }
        
        DISQUE.tabinode[indexInode].filetype=0;
        for (j=0;j<3;j++)
        {
            DISQUE.tabinode[indexInode].datecreation[j]=0;
        }
        DISQUE.tabinode[indexInode].taille=0;
        DISQUE.tabinode[indexInode].nbBlockUsed=0;
        strcpy(DISQUE.tabinode[indexInode].nom,"");
        DISQUE.sb.firstAvalaibleBlockIndex=get_firstAB();
        DISQUE.sb.firstAvalaibleInodeIndex=get_firstInode();
        return 0;
    }
}
 
int myopen(char nom[30]) //Primitive qui ouvre un fichier 
{
    int index,fd=-1,i=0;
    if (((index=getInode(nom))==-1)&&((index=mycreat(nom))==-1))
    //Si le fichier n'existe pas et qu'il y a une erreur lorsque qu'on esssaie de le créer
    {
        return fd;
    }
    else
    {
        while(i<TAILLETABFD) //On parcourt la table des fichiers ouverts
        {
            if(TABFD[i].inode==-1) // Dès que l'on trouve une entrée disponible
            {
                //On rentre les informations nécéssaires dans la table des fichiers ouverts
                fd=i; 
                TABFD[i].inode=index;
                TABFD[i].pos_cursor=0;
                break; //et on sort de la boucle
            }
            i++;
        }
    return fd; //On retourne l'entrée de la table correspondante au fichier donné lors de l'appel
    }
}

int myclose(int fd) //ferme un fichier
{
    if (TABFD[fd].inode==-1) //Si l'entrée ne contient pas une inode
    {
        return -1; //Erreur
    }
    else //Sinon on réinitialise l'entrée
    {
        TABFD[fd].inode=-1; 
        return 0;
    }
}

int my_mkdir(char nom[30]) //Primitive qui permet créer un répertoire
{
    if (getInode(nom)!=-1 //Si un fichier ou dossier porte déjà le même nom
    || (((DISQUE.sb).firstAvalaibleBlockIndex==-1) //ou qu'il n'y a pas de bloc disponible
    || ((DISQUE.sb).firstAvalaibleInodeIndex==-1)) // ou d'inode disponible
    )
        return -1; //Erreur
    else //Sinon on le crée de manière analogue à mycreate
    {
        int indexInode;
        time_t t;
        struct tm *timeComp;
        t=time(NULL);
        timeComp = localtime(&t);
        indexInode=DISQUE.sb.firstAvalaibleInodeIndex;
        DISQUE.tabinode[indexInode].filetype=2;
        DISQUE.tabinode[indexInode].droits[0]='r';
        DISQUE.tabinode[indexInode].droits[1]='w';
        DISQUE.tabinode[indexInode].droits[2]='x';
        DISQUE.tabinode[indexInode].datecreation[0]=timeComp->tm_mday;
        DISQUE.tabinode[indexInode].datecreation[1]=timeComp->tm_mon+1;
        DISQUE.tabinode[indexInode].datecreation[2]=timeComp->tm_year+1900;
        DISQUE.tabinode[indexInode].nbBlockUsed++;
        if ( strcmp(nom,"/")!=0)
            add (indexInode, PWD);
        strcpy(DISQUE.tabinode[indexInode].nom,nom);
        DISQUE.tabinode[indexInode].block=(int*)malloc(sizeof(int));
        DISQUE.tabinode[indexInode].block[0]= DISQUE.sb.firstAvalaibleBlockIndex;
        DISQUE.tabinode[indexInode].taille=0;
        DISQUE.tabblock[DISQUE.tabinode[indexInode].block[0]].dispo++;
        DISQUE.sb.firstAvalaibleBlockIndex=get_firstAB();
        DISQUE.sb.firstAvalaibleInodeIndex=get_firstInode();
        
        return indexInode;
    }
}
int my_rmdir(char nom[30]) //Primitive qui permet de supprimer un répertoire
{
    int indexInode = -1,j,i; 
    if (( (indexInode=getInode(nom))!=-1) //Si le fichier existe  
    && (DISQUE.tabinode[indexInode].filetype==2) //et qu c'est bien un répertoire
    && (est_vide_rep(indexInode)) //et qu'il est vide
    ) //On supprime le répertoire de manière analogue à mydelete
    {
        retire_fichier(indexInode);
        for (i=0;i<DISQUE.tabinode[indexInode].nbBlockUsed;i++)
        {
            DISQUE.tabblock[DISQUE.tabinode[indexInode].block[i]].dispo=0;
        }
        
        free(DISQUE.tabinode[indexInode].block);
        DISQUE.tabinode[indexInode].block=NULL;
        for ( j = 0; j<9;j++)
        {
            DISQUE.tabinode[indexInode].droits[j]='-';
        }
        
        DISQUE.tabinode[indexInode].filetype=0;
        for (j=0;j<3;j++)
        {
            DISQUE.tabinode[indexInode].datecreation[j]=0;
        }
        DISQUE.tabinode[indexInode].taille=0;
        DISQUE.tabinode[indexInode].nbBlockUsed=0;
        strcpy(DISQUE.tabinode[indexInode].nom,"");
        DISQUE.sb.firstAvalaibleBlockIndex=get_firstAB();
        DISQUE.sb.firstAvalaibleInodeIndex=get_firstInode();
        retire_fichier(indexInode);
        return 0;
    }
    else 
    {
        return -1; //Sinon Erreur
    } 
}

int add (int IndexF, int IndexD) //Ajoute un fichier à un répertoire
{
    int i, j;
    char str[3];
    if (DISQUE.tabinode[IndexD].filetype==2 && DISQUE.tabinode[IndexF].filetype != 0)
    //On vérifie que le type de fichier à ajouter est défini et que le fichier qui auquel on souhaite l'ajouter est un répertoire
    {
        i=DISQUE.tabinode[IndexD].taille;
        sprintf(str,"%d",IndexF);
        strcat(str,";");
        for ( j=0 ; j<strlen(str);j++)
        {
            DISQUE.tabblock[DISQUE.tabinode[IndexD].block[0]].data[i+j]=str[j];
        }
        DISQUE.tabblock[DISQUE.tabinode[IndexD].block[0]].data[i+j]='\0';
        DISQUE.tabinode[IndexD].taille+=strlen(str);
        DISQUE.tabinode[IndexF].irepParent=IndexD;
        return 1;
    }
    else return 0;
}

int retire_fichier(int IndexF) //Retire un fichier d'un répertoire
{
    int i=0;
    if ( DISQUE.tabinode[IndexF].filetype != 0 ) //On vérifie que le type du fichier est défini
    {
        char str[TAILLEBLOCK];
        while(DISQUE.tabblock[DISQUE.tabinode[DISQUE.tabinode[IndexF].irepParent].block[0]].data[i] != '\0')
        {
            str[i]=DISQUE.tabblock[DISQUE.tabinode[DISQUE.tabinode[IndexF].irepParent].block[0]].data[i];
            i++;
        }
        str[i+1]='\0';
        
        int i;
        char inodeASuppr[3];
        sprintf(inodeASuppr,"%d",IndexF);
        strcat(inodeASuppr,";");
        str_rem(str,inodeASuppr);
        str[strlen(str)]='\0';
        if (strlen(str)>0)
        {
            for ( i = 0 ; i<strlen(str) ; i ++ )
            {
                DISQUE.tabblock[DISQUE.tabinode[DISQUE.tabinode[IndexF].irepParent].block[0]].data[i]=str[i];
            }
            }
        DISQUE.tabblock[DISQUE.tabinode[DISQUE.tabinode[IndexF].irepParent].block[0]].data[strlen(str)]='\0';
        DISQUE.tabinode[DISQUE.tabinode[IndexF].irepParent].taille=strlen(str);
        return 1;
    }
    else return 0; 
}
int link(char name[30], char target[30]) //Primitive qui crée un lien
{
     if  ((((DISQUE.sb).firstAvalaibleBlockIndex==-1) //Si il n'y a pas de bloc disponible
     || (DISQUE.sb).firstAvalaibleInodeIndex==-1) // ou d'inode disponible
     || (getInode(target) < 0) //ou que le fichier cible n'existe pas
     )
    {
        return -1;
    }
    else //Sinon on créer le lien de manière analogue à mycreat
    {
        int indexInode;
        time_t t;
        struct tm *timeComp;
        t=time(NULL);
        timeComp = localtime(&t);
        indexInode=DISQUE.sb.firstAvalaibleInodeIndex;
        DISQUE.tabinode[indexInode].filetype=3;
        DISQUE.tabinode[indexInode].droits[0]='r';
        DISQUE.tabinode[indexInode].droits[1]='w';
        DISQUE.tabinode[indexInode].datecreation[0]=timeComp->tm_mday;
        DISQUE.tabinode[indexInode].datecreation[1]=timeComp->tm_mon+1;
        DISQUE.tabinode[indexInode].datecreation[2]=timeComp->tm_year+1900;
        DISQUE.tabinode[indexInode].nbBlockUsed++;
        strcpy(DISQUE.tabinode[indexInode].nom,name);
        DISQUE.tabinode[indexInode].block=(int*)malloc(sizeof(int));
        DISQUE.tabinode[indexInode].block[0]= DISQUE.sb.firstAvalaibleBlockIndex;
        DISQUE.tabblock[DISQUE.tabinode[indexInode].block[0]].dispo++;
        add(indexInode,PWD);
        DISQUE.sb.firstAvalaibleBlockIndex=get_firstAB();
        DISQUE.sb.firstAvalaibleInodeIndex=get_firstInode();
        
        return indexInode;
    }
}
int unlink(char nom[30]) //Primitive pour supprimer un lien
{
    int indexInode;
    int i,j;
    if ( ((indexInode=getInode(nom)) == -1 ) || (DISQUE.tabinode[indexInode].filetype != 3 ) ) 
    //Si le fichier n'existe pas ou que ce n'est pas un lien
    { 
        return -1;
    }
    else //Sinon on supprime de manière analogue à mydelete 
    {
        retire_fichier(indexInode);
        for (i=0;i<DISQUE.tabinode[indexInode].nbBlockUsed;i++)
        {
            DISQUE.tabblock[i].data[0]='\0';
            DISQUE.tabblock[DISQUE.tabinode[indexInode].block[i]].dispo=0;
        }
        
        free(DISQUE.tabinode[indexInode].block);
        DISQUE.tabinode[indexInode].block=NULL;
        for ( j = 0; j<9;j++)
        {
            DISQUE.tabinode[indexInode].droits[j]='-';
        }
        
        DISQUE.tabinode[indexInode].filetype=0;
        for (j=0;j<3;j++)
        {
            DISQUE.tabinode[indexInode].datecreation[j]=0;
        }
        DISQUE.tabinode[indexInode].taille=0;
        DISQUE.tabinode[indexInode].nbBlockUsed=0;
        strcpy(DISQUE.tabinode[indexInode].nom,"");
        DISQUE.sb.firstAvalaibleBlockIndex=get_firstAB();
        DISQUE.sb.firstAvalaibleInodeIndex=get_firstInode();
        return 0;
    }
}


int est_vide_rep(int indexInode) //Vérifie qu'un répertoire est vide
//Renvoie 1 si le répertoire est vide 0 sinon et -1 en cas d'erreur
{
    if ((indexInode==-1) || (DISQUE.tabinode[indexInode].filetype!=2)) 
    //Si l'index de l'inode ne correspond à aucune inode ou que le fichier n'est pas un répertoire
        return -1;
    else if (DISQUE.tabblock[DISQUE.tabinode[indexInode].block[0]].data[0]=='\0')
    //Si le repertoire n'a aucun fils 
        return 1;
    else 
        return 0;
        
}

