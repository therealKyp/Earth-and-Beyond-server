//CBAssetParser.h
//XML version

#ifndef CBAssetParser_H
#define CBAssetParser_H

#include "Net7.h"
#include "xmlParser/xmlParser.h"

class CBAssetParser
{
    public:
		CBAssetParser();
		virtual ~CBAssetParser();
        bool ParseRadii();
        
        float GetRadius(unsigned int baseID);
        
        unsigned int GetNumBases();
    private:
        unsigned int numBases;
        float *radii;
};

#endif
