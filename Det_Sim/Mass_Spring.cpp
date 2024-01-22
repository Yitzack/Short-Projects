#include"Mass_Spring.h"
#include"Vector3.h"

void Mass_Spring::Advance()
{
	const vector3 equilibrium[3][3][3] = 
		{{{vector3(-.01,-.01,-.01), vector3(-.01,-.01,0), vector3(-.01,-.01,.01)}, {vector3(-.01,0,-.01), vector3(-.01,0,0), vector3(-.01,0,.01)}, {vector3(-.01,.01,-.01), vector3(-.01,.01,0), vector3(-.01,.01,.01)}}, 
		{{vector3(0,-.01,-.01), vector3(0,-.01,0), vector3(0,-.01,.01)}, {vector3(0,0,-.01), vector3(0,0,0), vector3(0,0,.01)}, {vector3(0,.01,-.01), vector3(0,.01,0), vector3(0,.01,.01)}}, 
		{{vector3(.01,-.01,-.01), vector3(.01,-.01,0), vector3(.01,-.01,.01)}, {vector3(.01,0,-.01), vector3(.01,0,0), vector3(.01,0,.01)}, {vector3(.01,.01,-.01), vector3(.01,.01,0), vector3(.01,.01,.01)}}};
	vector3	Force(0,0,0);
	vector3 Acceleration;

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			for(int k = 0; k < 3; k++)
				if(Neighbors[i][j][k] != nullptr && (i != 1 && j != 1 && k!= 1))
					Force += spring_constant*((Position()-Neighbors[i][j][k]->Position()).normalize())*((Position()-Neighbors[i][j][k]->Position()).length()-equilibrium[i][j][k].length());	//I should have a force vector pointing at the mass on the other end of spring whose equilibrium length is set by the initialization. The spring network will mostly hold Mass_Springs in place. When the spring network fails, Mass_Springs may intersect each other.

	Force += Air_Pressure();

	Acceleration = Force/mass;
	Acceleration += vector3(0,0,-9.80665);	//Add in force of gravity
	velocity = prev_velocity + Acceleration*deltaT;
	position = prev_position + prev_velocity*deltaT + Acceleration*pow(deltaT,2)/2.;
	time += deltaT;

	if(position[2] < 0)	//If a mass is found below the ground, elastically bounce it off the ground (I think this will effectively create a normal force, but I may need to reconsider)
	{
		velocity[2] *= -1;
		position[2] *= -1;
	}

	return;
}

vector3 Mass_Spring::Air_Pressure() const
{
	vector3 Pressure_Gradient(0,0,0);
	double Equi_Pressure = 101325.*pow((288.15-prev_position[2]*.0065)/288.15,5.2558);
	double Shockwave_Pressure = Energy/(2.*M_PI/3.*pow(Shock_Radius(),3));
	double Pressure;

	if(true || (prev_position-Center).length() > Shock_Radius() || (prev_position-Center).inf_norm() > 1.65)	//Equilibrium pressure before shockwave and outside bunker
		Pressure = Equi_Pressure;
	else if((prev_position-Center).length() < Shock_Radius() && (prev_position-Center).inf_norm() > 1.5 && Shock_Radius() > 1.65)	//Pressure in the concrete after shockwave has passed all
		Pressure = ((prev_position-Center).inf_norm()-1.5)*(Shockwave_Pressure-Equi_Pressure)/.15;
	else
		Pressure = ((prev_position-Center).inf_norm()-1.5)*(Shockwave_Pressure-Equi_Pressure)/(Shock_Radius()-1.5);	//This is not quite what I had in mind, but it should be close

	if(Neighbors[0][1][1] == nullptr)
		Pressure_Gradient[0] += Pressure*.0001;
	if(Neighbors[2][1][1] == nullptr)
		Pressure_Gradient[0] -= Pressure*.0001;
	if(Neighbors[1][0][1] == nullptr)
		Pressure_Gradient[1] += Pressure*.0001;
	if(Neighbors[1][2][1] == nullptr)
		Pressure_Gradient[1] -= Pressure*.0001;
	if(Neighbors[1][1][0] == nullptr && prev_position[2] > deltaX)
		Pressure_Gradient[2] += Pressure*.0001;
	if(Neighbors[1][1][2] == nullptr)
		Pressure_Gradient[2] -= Pressure*.0001;

	return(Pressure_Gradient);
}

double Mass_Spring::Hot_Bubble_Radius() const
{
	double R = .0848;
	double deltaR;

	for(double T = 0; T <= prev_time; T += deltaT/10.)
	{
		deltaR = 2.*c_0/(gamma+1.)*(sqrt((gamma+1.)/(2.*gamma)*pow(Rf/R,3)+(gamma-1.)/(2.*gamma))-1./sqrt((gamma+1.)/(2.*gamma)*pow(Rf/R,3)+(gamma-1.)/(2.*gamma)));
		R += deltaR*deltaT;
	}

	return(R);
}

double Mass_Spring::Shock_Radius() const
{
	double HR = Hot_Bubble_Radius();
	return((gamma/6.*log((1.+HR/Rf+pow(HR/Rf,2))/pow(1.+HR/Rf,2))+gamma/sqrt(3.)*atan(sqrt(3.)*HR/Rf/(2.+HR/Rf))-(gamma-1.)/2.*HR/Rf)*Rf);
}

bool Mass_Spring::Is_Surface() const
{
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			for(int k = 0; k < 3; k++)
				if(Neighbors[i][j][k] == nullptr)
					return(true);
	return(false);
}

void Mass_Spring::Store_Neighbor(Mass_Spring* Neighbor, int i, int j, int k)
{
	Neighbors[i][j][k] = Neighbor;
	return;
}

void Mass_Spring::Get_Position(double pos[3])
{
	pos[0] = prev_position[0];
	pos[1] = prev_position[1];
	pos[2] = prev_position[2];
	return;
}

vector3 Mass_Spring::Position() const
{
	return(prev_position);
}

double Mass_Spring::Distance(Mass_Spring* Other) const
{
	return((prev_position-Other->prev_position).length());
}
