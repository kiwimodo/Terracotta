A simple program that will load a shared object into RAM and execute it's `run` function.

To try the simple loader, first compile the demo shared object with:
    cd Terracotta/simple-loader/libdemo
    make

Then compile the loader with:
    cd Terracotta/simple-loader
    make SO_PATH=./libdemo/build/libdemo.so

OR if target's host doesn't support memfd_create (e.g. kernel version < 3.17):
    cd Terracotta/simple-loader
    make MACRO=-DUSE_SHM SO_PATH=./libdemo/build/libdemo.so