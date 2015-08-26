#pragma once

//SS5Player.h�ɂ�������`���ڂ���
#include <string>
#include "common/SSRect.h"
#include "common/ssplayer_matrix.h"
#include "SS5PlayerTypes.h"

namespace ss{
class DataArrayReader;
struct AnimationInitialData;



/** LabelData */
struct LabelData{
	std::string	m_str;			/// String (zero terminated)
	int			m_strSize;		/// String size (byte count)
	int			m_frameNo;		/// Frame no
};


/**
 * �p�[�c�̏����i�[���܂��BState�̓��e�����Ƃɕ`�揈�����쐬���Ă��������B
 */
struct State {
	int m_flags;					/// ���̃t���[���ōX�V���s����X�e�[�^�X�̃t���O
	int m_cellIndex;				/// �p�[�c�Ɋ��蓖�Ă�ꂽ�Z���̔ԍ�
	float m_x;						/// SS5�A�g���r���[�g�FX���W
	float m_y;						/// SS5�A�g���r���[�g�FY���W
	float m_z;						/// SS5�A�g���r���[�g�FZ���W
	float m_anchorX;				/// ���_X�I�t�Z�b�g�{�Z���ɐݒ肳�ꂽ���_�I�t�Z�b�gX
	float m_anchorY;				/// ���_Y�I�t�Z�b�g�{�Z���ɐݒ肳�ꂽ���_�I�t�Z�b�gY
	float m_rotationX;				/// X��]�i�e�q�֌W�v�Z�ρj
	float m_rotationY;				/// Y��]�i�e�q�֌W�v�Z�ρj
	float m_rotationZ;				/// Z��]�i�e�q�֌W�v�Z�ρj
	float m_scaleX;					/// X�X�P�[���i�e�q�֌W�v�Z�ρj
	float m_scaleY;					/// Y�X�P�[���i�e�q�֌W�v�Z�ρj
	int   m_opacity;				/// �s�����x�i0�`255�j�i�e�q�֌W�v�Z�ρj
	float m_size_X;					/// SS5�A�g���r���[�g�FX�T�C�Y
	float m_size_Y;					/// SS5�A�g���r���[�g�FX�T�C�Y
	float m_uv_move_X;				/// SS5�A�g���r���[�g�FUV X�ړ�
	float m_uv_move_Y;				/// SS5�A�g���r���[�g�FUV Y�ړ�
	float m_uv_rotation;			/// SS5�A�g���r���[�g�FUV ��]
	float m_uv_scale_X;				/// SS5�A�g���r���[�g�FUV X�X�P�[��
	float m_uv_scale_Y;				/// SS5�A�g���r���[�g�FUV Y�X�P�[��
	float m_boundingRadius;			/// SS5�A�g���r���[�g�F�����蔼�a
	bool m_flipX;					/// �����]�i�e�q�֌W�v�Z�ρj
	bool m_flipY;					/// �c���]�i�e�q�֌W�v�Z�ρj
	bool m_isVisibled;				/// ��\���i�e�q�֌W�v�Z�ρj

	int m_colorBlendFunc;			/// SS5�A�g���r���[�g�F�J���[�u�����h�̃u�����h���@
	int m_colorBlendType;			/// SS5�A�g���r���[�g�F�J���[�u�����h�̒P�F�����_�J���[���B
	

	SSMatrix m_mat;					/// �p�[�c�̈ʒu���Z�o���邽�߂̃}�g���N�X�i�e�q�֌W�v�Z�ρj
	
	
	//reader����ăf�[�^��ǂݎ��
	void readData(DataArrayReader &reader, const AnimationInitialData *init);
	

	//���݂�State�̏�������uv���v�Z����
	void uvCompute(SSV3F_C4B_T2F_Quad *q, const SSRect &cellRect, int imgWidth, int imgHeight) const;
	//���݂�State�̏�������verex���v�Z����
	void vertexCompute(SSV3F_C4B_T2F_Quad *q, const SSRect &cellRect, const SSQuad3 &vertexTransform) const;
	//���݂�State�̏�������matrix���v�Z����BsetIdentity����n�߂܂�
	void matrixCompute(SSMatrix *matrix) const;

	void init();

	bool operator==(const State &s) const;
	bool operator!=(const State &s) const;

