A simple program that will load a shared object into RAM and execute it's run function.



To try the local loader, first compile the demo shared object with:

cd REPO/simple-loader/libdemo
make


Then compile the loader with:

cd REPO/simple-loader
make SO_PATH=./libdemo/build/libdemo.so