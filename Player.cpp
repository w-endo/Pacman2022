#include "Player.h"
#include "Gauge.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Debug.h"

//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"),hModel_(-1),pStage_(nullptr),
    maxHp_(120), nowHp_(40)
{
}

//デストラクタ
Player::~Player()
{
}

//初期化
void Player::Initialize()
{
    hModel_ = Model::Load("Player.fbx");
    assert(hModel_ >= 0);

    transform_.position_.x = 2;
    transform_.position_.z = 2;

    pStage_ = (Stage*)FindObject("Stage");
}

//更新
void Player::Update()
{
    prevPosition_ = transform_.position_;

    XMFLOAT3 fMove = XMFLOAT3(0, 0, 0);

    if (Input::IsKey(DIK_LEFT))
    {
        fMove.x = -0.1f;
    }
    if (Input::IsKey(DIK_RIGHT))
    {
        fMove.x = 0.1f;
    }
    if (Input::IsKey(DIK_DOWN))
    {
        fMove.z = -0.1f;
    }
    if (Input::IsKey(DIK_UP))
    {
        fMove.z = 0.1f;
    }

    XMVECTOR vMove;
    vMove = XMLoadFloat3(&fMove);
    vMove = XMVector3Normalize(vMove);
    vMove *= 0.4f;
    XMStoreFloat3(&fMove, vMove);

    transform_.position_.x += fMove.x;
    transform_.position_.z += fMove.z;

    //向き変更
    XMVECTOR vLength = XMVector3Length(vMove);
    float length = XMVectorGetX(vLength);

    if (length != 0)
    {
        XMVECTOR vFront = { 0, 0, 1, 0 };
        vMove = XMVector3Normalize(vMove);

        XMVECTOR vDot = XMVector3Dot(vFront, vMove);
        float dot = XMVectorGetX(vDot);
        float angle = acos(dot);

        XMVECTOR vCross = XMVector3Cross(vFront, vMove);
        if (XMVectorGetY(vCross) < 0)
        {
            angle *= -1;
        }

        transform_.rotate_.y = XMConvertToDegrees(angle);
    }

    //壁との判定
    int checkX;
    int checkZ;

    //右
    {
        checkX = (int)(transform_.position_.x + 0.3f);
        checkZ = (int)transform_.position_.z;
        if (pStage_->IsWall(checkX, checkZ) == true)
        {
            transform_.position_.x = prevPosition_.x;
        }
    }
    //左
    {
        checkX = (int)(transform_.position_.x - 0.3f);
        checkZ = (int)transform_.position_.z;
        if (pStage_->IsWall(checkX, checkZ) == true)
        {
            transform_.position_ = prevPosition_;
        }
    }
    //奥
    {
        checkX = (int)transform_.position_.x;
        checkZ = (int)(transform_.position_.z + 0.3f);
        if (pStage_->IsWall(checkX, checkZ) == true)
        {
            transform_.position_ = prevPosition_;
        }
    }
    //手前
    {
        checkX = (int)transform_.position_.x;
        checkZ = (int)(transform_.position_.z - 0.3f);
        if (pStage_->IsWall(checkX, checkZ) == true)
        {
            transform_.position_ = prevPosition_;
        }
    }

    if (Input::IsKeyDown(DIK_M))
    {
        nowHp_ += 30;
        if (nowHp_ > maxHp_)
        {
            nowHp_ = maxHp_;
        }
    }
    if (Input::IsKeyDown(DIK_N))
    {
        nowHp_ -= 30;
        if (nowHp_ < 0)
        {
            nowHp_ = 0;
        }
    }

    
    Gauge* pGauge = (Gauge*)FindObject("Gauge");
    pGauge->SetHp(nowHp_, maxHp_);
}

//描画
void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Player::Release()
{

}