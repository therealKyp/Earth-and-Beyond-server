// SpaceObject.h
#ifndef _SPACE_OBJECT_H_INCLUDED_
#define _SPACE_OBJECT_H_INCLUDED_

class SpaceObject
{
public:
    SpaceObject();
    virtual ~SpaceObject();

public:
	int m_GameID; // Object's GameID
	int m_Location; // Object's current location (SectorID)
	
	float m_Scale;
	float m_Signature;
	short m_Asset;
	
	bool m_IsHidden;
	bool m_IsHuge;
	
	float m_Position[3]; // Position
	float m_Orientation[4]; // Orientation
	
	char *m_Name;
};

#endif // _SPACE_OBJECT_H_INCLUDED_
