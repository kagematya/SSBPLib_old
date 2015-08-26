#pragma once

//SS5Player.hにあった定義を移した
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
 * パーツの情報を格納します。Stateの内容をもとに描画処理を作成してください。
 */
struct State {
	int m_flags;					/// このフレームで更新が行われるステータスのフラグ
	int m_cellIndex;				/// パーツに割り当てられたセルの番号
	float m_x;						/// SS5アトリビュート：X座標
	float m_y;						/// SS5アトリビュート：Y座標
	float m_z;						/// SS5アトリビュート：Z座標
	float m_anchorX;				/// 原点Xオフセット＋セルに設定された原点オフセットX
	float m_anchorY;				/// 原点Yオフセット＋セルに設定された原点オフセットY
	float m_rotationX;				/// X回転（親子関係計算済）
	float m_rotationY;				/// Y回転（親子関係計算済）
	float m_rotationZ;				/// Z回転（親子関係計算済）
	float m_scaleX;					/// Xスケール（親子関係計算済）
	float m_scaleY;					/// Yスケール（親子関係計算済）
	int   m_opacity;				/// 不透明度（0～255）（親子関係計算済）
	float m_size_X;					/// SS5アトリビュート：Xサイズ
	float m_size_Y;					/// SS5アトリビュート：Xサイズ
	float m_uv_move_X;				/// SS5アトリビュート：UV X移動
	float m_uv_move_Y;				/// SS5アトリビュート：UV Y移動
	float m_uv_rotation;			/// SS5アトリビュート：UV 回転
	float m_uv_scale_X;				/// SS5アトリビュート：UV Xスケール
	float m_uv_scale_Y;				/// SS5アトリビュート：UV Yスケール
	float m_boundingRadius;			/// SS5アトリビュート：当たり半径
	bool m_flipX;					/// 横反転（親子関係計算済）
	bool m_flipY;					/// 縦反転（親子関係計算済）
	bool m_isVisibled;				/// 非表示（親子関係計算済）

	int m_colorBlendFunc;			/// SS5アトリビュート：カラーブレンドのブレンド方法
	int m_colorBlendType;			/// SS5アトリビュート：カラーブレンドの単色か頂点カラーか。
	

	SSMatrix m_mat;					/// パーツの位置を算出するためのマトリクス（親子関係計算済）
	
	
	//readerを介してデータを読み取る
	void readData(DataArrayReader &reader, const AnimationInitialData *init);
	

	//現在のStateの情報を元にuvを計算する
	void uvCompute(SSV3F_C4B_T2F_Quad *q, const SSRect &cellRect, int imgWidth, int imgHeight) const;
	//現在のStateの情報を元にverexを計算する
	void vertexCompute(SSV3F_C4B_T2F_Quad *q, const SSRect &cellRect, const SSQuad3 &vertexTransform) const;
	//現在のStateの情報を元にmatrixを計算する。setIdentityから始めます
	void matrixCompute(SSMatrix *matrix) const;

	void init();

	bool operator==(const State &s) const;
	bool operator!=(const State &s) const;

	State() { init(); }
};

/**
 * ResluteState
 * ゲーム側に返すパーツステータス。
 * 必要に応じてカスタマイズしてください。
 */
struct ResluteState
{
	int flags;						/// このフレームで更新が行われるステータスのフラグ
	int cellIndex;					/// パーツに割り当てられたセルの番号
	float x;						/// SS5アトリビュート：X座標
	float y;						/// SS5アトリビュート：Y座標
	float z;						/// SS5アトリビュート：Z座標
	float anchorX;					/// 原点Xオフセット＋セルに設定された原点オフセットX
	float anchorY;					/// 原点Yオフセット＋セルに設定された原点オフセットY
	float rotationX;				/// X回転（親子関係計算済）
	float rotationY;				/// Y回転（親子関係計算済）
	float rotationZ;				/// Z回転（親子関係計算済）
	float scaleX;					/// Xスケール（親子関係計算済）
	float scaleY;					/// Yスケール（親子関係計算済）
	int opacity;					/// 不透明度（0～255）（親子関係計算済）
	float size_X;					/// SS5アトリビュート：Xサイズ
	float size_Y;					/// SS5アトリビュート：Xサイズ
	float uv_move_X;				/// SS5アトリビュート：UV X移動
	float uv_move_Y;				/// SS5アトリビュート：UV Y移動
	float uv_rotation;				/// SS5アトリビュート：UV 回転
	float uv_scale_X;				/// SS5アトリビュート：UV Xスケール
	float uv_scale_Y;				/// SS5アトリビュート：UV Yスケール
	float boundingRadius;			/// SS5アトリビュート：当たり半径
	int colorBlendFunc;				/// SS5アトリビュート：カラーブレンドのブレンド方法
	int colorBlendType;				/// SS5アトリビュート：カラーブレンドの単色か頂点カラーか。
	bool flipX;						/// 横反転（親子関係計算済）
	bool flipY;						/// 縦反転（親子関係計算済）
	bool isVisibled;				/// 非表示（親子関係計算済）

	int	part_type;					//パーツ種別
	int	part_boundsType;			//当たり判定種類
	int	part_alphaBlendType;		// BlendType
};

