#ifndef WELLTRIS_H
#define WELLTRIS_H
/*
# GENERIC X-BASED WELLTRIS
#
#      welltris.h
#
# 
# C++ implementation by Leo Khramov, leo@xnc.dubna.su
#
*/

/*** random number generator ***/

/* insert your favorite */
extern void SetRNG(long);
extern long LongRNG();
#define SRAND(X) SetRNG((long) X)
#define LRAND() LongRNG()

#define NRAND(X) ((int)(LRAND()%(X)))

/*** macros ***/

#define	ZLIM(X, Y)	(((int) X) < (Y) ? ((int) X) : (Y))

/*** constants ***/

#ifndef	SCOREFILE
#define	SCOREFILE	"welltris.scores"
#endif
#ifndef	POLYOMINOFILE
#define	POLYOMINOFILE	"polyomino.dat"
#endif

#define XWELL_DIR       ".xwelltris"
#define RC_FILE         "xwelltris.rc"

#define	BVOLUME		-90
#define NUM_FLASHES	5

#define SAMPLERATE      5000000
#define FLUSH_DELAY     100000

#define	MILLION		1000000
#define PLAYER_NAME_LEN 15
#define TOP_NINE        9    
#define L_DATE          11

#define	NUM_LEVELS	19



#define MAX_WIDTH	8
#define MAX_DEPTH	12
#define MAX_SIDES	4
#define MAX_PERIMETER	(MAX_SIDES*MAX_WIDTH)


#define EMPTY		(-1)
#define NODIAG		0 /* Normal, ie no diagonals */
#define DIAGONAL	1 /* Normal and diagonal */
#define NOMIX		0 /* Do not mix lowered number squares with current */
#define MIXED		1
#define MAX_MODES	2
#define NONE		0 /* No rotation or reflection */
#define NOREFL		1 /* Normal, no reflection allow rotation */
#define ALL		2 /* Allow reflection or rotation */
#define MAX_TYPES       3
#define DEFAULT_SQUARES	4
#define MIN_SQUARES	2 /* Why not 1 you say? */
#define MAX_SQUARES	6
#define TOTAL_SQUARES   (MAX_SQUARES - MIN_SQUARES + 1)
#define DEFAULT_DIAGONAL	3
#define MIN_DIAGONAL	2
#define MAX_DIAGONAL	3
#define MAX_POLYOMINOES	216
#define MAX_START_POLYOMINOES	55
#define JUMPIN		0
#define GRADUAL		1
#define LEGAL(n,g,m,o)	((n>=MIN_SQUARES+m)&&\
			(n<=((g)?MAX_DIAGONAL:MAX_SQUARES)-o))
#define THRESHOLD(x)	((x)*(x)*4-(x)+1)
#define CHECKUP(x)	((x)%8)
#define CHECKRIGHT(x)	(((x)/8)*8+(x)%4)
#define CHECKDOWN(x)	(((x)/4)*4+(x)%2)
#define CHECKLEFT(x)	(((x)/2)*2)

/*** types ***/

enum Actions {
             NOTHING, 
	     FALL, 
	     DROP, 
	     LEFT, 
	     RIGHT, 
	     ROTATE, 
	     REFLECT, 
	     PAUSE_GAME,
             END_GAME,
	     OUTER_ROTATION,
	     INNER_ROTATION,
	     LEVEL_UP,
	     LEVEL_DOWN
};

#define SYSCOLORS       7
#define MAX_GAME_COLORS (MAX_START_POLYOMINOES+SYSCOLORS)
#define COLOR_DELTA     MAX_START_POLYOMINOES
#define FREE_COLORS     (255 - 211 - SYSCOLORS)
enum Colors {BackColor=COLOR_DELTA,
	     GridColor=COLOR_DELTA+1,
	     GridColor2=COLOR_DELTA+2,
	     FreezeColor=COLOR_DELTA+3, 
	     FlashColor=COLOR_DELTA+4,
             BonusColor=COLOR_DELTA+5,
             BonusColor2=COLOR_DELTA+6};


enum Images { 
             imNone=-1,
             imBoardBG=0, 
	     imIntroBG=1, 
	     imCuts=2, 
	     imScoreBG=3, 
	     imFont1=4 };


#define MAX_IMAGES        20


#define NEXT_CENTER_X     700
#define NEXT_CENTER_Y     519
#define NEXT_BOX_SIZE     15
#define NEXT_X_SIZE       120
#define NEXT_Y_SIZE       120

//scores region in the game
#define SCORE_REG_X       638
#define SCORE_REG_Y       112
#define SCORE_REG_L       126
#define SCORE_REG_H       30

//Lines region in the game
#define LINES_REG_X       775-5*11
#define LINES_REG_Y       172
#define LINES_REG_L       55
#define LINES_REG_H       17

//Level up region in the game
#define LEVUP_REG_X       775-4*11
#define LEVUP_REG_Y       212
#define LEVUP_REG_L       44
#define LEVUP_REG_H       17

//Level region in the game
#define LEVEL_REG_X       775-3*11
#define LEVEL_REG_Y       252
#define LEVEL_REG_L       33
#define LEVEL_REG_H       17

//Challengers region in the game
#define CHAL1_REG_X       613
#define CHAL1_REG_Y       333
#define CHAL2_REG_X       613
#define CHAL2_REG_Y       358
#define CHAL3_REG_X       613
#define CHAL3_REG_Y       382
#define CHAL_REG_L        12*15
#define CHAL_REG_H        17

//Top nine text
#define TOPNINE_TXT_X     30
#define TOPNINE_TXT_Y     140
#define TOPNINE_TXT_L     750
#define TOPNINE_TXT_H     22
#define TOPNINE_TXT_STEPY 42

//challengers display name len in symbols
#define CHAL_NAME_LEN     9

//game view and other
#define VIEW_X            30
#define VIEW_Y            30
#define VIEW_L            540
#define VIEW_H            540

#define NUM_ROT_STEPS     8

//Font sizes, delta ...
#define FONT1_L           10
#define FONT1_H           18
#define FONT1_DX          0
#define FONT1_DY          0

#define FONT2_L           19
#define FONT2_H           30
#define FONT2_DX          12
#define FONT2_DY          0

#define FONT3_L           15
#define FONT3_H           23
#define FONT3_DX          30
#define FONT3_DY          0

#define FONT4_L           15
#define FONT4_H           24
#define FONT4_DX          45
#define FONT4_DY          0

#define FONT5_L           15
#define FONT5_H           24
#define FONT5_DX          65
#define FONT5_DY          0


#define GEO_NAME          20

//Game geo idexes for wellbase
#define GEO_PAUSE_IDX      0
#define GEO_ROT_IDX        1
#define GEO_NEXT_IDX       2
#define GEO_MIXED_IDX      3

#endif





