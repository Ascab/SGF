#ifndef STRUCT_H
#define STRUCT_H

#include "define.h"

    typedef struct
    {
     int inode;
     int pos_cursor;
    }Fd;
    typedef struct 
    {
        char dispo; //0=disponible, 1=occupé
        char data[TAILLEBLOCK];
    }blocks;
    
    typedef struct 
    {
        char droits[9]; 
        int *block; // liste des blocks occupés par le fichier
        int filetype; // 0 non defini , 1 classique  , 2 répertoire , 3 lien 
        int datecreation[3]; // Stocke le jour, le mois et l'année
        int taille; // taille totale du fichier
        int nbBlockUsed;
        char nom[30];
        int irepParent;
    }inode;
    
    typedef struct
    {
        int partitionSize;
        int nbBlocks;
        int firstAvalaibleBlockIndex;
        int firstAvalaibleInodeIndex;
    }superBlock;
    
    typedef struct 
    {
        inode tabinode[NBINODE];
        blocks tabblock[NBBLOCK];
        superBlock sb;
    }disk;
#endif