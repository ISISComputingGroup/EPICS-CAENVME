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
USR_SYS_LIBS_WIN32 += CAENVMElib

# copy relevant libraries locally to make building IOC easier later
# we copy CAENVMElib.dll PlxApi.dll via the O.$(T_A) as thet are needed there for the test runner
BIN_INSTALLS_WIN32 += CAENVMElib.dll PlxApi.dll
LIB_INSTALLS_WIN32 += CAENVMElib.lib

DBD += CAENVME.dbd

## old program from SECI, not used in IBEX, built for comparison/debugging
PROD_IOC += CAENTalk
CAENTalk_SRCS += CAENTalk.c getopt.c
CAENTalk_LIBS += $(EPICS_BASE_IOC_LIBS)

LIBRARY_IOC += CAENVME
CAENVME_SRCS += CAENVMEWrapper.cpp CAENVMEDriver.cpp
CAENVME_LIBS += asyn
CAENVME_LIBS_WIN32 += oncrpc
CAENVME_LIBS += $(EPICS_BASE_IOC_LIBS)

ifeq ($(findstring 10.0,$(VCVERSION)),)
GTESTPROD_HOST += runner
runner_SRCS += v895_tests.cpp CAENVMEWrapper.cpp
runner_LIBS += $(EPICS_BASE_IOC_LIBS)
GTESTS += runner
endif

TARGETS += CAENVMElib.lib CAENVMElib.dll PlxApi.dll

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
include $(GTEST)/cfg/compat.RULES_BUILD

# copy in libraries needed to run GTESTS into O.$(T_A)
ifdef T_A
CAENVMElib.lib : $(CAENVMEBASE)/lib/$(CAENARCH)/CAENVMElib.lib CAENVMElib.dll PlxApi.dll
	$(CP) -f $< $@

CAENVMElib.dll : $(CAENVMEBASE)/bin/$(CAENARCH)/CAENVMElib.dll
	$(CP) -f $< $@

PlxApi.dll : $(CAENVMEBASE)/bin/$(CAENARCH)/PlxApi.dll
	$(CP) -f $< $@
endif
