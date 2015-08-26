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


//読み取り
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
			m_infinity = true;		//無限ループ
		}
		if (lflags & INSTANCE_LOOP_FLAG_REVERSE){
			m_reverse = true;		//逆再生
		}
		if (lflags & INSTANCE_LOOP_FLAG_PINGPONG){
			m_pingpong = true;		//往復
		}
		if (lflags & INSTANCE_LOOP_FLAG_INDEPENDENT){
			m_independent = true;	//独立
		}
	}
}


//読み取り後の計算
int InstancePartStatus::getTime(int time) const
{
	int	reftime = static_cast<int>(time * m_refSpeed) - m_refKeyframe; //開始から現在の経過時間
	if(reftime < 0){ return time; }									   //そもそも生存時間に存在していない

	int inst_scale = (m_refEndframe - m_refStartframe) + 1; //インスタンスの尺
	if (inst_scale <= 0){ return time; }					//尺が０もしくはマイナス（あり得ない

	int	nowloop = reftime / inst_scale;		//現在までのループ数
	int nowframe = reftime % inst_scale;	//ループを加味しないインスタンスアニメ内のフレーム
	

	//無限ループで無い時にループ数をチェック
	if (!m_infinity){
		//回数制限があるので終端チェックをする
		int checkloopnum = m_refloopNum;		//ループ終了数
		if (m_pingpong){ checkloopnum *= 2; }	//pingpongの場合では２倍にする

		if (nowloop >= checkloopnum){
			return endFrame();		//ループ終了のときは終わりのフレームを返す
		}
	}

	//現在の再生フレームの計算
	bool isReverse = m_reverse;
	if (m_pingpong && (nowloop % 2 == 1)){
		isReverse = !isReverse;	//反転
	}

	if (isReverse){
		return m_refEndframe - nowframe;	//リバースの時
	}
	return nowframe + m_refStartframe;	//通常時
}



//フラグを考慮して終了フレームを返す
int InstancePartStatus::endFrame() const
{
	if(m_pingpong && m_reverse){
		return m_refEndframe;		//pingpong && 反転
	}
	else if(m_pingpong){
		return m_refStartframe;		//pingpong
	}
	else if(m_reverse){
		return m_refStartframe;		//反転
	}
	
	return m_refEndframe;			//通常
}



} //namespace ss
