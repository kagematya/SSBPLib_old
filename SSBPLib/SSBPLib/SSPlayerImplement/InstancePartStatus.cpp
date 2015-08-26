#include "InstancePartStatus.h"
#include "DataArrayReader.h"
#include "SSBPLib/SSPlayerStatus.h"

namespace ss{

InstancePartStatus::InstancePartStatus()
	: m_refKeyframe(0)
	, m_refStartframe(0)
	, m_refEndframe(0)
	, m_refSpeed(0)
	, m_refloopNum(0)
	, m_infinity(false)
	, m_reverse(false)
	, m_pingpong(false)
	, m_independent(false)
{
	/**/
}


//�ǂݎ��
void InstancePartStatus::readInstancePartStatus(DataArrayReader &reader, int readFlags)
{
	if (readFlags & PART_FLAG_INSTANCE_KEYFRAME){
		m_refKeyframe = reader.readS16();
	}
	if (readFlags & PART_FLAG_INSTANCE_START){
		m_refStartframe = reader.readS16();
	}
	if (readFlags & PART_FLAG_INSTANCE_END){
		m_refEndframe = reader.readS16();
	}
	if (readFlags & PART_FLAG_INSTANCE_SPEED){
		m_refSpeed = reader.readFloat();
	}
	if (readFlags & PART_FLAG_INSTANCE_LOOP){
		m_refloopNum = reader.readS16();
	}

	if (readFlags & PART_FLAG_INSTANCE_LOOP_FLG){

		int lflags = reader.readS16();
		if (lflags & INSTANCE_LOOP_FLAG_INFINITY ){
			m_infinity = true;		//�������[�v
		}
		if (lflags & INSTANCE_LOOP_FLAG_REVERSE){
			m_reverse = true;		//�t�Đ�
		}
		if (lflags & INSTANCE_LOOP_FLAG_PINGPONG){
			m_pingpong = true;		//����
		}
		if (lflags & INSTANCE_LOOP_FLAG_INDEPENDENT){
			m_independent = true;	//�Ɨ�
		}
	}
}


//�ǂݎ���̌v�Z
int InstancePartStatus::getTime(int time) const
{
	int	reftime = static_cast<int>(time * m_refSpeed) - m_refKeyframe; //�J�n���猻�݂̌o�ߎ���
	if(reftime < 0){ return time; }									   //���������������Ԃɑ��݂��Ă��Ȃ�

	int inst_scale = (m_refEndframe - m_refStartframe) + 1; //�C���X�^���X�̎�
	if (inst_scale <= 0){ return time; }					//�ڂ��O�������̓}�C�i�X�i���蓾�Ȃ�

	int	nowloop = reftime / inst_scale;		//���݂܂ł̃��[�v��
	int nowframe = reftime % inst_scale;	//���[�v���������Ȃ��C���X�^���X�A�j�����̃t���[��
	

	//�������[�v�Ŗ������Ƀ��[�v�����`�F�b�N
	if (!m_infinity){
		//�񐔐���������̂ŏI�[�`�F�b�N������
		int checkloopnum = m_refloopNum;		//���[�v�I����
		if (m_pingpong){ checkloopnum *= 2; }	//pingpong�̏ꍇ�ł͂Q�{�ɂ���

		if (nowloop >= checkloopnum){
			return endFrame();		//���[�v�I���̂Ƃ��͏I���̃t���[����Ԃ�
		}
	}

	//���݂̍Đ��t���[���̌v�Z
	bool isReverse = m_reverse;
	if (m_pingpong && (nowloop % 2 == 1)){
		isReverse = !isReverse;	//���]
	}

	if (isReverse){
		return m_refEndframe - nowframe;	//���o�[�X�̎�
	}
	return nowframe + m_refStartframe;	//�ʏ펞
}



//�t���O���l�����ďI���t���[����Ԃ�
int InstancePartStatus::endFrame() const
{
	if(m_pingpong && m_reverse){
		return m_refEndframe;		//pingpong && ���]
	}
	else if(m_pingpong){
		return m_refStartframe;		//pingpong
	}
	else if(m_reverse){
		return m_refStartframe;		//���]
	}
	
	return m_refEndframe;			//�ʏ�
}



} //namespace ss
