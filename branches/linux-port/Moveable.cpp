// Moveable.cpp

#include "Net7.h"
#include "ObjectClass.h"
#include "Moveable.h"


int signof(int a) { return (a == 0) ? 0 : (a<0 ? -1 : 1); }

Moveable::Moveable()
{
	memset(&m_Position_info, 0, sizeof(m_Position_info));		// Zero out memory
	m_IsMoveing = false;
	m_Mass = 300;
	m_MaxTurnRate = 1.04719758f;
	m_MaxTiltRate = 1.04719758f;
	m_MaxSpeed = 25600;
	m_MinSpeed = -12800;
	m_CurrentSpeed = 0;
	m_MoveType = 4;		// Stoped
	m_MaxAcceleration = 162.5f;
	m_LastUpdate = GetNet7TickCount();
	m_UpdateReate = 500;
	m_WarpSpeed = 0;
	m_MovementID = 0;
	m_MovementState = 0;

	m_Heading  = Vector(0,0,0);
	m_Velocity = Vector(0,0,0);

	/* Should not be changed */
	m_id   = Quat(0, 0, 0, 1);
	m_canZ = Quat(0, 0, 1, 1);
	m_canY = Quat(0, -1,0, 1);
	m_canX = Quat(1, 0, 0, 1);
	/* --------------------- */
	E_X    = Vector(1, 0, 0);


}

Moveable::~Moveable()
{

}

void Moveable::Tilt(float Intensity)
{
	LogMessage("[Moveable] Tilt: %f\n", Intensity);

	if (Intensity > 1.0f) Intensity = 1.0f;
	if (Intensity < -1.0f) Intensity = 1.0f;

	m_MovementState |= 0x01;
	m_Tilt_Intensity = Intensity;
	m_MovementID++;
	m_IsMoveing = true;
	// Add to Moveing thread
	UpdateLocation();
}

void Moveable::Turn(float Intensity)
{
	LogMessage("[Moveable] Turn: %f\n", Intensity);

	if (Intensity > 1.0f) Intensity = 1.0f;
	if (Intensity < -1.0f) Intensity = 1.0f;

	m_MovementState |= 0x01;
	m_Turn_Intensity = Intensity;
	m_MovementID++;
	m_IsMoveing = true;
	// Add to Moveing thread
	UpdateLocation();
}

void Moveable::Move(int MoveType)
{
	LogMessage("[Moveable] Move: %d\n", MoveType);

	switch(MoveType)
	{
		case 0:
			m_MovementState |= 0x02;
			m_CurrentAcceleration = m_MaxAcceleration;
			// TODO: Set engins on
			break;
		case 1:
			m_MovementState |= 0x02;
			m_CurrentAcceleration = -m_MaxAcceleration;
			// TODO: Set engins on
			break;
		case 2:
			m_MovementState |= 0x02;
			m_CurrentAcceleration = m_MaxAcceleration;
			// TODO: Set engins on
			break;
		case 3:
			m_MovementState |= 0x02;
			m_CurrentAcceleration = -m_MaxAcceleration;
			// TODO: Set engins on
			break;
		case 4:
			m_MovementState &= 0xFD;;
			m_CurrentAcceleration = signof(-m_CurrentSpeed) * m_MaxAcceleration;
			// TODO: Set engins on
			break;
		case 5:
			 //m_MovementState = 0x08;
			 m_SetSpeed = 3;
			break;
		case 6:
			 //m_MovementState = 0x10;
			 m_SetSpeed = 0;
			break;
	}
	m_MovementID++;
	m_IsMoveing = true;
	UpdateLocation();
	// Add to Moveing thread
}

bool Moveable::IsMoveing()
{
	return m_IsMoveing;
}

