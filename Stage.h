#pragma once
#include "Engine/GameObject.h"

enum
{
    TYPE_FLOOR,
    TYPE_WALL,
    TYPE_MAX
};


//◆◆◆を管理するクラス
class Stage : public GameObject
{
    int hModel_[TYPE_MAX];    //モデル番号

public:
    //コンストラクタ
    Stage(GameObject* parent);

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};