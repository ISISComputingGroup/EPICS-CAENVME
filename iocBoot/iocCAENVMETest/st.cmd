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

set_savefile_path("$(TOP)/iocBoot/$(IOC)","autosave")
set_requestfile_path("$(TOP)/iocBoot/$(IOC)", "")
set_requestfile_path("$(AUTOSAVE)/asApp/Db", "")

#set_pass0_restoreFile("auto_positions.sav")
set_pass0_restoreFile("auto_settings.sav")
set_pass1_restoreFile("auto_settings.sav")

set_pass0_restoreFile("info_positions.sav")
set_pass0_restoreFile("info_settings.sav")
set_pass1_restoreFile("info_settings.sav")

## setting this turns on simulation mode in CAENVMEWrapper
## we pass its value via CAENVMEConfigure()
epicsEnvSet("CAEMVMESIM", 1)

## create an instance for each VME crate
## crate is the logical crate number we want to refer to it by, board_id is the crate number that the crate has been assigned
## by the USB. If a usb board in the VME crate was changed for example then it may appear with a different board_id but we are
## able here to map it back to our own crate number so the PVs remain the same.  
## CAENVMEConfigure(const char *portName, int crate, int board_id, unsigned base_address, unsigned card_increment, bool simulate)

CAENVMEConfigure("CRATE0", 0, 0, 0, 0x10000, $(CAEMVMESIM=0))
CAENVMEConfigure("CRATE1", 1, 1, 0, 0x10000, $(CAEMVMESIM=0))

## Load our record instances - here for two crates with two cards
dbLoadRecords("$(TOP)/db/v895Crate.db","P=$(MYPVPREFIX),Q=CAENVMETEST:,CRATE=0,PORT=CRATE0")
dbLoadRecords("$(TOP)/db/v895Card.db","P=$(MYPVPREFIX),Q=CAENVMETEST:,CRATE=0,PORT=CRATE0,C=0")
dbLoadRecords("$(TOP)/db/v895Card.db","P=$(MYPVPREFIX),Q=CAENVMETEST:,CRATE=0,PORT=CRATE0,C=1")

dbLoadRecords("$(TOP)/db/v895Crate.db","P=$(MYPVPREFIX),Q=CAENVMETEST:,CRATE=1,PORT=CRATE1")
dbLoadRecords("$(TOP)/db/v895Card.db","P=$(MYPVPREFIX),Q=CAENVMETEST:,CRATE=1,PORT=CRATE1,C=0")
dbLoadRecords("$(TOP)/db/v895Card.db","P=$(MYPVPREFIX),Q=CAENVMETEST:,CRATE=1,PORT=CRATE1,C=1")

## load autosave configMenu for managing sets of PVs
dbLoadRecords("$(AUTOSAVE)/db/configMenu.db","P=$(MYPVPREFIX)CAENVMETEST:,CONFIG=vmeconfig")
 
cd "${TOP}/iocBoot/${IOC}"
iocInit

makeAutosaveFileFromDbInfo("vmeconfig_settings.req", "vmeconfig")
create_manual_set("vmeconfigMenu.req","P=$(MYPVPREFIX)CAENVMETEST:,CONFIG=vmeconfig,CONFIGMENU=1")

create_monitor_set("auto_settings.req", 30, "P=$(MYPVPREFIX)CAENVMETEST:")

# Handle autosave 'commands' contained in loaded databases.
makeAutosaveFiles()
create_monitor_set("info_positions.req", 5, "P=$(MYPVPREFIX)CAENVMETEST:")
create_monitor_set("info_settings.req", 30, "P=$(MYPVPREFIX)CAENVMETEST:")

## Start any sequence programs
#seq sncxxx,"user=_USER_"
