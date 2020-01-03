TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================

CAENVMEBASE = $(ICPBINARYDIR)/CAENVME/Windows

USR_CPPFLAGS_WIN32 += -I$(CAENVMEBASE)/include -DWIN32
USR_CFLAGS_WIN32 += -I$(CAENVMEBASE)/include -DWIN32
USR_SYS_LIBS_WIN32 += $(CAENVMEBASE)/lib/CAENVMElib
BIN_INSTALLS_WIN32 += $(CAENVMEBASE)/bin/CAENVMElib.dll $(CAENVMEBASE)/bin/PlxApi.dll

DBD += CAENVME.dbd

PROD_IOC += CAENTalk
CAENTalk_SRCS += CAENTalk.c getopt.c
CAENTalk_LIBS += $(EPICS_BASE_IOC_LIBS)

LIBRARY_IOC += CAENVME
CAENVME_SRCS += CAENVMEWrapper.cpp
CAENVME_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

