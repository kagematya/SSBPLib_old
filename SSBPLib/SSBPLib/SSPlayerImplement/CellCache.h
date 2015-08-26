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
	const Cell *m_cell;		//memo:�������g�͕s�v�Ȃ͂�
	const char *m_cellname;	//�Z���̖��O
	int m_cellMapIndex;		//�Z���}�b�v�ԍ�
	SSRect m_rect;			//cell�͈̔�
};


/**
 * ���̃f�[�^�������₷�����邽�߁ACell����Ɋւ��Ă̍\�z���s��
 *   data�����ɂ��āACellRef�Ɖ摜�p�X���\�z����
 */
class CellCache{
public:
	/** data������CellRef���\�z���� */
	CellCache(const ProjectData *data, const std::string &imageBaseDir);
	~CellCache();

	/** �w��ԍ���CellRef��Ԃ� */
	const CellRef* getReference(int index) const;


	//�w�肵��CellMap�̃e�N�X�`�������擾����
	std::string getTexturePath(int cellMapIndex) const;

	//CellMap��
	int getCellMapNum() const;

private:
	/** data������CellRef���\�z����*/
	void init(const ProjectData* data, const std::string& imageBaseDir);

	std::string m_imageBaseDir;
	std::vector<const char *> m_imagePaths;	//�e�N�X�`���t�@�C����(�Y���̓Z���}�b�v�ԍ��ɑΉ�)
	std::vector<CellRef> m_refs;			//CellRef�Q		//memo:�������I�ɕω����邱�Ƃ͂Ȃ��̂Ŏ��̂�˂�����ł܂�
};


} //namespace ss
