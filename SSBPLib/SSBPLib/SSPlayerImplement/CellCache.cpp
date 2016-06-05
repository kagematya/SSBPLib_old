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
	SS_ASSERT2(index >= 0 && index < m_refs.size(), "index is out of range.");
	return &(m_refs[index]);
}

int CellCache::indexOfCell(const std::string &cellName) const
{
	//cellnameは同名も存在できるようだが、ひとまず最初に見つかったものを返すことにする
	for(int i = 0; i < m_refs.size(); ++i){
		const CellRef *ref = getReference(i);
		if(cellName == ref->m_cellname){
			return i;		//名前一致したので返す
		}
	}
	return -1;				//名前一致しなかったとき
}



//データを見てcellrefとimagepathを構築
void CellCache::init(const ProjectData* data, const std::string& imageBaseDir)
{
	SS_ASSERT2(data, "Invalid data");
		
	m_imageBaseDir = imageBaseDir;
	m_refs.resize(data->numCells);	//cell数だけ領域確保しておく
	
	std::map<int, const char*> imagePathMap;	//数がわからないのでひとまず<index,path>のmapにしておく
	

	ToPointer ptr(data);
	const Cell* cells = ptr.toCell(data);
	
	for (int i = 0; i < data->numCells; i++){
		const Cell* cell = &cells[i];

		const CellMap* cellMap = ptr.toCellMap(cell);
		const char* cellname = ptr.toString(cell->name);			//セル名
		//const char* cellmapname = ptr.toString(cellMap->name);	//セルマップ名
		
		//ここではロードなどはせずに番号を保存しとくだけに留める
		imagePathMap[cellMap->index] = ptr.toString(cellMap->imagePath);		//memo:ここは何度も上書きされるだろうがconst char*のコピーなので大丈夫
		
		
		CellRef ref = {
			cell,  cellname, cellMap->index,
			SSRect(cell->x, cell->y, cell->width, cell->height)
		};
		
		m_refs[i] = ref;
	}

	
	//map --> vector に詰め直す -----------------
	auto it = std::max_element(imagePathMap.begin(), imagePathMap.end());
	if(it == imagePathMap.end()){			//画像無し
		m_imagePaths.clear();
		return;
	}
	
	int maxIndex = it->first;
	m_imagePaths.resize(maxIndex + 1);
	for(auto &index_path : imagePathMap){	//詰め替え
		m_imagePaths[index_path.first] = index_path.second;
	}
}



std::string CellCache::getTexturePath(int cellMapIndex) const
{
	assert(cellMapIndex >= 0 && cellMapIndex < m_imagePaths.size());	//indexのassertチェックはする
#if 0
	if(isAbsolutePath(m_imagePaths[cellMapIndex])){
		return m_imagePaths[cellMapIndex];	//絶対パスのときはそのまま扱う
	}
#endif
	std::string texturePath = m_imageBaseDir + m_imagePaths[cellMapIndex];
	return texturePath;
}


int CellCache::getCellMapNum() const{
	return m_imagePaths.size();
}


} //namespace ss
