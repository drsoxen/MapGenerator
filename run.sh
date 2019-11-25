#!/bin/sh
g++ -std=c++14 *.cpp && ./a.out &&  open out.ppm; rm -r a.out
