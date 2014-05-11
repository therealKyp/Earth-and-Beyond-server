// XmlParser.cpp
//
//      This class implements pretty simplistic XML parser routines.
//

#include "Net7.h"
#include "XmlParser.h"
#include "XmlTags.h"


XmlParser::XmlParser()
{
    m_Success = true;
}

XmlParser::~XmlParser()
{
}

bool XmlParser::ParseXmlTag(char **buffer)
{
    char *tag = m_Tag;
    char *attributes = m_Attributes;
    bool success = false;
    char *p = *buffer;
    *tag = '\0';
    *attributes = '\0';

    m_XmlTagID = XML_TAG_ID_INVALID;

    // Skip data until we reach the beginning of the tag or the end of the string
    bool done = false;
    while (!done)
    {
        // Skip forward until we find the '<' or reach the end of the buffer
        while ((*p != '\0') && (*p != '<'))
        {
            p++;
        }

        // Bail out if we have reached the end of the buffer
        if (*p == '\0')
        {
            return (false);
        }

        // Skip the '<'
        p++;

        // If this is a valid tag, stop the search
        // This procedure ignores closing tags
        // (i.e. "</Mission>") and comments (i.e. "<!--")
        if (isalpha(*p))
        {
            // If the first character is alpha, this is a valid tag
            done = true;
        }
    }

    // Copy data until we reach the end
    while (isalnum(*p))
    {
        // Warning: Nothing is done to prevent buffer overflow
        *tag++ = *p++;
    }
    *tag = '\0';

    if (*p == '\0')
    {
        return (false);
    }

    if (*p == ' ')
    {
        p++;
        while ((*p != '\0') && (*p != '>') && (*p != '/'))
        {
            if (isprint(*p))
            {
                // Warning: Nothing is done to prevent buffer overflow
                *attributes++ = *p++;
            }
			if (*p == '\n')
			{
				p++;
				while(*p++ == ' ' || *p++ == '\t');	// skip till we get to the actual data
				p-=2;
			}
        }
    }

    // skip the trailing slash
    if (*p == '/')
    {
        p++;
    }

    // skip the trailing '>' marking the end of the tag
    if (*p == '>')
    {
        p++;
    }

    *attributes = '\0';
    *buffer = p;

    // Search through the list to get the Tag ID
    m_XmlTagID = XML_TAG_ID_INVALID;
    XmlTagLookupTable *table = m_XmlTagLookupTable;
    while (table->tag_id != XML_TAG_ID_INVALID)
    {
        if (strcmp(table->tag, m_Tag) == 0)
        {
            // The tag matched an entry in the table
            m_XmlTagID = (eTagId) table->tag_id;
            break;
        }
        table++;
    }

    if (m_XmlTagID == XML_TAG_ID_INVALID)
    {
        printf("Invalid XML tag: %s\n", m_Tag);
    }

    return (m_XmlTagID != XML_TAG_ID_INVALID);
}

bool XmlParser::GetXmlAttribute(char *attrib, char *value, unsigned int length, bool required)
{
    bool success = false;
    char *buffer = m_Attributes;

    *value = '\0';
    char *p = strstr(buffer, attrib);
    if (p)
    {
        p += strlen(attrib);
        if (*p == '=')
        {
            p++;
            if (*p == '"')
            {
                p++;
                char temp[512];
                strncpy(temp, p, sizeof(temp));
                temp[511] = 0;
                if (strtok(temp, "\""))
                {
                    if (strlen(temp) < length)
                    {
                        strcpy(value, temp);
                        success = true;
                    }
                }
            }
        }
    }

    if (required && !success)
    {
        printf("Error parsing %s %s attribute\n", m_Tag, attrib);
        m_Success = false;
    }

    return (m_Success);
}

bool XmlParser::GetXmlData(char **buffer, char *data, long length)
{
    bool success = false;

    char *src = *buffer;
    char *workbuffer = new char[length + 6];
    char *dest = workbuffer;
    long remaining = length;

    if (dest)
    {
        *dest = '\0';

        while ((*src != '<') && (*src != '\0'))
        {
            *dest++ = *src++;
            // Check length to prevent buffer overflow
            if (--remaining <= 1)
            {
                delete [] workbuffer;
                return(false);
            }
        }

        // Add null terminator
        *dest = '\0';
        if ((src[0] == '<') && (src[1] == '/'))
        {
            *buffer = src;
            success = true;

            // Return the decoded string to the caller
            char *temp = DecodeXmlString(workbuffer);
            strncpy(data, temp, length);
            data[length-1] = 0;
            delete [] temp;
        }

        delete [] workbuffer;
    }

    return (success);
}

int XmlParser::ParseInt(char **buffer, int id, int min, int max)
{
    int value = 0;
    char data[512];

    CheckID(id);
    if (m_Success)
    {
        if (GetXmlData(buffer, data, sizeof(data)))
        {
            value = atoi(data);
            if ((value < min) || (value > max))
            {
                printf("Invalid XML value \"%s\" = %s (min=%d max=%d)\n", m_Tag, data, min, max);
                m_Success = false;
            }
        }
        else
        {
            printf("Error parsing XML data: \"%s\"\n", m_Tag);
            m_Success = false;
        }
    }

    return (value);
}

