#ifndef CAENVMEWRAPPER_H
#define CAENVMEWRAPPER_H

#include "CAENVMElib.h"

class CAENVMEWrapper
{
	bool m_simulate;
	int32_t m_handle;
    std::map<uint32_t,uint64_t> m_simDataMap;

    static const char* decodeError(int code);
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
