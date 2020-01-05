#ifndef CAENVMEDRIVER_H
#define CAENVMEDRIVER_H
 
#include "asynPortDriver.h"

class CAENVMEWrapper;

class CAENVMEDriver : public asynPortDriver 
{
public:
    CAENVMEDriver(const char *portName, int crate, int board_id, unsigned base_address, unsigned card_increment, bool simulate);
               
    // These are the methods that we override from asynPortDriver
    virtual asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);
    virtual asynStatus readUInt32Digital(asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask);
    virtual asynStatus drvUserCreate(asynUser *pasynUser, const char* drvInfo, const char** pptypeName, size_t* psize);
	virtual asynStatus drvUserDestroy(asynUser *pasynUser);
	virtual void report(FILE *f, int details);
	
private:
    CAENVMEWrapper* m_vme;
    unsigned m_baseAddress;
    unsigned m_cardIncrement;
	
	int P_crate;    // int
	int P_VMEWrite; // uint
	int P_VMERead;  // uint
	int P_boardId;  // int
	#define FIRST_CAENVME_PARAM P_crate
	#define LAST_CAENVME_PARAM  P_boardId
};

#define NUM_CAENVME_PARAMS (&LAST_CAENVME_PARAM - &FIRST_CAENVME_PARAM + 1)

#define P_crateString	 "CRATE"
#define P_VMEWriteString "VMEWRITE"
#define P_VMEReadString  "VMEREAD"
#define P_boardIdString	 "BOARDID"

#endif /* CAENVMEDRIVER_H */
