/****************************************************************************
*  Copyright (C) 1996-98 by Leo Khramov
*  email:     leo@xnc.dubna.su
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*            
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
 ****************************************************************************/
#include "image.h"
#include "im_exp.h"

#define ACCEL_H
#define ABS(x)                ((x) > 0 ? (x) : (-(x)))
extern "C"  void  (*im_coding)(unsigned int,unsigned int,unsigned int,char**);
extern  "C" void im_truetruecoding(unsigned int ir,unsigned int ig,unsigned int ib,char **imdata);
extern  "C" void im_truenocoding(unsigned int ir,unsigned int ig,unsigned int ib,char **imdata);
XColor col;
int vclass;
Visual *vis;
int pcd_resolution=3;
int depth,bdep,translation,b_pad=1;
int fromMem=0;
Colormap defc;                                                                          
PICINFO im;
char *trans;
uint rmask,gmask,bmask,rsh,bsh,gsh;
uint red_rsh,green_rsh,blue_rsh,red_lsh,green_lsh,blue_lsh;
XColor *XColorTable;
Colormap lcmp;
int ptnum,cmptype;
char check[256];
int  conv24;

void (*im_coding256)(char **,int);
void im_256truecoding(char**,int);
void im_256nocoding(char**,int);
void im_256256coding(char**,int);

unsigned long *global_pal=NULL;

#ifdef PSEUDOCOLOR                     
unsigned long XPixelTable[256];
unsigned long Xfreepix[256];
byte rpart[256];
byte gpart[256];
byte bpart[256];
#endif                     
int im_getpixels(unsigned long* pt)
{
#ifdef PSEUDOCOLOR                     
 if(vclass==PseudoColor)
 for(int i=0;i<ptnum;i++)
  pt[i]=Xfreepix[i];
#endif
 return 0;
}

void im_clear_pic()
{
 int i;
 for(i=0;i<768;i++)
  im.pal[i]=0;
 for(i=0;i<256;i++)
   check[i]=0;
}

Colormap im_choose_cmp()
{
 Colormap cmp;
 if(cmptype==CMP_OWN && vclass==PseudoColor)
 {
  cmp=XCreateColormap(disp,DefaultRootWindow(disp),vis,AllocNone);
  lcmp=cmp;
  XSync(disp,0);
 } else return defc;
 return cmp;
}

void im_freecmp(Colormap XColorMap,unsigned long *pt,int iptnum)
{
#ifdef PSEUDOCOLOR                     
  if(XColorMap==0) XColorMap=defc;
  if(vclass==PseudoColor && XColorMap==defc)
            XFreeColors(disp, XColorMap, pt, iptnum, 0L);
  if(XColorMap!=defc) XFreeColormap(disp,XColorMap);
#endif
}

Colormap im_get_colormap()
{
 if(cmptype==CMP_OWN && vclass==PseudoColor) return lcmp;
 else return 0;
}

#ifdef PSEUDOCOLOR                     
int im_get_checked()
{
 int i,j=0;
  return 256;
 if(translation)
 {
   for(i=0;i<256;i++)
    if(check[i]) j++;
   return j;
  }
  return 256;
}

void im_separate_colors()
{
 int i3;
 for(int i=0;i<256;i++)
 {
  i3=i*3;
  rpart[i]=im.pal[i3];
  gpart[i]=im.pal[i3+1];
  bpart[i]=im.pal[i3+2];
 }
}
 
void im_union_colors()
{
 int i3;
 for(int i=0;i<256;i++)
 {
  i3=i*3;
  im.pal[i3]=rpart[i];
  im.pal[i3+1]=gpart[i];
  im.pal[i3+2]=bpart[i];
 }
}
 
void im_add_cmp_entry(unsigned long ent)
{
  Xfreepix[ptnum++]=ent;
  if(global_pal)
          global_pal[ptnum-1]=ent;
}

