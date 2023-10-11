NOTE: Wayland is not tested yet. Please use X11 on Linux.

BUILD (Linux & macos)
git clone https://github.com/Kurapikov/KCAD.git
cd KCAD
git submodule init
git submodule update
cd 3rdparty_build
#if use proxy:
  cmake . -DUSE_HTTPS_PROXY=ON -DHTTPS_PROXY_ADDRESS=localhost:1080
#else
  cmake ../3rdparty_build
#endif
make build_3rdparty
cd ..
mkdir build
cd build
cmake ..
make -j8

You can find KCAD executable under KCAD/build/bin
