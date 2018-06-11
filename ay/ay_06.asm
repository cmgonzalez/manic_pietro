;; AY RELATED FOR BANK_6

; OWN COPY OF VTII PLAYER

; This bank exports the player entry addresses
; which are the same for all banks' copies

SECTION BANK_6_VTII

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

INCLUDE "src/VTII10bG-mfx.asm"

; OWN COPY OF MFXPLAYER

; This bank exports the mfx entry addresses
; which are the same for all banks' copies

SECTION BANK_6_MFX

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

INCLUDE "src/mfx.asm"

; SONGS & EFFECTS IN THIS BANK

SECTION BANK_6_AUDIO

PUBLIC _ay_song_06_lotr_lose_a_life
PUBLIC _ay_song_06_lotr_level_complete
PUBLIC _ay_song_06_lotr_level_start


PUBLIC _ay_fx_06_coin
PUBLIC _ay_fx_06_dano_por_toque
PUBLIC _ay_fx_06_explosion
PUBLIC _ay_fx_06_golpe
PUBLIC _ay_fx_06_magic1
PUBLIC _ay_fx_06_puerta
PUBLIC _ay_fx_06_salto
PUBLIC _ay_fx_06_sin_mana
PUBLIC _ay_fx_06_magic
PUBLIC _ay_fx_06_disparo1
PUBLIC _ay_fx_06_disparo2
PUBLIC _ay_fx_06_efecto
PUBLIC _ay_fx_06_explosion1
PUBLIC _ay_fx_06_mushroom
PUBLIC _ay_fx_06_checkpoint
PUBLIC _ay_fx_06_levelup
PUBLIC _ay_fx_06_respawn
PUBLIC _ay_fx_06_disparo_axe
PUBLIC _ay_fx_06_disparo_fire

_ay_song_06_lotr_lose_a_life:
   BINARY "songs/lotr-losealife.pt3"

_ay_song_06_lotr_level_complete:
   BINARY "songs/lotr-lvlcomplete.pt3"

_ay_song_06_lotr_level_start:
   BINARY "songs/lotr-lvlstart.pt3"



_ay_fx_06_coin:
  BINARY "fx/coin.afx"
_ay_fx_06_dano_por_toque:
  BINARY "fx/dano_por_toque.afx"
_ay_fx_06_explosion:
  BINARY "fx/explosion.afx"
_ay_fx_06_golpe:
  BINARY "fx/golpe.afx"
_ay_fx_06_magic1:
  BINARY "fx/magic1.afx"
_ay_fx_06_puerta:
  BINARY "fx/puerta.afx"
_ay_fx_06_salto:
  BINARY "fx/salto.afx"
_ay_fx_06_sin_mana:
  BINARY "fx/sin_mana.afx"
_ay_fx_06_magic:
  BINARY "fx/magic.afx"
_ay_song_04_ciel:
  BINARY "songs/ciel.pt3"
_ay_fx_06_disparo1:
  BINARY "fx/disparo1.afx"
_ay_fx_06_disparo2:
  BINARY "fx/disparo2.afx"
_ay_fx_06_efecto:
  BINARY "fx/efecto.afx"
_ay_fx_06_explosion1:
  BINARY "fx/explosion1.afx"
_ay_fx_06_mushroom:
   BINARY "fx/mushroom.afx"
_ay_fx_06_checkpoint:
   BINARY "fx/checkpoint.afx"
_ay_fx_06_levelup:
   BINARY "fx/levelup.afx"
_ay_fx_06_respawn:
   BINARY "fx/respawn.afx"
_ay_fx_06_disparo_axe:
   BINARY "fx/disparo_axe.afx"
_ay_fx_06_disparo_fire:
   BINARY "fx/disparo_fire.afx"
