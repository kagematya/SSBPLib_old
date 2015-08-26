#include "SSPlayerStatus.h"
#include "common/SSUtil.h"
#include "common/SSRect.h"
#include "SS5PlayerTypes.h"
#include "SSPlayerImplement/DataArrayReader.h"
#include "SS5PlayerData.h"

namespace ss{


void State::init(){
	m_flags = 0;
	m_cellIndex = 0;
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
	m_anchorX = 0.0f;
	m_anchorY = 0.0f;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
	m_scaleX = 1.0f;
	m_scaleY = 1.0f;
	m_opacity = 255;
	m_size_X = 1.0f;
	m_size_Y = 1.0f;
	m_uv_move_X = 0.0f;
	m_uv_move_Y = 0.0f;
	m_uv_rotation = 0.0f;
	m_uv_scale_X = 1.0f;
	m_uv_scale_Y = 1.0f;
	m_boundingRadius = 0.0f;
	m_flipX = false;
	m_flipY = false;
	m_isVisibled = false;
	m_colorBlendFunc = 0;
	m_colorBlendType = 0;	


	m_mat.setupIdentity();
}


bool State::operator==(const State &s) const{
	//memo:実装は旧CustomSpriteのsetStateを参考にした。いくつかの項目を諦めてたので、ひとまずそれに順ずる
	return (
		m_flags == s.m_flags
		&& m_cellIndex == s.m_cellIndex
		&& m_x == s.m_x
		&& m_y == s.m_y
		&& m_z == s.m_z
		&& m_anchorX == s.m_anchorX
		&& m_anchorY == s.m_anchorY
		&& m_rotationX == s.m_rotationX
		&& m_rotationY == s.m_rotationY
		&& m_rotationZ == s.m_rotationZ
		&& m_scaleX == s.m_scaleX
		&& m_scaleY == s.m_scaleY
		&& m_opacity == s.m_opacity
		&& m_size_X == s.m_size_X
		&& m_size_Y == s.m_size_Y
		&& m_uv_move_X == s.m_uv_move_X
		&& m_uv_move_Y == s.m_uv_move_Y
		&& m_uv_rotation == s.m_uv_rotation
		&& m_uv_scale_X == s.m_uv_scale_X
		&& m_uv_scale_Y == s.m_uv_scale_Y
		&& m_boundingRadius == s.m_boundingRadius
		&& m_flipX == s.m_flipX
		&& m_flipY == s.m_flipY
		&& m_isVisibled == s.m_isVisibled
		&& m_colorBlendFunc == s.m_colorBlendFunc
		&& m_colorBlendType == s.m_colorBlendType
		
		//&& mat == s.mat
	);	
}

bool State::operator!=(const State &s) const{
	return !(*this == s);
}



//SS5Player.cpp::setFrame()内の処理をごっそり持ってきた
void State::readData(DataArrayReader &reader, const AnimationInitialData *init)
{
	int flags_       = reader.readU32();

	m_flags      = flags_;
	m_cellIndex  = flags_ & PART_FLAG_CELL_INDEX ? reader.readS16() : init->cellIndex;
	m_x          = flags_ & PART_FLAG_POSITION_X ? (float)reader.readS16() : (float)init->positionX;
	m_y          = flags_ & PART_FLAG_POSITION_Y ? (float)-reader.readS16() : (float)-init->positionY;	//上がマイナスなので反転させる
	m_z          = flags_ & PART_FLAG_POSITION_Z ? (float)reader.readS16() : (float)init->positionZ;	//上がマイナスなので反転させる
	m_anchorX    = flags_ & PART_FLAG_ANCHOR_X ? reader.readFloat() : init->anchorX;
	m_anchorY    = flags_ & PART_FLAG_ANCHOR_Y ? reader.readFloat() : init->anchorY;
	m_rotationX  = flags_ & PART_FLAG_ROTATIONX ? -reader.readFloat() : -init->rotationX;
	m_rotationY  = flags_ & PART_FLAG_ROTATIONY ? -reader.readFloat() : -init->rotationY;
	m_rotationZ  = flags_ & PART_FLAG_ROTATIONZ ? -reader.readFloat() : -init->rotationZ;
	m_scaleX     = flags_ & PART_FLAG_SCALE_X ? reader.readFloat() : init->scaleX;
	m_scaleY     = flags_ & PART_FLAG_SCALE_Y ? reader.readFloat() : init->scaleY;
	m_opacity    = flags_ & PART_FLAG_OPACITY ? reader.readU16() : init->opacity;
	m_size_X     = flags_ & PART_FLAG_SIZE_X ? reader.readFloat() : init->size_X;
	m_size_Y     = flags_ & PART_FLAG_SIZE_Y ? reader.readFloat() : init->size_Y;
	m_uv_move_X   = flags_ & PART_FLAG_U_MOVE ? reader.readFloat() : init->uv_move_X;
	m_uv_move_Y   = flags_ & PART_FLAG_V_MOVE ? reader.readFloat() : init->uv_move_Y;
	m_uv_rotation = flags_ & PART_FLAG_UV_ROTATION ? reader.readFloat() : init->uv_rotation;
	m_uv_scale_X  = flags_ & PART_FLAG_U_SCALE ? reader.readFloat() : init->uv_scale_X;
	m_uv_scale_Y  = flags_ & PART_FLAG_V_SCALE ? reader.readFloat() : init->uv_scale_Y;
	m_boundingRadius = flags_ & PART_FLAG_BOUNDINGRADIUS ? reader.readFloat() : init->boundingRadius;
	m_flipX       = static_cast<bool>(flags_ & PART_FLAG_FLIP_H);
	m_flipY       = static_cast<bool>(flags_ & PART_FLAG_FLIP_V);
	m_isVisibled = !(flags_ & PART_FLAG_INVISIBLE);

	//固定少数を少数へ戻す
	static const float DOT = 10.0f;			/// 固定少数の定数 10=1ドット
	m_x /= DOT;
	m_y /= DOT;
	m_z /= DOT;
}



//uv計算
void State::uvCompute(SSV3F_C4B_T2F_Quad *q, const SSRect &cellRect, int imgWidth, int imgHeight) const
{
	float left, right, top, bottom;
	left = cellRect.origin.x / static_cast<float>(imgWidth);
	right = (cellRect.origin.x + cellRect.size.width) / static_cast<float>(imgWidth);
	top = cellRect.origin.y / static_cast<float>(imgHeight);
	bottom = (cellRect.origin.y + cellRect.size.height) / static_cast<float>(imgHeight);

	q->bl.texCoords = {left, bottom};
	q->br.texCoords = {right, bottom};
	q->tl.texCoords = {left, top};
	q->tr.texCoords = {right, top};


	//uvスクロール	//flagチェックする？
	{
		SSTex2F uvMove(m_uv_move_X, m_uv_move_Y);
		q->uvForeach( [&uvMove](SSTex2F &uv){
			uv += uvMove;
		});
	}


	//計算用にuv中心を求めておく
	//memo:回転・スケールで中央への戻し処理が多発するのでボトルネックになるようなら最初から中心合わせで計算して最後にオフセットを足すようにするのがいいかもしれない
	SSTex2F uvCenter = (q->br.texCoords + q->tl.texCoords) / 2.0f;


	//UV回転
	if (m_flags & PART_FLAG_UV_ROTATION){
		//頂点位置を回転させる
		float rotateRadian = DegreeToRadian(m_uv_rotation);
		q->uvForeach([&](SSTex2F &uv){
			uv.rotate(rotateRadian, uvCenter);
		});
	}

	//UVスケール
	if((m_flags & PART_FLAG_U_SCALE) || (m_flags & PART_FLAG_V_SCALE)){
		q->uvForeach([&](SSTex2F &uv){
			//中心を基準として拡大縮小させる
			uv -= uvCenter;
			uv.u *= m_uv_scale_X;
			uv.v *= m_uv_scale_Y;
			uv += uvCenter;
		});
	}

	
	////UV反転
	//if (m_flags & PART_FLAG_FLIP_H){	//左右反転を行う場合はlr入れ替え
	//	std::swap(q->tr.texCoords.u, q->tl.texCoords.u);
	//	std::swap(q->br.texCoords.u, q->bl.texCoords.u);
	//}
	//if (m_flags & PART_FLAG_FLIP_H){	//上下反転を行う場合はtb入れ替え
	//	std::swap(q->tr.texCoords.v, q->br.texCoords.v);
	//	std::swap(q->tl.texCoords.v, q->bl.texCoords.v);
	//}
}



//頂点計算
void State::vertexCompute(SSV3F_C4B_T2F_Quad *q, const SSRect &cellRect, const SSQuad3 &vertexTransform) const
{
	//memo:scaleとかrotateとかは後でmatrixを掛けて反映させる。今は中心(0,0)基準の矩形(+頂点変化)を作るだけ

	//ひとまずrectをベースに矩形をセットする
	float width = cellRect.size.width;
	float height = cellRect.size.height;
	q->bl.vertices = SSVertex3F(0, 0, 0);
	q->br.vertices = SSVertex3F(width, 0, 0);
	q->tl.vertices = SSVertex3F(0, height, 0);
	q->tr.vertices = SSVertex3F(width, height, 0);  //yが上方向+なので、左下基準の矩形になってるらしい
			
	//サイズ変化しているなら、そちらに設定を合わせる
	//普通はscaleで指定されるので、このフラグがONになるのはレアケース?
	//詳しくは http://www.webtech.co.jp/help/ja/spritestudio/window/attributewindow/
	//「アンカー機能を持つ特殊なパーツ」についての項目を参照
	if(m_flags & PART_FLAG_SIZE_X){
		q->br.vertices.x = m_size_X;
		q->tr.vertices.x = m_size_X;
	}
	if(m_flags & PART_FLAG_SIZE_Y){
		q->tl.vertices.y = m_size_Y;
		q->tr.vertices.y = m_size_Y;
	}

	//memo:座標系がy上方向が-になるように調整してます
	std::swap(q->tl.vertices.y, q->bl.vertices.y);
	std::swap(q->tr.vertices.y, q->br.vertices.y);


	////中心が座標基準なのでオフセットを追加
	SSVertex3F center = (q->bl.vertices + q->tr.vertices) / 2;
	q->bl.vertices -= center;
	q->br.vertices -= center;
	q->tl.vertices -= center;
	q->tr.vertices -= center;

	
	// 頂点変形のオフセット値を反映
	if (m_flags & PART_FLAG_VERTEX_TRANSFORM){
		*q += vertexTransform;
	}
}



//matrix
void State::matrixCompute(SSMatrix *matrix) const
{
	SSMatrix tmp;
	SSPoint flipScale(
		(m_flipX)? -1:1,		//反転だったら-1, 通常だったら1
		(m_flipY)? -1:1		//
	);

	matrix->setupIdentity();
	*matrix *= tmp.setupScale(m_scaleX * flipScale.x, m_scaleY * flipScale.y);
	*matrix *= tmp.setupRotationZ(DegreeToRadian(m_rotationZ));
	*matrix *= tmp.setupRotationY(DegreeToRadian(m_rotationY));
	*matrix *= tmp.setupRotationX(DegreeToRadian(m_rotationX));
	*matrix *= tmp.setupTranslation(m_x, m_y);
}



} //namespace ss
