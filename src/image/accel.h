#ifndef ACCEL_H
#define ACCEL_H
#include <stdlib.h>
extern int depth,bdep,b_pad,translation;
extern unsigned int rmask,gmask,bmask,rsh,bsh,gsh;
extern unsigned int red_rsh,green_rsh,blue_rsh,red_lsh,green_lsh,blue_lsh;
#ifdef __cplusplus
extern "C" char *im_alloc_true(int l,int h);
extern "C" void im_memcopy(char*,char*,int);
extern "C" void im_setzero(char*,int);
extern "C" unsigned int im_get_linew_true(unsigned int w);
#else
extern  char *im_alloc_true(int l,int h);
extern  void im_memcopy(char*,char*,int);
extern  void im_setzero(char*,int);
#endif
extern void  (*im_coding)(unsigned int,unsigned int,unsigned int,char**);
extern void im_truetruecoding(unsigned int ir,unsigned int ig,unsigned int ib,char **imdata);
extern void im_truenocoding(unsigned int ir,unsigned int ig,unsigned int ib,char **imdata);

#endif
/* ------------ End of file -------------- */

