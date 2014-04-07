#!/bin/bash

ruby ext/extconf.rb --with-narray-include=/home/magpie/.rvm/gems/ruby-2.1.1/gems/narray-nmatrix-0.6.1.0.pre
make
make install


