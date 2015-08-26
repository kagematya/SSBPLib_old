#pragma once

#include <string>
#include <vector>
#include <assert.h>
#include "CellCache.h"
#include "AnimeCache.h"
#include "SSBPLib/SS5PlayerData.h"
#include "SSBPLib/common/SSUtil.h"

namespace ss{


/** ���\�[�X�ƂȂ�f�[�^�ƁA���̍\���ւ̃A�N�Z�T�������Ă܂� */
class ResourceSet{
private:
	static const ss_u32 DATA_ID = 0x42505353;	//�f�[�^�̃`�F�b�N�l
	static const ss_u32 DATA_VERSION = 1;		//�f�[�^�̃`�F�b�N�l

	std::vector<char> m_src;	//�f�[�^�̎���
	
public:
	const ProjectData* m_data;	//�f�[�^���w������
	CellCache* m_cellCache;		//cell����̍\���ւ̃A�N�Z�T
	AnimeCache* m_animeCache;	//anim����̍\���ւ̃A�N�Z�T

	/** data_�̓R�s�[����܂� */
	ResourceSet(const char *data_, size_t dataSize, const std::string &imageBaseDir)
		: m_src(data_, data_+dataSize)	//�R�s�[
		, m_data(nullptr)
		, m_cellCache(nullptr)
		, m_animeCache(nullptr)
	{
		SS_ASSERT2(data_, "Invalid data");
		SS_ASSERT2(dataSize > 0, "dataSize is zero");
		
		m_data = reinterpret_cast<const ProjectData*>(&m_src[0]);
		SS_ASSERT2(m_data->dataId == DATA_ID, "Not data id matched");
		SS_ASSERT2(m_data->version == DATA_VERSION, "Version number of data does not match");

		m_cellCache = new CellCache(m_data, imageBaseDir);
		m_animeCache = new AnimeCache(m_data);
	}


	~ResourceSet(){
		SS_SAFE_DELETE(m_animeCache);
		SS_SAFE_DELETE(m_cellCache);
	}
};


} //namespace ss