void im_alloc_closest_cmp(Colormap XColorMap,int ColorMapSize=256)
{
    int i, j, Index = 0, Count = 0, XNumOfColors;
    unsigned long D, Distance, Red, Green, Blue;
    int Failed = False;
    XColor *XOldColorTable;
    ptnum=0;
    for (i = 0; i < 256; i++) {
        if (i < ColorMapSize) {          /* Prepere color entry in X format. */
            XColorTable[i].red = im.pal[i*3] << 8;
            XColorTable[i].green = im.pal[i*3+1] << 8;
            XColorTable[i].blue = im.pal[i*3+2] << 8;
            XColorTable[i].flags = DoRed | DoGreen | DoBlue;
            XPixelTable[i] =(unsigned long)-1;                       /* Not allocated yet. */
        }
        else
            XPixelTable[i] = 0;    /* Put reasonable color for out of range. */
    }

    for (i = 0; i < ColorMapSize; i++)              /* Allocate the colors from X: */
        if (XAllocColor(disp, XColorMap, &XColorTable[i]))
  {
            XPixelTable[i] = XColorTable[i].pixel;
     im_add_cmp_entry(XPixelTable[i]);
   }
        else
            Failed = True;
    if(ptnum<ColorMapSize/2 && cmptype==CMP_AUTO && Failed==True) 
    {
     cmptype=CMP_OWN;
     XColorMap=im_choose_cmp();
     im_alloc_closest_cmp(XColorMap,ColorMapSize);
     return;
    }
    if (Failed) {
        XNumOfColors = DisplayCells(disp,DefaultScreen(disp));
        XOldColorTable = (XColor *) malloc(sizeof(XColor) * XNumOfColors);
        for (i = 0; i < XNumOfColors; i++) XOldColorTable[i].pixel = i;
        XQueryColors(disp, XColorMap, XOldColorTable, XNumOfColors);
        
        for (i = 0; i < ColorMapSize; i++) {
            /* Allocate closest colors from X: */
            if (XPixelTable[i] == -1) {      /* Failed to allocate this one. */
                Distance = 0xffffffff;

                Red = XColorTable[i].red;
                Green = XColorTable[i].green;
                Blue = XColorTable[i].blue;

                for (j = 0; j < XNumOfColors; j++) {
                    /* Find the closest color in 3D RGB space using L1 norm. */
                    if ((D = ABS(Red - XOldColorTable[j].red) +
                             ABS(Green - XOldColorTable[j].green) +
                             ABS(Blue - XOldColorTable[j].blue)) < Distance) {
                        Distance = D;
                        Index = j;
                    }
                }
                XPixelTable[i] = Index;

                Count++;
            }
        }
        free(XOldColorTable);
    }
   for(i=0;i<ColorMapSize;i++)
     trans[i]=(XPixelTable[i]& 0xff);
}


void im_alloccmp(Colormap XColorMap,int ColorMapSize=256)
{
    int Strip, Msk, i;
    ptnum=0;
    im_alloc_closest_cmp(XColorMap,ColorMapSize);
    return;
    for (i = 0; i < 256; i++)
        XPixelTable[i] = 0;           /* Put reasonable color for out of range. */

    for (Strip = 0, Msk = 0xff; Strip < 8; Strip++, Msk <<= 1) {
        for (i = 0; i < ColorMapSize; i++) {
            /* Prepere color entry in X format. */
            XColorTable[i].red = (im.pal[i*3] & Msk) << 8;
            XColorTable[i].green = (im.pal[i*3+1] & Msk) << 8;
            XColorTable[i].blue = (im.pal[i*3+2] & Msk) << 8;
            XColorTable[i].flags = DoRed | DoGreen | DoBlue;
            if (XAllocColor(disp,XColorMap , &XColorTable[i]))
  {
                XPixelTable[i] = XColorTable[i].pixel;
                im_add_cmp_entry(XPixelTable[i]);
  }
            else
                break;
        }
        if (i < ColorMapSize)
  {
     ptnum=0;
            XFreeColors(disp, XColorMap, XPixelTable, i, 0L);
   }
        else
            break;
    }

    if (Strip == 8)
        fprintf(stderr,"Can not display the image - not enough colors available.\n");
  else
   for(i=0;i<ColorMapSize;i++)
     trans[i]=(XPixelTable[i]& 0xff);

}


