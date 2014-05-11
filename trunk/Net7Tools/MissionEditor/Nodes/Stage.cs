using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;
using CommonTools;
using System.Windows.Forms;
using MissionEditor.Database;

namespace MissionEditor.Nodes
{
    public class Stage
    {
        public enum ValidationType { FromDialog, Complete };
        public const int m_maxStages = 20;
        private String m_id;
        private String m_description;
        private List<Completion> m_completions;
        private List<Reward> m_rewards;
        private List<TalkTree> m_talkTrees;

        public Stage()
        {
            clear();
        }

        public void clear()
        {
            m_id = "";
            m_description = "";
            if (m_completions != null
                && m_completions.Count != 0)
            {
                m_completions.Clear();
            }
            m_completions = null;
            if (m_rewards != null
                && m_rewards.Count != 0)
            {
                m_rewards.Clear();
            }
            m_rewards = null;
            if (m_talkTrees != null
                && m_talkTrees.Count != 0)
            {
                m_talkTrees.Clear();
            }
            m_talkTrees = null;
        }

        public override string ToString()
        {
            return getId() + "." + getDescription();
        }

        public void setId(String id)
        {
            m_id = id;
        }

        public String getId()
        {
            return m_id;
        }

        public void setDescription(String description)
        {
            m_description = description;
        }

        public String getDescription()
        {
            return m_description;
        }

        public void addCompletion(Completion completion)
        {
            if (m_completions == null)
            {
                m_completions = new List<Completion>();
            }
            if (CheckBadCompletionDuplicates(completion))
            {
                m_completions.Add(completion);
            }
        }

        //TB to Rackle - is this the best way to do this?
        public Boolean CheckBadCompletionDuplicates(Completion completion)
        {
            switch (completion.getCompletionType())
            {
                case CompletionType.Nearest_Nav:
                    //there can be only one ... Nearest Nav
                    if (completion.contains(m_completions, CompletionType.Nearest_Nav))
                    {
                        //already contains a Nearest_Nav, don't allow another
                        DataConfiguration.addValidation("You can only have one Nearest_Nav per stage completion.");
                        return false;
                    }
                    break;
                default:
                    break;
            }
            return true;
        }

        public Boolean hasCompletions()
        {
            return m_completions != null && m_completions.Count != 0;
        }

        public List<Completion> getCompletions()
        {
            return m_completions;
        }

        public void removeCompletion(Completion completion)
        {
            if (m_completions != null)
            {
                m_completions.Remove(completion);
            }
        }

        public void clearCompletions()
        {
            if (m_completions != null)
            {
                m_completions.Clear();
            }
        }

        public void addReward(Reward reward)
        {
            if (m_rewards == null)
            {
                m_rewards = new List<Reward>();
            }
            m_rewards.Add(reward);
        }

        public Boolean hasRewards()
        {
            return m_rewards != null && m_rewards.Count != 0;
        }

        public List<Reward> getRewards()
        {
            return m_rewards;
        }

        public void removeReward(Reward reward)
        {
            if (m_rewards != null)
            {
                m_rewards.Remove(reward);
            }
        }

        public void clearRewards()
        {
            if (m_rewards != null)
            {
                m_rewards.Clear();
            }
        }

        public void addTalkTree(TalkTree talkTree)
        {
            if(m_talkTrees == null)
            {
                m_talkTrees = new List<TalkTree>();
            }
            m_talkTrees.Add(talkTree);
        }

        public void clearTalkTrees()
        {
            if (m_talkTrees != null)
            {
                m_talkTrees.Clear();
            }
        }

        public Boolean hasTalkTrees()
        {
            return m_talkTrees != null && m_talkTrees.Count != 0;
        }

        public List<TalkTree> getTalkTrees()
        {
            return m_talkTrees;
        }

        public void getTalkTreesXML(StringWriter stringWriter)
        {
            if (hasTalkTrees())
            {
                foreach (TalkTree talkTree in m_talkTrees)
                {
                    talkTree.getXML(stringWriter);
                }
            }
        }

