using System;
using System.Collections.Generic;
using System.Text;
using CommonTools;
using System.Xml;
using MissionEditor.Database;

namespace MissionEditor.Nodes
{
    public class Reward
    {
        private RewardType m_rewardType;
        private String m_value;
        private String m_flag;

        public Reward()
        {
            clear();
        }

        public void clear()
        {
            m_rewardType = RewardType.Credits;
            m_value = "";
            m_flag = "";
        }

        public void setRewardType(RewardType rewardType)
        {
            m_rewardType = rewardType;
        }

        public RewardType getRewardType()
        {
            return m_rewardType;
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
            switch (m_rewardType)
            {
                case RewardType.Award_Skill:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.skill, getValue());
                case RewardType.Faction:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.faction, getFlag());
                case RewardType.Item_ID:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.item, getValue());
            }
            return m_value;
        }

        public void setFlag(String flag)
        {
            m_flag = flag;
        }

        public Boolean hasFlag()
        {
            return m_flag != null && m_flag.Length != 0;
        }

        public String getFlag()
        {
            return m_flag;
        }

        /// <summary>
        /// Parse the XML nodes
        /// </summary>
        /// <param name="xmlNode">The <Reward></Reward> nodes</param>
        public void fromXml(XmlNode xmlNode)
        {
            String value;
            Int32 intValue;
            Xml.getAttribute(xmlNode, XmlAttributes.ID, true, out value);
            RewardType rewardType;
            if (Enumeration.TryParse<RewardType>(value, out rewardType))
            {
                setRewardType(rewardType);
            }
            else
            {
                throw (new Exception("Unable to convert '" + value + "' into a RewardType" + "\n\n" + xmlNode.InnerXml));
            }

            switch (rewardType)
            {
                case RewardType.Credits:
                case RewardType.Explore_XP:
                case RewardType.Combat_XP:
                case RewardType.Trade_XP:
                case RewardType.Faction:
                    Xml.getValue(xmlNode, true, out intValue);
                    setValue(intValue.ToString());
                    break;
                case RewardType.Advance_Mission:
                case RewardType.Award_Skill:
                case RewardType.Item_ID:
                    Xml.getValue(xmlNode, true, out value);
                    setValue(value);
                    break;
                case RewardType.Hull_Upgrade:
                    Xml.getValue(xmlNode, true, out intValue);
                    setValue(intValue.ToString());
                    break;
                case RewardType.Run_Script:
                    Xml.getValue(xmlNode, true, out value);
                    setValue(value);
                    break;
            }

            if (Xml.getAttribute(xmlNode, XmlAttributes.FLAGS, false, out value))
            {
                setFlag(value);
            }

            addValidations();
        }

        public void getXML(System.IO.StringWriter stringWriter)
        {
            stringWriter.WriteLine("<"
                                  + XmlTag.REWARD
                                  + Xml.attribute(XmlAttributes.ID, ((int)m_rewardType).ToString())
                                  + (hasFlag() ? Xml.attribute(XmlAttributes.FLAGS, getFlag()) : "")
                                  + Xml.tagEnd()
                                  + m_value.ToString()
                                  + Xml.tagEnd(XmlTag.REWARD));
        }

        public void addValidations()
        {
            if (m_value == null || m_value.Length == 0)
            {
                DataConfiguration.addValidation("The reward value is mandatory");
            }
            else
            {
                Int32 intValue;
                switch (getRewardType())
                {
                    case RewardType.Credits:
                    case RewardType.Explore_XP:
                    case RewardType.Combat_XP:
                    case RewardType.Trade_XP:
                        if (!Int32.TryParse(getValue(), out intValue)
                            || intValue < 0 || intValue > 500000)
                        {
                            DataConfiguration.addValidation("The reward quantity '" + getValue() + "' must be between 1 and 500,000");
                        }
                        break;
                    case RewardType.Faction:
                        if (!Int32.TryParse(getValue(), out intValue)
                            || intValue < 0 || intValue > 500000)
                        {
                            DataConfiguration.addValidation("The reward quantity '" + getValue() + "' must be between 1 and 500,000");
                        }
                        DataConfiguration.addValidation(DataConfiguration.DataType.faction, getFlag());
                        break;
                    case RewardType.Item_ID:
                        DataConfiguration.addValidation(DataConfiguration.DataType.item, getValue());
                        break;
                    case RewardType.Hull_Upgrade:
                        if (!Int32.TryParse(getValue(), out intValue)
                            || intValue < 0 || intValue > 6)
                        {
                            DataConfiguration.addValidation("The quantity '" + getValue() + "' must be between 1 and 6");
                        }
                        break;
                    case RewardType.Advance_Mission:
                        DataConfiguration.addValidation(DataConfiguration.DataType.mission, getValue());
                        break;
                    case RewardType.Run_Script:
                        break;
                }
            }
        }


        internal void getReport(System.IO.StringWriter stringWriter)
        {
            stringWriter.WriteLine("<tr><td>" + m_rewardType.ToString() + "</td>");
            stringWriter.WriteLine("<td>" + getFormattedValue() + "</td></tr>");
        }
    }
}
