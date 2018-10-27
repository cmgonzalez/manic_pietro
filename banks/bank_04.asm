;; AY RELATED FOR BANK_4

; OWN COPY OF VTII PLAYER
PUBLIC vtii_init
PUBLIC vtii_play
PUBLIC vtii_stop
PUBLIC vtii_setup
PUBLIC vtii_modaddr

defc vtii_init  = INIT
defc vtii_play  = PLAY
defc vtii_stop  = STOP
defc vtii_setup = SETUP
defc vtii_modaddr = MODADDR

PUBLIC mfx_init
PUBLIC mfx_add
PUBLIC mfx_playm
PUBLIC mfx_playe
PUBLIC mfx_mfxptr

defc mfx_init  = MFXINIT
defc mfx_add   = MFXADD
defc mfx_playm = MFXPLAYM
defc mfx_playe = MFXPLAYE
defc mfx_mfxptr = mfxPtr



SECTION BANK_4_VTII

INCLUDE "src/VTII10bG-mfx.asm"

; OWN COPY OF MFXPLAYER

SECTION BANK_4_MFX

INCLUDE "src/mfx.asm"

; SONGS & EFFECTS IN THIS BANK

SECTION BANK_4_AUDIO

PUBLIC _ay_song_ingame0
_ay_song_ingame0:
  BINARY "songs/pietro_cavern.pt3"

PUBLIC _ay_song_ingame1
_ay_song_ingame1:
  BINARY "songs/Davos_MOZART.pt3"


PUBLIC _ay_song_menu
_ay_song_menu:
    BINARY "songs/pietro_blue.pt3"

;SOUND FX

PUBLIC _ay_fx_coin
_ay_fx_coin:
BINARY "fx/coin.afx"

PUBLIC _ay_fx_coin_noentiendo
_ay_fx_coin_noentiendo:
BINARY "fx/coin_noentiendo.afx"

PUBLIC _ay_fx_dead
_ay_fx_dead:
BINARY "fx/dead.afx"

PUBLIC _ay_fx_jump
_ay_fx_jump:
BINARY "fx/jump_test.afx"

PUBLIC _ay_fx_fall
_ay_fx_fall:
BINARY "fx/fall.afx"

PUBLIC _ay_fx_game_start
_ay_fx_game_start:
BINARY "fx/game_start.afx"

PUBLIC _ay_fx_tick
_ay_fx_tick:
BINARY "fx/tick.afx"

PUBLIC _ay_fx_crumble
_ay_fx_crumble:
BINARY "fx/crumble.afx"

PUBLIC _ay_fx_door_open
_ay_fx_door_open:
BINARY "fx/door_open.afx"
