#include "Human.h"

void MCB::Human::Init(Model* model)
{
	Box[Root].Init();
	Box[Root].model = model;
	Box[Root].scale = { 4,4,4 };
	//ê“í≈
	Box[Spine].Init();
	Box[Spine].model = model;
	Box[Spine].parent = &Box[Root];
	Box[Spine].position = { 0,4.5f,0 };
	//è„îºêg
	Box[Chest].Init();
	Box[Chest].model = model;
	Box[Chest].parent = &Box[Spine];
	Box[Chest].position = { 0,-2.5f,0 };

	Box[Head].Init();
	Box[Head].model = model;
	Box[Head].parent = &Box[Chest];
	Box[Head].position = { 0,4.5f,0 };

	Box[ArmL].Init();
	Box[ArmL].model = model;
	Box[ArmL].parent = &Box[Chest];
	Box[ArmL].position = {-4.5f,0.0f,0 };

	Box[ArmR].Init();
	Box[ArmR].model = model;
	Box[ArmR].parent = &Box[Chest];
	Box[ArmR].position = { 4.5f,0.0f,0 };

	//â∫îºêg
	Box[Hip].Init();
	Box[Hip].model = model;
	Box[Hip].parent = &Box[Spine];
	Box[Hip].position = { 0,-7.0f,0 };

	Box[LegL].Init();
	Box[LegL].model = model;
	Box[LegL].parent = &Box[Hip];
	Box[LegL].position = { -4.5,-4.5f,0 };

	Box[LegR].Init();
	Box[LegR].model = model;
	Box[LegR].parent = &Box[Hip];
	Box[LegR].position = { 4.5,-4.5f,0 };
}

void MCB::Human::UpDate(View view, Projection proj)
{
	for (int i = 0; i < Box.size(); i++)
	{
		Box[i].Updata(view, proj);
	}
}

void MCB::Human::Draw()
{
	for (int i = 0; i < Box.size(); i++)
	{
		if (i < 2)continue;
		Box[i].Draw();
	}
}
