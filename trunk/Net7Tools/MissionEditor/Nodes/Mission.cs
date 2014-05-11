using System;
using System.Collections.Generic;
using System.Text;
using MissionEditor.Nodes;
using System.IO;
using System.Xml;
using CommonTools;
using System.Windows.Forms;
using MissionEditor.Database;

namespace MissionEditor
{
    public class Mission
    {
        private String m_id;
        private Boolean m_forfeitable;
        private Int32 m_allowedTime;
        private String m_name;
        private String m_summary;
        private Int32 m_overallLevel; // Should only be within the conditions?
        private String m_xml;
        private MissionType m_missionType;
        private String m_key;
        private List<Condition> m_conditions;
        private List<Stage> m_stages;

        public Mission()
        {
            m_conditions = null;
            m_stages = null;
            clear();
            m_name = "";
            m_key = "";
        }

        public void clear()
        {
            // Only "clear" variables that will be reloaded from missions.mission_XML
            //m_name = "";
            //m_key = "";
            m_id = "";
            m_forfeitable = true;
            m_allowedTime = 0;
            m_summary = "";
            m_overallLevel = -1;
            m_xml = "";
            if (hasConditions())
            {
                m_conditions.Clear();
            }
            if (hasStages())
            {
                m_stages.Clear();
            }
        }

        public void setId(String id)
        {
            m_id = id;
        }

        public String getId()
        {
            return m_id;
        }

        public void setForfeitable(Boolean forfeitable)
        {
            m_forfeitable = forfeitable;
        }

        public Boolean isForfeitable()
        {
            return m_forfeitable;
        }

        public void setAllowedTime(Int32 time)
        {
            m_allowedTime = time;
        }

        public Int32 getAllowedTime()
        {
            return m_allowedTime;
        }

        public void setName(String name)
        {
            m_name = name;
        }

        public String getName()
        {
            return m_name;
        }

        public void setSummary(String summary)
        {
            m_summary = summary;
        }

        public String getSummary()
        {
            return m_summary;
        }

        public void setOverallLevel(Int32 overallLevel)
        {
            m_overallLevel = overallLevel;
        }

        public Int32 getOverallLevel()
        {
            return m_overallLevel;
        }

        public void setXml(String xml)
        {
            m_xml = xml;
        }

        public String getXml()
        {
            return m_xml;
        }

        public void setType(MissionType type)
        {
            m_missionType = type;
        }

        public MissionType getType()
        {
            return m_missionType;
        }

        public void setKey(String key)
        {
            m_key = key;
        }

        public String getKey()
        {
            return m_key;
        }

        public void addCondition(Condition condition)
        {
            if (m_conditions == null)
            {
                m_conditions = new List<Condition>();
            }
            m_conditions.Add(condition);
        }

        public Boolean hasConditions()
        {
            return m_conditions != null && m_conditions.Count != 0;
        }

        public List<Condition> getConditions()
        {
            return m_conditions;
        }

        public void removeCondition(Condition condition)
        {
            if (m_conditions != null)
            {
                m_conditions.Remove(condition);
            }
        }

        public void clearConditions()
        {
            if (m_conditions != null)
            {
                m_conditions.Clear();
            }
        }

        public void addStage(Stage stage)
        {
            if (m_stages == null)
            {
                m_stages = new List<Stage>();
            }
            m_stages.Add(stage);
        }

        public Boolean hasStages()
        {
            return m_stages != null && m_stages.Count != 0;
        }

        public List<Stage> getStages()
        {
            return m_stages;
        }

        public void removeStage(Stage stage)
        {
            if (m_stages != null)
            {
                m_stages.Remove(stage);
            }
        }

        public void clearStages()
        {
            if (m_stages != null)
            {
                m_stages.Clear();
            }
        }


