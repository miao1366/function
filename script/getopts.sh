
#!/bin/bash

# getopts ahfvc: option表明选项a、h、f、v可以不加实际值进行传递，而选项c必须取值。使用选项取值时，必须使用变量OPTARG保存该值

while getopts "a:" opt; do
  case $opt in
    a)
      echo "this is -a the arg is ! $OPTARG" 
      ;;
    \?)
      echo "Invalid option: -$OPTARG" 
      ;;
  esac
done