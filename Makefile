

CSRC   = globals.c game.c game_enemies.c game_engine.c game_player.c game_sprite.c game_zx.c game_audio.c game_menu.c
ASRC   = banks/bank_01.asm banks/bank_03.asm banks/bank_04.asm banks/bank_06.asm banks/bank_07.asm btiles.asm font.asm game_ay.asm game_engine.asm game_zx.asm

OBJS = $(CSRC:.c=.o)
INCL = $(ZCCCFG)/../../include/_DEVELOPMENT/sdcc

OUT = bin/manic_pietro.tap
OUTC = bin/manic_pietro_zx7.tap
OUTS = bin/manic_pietro.sna
ORG = 23584

//CFLAGS = -SO3 --max-allocs-per-node200000 --opt-code-size
//CFLAGS = -SO3 --max-allocs-per-node10000 --opt-code-size
CFLAGS = -SO3 --max-allocs-per-node1000 --opt-code-size

.PHONY: depend clean

default: loader.tap game_scr.bin game.font $(OUT)
	@touch game.c
	@-rm -f zcc_opt.def
	@-ls -l *.bin
	@find . -type f -name '*.bin' -exec du -ch {} + | grep total$
	@sleep 1
	@-rm -f *.bin *.tap *.font *.zx7 game zcc_proj.lst zcc_opt.def
	@grep code_compiler_size game.map

zx7: loader.tap game_scr.bin game.font $(OUTC)
	-ls -l *.bin
	-rm -f *.bin *.tap *.font *.zx7 game zcc_proj.lst zcc_opt.def
	grep code_compiler_size game.map

sna: game_scr.bin game_scr.asm game.font $(OUTS)
	-ls -l *.bin
	-rm -f *.bin *.tap *.sna *.font *.zx7 game zcc_proj.lst zcc_opt.def
	grep code_compiler_size game.map

loader.tap: src_tap/loader.bas
	src_tap/bas2tap -sMPietro -a10 src_tap/loader.bas loader.tap >> /dev/null

game_scr.bin: src_tap/game_scr.bin
	cp src_tap/game_scr.bin game_scr.bin

game.font: src_font/game.font
	cp src_font/game.font game.font

$(OUT): $(OBJS) $(ASRC) game_loader.asm globals_06.o mmap.inc
	-rm -f $(OUT)
	zcc +zx -vn -m4 -clib=sdcc_iy banks/src/VTII10bG-mfx.asm.m4
	zcc +zx -vn -m -startup=31 -clib=sdcc_iy $(OBJS) $(ASRC) globals_06.o game_loader.asm -o game -pragma-include:zpragma.inc
	appmake +inject -b game_NIRVANAP.bin -o nirvanap_final.bin -i game_NIRVANA_HOLE.bin --offset 6299
	appmake +zx -b game_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
	appmake +zx -b game_LOADER.bin -o mcloader.tap --org 23296 --noloader --noheader
	appmake +zx -b game_scr.bin -o game_scr.tap --org 16384 --noloader --noheader
	appmake +zx -b nirvanap_final.bin -o nirvanap.tap --org 56323 --noloader --noheader
	appmake +zx -b game_CODE.bin -o game.tap --org $(ORG) --noloader --noheader
	appmake +zx -b game_BANK_1.bin -o game_banks_1.tap --org 49152 --noloader --noheader
	appmake +zx -b game_BANK_3.bin -o game_banks_3.tap --org 49152 --noloader --noheader
	appmake +zx -b game_BANK_4.bin -o game_banks_4.tap --org 49152 --noloader --noheader
	appmake +zx -b game_BANK_6.bin -o game_banks_6.tap --org 49152 --noloader --noheader
	appmake +zx -b game_BANK_7.bin -o game_banks_7.tap --org 49152 --noloader --noheader
	cat loader.tap mcload.tap mcloader.tap game_scr.tap nirvanap.tap game.tap game_banks_1.tap game_banks_3.tap game_banks_4.tap game_banks_6.tap game_banks_7.tap > $(OUT)
	grep code_compiler_size game.map

$(OUTS): $(OBJS) $(ASRC) game_sna.asm globals_06.o mmap.inc
	-rm -f $(OUTS)
	zcc +zx -vn -m4 -clib=sdcc_iy banks/src/VTII10bG-mfx.asm.m4
	zcc +zx -v -m -startup=31 -clib=sdcc_iy game.o game_sna.asm -o game_sna -pragma-include:zpragma.inc
	appmake +inject -b game_sna_NIRVANAP.bin -o nirvanap_final.bin -i game_sna_NIRVANA_HOLE.bin --offset 6299
	cp -f nirvanap_final.bin game_sna_NIRVANAP.bin
	appmake +zx --sna -b game_sna -o bin/game_release.sna --exclude-sections NIRVANA_HOLE
	grep code_compiler_size game.map

