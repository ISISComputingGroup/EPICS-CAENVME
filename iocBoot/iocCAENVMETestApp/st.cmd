#!../../bin/windows-x64-debug/CAENVMETest

## You may have to change CAENVMETest to something else
## everywhere it appears in this file

# Increase this if you get <<TRUNCATED>> or discarded messages warnings in your errlog output
errlogInit2(65536, 256)

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/CAENVMETest.dbd"
CAENVMETest_registerRecordDeviceDriver pdbbase

epicsEnvSet("CAEMVMESIM", 1)
CAENVMEConfigure("CRATE0", 0, 0, 0, 0x10000, $(CAEMVMESIM))

## Load our record instances - here for one crate with two cards
dbLoadRecords("$(TOP)/db/v895Crate.db","P=$(MYPVPREFIX),Q=CAENVMETEST:,CRATE=0,PORT=CRATE0")
dbLoadRecords("$(TOP)/db/v895Card.db","P=$(MYPVPREFIX),Q=CAENVMETEST:,CRATE=0,PORT=CRATE0,C=0")
dbLoadRecords("$(TOP)/db/v895Card.db","P=$(MYPVPREFIX),Q=CAENVMETEST:,CRATE=0,PORT=CRATE0,C=1")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=_USER_"
