#include<iostream>
#include<fstream>
#include<vector>
#include<queue>
#include<tuple>
#include<random>
#include<algorithm>
#include<ctime>
#include"Vector3.h"
#include"Boid.h"
using namespace std;

void Render(int i);
void Dispatch(vector<Boid>& Flock);

int main()
{
	mt19937_64 RNG;
	uniform_real_distribution<double> Uniform(0,200);
	vector<Boid> Flock;
	ofstream Civilian("Frame/Civilian");
	ofstream Emergency("Frame/EMT");
	vector3 pos, vel;
	int i = 0;

	RNG.seed(time(NULL));

	for(i = 0; i < 100; i++)
	{
		pos = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = (vel-vector3(100.,100.,100.))/10.;
		Flock.push_back(Boid(pos, vel, Civ));
	}
	for(i = 0; i < 6; i++)
	{
		pos = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = vector3(Uniform(RNG),Uniform(RNG),Uniform(RNG));
		vel = (vel-vector3(100.,100.,100.))/10.;
		Flock.push_back(Boid(pos, vel, EMT));
	}

	for(Boid& Self: Flock)
		if(Self.role() == Civ) Self.Render(Civilian);
	for(Boid& Self: Flock)
		if(Self.role() == EMT) Self.Render(Emergency);
	Civilian << endl;
	Emergency << endl;
	Render(0);

	for(i = 1; i <= 30; i++)
	{
		for(Boid& Self: Flock)
			for(Boid Other: Flock)
				Self.Accumalate(Other);

		for(Boid& Self: Flock)
		{
			Self.Update();
			if(Self.role() == Civ) Self.Render(Civilian);
			else if(Self.role() == EMT) Self.Render(Emergency);
		}
		Civilian << endl;
		Emergency << endl;
		Render(i);
	}

	uniform_int_distribution<int> Boid_Choice(0, Flock.size()-1);
	pos = Flock[Boid_Choice(RNG)].Pos();

	for(Boid& Self: Flock)
	{
		Self.Change_State(Roost);
		Self.Take_Damage(pos, 10.);
		if(Self.health() < .75 && Self.role() == EMT)
			Self.Change_Role(Civ);
	}

	for(i; i <= 300; i++)
	{
		Dispatch(Flock);
		for(Boid& Self: Flock)
			for(Boid Other: Flock)
				Self.Accumalate(Other);

		for(Boid& Self: Flock)
		{
			Self.Take_Damage();
			Self.Update();
			if(Self.role() == Civ) Self.Render(Civilian);
			else if(Self.role() == EMT) Self.Render(Emergency);
		}
		Civilian << endl;
		Emergency << endl;
		Render(i);
	}

	return(0);
}

