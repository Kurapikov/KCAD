# NOTE
1. Wayland is not tested yet. Please use X11 on Linux.

# BUILD (Linux & macos)
git clone https://github.com/Kurapikov/KCAD.git
cd KCAD
git submodule init
git submodule update
cd 3rdparty_build
#if use proxy
&nbsp;&nbsp; &nbsp; &nbsp; cmake . -DUSE_HTTPS_PROXY=ON -DHTTPS_PROXY_ADDRESS=localhost:1080
#else
&nbsp; &nbsp; &nbsp; &nbsp; cmake ../3rdparty_build
#endif
make build_3rdparty
cd ..
mkdir build
cd build
cmake ..
make -j8

Then you can find KCAD executable under KCAD/build/bin

# TASKS
- [X] Port to MacOS.
- [ ] Port to Windows.
- [ ] Use SimpleIni lib to load/save system preference.
- [ ] Config spdlog lib to get a singleton mt_safe logger. Would be better to toggle both file/console log though .ini file.
- [ ] Draw some triangles.
- [ ] Integrate bgfx with OCCT, open and draw a step file.

