#!/bin/bash

set -ex

clang++ -o generate_pdf  $(pkg-config --cflags --libs pangocairo)  generate_pdf.cc
