# Install DART
sudo apt-add-repository --yes ppa:libccd-debs
sudo apt-add-repository --yes ppa:fcl-debs
sudo apt-add-repository --yes ppa:dartsim
sudo apt-get update

APT='
freeglut3-dev
libboost-all-dev
libglu1-mesa-dev
libgtest-dev
libopenscenegraph-dev
libqt4-dev
libxmu-dev
libxi-dev
libdart6-all-dev
qt5-default
liburdfdom-dev
'

sudo apt-get -qq --yes --force-yes install $APT
