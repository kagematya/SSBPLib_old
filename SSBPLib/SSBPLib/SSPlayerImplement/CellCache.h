#pragma once

#include <vector>
#include <string>
#include "SSBPLib/common/SSRect.h"

namespace ss{
struct Cell;
struct ProjectData;


/**
 * CellRef
 */
struct CellRef{
	const Cell *m_cell;		//memo:こいつ自身は不要なはず
	const char *m_cellname;	//セルの名前
	int m_cellMapIndex;		//セルマップ番号
	SSRect m_rect;			//cellの範囲
};


/**
 * 生のデータを扱いやすくするため、Cell周りに関しての構築を行う
 *   dataを元にして、CellRefと画像パスを構築する
 */
class CellCache{
public:
	/** dataを元にCellRefを構築する */
	CellCache(const ProjectData *data, const std::string &imageBaseDir);
	~CellCache();

	/** 指定番号のCellRefを返す */
	const CellRef* getReference(int index) const;


	//指定したCellMapのテクスチャ名を取得する
	std::string getTexturePath(int cellMapIndex) const;

	//CellMap数
	int getCellMapNum() const;

private:
	/** dataを元にCellRefを構築する*/
	void init(const ProjectData* data, const std::string& imageBaseDir);

	std::string m_imageBaseDir;
	std::vector<const char *> m_imagePaths;	//テクスチャファイル名(添字はセルマップ番号に対応)
	std::vector<CellRef> m_refs;			//CellRef群		//memo:数が動的に変化することはないので実体を突っ込んでます
};


} //namespace ss
