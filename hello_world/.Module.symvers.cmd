cmd_/home/pi/linux-device-driver/hello_world/Module.symvers := sed 's/ko$$/o/' /home/pi/linux-device-driver/hello_world/modules.order | scripts/mod/modpost -m -a   -o /home/pi/linux-device-driver/hello_world/Module.symvers -e -i Module.symvers   -T -