void Render(int i)
{
	cout << "set terminal pngcairo size 1280,960\n";
	cout << "set output './Frame/Frame" << i << ".png'\n";
	cout << "set label 1 \"t = " << i << " s\" at screen 0.5, screen 0.95 center\n";
	cout << "set xrange[0:200];set yrange[0:200];set zrange[0:200]\n";
	cout << "set key off\n";
	//cout << "unset border;unset xtics;unset ytics;unset ztics\n";
	cout << "set multiplot\n";

	cout << "set origin 0,0;set size .5,.5\n";
	cout << "set view 90,360\n";
	//cout << "set lmargin at screen 0; set rmargin at screen .5\n";
	//cout << "set bmargin at screen 0; set tmargin at screen .5\n";
	cout << "splot \"Frame/Civilian\" using 1:2:($3/($4>=.9)) every :::" << i << "::" << i << " title \"Boids\", \"Frame/Civilian\" using 1:2:($3/($4>.5 && $4 <=.9)) every :::" << i << "::" << i << " title \"Amblitory\", \"Frame/Civilian\" using 1:2:($3/($4>0 && $4 <=.5)) every :::" << i << "::" << i << " title \"Critical\", \"Frame/Civilian\" using 1:2:($3/($4==0)) every :::" << i << "::" << i << " title \"Dead\", \"Frame/EMT\" using 1:2:3 every :::" << i << "::" << i << " title \"EMT\"" << endl;

	cout << "set origin 0,.5;set size .5,.5\n";
	cout << "set view 90,90\n";
	//cout << "set lmargin at screen 0; set rmargin at screen .5\n";
	//cout << "set bmargin at screen .5; set tmargin at screen 1\n";
	cout << "splot \"Frame/Civilian\" using 1:2:($3/($4>=.9)) every :::" << i << "::" << i << " title \"Boids\", \"Frame/Civilian\" using 1:2:($3/($4>.5 && $4 <=.9)) every :::" << i << "::" << i << " title \"Amblitory\", \"Frame/Civilian\" using 1:2:($3/($4>0 && $4 <=.5)) every :::" << i << "::" << i << " title \"Critical\", \"Frame/Civilian\" using 1:2:($3/($4==0)) every :::" << i << "::" << i << " title \"Dead\", \"Frame/EMT\" using 1:2:3 every :::" << i << "::" << i << " title \"EMT\"" << endl;

	cout << "set origin .5,0;set size .5,.5\n";
	cout << "set view 0,0\n";
	//cout << "set lmargin at screen .5; set rmargin at screen 1\n";
	//cout << "set bmargin at screen 0; set tmargin at screen .5\n";
	cout << "splot \"Frame/Civilian\" using 1:2:($3/($4>=.9)) every :::" << i << "::" << i << " title \"Boids\", \"Frame/Civilian\" using 1:2:($3/($4>.5 && $4 <=.9)) every :::" << i << "::" << i << " title \"Amblitory\", \"Frame/Civilian\" using 1:2:($3/($4>0 && $4 <=.5)) every :::" << i << "::" << i << " title \"Critical\", \"Frame/Civilian\" using 1:2:($3/($4==0)) every :::" << i << "::" << i << " title \"Dead\", \"Frame/EMT\" using 1:2:3 every :::" << i << "::" << i << " title \"EMT\"" << endl;

	cout << "set origin .5,.5;set size .5,.5\n";
	cout << "set view 45,315\n";
	cout << "set key on\n";
	//cout << "set lmargin at screen .575; set rmargin at screen .925\n";
	//cout << "set bmargin at screen .575; set tmargin at screen .925\n";
	cout << "splot \"Frame/Civilian\" using 1:2:($3/($4>=.9)) every :::" << i << "::" << i << " title \"Boids\", \"Frame/Civilian\" using 1:2:($3/($4>.5 && $4 <=.9)) every :::" << i << "::" << i << " title \"Amblitory\", \"Frame/Civilian\" using 1:2:($3/($4>0 && $4 <=.5)) every :::" << i << "::" << i << " title \"Critical\", \"Frame/Civilian\" using 1:2:($3/($4==0)) every :::" << i << "::" << i << " title \"Dead\", \"Frame/EMT\" using 1:2:3 every :::" << i << "::" << i << " title \"EMT\"" << endl;

	cout << "unset multiplot" << endl;
}

struct comp
{
	bool operator()(const pair<double, Boid*>& lhs, const pair<double, Boid*>& rhs) const
	{
		return(lhs.first > rhs.first);
	}
};

