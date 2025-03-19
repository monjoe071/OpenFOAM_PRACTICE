#!/bin/bash

wclean
rm -rf testCase/log.*
foamCleanTutorials testCase
echo "Cleaning done."