	State() { init(); }
};

/**
 * ResluteState
 * �Q�[�����ɕԂ��p�[�c�X�e�[�^�X�B
 * �K�v�ɉ����ăJ�X�^�}�C�Y���Ă��������B
 */
struct ResluteState
{
	int flags;						/// ���̃t���[���ōX�V���s����X�e�[�^�X�̃t���O
	int cellIndex;					/// �p�[�c�Ɋ��蓖�Ă�ꂽ�Z���̔ԍ�
	float x;						/// SS5�A�g���r���[�g�FX���W
	float y;						/// SS5�A�g���r���[�g�FY���W
	float z;						/// SS5�A�g���r���[�g�FZ���W
	float anchorX;					/// ���_X�I�t�Z�b�g�{�Z���ɐݒ肳�ꂽ���_�I�t�Z�b�gX
	float anchorY;					/// ���_Y�I�t�Z�b�g�{�Z���ɐݒ肳�ꂽ���_�I�t�Z�b�gY
	float rotationX;				/// X��]�i�e�q�֌W�v�Z�ρj
	float rotationY;				/// Y��]�i�e�q�֌W�v�Z�ρj
	float rotationZ;				/// Z��]�i�e�q�֌W�v�Z�ρj
	float scaleX;					/// X�X�P�[���i�e�q�֌W�v�Z�ρj
	float scaleY;					/// Y�X�P�[���i�e�q�֌W�v�Z�ρj
	int opacity;					/// �s�����x�i0�`255�j�i�e�q�֌W�v�Z�ρj
	float size_X;					/// SS5�A�g���r���[�g�FX�T�C�Y
	float size_Y;					/// SS5�A�g���r���[�g�FX�T�C�Y
	float uv_move_X;				/// SS5�A�g���r���[�g�FUV X�ړ�
	float uv_move_Y;				/// SS5�A�g���r���[�g�FUV Y�ړ�
	float uv_rotation;				/// SS5�A�g���r���[�g�FUV ��]
	float uv_scale_X;				/// SS5�A�g���r���[�g�FUV X�X�P�[��
	float uv_scale_Y;				/// SS5�A�g���r���[�g�FUV Y�X�P�[��
	float boundingRadius;			/// SS5�A�g���r���[�g�F�����蔼�a
	int colorBlendFunc;				/// SS5�A�g���r���[�g�F�J���[�u�����h�̃u�����h���@
	int colorBlendType;				/// SS5�A�g���r���[�g�F�J���[�u�����h�̒P�F�����_�J���[���B
	bool flipX;						/// �����]�i�e�q�֌W�v�Z�ρj
	bool flipY;						/// �c���]�i�e�q�֌W�v�Z�ρj
	bool isVisibled;				/// ��\���i�e�q�֌W�v�Z�ρj

	int	part_type;					//�p�[�c���
	int	part_boundsType;			//�����蔻����
	int	part_alphaBlendType;		// BlendType
};

/**
 * �Đ�����t���[���Ɋ܂܂��p�[�c�f�[�^�̃t���O
 *  �l����read����΂����킯�ł��Ȃ��炵���E�E�E
 *  �R���o�[�^�Ƃ̑Ή������Ȃ��珈���������Ă�������
 */
enum {
	PART_FLAG_INVISIBLE			= 1 << 0,		/// ��\��
	PART_FLAG_FLIP_H			= 1 << 1,		/// �����]
	PART_FLAG_FLIP_V			= 1 << 2,		/// �c���]

	// optional parameter flags
	PART_FLAG_CELL_INDEX		= 1 << 3,		/// �Z���ԍ�
	PART_FLAG_POSITION_X		= 1 << 4,		/// X���W
	PART_FLAG_POSITION_Y		= 1 << 5,		/// Y���W
	PART_FLAG_POSITION_Z		= 1 << 6,		/// Z���W
	PART_FLAG_ANCHOR_X			= 1 << 7,		/// ���_�I�t�Z�b�gX
	PART_FLAG_ANCHOR_Y			= 1 << 8,		/// ���_�I�t�Z�b�gY
	PART_FLAG_ROTATIONX			= 1 << 9,		/// X��]
	PART_FLAG_ROTATIONY			= 1 << 10,		/// Y��]
	PART_FLAG_ROTATIONZ			= 1 << 11,		/// Z��]
	PART_FLAG_SCALE_X			= 1 << 12,		/// �X�P�[��X
	PART_FLAG_SCALE_Y			= 1 << 13,		/// �X�P�[��Y
	PART_FLAG_OPACITY			= 1 << 14,		/// �s�����x
	PART_FLAG_COLOR_BLEND		= 1 << 15,		/// �J���[�u�����h
	PART_FLAG_VERTEX_TRANSFORM	= 1 << 16,		/// ���_�ό`