double XmlParser::ParseDouble(char **buffer, int id, double min, double max, bool allow_minus_one)
{
    double value = 0.0;
    char data[512];

    CheckID(id);
    if (m_Success)
    {
        if (GetXmlData(buffer, data, sizeof(data)))
        {
            value = atof(data);
            if (allow_minus_one && (value == -1))
            {
                // negative one is a legal value
            }
            else if ((value < min) || (value > max))
            {
                printf("Invalid XML value \"%s\" = %s (min=%g max=%g)\n", m_Tag, data, min, max);
                m_Success = false;
            }
        }
        else
        {
            printf("Error parsing XML data: \"%s\"\n", m_Tag);
            m_Success = false;
        }
    }

    return (value);
}

DWORD XmlParser::ParseColor(char **buffer, int id)
{
    DWORD color = 0;
    char data[512];

    CheckID(id);

    if (m_Success)
    {
        if (GetXmlData(buffer, data, sizeof(data)))
        {
            if (strlen(data) == 6)
            {
                // Convert hex to decimal
                char *endptr;
                color = strtoul(data, &endptr, 16);
            }
            else
            {
                printf("Invalid XML color value \"%s\" = %s\n", m_Tag, data);
                m_Success = false;
            }
        }
        else
        {
            printf("Error parsing XML data: \"%s\"\n", m_Tag);
            m_Success = false;
        }
    }

    return (color);
}

char * XmlParser::ParseString(char **buffer, int id)
{
    char data[512];

    CheckID(id);
    if (m_Success)
    {
        if (!GetXmlData(buffer, data, sizeof(data)))
        {
            printf("Error parsing XML data: \"%s\"\n", m_Tag);
            m_Success = false;
        }
    }

    return (DecodeXmlString(data));
}

bool XmlParser::CheckID(int id)
{
    if (m_CurrentID != id)
    {
        printf("Unexpected XML tag: %s\n", m_Tag);
        m_Success = false;
    }

    return (m_Success);
}

// This is a static member function so that it can be called without
// instantiating an XmlFile object.
// NOTE: The caller must delete the returned string!!
char * XmlParser::EncodeXmlString(char * str)
{
    // Replaces ampersand, less than, and greater than symbols in a string
    char *result = NULL;
    int length = strlen(str) * 5 + 1; // make sure we have enough room to avoid buffer overflow
    char * temp_string = new char[length];
    if (temp_string)
    {
        temp_string[0] = 0;
        char *p = str;

        while (*p)
        {
            char c = *p;
            if (c == '&')
            {
                strcat(temp_string, "&amp;");
            }
            else if (c == '<')
            {
                strcat(temp_string, "&lt;");
            }
            else if (c == '>')
            {
                strcat(temp_string, "&gt;");
            }
            else
            {
                int index = strlen(temp_string);
                temp_string[index++] = c;
                temp_string[index] = 0;
            }

            p++;
        }

        result = new char[strlen(temp_string) + 1];
        if (result)
        {
            strcpy(result, temp_string);
        }
        delete [] temp_string;
    }

    return (result);
}


// This is a static member function so that it can be called without
// instantiating an XmlFile object.
char *XmlParser::DecodeXmlString(char * str)
{
    // Restores ampersand, less than, and greater than symbols in a string
    char *result = NULL;
    int length = strlen(str) * 5 + 1; // make sure we have enough room to avoid buffer overflow
    char * temp_string = new char[length];
    if (temp_string)
    {
        temp_string[0] = 0;
        char *p = str;

        while (*p)
        {
            if (strncmp(p, "&amp;", 5) == 0)
            {
                strcat(temp_string, "&");
                p += 5;
            }
            if (strncmp(p, "&apos;", 6) == 0)
            {
                strcat(temp_string, "'");
                p += 6;
            }
            else if (strncmp(p, "&lt;", 4) == 0)
            {
                strcat(temp_string, "<");
                p += 4;
            }
            else if (strncmp(p, "&gt;", 4) == 0)
            {
                strcat(temp_string, ">");
                p += 4;
            }
            else
            {
                int index = strlen(temp_string);
                temp_string[index++] = *p;
                temp_string[index] = 0;
                p++;
            }
        }

        result = new char[strlen(temp_string) + 1];
        if (result)
        {
            strcpy(result, temp_string);
        }
        delete [] temp_string;
    }

    return (result);
}

bool XmlParser::ParseFloatArray(char *attrib_values, int count, float *array)
{
    bool success = true; // assume we will succeed

    char *p = strtok(attrib_values, ",");
    for (int i = 0; i < count; i++)
    {
        if (!p)
        {
            // abort if we can't find a comma
            success = false;
            break;
        }
        array[i] = (float) atof(p);
        p = strtok(NULL, ",");
    }

    return (success);
}

