cmd_/home/pi/linux-device-driver/03_device_file/Module.symvers := sed 's/ko$$/o/' /home/pi/linux-device-driver/03_device_file/modules.order | scripts/mod/modpost -m -a   -o /home/pi/linux-device-driver/03_device_file/Module.symvers -e -i Module.symvers   -T -
