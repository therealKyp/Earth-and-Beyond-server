using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Serialization;

namespace CommonTools
{
    public static class Xml
    {
        /// <summary>
        /// Retrieve an XML tag from a collection of attributes.
        /// </summary>
        /// <param name="attributes">The collection of attributes</param>
        /// <param name="xmlTag">The XML tag to retrieve</param>
        /// <param name="required">Whether the XML tag is required</param>
        /// <param name="value">The value of the attributed</param>
        /// <returns>Whether the attribute was retrieved</returns>
        public static Boolean getAttribute(XmlNode xmlNode, String xmlTag, Boolean required, out String value)
        {
            foreach (XmlAttribute attribute in xmlNode.Attributes)
            {
                if (xmlNode == null)
                {
                    throw (new Exception("The specified node is null"));
                }

                if (attribute.Name.Equals(xmlTag))
                {
                    value = attribute.Value;
                    return true;
                }
            }

            if (required)
            {
                throw(new XmlException("The mandatory XML attribute '"
                                     + xmlTag
                                     + "' was not specified in the node.\n\n"
                                     + xmlNode.InnerXml));
            }
            value = "";
            return false;
        }

        /// <summary>
        /// Retrieve an XML tag from a collection of attributes.
        /// </summary>
        /// <param name="attributes">The collection of attributes</param>
        /// <param name="xmlTag">The XML tag to retrieve</param>
        /// <param name="required">Whether the XML tag is required</param>
        /// <param name="value">The value of the attributed</param>
        /// <returns>Whether the attribute was retrieved</returns>
        public static Boolean getAttribute(XmlNode xmlNode, String xmlTag, Boolean required, out Int32 value)
        {
            String stringValue;
            value = 0;
            Boolean hasAttribute = getAttribute(xmlNode, xmlTag, required, out stringValue);
            if (hasAttribute
                && !Int32.TryParse(stringValue, out value))
            {
                throw (new XmlException("The value '"
                                      + stringValue
                                      + "' of the XML attribute '"
                                      + xmlTag
                                      + "' could not be converted to an integer.\n\n"
                                      + xmlNode.InnerXml));
            }
            return hasAttribute;
        }

        /// <summary>
        /// Retrieve the value specified within a node:  &lt;Node>value&lt;/Node>
        /// </summary>
        /// <param name="node">The node from which to extract the value</param>
        /// <param name="required">Whether the XML tag is required</param>
        /// <param name="value">The value of the node</param>
        /// <returns>Whether the value was retrieved</returns>
        public static Boolean getValue(XmlNode xmlNode, Boolean required, out String value)
        {
            value = null;

            if (xmlNode == null)
            {
                throw (new Exception("The specified node is null"));
            }

            /*if (xmlNode.HasChildNodes
                && xmlNode.ChildNodes.Count == 1
                && xmlNode.FirstChild != null
                && xmlNode.FirstChild.NodeType.Equals(XmlNodeType.Text))*/
            if(xmlNode.InnerText != null)
            {
                //value = xmlNode.FirstChild.Value;
                value = xmlNode.InnerText;
            }
            else
            {
                throw (new XmlException("The node '"
                                      + xmlNode.Name
                                      + "' was expected to contain a value within its start and end.\n\n"
                                      + xmlNode.OuterXml));
            }

            if (required && (value == null || value.Length == 0))
            {
                throw (new XmlException("The value of the node '"
                                     + xmlNode.Name
                                     + "' is empty.\n\n"
                                     + xmlNode.InnerXml));
            }
            return value != null;
        }

        /// <summary>
        /// Retrieve the value specified within a node:  &lt;Node>value&lt;/Node>
        /// </summary>
        /// <param name="node">The node from which to extract the value</param>
        /// <param name="required">Whether the XML tag is required</param>
        /// <param name="value">The value of the node</param>
        /// <returns>Whether the value was retrieved</returns>
        public static Boolean getValue(XmlNode xmlNode, Boolean required, out Int32 value)
        {
            value = 0;
            String stringValue;
            Boolean hasValue = getValue(xmlNode, required, out stringValue);
            if (hasValue
                && !Int32.TryParse(stringValue, out value))
            {
                throw (new XmlException("The value '"
                                      + stringValue
                                      + "' of the node '"
                                      + xmlNode.Name
                                      + "' could not be converted to an integer.\n\n"
                                      + xmlNode.InnerXml));
            }
            return hasValue;
        }

        /// <summary>
        /// Return a start tag "&lt;"
        /// </summary>
        /// <returns></returns>
        public static String tagStart()
        {
            return "<";
        }

        /// <summary>
        /// Return a start tag &lt;tag>
        /// </summary>
        /// <param name="tag">The tag to create</param>
        /// <returns></returns>
        public static String tagStart(String tag)
        {
            return tagStart() + tag + tagEnd();
        }

        /// <summary>
        /// Return a tag "&lt;tag>value&lt;/tag>"
        /// </summary>
        /// <param name="tag">The tag to create</param>
        /// <returns></returns>
        public static String tag(String tag, String value)
        {
            return tagStart(tag) + value + tagEnd(tag);
        }

        /// <summary>
        /// Return an end tag "&lt;/tag>"
        /// </summary>
        /// <param name="tag"></param>
        /// <returns></returns>
        public static String tagEnd(String tag)
        {
            return tagStart() + "/" + tag + tagEnd();
        }

        /// <summary>
        /// Return an end tag ">"
        /// </summary>
        /// <returns></returns>
        public static String tagEnd()
        {
            return ">";
        }

        /// <summary>
        /// Return an attribute
        /// </summary>
        /// <param name="attrib">The attribute</param>
        /// /// <param name="value">The value</param>
        /// <returns></returns>
        public static String attribute(String attrib, String value)
        {
            return " " + attrib + "=\"" + value + "\"";
        }

        /// <summary>
        /// Return an attribute
        /// </summary>
        /// <param name="attrib">The attribute</param>
        /// /// <param name="value">The value</param>
        /// <returns></returns>
        public static String attribute(String attrib, Int32 value)
        {
            return attribute(attrib, value.ToString());
        }

        /// <summary>
        /// Return an attribute
        /// </summary>
        /// <param name="attrib">The attribute</param>
        /// /// <param name="value">The value</param>
        /// <returns></returns>
        public static String attribute(String attrib, Boolean value)
        {
            return attribute(attrib, (value ? "1" : "0"));
        }

    }
}
