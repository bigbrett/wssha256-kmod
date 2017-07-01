# wssha256-kmod
Linux loadable kernel module supporting the wssha256 hardware accelerator, complete with userspace API and test programs. 

# Table of Contents
1. Overview
2. Dependencies 
1. Building the software
2. The Kernel Module
3. Userspace API Libraries
4. Userspace Test Program
5. Misc.

# Overview 
The wssha256 hardware accelerator targets the Xilinx Zynq7000 running the [linux-xlnx kernel](https://github.com/Xilinx/linux-xlnx "linux-xlnx"). This repository can be built in a number of ways: on its own on an x86 host system, natively on the Zynq (if you are insane), or in a cross-compiling environment. However, it was primarily designed to be integrated into a custom image built with the Yocto Linux build system. The Yocto layer that incorporates the software provided in this
repository can be found at [meta-wscryptohw](https://github.com/bigbrett/meta-wscryptohw "meta-wscryptohw")

# Dependedncies
* Linux (somewhat recent)
* CMake 
* Make 
* OpenSSL 

# Building the software
Before you can build the software ensure you have the dependencies installed.  

To build the module and supporting software targeting a host machine (I don't know why you would...maybe just to see if it builds?), do the following: 
```
$ git clone https://github.com/bigbrett/wssha256-kmod
$ cd wssha256-kmod
$ make
```
Do note that CMake does not perform any installation, as this will be handled by the Yocto recipe. The important output binaries are as follows:
* `wssha256-kmod/build/kmod/wssha256kern.ko`: the loadable kernel module
* `wssha256-kmod/build/lib/libwssha256-kmod.so`: Userspace API shared (dynamic) library 
* `wssha256-kmod/build/lib/libwssha256-kmod.so`: Userspace API static library
* `wssha256-kmod/build/test/wssha256-kmodtest`: Userspace test executable

# Usage 

# TODO 

# License

