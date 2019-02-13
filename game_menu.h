/*
	This file is part of Manic Pietro.

	Manic Pietro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Manic Pietro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Manic Pietro.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "globals.h"

extern void          menu_main();
extern void          menu_main_print(void);
extern void          menu_redefine();
extern unsigned int  menu_define_key();
extern unsigned char menu_read_key( unsigned char row, unsigned char col);
extern unsigned char menu_read_code();
//extern unsigned char menu_mcodechk(unsigned char *s);



#endif
