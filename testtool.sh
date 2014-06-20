#!/bin/bash 

if [ ! -f "./build/src/si_service" ];then
    echo "Please build our project first"
fi

tool="./build/src/si_service"
config="./src/secureindex.ini"

$tool $config<<EOF
test README.md 1234
EOF

