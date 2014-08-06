#!/bin/bash
#
#     Linux-Installer.sh
#
#     An install script for Fedora and OpenSUSE Systems.and Debian.
#     Installs to home directory
#
#     Copyright (c) 2014
#
#      Author:
#              Magdalen Berns
#     Contact:
#              <m.berns@thismagpie.com>

# Check to see whether the installer is zypper.
# If so, then install necessary packages.
if [ -x /usr/bin/zypper ] ; then
    echo ""
    echo "Going to install the following packages:"
    echo "gcc gcc-c++ curl and cpupower."
    echo "Login as root, now..."
    sudo zypper in -y gcc gcc-c++ curl cpupower
    sudo cpupower frequency-set -g performance
    echo
    echo "Going to install the following packages:"
    echo "libatlas3 libatlas3-devel blas-devel ruby-devel laptack-devel fftw-devel"
    sudo zypper in -y libatlas3-devel blas-devel ruby-devel lapack-devel fftw-devel
elif [ -f /usr/bin/yum ] ; then
    echo ""
    echo "Login as root, now..."
    sudo yum install gcc gcc-c++ curl cpupower
    sudo cpupower frequency-set -g performance
    echo
    echo "Going to install the following packages:"
    echo "libatlas3 libatlas3-devel blas-devel ruby-devel lapack-devel fftw-devel"
    sudo yum install atlas-devel blas-devel ruby-devel lapack-devel fftw-devel
fi

# Check to see whether the installer is apt-get
# If so, then install necessary packages.
if [ -f /usr/bin/apt-get ] ; then
    echo ""
    source ~/.bash_profile
    echo "Login as root, now..."
    sudo apt-get install gcc gcc-c++ curl cpufrequtils
    sudo cpufreq-set -g performance
    echo
    echo "Going to install the following packages:"
    echo "libatlas3 and libatlas-dev ruby-dev fftw-dev"
    sudo apt-get install libatlas-dev ruby-dev liblapack-dev
fi

echo "Installation for $s complete!"
echo "Setting PATHS"

if [ -d /usr/include/atlas ] ; then
    echo "Setting CPLUS_INCLUDE_PATH in ~/.bashrc"
    echo "export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/include/atlas:$GEM_HOME/gems/fftw/ext/fftw/fftw3/api" >> ~/.bashrc
    echo "Setting C_INCLUDE_PATH in ~/.bashrc"
    echo "export C_INCLUDE_PATH=$C_INCLUDE_PATH:/usr/include/atlas:GEM_HOME/gems/fftw/ext/fftw3/api" >> ~/.bashrc
    echo "source ~/.bashrc"
fi

if [ -d ../fftw ]; then
  echo "FFTW found!"
  if [ -f  Gemfile ] ; then
    bundle install
    bundle exec rake compile -- --with-lapacklib
    bundle exec rake spec
   fi
fi

