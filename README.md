# How to compile kernel for Redmi Note 9 Pro (joyeuse)
### Intro
I just added some features for nethunter and fix when adding features.
### Features
Differences from stock are minimal:
* add support external adapter 
* Postgresql Support
* and others

## 1. Downloading
install some needed for ubuntu
```bash
sudo apt-get install clang libssl-dev make bison gcc ncurses-dev flex
```
Create a working folder, for example `kernel`:
```bash
mkdir kernel
cd kernel
```
Download kernel source:
```bash
git clone --depth=1 https://github.com/dhiimr/kernel_joyeuse_nethunter.git kernel_joyeuse_nethunter
```
Download a compatible GCC toolchain. I used UBERCT GCC 4.9 for 
[arm64](https://bitbucket.org/UBERTC/aarch64-linux-android-4.9/src/master/).
```bash
git clone https://bitbucket.org/UBERTC/aarch64-linux-android-4.9.git aarch64-linux-android
```
Download Clang toolchain. 
I used [Proton Clang](https://github.com/kdrag0n/proton-clang), you can use another suitable one, for example AOSP Clang.
```bash
git clone https://github.com/kdrag0n/proton-clang.git proton-clang
```
## 2. Building the kernel
Move to the kernel folder:
```bash
cd kernel_joyeuse_nethunter
```
Setup default config:
```bash
./build.sh nethunter_defconfig
```
**Note**: `build.sh` - a simple script that sets up environment variables and starts the compilation process.

**Optional**. You can tweak some kernel parameters:
```bash
./build.sh menuconfig
```

Compiling:
```bash
./build.sh
```
## 3. Flashing
If the compilation passed without errors, then in the `arch/arm64/boot` folder you will see the following files:
* `Image.gz` - kernel image
* `dtbo.img` - board-specific device tree overlay
* `dtb` - SoC device tree blob

### Preparation
These files must be flashed into the `boot` section of the phone. To do this, we will use the [AnyKernel3](https://github.com/osm0sis/AnyKernel3) utility.

Download AnyKernel3.zip, unpack and change the following lines in `anykernel.sh` file as shown below:
```
kernel.string=Kernel for Joyeuse
do.devicecheck=1
do.modules=0
do.systemless=1
do.cleanup=1
do.cleanuponabort=0
device.name1=joyeuse

# remove others lines like "device.name*"!

block=/dev/block/bootdevice/by-name/boot;
```
Also remove the lines from `# begin ramdisk changes` to `# end ramdisk changes`.

Place the files `Image.gz`, `dtbo.img` and `dtb` where the script is located and repack all the contents of the folder into a zip archive.

### Flashing
Reboot into recovery mode, backup the `boot` and `dtbo` partitions. Then install our zip archive. Reboot into the system.

* Note
if you want to enable monitor mode on wlan0 without installing nethunter type:
```bash
echo "4" > /sys/module/wlan/parameters/con_mode
```
Stop monitor mode
```bash
ip link set wlan0 down
echo "0" > /sys/module/wlan/parameters/con_mode
ip link set wlan0 up
```
### End
Thank you for :
* Allah SWT
* original Source [tifictive](https://github.com/tifictive)
