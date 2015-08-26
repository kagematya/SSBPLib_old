#include "CellCache.h"
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "ToPointer.h"
#include "SSBPLib/common/SSUtil.h"

namespace ss{


CellCache::CellCache(const ProjectData *data, const std::string &imageBaseDir)
{
	init(data, imageBaseDir);
}

CellCache::~CellCache()
{
}


const CellRef* CellCache::getReference(int index) const
{
	if (index < 0 || index >= static_cast<int>(m_refs.size())){
		SSLOGERROR("Index out of range > %d", index);
		SS_ASSERT(0);
	}
	
	return &(m_refs[index]);
}


//�f�[�^������cellref��imagepath���\�z
void CellCache::init(const ProjectData* data, const std::string& imageBaseDir)
{
	SS_ASSERT2(data, "Invalid data");
		
	m_imageBaseDir = imageBaseDir;
	m_refs.resize(data->numCells);	//cell�������̈�m�ۂ��Ă���
	
	std::map<int, const char*> imagePathMap;	//�����킩��Ȃ��̂łЂƂ܂�<index,path>��map�ɂ��Ă���
	

	ToPointer ptr(data);
	const Cell* cells = ptr.toCell(data);
	
	for (int i = 0; i < data->numCells; i++){
		const Cell* cell = &cells[i];

		const CellMap* cellMap = ptr.toCellMap(cell);
		const char* cellname = ptr.toString(cell->name);			//�Z����
		//const char* cellmapname = ptr.toString(cellMap->name);	//�Z���}�b�v��
		
		//�����ł̓��[�h�Ȃǂ͂����ɔԍ���ۑ����Ƃ������ɗ��߂�
		imagePathMap[cellMap->index] = ptr.toString(cellMap->imagePath);		//memo:�����͉��x���㏑������邾�낤��const char*�̃R�s�[�Ȃ̂ő��v
		
		
		CellRef ref = {
			cell,  cellname, cellMap->index,
			SSRect(cell->x, cell->y, cell->width, cell->height)
		};
		
		m_refs[i] = ref;
	}

	
	//map --> vector �ɋl�ߒ��� -----------------
	auto it = std::max_element(imagePathMap.begin(), imagePathMap.end());
	if(it == imagePathMap.end()){			//�摜����
		m_imagePaths.clear();
		return;
	}
	
	int maxIndex = it->first;
	m_imagePaths.resize(maxIndex + 1);
	for(auto &index_path : imagePathMap){	//�l�ߑւ�
		m_imagePaths[index_path.first] = index_path.second;
	}
}



std::string CellCache::getTexturePath(int cellMapIndex) const
{
	assert(cellMapIndex >= 0 && cellMapIndex < m_imagePaths.size());	//index��assert�`�F�b�N�͂���
#if 0
	if(isAbsolutePath(m_imagePaths[cellMapIndex])){
		return m_imagePaths[cellMapIndex];	//��΃p�X�̂Ƃ��͂��̂܂܈���
	}
#endif
	std::string texturePath = m_imageBaseDir + m_imagePaths[cellMapIndex];
	return texturePath;
}


int CellCache::getCellMapNum() const{
	return m_imagePaths.size();
}


} //namespace ss