        /// <summary>
        /// Parse the XML nodes
        /// </summary>
        /// <param name="xmlNode">The <Stage></Stage> nodes</param>
        public void fromXml(XmlNode xmlNode)
        {
            String value;
            Xml.getAttribute(xmlNode, XmlAttributes.ID, true, out value);
            setId(value);

            foreach (XmlNode stageChildNode in xmlNode.ChildNodes)
            {
                if (stageChildNode.Name.Equals(XmlTag.DESCRIPTION))
                {
                    Xml.getValue(stageChildNode, !getId().Equals("0"), out value); // Optional for stage 0, required for others
                    setDescription(value);
                }
                else if (stageChildNode.Name.Equals(XmlTag.COMPLETION))
                {
                    Completion completion = new Completion();
                    completion.fromXml(stageChildNode);
                    addCompletion(completion);
                }
                else if (stageChildNode.Name.Equals(XmlTag.REWARD))
                {
                    Reward reward = new Reward();
                    reward.fromXml(stageChildNode);
                    addReward(reward);
                }
                else if (stageChildNode.Name.Equals(XmlTag.TALKTREE))
                {
                    TalkTree talkTree = new TalkTree();
                    talkTree.fromXML(stageChildNode);
                    addTalkTree(talkTree);
                }
                else
                {
                    MessageBox.Show("Unexpected node: " + stageChildNode.Name + " in the" + XmlTag.STAGE + " tag");
                }
            }

            addValidations(ValidationType.Complete);
            /*if (!valid(ValidationType.Complete, out value))
            {
                throw (new Exception(value + "\n\n" + xmlNode.InnerXml));
            }*/
        }

        public void getXML(StringWriter stringWriter)
        {
            stringWriter.WriteLine("<Stage ID=\"" + m_id + "\">");
            if (m_description != null && m_description.Length != 0)
            {
                stringWriter.WriteLine("<Description>" + m_description + "</Description>");
            }
            if (hasCompletions())
            {
                foreach (Completion completion in getCompletions())
                {
                    completion.getXML(stringWriter);
                }
            }
            if (hasTalkTrees())
            {
                foreach (TalkTree talkTree in getTalkTrees())
                {
                    talkTree.getXML(stringWriter);
                }
            }
            if (hasRewards())
            {
                foreach (Reward reward in getRewards())
                {
                    reward.getXML(stringWriter);
                }
            }
            stringWriter.WriteLine("</Stage>");
        }

        public void getReport(StringWriter stringWriter)
        {
            if (hasCompletions())
            {
                stringWriter.WriteLine("<tr><td colspan=\"2\"><BR><B>Completions<B></td></tr>");
                foreach (Completion completion in getCompletions())
                {
                    completion.getReport(stringWriter);
                }
            }
            if (hasRewards())
            {
                stringWriter.WriteLine("<tr><td colspan=\"2\"><BR><B>Rewards<B></td></tr>");
                foreach (Reward reward in getRewards())
                {
                    reward.getReport(stringWriter);
                }
            }
            if (hasTalkTrees())
            {
                stringWriter.WriteLine("<tr><td colspan=\"2\"><BR><B>Talk Tree<B></td></tr>");
                foreach (TalkTree talkTree in getTalkTrees())
                {
                    talkTree.getReport(stringWriter);
                }
            }
        }

        /// <summary>
        /// Add the Stage validations
        /// </summary>
        /// <param name="validationType">The type of validation to perform. 
        ///     Only the dialog should use the FromDialog type.</param>
        public void addValidations(ValidationType validationType)
        {
            Int32 stageId;
            if (!Int32.TryParse(getId(), out stageId)
                || stageId < 0 || stageId >= m_maxStages)
            {
                DataConfiguration.addValidation("The stage ID must be within 0 and " + (m_maxStages - 1).ToString());
            }
            else if (stageId != 0 && getDescription().Length == 0)
            {
                DataConfiguration.addValidation("The stage description must be specified (Stage: " + getId() + ")");
            }

            if (validationType.Equals(ValidationType.Complete))
            {
                if (!hasCompletions())
                {
                    DataConfiguration.addValidation("Stage completions are mandatory (Stage: " + getId() + ")");
                }
                else
                {
                    int actionCount;
                    int totalActionCount = 0;
                    foreach (Completion completion in getCompletions())
                    {
                        switch (completion.getCompletionType())
                        {
                            case CompletionType.Fight_Mob:
                            case CompletionType.Use_Skill_On_Mob_Type:
                            case CompletionType.Use_Skill_On_Object:
                                actionCount = completion.getCount();
                                if (actionCount == -1)
                                {
                                    // If there is no count then it "counts" as one
                                    actionCount = 1;
                                }
                                totalActionCount += actionCount;
                                break;
                        }
                    }
                    if (totalActionCount >= 32)
                    {
                        DataConfiguration.addValidation("There cannot be more than 32 actions in a stage (Stage: " + getId() + ")");
                    }

                    // Verify CompletionType interdependencies
                    /*if (Completion.contains(getCompletions(), CompletionType.Take_Item_To_Location)
                        && !Completion.contains(getCompletions(), CompletionType.Nearest_Nav))
                    {
                        DataConfiguration.addValidation("The type '"
                                                            + CompletionType.Take_Item_To_Location.ToString()
                                                            + "' requires the '"
                                                            + CompletionType.Nearest_Nav.ToString()
                                                            + "' type (Stage: " + getId() + ").");
                    }*/
                }
            } // ValidationType.Complete
        }

    }
}
