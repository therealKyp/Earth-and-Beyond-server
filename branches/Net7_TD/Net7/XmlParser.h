// XmlParser.h
//
//      This class implements pretty simplistic XML parser routines.
//

#ifndef _XML_PARSER_H_INCLUDED_
#define _XML_PARSER_H_INCLUDED_

#include "Net7.h"
#include "XmlTags.h"


class XmlParser
{
public:
    // forward reference
    struct XmlTagLookupTable;

// Constructor/Destructur
public:
    XmlParser();
    ~XmlParser();

// Data Structures
public:
    struct XmlTagLookupTable
    {
        enum eTagId tag_id;
        char *tag;
    };
    typedef struct XmlTagLookupTable XmlTagLookupTable;

// Public Methods
public:
    bool    ParseXmlTag(char **buffer);
    bool    GetXmlAttribute(char *attrib, char *value, unsigned int length, bool required = false);
    bool    GetXmlData(char **buffer, char *data, long length);
    int     ParseInt(char **buffer, int id, int min, int max);
    double  ParseDouble(char **buffer, int id, double min, double max, bool allow_minus_one = false);
    u32_t   ParseColor(char **buffer, int id);
    bool    ParseFloatArray(char *attrib_values, int count, float *array);
    bool    CheckID(int id);

    // NOTE: The caller must delete the returned string!!
    char  * ParseString(char **buffer, int id);

    // These are a static member function so that they can be
    // called without instantiating an XmlParser object.
    // NOTE: The caller must delete the returned string!!
    static char * EncodeXmlString(char * str);
    static char * DecodeXmlString(char * str);

// Public Member Attributes
public:
    bool    m_Success;
    eTagId  m_XmlTagID;

// Protected Member Attributes
protected:
    int     m_CurrentID;
    char    m_Tag[512];
    char    m_Attributes[512];
    XmlTagLookupTable * m_XmlTagLookupTable;
};

#endif // _XML_PARSER_H_INCLUDED_
