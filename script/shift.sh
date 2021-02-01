#!/bin/bash

# ./shift.sh  a b c d e f g
while [ $# != 0 ];do
echo "第一个参数为：$1,参数个数为：$#"
shift
done