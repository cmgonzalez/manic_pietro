@echo off
@setlocal

set STARTTIME=%time%
echo Start     %STARTTIME%

set "CFLAGS=-SO3 --max-allocs-per-node200000 --opt-code-size --list"

@rem MAKE BASIC LOADER
echo Creating Basic Loader.
src_tap\bas2tap -sMPietro -a10 src_tap\loader.bas loader.tap 1>nul

@rem COPY SCREEN$
echo Copying Loading Screen.
copy /b src_tap\game_scr.bin game_scr.bin 1>nul

@rem COPY FONT
echo Copying Fonts.
copy /b src_font\game.font game.font 1>nul

@rem EXPAND MACROS
echo Expanding Macros
zcc +zx -v -m4 -clib=sdcc_iy ay/src/VTII10bG-mfx.asm.m4

@rem COMPILE C CODE FOR BANK_6_MISC
echo Compiling C Code for BANK_6_MISC
zcc +zx -v -c -clib=sdcc_iy %CFLAGS% --fsigned-char --codesegBANK_6_MISC --constsegBANK_6_MISC globals_06.c

@rem BUILD CONSOLIDATED OBJECT FILE
echo Building Consolidated Object File
zcc +zx -v -c -clib=sdcc_iy %CFLAGS% --fsigned-char -o game @zproject.lst globals_06.o

@rem BUILD SNA
echo Building SNA Snapshot
del bin\game_release.sna 1>nul
zcc +zx -v -m -startup=31 -clib=sdcc_iy game.o game_sna.asm -o game_sna -pragma-include:zpragma.inc
appmake +inject -b game_sna_NIRVANAP.bin -o nirvanap_final.bin -i game_sna_NIRVANA_HOLE.bin --offset 6299
copy /b /Y nirvanap_final.bin game_sna_NIRVANAP.bin
appmake +zx --sna -b game_sna -o bin/game_release.sna --exclude-sections NIRVANA_HOLE

@rem MAKE NORMAL LOADING BINARY
echo Making Normal Loading Binary
del bin\game_release.tap 1>nul
zcc +zx -v -m -startup=31 -clib=sdcc_iy game.o game_loader.asm -o game -pragma-include:zpragma.inc
appmake +inject -b game_NIRVANAP.bin -o nirvanap_final.bin -i game_NIRVANA_HOLE.bin --offset 6299
appmake +zx -b game_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
appmake +zx -b game_LOADER.bin -o mcloader.tap --org 23296 --noloader --noheader
appmake +zx -b game_scr.bin -o game_scr.tap --org 16384 --noloader --noheader
appmake +zx -b nirvanap_final.bin -o nirvanap.tap --org 56323 --noloader --noheader
appmake +zx -b game_CODE.bin -o game.tap --org 23584 --noloader --noheader
appmake +zx -b game_BANK_3.bin -o game_ay_3.tap --org 49152 --noloader --noheader
appmake +zx -b game_BANK_4.bin -o game_ay_4.tap --org 49152 --noloader --noheader
appmake +zx -b game_BANK_6.bin -o game_ay_6.tap --org 49152 --noloader --noheader
copy /b loader.tap + mcload.tap + mcloader.tap + game_scr.tap + nirvanap.tap + game.tap + game_ay_3.tap + game_ay_4.tap + game_ay_6.tap bin\game_release.tap 1>nul