$(OUTC): $(OBJS) $(ASRC) game_loader.asm globals_06.o mmap.inc
	-rm -f $(OUTC)
	zcc +zx -vn -m4 -clib=sdcc_iy banks/src/VTII10bG-mfx.asm.m4
	zcc +zx -vn -m -startup=31 -clib=sdcc_iy $(OBJS) $(ASRC) globals_06.o game_loader.asm -o game -pragma-include:zpragma.inc
	appmake +inject -b game_NIRVANAP.bin -o nirvanap_final.bin -i game_NIRVANA_HOLE.bin --offset 6299
	zx7 -f game_scr.bin
	zx7 -f nirvanap_final.bin
	zx7 -f game_CODE.bin
	zx7 -f game_BANK_1.bin
	zx7 -f game_BANK_3.bin
	zx7 -f game_BANK_4.bin
	zx7 -f game_BANK_6.bin
	zx7 -f game_BANK_7.bin
	echo PUBLIC LEN_SCREEN, LEN_NIRVANAP, LEN_GAME, LEN_BANK_1, LEN_BANK_3, LEN_BANK_4, LEN_BANK_6, LEN_BANK_7 > zx7_game_sizes.asm
	echo defc LEN_SCREEN = $(shell wc -c < game_scr.bin.zx7) >> zx7_game_sizes.asm
	echo defc LEN_NIRVANAP = $(shell wc -c < nirvanap_final.bin.zx7) >> zx7_game_sizes.asm
	echo defc LEN_GAME = $(shell wc -c < game_CODE.bin.zx7) >> zx7_game_sizes.asm
	echo defc LEN_BANK_1 = $(shell wc -c < game_BANK_1.bin.zx7) >> zx7_game_sizes.asm
	echo defc LEN_BANK_3 = $(shell wc -c < game_BANK_3.bin.zx7) >> zx7_game_sizes.asm
	echo defc LEN_BANK_4 = $(shell wc -c < game_BANK_4.bin.zx7) >> zx7_game_sizes.asm
	echo defc LEN_BANK_6 = $(shell wc -c < game_BANK_6.bin.zx7) >> zx7_game_sizes.asm
	echo defc LEN_BANK_7 = $(shell wc -c < game_BANK_7.bin.zx7) >> zx7_game_sizes.asm
	zcc +zx -vn -startup=31 -clib=sdcc_iy -Ca-DPCOMPRESS $(OBJS) $(ASRC) globals_06.o zx7_game_sizes.asm game_loader.asm -o game -pragma-include:zpragma.inc
	appmake +zx -b game_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
	appmake +zx -b game_LOADER.bin -o mcloader.tap --org 23296 --noloader --noheader
	appmake +zx -b game_scr.bin.zx7 -o game_scr.tap --org 16384 --noloader --noheader
	appmake +zx -b nirvanap_final.bin.zx7 -o nirvanap.tap --org 56323 --noloader --noheader
	appmake +zx -b game_CODE.bin.zx7 -o game.tap --org $(ORG) --noloader --noheader
	appmake +zx -b game_BANK_1.bin.zx7 -o game_banks_1.tap --org 49152 --noloader --noheader
	appmake +zx -b game_BANK_3.bin.zx7 -o game_banks_3.tap --org 49152 --noloader --noheader
	appmake +zx -b game_BANK_4.bin.zx7 -o game_banks_4.tap --org 49152 --noloader --noheader
	appmake +zx -b game_BANK_6.bin.zx7 -o game_banks_6.tap --org 49152 --noloader --noheader
	appmake +zx -b game_BANK_7.bin.zx7 -o game_banks_7.tap --org 49152 --noloader --noheader
	cat loader.tap mcload.tap mcloader.tap game_scr.tap nirvanap.tap game.tap game_banks_1.tap game_banks_3.tap game_banks_4.tap game_banks_6.tap game_banks_7.tap > $(OUTC)
	grep code_compiler_size game.map

.c.o:
	zcc +zx -vn --list -c -clib=sdcc_iy $(CFLAGS) --fsigned-char $<

globals_06.o: globals.h globals_06.c
	zcc +zx -vn --list -c -clib=sdcc_iy $(CFLAGS) --fsigned-char --codesegBANK_6_MISC --constsegBANK_6_MISC globals_06.c

clean:
	-rm -f *.o *.lis *.bin *.tap *.font *.map *.zx7 game zcc_proj.lst zcc_opt.def

depend: $(CSRC)
	makedepend $(INCL) $^

# DO NOT DELETE THIS LINE -- make depend needs it
