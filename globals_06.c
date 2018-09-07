#include "globals.h"
#include <arch/zx.h>
/* This Data is placed on Bank #6
/ zsdcc can only place code and rodata into different banks at the moment
/ anything else use assembly language and sections
/ Compressed MAP Data for game paging
*/
const unsigned char world0_w[]   = {16, 4, 8,  1};
const unsigned char world0_h[]   = { 1, 4, 2, 16};
const unsigned char start_scr0[] = {0,  0, 0, 15};
const unsigned char paper0[] = {
PAPER_BLACK ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLUE ,PAPER_BLACK,PAPER_BLUE ,PAPER_BLUE ,
PAPER_BLUE ,PAPER_BLACK,PAPER_BLACK,PAPER_BLUE ,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,
PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,
PAPER_BLUE ,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_BLACK,PAPER_RED  ,PAPER_RED  ,PAPER_RED  ,PAPER_RED  ,PAPER_RED  ,PAPER_RED
};
const unsigned char lenght0[] = {
128,169,138,119,119,119,119,119,119,119,119,119,119,119,119,119,119
,119
,119
,119

};
const unsigned char world0[] = {
 3,136,0,8,0,6,132,0,6,140,0,8,0,130,3,143,0,8,142,0,130,3,158,0,130,3,158,0,130,3,150,0,5,8,130,0,5,131,0,130,3,141,1,131,2,1,131,2,138,1,130,3,157,0,8,130,3,131,1,131,0,64,151,0,130,3,134,0,7,14,135,0,131,3,0,5,138,0,130,3,132,1,130,0,149,4,131,0,130,3,156,0,130,1,130,3,158,0,130,3,139,0,5,134,0,131,3,133,2,132,1,130,3,132,0,142,1,140,0,130,3,158,0,130,3,158,1,3
,3,146,0,142,3,134,0,8,144,0,8,133,0,6,130,3,158,0,130,3,145,0,66,130,0,131,2,1,134,0,130,3,145,0,1,18,139,0,130,3,147,1,136,0,3,130,0,130,3,148,0,132,1,3,130,2,3,130,0,130,3,1,133,2,146,0,3,8,0,3,130,0,130,3,152,0,3,130,2,3,130,0,130,3,130,0,8,133,0,135,1,131,0,132,2,130,0,3,130,2,3,130,0,130,3,152,0,3,130,2,3,130,0,130,3,130,0,4,0,130,4,146,0,3,130,2,3,130,0,130,3,141,0,132,1,0,8,133,0,3,130,2,3,130,0,130,3,135,0,132,2,143,0,67,131,0,130,3,154,0,12,29,130,0,130,3,158,1,3
,10,133,0,14,131,0,13,132,0,14,130,0,12,132,0,14,131,0,13,131,0,130,10,145,0,13,140,0,130,10,158,0,130,10,143,0,64,136,0,64,133,0,130,10,143,0,7,14,135,0,7,14,132,0,130,10,132,1,154,2,130,10,148,0,14,136,0,14,130,10,134,1,148,0,132,1,130,10,12,157,0,130,10,12,132,0,16,0,132,16,147,0,130,10,12,151,0,134,1,130,10,13,140,0,133,1,140,0,130,10,132,0,134,1,148,0,130,10,134,0,64,141,0,138,1,130,10,134,0,7,14,150,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
,10,136,0,13,133,0,13,143,0,130,10,158,0,130,10,158,0,130,10,158,0,130,10,150,0,12,131,0,12,131,0,130,10,141,1,131,2,1,131,2,138,1,130,10,158,0,130,10,131,1,131,0,64,151,0,130,10,134,0,7,14,135,0,131,10,0,12,138,0,130,10,132,1,130,0,149,11,131,0,130,10,156,0,130,1,130,10,158,0,130,10,139,0,12,134,0,131,10,133,2,132,1,130,10,132,0,142,1,140,0,130,10,158,0,130,10,158,1,10
};
