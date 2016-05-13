@ECHO OFF

make clean

make GAMEPAD="SNES"
copy .\gamepad.hex ..\hex\usb_gamepad_snes.hex /y
make clean

make GAMEPAD="NES"
copy .\gamepad.hex ..\hex\usb_gamepad_nes.hex /y
make clean

make GAMEPAD="GENS"
copy .\gamepad.hex ..\hex\usb_gamepad_genesis.hex /y
make clean

make GAMEPAD="SATURN"
copy .\gamepad.hex ..\hex\usb_gamepad_saturn.hex /y
make clean

pause