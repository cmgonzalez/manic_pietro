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
#ifndef GAME_AUDIO_H
#define GAME_AUDIO_H

#include "globals.h"
extern void audio_ingame(void);
extern void audio_time(void);
extern void audio_dead(void);
extern void audio_jump(void);
extern void audio_fall(void);
extern void audio_coin(void);
extern void audio_coin_noentiendo(void);
extern void audio_door_open(void);
extern void audio_tick(void);
extern void audio_crumble(void);
extern void audio_puerta(void);
extern void audio_level_start(void);
extern void audio_level_complete(void);
extern void audio_game_start(void);
extern void audio_game_end(void);
extern void audio_menu(void);
extern void audio_levelup(void);
extern void audio_round_init(void);

#endif
