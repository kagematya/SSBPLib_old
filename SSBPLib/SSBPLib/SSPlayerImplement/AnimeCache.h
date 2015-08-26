#pragma once

#include <map>
#include <string>
#include <list>
#include <assert.h>
#include "ToPointer.h"

namespace ss{
struct ProjectData;
struct AnimationData;
struct PartData;
struct AnimePackData;


/** AnimationData�ւ̃A�N�Z�X��񋟁B�p�[�c�f�[�^�̓p�b�P�[�W���ʂ̂��̂����A���֐��̂��߂����ŕ����Ă��� */
struct AnimeRef{
	std::string				m_packName;			//todo:string����Ȃ���const char*�̕����悳����
	std::string				m_animeName;		//todo:string����Ȃ���const char*�̕����悳����
	const AnimationData*	m_animationData;	//�A�j���[�V����

	//const AnimePackData*	animePackData;		//������p�b�P�[�W
	const PartData*			m_partDatas;		//�p�[�c�f�[�^�ւ̃V���[�g�J�b�ganiemPackData->partDatas[]
	int						m_numParts;			//�p�[�c�f�[�^��

	//PartData�ւ̃A�N�Z�T
	const PartData* getPartData(int partIndex) const{
		assert(partIndex >= 0 && partIndex < m_numParts);
		return &m_partDatas[partIndex];
	}
};



/** AnimationData�ɃA�N�Z�X���₷���悤�ɂ����ŕ����Ă��� */
class AnimeCache{
public:
	/** data������AnimeRef���\�z */
	AnimeCache(const ProjectData *data);
	~AnimeCache();
	

	/** packName��animeName���w�肵��AnimeRef�𓾂� */
	const AnimeRef* getReference(const std::string& packName, const std::string& animeName) const;

	/** animeName�̂ݎw�肵��AnimeRef�𓾂� */
	const AnimeRef* getReference(const std::string& animeName) const;
	

	/** �擪�̃A�j���[�V������Ԃ� */	//memo:SS5Player�̏������ŕK�v
	const AnimeRef* getFirstReference() const;

	
	/** �A�j���[�V�����̓o�^���̃��X�g���擾����B��Ƀf�o�b�O�p */
	void getAnimationList(std::list<std::string> *animlist) const;

private:
	/** data������AnimeRef���\�z */
	void init(const ProjectData* data);
	void addAnimationData(ToPointer ptr, const AnimePackData *pack);

	/** �t�@�C���p�X���� */
	static std::string toPackAnimeKey(const std::string& packName, const std::string& animeName);
	

	std::map<std::string, AnimeRef>	m_animeRefs;	//memo:map�Ȃ���̂ł����Ȃ��͂�   <animekey, animeref>
};


} //namespace ss
