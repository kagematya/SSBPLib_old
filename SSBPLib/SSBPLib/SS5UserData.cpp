#include "SS5UserData.h"
#include "SSPlayerImplement/DataArrayReader.h"
#include "SSPlayerImplement/ToPointer.h"


namespace ss{


void UserData::init()
{
	m_flags = 0;
	m_partIndex = 0;
	m_integer = 0;
	m_rect[0] = m_rect[1] = m_rect[2] = m_rect[3] = 0;
	m_point[0] = m_point[1] = 0;
	m_strSize = 0;
	m_str = nullptr;
}


void UserData::readData(DataArrayReader &reader, const ToPointer &ptr)
{
	init();	//èâä˙âª


	m_flags = reader.readU16();
	m_partIndex = reader.readU16();

	if (m_flags & UserData::FLAG_INTEGER){
		m_integer = reader.readS32();
	}
		
	if (m_flags & UserData::FLAG_RECT){
		m_rect[0] = reader.readS32();
		m_rect[1] = reader.readS32();
		m_rect[2] = reader.readS32();
		m_rect[3] = reader.readS32();
	}
		
	if (m_flags & UserData::FLAG_POINT){
		m_point[0] = reader.readS32();
		m_point[1] = reader.readS32();
	}
		
	if (m_flags & UserData::FLAG_STRING){
		m_strSize = reader.readU16();
	
		ss_offset offset = reader.readOffset();
		m_str = ptr.toString(offset);
	}
	
}




} //namespace ss

