# wssha256-kmod
Linux loadable kernel module supporting the wssha256 hardware accelerator, complete with userspace API and test programs. 

# Table of Contents
1. Overview
2. Dependencies 
3. Building the software
4. Usage
5. Misc.

# 1. Overview 
The wssha256 hardware accelerator targets the Xilinx Zynq7000 running the [linux-xlnx kernel](https://github.com/Xilinx/linux-xlnx "linux-xlnx"). This repository can be built in a number of ways: on its own on an x86 host system, natively on the Zynq (if you are insane), or in a cross-compiling environment. However, it was primarily designed to be integrated into a custom image built with the Yocto Linux build system. The Yocto layer that incorporates the software provided in this
repository can be found at [meta-wscryptohw](https://github.com/bigbrett/meta-wscryptohw "meta-wscryptohw")

# 2. Dependencies
* Linux (somewhat recent)
* CMake 
* Make 
* OpenSSL 

# 3. Building the Software
Before you can build the software ensure you have the dependencies installed.  

To build the module and supporting software targeting a host machine (I don't know why you would...maybe just to see if it builds?), do the following: 
```
$ git clone https://github.com/bigbrett/wssha256-kmod
$ cd wssha256-kmod
$ make
```
Do note that CMake does not perform any installation, as this is expected to be handled by the Yocto recipe. Why? because I'm lazy. The important output binaries are as follows:
* `wssha256-kmod/build/kmod/wssha256kern.ko`:           the loadable kernel module
* `wssha256-kmod/build/lib/libwssha256-uapi.so`:        Userspace API shared (dynamic) library 
* `wssha256-kmod/build/lib/libwssha256-uapi-static.a`:  Userspace API static library
* `wssha256-kmod/build/test/wssha256-kmodtest`:         Userspace test executable

# 4. Usage 
### Loadable Kernel Module
* Before loading the kernel module, ensure the FPGA bitstream is programmed correctly. **If the wssha256 hardware block is not accessible in the memory map, then loading the kernel module will cause a kernel panic!!**
* Load the module using `$ insmod /path/to/wssha256-kmod/build/kmod/wssha256kern.ko`. 
* You can disable the kernel buffer printing to stdout by running `dmesg -D` (and re-enable with `dmesg -E`). 

### Userspace API Shared Library 
To link a C program against the shared library, just use the standard steps. E.G. when compiling foo.c, which uses the library: `$ gcc foo.c -L/path/to/lib -lwssha256-uapi`. And don't forget to set `LD_LIBRARY_PATH` to `/path/to/lib` if the library is not located at /usr/lib!

### Userspace API Static Library 
To link a C program against the static library, just use the standard steps. E.G. when compiling foo.c, which uses the library: `$ gcc foo.c libwssha256uapi.a`

### Userspace Test Executable
`wssha256-kmodtest` is a self-checking test program that can be run in userspace to test the correct operation of the LKM. Before you run it, ensure that:
1. The bitstream containing the hardware is programmed. This can be done through Vivado on the host machine, or on the target by running `$ cat /path/to/bitstream.bit > /dev/xdevcfg`
2. The wssha256kern.ko module is loaded. If not, load it with `$ insmod /path/to/wssha256-kmod/build/kmod/wssha256kern.ko`. 

To run the test program, just invoke the executable.

# 5. Misc. 

