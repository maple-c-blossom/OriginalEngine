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

Vector2D Vector2D::VecGet(Vector2D startVec, Vector2D endVec)
{
    Vector2D Vectol;
    Vectol.SetX(endVec.GetX() - startVec.GetX());
    Vectol.SetY(endVec.GetY() - startVec.GetY());
    return Vectol;
}

Vector2D Vector2D::VecGet(Vector2D startVec)
{
    Vector2D Vectol;
    Vectol.SetX(x - startVec.GetX());
    Vectol.SetY(y - startVec.GetY());
    return Vectol;
}

Vector2D::Vector2D()
{
    x = 0;
    y = 0;

}

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Vector2D::GetCrossProduct(Vector2D a, Vector2D b)
{
    return a.x * b.y - a.y * b.x;
}

float Vector2D::GetCrossProduct(Vector2D b)
{
    return x * b.y - y * b.x;
}

float Vector2D::GetDot(Vector2D vec)
{
    return x * vec.x + y * vec.y;
}

bool Vector2D::HitCrossProduct(Vector2D* points, const int ArraySize, const Vector2D point)
{
    if (points == nullptr) return false;
    bool isHit = false;
    for (int i = 0; i < ArraySize; i++)
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

bool Vector2D::HitCrossProduct(Vector2D* points, const int ArraySize)
{
    if (points == nullptr) return false;
    bool isHit = false;
    for (int i = 0; i < ArraySize; i++)
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
    return sqrtf(x * x + y * y);
}

void Vector2D::VecNorm()
{
    float vecLen = GetLenge();
    x = x / vecLen;
    y = y / vecLen;
}


float Vector2D::GetX()const
{
    return x;
}

float Vector2D::GetY()const
{
    return y;
}


void Vector2D::SetX(float x)
{
    this->x = x;
}

void Vector2D::SetY(float y)
{
    this->y = y;
}



void Vector2D::SetVec(Vector2D vec)
{
    x = vec.x;
    y = vec.y;
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
    this->x = this->x + vec.x;
    this->y = this->y + vec.y;
    return *this;
    // TODO: return ステートメントをここに挿入します
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
    this->x = this->x - vec.x;
    this->y = this->y - vec.y;
    return *this;
    // TODO: return ステートメントをここに挿入します
}

Vector2D& Vector2D::operator*=(float speed)
{
    x *= speed;
    y *= speed;
    return *this;
    // TODO: return ステートメントをここに挿入します
}

Vector2D& Vector2D::operator/=(float Lenge)
{
    x /= Lenge;
    y /= Lenge;
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