XImage *im_process_pseudo_256(int cmpsize=256)
{
 char *imdata;
 XImage *xim;
 int imlen=im.w*im.h,i;
 Colormap cmp;
 if(translation==0)
   {
   cmp=im_choose_cmp();
    trans=new char[256*bdep];
    im_alloccmp(cmp,cmpsize);
    for(i=0;i<imlen;i++)
     im.pic[i]=trans[im.pic[i]];
  }
     imdata=(char*)im.pic;
     delete trans;
     if((xim=XCreateImage(disp,vis,depth,ZPixmap,0,imdata,im.w,im.h,8,0))==NULL)
     {
       delete imdata;
       fprintf(stderr,"Image Engine Error: XCreateImage\n");
       return NULL;
     }
     xim->byte_order=MSBFirst;
      return xim;
}  
#endif

char *getpicinfo()
{
  char *tmp=new char[strlen(im.fullInfo)+1];
  strcpy(tmp,im.fullInfo);
  return tmp;
}

void do_masks(uint mask,uint &lsh,uint &rsh)
{
  lsh=rsh=0;
  while((mask & 1)==0) {lsh++;mask>>=1;};
  while(mask & 1) {rsh++;mask>>=1;};
  rsh=8-rsh;
}

void im_reset_coding()
{
 im_coding256=im_256nocoding;
 translation=0;
}

void im_set_coding()
{
 im_coding256=im_256truecoding;
 translation=1;
}

long cmap_distance[256];
int XCmapNumColors;
XColor *XCmapColorTable=NULL;
Colormap XCmapCmap;

long im_calc_weight(unsigned short red,
                               unsigned short green,
                               unsigned short blue)
{
  double r=(double)red;
  double g=(double)green;
  double b=(double)blue;
  return (long)sqrt(r*r+g*g+b*b);
}

void im_init_cmap(Colormap XColorMap)
{
  
    int i, XNumOfColors;
    int screen=DefaultScreen(disp);
    
        depth=DefaultDepth(disp,screen);
        vis=DefaultVisual(disp,screen);
        defc=DefaultColormap(disp,screen);
        vclass=vis->c_class;
        XNumOfColors = DisplayCells(disp,DefaultScreen(disp));
        if(XCmapColorTable == NULL)
          XCmapColorTable = (XColor *) malloc(sizeof(XColor) * XNumOfColors);
        for (i = 0; i < XNumOfColors; i++) 
                XCmapColorTable[i].pixel = i;
        XQueryColors(disp, XColorMap, XCmapColorTable, XNumOfColors);
        
        for(i=0;i< XNumOfColors;i++)
        {
             cmap_distance[i]=im_calc_weight(XCmapColorTable[i].red,
                                            XCmapColorTable[i].green,
                                            XCmapColorTable[i].blue);
        }
        XCmapNumColors=XNumOfColors;
        XCmapCmap=XColorMap;
}

int im_find_closest_color(XColor* orig, XColor* tocol)
{
  unsigned long weight,distance, Red, Green, Blue, D;
  int idx=0,i;
  if(vclass==TrueColor)
  {
     *tocol=*orig;
     return 0;
  }
  Red=orig->red;
  Green=orig->green;
  Blue=orig->blue;
  distance=0xffffffff;
  for(i=0;i<XCmapNumColors;i++)
  {
      if ((D = ABS(Red - XCmapColorTable[i].red) +
           ABS(Green - XCmapColorTable[i].green) +
           ABS(Blue - XCmapColorTable[i].blue)) < distance) 
              {
                  distance = D;
                  idx = i;
              }
  }
  *tocol=XCmapColorTable[idx];
  tocol->flags = DoGreen | DoRed | DoBlue;
  return 1;
}

int im_find_closest_color2(XColor* orig, XColor* tocol)
{
  long weight,distance;
  int idx=0,i;
  if(vclass==TrueColor)
  {
     *tocol=*orig;
     return 0;
  }
  weight=im_calc_weight(orig->red, orig->green, orig->blue);
  distance=ABS(cmap_distance[idx]-weight);
  for(i=1;i<XCmapNumColors;i++)
    if(distance>ABS(cmap_distance[i]-weight))
    {
            idx=i;
            distance=ABS(cmap_distance[i]-weight);
    }
  *tocol=XCmapColorTable[idx];
  tocol->flags = DoGreen | DoRed | DoBlue;
  return 1;
}

