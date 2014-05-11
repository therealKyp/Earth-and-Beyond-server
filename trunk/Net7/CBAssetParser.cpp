//CBAssetParser.cpp
//XML version

#include "CBAssetParser.h"
#include "xmlParser/xmlParser.h"

#include <sstream>
using namespace std;

CBAssetParser::CBAssetParser()
{
	numBases = 0;
	radii    = (0);
}

CBAssetParser::~CBAssetParser()
{
	if(radii != (0))
	{
		delete [] radii;
		radii = (0);
	}
}

bool CBAssetParser::ParseRadii()
{
	if(radii != (0))
	{
		delete [] radii;
		radii = (0);
	}
	
	LogMessage("Parsing \'cbasset.xml\'...");
	XMLNode xMainNode = XMLNode::openFileHelper("C:\\Net7\\database\\cbasset.xml");

	numBases = xMainNode.getChildNode("Bases").nChildNode("Base");
	
	radii = new float[numBases];
	for(unsigned int currentBase = 0; currentBase < numBases; currentBase++)
	{
		stringstream s;		//We should avoid using strings & streams but providing they are not used in the main app it's ok.
							//essentially these templates are undebuggable, but if the data is ok at the end they should be alright.
							//note for future - NEVER use strings within the main loop where memory can be allocated/deallocated.
		s << currentBase;
		radii[currentBase] = (float)atof( xMainNode.getChildNode("Bases").getChildNodeWithAttribute("Base", "ID", s.str().c_str()).getChildNode("Radius").getAttribute("Value") );
		//float adjust = float)atof( xMainNode.getChildNode("Bases").getChildNodeWithAttribute("Base", "ID", s.str().c_str()).getChildNode("Radius").getAttribute("Value") );
	}

	printf(".\n");
    
    return true;
}





float CBAssetParser::GetRadius(unsigned int baseID)
{
    if(radii == NULL)
    {
        //Error, Radius data hasn't been parsed yet
        LogMessage("Error - Cannot read Radius values, data has not been parsed yet!!!\n");
        return -1.0f;
    }
    if(baseID >= numBases)
    {
        //Error, out of bounds of the 'radii' array
        LogMessage("Error - Specified BASE id does not exist!!!\n");
        return -1.0f;
    }
    
    return radii[baseID];
}



unsigned int CBAssetParser::GetNumBases()
{
	return numBases;
}
