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
extern void audio_explosion(void);
extern void audio_explosion1(void);
extern void audio_disparo1(void);
extern void audio_disparo2(void);
extern void audio_disparo_axe(void);
extern void audio_disparo_fire(void);
extern void audio_efecto(void);
extern void audio_salto(void);
extern void audio_golpe(void);
extern void audio_coin(void);
extern void audio_magic1(void);
extern void audio_sin_mana(void);
extern void audio_puerta(void);
extern void audio_lotr_lose_a_life(void);
extern void audio_level_start(void);
extern void audio_level_complete(void);
extern void audio_game_end(void);
extern void audio_menu(void);
extern void audio_checkpoint(void);
extern void audio_levelup(void);
#endif
