#pragma once

#include "SSBPLib/SS5PlayerData.h"
#include "SSBPLib/SS5PlayerTypes.h"

namespace ss{

/**
 * DataArrayReader
 * �^����ꂽ�o�b�t�@��ǂݐi�߂Ă�����
 */
class DataArrayReader{
public:
	DataArrayReader(const ss_u16* dataPtr)
		: m_dataPtr(dataPtr)
	{}

	ss_u16 readU16() { return *m_dataPtr++; }
	ss_s16 readS16() { return static_cast<ss_s16>(*m_dataPtr++); }

	unsigned int readU32(){
		unsigned int l = readU16();
		unsigned int u = readU16();
		return static_cast<unsigned int>((u << 16) | l);
	}

	int readS32(){
		return static_cast<int>(readU32());
	}

	float readFloat(){
		union {
			float			f;
			unsigned int	i;
		} c;
		c.i = readU32();
		return c.f;
	}
	
	ss_offset readOffset(){
		return static_cast<ss_offset>(readS32());
	}

private:
	const ss_u16*	m_dataPtr;
};


} //namespace ss
