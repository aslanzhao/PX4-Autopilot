# upload firmware to hardware
the upload tool is openocd, stlink
```bash
openocd -f interface/stlink.cfg -f target/stm32f7x.cfg -c "program ./build/px4_fmu-v5_default/px4_fmu-v5_default.bin 0x8008000 verify reset exit"

openocd -f interface/stlink.cfg -f target/stm32h7x.cfg -c "program ./build/micoair_h743_bootloader/micoair_h743_bootloader.bin 0x8000000 verify reset exit"

openocd -f interface/stlink.cfg -f target/stm32h7x.cfg -c "program ./boards/micoair/h743/extras/micoair_h743_bootloader.bin 0x8000000 verify reset exit"

openocd -f interface/stlink.cfg -f target/stm32h7x.cfg -c "program ./boards/micoair/h743-aio/extras/micoair_h743-aio_bootloader.bin 0x8000000 verify reset exit"
```
