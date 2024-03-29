#include <map>
#include <bitset>
#include <string>
#include <iostream>
#include <epicsThread.h>

#include "CAENVMEWrapper.h"

#define CHECK_RET(__func) \
    int ret = (__func); \
	if ((int)cvSuccess != ret) \
    { \
	     std::string errmsg(#__func); \
		 errmsg += ": "; \
		 errmsg += CAENVMEWrapper::decodeError(ret); \
	     throw std::runtime_error(errmsg); \
	}

#define CHECK_INIT \
        if (!m_init_ok) \
        { \
            throw std::runtime_error("CAENVME not initialised"); \
        }

static double sim_read_delay = 0.1;
static double sim_write_delay = 0.1;

const char* CAENVMEWrapper::decodeError(int code)
{
    return CAENVME_DecodeError(static_cast<CVErrorCodes>(code));
}

	CAENVMEWrapper::CAENVMEWrapper(bool simulate, CVBoardTypes BdType, short Link, short BdNum) : m_simulate(simulate), m_BdType(BdType), m_BdNum(BdNum), m_handle(0), m_init_ok(true)
	{
        const int MAXCARDS = 10;
		if (simulate)
		{
			std::cerr << "Creating simlated CAENVME for v895 module" << std::endl;
			std::bitset<16> mod_code(std::string("0000100001010100")); // Fixed manufacturer and model number for CAEN v895
            for(int i=0; i<MAXCARDS; ++i) {
			    m_simDataMap[0xFA + i * 0x10000] = 0xFAF5; // fixed code
			    m_simDataMap[0xFC + i * 0x10000] = mod_code.to_ulong();
			    m_simDataMap[0xFE + i * 0x10000] = 0x1234; // version and serial number of board
            }
		}
		else
		{
            int ret = CAENVME_Init(BdType, Link, BdNum, &m_handle);
            if ((int)cvSuccess != ret)
            {
                m_init_ok = false;
                std::cerr << "CAENVME: failed to initialise: " << CAENVMEWrapper::decodeError(ret) << std::endl;
            }
		}
	}
	
    std::string CAENVMEWrapper::swRelease()
	{
        char buffer[64];
	    CHECK_RET(CAENVME_SWRelease(buffer));
	    return buffer;
	}
	
    std::string CAENVMEWrapper::boardFWRelease()
	{
        char buffer[64];
        if (!m_init_ok)
        {
            return "UNKNOWN";
        }
		if (!m_simulate)
		{
	        CHECK_RET(CAENVME_BoardFWRelease(m_handle, buffer));
	        return buffer;
		}
		else
		{
			return "SIM";
		}
	}

    std::string CAENVMEWrapper::driverRelease()
    {
        char buffer[64];
        if (!m_init_ok)
        {
            return "UNKNOWN";
        }
		if (!m_simulate)
		{
	        CHECK_RET(CAENVME_DriverRelease(m_handle, buffer));
	        return buffer;
		}
		else
		{
			return "SIM";
		}
	}

    CAENVMEWrapper::~CAENVMEWrapper()
	{
		if (m_init_ok && !m_simulate)
		{
	        CAENVME_End(m_handle);
		}
    }

    void CAENVMEWrapper::systemReset()
    {
        CHECK_INIT;
		if (!m_simulate)
		{
            CHECK_RET(CAENVME_SystemReset(m_handle));
		}
	}
	
    int CAENVMEWrapper::dataSizeBytes(CVDataWidth DW)
	{
		switch(DW)
		{
			case cvD8:
			    return 1;
			case cvD16:
			    return 2;
			case cvD32:
			    return 4;
			case cvD64:
			    return 8;
			default:
			    return 0;
		}			    
	}

	uint64_t CAENVMEWrapper::getData(const void *data, CVDataWidth DW)
	{
		switch(DW)
		{
			case cvD8:
			    return *(uint8_t*)data;
			case cvD16:
			    return *(uint16_t*)data;
			case cvD32:
			    return *(uint32_t*)data;
			case cvD64:
			    return *(uint64_t*)data;
			default:
			    return 0;
		}			    
	}

	void CAENVMEWrapper::setData(uint64_t value, void *data, CVDataWidth DW)
	{
		switch(DW)
		{
			case cvD8:
			    *(uint8_t*)data = (uint8_t)value;
				break;
			case cvD16:
			    *(uint16_t*)data = (uint16_t)value;
				break;
			case cvD32:
			    *(uint32_t*)data = (uint32_t)value;
				break;
			case cvD64:
			    *(uint64_t*)data = (uint64_t)value;
				break;
			default:
			    break;
		}			    
	}

    void CAENVMEWrapper::readCycle(uint32_t address, void *data, CVAddressModifier AM, CVDataWidth DW)
    {
        CHECK_INIT;
		if (!m_simulate)
		{
	        CHECK_RET(CAENVME_ReadCycle(m_handle, address, data, AM, DW));
		}
		else
		{
			setData(m_simDataMap[address], data, DW); 			
            epicsThreadSleep(sim_read_delay);
		}
    }

    void CAENVMEWrapper::writeCycle(uint32_t address, const void *data, CVAddressModifier AM, CVDataWidth DW)
    {
        CHECK_INIT;
		if (!m_simulate)
		{
	        CHECK_RET(CAENVME_WriteCycle(m_handle, address, const_cast<void*>(data), AM, DW));
		}
		else
		{
			m_simDataMap[address] = getData(data, DW);
			std::cerr << "CAENSIM: writing " << m_simDataMap[address] << " to address 0x" << std::hex << address << std::dec << std::endl;
            epicsThreadSleep(sim_write_delay);
		}
    }

void CAENVMEWrapper::writeSequence(CVDataWidth DW, uint32_t address, uint32_t data, CVAddressModifier AM, int count, int data_step)
{
  for (int i=0; i < count; ++i)
  {
	writeCycle(address,&data,AM,DW);
	data += data_step;
	address += dataSizeBytes(DW);
  }
}

void CAENVMEWrapper::writeArray(CVDataWidth DW, uint32_t address, const void* data, 
						 CVAddressModifier AM, int count)
{
    char* cdata = (char*)data;
	int incr = dataSizeBytes(DW);
    for (int i=0; i < count; ++i)
    {
		writeCycle(address,cdata,AM,DW);
	    address += incr;
		cdata += incr;
   }
}

void CAENVMEWrapper::readArray(CVDataWidth DW, uint32_t address, void* data, 
						 CVAddressModifier AM, int count)
{
    char* cdata = (char*)data;
	int incr = dataSizeBytes(DW);
    for (int i=0; i < count; ++i)
    {
		readCycle(address,cdata,AM,DW);
	    address += incr;
		cdata += incr;
   }
}

void CAENVMEWrapper::report(FILE* f)
{
	fprintf(f, "CAENVMEWrapper: init ok: %s\n", (m_init_ok ? "YES" : "NO"));
	fprintf(f, "CAENVMEWrapper: connected to board number %d (board type: %d)\n", (int)m_BdNum, (int)m_BdType);
	fprintf(f, "CAENVMEWrapper: swRelease = %s\n", swRelease().c_str());
	fprintf(f, "CAENVMEWrapper: boardFWRelease = %s\n", boardFWRelease().c_str());
	fprintf(f, "CAENVMEWrapper: driverRelease = %s\n", driverRelease().c_str());
	if (m_simulate)
	{
	    for(std::map<uint32_t,uint64_t>::const_iterator it = m_simDataMap.begin(); it != m_simDataMap.end(); ++it)
	    {
		    fprintf(f, "CAENSIM: address 0x%x has value %llu\n", it->first, (unsigned long long)it->second);
	    }
	}
}	

