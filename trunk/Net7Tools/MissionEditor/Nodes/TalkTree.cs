using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using CommonTools;
using System.Xml;

namespace MissionEditor.Nodes
{
    public class TalkTree
    {
        public class Branch
        {
            public String gotoNode;
            public String text;

            public Branch()
            {
                gotoNode = "";
                text = "";
            }

            public void getXML(StringWriter stringWriter)
            {
                stringWriter.WriteLine(Xml.tagStart()
                                     + XmlTag.BRANCH
                                     + Xml.attribute(XmlAttributes.TREENODEID, gotoNode)
                                     + Xml.tagEnd()
                                     + text
                                     + Xml.tagEnd(XmlTag.BRANCH));
            }
        }

        public class Flag
        {
            public String data;
            public TalkTreeFlag value;

            public Flag()
            {
                data = "";
            }

            public void getXML(StringWriter stringWriter)
            {
                stringWriter.Write(Xml.tagStart() + XmlAttributes.FLAGS);
                if (data.Length != 0)
                {
                    stringWriter.Write(Xml.attribute(XmlAttributes.DATA, data));
                }
                stringWriter.WriteLine(Xml.tagEnd()
                                       + ((int)value).ToString()
                                       + Xml.tagEnd(XmlAttributes.FLAGS));
            }
        }

        private String m_nodeId;
        private String m_npcText;
        private List<Branch> m_pcReplies;
        private Flag m_flag;

        public TalkTree()
        {
            m_nodeId = "";
            m_npcText = "";
            m_pcReplies = null;
            m_flag = null;
        }

        public void setNodeId(String nodeId)
        {
            m_nodeId = nodeId;
        }

        public String getNodeId()
        {
            return m_nodeId;
        }

        public void setNpcText(String npcText)
        {
            m_npcText = npcText;
        }

        public String getNpcText()
        {
            return m_npcText;
        }

        public void addPcReply(Branch branch)
        {
            if (m_pcReplies == null)
            {
                m_pcReplies = new List<Branch>();
            }
            m_pcReplies.Add(branch);
        }

        public Boolean hasReplies()
        {
            return m_pcReplies != null && m_pcReplies.Count != 0;
        }

        public List<Branch> getPcReplies()
        {
            return m_pcReplies;
        }

        public void setFlag(Flag flag)
        {
            m_flag = flag;
        }

        public Boolean hasFlag()
        {
            return m_flag != null;
        }

        public Flag getFlag()
        {
            return m_flag;
        }

        public void getXML(StringWriter stringWriter)
        {
            if (m_nodeId.Length != 0)
            {
                stringWriter.WriteLine(Xml.tagStart()
                                     + XmlTag.TALKTREE
                                     + Xml.attribute(XmlAttributes.TREENODEID, m_nodeId)
                                     + Xml.tagEnd());
                if (m_npcText != null && m_npcText.Length != 0)
                {
                    stringWriter.WriteLine(Xml.tag(XmlTag.TEXT, m_npcText));
                }
                if (hasReplies())
                {
                    foreach (Branch branch in m_pcReplies)
                    {
                        branch.getXML(stringWriter);
                    }
                }
                if (hasFlag())
                {
                    m_flag.getXML(stringWriter);
                }
                stringWriter.WriteLine(Xml.tagEnd(XmlTag.TALKTREE));
            }
        }

        /// <summary>
        /// Parse the XML nodes
        /// </summary>
        /// <param name="xmlNode">The <Tree></Tree> nodes</param>
        public void fromXML(XmlNode xmlNode)
        {
            setNodeId(xmlNode.Attributes[XmlAttributes.TREENODEID].Value);
            foreach (XmlNode talkTreeNode in xmlNode.ChildNodes)
            {
                if (talkTreeNode.Name.Equals(XmlTag.TEXT))
                {
                    setNpcText(talkTreeNode.FirstChild.Value);
                }
                else if (talkTreeNode.Name.Equals(XmlTag.BRANCH))
                {
                    TalkTree.Branch branch = new TalkTree.Branch();
                    branch.gotoNode = talkTreeNode.Attributes[XmlAttributes.TREENODEID].Value;
                    branch.text = talkTreeNode.FirstChild.Value;
                    addPcReply(branch);
                }
                else if (talkTreeNode.Name.Equals(XmlAttributes.FLAGS))
                {
                    TalkTree.Flag flag = new TalkTree.Flag();
                    foreach (XmlAttribute attribute in talkTreeNode.Attributes)
                    {
                        if (attribute.Name.Equals(XmlAttributes.DATA))
                        {
                            flag.data = attribute.Value;
                        }
                    }
                    flag.value = (CommonTools.TalkTreeFlag)Enum.Parse(typeof(CommonTools.TalkTreeFlag), talkTreeNode.FirstChild.Value);
                    setFlag(flag);
                }
            }
        }

        public void addValidations()
        {
            
        }

        public Boolean valid(out String error)
        {
            error = "";
            return error.Length == 0;
        }


        internal void getReport(StringWriter stringWriter)
        {
            if (m_nodeId.Length != 0)
            {
                stringWriter.WriteLine("<tr><td></td><td><TABLE>");
                stringWriter.WriteLine("<TR><th colspan=\"3\">" + m_nodeId + ". " + m_npcText + "</th></tr>");
                if (hasReplies())
                {
                    foreach (Branch branch in m_pcReplies)
                    {
                        stringWriter.Write("<TR><td>&nbsp;</td>");
                        stringWriter.Write("<td>" + branch.gotoNode + "</td>");
                        stringWriter.WriteLine("<td>" + branch.text + "</td></tr>");
                   }
                }
                if (hasFlag())
                {
                    stringWriter.Write("<TR><td>&nbsp;</td>");
                    stringWriter.Write("<td>" + m_flag.value.ToString() + "</td>");
                    stringWriter.WriteLine("<td>"
                                         + (m_flag.data == null || m_flag.data.Length == 0 ? "&nbsp;" : m_flag.data)
                                         + "</td></tr>");
                }
                stringWriter.WriteLine("</TABLE></td></tr>");

            }
        }

    }
}
