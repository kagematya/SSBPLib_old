#pragma once

#include <string>
#include <map>
#include <memory>
#include "SSPlayerImplement/ResourceSet.h"

namespace ss{
struct ProjectData;
class SS5Player;
class SS5Renderer;
class SS5EventListener;

	
/** addData���ɁAanime,cell�̐������s���BSS5Player�͂������烊�\�[�X��find���Đ������� */
class SS5ResourceCache{
public:
	SS5ResourceCache();
	~SS5ResourceCache();

	
	static const std::string s_null;

	/**
	 * ssbp�t�@�C����ǂݍ��݊Ǘ��ΏۂƂ��܂��B���d�o�^�΍�̂��߂ɎQ�ƃJ�E���^�ɂ��Ǘ��ɂȂ�܂�
	 *
	 * @param  data			ssbp�̃f�[�^(���ŃR�s�[����܂�)
	 * @param  dataSize		data�̃T�C�Y
	 * @param  dataKey		�o�^��
	 * @param  imageBaseDir �摜�t�@�C���̓ǂݍ��݌����[�g�p�X. �ȗ����̓R���o�[�g���Ɏw�肳�ꂽ�p�X���g�p����.(�w�肷��̂���������)
	 */
	void regist(const void *data, size_t dataSize, const std::string &dataKey, const std::string &imageBaseDir = s_null);

	
	/** �w��f�[�^��������܂��B�o�^�����w�肵�Ă������� */
	void unregist(const std::string& dataKey);

	/** �S�Ẵf�[�^��������܂� */
	void unregistAll();


	/** �w�肵���f�[�^���K�v�Ƃ���e�N�X�`�����̃��X�g���擾 */
	void getTextureList(std::vector<std::string> *textureList, const std::string &dataKey) const;
	

	/** SS5Player�̐��� */
	SS5Player* createPlayer(const std::string &dataKey, SS5Renderer *renderer, SS5EventListener *eventListener) const;
	void destroyPlayer(SS5Player *&player) const;
	

private:
	//imageBaseDir�̎w�肪�Ȃ��Ƃ���data�̒������ăf�B���N�g����Ԃ�
	std::string getImageBaseDir(const std::string &imageBaseDir, const ProjectData *data) const;
	
	//���O�ɑΉ�����f�[�^���擾
	const ResourceSet* findData(const std::string& dataKey) const;


	
	/** regist�����J�E���g���邽�߂̍\�� */
	class RefcountResourceSet{
	public:
		RefcountResourceSet(const char *data, size_t dataSize, const std::string &imageBaseDir)
		: m_refCount(0), m_resourceSet(new ResourceSet(data, dataSize, imageBaseDir)){
			incCount();
		}
		~RefcountResourceSet(){}
		
		int getCount() const{ return m_refCount; }
		void incCount(){ ++m_refCount; }
		void decCount(){ --m_refCount; }

		ResourceSet* getResourceSet() const{
			return m_resourceSet.get();
		}

	private:
		int m_refCount;
		std::unique_ptr<ResourceSet> m_resourceSet;
	};
	
	std::map<std::string, RefcountResourceSet*>	m_dataDic;	//�����Ƀf�[�^��o�^����


private: //non copyable
	SS5ResourceCache(const SS5ResourceCache &o) = delete;
	SS5ResourceCache& operator=(const SS5ResourceCache &o) = delete;
};


} //namespace ss
