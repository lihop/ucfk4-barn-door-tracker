UCFK4 Barn Door Tracker
=======================

A [barn door tracker](http://en.wikipedia.org/wiki/Barn_door_tracker) to aid the photography of astronomical objects.

Hardware
--------
This program is specifically designed for use with a UCFK4 (University of Canterbury Fun Kit 4) and a 28BYJ-48 stepper motor.

Full setup instructions
-----------------------
**1. Download and install git and the AVR compilation tools:**
  * gcc-avr
  * binutils-avr
  * avr-libc
  * dfu-programmer

On Debian based distributions this can be done with :
```
sudo apt-get install git gcc-avr binutils-avr avr-libc dfu-programmer
```

**2. Add a udev rule.**
  * Create a file called `/etc/udev/rules.d/52-bootloadDFU.rules` with the following contents:
    ```
    SUBSYSTEM=="usb", ATTRS{idVendor}=="03eb", ATTRS{idProduct}=="2ff0", GROUP="plugdev"
    ```
    This can be done with the command:
    ```
    echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="03eb", ATTRS{idProduct}=="2ff0", GROUP="plugdev"' \
    | sudo tee  /etc/udev/rules.d/52-bootloadDFU.rules > /dev/null
    ```
  
  * Make sure you are in the plugdev group. If you are not, you can add yourself with the command:
    ```
    sudo usermod -a -G plugdev $USER
    ```
    Log out and log back in so the changes take effect.

**4. Plug in the UCFK4 via usb cable and run the following commands:**
  ```
  git clone https://eng-git.canterbury.ac.nz/mph/ucfk4.git
  cd ucfk4/apps
  git clone https://github.com/lihop/ucfk4-barn-door-tracker.git
  cd ucfk4-barn-door-tracker
  make program
  ```
