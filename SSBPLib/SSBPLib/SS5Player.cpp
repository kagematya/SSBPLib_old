#include "SS5Player.h"
#include "SS5PlayerData.h"
#include "common/ssplayer_matrix.h"
#include "SSPlayerImplement/ToPointer.h"
#include "SSPlayerImplement/CustomSprite.h"
#include "SS5ResourceCache.h"
#include "common/SSUtil.h"
#include "SSPlayerImplement/Impl.h"
#include "SSPlayerImplement/ResourceSet.h"
#include "SSPlayerImplement/CellCache.h"
#include "SSPlayerImplement/AnimeCache.h"
#include "SSPlayerImplement/DataArrayReader.h"
#include "SSPlayerImplement/InstancePartStatus.h"
#include "SS5UserData.h"

#include "SS5Renderer.h"
#include "SS5EventListener.h"

namespace ss{
	

SS5Player::SS5Player(const ResourceSet *resource, SS5Renderer *renderer, SS5EventListener *eventListener)
	: m_renderer(renderer)
	, m_eventListener(eventListener)
	, m_currentRs(resource)
	, m_currentAnimeRef(nullptr)
	, m_nowTime(0.0f)
{
	initialize();

	//null回避のために適当にアニメーションをつけておく
	play(m_currentRs->m_animeCache->getFirstReference(), 0);
	SS_ASSERT(m_currentAnimeRef);
}

SS5Player::SS5Player(const ResourceSet *resource, SS5Renderer *renderer, SS5EventListener *eventListener, const std::string& animeName)
	: m_renderer(renderer)
	, m_eventListener(eventListener)
	, m_currentRs(resource)
	, m_currentAnimeRef(nullptr)
	, m_nowTime(0.0f)
{
	initialize();

	//null回避のために適当にアニメーションをつけておく
	play(animeName, 0);
	SS_ASSERT(m_currentAnimeRef);
}

void SS5Player::initialize()
{
	std::fill(std::begin(m_partVisible), std::end(m_partVisible), true);
	std::fill(std::begin(m_insteadCellIndex), std::end(m_insteadCellIndex), -1);
	std::fill(std::begin(m_priorityPartIndex), std::end(m_priorityPartIndex), -1);

	m_state.init();

	//データを取得
	SS_ASSERT2(m_currentRs, "resource is null");
	
	//ロードイベントを投げてcellMapのテクスチャを取得する
	int cellMapNum = m_currentRs->m_cellCache->getCellMapNum();
	m_textures.resize(cellMapNum);
	for(int i = 0; i < cellMapNum; ++i){
		std::string textureName = m_currentRs->m_cellCache->getTexturePath(i);
		TextureID textureid = m_eventListener->SSTextureLoad(textureName.c_str());	//ロードイベント
		m_textures[i] = textureid;
	}
}


SS5Player::~SS5Player()
{
	releaseParts();

	//テクスチャの解放イベントを投げる
	for(TextureID textureid : m_textures){
		m_eventListener->SSTextureRelese(textureid);
	}
	m_textures.clear();
}



int SS5Player::getMaxFrame() const{
	return m_currentAnimeRef->m_animationData->numFrames;
}

void SS5Player::setCurrentFrame(int frame){
	if(getCurrentFrame() == frame){
		return;					//同じフレームのときは時間を維持する(fpsが違うときに、毎フレームcurrentframeをsetされるとまずいので)
	}
	m_nowTime = (frame + 0.5) / getAnimeFPS();	//frame / getAnimeFPS();
}

int SS5Player::getCurrentFrame() const{
	//return _nowTime * getAnimeFPS();
	return ss::clamp<int>(m_nowTime * getAnimeFPS(), 0, getMaxFrame()-1);	//memo:フレーム外を踏ませないための対策
}




void SS5Player::play(const std::string& packName, const std::string& animeName, int startFrameNo)
{	
	const AnimeRef* animeRef = m_currentRs->m_animeCache->getReference(packName, animeName);

	play(animeRef, startFrameNo);
}

void SS5Player::play(const std::string& animeName, int startFrameNo)
{
	const AnimeRef* animeRef = m_currentRs->m_animeCache->getReference(animeName);

	play(animeRef, startFrameNo);
}

void SS5Player::play(const AnimeRef* animeRef, int startFrameNo)
{
	SS_ASSERT(animeRef);
	if (m_currentAnimeRef != animeRef){
		m_currentAnimeRef = animeRef;
		
		allocParts(animeRef->m_numParts);
		setPartsParentage();
	}

	setCurrentFrame(startFrameNo);
	setFrame(startFrameNo);
	
	//play実行時に最初のフレームのユーザーデータを確認する
	checkUserData(startFrameNo);
	checkEvent(startFrameNo);
}


const std::string& SS5Player::getPlayPackName() const
{
	return m_currentAnimeRef->m_packName;
}

const std::string& SS5Player::getPlayAnimeName() const
{
	return m_currentAnimeRef->m_animeName;
}


void SS5Player::update(float dt)
{
	updateFrame(dt);
}

void SS5Player::updateFrame(float dt)
{
	double nextTime = m_nowTime + dt;

	int currentFrame = getCurrentFrame();
	int nextFrame = nextTime * getAnimeFPS();
		
	int seekCount = nextFrame - currentFrame;
	//順再生
	for(int i=0; i < seekCount; ++i){
		//通過フレームのイベントのチェックをする
		int frame = currentFrame + i;
		frame = ss::wrap<int>(frame, 0, getMaxFrame());	//範囲制限

		checkUserData(frame);		//ユーザーデータチェック
		checkEvent(frame);			//イベントチェック
		/*memo:最終フレームなどで止めたいときはここでbreakする必要がある。今はひとまずこのまま*/
	}

	//逆順再生
	for(int i=0; i > seekCount; --i){
		//通過フレームのイベントのチェックをする
		int frame = currentFrame + i;
		frame = ss::wrap<int>(frame, 0, getMaxFrame());	//範囲制限

		checkUserData(frame);		//ユーザーデータチェック
		checkEvent(frame);			//イベントチェック
		/*memo:最終フレームなどで止めたいときはここでbreakする必要がある。今はひとまずこのまま*/
	}
		

	double maxTime = static_cast<double>(getMaxFrame()) / getAnimeFPS();	//下方向に誤差る分には問題ないはず
	m_nowTime = ss::fwrap<double>(nextTime, 0, maxTime);		//範囲内に時間を収める
		
	
	//アニメーションをこのフレームに合わせる
	setFrame(getCurrentFrame());
}


void SS5Player::allocParts(int numParts)
{
	releaseParts();	//すべてのパーツを消す

	// パーツ数だけCustomSpriteを作成する
	m_parts.resize(numParts);
	for(CustomSprite &sprite : m_parts){
		sprite.initialize();
	}
}

void SS5Player::releaseParts()
{
	for(CustomSprite &sprite : m_parts){
		sprite.finalize();
	}

	//m_parts.clear();
}

void SS5Player::setPartsParentage()
{
	ToPointer ptr(m_currentRs->m_data);
	int numParts = getPartNum();

	//親子関係を設定
	for (int partIndex = 0; partIndex < numParts; partIndex++)
	{
		const PartData* partData = m_currentAnimeRef->getPartData(partIndex);
		CustomSprite* sprite = &m_parts.at(partIndex);
		
		if (partIndex > 0){
			sprite->m_parent = &m_parts.at(partData->parentIndex);
		}
		else{
			sprite->m_parent = nullptr;
		}

		//インスタンスパーツの生成
		std::string refanimeName = ptr.toString(partData->refname);

		if (refanimeName != ""){
			//インスタンスパーツが設定されている
			sprite->m_ssplayer.reset( new ss::SS5Player(m_currentRs, m_renderer, m_eventListener, refanimeName) );	//todo:これそのまま_rendererとか_eventListenerとかつっこんでいいかは要検討::eventlistenerにcreateplayerのイベント持たせるとか?
			//sprite->m_ssplayer->play(refanimeName);				 // アニメーション名を指定(ssae名/アニメーション名も可能、詳しくは後述)
		}
	}
}



//ラベル名からラベルの設定されているフレームを取得
//ラベルが存在しない場合は戻り値が-1となります。
//ラベル名が全角でついていると取得に失敗します。
int SS5Player::getLabelToFrame(char* findLabelName)
{
	ToPointer ptr(m_currentRs->m_data);
	const AnimationData* animeData = m_currentAnimeRef->m_animationData;

	if (!animeData->labelData) return -1;
	const ss_offset* labelDataIndex = static_cast<const ss_offset*>(ptr(animeData->labelData));


	for (int idx = 0; idx < animeData->labelNum; idx++ )
	{
		if (!labelDataIndex[idx]) return -1;
		const ss_u16* labelDataArray = static_cast<const ss_u16*>(ptr(labelDataIndex[idx]));

		DataArrayReader reader(labelDataArray);

		LabelData ldata;
		ss_offset offset = reader.readOffset();
		const char* str = ptr.toString(offset);
		int labelFrame = reader.readU16();
		ldata.m_str = str;
		ldata.m_frameNo = labelFrame;

		if (ldata.m_str.compare(findLabelName) == 0 )
		{
			//同じ名前のラベルが見つかった
			return (ldata.m_frameNo);
		}
	}

	return -1;
}


//パーツ数を取得
int SS5Player::getPartNum() const
{
	return m_currentAnimeRef->m_numParts;
}

//indexからパーツ名を取得
const char* SS5Player::getPartName(int partId) const
{
	ToPointer ptr(m_currentRs->m_data);
	
	SS_ASSERT2(partId >= 0 && partId < getPartNum(), "partId is out of range.");

	const PartData* partData = m_currentAnimeRef->getPartData(partId);
	const char* name = ptr.toString(partData->name);
	return name;
}

//パーツ名からindexを取得
int SS5Player::indexOfPart(const char* partName) const
{
	for (int i = 0; i < m_currentAnimeRef->m_numParts; i++){
		
		const char* name = getPartName(i);
		if (strcmp(partName, name) == 0){
			return i;
		}
	}
	return -1;
}


//現在のフレームでのパーツステータスを取得
bool SS5Player::getPartState(ResluteState& result, const char* name) const
{
	//パーツをひいてくる
	int partIndex = indexOfPart(name);
	if(partIndex < 0){
		return false;
	}
	const PartData* partData = m_currentAnimeRef->getPartData(partIndex);

	//必要に応じて取得するパラメータを追加してください。
	//当たり判定などのパーツに付属するフラグを取得する場合は　partData　のメンバを参照してください。
	const CustomSprite* sprite = &m_parts.at(partIndex);
	const SSMatrix& mat = sprite->m_state.m_mat;

	mat.getTranslation(&result.m_x, &result.m_y, &result.m_z);
	mat.getRotation(&result.m_rotationX, &result.m_rotationY, &result.m_rotationZ);
	float z;
	mat.getScale(&result.m_scaleX, &result.m_scaleY, &z);

	result.m_opacity = sprite->m_state.m_opacity;				// 不透明度（0～255）（親子関係計算済）
	result.m_boundingRadius = sprite->m_state.m_boundingRadius;	// SS5アトリビュート：当たり半径
	result.m_isVisibled = sprite->m_state.m_isVisibled;			// 非表示（親子関係計算済）

	//パーツ設定
	result.m_part_type = partData->type;						//パーツ種別
	result.m_part_boundsType = partData->boundsType;			//当たり判定種類
	result.m_part_alphaBlendType = partData->alphaBlendType;	// BlendType

	return true;
}


//特定パーツの表示、非表示を設定します
//パーツ番号はスプライトスタジオのフレームコントロールに配置されたパーツが
//プライオリティでソートされた後、上に配置された順にソートされて決定されます。
void SS5Player::setPartVisible(int partId, bool flg)
{
	SS_ASSERT2(partId >= 0 && partId < getPartNum(), "partId is out of range.");
	m_partVisible[partId] = flg;
}


//cell名からcellIndexを取得
int SS5Player::indexOfCell(const char* cellName) const
{
	return m_currentRs->m_cellCache->indexOfCell(cellName);
}

//パーツIDを指定し、セルを入れ替える(cellIndex==-1ならデフォルト)
void SS5Player::changePartCell(int partId, int cellIndex)
{
	SS_ASSERT2(partId >= 0 && partId < getPartNum(), "partId is out of range.");
	m_insteadCellIndex[partId] = cellIndex;
}



// インスタンスパーツが再生するアニメを変更します。
bool SS5Player::changeInstanceAnime(std::string partsname, std::string animename)
{
	//名前からパーツを取得
	int partIndex = indexOfPart(partsname.c_str());
	if(partIndex < 0){
		return false;
	}

	CustomSprite* sprite = &m_parts.at(partIndex);
	if (sprite->m_ssplayer){
		//パーツがインスタンスパーツの場合は再生するアニメを設定する
		//アニメが入れ子にならないようにチェックする
		if (m_currentAnimeRef->m_animeName != animename){	//todo:package名付け足す必要あるかも
			sprite->m_ssplayer->play(animename);
			return true;
		}
	}

	return false;
}


void SS5Player::setFrame(int frameNo)
{
	// フリップに変化があったときは必ず描画を更新する
	m_parts[0].m_isStateChanged = true;	//root.
	

	ToPointer ptr(m_currentRs->m_data);
	
	const AnimationData* animeData = m_currentAnimeRef->m_animationData;
	const ss_offset* frameDataIndex = static_cast<const ss_offset*>(ptr(animeData->frameData));
	
	const ss_u16* frameDataArray = static_cast<const ss_u16*>(ptr(frameDataIndex[frameNo]));
	DataArrayReader reader(frameDataArray);
	
	const AnimationInitialData* initialDataList = ptr.toAnimationInitialData(animeData);


	for (int index = 0; index < m_currentAnimeRef->m_numParts; index++){
		State state;

		int partIndex = reader.readS16();
		m_priorityPartIndex[index] = partIndex;	//更新はインデックス順ではなく、パーツの描画優先順で来る。ここで描画優先順を記録しておく

		const PartData* partData = m_currentAnimeRef->getPartData(partIndex);
		const AnimationInitialData* init = &initialDataList[partIndex];
		
		//データ読み取り
		state.readData(reader, init);		//memo:Stateの方に処理を移動中

		// optional parameters
		int flags_       = state.m_flags;


		CustomSprite* sprite = &m_parts.at(partIndex);


		int cellIndex_ = state.m_cellIndex;
		if(m_insteadCellIndex[partIndex] >= 0){
			cellIndex_ = m_insteadCellIndex[partIndex];	//cellが差し替わっていればそっちを使う
		}
		const CellRef* cellRef = cellIndex_ >= 0 ? m_currentRs->m_cellCache->getReference(cellIndex_) : nullptr;

		sprite->m_texture = -1;
		if (cellRef){
			//各パーツのテクスチャ情報を設定
			sprite->m_texture = m_textures[cellRef->m_cellMapIndex];
			sprite->m_rect = cellRef->m_rect;
			sprite->m_blendfunc = partData->alphaBlendType;
		}
		

		//頂点データの設定
		//quadにはプリミティブの座標（頂点変形を含む）、UV、カラー値が設定されます。
		SSV3F_C4B_T2F_Quad quad;

		if (cellRef){
			//UVを設定する	//ここもrectをベースに画像サイズからuv出してるだけ
			int atlasWidth = 0;
			int atlasHeight = 0;
			m_renderer->SSGetTextureSize(sprite->m_texture, atlasWidth, atlasHeight);

			state.uvCompute(&quad, cellRef->m_rect, atlasWidth, atlasHeight);
		}

		if(cellRef){
			//頂点を設定する
			SSQuad3 vertexTransform;							//頂点変形のオフセット値
			if(flags_ & PART_FLAG_VERTEX_TRANSFORM){
				vertexTransform.readVertexTransform(reader);	//頂点オフセット読み込み
			}
			state.vertexCompute(&quad, cellRef->m_rect, vertexTransform);
		}

		
		//頂点情報の取得
		SSColor4B color4 = { 0xff, 0xff, 0xff, 0xff };

		quad.tl.colors =
		quad.tr.colors =
		quad.bl.colors =
		quad.br.colors = color4;


		// カラーブレンドの反映
		if (flags_ & PART_FLAG_COLOR_BLEND){

			int typeAndFlags = reader.readU16();
			int funcNo = typeAndFlags & 0xff;
			int cb_flags = (typeAndFlags >> 8) & 0xff;

			sprite->m_state.m_colorBlendFunc = funcNo;
			sprite->m_state.m_colorBlendType = cb_flags;

			if (cb_flags & VERTEX_FLAG_ONE){

				color4.readColorWithRate(reader);
				quad.tl.colors =
				quad.tr.colors =
				quad.bl.colors =
				quad.br.colors = color4;
			}
			else{
				if (cb_flags & VERTEX_FLAG_LT){
					quad.tl.colors.readColorWithRate(reader);
				}
				if (cb_flags & VERTEX_FLAG_RT){
					quad.tr.colors.readColorWithRate(reader);
				}
				if (cb_flags & VERTEX_FLAG_LB){
					quad.bl.colors.readColorWithRate(reader);
				}
				if (cb_flags & VERTEX_FLAG_RB){
					quad.br.colors.readColorWithRate(reader);
				}
			}
		}
		
		//スプライトステータスの保存
		sprite->setState(state);
		sprite->m_quad = quad;


		//インスタンスパーツの場合
		if (partData->type == PARTTYPE_INSTANCE){

			InstancePartStatus ips;
			ips.readInstancePartStatus(reader, flags_);	//データ読み取り


			//タイムライン上の時間 （絶対時間）
			int time = frameNo;		//todo:timeじゃなくてframeっぽい

			//独立動作の場合
			if (ips.m_independent){
				//memo:この辺り、updateのほうに持って行きたいが・・・
				float dt = 1000.0f/60.0f;
				float delta = sprite->m_ssplayer->getAnimeFPS() / dt;	//ゲームFPSからアニメーション時間を求める

				sprite->m_liveFrame += delta;
				time = static_cast<int>(sprite->m_liveFrame);
			}

			//インスタンス用SSPlayerに再生フレームを設定する
			time = ips.getTime(time);
			sprite->m_ssplayer->setCurrentFrame(time);

			//インスタンスパラメータを設定
			sprite->m_ssplayer->setAlpha(state.m_opacity);
			sprite->m_ssplayer->setRotation(state.m_rotationX, state.m_rotationY, state.m_rotationZ);
		}

	}


	/*-------------------------------------------------------------*/
	// 親に変更があるときは自分も更新するようフラグを設定する
	for (int partIndex = 0; partIndex < m_currentAnimeRef->m_numParts; partIndex++){
		CustomSprite* sprite = &m_parts.at(partIndex);
		CustomSprite* parent = sprite->m_parent;
		
		if(parent && parent->m_isStateChanged){
			sprite->m_isStateChanged = true;
		}
	}

	/*-------------------------------------------------------------*/
	// 行列の更新
	{
		//rootの設定を渡す
		SSMatrix rootMatrix;
		m_state.matrixCompute(&rootMatrix);
		int rootOpacity = m_state.m_opacity;
		
		for (int partIndex = 0; partIndex < m_currentAnimeRef->m_numParts; partIndex++){
			CustomSprite* sprite = &m_parts.at(partIndex);

			if (sprite->m_isStateChanged){			
				sprite->updateMatrixAndOpacity(rootMatrix, rootOpacity);
			}
		}
	}

	//頂点データにmatrixを掛ける
	for(int partIndex = 0; partIndex < m_currentAnimeRef->m_numParts; partIndex++){
		CustomSprite* sprite = &m_parts.at(partIndex);
		sprite->m_quad.vertexForeach([&](SSVertex3F &v){
			v *= sprite->m_state.m_mat;				//頂点変換
		});
		sprite->m_quad.colorsForeach([&](SSColor4B &c){
			double rate = sprite->m_state.m_opacity / 255.0;
			c.a *= rate;		//アルファ適用
		});
	}
}

//プレイヤーの描画
void SS5Player::draw()
{
	for (int index = 0; index < m_currentAnimeRef->m_numParts; index++)
	{
		int partIndex = m_priorityPartIndex[index];	//描画優先順に沿ってパーツを描画する
		//スプライトの表示
		CustomSprite* sprite = &m_parts.at(partIndex);
		if (sprite->m_ssplayer){
			//インスタンスパーツの場合は子供のプレイヤーを再生
			sprite->m_ssplayer->update(0);
			sprite->m_ssplayer->draw();
		}
		else{

			if (m_partVisible[partIndex]){		//ユーザーが任意に非表示としたパーツは描画しない
				if ((sprite->m_texture != -1) && (sprite->m_state.m_isVisibled == true)){

					m_renderer->drawSprite(sprite->m_quad, sprite->m_texture);	//sprite->_blendfunc
				}
			}
		}
	}
}

void SS5Player::checkUserData(int frameNo)
{
	ToPointer ptr(m_currentRs->m_data);

	const AnimationData* animeData = m_currentAnimeRef->m_animationData;

	if (!animeData->userData) return;
	const ss_offset* userDataIndex = static_cast<const ss_offset*>(ptr(animeData->userData));

	if (!userDataIndex[frameNo]) return;
	const ss_u16* userDataArray = static_cast<const ss_u16*>(ptr(userDataIndex[frameNo]));
	
	DataArrayReader reader(userDataArray);
	int numUserData = reader.readU16();

	for (int i = 0; i < numUserData; i++){
		UserData userData;
		userData.readData(reader, ptr);
		
		const char *partName = getPartName(userData.m_partIndex);

		m_eventListener->SSonUserData(this, userData, partName, frameNo);
	}
}

void SS5Player::checkEvent(int frameNo)
{
	if(frameNo == 0){
		m_eventListener->SSonBeginFrame(this);	//最初のフレームを踏んだ
	}
	
	if(frameNo == getMaxFrame() -1){
		m_eventListener->SSonEndFrame(this);		//最終フレームを踏んだ
	}
}

int SS5Player::getAnimeFPS() const{
	return m_currentAnimeRef->m_animationData->fps;
}




void  SS5Player::setPosition(float x, float y)
{
	m_state.m_x = x;
	m_state.m_y = y;
}
void  SS5Player::getPosition(float *x, float *y) const{
	*x = m_state.m_x;
	*y = m_state.m_y;
}
void  SS5Player::setRotation(float x, float y, float z)
{
	m_state.m_rotationX = x;
	m_state.m_rotationY = y;
	m_state.m_rotationZ = z;
}
void  SS5Player::getRotation(float *x, float *y, float *z) const{
	*x = m_state.m_rotationX;
	*y = m_state.m_rotationY;
	*z = m_state.m_rotationZ;
}


void  SS5Player::setScale(float x, float y)
{
	m_state.m_scaleX = x;
	m_state.m_scaleY = y;
}
void SS5Player::getScale(float *x, float *y) const{
	*x = m_state.m_scaleX;
	*y = m_state.m_scaleY;
}

void  SS5Player::setAlpha(int alpha){
	m_state.m_opacity = alpha;
}
int SS5Player::getAlpha() const{
	return m_state.m_opacity;
}

void  SS5Player::setFlip(bool flipX, bool flipY)
{
	m_state.m_flipX = flipX;
	m_state.m_flipY = flipY;
}
void  SS5Player::getFlip(bool *flipX, bool *flipY) const{
	*flipX = m_state.m_flipX;
	*flipY = m_state.m_flipY;
}



void SS5Player::getAnimationList(std::list<std::string> *animlist) const{
	m_currentRs->m_animeCache->getAnimationList(animlist);
}


};
