#!/bin/bash

blockMesh -case testCase | tee testCase/log.blockMesh
echo "Meshing done."

OF01IO -case testCase | tee testCase/log.run
