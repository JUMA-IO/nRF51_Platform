fromelf -c  %1 -o %2.bin.asm
fromelf --bin  %1 -o %2.bin
copy .\Objects\BoardConfig.hex ..\image

copy .\Objects\BoardConfig.hex ..\image
copy .\Objects\BoardConfig.hex ..\..\..\..\basic_image_download\Board\SMP

