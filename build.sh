#!/bin/bash

case "$1" in
    "-b" )
      echo "Building..."
      # 执行构建操作
      cmake -S . -B build
      cd ./build
      make
      echo "Done"
      ;;
    "-c" )
      echo "Cleaning..."
      # 执行清除操作，直接一次
      rm -rf ./bin ./build
      echo "Done"
      ;;
    *)
      echo "Invalid option: $1"
      exit 1
      ;;
  esac
exit 0