Status im_AllocColor(Display* d, Colormap cmap, XColor *cl)
{
    XColor cl2;
    Status st;
    if((st=XAllocColor(disp, cmap, cl))!=0)
       return st; 
    if(cmap!=XCmapCmap || XCmapColorTable==NULL)
      im_init_cmap(cmap);
    im_find_closest_color2(cl, &cl2);
    *cl=cl2;
    return XAllocColor(disp, cmap, cl);
}

Status (*pim_AllocColor)(Display*, Colormap, XColor*)=im_AllocColor;

void init_imagengine()
{
 XImage* xim;
 XPixmapFormatValues *pixformat;
 int i, count;
 int screen=DefaultScreen(disp);

 depth=DefaultDepth(disp,screen);
 vis=DefaultVisual(disp,screen);
 defc=DefaultColormap(disp,screen);
 vclass=vis->c_class;
 fprintf(stderr,   "**********Image Engine**********\n");
 fprintf(stderr,"*                              *\n");
 switch(vclass)
 {
 
 case TrueColor: 
    pixformat = XListPixmapFormats(disp, &count);
    for (i=0; i<count; i++) {
       if (pixformat[i].depth == depth) {
          bdep = (pixformat[i].bits_per_pixel+7) / 8;
          b_pad = pixformat[i].scanline_pad / 8;
          break;
       }
    }
    XFree(pixformat);
    translation=1;
    if((xim=XCreateImage(disp,vis,depth,ZPixmap,0,NULL,10,10,b_pad*8,0))==NULL)
     {
       fprintf(stderr,"Image Engine Error: XCreateImage\n");
       return;
     }
     rmask=xim->red_mask;
     gmask=xim->green_mask;
     bmask=xim->blue_mask;
#ifdef PCD_SUPPORT
    x11_create_lut(rmask,gmask,bmask);
#endif    
    do_masks(rmask,red_lsh,red_rsh);
    do_masks(gmask,green_lsh,green_rsh);
    do_masks(bmask,blue_lsh,blue_rsh);
    im_coding=im_truetruecoding;
    im_coding256=im_256truecoding;
    fprintf(stderr,"*Visual:  TrueColor            *\n");
    fprintf(stderr,"*Depth:   %-3d (%d bytes/pixel)  *\n",depth,bdep);
    fprintf(stderr,"*RGB:     %d:%d:%d                *\n",8-red_rsh,8-green_rsh,8-blue_rsh);
    fprintf(stderr,"*Colors:  %-9d            *\n",1<<depth);
    fprintf(stderr,"*Images:  GIF,JPEG,PCX         *\n");
    XFree(xim);
    break;
  case PseudoColor:
#ifdef PSEUDOCOLOR                     
    bdep=(depth+7)/8;
    b_pad=1;
    translation=1;
    XColorTable=new XColor[256];
    im_init_cmap(defc);
    im_coding256=im_256256coding;
    im_coding=im_truenocoding;
    fprintf(stderr,"*Visual:  PseudoColor          *\n");
    fprintf(stderr,"*Depth:   %-3d (%d bytes)        *\n",depth,bdep);
    fprintf(stderr,"*Colors:  %-9d            *\n",1<<depth);
    fprintf(stderr,"*Images:  GIF,JPEG,PCX         *\n");
    break;
#else
    fprintf(stderr,"*Visual:  PseudoColor           *\n");
    fprintf(stderr,"*Not supported                 *\n");
    vclass=DirectColor;
    break;
#endif
   case DirectColor:
    fprintf(stderr,"*Visual:  DirectColor          *\n");
    fprintf(stderr,"*Not supported                 *\n");
    break;
  }                                                                              
    fprintf(stderr,"*                              *\n");
    fprintf(stderr,"******** (c) Leo 96-98 *********\n");
}

