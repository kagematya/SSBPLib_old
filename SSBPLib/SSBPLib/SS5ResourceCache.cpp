#include "SS5ResourceCache.h"
#include <string>
#include "common/SSUtil.h"
#include "SS5PlayerData.h"
#include "SSPlayerImplement/ToPointer.h"
#include "SSPlayerImplement/CellCache.h"
#include "SSPlayerImplement/AnimeCache.h"
#include "SSPlayerImplement/ResourceSet.h"
#include "SS5Player.h"
#include "SS5Renderer.h"
#include "SS5EventListener.h"


static void splitPath(std::string& directoty, std::string& filename, const std::string& path)
{
    std::string f = path;
    std::string d = "";

    size_t pos = path.find_last_of("/");
	if (pos == std::string::npos) pos = path.find_last_of("\\");	// for win

    if (pos != std::string::npos)
    {
        d = path.substr(0, pos+1);
        f = path.substr(pos+1);
    }

	directoty = d;
	filename = f;
}



namespace ss{


const std::string SS5ResourceCache::s_null;


SS5ResourceCache::SS5ResourceCache()
{
}

SS5ResourceCache::~SS5ResourceCache()
{
	unregistAll();
}



void SS5ResourceCache::regist(const void *data, size_t dataSize, const std::string &dataKey, const std::string &imageBaseDir)
{
	SS_ASSERT2(data, "Invalid data");
	//�o�^�ς݂��ǂ����̔���
	if(m_dataDic.find(dataKey) != m_dataDic.end()){
		RefcountResourceSet *ref = m_dataDic.at(dataKey);
		ref->incCount();	//�o�^�ς݂̏ꍇ�̓J�E���g�A�b�v���邾���Bdata�̓��e�͖���(�ŏ��ɓo�^����Ă����̗D��)
		return;
	}

	/***** �V�K�o�^ *****/
	
	//�摜�t�@�C���̃f�B���N�g���p�X�����
	std::string baseDir = getImageBaseDir(imageBaseDir, static_cast<const ProjectData*>(data));

	//�f�[�^������ēo�^
	RefcountResourceSet* rs = new RefcountResourceSet(static_cast<const char*>(data), dataSize, baseDir);
	
	m_dataDic.insert(std::make_pair(dataKey, rs));
}


void SS5ResourceCache::unregist(const std::string& dataKey)
{
	auto it = m_dataDic.find(dataKey);
	assert(it != m_dataDic.end());

	RefcountResourceSet *ref = it->second;
	ref->decCount();				//
	assert(ref->getCount() >= 0);	//�}�C�i�X�ɂ͂Ȃ�Ȃ��͂�

	if(ref->getCount() == 0){
		//����dataKey�͏�����OK�B�Ȃ̂ŏ���
		SS_SAFE_DELETE(ref);
		m_dataDic.erase(it);
	}
}

void SS5ResourceCache::unregistAll()
{
	for(auto &str_rs : m_dataDic){
		SS_SAFE_DELETE(str_rs.second);
	}
	m_dataDic.clear();
}




void SS5ResourceCache::getTextureList(std::vector<std::string> *textureList, const std::string &dataKey) const
{
	const ResourceSet *rs = findData(dataKey);
	const CellCache *cellCache = rs->m_cellCache;

	//todo:ss5player�ɂ������R�[�h������E�E�Eresourceset������Ɉꗗ�擾�@�\������������������������Ȃ�
	int cellMapNum = cellCache->getCellMapNum();
	textureList->resize(cellMapNum);
	for (int i = 0; i < cellMapNum; ++i){
		(*textureList)[i] = cellCache->getTexturePath(i);
	}
}



SS5Player* SS5ResourceCache::createPlayer(const std::string &dataKey, SS5Renderer *renderer, SS5EventListener *eventListener) const
{
	const ResourceSet *rs = findData(dataKey);
	return new SS5Player(rs, renderer, eventListener);
}

void SS5ResourceCache::destroyPlayer(SS5Player *&player) const
{
	delete player;
	player = nullptr;
}





std::string SS5ResourceCache::getImageBaseDir(const std::string &imageBaseDir, const ProjectData *data) const
{
	if (imageBaseDir == s_null){	// imageBaseDir�̎w�肪�Ȃ��Ƃ��̓p�X�����
		
		if (data->imageBaseDir){
			// �R���o�[�g���Ɏw�肳�ꂽ�p�X���g�p����
			ToPointer ptr(data);
			return ptr.toString(data->imageBaseDir);
		}
		
		//// �v���W�F�N�g�t�@�C���Ɠ����f�B���N�g�����w�肷��
		//std::string directory;
		//std::string filename;
		//splitPath(directory, filename, ssbpFilepath);
		//return directory;
	}
	return imageBaseDir;
}


const ResourceSet* SS5ResourceCache::findData(const std::string& dataKey) const
{
	auto it = m_dataDic.find(dataKey);
	assert(it != m_dataDic.end());
	
	RefcountResourceSet *rrs = it->second;
	return rrs->getResourceSet();	//���ɃJ�E���^�̏グ�����Ȃǂ͂��܂���
}


} //namespace ss
