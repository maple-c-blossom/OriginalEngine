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

    //std::forward_list<BaseCollider*>::iterator itrA;
    //std::forward_list<BaseCollider*>::iterator itrB;

    //*itrA = colliders_.begin()->get();
    //BaseCollider* itr_hit = nullptr;
    for (auto& itr : colliders_)
    {

        for (auto& itr2 : colliders_)
        {
            BaseCollider* itrCollA = itr.get();
            BaseCollider* itrCollB = itr2.get();
            if (itrCollA == itrCollB)continue;
            if (itrCollA->attribute_ == ATTRIBUTE_LANDSHAPE || itrCollB->attribute_ == ATTRIBUTE_LANDSHAPE) continue;
            if (itrCollA->GetPrimitive() == PrimitiveType::RAY || itrCollB->GetPrimitive() == PrimitiveType::RAY)
            {
                if (itrCollA->GetPrimitive() == PrimitiveType::RAY)
                {
                    Ray* rayA = dynamic_cast<Ray*>(itrCollA);
                    if (rayA->rayCasted_)
                    {
                        continue;
                    }
                }
                else
                {
                    Ray* rayA = dynamic_cast<Ray*>(itrCollB);
                    if (rayA->rayCasted_)
                    {
                        continue;
                    }
                }
               
            }
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
                    if (!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
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
                    if (!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
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
                    if (!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
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
                    if (!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
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
                    if (!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
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
                    if (!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
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
                    if (!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
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
                   if(!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                   if(!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
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
                    if (!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
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
                    if (!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
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
                    if (!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }
            else if (itrCollA->GetPrimitive() == PrimitiveType::SPHERE && itrCollB->GetPrimitive() == PrimitiveType::MESH)
            {
                Sphere* sphereA = dynamic_cast<Sphere*>(itrCollA);
                MeshCollider* meshB = dynamic_cast<MeshCollider*>(itrCollB);
                Vector3D inter;
                //if (Collision::CalcSphere(meshB->sphere, *sphereA, &inter)) //‹…“¯Žm‚Ì“–‚½‚è”»’è‚ÅŒy‚­”»’è(radius1.5”{)
                //{
                    if (meshB->ChakeCollisionSphere(*sphereA, &inter))//–{”Ô
                    {
                        itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                        itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                    }
                //}
                else {
                    if (!itrCollA->GetObject3D()->hited_)itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            }

            else if (itrCollA->GetPrimitive() == PrimitiveType::MESH && itrCollB->GetPrimitive() == PrimitiveType::SPHERE)
            {
            MeshCollider* meshA = dynamic_cast<MeshCollider*>(itrCollA);
            Sphere* sphereB = dynamic_cast<Sphere*>(itrCollB);
            Vector3D inter;
            //if (Collision::CalcSphere(meshA->sphere, *sphereB, &inter)) //‹…“¯Žm‚Ì“–‚½‚è”»’è‚ÅŒy‚­”»’è(radius1.5”{)
            //{
                if (meshA->ChakeCollisionSphere(*sphereB, &inter))//–{”Ô
                {
                    itrCollA->OnCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                    itrCollB->OnCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
                }
            //}
            else {
                if (!itrCollA->GetObject3D()->hited_) itrCollA->OffCollision(CollisionInfomation(itrCollB->GetObject3D(), itrCollB, inter));
                if (!itrCollB->GetObject3D()->hited_) itrCollB->OffCollision(CollisionInfomation(itrCollA->GetObject3D(), itrCollA, inter));
            }
            }

        }
    }
}

bool MCB::CollisionManager::Raycast( Ray& ray, RayCastHit* hitinfo, float maxDistance)
{
    return Raycast(ray,0xffff,hitinfo,maxDistance);
}

bool MCB::CollisionManager::Raycast(Ray& ray, uint16_t attribute, RayCastHit* hitinfo, float maxDistance)
{
    const Ray& raytemp = ray;
    bool result = false;
    //std::forward_list<BaseCollider*>::iterator itr;
    //std::forward_list<BaseCollider*>::iterator itr_hit;
    float dist = maxDistance;
    Vector3D inter;
    //*itr = colliders_.begin()->get();
    BaseCollider* itr_hit = nullptr;
    for (auto& itr:colliders_)
    {
        OutputDebugStringW(L"RayCastForIn\n");
        BaseCollider* col = itr.get();
        if (!(col->attribute_ & attribute))continue;
        if (col->GetPrimitive() == PrimitiveType::SPHERE)
        {
            OutputDebugStringW(L"SPHERE\n");
            Sphere* prim = dynamic_cast<Sphere*>(col);
            float disttemp;
            Vector3D intertemp;
            if (!Collision::CalcRaySphere(raytemp, *prim, &disttemp, &intertemp))continue;
            if (disttemp >= dist)continue;
            result = true;
            dist = disttemp;
            inter = intertemp;
            itr_hit = itr.get();
        }
        else if (col->GetPrimitive() == PrimitiveType::PLANE)
        {
            OutputDebugStringW(L"PLANE\n");
            Plane* prim = dynamic_cast<Plane*>(col);
            float disttemp;
            Vector3D intertemp;
            if (!Collision::CalcPlaneRay(*prim, raytemp, &disttemp, &intertemp))continue;
            if (disttemp >= dist)continue;
            result = true;
            dist = disttemp;
            inter = intertemp;
            itr_hit = itr.get();
        }
        else if (col->GetPrimitive() == PrimitiveType::TRIANGLE)
        {
            OutputDebugStringW(L"TRIANGLE\n");
            Triangle* prim = dynamic_cast<Triangle*>(col);
            float disttemp;
            Vector3D intertemp;
            if (!Collision::CalcTriangleRay(*prim, raytemp, &disttemp, &intertemp))continue;
            if (disttemp >= dist)continue;
            result = true;
            dist = disttemp;
            inter = intertemp;
            itr_hit = itr.get();
        }
        else if (col->GetPrimitive() == PrimitiveType::MESH)
        {
            OutputDebugStringW(L"Mesh\n");
            MeshCollider* prim = dynamic_cast<MeshCollider*>(col);
            float disttemp;
            Vector3D intertemp;
            //if (!Collision::CalcRaySphere(raytemp, prim->sphere, &disttemp, &intertemp))continue;//‚Ü‚¸‚Í‹…‚Å”»’è
            if (!prim->ChakeCollisionRay(raytemp, &disttemp, &intertemp))
            {
                continue;
            }
            if (disttemp >= dist)continue;
            result = true;
            dist = disttemp;
            inter = intertemp;
            itr_hit = itr.get();
        }

    }
    if (result && hitinfo)
    {
        hitinfo->dist_ = dist;
        hitinfo->collPtr_ = itr_hit;
        hitinfo->inter_ = inter;
        hitinfo->objctPtr_ = hitinfo->collPtr_->GetObject3D();
    }
    ray.rayCasted_ = true;
    return result;
}

void MCB::CollisionManager::QuerySphere(const Sphere& sphere, QueryCallBack* callBack, uint16_t attribute)
{
    assert(callBack && "QuerySphere:callBack NullptrExeption");


   
    for ( auto& itr: colliders_)
    {
        BaseCollider* col = itr.get();
        if ((col->attribute_ & attribute))
        {
            if (col->GetPrimitive() == PrimitiveType::SPHERE)
            {
                Sphere* sphere2 = dynamic_cast<Sphere*>(col);
                Vector3D tempInter;
                Vector3D tempReject;
                if (!Collision::CalcSphere(sphere, *sphere2, &tempInter, &tempReject)) continue;
                
                QueryHit info;
                info.baseColl = col;
                info.obj = col->object3d_;
                info.inter = tempInter;
                info.reject = tempReject;

                if (!callBack->OnQueryHit(info))
                {
                    return;
                }
            }

            if (col->GetPrimitive() == PrimitiveType::MESH)
            {
                MeshCollider* mesh = dynamic_cast<MeshCollider*>(col);
                Vector3D tempInter;
                Vector3D tempReject;
                if (!mesh->ChakeCollisionSphere(sphere,&tempInter, &tempReject)) continue;
                QueryHit info;
                info.baseColl = col;
                info.obj = col->object3d_;
                info.inter = tempInter;
                info.reject = tempReject;
                if (!callBack->OnQueryHit(info))
                {
                    return;
                }

            }
        }
    }
}