	PART_FLAG_SIZE_X			= 1 << 17,		/// �T�C�YX
	PART_FLAG_SIZE_Y			= 1 << 18,		/// �T�C�YY

	PART_FLAG_U_MOVE			= 1 << 19,		/// UV�ړ�X
	PART_FLAG_V_MOVE			= 1 << 20,		/// UV�ړ�Y
	PART_FLAG_UV_ROTATION		= 1 << 21,		/// UV��]
	PART_FLAG_U_SCALE			= 1 << 22,		/// UV�X�P�[��X
	PART_FLAG_V_SCALE			= 1 << 23,		/// UV�X�P�[��Y
	PART_FLAG_BOUNDINGRADIUS	= 1 << 24,		/// �����蔼�a

	PART_FLAG_INSTANCE_KEYFRAME = 1 << 25,		/// �C���X�^���X
	PART_FLAG_INSTANCE_START	= 1 << 26,		/// �C���X�^���X�F�J�n�t���[��
	PART_FLAG_INSTANCE_END		= 1 << 27,		/// �C���X�^���X�F�I���t���[��
	PART_FLAG_INSTANCE_SPEED	= 1 << 28,		/// �C���X�^���X�F�Đ����x
	PART_FLAG_INSTANCE_LOOP		= 1 << 29,		/// �C���X�^���X�F���[�v��
	PART_FLAG_INSTANCE_LOOP_FLG = 1 << 30,		/// �C���X�^���X�F���[�v�ݒ�

	NUM_PART_FLAGS
};

/** ���_�ό`�t���O */
enum {
	VERTEX_FLAG_LT = 1 << 0,
	VERTEX_FLAG_RT = 1 << 1,
	VERTEX_FLAG_LB = 1 << 2,
	VERTEX_FLAG_RB = 1 << 3,
	VERTEX_FLAG_ONE = 1 << 4	// color blend only
};

/** �C���X�^���X���[�v�ݒ�t���O */
enum {
	INSTANCE_LOOP_FLAG_INFINITY = 1 << 0,		//
	INSTANCE_LOOP_FLAG_REVERSE = 1 << 1,
	INSTANCE_LOOP_FLAG_PINGPONG = 1 << 2,
	INSTANCE_LOOP_FLAG_INDEPENDENT = 1 << 3,
};

/** Animation Part Type */
enum {
	PARTTYPE_INVALID = -1,
	PARTTYPE_NULL,			/// null�B�̈��������SRT���̂݁B�������~�`�̓����蔻��͐ݒ�\�B
	PARTTYPE_NORMAL,		/// �ʏ�p�[�c�B�̈�����B�摜�͖����Ă������B
	PARTTYPE_TEXT,			/// �e�L�X�g(�\��@�������j
	PARTTYPE_INSTANCE,		/// �C���X�^���X�B���A�j���A�p�[�c�ւ̎Q�ƁB�V�[���ҏW���[�h�̑�ւɂȂ����
	PARTTYPE_NUM
};

/** �����蔻��̎�� */
enum {
	INVALID = -1,
	NONE,			///< �����蔻��Ƃ��Ďg��Ȃ��B
	QUAD,			///< ���݂ɕό`����l�ӌ`�B���_�ό`�ȂǓK�p��̂S�p�����񂾗̈�B�ł��d���B
	AABB,			///< ��]���Ȃ��S�̂��͂ދ�`�Ō�������
	CIRCLE,			///< �^�~�̔��a�ŋ����ɂ�蔻�肷��
	CIRCLE_SMIN,	///< �^�~�̔��a�ŋ����ɂ�蔻�肷�� (�X�P�[����x,y�̍ŏ��l���Ƃ�j
	CIRCLE_SMAX,	///< �^�~�̔��a�ŋ����ɂ�蔻�肷�� (�X�P�[����x,y�̍ő�l���Ƃ�j
	//num
};

/** ���u�����h���@ */
enum BlendType {
	BLEND_MIX,		///< 0 �u�����h�i�~�b�N�X�j
	BLEND_MUL,		///< 1 ��Z
	BLEND_ADD,		///< 2 ���Z
	BLEND_SUB		///< 3 ���Z
};



} //namespace ss
