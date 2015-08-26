#pragma once

#include "SSBPLib/SS5PlayerData.h"

namespace ss{

/**
 * ProjectData����e�f�[�^�ւ̃A�N�Z�T
 *  ����̌^�ւ̃A�N�Z�X�������Ȃ��̂ŁA�ǂ݂₷���̂��߂ɂ��A�N�Z�X���@��񋟂��Ă���
 */
class ToPointer{
public:
	explicit ToPointer(const ProjectData* data)
		: _base(static_cast<const char*>(static_cast<const void*>(data))) {}
	

	const Cell* toCell(const ProjectData *adr) const{
		return toClass<Cell>( adr->cells );					//return static_cast<const Cell*>(getAddress( adr->cells ));
	}
	const CellMap* toCellMap(const Cell *adr) const{
		return toClass<CellMap>( adr->cellMap );
	}
	
	
	const AnimePackData* toAnimePackData(const ProjectData *adr) const{
		return toClass<AnimePackData>( adr->animePacks );	//return static_cast<const AnimePackData*>(getAddress( adr->animePacks));
	}
	const PartData* toPartData(const AnimePackData *adr) const{
		return toClass<PartData>( adr->parts );
	}
	const AnimationData* toAnimationData(const AnimePackData *adr) const{
		return toClass<AnimationData>( adr->animations );
	}
	const AnimationInitialData* toAnimationInitialData(const AnimationData *adr) const{
		return toClass<AnimationInitialData>( adr->defaultData );
	}


	//�ق��̃A�N�Z�X�͂قƂ��const char*�Ȃ̂ł����offset�����悤�ɂ��Ƃ�
	const char* toString(ss_offset offset) const{
		return toClass<char>( offset );
	}

		
		
		
//private:	
	const void* operator()(ss_offset offset) const{
		return getAddress(offset);
	}
	
private:
	//�L���X�g�̃e���v���[�g��
	template<class T>
	const T* toClass(ss_offset offset) const{
		return static_cast<const T*>(getAddress( offset ));
	}

	const void* getAddress(ss_offset offset) const{
		return (_base + offset);
	}


	const char*	_base;			//
};


} //namespace ss
