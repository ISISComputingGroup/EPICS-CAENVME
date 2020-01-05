#ifndef CAENVMEWRAPPER_H
#define CAENVMEWRAPPER_H

#include "CAENVMElib.h"

/// class to manage interaction with vendor CAENVMElib
class CAENVMEWrapper
{
	bool m_simulate; ///< true if we in VME simulation mode
	int32_t m_handle; ///< reference returned by underlying CAENVMElib library
    std::map<uint32_t,uint64_t> m_simDataMap; ///< holds address -> value mapping in simulation mode

    static const char* decodeError(int code); ///< decode error number returned by vendor CAENVMElib
	static uint64_t getData(const void *data, CVDataWidth DW);
	static void setData(uint64_t value, void *data, CVDataWidth DW);
	int dataSizeBytes(CVDataWidth DW);

	public:

	CAENVMEWrapper(bool simulate, CVBoardTypes BdType, short Link, short BdNum);
	static std::string swRelease();
    std::string boardFWRelease();
    std::string driverRelease();
    ~CAENVMEWrapper();
    void systemReset();
    void readCycle(uint32_t address, void *data, CVAddressModifier AM, CVDataWidth DW);
    void writeCycle(uint32_t address, const void *data, CVAddressModifier AM, CVDataWidth DW);
    void writeSequence(CVDataWidth DW, uint32_t address, uint32_t data, CVAddressModifier AM, int count, int data_step);
    void writeArray(CVDataWidth DW, uint32_t address, const void* data, CVAddressModifier AM, int count);
    void readArray(CVDataWidth DW, uint32_t address, void* data, CVAddressModifier AM, int count);
	void report(FILE* f);
};


#endif /* CAENVMEWRAPPER_H */
