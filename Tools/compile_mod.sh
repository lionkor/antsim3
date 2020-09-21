#!/bin/bash

gcc -std=c11 -shared -fPIC -Os -Wall -Wextra --pedantic --pedantic-errors -o $*
