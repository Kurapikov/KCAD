# NOTE
1. Wayland is not tested yet. Please use X11 on Linux.
2. Windows is not ported yet.

# BUILD (Linux & macos)
git clone https://github.com/Kurapikov/KCAD.git  
cd KCAD  
git submodule init  
git submodule update  
cd 3rdparty_build  
#if use proxy  
&nbsp;&nbsp;&nbsp;&nbsp;cmake . -DUSE_HTTPS_PROXY=ON -DHTTPS_PROXY_ADDRESS=localhost:1080  
#else  
&nbsp;&nbsp;&nbsp;&nbsp;cmake .  
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
- [ ] More flexible 3rd-party build script, check downloads and installations exist first.
- [ ] Fix VSCode environment.
- [ ] Use Ninja as backends.
- [ ] Port to Windows.
- [ ] Integrate with github CI tools to verify cross platform build.
- [ ] Use SimpleIni lib to load/save system preference.
- [ ] Config spdlog lib to get a singleton mt_safe logger. Would be better to toggle both file/console log though .ini file.
- [ ] Draw some triangles.
- [ ] Put BGFX and ImGUI into different threads.
- [ ] Change sync mode to render "on demand" from "60Hz V-Sync".
- [ ] Integrate bgfx with OCCT, open and draw a step file.

