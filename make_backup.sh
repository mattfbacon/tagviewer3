#!/bin/bash

set -euxo pipefail

mkdir -p ../tagviewer3backup
rsync --recursive . ../tagviewer3backup --delete -rl
diff -r . ../tagviewer3backup -q
