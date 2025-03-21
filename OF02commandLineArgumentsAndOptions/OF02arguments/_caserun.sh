#!/bin/bash

blockMesh -case testCase | tee testCase/log.blockMesh
echo "Meshing done."

OF02arguments -case testCase | tee testCase/log.run
