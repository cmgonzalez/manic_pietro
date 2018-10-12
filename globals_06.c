#include "globals.h"
#include <arch/zx.h>
/* This Data is placed on Bank #6
/ zsdcc can only place code and rodata into different banks at the moment
/ anything else use assembly language and sections
/ Compressed MAP Data for game paging
*/
const unsigned char paper0[] = {
PAPER_BLACK,
PAPER_BLUE,
PAPER_BLACK,
PAPER_BLACK,
PAPER_RED,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLUE,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_GREEN,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLUE,
PAPER_BLACK,
PAPER_BLACK,
120,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLUE,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_BLACK,
PAPER_GREEN,
PAPER_BLACK,
};
const unsigned int lenght0[] = {
133,172,141,154,162,168,169,212,173,200,203,213,198,159,192,165,236
,193
,154
,117
,148
,184
,144
,314
,195
,166
,169
,212
,173
,200
,203
,213
,198
,142
,192
,165
,236
,193
,154
,117

};
const unsigned char world0[] = {
 3,136,0,31,0,6,132,0,6,140,0,31,0,130,3,143,0,31,142,0,130,3,158,0,130,3,158,0,130,3,150,0,5,31,130,0,5,131,0,130,3,141,1,131,2,1,131,2,138,1,130,3,157,0,31,130,3,131,1,131,0,65,4,150,0,130,3,134,0,7,14,135,0,131,3,0,5,138,0,130,3,132,1,130,0,149,4,131,0,130,3,156,0,130,1,130,3,158,0,130,3,139,0,5,134,0,131,3,133,2,132,1,130,3,0,127,130,0,142,1,138,0,32,0,130,3,158,0,130,3,158,1,3
,3,146,0,142,3,134,0,31,144,0,31,133,0,6,130,3,158,0,130,3,143,0,67,4,131,0,131,2,1,134,0,130,3,144,0,18,141,0,130,3,147,1,136,0,3,130,0,130,3,148,0,132,1,3,130,2,3,130,0,130,3,1,133,2,146,0,3,31,0,3,130,0,130,3,152,0,3,130,2,3,130,0,130,3,130,0,31,133,0,135,1,131,0,132,2,130,0,3,130,2,3,130,0,130,3,152,0,3,130,2,3,130,0,130,3,130,0,4,0,130,4,146,0,3,130,2,3,130,0,130,3,141,0,132,1,0,31,133,0,3,130,2,3,130,0,130,3,0,127,133,0,132,2,143,0,67,4,32,0,130,3,154,0,12,29,130,0,130,3,158,1,3
,3,133,0,31,131,0,6,132,0,31,130,0,7,132,0,31,131,0,6,131,0,130,3,145,0,6,140,0,130,3,158,0,130,3,143,0,68,4,130,0,69,4,137,0,130,3,143,0,1,16,130,0,18,29,137,0,130,3,132,1,154,2,130,3,148,0,31,136,0,31,130,3,134,1,148,0,132,1,130,3,7,157,0,130,3,7,132,0,134,4,147,0,130,3,7,151,0,134,1,130,3,6,140,0,133,1,138,0,32,0,130,3,132,0,134,1,148,0,130,3,0,127,143,0,68,4,0,138,1,130,3,146,0,19,139,0,130,3,158,1,3
,3,31,133,0,6,134,0,147,3,139,0,31,140,0,31,131,0,32,0,130,3,158,0,130,3,146,0,134,1,134,0,130,3,154,0,132,1,130,3,1,133,0,1,137,0,1,141,0,130,3,139,0,130,1,130,0,31,132,0,131,1,134,0,31,130,3,131,2,155,0,130,3,134,0,130,1,145,0,131,1,130,0,130,3,145,0,131,1,138,0,130,3,4,0,4,154,0,1,130,3,139,0,131,1,135,0,131,1,134,0,130,3,133,0,130,1,143,0,6,132,0,131,1,130,3,71,4,133,0,71,4,136,0,130,1,137,0,127,0,130,3,1,10,133,0,6,16,149,0,130,3,158,1,3
,3,142,0,74,12,142,0,130,3,143,0,88,141,0,31,130,3,158,0,130,3,127,136,0,72,5,147,0,130,3,137,0,1,12,140,0,5,134,0,130,3,141,1,132,0,132,2,134,1,131,0,130,3,137,0,31,146,0,130,1,130,3,131,0,73,5,143,0,5,135,0,31,0,130,3,131,0,4,12,140,0,138,4,131,0,130,3,131,0,138,1,145,0,130,3,158,0,130,3,130,2,139,1,132,0,135,1,133,0,1,130,3,134,0,31,3,31,149,0,130,3,130,1,133,0,3,133,0,3,32,0,3,141,0,130,3,132,0,5,130,0,3,133,0,3,130,0,135,3,130,5,133,0,130,3,135,1,144,3,135,1,3
,3,156,0,32,0,130,3,158,0,130,3,142,0,127,143,0,130,3,148,0,6,137,0,130,3,135,0,131,1,132,0,130,1,132,0,133,1,133,0,130,3,130,0,130,1,138,0,31,3,31,138,0,131,1,130,3,143,0,3,141,0,31,130,3,134,0,77,4,134,0,77,4,134,0,133,1,131,0,130,3,130,1,132,0,6,13,134,0,14,21,142,0,130,3,31,133,0,137,1,3,137,1,133,0,130,3,140,0,31,130,0,3,6,141,0,130,3,143,0,3,142,0,130,3,130,0,5,152,0,131,1,130,3,130,0,132,4,131,0,76,4,138,0,130,1,130,0,76,4,131,0,130,3,137,0,7,20,142,0,24,29,131,0,130,3,158,1,3
,3,141,0,147,3,144,0,79,4,140,0,130,3,144,0,15,29,140,0,130,3,142,0,130,1,3,140,2,31,130,3,144,0,3,141,2,130,3,134,0,133,4,130,0,131,1,3,138,2,6,130,2,130,3,131,1,141,0,3,130,2,31,138,2,130,3,144,0,3,137,2,31,131,2,130,3,1,135,0,78,4,134,0,3,133,2,6,135,2,130,3,136,0,2,10,131,0,131,1,3,141,2,130,3,139,1,133,0,3,2,31,136,2,6,130,2,130,3,144,0,3,140,2,31,130,3,141,0,132,3,133,2,6,135,2,130,3,127,136,0,130,1,130,0,3,32,131,0,79,4,138,0,130,3,141,0,3,132,0,17,29,138,0,130,3,141,1,146,3
,3,0,6,130,0,7,132,0,6,132,0,82,31,3,7,0,3,138,0,130,3,144,0,3,130,0,3,138,0,130,3,140,0,31,0,130,1,3,141,0,130,3,144,0,3,141,0,130,3,144,0,3,141,0,130,3,131,1,133,0,134,1,130,0,3,130,1,139,0,130,3,141,0,31,130,0,3,131,0,132,1,133,0,1,130,3,0,131,1,140,0,3,0,80,5,134,0,1,131,0,130,3,0,31,133,0,131,1,134,0,3,0,18,21,138,0,130,3,144,0,3,133,1,136,0,130,3,1,138,0,131,1,130,0,3,137,0,132,1,130,3,136,0,130,1,81,7,132,0,3,141,0,130,3,131,0,130,1,133,0,12,15,132,0,3,132,0,133,1,132,0,130,3,0,127,132,0,80,5,130,0,131,4,3,32,0,3,130,1,137,0,31,0,130,3,134,0,1,9,133,0,3,130,0,3,141,0,130,3,158,1,3
,3,32,0,3,155,0,130,3,143,0,31,142,0,130,3,158,0,130,3,132,0,84,10,136,0,86,4,131,0,84,10,137,0,130,3,133,0,96,131,0,84,6,131,0,12,18,132,0,96,131,0,84,6,132,0,130,3,132,1,130,0,131,1,0,96,136,1,130,0,131,1,0,96,130,1,130,0,130,3,158,0,130,3,156,0,130,1,130,3,130,0,130,1,130,0,131,1,130,0,136,4,139,0,130,3,149,0,131,1,130,0,130,1,130,0,130,3,130,1,142,0,87,6,140,0,130,3,144,0,12,18,140,0,130,3,130,0,130,1,130,0,131,1,130,0,136,1,130,0,131,1,130,0,130,1,130,0,130,3,127,155,0,130,1,130,3,158,0,130,3,158,1,3
,3,138,0,6,0,131,1,3,0,6,0,6,138,1,130,3,141,0,31,0,3,132,0,6,136,0,31,130,3,134,1,137,0,3,132,0,31,133,0,132,1,130,3,130,0,6,140,0,3,142,0,130,3,127,142,0,3,132,1,138,0,130,3,135,0,1,135,2,3,131,0,89,5,0,136,1,130,3,132,1,135,0,31,131,0,3,131,0,17,21,137,0,130,3,138,0,89,5,131,0,3,135,1,131,2,132,0,130,3,133,1,133,0,9,14,131,0,3,0,31,140,0,130,3,6,135,0,135,1,3,140,0,130,1,130,3,132,1,130,2,134,0,89,8,0,3,135,1,134,0,6,130,3,140,0,8,14,0,3,134,0,6,131,2,132,0,130,3,135,0,138,7,141,0,130,3,131,1,135,0,89,5,32,142,0,131,1,130,3,138,0,4,16,146,0,130,3,158,7,3
,135,3,140,0,6,132,0,31,134,0,130,3,32,0,127,143,0,5,138,0,31,130,3,139,0,90,6,145,0,130,3,132,1,135,0,8,56,130,0,93,4,141,0,130,3,31,142,0,15,24,136,0,90,4,131,0,130,3,130,0,90,8,134,1,132,0,130,1,135,7,130,1,0,96,131,0,130,3,130,0,32,96,142,0,31,132,0,6,132,0,130,1,130,3,141,0,93,10,133,0,90,6,0,6,134,0,130,3,130,1,130,0,130,4,135,0,14,18,133,0,56,96,0,6,132,0,130,1,130,3,138,0,137,1,132,0,5,134,0,130,3,138,0,6,135,0,6,136,0,1,130,0,130,3,132,0,132,2,130,0,6,135,0,5,139,0,130,3,138,0,5,144,0,131,1,130,3,130,1,138,0,93,4,136,0,131,1,132,0,31,130,3,140,0,5,19,144,0,130,3,158,1,3
,3,0,6,131,0,7,132,0,6,131,0,82,31,3,7,130,0,3,137,0,130,3,158,0,130,3,142,0,130,2,142,0,130,3,142,0,31,143,0,130,3,158,0,130,3,131,1,133,0,133,2,3,130,0,3,134,2,130,1,31,132,0,130,3,0,31,139,0,3,130,0,3,136,0,81,5,130,0,1,130,3,133,0,130,1,134,0,3,0,31,3,136,0,24,28,131,0,130,3,130,0,1,138,0,3,130,0,3,135,0,134,1,130,3,137,0,132,1,3,130,0,3,141,0,130,3,133,0,1,138,0,3,131,2,138,0,130,3,140,0,81,7,130,0,3,135,0,130,1,132,0,130,3,134,1,134,0,10,15,130,0,3,133,0,5,132,0,5,130,0,130,3,0,127,134,0,81,5,131,1,3,32,0,3,4,0,137,4,31,0,130,3,136,0,1,9,131,0,3,130,0,3,141,0,130,3,140,1,132,3,142,1,3
,161,3,130,0,7,131,0,94,4,150,0,130,3,130,0,7,0,96,5,7,29,150,0,130,3,130,0,7,0,8,96,146,1,130,0,132,1,130,3,130,0,7,139,0,94,10,142,0,130,3,130,0,7,139,0,7,29,142,0,130,3,130,0,7,131,0,1,0,31,133,1,130,0,133,1,130,0,132,1,130,0,1,130,3,130,0,7,146,0,94,4,135,0,130,3,130,0,7,146,0,7,29,135,0,130,3,130,0,7,131,0,133,1,130,0,131,1,130,0,31,133,1,0,31,132,1,130,3,130,0,7,141,0,94,10,140,0,130,3,130,0,7,141,0,7,29,140,0,130,3,130,0,7,131,0,131,1,130,0,131,1,130,0,132,1,130,0,132,1,130,0,130,1,130,3,32,0,7,153,0,127,0,130,3,130,0,7,155,0,130,3,130,1,154,4,130,1,3
,3,98,4,136,0,98,4,130,0,32,133,0,98,4,136,0,130,3,0,96,137,0,96,137,0,96,136,0,130,3,150,0,31,135,0,130,3,158,0,130,3,158,0,130,3,142,0,1,7,142,0,130,3,130,0,1,7,134,0,1,7,134,0,1,7,134,0,1,7,130,0,130,3,134,0,1,7,135,0,31,134,0,1,7,130,0,31,131,0,130,3,130,0,31,155,0,130,3,132,0,1,7,134,0,1,7,134,0,1,7,134,0,1,7,130,3,158,0,130,3,1,7,134,0,1,7,132,0,4,0,132,4,132,0,1,7,132,0,130,3,158,0,130,3,134,0,1,7,146,0,31,131,0,130,3,158,0,161,3
,3,133,0,155,3,156,0,130,7,130,3,152,0,31,131,0,130,7,130,3,32,134,0,144,4,133,1,130,7,130,3,135,0,6,147,0,6,130,7,130,3,133,1,130,0,5,100,5,145,0,6,130,7,130,3,136,0,32,96,0,31,139,0,130,1,130,0,6,130,7,130,3,134,0,2,132,0,130,1,142,0,6,130,7,130,3,130,0,130,1,138,0,100,8,0,130,1,136,0,6,130,7,130,3,142,0,32,96,136,0,130,1,0,6,130,7,130,3,130,1,137,0,130,1,135,0,100,4,133,0,6,130,7,130,3,145,0,130,1,0,32,96,133,0,5,130,7,130,3,132,0,131,1,143,0,130,1,132,0,130,7,130,3,0,127,137,0,130,1,132,0,102,5,137,0,130,7,130,3,145,0,17,19,134,0,31,130,0,130,7,161,3
,3,31,157,0,130,3,158,0,130,3,157,0,31,130,3,158,0,130,3,158,0,130,3,1,132,0,1,132,0,3,32,0,3,135,0,131,1,130,0,104,8,130,0,130,3,138,0,3,130,0,130,3,139,0,25,29,130,0,130,3,131,0,1,134,0,3,0,31,131,3,130,0,104,6,132,0,134,1,130,3,138,0,3,130,0,132,3,0,18,23,138,0,130,3,130,2,152,4,132,0,130,3,0,104,4,140,0,31,142,0,130,3,0,1,31,134,0,130,3,130,1,140,0,1,132,0,130,3,137,1,149,0,130,3,0,127,135,0,104,4,137,0,1,132,0,1,132,0,130,3,137,0,1,19,139,0,131,5,133,0,130,3,158,1,3
,3,156,0,132,3,156,0,32,0,130,3,154,0,106,3,130,0,130,3,127,153,0,8,88,130,0,130,3,133,0,5,130,0,5,131,0,5,130,0,5,133,0,5,0,5,134,0,130,3,130,1,135,2,130,0,135,2,130,0,131,2,31,130,2,130,0,130,1,130,3,130,2,6,134,2,106,6,135,2,130,0,134,2,130,0,130,2,130,3,137,2,8,96,131,2,31,131,2,106,10,133,2,6,130,0,130,2,130,3,130,2,130,0,133,2,130,0,130,2,133,4,8,64,134,2,130,0,130,2,130,3,31,2,106,31,133,2,130,0,135,2,130,0,134,2,130,0,130,2,130,3,130,2,64,96,133,2,130,0,135,2,31,130,2,6,132,2,130,0,130,2,130,3,130,2,130,0,132,2,6,130,0,142,2,31,130,0,130,2,130,3,130,2,130,0,133,2,130,0,143,2,130,0,130,2,130,3,132,0,108,10,133,0,108,5,145,0,130,3,132,0,5,31,133,0,12,25,141,0,132,1,130,3,158,1,3
,3,155,0,3,32,0,130,3,143,0,31,136,0,110,10,132,0,130,3,137,0,110,6,142,0,96,132,0,130,3,132,0,110,4,132,0,96,0,86,10,133,0,110,3,137,0,130,3,133,0,96,134,0,12,18,134,0,96,137,0,130,3,130,0,130,1,130,0,131,1,130,0,136,1,130,0,131,1,130,0,132,1,130,3,144,0,86,4,140,0,130,3,130,1,142,0,12,18,140,0,130,3,139,0,136,1,130,0,131,1,130,0,130,1,130,0,130,3,130,0,130,1,130,0,131,1,149,0,130,3,144,0,86,10,138,0,130,1,130,3,144,0,12,18,140,0,130,3,130,0,130,1,130,0,131,1,130,0,136,1,130,0,131,1,130,0,130,1,130,0,130,3,130,1,142,0,86,4,138,0,127,0,130,3,144,0,12,18,140,0,3,160,1
,131,3,156,0,130,3,32,156,0,31,130,3,158,0,130,3,152,0,114,5,132,0,130,3,152,0,24,29,132,0,130,3,31,0,130,1,132,0,134,1,137,0,135,1,130,3,156,0,114,5,130,3,146,0,131,1,135,0,24,29,130,3,130,1,136,0,112,5,131,1,113,6,134,0,135,1,130,3,138,0,48,80,131,0,8,80,139,0,114,5,130,3,132,0,112,4,135,0,127,132,0,130,1,136,0,24,29,130,3,132,1,8,88,145,0,135,1,130,3,134,0,132,4,131,0,133,1,139,0,31,130,3,144,0,114,5,140,0,132,3,142,0,14,29,140,0,132,3,148,1,3,135,1,3
,158,3,10,159,3,10,159,3,10,159,3,10,159,3,10,148,3,32,0,3,0,31,135,0,148,3,130,0,3,130,0,96,3,132,0,31,148,3,130,0,3,130,0,48,96,133,0,130,3,156,0,130,1,130,3,158,0,130,3,150,4,131,0,2,132,0,130,3,136,0,5,31,0,5,0,31,130,0,5,0,31,0,5,134,0,1,130,0,130,3,130,1,156,0,130,3,132,0,130,1,0,82,8,144,0,127,132,0,130,3,158,0,130,3,158,1,3
,161,3,136,16,23,140,16,23,136,16,130,3,132,0,31,131,0,6,140,0,6,131,0,31,132,0,130,3,0,19,7,133,0,8,131,0,31,132,0,31,137,0,24,20,0,130,3,17,18,154,0,21,17,130,3,131,0,5,150,0,5,131,0,130,3,136,2,142,0,136,2,130,3,0,6,135,0,140,4,135,0,6,0,130,3,138,0,138,16,138,0,130,3,134,1,146,0,134,1,130,3,158,0,130,3,138,1,132,0,32,133,0,138,1,130,3,137,16,140,0,137,16,130,3,130,17,7,0,127,137,0,65,4,139,0,24,130,17,130,3,142,0,4,26,142,0,161,3
,130,0,21,148,0,7,138,0,21,132,0,31,135,0,7,142,0,7,0,7,0,21,151,0,7,135,0,21,136,0,67,4,149,0,21,136,0,8,15,149,0,21,0,131,1,4,0,136,4,131,1,132,0,5,131,0,31,133,0,21,0,131,16,138,0,131,16,0,7,130,0,3,137,0,21,140,0,31,136,0,22,131,0,6,133,0,21,131,0,67,4,143,0,136,1,131,0,21,131,0,5,12,143,0,136,16,130,0,1,17,139,1,131,0,132,1,140,0,141,16,131,0,132,16,136,0,32,151,0,3,131,0,3,136,0,127,135,0,17,134,2,17,131,0,22,0,5,0,22,155,0,22,0,3,0,22,134,0,18,19,18,19,18,19,18,19,18,19,136,23,18,19,18,19,18,19,18,19,18,19,18,19,18,19
,137,0,31,153,0,82,5,131,0,1,7,1,0,32,134,0,31,134,0,31,142,0,7,139,0,81,3,139,0,142,1,3,132,0,18,27,141,0,134,16,7,134,16,139,1,7,1,134,0,31,151,0,7,138,0,23,130,0,7,132,0,3,142,1,132,0,1,7,139,1,133,16,7,135,16,135,0,7,132,0,81,4,146,0,31,134,0,7,132,0,5,15,141,0,81,5,137,0,142,1,3,131,0,7,130,0,22,27,139,0,141,16,139,1,7,1,158,0,7,134,0,127,143,0,23,136,0,7,137,0,5,136,0,3,142,1,132,0,141,1,141,16,132,0
,18,19,18,19,18,19,18,1,18,19,18,19,18,19,18,19,18,19,18,19,18,19,1,19,18,19,18,1,18,19,18,19,17,3,130,0,31,130,0,6,130,0,31,139,0,6,132,0,6,130,0,22,20,17,3,144,0,31,139,0,22,20,17,3,132,0,71,4,150,0,22,20,17,3,132,0,5,10,141,0,71,4,0,32,133,0,22,20,18,19,130,0,18,19,18,19,1,18,19,18,19,18,19,134,0,19,26,135,0,18,19,17,3,0,31,132,0,21,137,0,1,19,18,19,18,19,18,19,18,19,130,0,22,20,17,3,134,0,21,0,130,16,0,7,0,130,16,0,7,139,0,22,20,17,3,134,0,21,0,130,16,0,7,0,130,16,0,7,133,0,31,133,0,22,20,17,3,0,130,2,0,18,19,18,19,18,19,18,1,19,18,19,18,1,139,0,22,20,18,19,139,0,7,132,0,7,133,0,5,133,0,18,19,17,3,132,0,31,134,0,7,132,0,7,130,0,135,4,130,0,22,20,17,3,132,0,73,4,133,0,7,132,0,7,139,0,22,20,17,3,132,0,2,11,133,0,7,0,127,130,0,7,137,0,130,2,22,20,17,3,138,16,0,7,132,0,7,0,138,16,22,20,18,19,18,19,18,19,18,19,18,19,18,19,18,19,18,19,18,19,18,19,18,19,18,19,18,19,18,19,18,19,18,19
,2,142,0,130,3,142,0,130,2,0,118,10,141,0,130,3,140,0,130,2,146,0,22,130,3,137,0,130,2,132,3,142,0,7,130,0,132,3,133,0,130,2,146,0,7,0,31,132,0,133,3,130,2,146,0,7,139,0,130,2,32,142,0,78,3,141,0,130,2,132,0,31,138,0,8,22,135,0,31,133,0,130,2,130,3,139,0,76,3,140,0,24,7,0,130,2,141,0,7,26,140,0,24,7,0,130,2,130,0,7,131,0,131,3,22,131,3,22,132,3,22,136,3,24,7,0,130,2,130,0,7,134,0,7,131,0,7,132,0,7,136,0,24,7,0,130,2,130,0,7,134,0,7,0,31,0,7,132,0,7,134,0,31,0,24,7,0,130,2,130,0,127,134,0,7,131,0,7,132,0,7,136,0,24,7,0,130,2,158,0,130,2,158,3,2
,3,156,0,32,0,130,3,158,0,130,3,142,0,127,143,0,130,3,148,0,6,137,0,130,3,135,0,131,1,132,0,130,1,132,0,133,1,133,0,130,3,130,0,130,1,138,0,31,3,31,138,0,131,1,130,3,143,0,3,141,0,31,130,3,134,0,77,4,134,0,77,4,134,0,133,1,131,0,130,3,130,1,132,0,6,13,134,0,14,21,142,0,130,3,31,133,0,137,1,3,137,1,133,0,130,3,140,0,31,130,0,3,6,141,0,130,3,143,0,3,142,0,130,3,130,0,5,155,0,130,3,130,0,132,4,131,0,76,4,138,0,130,1,130,0,76,4,131,0,130,3,137,0,7,20,142,0,24,29,131,0,130,3,158,1,3
,3,141,0,147,3,144,0,79,4,140,0,130,3,144,0,15,29,140,0,130,3,142,0,130,1,3,140,2,31,130,3,144,0,3,141,2,130,3,134,0,133,4,130,0,131,1,3,138,2,6,130,2,130,3,131,1,141,0,3,130,2,31,138,2,130,3,144,0,3,137,2,31,131,2,130,3,1,135,0,78,4,134,0,3,133,2,6,135,2,130,3,136,0,2,10,131,0,131,1,3,141,2,130,3,139,1,133,0,3,2,31,136,2,6,130,2,130,3,144,0,3,140,2,31,130,3,141,0,132,3,133,2,6,135,2,130,3,127,136,0,130,1,130,0,3,32,131,0,79,4,138,0,130,3,141,0,3,132,0,17,29,138,0,130,3,141,1,146,3
,3,0,6,130,0,7,132,0,6,132,0,82,31,3,7,0,3,138,0,130,3,144,0,3,130,0,3,138,0,130,3,140,0,31,0,130,1,3,141,0,130,3,144,0,3,141,0,130,3,144,0,3,141,0,130,3,131,1,133,0,134,1,130,0,3,130,1,139,0,130,3,141,0,31,130,0,3,131,0,132,1,133,0,1,130,3,0,131,1,140,0,3,0,80,5,134,0,1,131,0,130,3,0,31,133,0,131,1,134,0,3,0,18,21,138,0,130,3,144,0,3,133,1,136,0,130,3,1,138,0,131,1,130,0,3,137,0,132,1,130,3,136,0,130,1,81,7,132,0,3,141,0,130,3,131,0,130,1,133,0,12,15,132,0,3,132,0,133,1,132,0,130,3,0,127,132,0,80,5,130,0,131,4,3,32,0,3,130,1,137,0,31,0,130,3,134,0,1,9,133,0,3,130,0,3,141,0,130,3,158,1,3
,3,32,0,3,155,0,130,3,143,0,31,142,0,130,3,158,0,130,3,132,0,84,10,136,0,86,4,131,0,84,10,137,0,130,3,133,0,96,131,0,84,6,131,0,12,18,132,0,96,131,0,84,6,132,0,130,3,132,1,130,0,131,1,0,96,136,1,130,0,131,1,0,96,130,1,130,0,130,3,158,0,130,3,156,0,130,1,130,3,130,0,130,1,130,0,131,1,130,0,136,4,139,0,130,3,149,0,131,1,130,0,130,1,130,0,130,3,130,1,142,0,87,6,140,0,130,3,144,0,12,18,140,0,130,3,130,0,130,1,130,0,131,1,130,0,136,1,130,0,131,1,130,0,130,1,130,0,130,3,127,155,0,130,1,130,3,158,0,130,3,158,1,3
,3,138,0,6,0,131,1,3,0,6,0,6,138,1,130,3,141,0,31,0,3,132,0,6,136,0,31,130,3,134,1,137,0,3,132,0,31,133,0,132,1,130,3,130,0,6,140,0,3,142,0,130,3,127,142,0,3,132,1,138,0,130,3,135,0,1,135,2,3,131,0,89,5,0,136,1,130,3,132,1,135,0,31,131,0,3,131,0,17,21,137,0,130,3,138,0,89,5,131,0,3,135,1,131,2,132,0,130,3,133,1,133,0,9,14,131,0,3,0,31,140,0,130,3,6,135,0,135,1,3,140,0,130,1,130,3,132,1,130,2,134,0,89,8,0,3,135,1,134,0,6,130,3,140,0,8,14,0,3,134,0,6,131,2,132,0,130,3,135,0,138,7,141,0,130,3,131,1,135,0,89,5,32,142,0,131,1,130,3,138,0,4,16,146,0,130,3,158,7,3
,135,3,140,0,6,132,0,31,134,0,130,3,32,0,127,143,0,5,138,0,31,130,3,139,0,90,6,145,0,130,3,132,1,135,0,8,56,130,0,93,4,141,0,130,3,31,142,0,15,24,136,0,90,4,131,0,130,3,130,0,90,8,134,1,132,0,130,1,135,7,130,1,0,96,131,0,130,3,130,0,32,96,142,0,31,132,0,6,132,0,130,1,130,3,141,0,93,10,133,0,90,6,0,6,134,0,130,3,130,1,130,0,130,4,135,0,14,18,133,0,56,96,0,6,132,0,130,1,130,3,138,0,137,1,132,0,5,134,0,130,3,138,0,6,135,0,6,136,0,1,130,0,130,3,132,0,132,2,130,0,6,135,0,5,139,0,130,3,138,0,5,144,0,131,1,130,3,130,1,138,0,93,4,136,0,131,1,132,0,31,130,3,140,0,5,19,144,0,130,3,158,1,3
,3,0,6,131,0,7,132,0,6,131,0,82,31,3,7,130,0,3,137,0,130,3,158,0,130,3,142,0,130,2,142,0,130,3,142,0,31,143,0,130,3,158,0,130,3,131,1,133,0,133,2,3,130,0,3,134,2,130,1,31,132,0,130,3,0,31,139,0,3,130,0,3,136,0,81,5,130,0,1,130,3,133,0,130,1,134,0,3,0,31,3,136,0,24,28,131,0,130,3,130,0,1,138,0,3,130,0,3,135,0,134,1,130,3,137,0,132,1,3,130,0,3,141,0,130,3,133,0,1,138,0,3,131,2,138,0,130,3,140,0,81,7,130,0,3,135,0,130,1,132,0,130,3,134,1,134,0,10,15,130,0,3,133,0,5,132,0,5,130,0,130,3,0,127,134,0,81,5,131,1,3,32,0,3,4,0,137,4,31,0,130,3,136,0,1,9,131,0,3,130,0,3,141,0,130,3,140,1,132,3,142,1,3
,161,3,130,0,7,131,0,94,4,150,0,130,3,130,0,7,0,96,5,7,29,150,0,130,3,130,0,7,0,8,96,146,1,130,0,132,1,130,3,130,0,7,139,0,94,10,142,0,130,3,130,0,7,139,0,7,29,142,0,130,3,130,0,7,131,0,1,0,31,133,1,130,0,133,1,130,0,132,1,130,0,1,130,3,130,0,7,146,0,94,4,135,0,130,3,130,0,7,146,0,7,29,135,0,130,3,130,0,7,131,0,133,1,130,0,131,1,130,0,31,133,1,0,31,132,1,130,3,130,0,7,141,0,94,10,140,0,130,3,130,0,7,141,0,7,29,140,0,130,3,130,0,7,131,0,131,1,130,0,131,1,130,0,132,1,130,0,132,1,130,0,130,1,130,3,32,0,7,153,0,127,0,130,3,130,0,7,155,0,130,3,130,1,154,4,130,1,3
,3,0,96,137,0,96,137,0,96,136,0,130,3,150,0,31,135,0,130,3,158,0,130,3,158,0,130,3,142,0,1,7,142,0,130,3,130,0,1,7,134,0,1,7,134,0,1,7,134,0,1,7,130,0,130,3,134,0,1,7,135,0,31,134,0,1,7,130,0,31,131,0,130,3,130,0,31,155,0,130,3,132,0,1,7,134,0,1,7,134,0,1,7,134,0,1,7,130,3,158,0,130,3,1,7,134,0,1,7,132,0,4,0,132,4,132,0,1,7,132,0,130,3,158,0,130,3,134,0,1,7,146,0,31,131,0,130,3,158,0,161,3
,3,133,0,155,3,156,0,130,7,130,3,152,0,31,131,0,130,7,130,3,32,134,0,144,4,133,1,130,7,130,3,135,0,6,147,0,6,130,7,130,3,133,1,130,0,5,100,5,145,0,6,130,7,130,3,136,0,32,96,0,31,139,0,130,1,130,0,6,130,7,130,3,134,0,2,132,0,130,1,142,0,6,130,7,130,3,130,0,130,1,138,0,100,8,0,130,1,136,0,6,130,7,130,3,142,0,32,96,136,0,130,1,0,6,130,7,130,3,130,1,137,0,130,1,135,0,100,4,133,0,6,130,7,130,3,145,0,130,1,0,32,96,133,0,5,130,7,130,3,132,0,131,1,143,0,130,1,132,0,130,7,130,3,0,127,137,0,130,1,132,0,102,5,137,0,130,7,130,3,145,0,17,19,134,0,31,130,0,130,7,161,3
,3,31,157,0,130,3,158,0,130,3,157,0,31,130,3,158,0,130,3,158,0,130,3,1,132,0,1,132,0,3,32,0,3,135,0,131,1,130,0,104,8,130,0,130,3,138,0,3,130,0,130,3,139,0,25,29,130,0,130,3,131,0,1,134,0,3,0,31,131,3,130,0,104,6,132,0,134,1,130,3,138,0,3,130,0,132,3,0,18,23,138,0,130,3,130,2,152,4,132,0,130,3,0,104,4,140,0,31,142,0,130,3,0,1,31,134,0,130,3,130,1,140,0,1,132,0,130,3,137,1,149,0,130,3,0,127,135,0,104,4,137,0,1,132,0,1,132,0,130,3,137,0,1,19,139,0,131,5,133,0,130,3,158,1,3
,3,156,0,132,3,156,0,32,0,130,3,154,0,106,3,130,0,130,3,127,153,0,8,88,130,0,130,3,133,0,5,130,0,5,131,0,5,130,0,5,133,0,5,0,5,134,0,130,3,130,1,135,2,130,0,135,2,130,0,131,2,31,130,2,130,0,130,1,130,3,130,2,6,134,2,106,6,135,2,130,0,134,2,130,0,130,2,130,3,137,2,8,96,131,2,31,131,2,106,10,133,2,6,130,0,130,2,130,3,130,2,130,0,133,2,130,0,130,2,133,4,8,64,134,2,130,0,130,2,130,3,31,2,106,31,133,2,130,0,135,2,130,0,134,2,130,0,130,2,130,3,130,2,64,96,133,2,130,0,135,2,31,130,2,6,132,2,130,0,130,2,130,3,130,2,130,0,132,2,6,130,0,142,2,31,130,0,130,2,130,3,130,2,130,0,133,2,130,0,143,2,130,0,130,2,130,3,132,0,108,10,133,0,108,5,145,0,130,3,132,0,5,31,133,0,12,25,141,0,132,1,130,3,158,1,3
,3,155,0,3,32,0,130,3,143,0,31,136,0,110,10,132,0,130,3,137,0,110,6,142,0,96,132,0,130,3,132,0,110,4,132,0,96,0,86,10,133,0,110,3,137,0,130,3,133,0,96,134,0,12,18,134,0,96,137,0,130,3,130,0,130,1,130,0,131,1,130,0,136,1,130,0,131,1,130,0,132,1,130,3,144,0,86,4,140,0,130,3,130,1,142,0,12,18,140,0,130,3,139,0,136,1,130,0,131,1,130,0,130,1,130,0,130,3,130,0,130,1,130,0,131,1,149,0,130,3,144,0,86,10,138,0,130,1,130,3,144,0,12,18,140,0,130,3,130,0,130,1,130,0,131,1,130,0,136,1,130,0,131,1,130,0,130,1,130,0,130,3,130,1,142,0,86,4,138,0,127,0,130,3,144,0,12,18,140,0,3,160,1
,131,3,156,0,130,3,32,156,0,31,130,3,158,0,130,3,152,0,114,5,132,0,130,3,152,0,24,29,132,0,130,3,31,0,130,1,132,0,134,1,137,0,135,1,130,3,156,0,114,5,130,3,146,0,131,1,135,0,24,29,130,3,130,1,136,0,112,5,131,1,113,6,134,0,135,1,130,3,138,0,48,80,131,0,8,80,139,0,114,5,130,3,132,0,112,4,135,0,127,132,0,130,1,136,0,24,29,130,3,132,1,8,88,145,0,135,1,130,3,134,0,132,4,131,0,133,1,139,0,31,130,3,144,0,114,5,140,0,132,3,142,0,14,29,140,0,132,3,148,1,3,135,1,3
,158,3,10,159,3,10,159,3,10,159,3,10,159,3,10,148,3,32,0,3,0,31,135,0,148,3,130,0,3,130,0,96,3,132,0,31,148,3,130,0,3,130,0,48,96,133,0,130,3,156,0,130,1,130,3,158,0,130,3,150,4,131,0,2,132,0,130,3,136,0,5,31,0,5,0,31,130,0,5,0,31,0,5,134,0,1,130,0,130,3,130,1,156,0,130,3,132,0,130,1,0,82,8,144,0,127,132,0,130,3,158,0,130,3,158,1,3
};