        /// <summary>
        /// Load an XML mission from the XML String
        /// </summary>
        public void parseXml()
        {
            XmlDocument xmlDocument = new XmlDocument();
            xmlDocument.Load(new StringReader(Xml.tagStart(XmlTag.MISSIONS)
                                            + getXml()
                                            + Xml.tagEnd(XmlTag.MISSIONS)));
            XmlNode xmlMissionNode = xmlDocument.DocumentElement;
            if (xmlMissionNode.HasChildNodes)
            {
                XmlNode xmlNode;
                XmlNodeList nodeList = xmlMissionNode.ChildNodes;
                for (int i = 0; i <= nodeList.Count - 1; i++)
                {
                    xmlNode = xmlMissionNode.ChildNodes[i];
                    if (xmlNode.Name.Equals(XmlTag.MISSION))
                    {
                        clear();
                        fromXML(xmlNode);
                    }
                    else
                    {
                        throw(new Exception("Unexpected node: " + xmlNode.Name + " in the" + XmlTag.MISSIONS + " tag"));
                    }
                }
            }
            else
            {
                throw(new Exception("Invalid XML structure for: " + xmlMissionNode.Name + "\nShould not have a node without children"));
            }
        }

        /// <summary>
        /// Parse the XML Mission nodes
        /// </summary>
        /// <param name="xmlNode">The <Mission></Mission> nodes</param>
        private void fromXML(XmlNode xmlNode)
        {
            String value;
            Int32 intValue;

            Xml.getAttribute(xmlNode, XmlAttributes.ID, true, out value);
            setId(value);

            if (Xml.getAttribute(xmlNode, XmlAttributes.FORFEITABLE, false, out value))
            {
                setForfeitable(value.Equals("1"));
            }

            Xml.getAttribute(xmlNode, XmlAttributes.TIME, true, out intValue);
            setAllowedTime(intValue);

            if (Xml.getAttribute(xmlNode, XmlAttributes.OVERALL, false, out intValue))
            {
                setOverallLevel(intValue);
            }

            foreach (XmlNode missionChild in xmlNode.ChildNodes)
            {
                if (missionChild.Name.Equals(XmlTag.NAME))
                {
                    Xml.getValue(missionChild, true, out value);
                    setName(value);
                }
                else if (missionChild.Name.Equals(XmlTag.SUMMARY))
                {
                    Xml.getValue(missionChild, true, out value);
                    setSummary(value);
                }
                else if (missionChild.Name.Equals(XmlTag.CONDITION))
                {
                    Condition condition = new Condition();
                    condition.fromXml(missionChild);
                    addCondition(condition);
                }
                else if (missionChild.Name.Equals(XmlTag.STAGE))
                {
                    Stage stage = new Stage();
                    stage.fromXml(missionChild);
                    addStage(stage);
                }
                else
                {
                    MessageBox.Show("Unexpected node: " + missionChild.Name + " in the" + XmlTag.MISSION + " tag");
                }
            }

            addValidations();
        }

        public String getXML()
        {
            StringWriter stringWriter = new StringWriter();
            stringWriter.WriteLine(Xml.tagStart()
                                  + XmlTag.MISSION
                                  + Xml.attribute(XmlAttributes.ID, getId())
                                  + Xml.attribute(XmlAttributes.FORFEITABLE, isForfeitable())
                                  + Xml.attribute(XmlAttributes.TIME, getAllowedTime())
                                  + Xml.tagEnd());
            stringWriter.WriteLine(Xml.tag(XmlTag.NAME, getName()));
            stringWriter.WriteLine(Xml.tag(XmlTag.SUMMARY, getSummary()));
            if (hasConditions())
            {
                foreach (Condition condition in getConditions())
                {
                    condition.getXML(stringWriter);
                }
            }
            if (hasStages())
            {
                foreach (Stage stage in getStages())
                {
                    stage.getXML(stringWriter);
                }
            }
            stringWriter.WriteLine(Xml.tagEnd(XmlTag.MISSION));
            return stringWriter.ToString();
        }

