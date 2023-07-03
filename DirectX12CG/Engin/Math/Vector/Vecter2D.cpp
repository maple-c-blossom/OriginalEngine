#include "Vecter2D.h"
#include <cmath>
//float Vec2::GetCrossProduct(Vec2 startVec,Vec2 endVec)//提出後改良
//{
//    VECTOR vecLine = VGet(endVec.GetX() - startVec.GetX(), endVec.GetY() - startVec.GetY(), 0);
//    vecLine = VNorm(vecLine);
//    VECTOR 	vecCur = VGet(x - startVec.GetX(), y - startVec.GetY(), 0);
//    vecCur = VNorm(vecCur);
//    return vecCur.x * vecLine.y - vecCur.y * vecLine.x;
//}
using namespace MCB;

Vector2D Vector2D::VecGet(const Vector2D& startVec,const Vector2D& endVec)
{
    Vector2D Vectol;
    Vectol.SetX(endVec.GetX() - startVec.GetX());
    Vectol.SetY(endVec.GetY() - startVec.GetY());
    return Vectol;
}

Vector2D Vector2D::VecGet(const Vector2D& startVec)
{
    Vector2D Vectol;
    Vectol.SetX(x_ - startVec.GetX());
    Vectol.SetY(y_ - startVec.GetY());
    return Vectol;
}

Vector2D::Vector2D()
{
    x_ = 0;
    y_ = 0;

}

Vector2D::Vector2D(float x, float y)
{
    x_ = x;
    y_ = y;
}

float Vector2D::GetCrossProduct(const Vector2D& a, const Vector2D& b)
{
    return a.x_ * b.y_ - a.y_ * b.x_;
}

float Vector2D::GetCrossProduct(const Vector2D& b)
{
    return x_ * b.y_ - y_ * b.x_;
}

float Vector2D::GetDot(const Vector2D& vec)
{
    return x_ * vec.x_ + y_ * vec.y_;
}

bool Vector2D::HitCrossProduct(Vector2D* points, int32_t ArraySize, const Vector2D& point)
{
    if (points == nullptr) return false;
    bool isHit = false;
    for (int32_t i = 0; i < ArraySize; i++)
    {
        Vector2D& vecs = points[i];
        Vector2D& vece = points[i + 1 % ArraySize];

        Vector2D a;
        a.SetVec(VecGet(vecs, vece));

        Vector2D b;
        b.SetVec(VecGet(vecs, point));


        if (GetCrossProduct(a, b) >= 0)
        {
            isHit = true;;
        }
        else
        {
            isHit = false;
            break;
        }
    }
    return isHit;
}

bool Vector2D::HitCrossProduct(Vector2D* points, int32_t ArraySize)
{
    if (points == nullptr) return false;
    bool isHit = false;
    for (int32_t i = 0; i < ArraySize; i++)
    {
        Vector2D& vecs = points[i];
        Vector2D& vece = points[(i + 1) % ArraySize];

        Vector2D a;
        a.SetVec(VecGet(vecs, vece));

        Vector2D b;
        b.SetVec(VecGet(vecs));


        if (GetCrossProduct(a, b) >= 0)
        {
            isHit = true;;
        }
        else
        {
            isHit = false;
            break;
        }
    }
    return isHit;
}

float Vector2D::GetLenge()const
{
    return sqrtf(x_ * x_ + y_ * y_);
}

void Vector2D::VecNorm()
{
    float vecLen = GetLenge();
    x_ = x_ / vecLen;
    y_ = y_ / vecLen;
}


float Vector2D::GetX()const
{
    return x_;
}

float Vector2D::GetY()const
{
    return y_;
}


void Vector2D::SetX( float x)
{
    x_ = x;
}

void Vector2D::SetY( float y)
{
    y_ = y;
}



void Vector2D::SetVec(const Vector2D& vec)
{
    x_ = vec.x_;
    y_ = vec.y_;
}

Vector2D Vector2D::operator+()
{
    return Vector2D(*this);
}

Vector2D Vector2D::operator-()
{
    Vector2D temp = *this;
    temp *= -1;
    return temp;
}




Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
    x_ = x_ + vec.x_;
    y_ = y_ + vec.y_;
    return *this;
    // TODO: return ステートメントをここに挿入します
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
    x_ = x_ - vec.x_;
    y_ = y_ - vec.y_;
    return *this;
    // TODO: return ステートメントをここに挿入します
}

Vector2D& Vector2D::operator*=( float speed)
{
    x_ *= speed;
    y_ *= speed;
    return *this;
    // TODO: return ステートメントをここに挿入します
}

Vector2D& Vector2D::operator/=( float Lenge)
{
    x_ /= Lenge;
    y_ /= Lenge;
    return *this;
    // TODO: return ステートメントをここに挿入します
}

Vector2D operator- (const Vector2D& a, const Vector2D& b)
{
    Vector2D temp(a);
    return temp -= b;
}

Vector2D operator +(const Vector2D& a, const Vector2D& b)
{
    Vector2D temp(a);
    return temp += b;
}

Vector2D operator /(Vector2D a, float Lenge)
{
    Vector2D temp(a);
    return temp /= Lenge;
}

Vector2D operator *(Vector2D a, float speed)
{
    Vector2D temp(a);
    return temp *= speed;
}