void Dispatch(vector<Boid>& Flock)
{
	priority_queue<pair<double,Boid*>, vector<pair<double,Boid*>>, comp> Triage;
	vector<Boid*> Avalible_EMT;
	static vector<tuple<Boid*,Boid*,Boid*>> Assignments;	//Assignments are recalled frame to frame so that assignments aren't altered once set

	for(int i = Assignments.size()-1; i >= 0; i--)		//Remove completed (successful or failed) Assignments
	{
		if(get<0>(Assignments[i])->Pos().length() < 10 || get<0>(Assignments[i])->health() == 0)
			Assignments.erase(Assignments.begin()+i);
	}

	for(Boid& Self: Flock)	//Triage the flock
		if(0 < Self.health() && Self.health() < .5 && !(Self.state() & (Under_way | In_care | Assign1 | Assign2)))
			Triage.push(pair<double,Boid*>(Self.health(),&Self));

	for(Boid& Self: Flock)	//Find the avalible EMTs
	{
		if(Self.role() == EMT && Self.health() > .75)
		{
			if(!any_of(Assignments.begin(), Assignments.end(),
				[&Self](const tuple<Boid*,Boid*,Boid*>& Assignment)
				{return(get<1>(Assignment) == &Self || get<2>(Assignment) == &Self);}
				))
			{
				Avalible_EMT.push_back(&Self);
				Self.Assign_Destination(vector3(0,0,0));
			}
		}
	}

	if(!Avalible_EMT.empty() && !Assignments.empty() && get<2>(Assignments.back()) == nullptr)	//Complete the incomplete assignment if possible and necessary
	{
		Boid* Patient = get<0>(Assignments.back()); 
		tuple<int, double, Boid*> Min(0, 1000., nullptr);
		for(int i = 0; i < Avalible_EMT.size(); i++)
			if(get<1>(Min) > (Avalible_EMT[i]->Pos()-Patient->Pos()).length())
				Min = tuple<int, double, Boid*>(i, (Avalible_EMT[i]->Pos()-Patient->Pos()).length(), Avalible_EMT[i]);
		get<2>(Min)->Assign_Destination(Patient->Pos());
		Avalible_EMT.erase(Avalible_EMT.begin()+get<0>(Min));
		Patient->Change_State(States(Patient->state()|Assign2));
	}

	while(!Avalible_EMT.empty() && !Triage.empty())	//Assign remaining EMTs to patients
	{
		Boid* Patient = Triage.top().second;
		tuple<int, double, Boid*> Min1(0, 1000., nullptr);
		tuple<int, double, Boid*> Min2(0, 1000., nullptr);
		Triage.pop();

		for(int i = 0; i < Avalible_EMT.size(); i++)
			if(get<1>(Min1) > (Avalible_EMT[i]->Pos()-Patient->Pos()).length())
				Min1 = tuple<int, double, Boid*>(i, (Avalible_EMT[i]->Pos()-Patient->Pos()).length(), Avalible_EMT[i]);
		get<2>(Min1)->Assign_Destination(Patient->Pos());
		Avalible_EMT.erase(Avalible_EMT.begin()+get<0>(Min1));
		Patient->Change_State(States(Patient->state()|Assign1));

		if(!Avalible_EMT.empty())
		{
			for(int i = 0; i < Avalible_EMT.size(); i++)
				if(get<1>(Min2) > (Avalible_EMT[i]->Pos()-Patient->Pos()).length())
					Min2 = tuple<int, double, Boid*>(i, (Avalible_EMT[i]->Pos()-Patient->Pos()).length(), Avalible_EMT[i]);
			get<2>(Min2)->Assign_Destination(Patient->Pos());
			Avalible_EMT.erase(Avalible_EMT.begin()+get<0>(Min2));
			Patient->Change_State(States(Patient->state()|Assign2));
		}

		Assignments.push_back(tuple<Boid*,Boid*,Boid*>(Patient,get<2>(Min1),get<2>(Min2)));
	}

	for(tuple<Boid*,Boid*,Boid*> Assign: Assignments)	//Alter states as EMTs arrive
	{
		if((get<0>(Assign)->Pos()-get<1>(Assign)->Pos()).length() < 5 && (get<0>(Assign)->Pos()-get<2>(Assign)->Pos()).length() < 5)
		{//The second or both EMTs arrive
			if(get<0>(Assign)->state() & In_care)	//Patient is no longer, just in care of an EMT
				get<0>(Assign)->Change_State(States(get<0>(Assign)->state() ^ In_care));
			if(get<1>(Assign)->state() & Render_aid)	//EMTs are no longer rendering aid
				get<1>(Assign)->Change_State(States(get<1>(Assign)->state() ^ Render_aid));
			if(get<2>(Assign)->state() & Render_aid)
				get<2>(Assign)->Change_State(States(get<2>(Assign)->state() ^ Render_aid));

			get<0>(Assign)->Change_State(States(get<0>(Assign)->state() | Under_way));	//EMTs and patient are under way
			get<1>(Assign)->Change_State(States(get<1>(Assign)->state() | Under_way));
			get<2>(Assign)->Change_State(States(get<2>(Assign)->state() | Under_way));

			double Max_v = min(get<1>(Assign)->Get_Max_v(),get<2>(Assign)->Get_Max_v());	//Determine maxium velocity due to EMT health
			get<0>(Assign)->Set_Max_v(Max_v);	//Set it to all members in the ambulance
			get<1>(Assign)->Set_Max_v(Max_v);
			get<2>(Assign)->Set_Max_v(Max_v);

			get<0>(Assign)->Assign_Destination(vector3(0,0,0));	//Every one goes to roost
			get<1>(Assign)->Assign_Destination(vector3(0,0,0));
			get<2>(Assign)->Assign_Destination(vector3(0,0,0));
		}
		else if((get<0>(Assign)->Pos()-get<1>(Assign)->Pos()).length() < 5 || (get<2>(Assign) != nullptr && (get<0>(Assign)->Pos()-get<2>(Assign)->Pos()).length() < 5))	//First EMT arrives
		{
			get<0>(Assign)->Change_State(States(get<0>(Assign)->state() | In_care));
			if((get<0>(Assign)->Pos()-get<1>(Assign)->Pos()).length() < 5)
				get<1>(Assign)->Change_State(States(get<1>(Assign)->state() | Render_aid));
			else
				get<2>(Assign)->Change_State(States(get<2>(Assign)->state() | Render_aid));
		}
	}

	if(Triage.empty() && Assignments.empty())	//All patients are rescued or dead. Can't early terminate or EMTs won't lock in an arrival
		for(Boid& Self: Flock)
			if(Self.role() == EMT)
				Self.Assign_Destination(vector3(0,0,0));
}