        public String getReport()
        {
            StringWriter stringWriter = new StringWriter();
            stringWriter.WriteLine("<HTML>");
            stringWriter.WriteLine("<BODY>");
            stringWriter.WriteLine("<H1>" + getName() + "</H1>");
            stringWriter.WriteLine(getSummary());
            stringWriter.WriteLine("<BR><BR><TABLE border=\"1\">");
            stringWriter.WriteLine("<TR><TH>ID</TH><TH>Forfeitable</TH><TH>Allowed Time</TH></TR>");
            stringWriter.WriteLine("<TR><TD>" + getId() + "</TD>"
                                  + "<TD>" + (isForfeitable() ? "Yes" : "&nbsp;") + "</TD>"
                                  + "<TD>" + (getAllowedTime() == 0 ? "Unlimited" : getAllowedTime().ToString()) + "</TD>"
                                   + "</TR>");
            stringWriter.WriteLine("</TABLE>");
            stringWriter.WriteLine("</BODY>");
            stringWriter.WriteLine("</HTML>");
            if (hasConditions())
            {
                stringWriter.WriteLine("<BR><TABLE border=\"1\">");
                stringWriter.WriteLine("<thead><tr><th colspan=\"2\">Conditions</th></tr></thead>");
                foreach (Condition condition in getConditions())
                {
                    condition.getReport(stringWriter);
                }
                stringWriter.WriteLine("</TABLE>");
            }
            if (hasStages())
            {
                foreach (Stage stage in getStages())
                {
                    stringWriter.WriteLine("<BR><TABLE border=\"1\">");
                    stringWriter.WriteLine("<thead><tr><th colspan=\"2\">Stage " + stage.getId() + "</th></tr></thead>");
                    if (stage.getId().CompareTo("0") != 0)
                    {
                        stringWriter.WriteLine("<thead><tr><th colspan=\"2\">" + stage.getDescription() + "</th></tr></thead>");
                    }
                    stringWriter.WriteLine("<tbody>");
                    stage.getReport(stringWriter);
                    stringWriter.WriteLine("</tbody></TABLE>");
                }
            }
            return stringWriter.ToString();
        }

        public void addValidations()
        {
            if (getId().Length == 0)
            {
                DataConfiguration.addValidation("The mission Id is required.");
            }
            else if (getKey().Length == 0)
            {
                DataConfiguration.addValidation("The search key is mandatory.");
            }
            else if (getName().Length == 0)
            {
                DataConfiguration.addValidation("The mission name is mandatory.");
            }
            else if (getSummary().Length == 0)
            {
                DataConfiguration.addValidation("The mission summary is mandatory.");
            }
            else if (m_overallLevel != -1
                        && (m_overallLevel < 1 || m_overallLevel > 150))
            {
                DataConfiguration.addValidation("The overall level must be between 1 and 150");
            }
            else if (!hasConditions())
            {
                DataConfiguration.addValidation("Conditions are mandatory");
            }
            else if (!hasStages())
            {
                DataConfiguration.addValidation("Stages are mandatory");
            }
            else
            {
                // The basic validations have been performed

                // Validate the search key based on the type of search
                switch (getType())
                {
                    case CommonTools.MissionType.Npc:
                        DataConfiguration.addValidation(DataConfiguration.DataType.npc, getKey());
                        break;
                    case CommonTools.MissionType.Sector:
                        DataConfiguration.addValidation(DataConfiguration.DataType.sector, getKey());
                        break;
                }

                // Verify that the stages are numbered 0, 1, 2, 3, etc
                List<Stage> stageList = getStages();
                Int32 stageId;
                Stage stage;
                for (int stageIndex = 0; stageIndex < stageList.Count; stageIndex++)
                {
                    stage = stageList[stageIndex];
                    stageId = Int32.Parse(stage.getId());
                    if (stageId != stageIndex)
                    {
                        DataConfiguration.addValidation("Stage ID '" + stageIndex + "' is missing or out of sequence (Stage: " + stage.getId() + ")");
                        break;
                    }
                    stage.addValidations(Stage.ValidationType.Complete);
                }
            }
        }

        /// <summary>
        /// Verify every aspect of the mission
        /// </summary>
        public void addFullValidations()
        {
            addValidations();

            if (hasConditions())
            {
                foreach (Condition condition in getConditions())
                {
                    condition.addValidations();
                }
            }
            if (hasStages())
            {
                foreach (Stage stage in getStages())
                {
                    stage.addValidations(Stage.ValidationType.Complete);
                    if (stage.hasCompletions())
                    {
                        foreach (Completion completion in stage.getCompletions())
                        {
                            completion.addValidations();
                        }
                    }
                    if (stage.hasRewards())
                    {
                        foreach (Reward reward in stage.getRewards())
                        {
                            reward.addValidations();
                        }
                    }
                    if (stage.hasTalkTrees())
                    {
                        foreach (TalkTree talkTree in stage.getTalkTrees())
                        {
                            talkTree.addValidations();
                        }
                    }
                }

                // TODO: Validate that the mission ends/completes/terminates with a Talk Tree
                // with the option of Drop mission, Mission completed, Mission goto stage:
                // completed & repeatable, or Postpone
            }
        }

    }
}
