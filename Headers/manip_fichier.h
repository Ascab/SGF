#ifndef MANIP_FICHIER_H
#define MANIP_FICHIER_H

#include "include.h"

int mycreat(char nom[30]);
int mydelete (char nom[30]);
int myopen (char nom[30]);
int myclose (int fd);
int my_mkdir(char nom[30]);
int my_rmdir(char nom[30]);
int add (int IndexF, int IndexD);
int retire_fichier(int IndexF);
int link(char name[30], char target[30]);
int unlink(char nom[30]);
int est_vide_rep(int indexInode);
#endif