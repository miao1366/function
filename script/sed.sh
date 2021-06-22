#!/bin/bash
VERSION_JSON=`cat sed.txt | sed 's/''"'/'\\\"''/g'`
echo $VERSION_JSON
