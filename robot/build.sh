#!/bin/bash

RPLIDARSDK="v1.12.0"
OS=`uname -s`

echo "Downloading Slamtec RPLIDAR SDK"
curl -LOs https://github.com/Slamtec/rplidar_sdk/archive/refs/tags/release/${RPLIDARSDK}.tar.gz
echo

echo "Unpacking RPLIDAR SDK"
tar xfz "${RPLIDARSDK}.tar.gz"
rm "${RPLIDARSDK}.tar.gz"
echo

echo "Building Slamtec RPLIDAR SDK library for ${OS}"
pushd . > /dev/null
cd rplidar_sdk-release-${RPLIDARSDK}/sdk
make clean >& /dev/null
make >& /dev/null
popd > /dev/null
echo

echo "Copying headers and library"
mkdir -p include
mkdir -p include/hal
mkdir -p lib
cp "rplidar_sdk-release-${RPLIDARSDK}/sdk/output/${OS}/Release/librplidar_sdk.a" lib
cp rplidar_sdk-release-${RPLIDARSDK}/sdk/sdk/include/* include
cp rplidar_sdk-release-${RPLIDARSDK}/sdk/sdk/src/hal/* include/hal
cp "rplidar_sdk-release-${RPLIDARSDK}/LICENSE" lib
cp "rplidar_sdk-release-${RPLIDARSDK}/LICENSE" include
echo

