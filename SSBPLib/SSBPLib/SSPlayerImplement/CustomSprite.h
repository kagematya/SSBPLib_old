#pragma once

#include <memory>
#include <string.h>
#include "SSBPLib/common/ssplayer_matrix.h"
#include "SSBPLib/SS5PlayerTypes.h"
#include "SSBPLib/SSPlayerStatus.h"
#include "SSBPLib/SSPlayerImplement/InstancePartStatus.h"

namespace ss{
class SS5Player;

/**
 * CustomSprite
 * 現フレームでのデータをParts毎にここに入れる
 */
class CustomSprite{
public:
	SSMatrix			m_mat;			//今のところ、cellのworldMatrixって感じだ。
	State				m_state;
	bool				m_isStateChanged;
	CustomSprite*		m_parent;
	
	bool				m_haveChildPlayer;		//ChildPlayer(インスタンスアニメーション)を持っているならtrue
	InstancePartStatus  m_instancePartStatus;	//インスタンスアニメーションの時間制御変数
	
	void initialize();
	void finalize();

	void updateMatrixAndOpacity(const SSMatrix& rootMatrix, int rootOpacity);

public:
	CustomSprite();
	~CustomSprite();


	void setState(const State& state);


	SSV3F_C4B_T2F_Quad m_quad;		/// 頂点データ、座標、カラー値、UVが含まれる（頂点変形、サイズXY、UV移動XY、UVスケール、UV回転、反転が反映済）
	TextureID m_texture;			/// セルに対応したテクスチャ番号（ゲーム側で管理している番号を設定する）
	SSRect m_rect;					/// セルに対応したテクスチャ内の表示領域（開始座標、幅高さ）
	int m_blendfunc;				/// パーツに設定されたブレンド方法
};


} //namespace ss
