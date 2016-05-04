#pragma once

namespace ss{
class DataArrayReader;

/** インスタンスパーツの状態を表現(・・・してるものと思われる。SS5Player.cppから抜きだしただけ) */
class InstancePartStatus{
public:
	int m_refKeyframe;	//このインスタンスが配置されたキーフレーム（絶対時間）
	int m_refStartframe;
	int m_refEndframe;
	float m_refSpeed;
	int m_refloopNum;

	bool m_infinity;	//無限ループ
	bool m_reverse;		//逆再生
	bool m_pingpong;	//往復
	bool m_independent;	//独立

	
	InstancePartStatus();

	//インスタンスパーツの状態を読み取り
	void readInstancePartStatus(DataArrayReader &readerm, int readFlags);

	//読み取り後になんか計算してる部分を抜き出した
	int getTime(int time) const;	//timeというかframeだと思う

private:
	int endFrame() const;	//フラグ情報をみて終了フレームを返す
};


} //namespace ss