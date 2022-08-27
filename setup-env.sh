#!/bin/bash
#
# **********************************************************
# Copyright (c) 2022 Xuhpclab. All rights reserved.
# Licensed under the MIT License.
# See LICENSE file for more information.
# **********************************************************
#
# This tool will initialize the environment for developping clients of DrCCTProf on
# remote.eos.ncsu.edu
# First, it will auto initiallize spack and use spack to install and load gcc v7.5.0 and
# cmake v3.7.2 in '$HOME/drcctprof_env'. Then it will generate a e
# to create the virtual
# environment called 'cct' for developping clients of DrCCTProf. 

function eos_env_init() {
  local env_path="$1"
  if [ ! -d ${env_path} ]; then
    mkdir ${env_path}
  fi
  
  # Download spack and install dependece library
  git clone -c feature.manyFiles=true https://github.com/spack/spack.git ${env_path}/spack
  source ${env_path}/spack/share/spack/setup-env.sh
  spack config add "config:allow_sgid:false"
  spack install -j 6 gcc@7.5.0 %gcc@4.8.5 ^ncurses@6.0+symlinks
  spack install -j 6 cmake@3.7.2 %gcc@4.8.5 ^ncurses@6.0+symlinks

  # Write the config command to config file
  local env_config_file_path=${env_path}/drcctprof.env
  cat > ${env_config_file_path} <<-END
# Set the running and build environments for DrCCTProf on EOS machine
. ${env_path}/spack/share/spack/setup-env.sh
spack config add "config:allow_sgid:false"
spack load gcc@7.5.0
spack load cmake@3.7.2
END
}

if [ ! -f "$HOME/drcctprof_env/drcctprof.env" ]; then
  eos_env_init "$HOME/drcctprof_env"
fi
. $HOME/drcctprof_env/drcctprof.env
