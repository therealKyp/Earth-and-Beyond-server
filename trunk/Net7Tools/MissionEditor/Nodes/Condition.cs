using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using CommonTools;
using System.Xml;
using System.Windows.Forms;
using MissionEditor.Database;

namespace MissionEditor.Nodes
{
    public class Condition
    {
        private ConditionType m_conditionType;
        private String m_value;
        private String m_flag;

        public Condition()
        {
            clear();
        }

        public void clear()
        {
            m_value = "";
            m_flag = "";
        }

        public void setConditionType(ConditionType restrictionType)
        {
            m_conditionType = restrictionType;
        }

        public ConditionType getConditionType()
        {
            return m_conditionType;
        }

        public void setFlag(String flag)
        {
            m_flag = flag;
        }

        public String getFlag()
        {
            return m_flag;
        }

        public void setValue(String value)
        {
            m_value = value;
        }

        public String getValue()
        {
            return m_value;
        }

        public String getFormattedValue()
        {
            switch (m_conditionType)
            {
                case ConditionType.Item_Required:
                {
                    String item = DataConfiguration.getDescription(DataConfiguration.DataType.item, getFlag());
                    return getValue() + " x " + item;
                }
                case ConditionType.Faction_Required:
                {
                    String item = DataConfiguration.getDescription(DataConfiguration.DataType.faction, getFlag());
                    return getValue() + " x " + item;
                }
                case ConditionType.Race:
                {
                    Races race;
                    if (Enumeration.TryParse<Races>(m_value, out race))
                    {
                        return race.ToString();
                    }
                    break;
                }
                case ConditionType.Profession:
                {
                    Professions profession;
                    if (Enumeration.TryParse<Professions>(m_value, out profession))
                    {
                        return profession.ToString();
                    }
                    break;
                }
                case ConditionType.Mission_Required:
                {
                    String mission = DataConfiguration.getDescription(DataConfiguration.DataType.mission, getValue());
                    return mission;
                }
            }
            return m_value;
        }

        /// <summary>
        /// Parse the XML nodes
        /// </summary>
        /// <param name="xmlNode">The <Condition></Condition> nodes</param>
        public void fromXml(XmlNode xmlNode)
        {
            String value;
            Xml.getAttribute(xmlNode, XmlAttributes.ID, true, out value);
            ConditionType conditionType;
            if (Enumeration.TryParse<ConditionType>(value, out conditionType))
            {
                setConditionType(conditionType);
            }
            else
            {
                throw (new Exception("Unable to convert '" + value + "' into a ConditionType" + "\n\n" + xmlNode.InnerXml));
            }
            Xml.getValue(xmlNode, true, out value);
            setValue(value);
            if (Xml.getAttribute(xmlNode, XmlAttributes.FLAGS, false, out value))
            {
                setFlag(value);
            }

            addValidations();
        }

        public void getXML(StringWriter stringWriter)
        {
            stringWriter.WriteLine("<"
                                 + XmlTag.CONDITION
                                 + Xml.attribute(XmlAttributes.ID, (int)m_conditionType)
                                 + (m_flag.Length != 0 ? Xml.attribute(XmlAttributes.FLAGS, m_flag) : "")
                                 + ">"
                                 + m_value
                                 + Xml.tagEnd(XmlTag.CONDITION));
        }

        public void getReport(StringWriter stringWriter)
        {
            stringWriter.WriteLine("<TR><TD>" + m_conditionType.ToString() + "</TD>");
            //stringWriter.WriteLine("<TD>" + "</TD>");
            stringWriter.WriteLine("<TD>" + getFormattedValue() + "</TD></TR>");
        }

        public void addValidations()
        {
            Int32 value;
            switch (m_conditionType)
            {
                case CommonTools.ConditionType.Overall_Level:
                    if (!Int32.TryParse(getValue(), out value)
                        || value < 1 || value > 150)
                    {
                        DataConfiguration.addValidation("The " + m_conditionType + " should be within 1 and 150.");
                    }
                    break;
                case CommonTools.ConditionType.Combat_Level:
                case CommonTools.ConditionType.Explore_Level:
                case CommonTools.ConditionType.Trade_Level:
                    if (!Int32.TryParse(getValue(), out value)
                        || value < 1 || value > 50)
                    {
                        DataConfiguration.addValidation("The " + m_conditionType + " should be within 1 and 50.");
                    }
                    break;
                case CommonTools.ConditionType.Hull_Level:
                    if (!Int32.TryParse(getValue(), out value)
                        || value < 1 || value > 6)
                    {
                        DataConfiguration.addValidation("The " + m_conditionType + " should be within 1 and 6.");
                    }
                    break;
                case CommonTools.ConditionType.Faction_Required:
                    if (!Int32.TryParse(getValue(), out value)
                        || value < 1 || value > 999999)
                    {
                        DataConfiguration.addValidation("The " + m_conditionType + " should be within 1 and 999,999.");
                    }
                    DataConfiguration.addValidation(DataConfiguration.DataType.faction, getFlag());
                    break;
                case CommonTools.ConditionType.Item_Required:
                    if (!Int32.TryParse(getValue(), out value)
                        || value < 1 || value > 5000)
                    {
                        DataConfiguration.addValidation("The " + m_conditionType + " should be within 1 and 5,000.");
                    }
                    DataConfiguration.addValidation(DataConfiguration.DataType.item, getFlag());
                    break;
                case CommonTools.ConditionType.Profession:
                    if (!"012".Contains(getValue()))
                    {
                        DataConfiguration.addValidation("Profession type is invalid.");
                    }
                    break;
                case CommonTools.ConditionType.Race:
                    if (!"012".Contains(getValue()))
                    {
                        DataConfiguration.addValidation("Race type is invalid.");
                    }
                    break;
                case CommonTools.ConditionType.Mission_Required:
                    DataConfiguration.addValidation(DataConfiguration.DataType.mission, getValue());
                    break;
            }
        }

    }
}
