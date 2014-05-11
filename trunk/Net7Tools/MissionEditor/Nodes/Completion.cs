using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using CommonTools;
using System.Xml;
using MissionEditor.Database;

namespace MissionEditor.Nodes
{
    public class Completion
    {
        private CompletionType m_completionType;
        private String m_value;
        private Int32 m_count;
        private String m_data;

        public Completion()
        {
            clear();
        }

        public void clear()
        {
            m_value = "";
            m_count = -1;
            m_data = "";
        }

        public void setCompletionType(CompletionType id)
        {
            m_completionType = id;
        }

        public CompletionType getCompletionType()
        {
            return m_completionType;
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
            switch (m_completionType)
            {
                case CompletionType.Arrive_At:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.sector_object, getValue());
                case CompletionType.Talk_Space_Npc:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.sector_object, getValue());
                case CompletionType.Proximity_To_Space_Npc:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.sector_object, getValue());
                case CompletionType.Nav_Message:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.sector_object, getValue());
                case CompletionType.Fight_Mob:
                    {
                        String mob = DataConfiguration.getDescription(DataConfiguration.DataType.mob, getValue());
                        return getCount().ToString() + " x " + mob;
                    }
                case CompletionType.Talk_To_Npc:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.npc, getValue());
                case CompletionType.Give_Item:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.item, getValue());
                case CompletionType.Current_Sector:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.sector, getValue());
                case CompletionType.Obtain_Items:
                case CompletionType.Possess_Item:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.item, getValue());
                /*case CompletionType.Obtain_Items_At_Location:
                case CompletionType.Take_Item_To_Location:
                    {
                        String item = DataConfiguration.getDescription(DataConfiguration.DataType.item, getData());
                        String location = DataConfiguration.getDescription(DataConfiguration.DataType.sector_object, getValue());
                        return item + " @ " + location;
                    }*/
                case CompletionType.Use_Skill_On_Mob_Type:
                    {
                        String skill = DataConfiguration.getDescription(DataConfiguration.DataType.skill, getData());
                        String mob = DataConfiguration.getDescription(DataConfiguration.DataType.mob, getValue());
                        return skill + " > " + mob;
                    }
                case CompletionType.Use_Skill_On_Object:
                    {
                        String skill = DataConfiguration.getDescription(DataConfiguration.DataType.skill, getData());
                        String mob = DataConfiguration.getDescription(DataConfiguration.DataType.sector_object, getValue());
                        return skill + " > " + mob;
                    }
                case CompletionType.Nearest_Nav:
                    return DataConfiguration.getDescription(DataConfiguration.DataType.sector_object, getValue());
            }
            return m_value;
        }

        public void setCount(Int32 count)
        {
            m_count = count;
        }

        public Int32 getCount()
        {
            return m_count;
        }

        public void setData(String data)
        {
            m_data = data;
        }

        public String getData()
        {
            return m_data;
        }

        /// <summary>
        /// Test whether a list of Completion items contains an entry of the specified CompletionType
        /// </summary>
        /// <param name="completionList">The list of Completion items</param>
        /// <param name="completionType">The CompletionType to search for</param>
        /// <returns>Whether the list contains the searched CompletionType</returns>
        public Boolean contains(List<Completion> completionList, CompletionType completionType)
        {
            if (completionList != null)
            {
                foreach (Completion completion in completionList)
                {
                    if (completion.getCompletionType().Equals(completionType))
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// Parse the XML nodes
        /// </summary>
        /// <param name="xmlNode">The <Completion></Completion> nodes</param>
        public void fromXml(XmlNode xmlNode)
        {
            String value;
            Int32 intValue;
            Xml.getAttribute(xmlNode, XmlAttributes.ID, true, out value);
            CompletionType completionType;
            if (CommonTools.Enumeration.TryParse<CompletionType>(value, out completionType))
            {
                setCompletionType(completionType);
            }
            else
            {
                throw (new Exception("Unable to convert '" + value + "' into a CompletionType" + "\n\n" + xmlNode.InnerXml));
            }

            if (Xml.getAttribute(xmlNode, XmlAttributes.COUNT, false, out intValue))
            {
                setCount(intValue);
            }

            if (Xml.getAttribute(xmlNode, XmlAttributes.DATA, false, out value))
            {
                setData(value);
            }

            Xml.getValue(xmlNode, true, out value);
            setValue(value);

            addValidations();
        }

        public void getXML(StringWriter stringWriter)
        {
            stringWriter.Write(Xml.tagStart()
                             + XmlTag.COMPLETION
                             + Xml.attribute(XmlAttributes.ID, ((int)m_completionType).ToString()));
            switch (getCompletionType())
            {
                case CompletionType.Arrive_At:
                case CompletionType.Proximity_To_Space_Npc:
                case CompletionType.Talk_Space_Npc:
                    break;
                case CompletionType.Nav_Message:
                    stringWriter.Write(Xml.attribute(XmlAttributes.COUNT, getCount()));
                    break;
                case CompletionType.Fight_Mob:
                    stringWriter.Write(Xml.attribute(XmlAttributes.COUNT, getCount()));
                    break;
                case CompletionType.Give_Credits:
                    break;
                case CompletionType.Give_Item:
                    if (getCount() != -1)
                    {
                        stringWriter.Write(Xml.attribute(XmlAttributes.COUNT, getCount()));
                    }
                    break;
                case CompletionType.Nearest_Nav:
                    break;
                case CompletionType.Obtain_Items:
                    if (getCount() != -1)
                    {
                        stringWriter.Write(Xml.attribute(XmlAttributes.COUNT, getCount()));
                    }
                    break;
                /*case CompletionType.Obtain_Items_At_Location:
                    if (getCount() != -1)
                    {
                        stringWriter.Write(Xml.attribute(XmlAttributes.COUNT, getCount()));
                    }
                    stringWriter.Write(Xml.attribute(XmlAttributes.DATA, getData()));
                    break;*/
                case CompletionType.Possess_Item:
                    stringWriter.Write(Xml.attribute(XmlAttributes.COUNT, getCount()));
                    break;
                case CompletionType.Receive_Item:
                    break;
                case CompletionType.Current_Sector:
                    break;
                /*case CompletionType.Take_Item_To_Location:
                    stringWriter.Write(Xml.attribute(XmlAttributes.DATA, getData()));
                    stringWriter.Write(Xml.attribute(XmlAttributes.COUNT, getCount()));
                    break;*/
                case CompletionType.Talk_To_Npc:
                    break;
                case CompletionType.Use_Skill_On_Mob_Type:
                    stringWriter.Write(Xml.attribute(XmlAttributes.DATA, getData()));
                    break;
                case CompletionType.Use_Skill_On_Object:
                    stringWriter.Write(Xml.attribute(XmlAttributes.DATA, getData()));
                    break;
            }
            stringWriter.WriteLine(">"
                                 + getValue()
                                 + Xml.tagEnd(XmlTag.COMPLETION));
        }

        public void addValidations()
        {
            if (m_value == null || m_value.Length == 0)
            {
                DataConfiguration.addValidation("The completion value is mandatory");
            }
            else
            {
                Int32 count;
                switch (getCompletionType())
                {
                    case CompletionType.Arrive_At:
                    case CompletionType.Proximity_To_Space_Npc:
                    case CompletionType.Talk_Space_Npc:
                        DataConfiguration.addValidation(DataConfiguration.DataType.sector_object, getValue());
                        break;
                    case CompletionType.Nav_Message:
                        if (getCount() == -1)
                        {
                            setCount(5000);
                        }
                        if (getCount() != 5000 && getCount() != 10000 && getCount() != 30000)
                        {
                            DataConfiguration.addValidation("The 3 Nav Message ranges are 5000, 10000 and 30000. You must use one of these ranges to trigger your message.");
                        }
                        DataConfiguration.addValidation(DataConfiguration.DataType.sector_object, getValue());
                        break;
                    case CompletionType.Fight_Mob:
                        if (getCount() == -1)
                        {
                            setCount(1);
                        }
                        if (getCount() < 0 || getCount() > 5000)
                        {
                            DataConfiguration.addValidation("The number of mobs to fight must be between 1 and 5,000");
                        }
                        DataConfiguration.addValidation(DataConfiguration.DataType.mob, getValue());
                        break;
                    case CompletionType.Give_Credits:
                        if (!Int32.TryParse(getValue(), out count)
                            || count < 0 || count > 25000)
                        {
                            DataConfiguration.addValidation("The amount of credits to give must be between 1 and 25,000");
                        }
                        break;
                    case CompletionType.Give_Item:
                        if (getCount() == -1)
                        {
                            setCount(1);
                        }
                        if (getCount() < 0 || getCount() > 500)
                        {
                            DataConfiguration.addValidation("The quantity of items to give must be between 1 and 500");
                        }
                        DataConfiguration.addValidation(DataConfiguration.DataType.item, getValue());
                        break;
                    case CompletionType.Nearest_Nav:
                        //can only have one nearest Nav for completions
                        DataConfiguration.addValidation(DataConfiguration.DataType.sector_object, getValue());
                        break;
                    case CompletionType.Obtain_Items:
                        DataConfiguration.addValidation(DataConfiguration.DataType.item, getValue());
                        break;
                    /*case CompletionType.Obtain_Items_At_Location:
                        DataConfiguration.addValidation(DataConfiguration.DataType.sector_object, getValue());
                        DataConfiguration.addValidation(DataConfiguration.DataType.item, getData());
                        break;*/
                    case CompletionType.Possess_Item:
                        if (getCount() == -1)
                        {
                            setCount(1);
                        }
                        if (getCount() < 0 || getCount() > 5000)
                        {
                            DataConfiguration.addValidation("The quantity must be between 1 and 5,000");
                        }
                        DataConfiguration.addValidation(DataConfiguration.DataType.item, getValue());
                        break;
                    case CompletionType.Receive_Item:
                        if (!Int32.TryParse(getValue(), out count)
                            || count < 0 || count > 10)
                        {
                            DataConfiguration.addValidation("The quantity must be between 1 and 10");
                        }
                        break;
                    case CompletionType.Current_Sector:
                        DataConfiguration.addValidation(DataConfiguration.DataType.sector, getValue());
                        break;
                    /*case CompletionType.Take_Item_To_Location:
                        DataConfiguration.addValidation(DataConfiguration.DataType.sector_object, getValue());
                        DataConfiguration.addValidation(DataConfiguration.DataType.item, getData());
                        break;*/
                    case CompletionType.Talk_To_Npc:
                        DataConfiguration.addValidation(DataConfiguration.DataType.npc, getValue());
                        break;
                    case CompletionType.Use_Skill_On_Mob_Type:
                        DataConfiguration.addValidation(DataConfiguration.DataType.mob, getValue());
                        DataConfiguration.addValidation(DataConfiguration.DataType.skill, getData());
                        break;
                    case CompletionType.Use_Skill_On_Object:
                        DataConfiguration.addValidation(DataConfiguration.DataType.sector_object, getValue());
                        DataConfiguration.addValidation(DataConfiguration.DataType.skill, getData());
                        break;
                }
            }
        }

        internal void getReport(StringWriter stringWriter)
        {
            stringWriter.WriteLine("<tr><td>" + m_completionType.ToString() + "</td>");
            stringWriter.WriteLine("<td>" + getFormattedValue() + "</td></tr>");
        }

    }
}
