TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================

CAENVMEBASE = $(ICPBINARYDIR)/CAENVME/Windows
ifneq ($(findstring windows,$(EPICS_HOST_ARCH)),)
CAENARCH=x64
else
CAENARCH=x86
endif

USR_CPPFLAGS_WIN32 += -I$(CAENVMEBASE)/include -DWIN32
USR_CFLAGS_WIN32 += -I$(CAENVMEBASE)/include -DWIN32
USR_SYS_LIBS_WIN32 += $(CAENVMEBASE)/lib/$(CAENARCH)/CAENVMElib

# copy relevant libraries locally to make building IOC easier later
BIN_INSTALLS_WIN32 += $(CAENVMEBASE)/bin/$(CAENARCH)/CAENVMElib.dll $(CAENVMEBASE)/bin/$(CAENARCH)/PlxApi.dll
LIB_INSTALLS_WIN32 += $(CAENVMEBASE)/lib/$(CAENARCH)/CAENVMElib.lib

DBD += CAENVME.dbd

PROD_IOC += CAENTalk
CAENTalk_SRCS += CAENTalk.c getopt.c
CAENTalk_LIBS += $(EPICS_BASE_IOC_LIBS)

LIBRARY_IOC += CAENVME
CAENVME_SRCS += CAENVMEWrapper.cpp CAENVMEDriver.cpp
CAENVME_LIBS += asyn
CAENVME_LIBS_WIN32 += oncrpc
CAENVME_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