//return -1 if not detected
int autodetect(char *f)
{
 int fp=open(f,O_RDONLY);
 if(fp==-1) return -1;
 u_char buf[40];
 if(read(fp,buf,30)<30)
 {
 close(fp);
 return -1;
 }
 close(fp);
 if(buf[0]=='G' && buf[1]=='I' && buf[2]=='F') return F_GIF;
#ifdef TIFFSUPPORT
 else if(buf[0]==buf[1] && (buf[0]=='I' || buf[0]=='M')) return F_TIFF;
#endif
#ifdef PCXSUPPORT
 else if(buf[0]==0xa && buf[1]<=5 && buf[2]==1) return F_PCX;
#endif
#ifdef BMPSUPPORT
 else if(buf[0]=='B' && buf[1]=='M') return F_BMP;
#endif
#ifdef XPMSUPPORT
 else if (strncmp((char*)buf, "/* XPM */", (size_t) 9)==0) return F_XPM;
#endif
#ifdef XBMSUPPORT
 else if (strncmp((char*)buf,"#define", (size_t) 7)==0)  return F_XBM;
#endif
#ifdef TGASUPPORT
 else if (buf[0]==0 && buf[1]==0 &&
           buf[2]==2 && buf[3]==0 &&
           buf[4]==0 && buf[5]==0 &&
           buf[6]==0 && buf[7]==0)  return F_TARGA;
#endif
#ifdef PNG_SUPPORT
 else if (strncmp((char*)buf+1,"PNG", (size_t) 3)==0)  return F_PNG;
#endif

 else if(buf[0]==0xff && buf[1]==0xd8 && buf[2]==0xff) return F_JPEG;

#ifdef PCD_SUPPORT
 else 
 while(*f!=0)
 {
  if(*f=='.')
   if(strncmp(f,".pcd",4)==0 || strncmp(f,".PCD",4)==0)
    return F_PCD;
  f++;
  }
#endif
 return -1;
}


void im_init_translation()
{
   int i,j,i3,i2;
   Colormap cmp;
   if(translation)
   {
   trans=new char[256*bdep];
#ifdef PSEUDOCOLOR                     
    if(vclass==PseudoColor) 
    {
     cmp=im_choose_cmp();
     im_alloccmp(cmp);
     return;
    };
#endif
   col.flags=DoRed | DoGreen | DoBlue;
   for(i=0;i<256;i++)
    {
     i3=i*3;i2=bdep*(i+1);
     col.red=(unsigned short)(im.pal[i3])<<8;
     col.green=(unsigned short)(im.pal[i3+1])<<8;
     col.blue=(unsigned short)(im.pal[i3+2])<<8;
     XAllocColor(disp,defc,&col);
     if(global_pal)
       global_pal[i]=col.pixel;
     for(j=bdep-1;j>=0;j--)
      {
       trans[i2-j-1]=(char)((col.pixel>>(j<<3))& 0xff);
       }
     }
     }
}


