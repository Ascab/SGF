#ifndef MANIP_DISK_H
#define MANIP_DISK_H    

int get_firstAB(void);
int get_firstInode(void);
int getInode(char nom[30]);
int myReallocBlock(inode* a);

#endif