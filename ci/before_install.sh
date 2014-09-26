# Set github and bitbucket free of host checking questions
echo -e "Host github.com\n\tStrictHostKeyChecking no\n" >> ~/.ssh/config
echo -e "Host bitbucket.org\n\tStrictHostKeyChecking no\n" >> ~/.ssh/config

# Install DART
sudo apt-add-repository --yes ppa:libccd-debs
sudo apt-add-repository --yes ppa:fcl-debs
sudo apt-add-repository --yes ppa:dartsim
sudo apt-get update
sudo apt-get install libdart4-dev

APT='
freeglut3-dev
libboost-all-dev
libglu1-mesa-dev
libgtest-dev
libopenscenegraph-dev
libqt4-dev
libxmu-dev
libxi-dev
libois-dev
libogre-1.7.4-dev
'

sudo apt-get install $APT