@rem MAKE ZX7 COMPRESSED LOADING BINARY
echo Making ZX7-Compressed Loading Binary
del bin\game_release_zx7.tap 1>nul
zx7 -f game_scr.bin
zx7 -f nirvanap_final.bin
zx7 -f game_CODE.bin
zx7 -f game_BANK_3.bin
zx7 -f game_BANK_4.bin
zx7 -f game_BANK_6.bin
FOR %%A IN ("game_scr.bin.zx7") DO set LEN_SCREEN=%%~zA
FOR %%A IN ("nirvanap_final.bin.zx7") DO set LEN_NIRVANAP=%%~zA
FOR %%A IN ("game_CODE.bin.zx7") DO set LEN_GAME=%%~zA
FOR %%A IN ("game_BANK_3.bin.zx7") DO set LEN_BANK_3=%%~zA
FOR %%A IN ("game_BANK_4.bin.zx7") DO set LEN_BANK_4=%%~zA
FOR %%A IN ("game_BANK_6.bin.zx7") DO set LEN_BANK_6=%%~zA
echo PUBLIC LEN_SCREEN, LEN_NIRVANAP, LEN_GAME, LEN_BANK_3, LEN_BANK_4, LEN_BANK_6 > zx7_game_sizes.asm
echo defc LEN_SCREEN = %LEN_SCREEN% >> zx7_game_sizes.asm
echo defc LEN_NIRVANAP = %LEN_NIRVANAP% >> zx7_game_sizes.asm
echo defc LEN_GAME = %LEN_GAME% >> zx7_game_sizes.asm
echo defc LEN_BANK_3 = %LEN_BANK_3% >> zx7_game_sizes.asm
echo defc LEN_BANK_4 = %LEN_BANK_4% >> zx7_game_sizes.asm
echo defc LEN_BANK_6 = %LEN_BANK_6% >> zx7_game_sizes.asm
zcc +zx -v -startup=31 -clib=sdcc_iy -Ca-DPCOMPRESS game.o game_loader.asm zx7_game_sizes.asm -o game -pragma-include:zpragma.inc
appmake +zx -b game_MCLOAD.bin -o mcload.tap --blockname mcload --org 16384 --noloader
appmake +zx -b game_LOADER.bin -o mcloader.tap --org 23296 --noloader --noheader
appmake +zx -b game_scr.bin.zx7 -o game_scr.tap --org 16384 --noloader --noheader
appmake +zx -b nirvanap_final.bin.zx7 -o nirvanap.tap --org 56323 --noloader --noheader
appmake +zx -b game_CODE.bin.zx7 -o game.tap --org 23584 --noloader --noheader
appmake +zx -b game_BANK_3.bin.zx7 -o game_ay_3.tap --org 49152 --noloader --noheader
appmake +zx -b game_BANK_4.bin.zx7 -o game_ay_4.tap --org 49152 --noloader --noheader
appmake +zx -b game_BANK_6.bin.zx7 -o game_ay_6.tap --org 49152 --noloader --noheader
copy /b loader.tap + mcload.tap + mcloader.tap + game_scr.tap + nirvanap.tap + game.tap + game_ay_3.tap + game_ay_4.tap + game_ay_6.tap bin\game_release_zx7.tap 1>nul

@rem REPORT BINARY SIZES
dir *.bin

@rem CLEANUP
echo Cleanup
@rem del *.o *.lis *.bin *.tap *.font *.zx7 game zcc_opt.def > nul 2>&1
del *.bin *.tap *.font *.zx7 game zcc_opt.def > nul 2>&1

set ENDTIME=%TIME%

@rem Change formatting for the start and end times
for /F "tokens=1-4 delims=:.," %%a in ("%STARTTIME%") do (
 set /A "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

for /F "tokens=1-4 delims=:.," %%a in ("%ENDTIME%") do (
 set /A "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)
@rem Calculate the elapsed time by subtracting values
set /A elapsed=end-start

@rem Format the results for output
set /A hh=elapsed/(60*60*100), rest=elapsed%%(60*60*100), mm=rest/(60*100), rest%%=60*100, ss=rest/100, cc=rest%%100
if %hh% lss 10 set hh=0%hh%
if %mm% lss 10 set mm=0%mm%
if %ss% lss 10 set ss=0%ss%
if %cc% lss 10 set cc=0%cc%

set DURATION=%hh%:%mm%:%ss%,%cc%
echo Elapsed   %DURATION%
@rem LAUNCH
