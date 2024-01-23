#include"Mass_Spring.h"
#include"Vector3.h"
/*void Sympl(Vector* q, Vector* p)
{
	const double DeltaT = Step_Size;	//Set the time step
	const double Coeff[4] = {1/(4.-pow(2.,4./3.)),
				(1.-pow(2,1./3.))/(4.-pow(2.,4./3.)),
				1./(2.-pow(2.,1./3.)),
				pow(2,1./3.)/(pow(2.,1./3.)-2.)};
	q[0] = X_0;	//Intialize position
	p[0] = V_0;	//Intialize velocity (momentum)

	for(int i = 0; i < Steps; i++)	//Iterate the algorithm
	{
		q[i+1] = q[i] + p[i]*DeltaT*Coeff[0];
		p[i+1] = p[i] + Force(q[i+1])*DeltaT*Coeff[2];
		
		q[i+1] = q[i+1] + p[i+1]*DeltaT*Coeff[1];
		p[i+1] = p[i+1] + Force(q[i+1])*DeltaT*Coeff[3];
		
		q[i+1] = q[i+1] + p[i+1]*DeltaT*Coeff[1];
		p[i+1] = p[i+1] + Force(q[i+1])*DeltaT*Coeff[2];
		
		q[i+1] = q[i+1] + p[i+1]*DeltaT*Coeff[0];
	}

	return;
}*/

void Mass_Spring::Advance0()
{
	vector3	Force = Force_f(0);
	vector3 Acceleration = Force/mass;
	Acceleration += vector3(0,0,-9.80665);	//Add in force of gravity

	position[1] = position[0] + velocity[0]*deltaT*1./(4.-pow(2.,4./3.));
	velocity[1] = velocity[0] + Acceleration*deltaT*1./(2.-pow(2.,1./3.));
	time[1] = time[0] + deltaT*1./(4.-pow(2.,4./3.));
	Normal_Force(1);

	return;
}

void Mass_Spring::Advance1()
{
	vector3	Force = Force_f(1);
	vector3 Acceleration = Force/mass;
	Acceleration += vector3(0,0,-9.80665);	//Add in force of gravity

	position[2] = position[1] + velocity[1]*deltaT*(1.-pow(2.,1./3.))/(4.-pow(2.,4./3.));
	velocity[2] = velocity[1] + Acceleration*deltaT*pow(2.,1./3.)/(pow(2.,1./3.)-2.);
	time[2] = time[1] + deltaT*(1.-pow(2,1./3.))/(4.-pow(2.,4./3.));
	Normal_Force(2);

	return;
}

void Mass_Spring::Advance2()
{
	vector3	Force = Force_f(2);
	vector3 Acceleration = Force/mass;
	Acceleration += vector3(0,0,-9.80665);	//Add in force of gravity

	position[3] = position[2] + velocity[2]*deltaT*(1.-pow(2.,1./3.))/(4.-pow(2.,4./3.));
	velocity[3] = velocity[2] + Acceleration*deltaT*1./(2.-pow(2.,1./3.));
	time[3] = time[2] + deltaT*(1.-pow(2,1./3.))/(4.-pow(2.,4./3.));
	Normal_Force(3);

	return;
}

void Mass_Spring::Update_Prev()
{
	position[4] = position[3] + velocity[3]*deltaT*1./(4.-pow(2.,4./3.));
	time[4] = time[3] + deltaT*1./(4.-pow(2.,4./3.));
	Normal_Force(4);

	position[0] = position[4];
	velocity[0] = velocity[3];
	time[0] = time[4];
	prev_thermal_energy = thermal_energy;

	return;
}

vector3 Mass_Spring::Force_f(int i) const
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
					Force += spring_constant*((Position(i)-Neighbors[i][j][k]->Position(i)).normalize())*((Position(i)-Neighbors[i][j][k]->Position(i)).length()-equilibrium[i][j][k].length());	//I should have a force vector pointing at the mass on the other end of spring whose equilibrium length is set by the initialization. The spring network will mostly hold Mass_Springs in place. When the spring network fails, Mass_Springs may intersect each other.

	Force += Air_Pressure_Grad(i);

	return(Force);
}

