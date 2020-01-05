TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

### NOTE: there should only be one build.mak for a given IOC family and this should be located in the ###-IOC-01 directory

#=============================
# Build the IOC application _APPNAME_
# We actually use $(APPNAME) below so this file can be included by multiple IOCs

PROD_IOC = $(APPNAME)
# _APPNAME_.dbd will be created and installed
DBD += $(APPNAME).dbd

# _APPNAME_.dbd will be made up from these files:
$(APPNAME)_DBD += base.dbd
## ISIS standard dbd ##
$(APPNAME)_DBD += CAENVME.dbd
$(APPNAME)_DBD += asyn.dbd
$(APPNAME)_DBD += busySupport.dbd
$(APPNAME)_DBD += calcSupport.dbd
$(APPNAME)_DBD += asSupport.dbd

# Add all the support libraries needed by this IOC

## Add additional libraries here ##
#$(APPNAME)_LIBS += xxx

$(APPNAME)_LIBS += CAENVME CAENVMElib
$(APPNAME)_LIBS += asyn
$(APPNAME)_LIBS += autosave
$(APPNAME)_LIBS += busy
$(APPNAME)_LIBS += calc sscan
ifdef ONCRPC
$(APPNAME)_LIBS_WIN32 += oncrpc
endif

# _APPNAME__registerRecordDeviceDriver.cpp derives from _APPNAME_.dbd
$(APPNAME)_SRCS += $(APPNAME)_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
$(APPNAME)_SRCS_DEFAULT += $(APPNAME)Main.cpp
$(APPNAME)_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#$(APPNAME)_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
$(APPNAME)_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

