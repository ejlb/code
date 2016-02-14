#!/bin/sh

/bin/ls /bin/ | while read a ; do ./detector /bin/$a ; done
