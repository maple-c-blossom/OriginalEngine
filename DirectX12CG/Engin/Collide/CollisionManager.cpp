#include "BaseCollider.h"
#include "CollisionInfomation.h"
#include "CollisionManager.h"


MCB::CollisionManager* MCB::CollisionManager::GetInstance()
{
    static CollisionManager inst;
    return &inst;
}

void MCB::CollisionManager::CheckAllCollision()
{
    std::forward_list<BaseCollider*>::iterator itrA;
    std::forward_list<BaseCollider*>::iterator itrB;

    itrA = colliders.begin();
    for (; itrA != colliders.end(); ++itrA)
    {
        itrB = itrA;
        ++itrB;
        for (; itrB != colliders.end(); ++itrB)
        {
            BaseCollider* itrCollA = *itrA;
            BaseCollider* itrCollB = *itrB;
            if (itrCollA->GetPrimitive() == PrimitiveType::SPHERE && itrCollB->GetPrimitive() == PrimitiveType::SPHERE)
            {
                Sphere* sphereA = dynamic_cast<Sphere*>(itrCollA);
                Sphere* sphereB = dynamic_cast<Sphere*>(itrCollB);
                Vector3D inter;
                if (Collision::CalcSphere(*sphereA, *sphereB,&inter)) {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
                else {
                    itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
            else if (itrCollA->GetPrimitive() == PrimitiveType::RAY && itrCollB->GetPrimitive() == PrimitiveType::SPHERE)
            {
                Ray* rayA = dynamic_cast<Ray*>(itrCollA);
                Sphere* sphereB = dynamic_cast<Sphere*>(itrCollB);
                Vector3D inter;
                float distance;
                if (Collision::CalcRaySphere(*rayA, *sphereB, &distance, &inter)) {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter,distance));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter,distance));
                }
                else {
                    itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
            else if (itrCollA->GetPrimitive() == PrimitiveType::SPHERE && itrCollB->GetPrimitive() == PrimitiveType::RAY)
            {
                Sphere* sphereA = dynamic_cast<Sphere*>(itrCollA);
                Ray* rayB = dynamic_cast<Ray*>(itrCollB);
                Vector3D inter;
                float distance;
                if (Collision::CalcRaySphere(*rayB, *sphereA, &distance, &inter)) {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter,distance));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter,distance));
                }
                else {
                    itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
            else if (itrCollA->GetPrimitive() == PrimitiveType::PLANE && itrCollB->GetPrimitive() == PrimitiveType::SPHERE)
            {
                Plane* planeA = dynamic_cast<Plane*>(itrCollA);
                Sphere* sphereB = dynamic_cast<Sphere*>(itrCollB);
                Vector3D inter;
                if (Collision::CalcPlaneSpher(*planeA, *sphereB, &inter)) {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
                else {
                    itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
            else if (itrCollA->GetPrimitive() == PrimitiveType::SPHERE && itrCollB->GetPrimitive() == PrimitiveType::PLANE)
            {
                Sphere* sphereA = dynamic_cast<Sphere*>(itrCollA);
                Plane* planeB = dynamic_cast<Plane*>(itrCollB);
                Vector3D inter;
                if (Collision::CalcPlaneSpher(*planeB, *sphereA, &inter)) {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
                else {
                    itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
            else if (itrCollA->GetPrimitive() == PrimitiveType::PLANE && itrCollB->GetPrimitive() == PrimitiveType::RAY)
            {
                Plane* planeA = dynamic_cast<Plane*>(itrCollA);
                Ray* rayB = dynamic_cast<Ray*>(itrCollB);
                Vector3D inter;
                float distance;
                if (Collision::CalcPlaneRay(*planeA, *rayB, &distance , &inter)) {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter,distance));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter,distance));
                }
                else {
                    itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
            else if (itrCollA->GetPrimitive() == PrimitiveType::RAY && itrCollB->GetPrimitive() == PrimitiveType::PLANE)
            {
                Ray* rayA = dynamic_cast<Ray*>(itrCollA);
                Plane* planeB = dynamic_cast<Plane*>(itrCollB);
                Vector3D inter;
                float distance;
                if (Collision::CalcPlaneRay(*planeB, *rayA,&distance, &inter)) {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter,distance));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter,distance));
                }
                else {
                    itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
            else if (itrCollA->GetPrimitive() == PrimitiveType::TRIANGLE && itrCollB->GetPrimitive() == PrimitiveType::RAY)
            {
                Triangle* triangleA = dynamic_cast<Triangle*>(itrCollA);
                Ray* rayB = dynamic_cast<Ray*>(itrCollB);
                Vector3D inter;
                float distance;
                if (Collision::CalcTriangleRay(*triangleA, *rayB, &distance, &inter)) {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter, distance));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter, distance));
                }
                else {
                    itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
            else if (itrCollA->GetPrimitive() == PrimitiveType::RAY && itrCollB->GetPrimitive() == PrimitiveType::TRIANGLE)
            {
                Ray* rayA = dynamic_cast<Ray*>(itrCollA);
                Triangle* triangleB = dynamic_cast<Triangle*>(itrCollB);
                Vector3D inter;
                float distance;
                if (Collision::CalcTriangleRay(*triangleB, *rayA, &distance, &inter)) {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter, distance));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter, distance));
                }
                else {
                    itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
            else if (itrCollA->GetPrimitive() == PrimitiveType::TRIANGLE && itrCollB->GetPrimitive() == PrimitiveType::SPHERE)
            {
                Triangle* triangleA = dynamic_cast<Triangle*>(itrCollA);
                Sphere* sphereB = dynamic_cast<Sphere*>(itrCollB);
                Vector3D inter;
                if (Collision::CalcTriangleSpher(*triangleA, *sphereB, &inter)) {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
                else {
                    itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
            else if (itrCollA->GetPrimitive() == PrimitiveType::SPHERE && itrCollB->GetPrimitive() == PrimitiveType::TRIANGLE)
            {
                Sphere* sphereA = dynamic_cast<Sphere*>(itrCollA);
                Triangle* triangleB = dynamic_cast<Triangle*>(itrCollB);
                Vector3D inter;
                if (Collision::CalcTriangleSpher(*triangleB, *sphereA, &inter)) {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
                else {
                    itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
        }
    }
}

bool MCB::CollisionManager::Raycast(const Ray& ray, RayCastHit* hitinfo, float maxDistance)
{
    bool result = false;
    std::forward_list<BaseCollider*>::iterator itr;
    std::forward_list<BaseCollider*>::iterator itr_hit;
    float dist = maxDistance;
    Vector3D inter;
    itr = colliders.begin();
    for (; itr != colliders.end(); ++itr)
    {
        BaseCollider* col = *itr;
        if (col->GetPrimitive() == PrimitiveType::SPHERE)
        {
            Sphere* prim = dynamic_cast<Sphere*>(col);
            float disttemp;
            Vector3D intertemp;
            if (!Collision::CalcRaySphere(ray, *prim, &disttemp, &intertemp))continue;
            if (disttemp >= dist)continue;
            result = true;
            dist = disttemp;
            inter = intertemp;
            itr_hit = itr;
        }
        else if (col->GetPrimitive() == PrimitiveType::PLANE)
        {
            Plane* prim = dynamic_cast<Plane*>(col);
            float disttemp;
            Vector3D intertemp;
            if (!Collision::CalcPlaneRay(*prim,ray, &disttemp, &intertemp))continue;
            if (disttemp >= dist)continue;
            result = true;
            dist = disttemp;
            inter = intertemp;
            itr_hit = itr;
        }
        else if (col->GetPrimitive() == PrimitiveType::TRIANGLE)
        {
            Triangle* prim = dynamic_cast<Triangle*>(col);
            float disttemp;
            Vector3D intertemp;
            if (!Collision::CalcTriangleRay(*prim, ray, &disttemp, &intertemp))continue;
            if (disttemp >= dist)continue;
            result = true;
            dist = disttemp;
            inter = intertemp;
            itr_hit = itr;
        }

    }
    if (result && hitinfo)
    {
        hitinfo->dist = dist;
        hitinfo->collPtr = *itr_hit;
        hitinfo->inter = inter;
        hitinfo->objctPtr = hitinfo->collPtr->GetObject3D();
    }
    return result;
}
