// Moveable.h

#ifndef _MOVEABLE_CLASS_H_INCLUDED_
#define _MOVEABLE_CLASS_H_INCLUDED_

#include "SectorData.h"
#include "ObjectClass.h"
#include "VectorMath.h"

class Moveable
{
public:
    Moveable();
    virtual ~Moveable();

	void	Turn(float Intensity);
	void	Tilt(float Intensity);
	void	Move(int Type);				
	void	UpdateLocation();			// Updates location of object

	void	SetSpeed(int Speed);
	void	SetCurrentSpeed(int Speed);
	void	SetMaxSpeed(int Speed);
	void	SetMinSpeed(int Speed);
	void	SetMaxTurn(float Tilt);
	void	SetMaxTilt(float Tilt);

	void	SetAcceleration(float Acc);
	void	SetMass(float Mass);

	float * GetPosition();
	float * GetOrientation();

	Quat	*OrientationFromVector(Vector v);

	bool	IsMoveing();

	void	SetPosition(float x, float y, float z);
	void	SetPosition(float *pos);
	void	SetOrientation(float o1, float o2, float o3, float o4);
	void	SetOrientation(float *ori);

private:

	/* Should not be changed */
	Quat m_id;		//(0f, 0f, 0f, 1f);
	Quat m_canZ;		//(0f, 0f, 1f, 1f);
	Quat m_canY;		//(0f, -1f, 0f, 1f);
	Quat m_canX;		//(1f, 0f, 0f, 1f);
	/* --------------------- */
	Vector E_X;		//(1f, 0f, 0f);

	float	m_MaxTurnRate;
	float	m_MaxTiltRate;
	int		m_MaxSpeed;
	int		m_MinSpeed;
	float	m_Mass;


	float	m_MaxAcceleration;
	int		m_CurrentAcceleration;
	int		m_WarpSpeed;
	bool	m_IsMoveing;

	float	m_Turn_Intensity;
	float	m_Tilt_Intensity;
	int		m_CurrentSpeed;
	int		m_SetSpeed;
	int		m_MoveType;
	int		m_LastUpdate;
	int		m_UpdateReate;
	int		m_MovementID;
	int		m_MovementState;

	Vector	m_Velocity;
	Vector	m_Heading;

	PositionInformation m_Position_info;
};

#endif // _MOVEABLE_CLASS_H_INCLUDED_