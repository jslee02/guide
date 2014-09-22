# Set github and bitbucket free of host checking questions
echo -e "Host github.com\n\tStrictHostKeyChecking no\n" >> ~/.ssh/config
echo -e "Host bitbucket.org\n\tStrictHostKeyChecking no\n" >> ~/.ssh/config

# Install DART
git clone https://github.com/dartsim/dart.git
(cd dart; ci/before_install.sh; cmake .; make && sudo make install)

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