/**
 * 再生するフレームに含まれるパーツデータのフラグ
 *  値順にreadすればいいわけでもないらしい・・・
 *  コンバータとの対応を見ながら処理を書いてください
 */
enum {
	PART_FLAG_INVISIBLE			= 1 << 0,		/// 非表示
	PART_FLAG_FLIP_H			= 1 << 1,		/// 横反転
	PART_FLAG_FLIP_V			= 1 << 2,		/// 縦反転

	// optional parameter flags
	PART_FLAG_CELL_INDEX		= 1 << 3,		/// セル番号
	PART_FLAG_POSITION_X		= 1 << 4,		/// X座標
	PART_FLAG_POSITION_Y		= 1 << 5,		/// Y座標
	PART_FLAG_POSITION_Z		= 1 << 6,		/// Z座標
	PART_FLAG_ANCHOR_X			= 1 << 7,		/// 原点オフセットX
	PART_FLAG_ANCHOR_Y			= 1 << 8,		/// 原点オフセットY
	PART_FLAG_ROTATIONX			= 1 << 9,		/// X回転
	PART_FLAG_ROTATIONY			= 1 << 10,		/// Y回転
	PART_FLAG_ROTATIONZ			= 1 << 11,		/// Z回転
	PART_FLAG_SCALE_X			= 1 << 12,		/// スケールX
	PART_FLAG_SCALE_Y			= 1 << 13,		/// スケールY
	PART_FLAG_OPACITY			= 1 << 14,		/// 不透明度
	PART_FLAG_COLOR_BLEND		= 1 << 15,		/// カラーブレンド
	PART_FLAG_VERTEX_TRANSFORM	= 1 << 16,		/// 頂点変形

	PART_FLAG_SIZE_X			= 1 << 17,		/// サイズX
	PART_FLAG_SIZE_Y			= 1 << 18,		/// サイズY

	PART_FLAG_U_MOVE			= 1 << 19,		/// UV移動X
	PART_FLAG_V_MOVE			= 1 << 20,		/// UV移動Y
	PART_FLAG_UV_ROTATION		= 1 << 21,		/// UV回転
	PART_FLAG_U_SCALE			= 1 << 22,		/// UVスケールX
	PART_FLAG_V_SCALE			= 1 << 23,		/// UVスケールY
	PART_FLAG_BOUNDINGRADIUS	= 1 << 24,		/// 当たり半径

	PART_FLAG_INSTANCE_KEYFRAME = 1 << 25,		/// インスタンス
	PART_FLAG_INSTANCE_START	= 1 << 26,		/// インスタンス：開始フレーム
	PART_FLAG_INSTANCE_END		= 1 << 27,		/// インスタンス：終了フレーム
	PART_FLAG_INSTANCE_SPEED	= 1 << 28,		/// インスタンス：再生速度
	PART_FLAG_INSTANCE_LOOP		= 1 << 29,		/// インスタンス：ループ回数
	PART_FLAG_INSTANCE_LOOP_FLG = 1 << 30,		/// インスタンス：ループ設定

	NUM_PART_FLAGS
};

/** 頂点変形フラグ */
enum {
	VERTEX_FLAG_LT = 1 << 0,
	VERTEX_FLAG_RT = 1 << 1,
	VERTEX_FLAG_LB = 1 << 2,
	VERTEX_FLAG_RB = 1 << 3,
	VERTEX_FLAG_ONE = 1 << 4	// color blend only
};

/** インスタンスループ設定フラグ */
enum {
	INSTANCE_LOOP_FLAG_INFINITY = 1 << 0,		//
	INSTANCE_LOOP_FLAG_REVERSE = 1 << 1,
	INSTANCE_LOOP_FLAG_PINGPONG = 1 << 2,
	INSTANCE_LOOP_FLAG_INDEPENDENT = 1 << 3,
};

/** Animation Part Type */
enum {
	PARTTYPE_INVALID = -1,
	PARTTYPE_NULL,			/// null。領域を持たずSRT情報のみ。ただし円形の当たり判定は設定可能。
	PARTTYPE_NORMAL,		/// 通常パーツ。領域を持つ。画像は無くてもいい。
	PARTTYPE_TEXT,			/// テキスト(予約　未実装）
	PARTTYPE_INSTANCE,		/// インスタンス。他アニメ、パーツへの参照。シーン編集モードの代替になるもの
	PARTTYPE_NUM
};

/** 当たり判定の種類 */
enum {
	INVALID = -1,
	NONE,			///< 当たり判定として使わない。
	QUAD,			///< 自在に変形する四辺形。頂点変形など適用後の４角を結んだ領域。最も重い。
	AABB,			///< 回転しない全体を囲む矩形で交差判定
	CIRCLE,			///< 真円の半径で距離により判定する
	CIRCLE_SMIN,	///< 真円の半径で距離により判定する (スケールはx,yの最小値をとる）
	CIRCLE_SMAX,	///< 真円の半径で距離により判定する (スケールはx,yの最大値をとる）
	//num
};

/** αブレンド方法 */
enum BlendType {
	BLEND_MIX,		///< 0 ブレンド（ミックス）
	BLEND_MUL,		///< 1 乗算
	BLEND_ADD,		///< 2 加算
	BLEND_SUB		///< 3 減算
};



} //namespace ss