void Mass_Spring::Normal_Force(int i)
{
	if(position[i][2] < 0)	//If a mass is found below the ground, elastically bounce it off the ground (I think this will effectively create a normal force, but I may need to reconsider)
	{
		velocity[i][2] *= -1;
		position[i][2] *= -1;
	}
}

vector3 Mass_Spring::Air_Pressure_Grad(int i) const
{
	vector3 Pressure_Gradient(0,0,0);

	Pressure_Gradient[0] += Air_Pressure(position[i]-vector3(deltaX/2.,0,0), time[i])*pow(deltaX,2)/(2.*deltaX);
	Pressure_Gradient[0] -= Air_Pressure(position[i]+vector3(deltaX/2.,0,0), time[i])*pow(deltaX,2)/(2.*deltaX);
	Pressure_Gradient[1] += Air_Pressure(position[i]-vector3(0,deltaX/2.,0), time[i])*pow(deltaX,2)/(2.*deltaX);
	Pressure_Gradient[1] -= Air_Pressure(position[i]+vector3(0,deltaX/2.,0), time[i])*pow(deltaX,2)/(2.*deltaX);
	Pressure_Gradient[2] += Air_Pressure(position[i]-vector3(0,0,deltaX/2.), time[i])*pow(deltaX,2)/(2.*deltaX);
	Pressure_Gradient[2] -= Air_Pressure(position[i]+vector3(0,0,deltaX/2.), time[i])*pow(deltaX,2)/(2.*deltaX);

	return(Pressure_Gradient);
}

double Mass_Spring::Air_Pressure(vector3 location, double time) const
{
	double Equi_Pressure = 288.15-location[2]*.0065>0?101325.*pow((288.15-location[2]*.0065)/288.15,5.2558):0;
	double Shockwave_Pressure = Energy/(2.*M_PI/3.*pow(Shock_Radius(time),3));

	if(true || (location-Center).length() > Shock_Radius(time) || (location-Center).inf_norm() > 1.65)	//Equilibrium pressure before shockwave and outside bunker
		return(Equi_Pressure);
	else if((location-Center).length() < Shock_Radius(time) && (location-Center).inf_norm() > 1.5 && Shock_Radius(time) > 1.65)	//Pressure in the concrete after shockwave has passed
		return(((location-Center).inf_norm()-1.5)*(Shockwave_Pressure-Equi_Pressure)/.15);
	else
		return(((location-Center).inf_norm()-1.5)*(Shockwave_Pressure-Equi_Pressure)/(Shock_Radius(time)-1.5));	//This is probably not quite what I had in mind, but it should be close
}

double Mass_Spring::Hot_Bubble_Radius(double time) const
{
	double R = .0848;
	double deltaR, Temp;

	for(double T = 0; T < time; T += deltaT*100.)
	{
		Temp = sqrt((gamma+1.)/(2.*gamma)*pow(Rf/R,3)+(gamma-1.)/(2.*gamma));
		deltaR = 2.*c_0/(gamma+1.)*(Temp-1./Temp);
		R += deltaR*deltaT*100.;
	}

	return(R);
}

double Mass_Spring::Shock_Radius(double time) const
{
	double HR = Hot_Bubble_Radius(time);
	double T1 = -(gamma-1.)/2.*HR/Rf;
	double T2 = gamma/sqrt(3.)*atan(sqrt(3.)*HR/Rf/(2.+HR/Rf));
	double T3 = gamma/6.*log((1.+HR/Rf+pow(HR/Rf,2))/pow(1.-HR/Rf,2));
	return((T1+T2+T3)*Rf);
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
	pos[0] = position[0][0];
	pos[1] = position[0][1];
	pos[2] = position[0][2];
	return;
}

vector3 Mass_Spring::Position(int i) const
{
	return(position[i]);
}

double Mass_Spring::Distance(Mass_Spring* Other) const
{
	return((position[0]-Other->position[0]).length());
}
