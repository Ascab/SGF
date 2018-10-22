#ifndef CMD_H
#define CMD_H

int getArgc ( char* , char c);
void cmdmkdir (int argc , char* argv[]);
void cmdrmdir(int argc, char *argv[]);
void cmdls(int argc, char *argv[]);
void cmdtouch(int argc, char *argv[]);
void cmdrm(int argc, char *argv[]);
void cmdcd(int argc, char *argv[]);
void cmddf(int argc, char *argv[]);
void cmdcat(int argc, char *argv[]);
void cmdecho(int argc, char *argv[]);
void cmdmv(int argc , char *argv[]);
void cmdchmod(int argc,char *argv[]);
int verif_droit(char *chaine);
void cmdcp(int argc, char *argv[]);
void wecho(int argc,char *argv[]);

#endif