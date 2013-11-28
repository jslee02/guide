before_install() {
  cd /tmp
  PROJECTS=''
  for REPO in $PROJECTS
  do
    git clone git://github.com/dartsim/$REPO.git
    (cd $REPO; cmake .; make && sudo make install)
  done

  # Set github and bitbucket free of host checking questions
  echo -e "Host github.com\n\tStrictHostKeyChecking no\n" >> ~/.ssh/config
  echo -e "Host bitbucket.org\n\tStrictHostKeyChecking no\n" >> ~/.ssh/config

  # Install DART
  git clone https://github.com/dartsim/dart.git
  (cd dart; ci/before_install.sh; cmake .; make && sudo make install)
}

APT='libboost-all-dev cmake cmake-curses-gui libgtest-dev libqt4-dev libopenscenegraph-dev'

sudo apt-get install $APT
(before_install)