XImage *LoadXImage(char *file,int cmptype,int type)
{
 int i,j,l,lw,x,y;
 ulong mask;
 char *imdata,*imdat;
 int ind;
 XImage *xim;
 ::cmptype=cmptype;
 if(vclass==DirectColor) return NULL;
#ifndef PSEUDOCOLOR
 if(vclass==PseudoColor) return NULL;
#endif
 if(type==F_AUTO) type=autodetect(file);
  switch(type)
  {
    case F_GIF:
        translation=0;
        im_clear_pic();
	im_load_gif(file,&im,fromMem);
	break;

    case F_JPEG:
#ifdef JPEGSUPPORT
        im_clear_pic();
        if(vclass==PseudoColor) 
	  translation= 0;
        else 
	  translation= 1;
        if(LoadJFIF(file,&im)) 
	  break;
	else 
	{
	  fprintf(stderr,"Image Engine: LoadJpeg\n");
	  return NULL;
	};
#else
         return NULL;
#endif
    case F_TIFF:
#ifdef TIFFSUPPORT
        im_clear_pic();
         translation= 0;
         if(LoadTIFF(file,&im)) break;else {fprintf(stderr,"Image Engine: LoadTiff\n");return NULL;};
#else
         return NULL;
#endif
    case F_PNG:
#ifdef PNG_SUPPORT
        im_clear_pic();
         translation= 0;
         if(LoadPNG(file,&im)) break;else {fprintf(stderr,"Image Engine: LoadPNG\n");return NULL;};
#else
         return NULL;
#endif
    case F_PCX:
#ifdef PCXSUPPORT
        im_clear_pic();
         translation= 0;
         if(LoadPCX(file,&im)) break;else {fprintf(stderr,"Image Engine: LoadPcx\n");return NULL;};
#else
         return NULL;
#endif
    case F_PCD:
#ifdef PCD_SUPPORT
        im_clear_pic();
         if(vclass==PseudoColor) translation=0;
         else translation= 1;
         if(LoadPCD(file,&im)) break;else {fprintf(stderr,"Image Engine: LoadPcd\n");return NULL;};
#else
         return NULL;
#endif
    case F_XBM:
#ifdef XBMSUPPORT
        im_clear_pic();
         translation=0;
         if(LoadXBM(file,&im)) break;else {fprintf(stderr,"Image Engine: LoadXBM\n");return NULL;};
#else
         return NULL;
#endif
    case F_XPM:
#ifdef XPMSUPPORT
        im_clear_pic();
         translation=0;
         if(LoadXPM(file,&im)) break;else {fprintf(stderr,"Image Engine: LoadXPM\n");return NULL;};
#else
         return NULL;
#endif
    case F_BMP:
#ifdef BMPSUPPORT
        im_clear_pic();
         translation= 1;
         if(LoadBMP(file,&im)) break;else {fprintf(stderr,"Image Engine: LoadBmp\n");return NULL;};
#else
         return NULL;
#endif
    case F_TARGA:
#ifdef TGASUPPORT
        im_clear_pic();
         translation=0;
         if(LoadTarga(file,&im)) break;else {fprintf(stderr,"Image Engine: LoadTGA\n");return NULL;};
#else
         return NULL;
#endif
    default: return NULL;
   };
#ifdef PSEUDOCOLOR                     
  if(vclass==PseudoColor)
  {
   if(im.type==PIC8)
   return im_process_pseudo_256(im_get_checked());
   else {
             im_convert_true_to_pseudo(im,256);
             translation=0;
             return im_process_pseudo_256(conv24!=CONV24_FAST ? 230:256);
            }
   }
#endif
  if(im.type==PIC8 && vclass==TrueColor)
  {
   if(translation==0)
   {
     translation=1;
     im_init_translation();
     translation=0;
     lw = im.w * bdep;
     if (lw % b_pad)
        lw += b_pad - lw % b_pad;
     l=im.h*im.w;
     imdata=new char[lw*im.h];
     i = 0;
     for (y=0; y<im.h; y++) {
        l = y * lw;
        for (x=0; x<im.w; x++) {
           ind = im.pic[i++] * bdep;
           for (j=0; j<bdep; j++)
              imdata[l++] = trans[ind++];
        }
     }
     free(im.pic);
     }
   else
     imdata=(char*)im.pic;
     delete trans;
     if((xim=XCreateImage(disp,vis,depth,ZPixmap,0,imdata,im.w,im.h,b_pad*8,0))==NULL)
     {
       delete imdata;
       fprintf(stderr,"Image Engine Error: XCreateImage\n");
       return NULL;
     }
     xim->byte_order=MSBFirst;
      return xim;
  } else if(im.type==PIC24 && vclass==TrueColor)
  {
   if(translation==0)
   {
     lw = im.w * bdep;
     if (lw % b_pad)
        lw += b_pad - lw % b_pad;
     imdata = new char[lw*im.h];
     mask = 0xff;
     i = 0;
     for (y=0; y < im.h; y++) {
        imdat = imdata + y * lw;
        for (x=0; x < im.w; x++, i+=3)
           im_truetruecoding(im.pic[i],im.pic[i+1],im.pic[i+2],&imdat);
     }
    free(im.pic);
    }
   else
     imdata=(char*)im.pic;
     if((xim=XCreateImage(disp,vis,depth,ZPixmap,0,imdata,im.w,im.h,b_pad*8,0))==NULL)
     {
       fprintf(stderr,"Image Engine Error: XCreateImage\n");
       delete imdata;
       return NULL;
     }
     xim->byte_order=MSBFirst;
      return xim;
     }
     return NULL;
}

