#!/bin/bash
TOOLS_DEVTREE=../../tools/Air_Vantage_Dev_Tree_v2
TOOLS_FWUPDAT=../../tools/Air_Vantage_fwupdate 

SLQSHOSTTYPE=`uname -m`

CPU=$1
UPPER_CPU=${CPU^^}

BUILD_FOLDER=build.$CPU
CONFIG_FILE_PATH=../../build/bin/$CPU

if [ -z $1  ]
then
	BUILD_FOLDER=build.default
	CONFIG_FILE_PATH=../../build/bin/host$SLQSHOSTTYPE
fi;


CONFIG_FILE_TAG='AVA_PATH'
CONFIG_FILE_NAME=$CONFIG_FILE_PATH/.sdk_config


echo "--------------------------"
echo "---- Building AVAgent ----"
echo "--------------------------"

sudo rm -rf $BUILD_FOLDER
if [ -z $1  ]
then
./avagent_r8m/bin/build.sh
(cd $TOOLS_DEVTREE; make clean; make ;)
(cd $TOOLS_FWUPDAT; make clean; make ;)
else
./avagent_r8m/bin/build.sh -t $CPU

if [ -d $TOOLS_DEVTREE ]; then
  (cd $TOOLS_DEVTREE; make clean; make CPU=$CPU ;)
  if [ $? -ne 0 ]
  then
      echo $TOOLS_DEVTREE failed
      exit 1
  fi
fi

if [ -d $TOOLS_FWUPDAT ]; then
  (cd $TOOLS_FWUPDAT; make clean; make CPU=$CPU ;)
  if [ $? -ne 0 ]
  then
      echo $TOOLS_FWUPDAT failed
      exit 1
  fi
else
  cp ../../build/bin/$CPU/libfwupdate.so $PWD/$BUILD_FOLDER/runtime/lib
fi

fi;
#DevTree handler & Firmware update asset
(cd $BUILD_FOLDER; make devtree2_install slqs_fw_update ;)
if [ $? -ne 0 ]
then
    echo $BUILD_FOLDER failed
    exit 1
fi
#enable security (http://wiki.eclipse.org/Mihini/Security)
(cd $BUILD_FOLDER; make all agent_provisioning;)
if [ $? -ne 0 ]
then
    echo $BUILD_FOLDER agent_provisioning failed
    exit 1
fi
echo "$CONFIG_FILE_TAG=$PWD/$BUILD_FOLDER/runtime" > $CONFIG_FILE_NAME
echo "$CONFIG_FILE_TAG=NO_AVA" >> $CONFIG_FILE_NAME
if [ -z $1  ]
then
cp ../../build/bin/host$SLQSHOSTTYPE/slqssdk $PWD/$BUILD_FOLDER/runtime/bin
else
cp ../../build/bin/$CPU/slqssdk $PWD/$BUILD_FOLDER/runtime/bin
fi;
