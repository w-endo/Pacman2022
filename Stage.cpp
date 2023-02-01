#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"

//コンストラクタ
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), 
    hModel_{ -1, -1 },table_(nullptr),
    width_(0), height_(0)
{
    //ZeroMemory(table_, sizeof(table_));

    CsvReader csv;
    csv.Load("map.csv");

    width_ = csv.GetWidth();
    height_ = csv.GetHeight();

    table_ = new int* [width_];

    for (int x = 0; x < width_; x++)
    {
        table_[x] = new int[height_];
    }

    for (int x = 0; x < width_; x++)
    {
        for (int z = 0; z < height_; z++)
        {
            table_[x][height_ -1 -z] = csv.GetValue(x, z);
        }
    }

}

//デストラクタ
Stage::~Stage()
{
}

//初期化
void Stage::Initialize()
{
    const char* fileName[] = { 
        "Floor.fbx" ,
        "Wall.fbx" 
    };

    //モデルデータのロード
    for (int i = 0; i < TYPE_MAX; i++)
    {
        hModel_[i] = Model::Load(fileName[i]);
        assert(hModel_[i] >= 0);
    }

}

//更新
void Stage::Update()
{
}

//描画
void Stage::Draw()
{
    Transform blockTrans;

    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            blockTrans.position_.x = x;
            blockTrans.position_.z = z;

            int type = table_[x][z];

            Model::SetTransform(hModel_[type], blockTrans);
            Model::Draw(hModel_[type]);
        }
    }
}

//開放
void Stage::Release()
{
    for (int x = 0; x < width_; x++)
    {
        delete[] table_[x];
    }
    delete[] table_;
}