XImage* encoding256(PICINFO& im,int ncol)
{
 int i,j,l,k,lw;
 ulong mask;
 char *trans;
 char *imdata;
 int ind,delta;
 XImage *xim;
   trans=new char[256*bdep];
   col.flags=DoRed | DoGreen | DoBlue;
   ptnum=0;
   for(i=0;i<ncol;i++)
    {
     col.red=(unsigned short)(im.pal[i*3])<<8;
     col.green=(unsigned short)(im.pal[i*3+1])<<8;
     col.blue=(unsigned short)(im.pal[i*3+2])<<8;
     XAllocColor(disp,defc,&col);
#ifdef PSEUDOCOLOR
     if(vclass==PseudoColor)
      im_add_cmp_entry(col.pixel);
#endif
     mask=bdep*(i+1);
     for(j=bdep-1;j>=0;j--)
      {
       trans[mask-j-1]=(char)((col.pixel>>(j<<3))& 0xff);
       }
     }
   l=im.h*im.w;
   lw = im.w * bdep;
   if (lw % b_pad) {
      lw += b_pad - lw % b_pad;
   }
   imdata=new char[im.h*lw];
   for (i=0; i<im.h; i++) {
      for (k=0; k<im.w; k++) {
         ind = im.pic[i*im.w+k];
         ind *= bdep;
         delta = i * lw + k * bdep;
         for(j=0;j<bdep;j++)
            imdata[delta+j]=trans[ind+j];
      }
   }
     delete trans;
     if((xim=XCreateImage(disp,vis,depth,ZPixmap,0,imdata,im.w,im.h,b_pad*8,0))==NULL)
     {
       delete imdata;
       fprintf(stderr,"Image Engine Error: XCreateImage\n");
       return NULL;
     }
     xim->byte_order=MSBFirst;
      return xim;
}

void im_256truecoding(char **data,int ind)
{
   char *ltrans=trans+ind*bdep;
   for(int i=0;i<bdep;i++)
      *((*data)++)=*(ltrans++);
}

void im_256nocoding(char **data,int ind)
{
   *((*data)++)=ind;
}

void im_256256coding(char **data,int ind)
{
   *((*data)++)=trans[ind];
}

Pixmap LoadPixmap(char *fname,int type,int& l,int& h,int cmptype)
{
 XImage *iim;
 Pixmap im;
 GC gcw;
  iim=LoadXImage(fname,cmptype,type);
  if(iim)
  {
   im=XCreatePixmap(disp,DefaultRootWindow(disp),
     iim->width,iim->height,depth);
   gcw=XCreateGC(disp,im,0,NULL);
   l=iim->width;h=iim->height;
   XPutImage(disp,im,gcw,iim,0,0,0,0,l,h);
   XFreeGC(disp,gcw);
   delete iim->data;iim->data=NULL;XDestroyImage(iim);
   return im;
   }
 return 0;
}  
 
Pixmap LoadPixmapFromMem(char *fname,int type,int& l,int& h,
			 int cmptype,int siz)
{
 XImage *iim;
 Pixmap im;
 GC gcw;
  fromMem=siz;
  iim=LoadXImage(fname,cmptype,type);
  fromMem=0;
  if(iim)
  {
   im=XCreatePixmap(disp,DefaultRootWindow(disp),
     iim->width,iim->height,depth);
   gcw=XCreateGC(disp,im,0,NULL);
   l=iim->width;h=iim->height;
   XPutImage(disp,im,gcw,iim,0,0,0,0,l,h);
   XFreeGC(disp,gcw);
   delete iim->data;iim->data=NULL;XDestroyImage(iim);
   return im;
   }
 return 0;
}  

void im_populate_pallete(unsigned long* pal)
{
        global_pal=pal;
}

void im_clear_global_pal()
{
        global_pal=NULL;
}  
                                                                  