void Moveable::UpdateLocation()
{
	int TimeDiffer = GetNet7TickCount() - m_LastUpdate;
	m_LastUpdate = GetNet7TickCount();
	m_Position_info.Bitmask = 0x00;

	LogMessage("[Moveable] TimeDiffer: %d CurrentAcc: %d\n", TimeDiffer, m_CurrentAcceleration);

	if(((m_MovementState >> 1) & 0x01) == 1)
	{
		LogMessage("[Moveable] Update: CurSpeed: %d\n", m_CurrentSpeed);

		m_Position_info.Bitmask |= 0x03;
		m_CurrentSpeed += m_CurrentAcceleration * TimeDiffer;

		if (m_CurrentSpeed > m_MaxSpeed)				// Stop Accelorating
		{
			m_CurrentSpeed = m_MaxSpeed;
			m_CurrentAcceleration = 0;
		} else if (m_CurrentSpeed < m_MinSpeed) {		// Stop Accelorating
			m_CurrentSpeed = m_MinSpeed;
			m_CurrentAcceleration = 0;
		} else {										// Accelorate
			m_Position_info.Bitmask |= 0x04;
		}

	} else if(((m_MovementState >> 1) & 0x01) == 0x00 && m_CurrentSpeed !=0) {			// if we are moveing
		m_Position_info.Bitmask |= 0x07;

		if (signof(m_CurrentSpeed) != signof(m_CurrentSpeed += m_CurrentAcceleration * TimeDiffer))
		{ 
			m_CurrentSpeed = 0;
			m_CurrentAcceleration = 0;
			m_IsMoveing = false;
		}
	}

	if(((m_MovementState) & 0x01) == 1) //If we are turning the ship 
	{ 
		m_Position_info.RotZ = m_Turn_Intensity * m_MaxTurnRate; 
		m_Position_info.RotY = m_Tilt_Intensity * m_MaxTiltRate;

		LogMessage("[Moveable] UpdateTurn: CurSpeed: %d\n", m_CurrentSpeed);


		Vector cross = m_Heading.cross(Vector(0,0,1));
		LogMessage("[Moveable] UpdateTurn: Cross(%f,%f,%f)\n", cross.x, cross.y, cross.z);

		m_Heading = ( cross * (m_Position_info.RotZ * TimeDiffer) ) + m_Heading;
		m_Heading.normalize();
		LogMessage("[Moveable] UpdateTurn: After First Cross (%f,%f,%f)\n", m_Heading.x, m_Heading.y, m_Heading.z);

		cross = cross.cross(m_Heading);
		m_Heading = ( cross * (m_Position_info.RotY * TimeDiffer * 0.9f) ) + m_Heading;
		LogMessage("[Moveable] UpdateTurn: After 2nd Cross (%f,%f,%f)\n", m_Heading.x, m_Heading.y, m_Heading.z);

		if (m_Heading.z > 0.94158f)
			m_Heading.z = 0.94158f;
		else if (m_Heading.z < - 0.94158f)
			m_Heading.z = - 0.94158f;

		m_Heading.normalize();

		m_Position_info.Orientation[0] = m_Heading.x;
		m_Position_info.Orientation[1] = m_Heading.y;
		m_Position_info.Orientation[2] = m_Heading.z;

		m_Position_info.Bitmask |= 0x29;
	} 



	if(((m_MovementState >> 3) & 0x01) == 1) //Warping 
	{ 
		m_Position_info.Bitmask |= 0x01; 
		m_CurrentSpeed = m_WarpSpeed; 
	} 

	if(((m_MovementState >> 4) & 0x01) == 1) //WarpStop 
	{ 
		m_Position_info.Bitmask |= 0x01; 
		m_CurrentSpeed = 0;
	} 
          
	if(m_MovementState != 0x00 || m_CurrentSpeed != 0) 
	{
		Vector Position(m_Position_info.Position[0], m_Position_info.Position[1], m_Position_info.Position[2]);

		m_Velocity = m_Heading;
		m_Velocity.normalize();
		m_Velocity *= (m_CurrentSpeed * TimeDiffer);
		//Position += m_Velocity;

		m_Position_info.Position[0] = Position.x;
		m_Position_info.Position[1] = Position.y;
		m_Position_info.Position[2] = Position.z;
	}

	if (m_CurrentSpeed == 0) 
	{
		m_IsMoveing = false;
	} else {
		LogMessage("[Moveable] CurrentSpeed: %d Current Acc: %d Pos: [%f,%f,%f] Orientation: [%f,%f,%f,%f]\n", m_CurrentSpeed, m_CurrentAcceleration, m_Position_info.Position[0], m_Position_info.Position[1], m_Position_info.Position[2],
			m_Position_info.Orientation[0],m_Position_info.Orientation[1],m_Position_info.Orientation[2],m_Position_info.Orientation[3]);
	}


}

// Basic Writing of Pos and Orientation
Quat *OrientationFromVector(Vector v)
{

	/*
	xyProjection.set(v.x, v.y, 0);

	float angle = E_X.angle(v);
	float phi = E_X.angle(xyProjection) * Math.signum(this.xyProjection.y);
	float theta= -v.angle(this.xyProjection) * Math.signum(v.z);

	Quat q10(0f, (float)Math.sin(theta/2), 0f, (float)Math.cos(theta/2));
	Quat q11(0f, 0f, (float)Math.sin(phi/2), (float)Math.cos(phi/2));
	Orientation o = new Orientation(0, 0, 0, 1);
	o.rotate(q11);
	o.rotate(q10);
	this.set(o);
	*/
	return 0;
}


void Moveable::SetPosition(float x, float y, float z)
{
    m_Position_info.Position[0] = x;
    m_Position_info.Position[1] = y;
    m_Position_info.Position[2] = z;
}

void Moveable::SetPosition(float *pos)
{
    m_Position_info.Position[0] = pos[0];
    m_Position_info.Position[1] = pos[1];
    m_Position_info.Position[2] = pos[2];
}

void Moveable::SetOrientation(float o1, float o2, float o3, float o4)
{
    m_Position_info.Orientation[0] = o1;
    m_Position_info.Orientation[1] = o2;
    m_Position_info.Orientation[2] = o3;
    m_Position_info.Orientation[3] = o4;
}

void Moveable::SetOrientation(float *ori)
{
    m_Position_info.Orientation[0] = ori[0];
    m_Position_info.Orientation[1] = ori[1];
    m_Position_info.Orientation[2] = ori[2];
    m_Position_info.Orientation[3] = ori[3];
}


void Moveable::SetSpeed(int Speed)
{
	m_SetSpeed = Speed;
}

void Moveable::SetCurrentSpeed(int Speed)
{
	m_CurrentSpeed = Speed;
}

void Moveable::SetMaxSpeed(int Speed)
{
	m_MaxSpeed = Speed;
}

void Moveable::SetMinSpeed(int Speed)
{
	m_MinSpeed = Speed;
}

void Moveable::SetMaxTurn(float Turn)
{
	m_MaxTurnRate = Turn;
}

void Moveable::SetMaxTilt(float Tilt)
{
	m_MaxTiltRate = Tilt;
}

void Moveable::SetAcceleration(float Acc)
{
	m_MaxAcceleration = Acc;
}

void Moveable::SetMass(float Mass)
{
	m_Mass = Mass;
}

float * Moveable::GetPosition()
{
	return m_Position_info.Position;
}

float * Moveable::GetOrientation()
{
	return m_Position_info.Orientation;
}
