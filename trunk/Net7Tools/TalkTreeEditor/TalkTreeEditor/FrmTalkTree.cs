using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;
using TalkTreeEditor.Reply;
using CommonTools;
using CommonTools.Database;
using CommonTools.Gui;

namespace TalkTreeEditor
{
    public partial class FrmTalkTree : Form
    {
        private String m_conversation;
        private String[] m_replieTypes = new String[] { TalkNodeTypes.None.ToString(),
                                                        TalkNodeTypes.Branch.ToString(),
                                                        //TalkNodeTypes.Trade.ToString(), // Replaced with a Flag
                                                        TalkNodeTypes.Flags.ToString()};

        private List<Replies> m_replies;
        private List<Branch> m_replyBranch;
        private List<Trade> m_replyTrade;
        private List<Flag> m_replyFlag;
        private TreeNode m_currentTreeNode;
        private Stack<TreeNode> m_previousNodes;
        private Boolean m_muteFieldEvents = false;
        private List<CodeValue> m_stages;
        private Boolean m_madeSelection;
        private DlgEditXml m_dlgEditXml;

        public FrmTalkTree()
        {
            m_conversation = "";
            m_currentTreeNode = null;
            m_previousNodes = new Stack<TreeNode>();
            m_stages = null;
            m_dlgEditXml = null;
            InitializeComponent();
            initReplies();
        }

        private void initReplies()
        {
            m_replies = new List<Replies>();
            m_replyBranch = new List<Branch>();
            m_replyTrade = new List<Trade>();
            m_replyFlag = new List<Flag>();

            manageReply(guiReply0Cbo);
            manageReply(guiReply1Cbo);
            manageReply(guiReply2Cbo);
            manageReply(guiReply3Cbo);
        }

        private void manageReply(ComboBox cbo)
        {
            m_replies.Add(new Replies(null, cbo));
            cbo.SelectedValueChanged += new EventHandler(onReplyTypeSelected);
            cbo.Items.Clear();
            cbo.Items.AddRange(m_replieTypes);
            cbo.SelectedItem = cbo.Items[0];

            Reply.Branch branch = new Reply.Branch(this);
            positionReply(cbo, branch);
            m_replyBranch.Add(branch);

            Reply.Trade trade = new Reply.Trade();
            positionReply(cbo, trade);
            m_replyTrade.Add(trade);

            Reply.Flag flag = new Reply.Flag(this);
            positionReply(cbo, flag);
            m_replyFlag.Add(flag);
        }

        private void positionReply(ComboBox cbo, UserControl gui)
        {
            gui.CreateControl();
            this.Controls.Add(gui);
            Point location = cbo.Location;
            location.X += cbo.Size.Width + 2;
            gui.Location = location;
        }

        /// <summary>
        /// Set the stages that will be available for the "goto stage" flag
        /// </summary>
        /// <param name="stages"></param>
        public void setStages(List<CodeValue> stages)
        {
            m_stages = stages;
        }

        public List<CodeValue> getStages()
        {
            return m_stages;
        }

        public void setConversation(String conversation)
        {
            m_conversation = conversation;
        }

        public Boolean getConversation(out String conversation)
        {
            conversation = m_conversation;
            return m_madeSelection;
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            loadConversation(m_conversation);
            m_madeSelection = false;
        }

        /// <summary>
        /// Load an XML conversation from a String
        /// </summary>
        /// <param name="conversation">The string containing the conversation</param>
        public void loadConversation(String conversation)
        {
            guiTalkTree.Nodes.Clear();
            if (conversation == null)
            {
                // Creating a new talk tree; add an empty node
                addNodeText("1", "");
            }
            else
            {
                // Editing an existing talk tree
                try
                {
                    XmlDocument xmlDocument = new XmlDocument();
                    xmlDocument.Load(new StringReader("<Chat>" + conversation + "</Chat>"));

                    AddNodes(xmlDocument.DocumentElement);
                }
                catch (XmlException xmlEx)
                {
                    MessageBox.Show(xmlEx.Message);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
            if (guiTalkTree.Nodes.Count != 0)
            {
                guiTalkTree.SelectedNode = guiTalkTree.Nodes[0];
                guiTalkTree.ExpandAll();
            }
        }

        private void AddNodes(XmlNode xmlNode)
        {
            XmlNode xNode;
            XmlNodeList nodeList;
            int i;

            // The current XML always has a child node
            if (xmlNode.HasChildNodes)
            {
                String id;
                String text;
                TreeNode textNode = null;
                nodeList = xmlNode.ChildNodes;
                for (i = 0; i <= nodeList.Count - 1; i++)
                {
                    xNode = xmlNode.ChildNodes[i];
                    if (xNode.Name.Equals("Chat"))
                    {
                        // Ignore this node
                    }
                    else if (xNode.Name.Equals("Tree")) // Currenly only supports Tree children
                    {
                        id = xNode.Attributes[0].Value;
                        foreach (XmlNode treeChild in xNode.ChildNodes)
                        {
                            if (treeChild.Name.Equals("Text"))
                            {
                                text = treeChild.FirstChild.Value;
                                textNode = addNodeText(id, text);
                            }
                            else if (treeChild.Name.Equals("Trade"))
                            {
                                //id = "";
                                //addNodeTrade(textNode, id);
                                //Auto-convert to Flags
                                addNodeFlag(textNode, "", ((int)CommonTools.TalkTreeFlag.Trade).ToString());
                            }
                            else if (treeChild.Name.Equals("Branch"))
                            {
                                id = treeChild.Attributes[0].Value;
                                text = treeChild.FirstChild.Value;
                                addNodeBranch(textNode, id, text);
                            }
                            else if (treeChild.Name.Equals("Flags"))
                            {
                                id = (treeChild.Attributes.Count != 0) ? treeChild.Attributes[0].Value : "";
                                text = treeChild.FirstChild == null ? "" : treeChild.FirstChild.Value;
                                addNodeFlag(textNode, id, text);
                            }
                            else
                            {
                                MessageBox.Show("Unexpected child node of Tree: '" + treeChild.Name + "' in FrmTalkTree.AddNodes()");
                            }
                        }
                    }
                    else
                    {
                        MessageBox.Show("Unexpected node: " + xNode.Name);
                    }
                }
            }
            else
            {
                MessageBox.Show("Invalid XML structure for: " + xmlNode.Name + "\nShould not have a node without children");
                //inTreeNode.Text = (inXmlNode.OuterXml).Trim();
            }
        }

        private TreeNode addNodeText(String id, String text)
        {
            TalkNode talkNode = new TalkNode(null, TalkNodeTypes.None, id, text);
            TreeNode textNode = guiTalkTree.Nodes.Add(talkNode.ToString());
            textNode.Tag = talkNode;
            return textNode;
        }

        private Boolean canSetNodeToTrade(TreeNode node)
        {
            // Check that there isn't already a trade node
            if (node != null
                && node.Nodes.Count != 0
                && node.Nodes[0].Tag != null
                && ((TalkNode)node.Nodes[0].Tag).type.Equals(TalkNodeTypes.Trade))
            {
                MessageBox.Show("This node already contains a Trade sub-node");
                return false;
            }
            return true;
        }

        private void addNodeTrade(TreeNode textNode, String id)
        {
            TalkNode talkNode = new TalkNode(textNode, TalkNodeTypes.Trade, id, "");
            textNode.Nodes.Insert(0, new TreeNode(talkNode.ToString())); // Always shown first
            textNode.Nodes[0].Tag = talkNode;
        }

        private void addNodeBranch(TreeNode textNode, String id, String text)
        {
            TalkNode talkNode = new TalkNode(textNode, TalkNodeTypes.Branch, id, text);
            int childNode = textNode.Nodes.Add(new TreeNode(talkNode.ToString()));
            textNode.Nodes[childNode].Tag = talkNode;
        }

        private void addNodeFlag(TreeNode textNode, String id, String text)
        {
            TalkNode talkNode = new TalkNode(textNode, TalkNodeTypes.Flags, id, text);
            int childNode = textNode.Nodes.Add(new TreeNode(talkNode.ToString()));
            textNode.Nodes[childNode].Tag = talkNode;
        }

        /// <summary>
        /// Convert the information from the TreeView into an XML representation.
        /// This XML data is saved to a String rather than written to a file.
        /// </summary>
        private void saveConversation()
        {
            StringWriter stringWriter = new StringWriter();
            TalkNode talkNode;

            foreach (TreeNode node in guiTalkTree.Nodes)
            {
                talkNode = (TalkNode)node.Tag;
                stringWriter.WriteLine(Xml.tagStart() + XmlTag.TALKTREE + Xml.attribute(XmlAttributes.TREENODEID, talkNode.id) + Xml.tagEnd());
                stringWriter.WriteLine(Xml.tag(XmlTag.TEXT, talkNode.text));
                saveConversation(stringWriter, node);
                stringWriter.WriteLine(Xml.tagEnd(XmlTag.TALKTREE));
            }
            m_conversation = stringWriter.ToString();
        }

        private void saveConversation(StringWriter stringWriter, TreeNode treeNode)
        {
            TalkNode talkNode = (TalkNode) treeNode.Tag;
            if (!talkNode.type.Equals(TalkNodeTypes.None))
            {
                stringWriter.Write(Xml.tagStart() + talkNode.type.ToString());
                if (talkNode.type.Equals(TalkNodeTypes.Branch))
                {
                    stringWriter.Write(Xml.attribute(XmlAttributes.TREENODEID, talkNode.id));
                }
                else if (talkNode.type.Equals(TalkNodeTypes.Flags)
                        && talkNode.id.Length != 0)
                {
                    stringWriter.Write(Xml.attribute(XmlAttributes.DATA, talkNode.id));
                }
                stringWriter.WriteLine(Xml.tagEnd() + talkNode.text + Xml.tagEnd(talkNode.type.ToString()));
            }

            // Recurse the nodes
            foreach (TreeNode childTreeNode in treeNode.Nodes)
            {
                saveConversation(stringWriter, childTreeNode);
            }
        }

        /// <summary>
        /// A node in the tree has been selected. Move the highlight to the parent, the text
        /// and display the contents in the right-hand side fields.
        /// </summary>
        private void onTreeSelection(object sender, TreeViewEventArgs e)
        {
            m_muteFieldEvents = true;
            if (e.Node.Tag != null)
            {
                TalkNode talkNode = (TalkNode)e.Node.Tag;
                if (talkNode.parentNode != null)
                {
                    // A child of the text node was selected by the user.
                    // Change the selection to that of the text node.
                    guiTalkTree.SelectedNode = talkNode.parentNode;
                }
                else
                {
                    // A text node is selected, display the contents of these nodes
                    m_currentTreeNode = e.Node;
                    guiNpcText.Text = talkNode.text;
                    TreeNode childNode = e.Node.FirstNode;
                    TreeNode currentChild;
                    String id;
                    TalkNodeTypes type;
                    String text;
                    for (int childIndex = 0; childIndex < m_replies.Count; ++childIndex)
                    {
                        currentChild = childNode;
                        if (childNode == null)
                        {
                            id = "";
                            type = TalkNodeTypes.None;
                            text = "";
                        }
                        else
                        {
                            talkNode = (TalkNode)childNode.Tag;
                            id = talkNode.id;
                            type = talkNode.type;
                            text = talkNode.text;
                            childNode = childNode.NextNode;
                        }

                        m_replies[childIndex].treeNode = currentChild;
                        m_replies[childIndex].typeField.SelectedItem = type.ToString();
                    }
                    setFields(null);
                }
            }
            m_muteFieldEvents = false;
        }

        /// <summary>
        /// The type has been changed.  Find the appropriate reply and enable/disable its fields.
        /// </summary>
        private void onReplyTypeSelected(object sender, EventArgs e)
        {
            if(!m_muteFieldEvents)
            {
                // User is manually changing the field
                for (int childIndex = 0; childIndex < m_replies.Count; ++childIndex)
                {
                    if (sender == m_replies[childIndex].typeField)
                    {
                        TalkNodeTypes newType = (TalkNodeTypes)Enum.Parse(typeof(TalkNodeTypes), (String)m_replies[childIndex].typeField.SelectedItem);

                        TalkNodeTypes currentType = m_replies[childIndex].treeNode == null
                                                  ? TalkNodeTypes.None
                                                  : ((TalkNode)m_replies[childIndex].treeNode.Tag).type;
                        if (!currentType.Equals(newType))
                        {
                            // The type has been changed
                            if (newType.Equals(TalkNodeTypes.None))
                            {
                                // Delete this sub-node
                                TreeNode deletedNode = m_replies[childIndex].treeNode;
                                guiTalkTree.SelectedNode.Nodes.Remove(deletedNode);
                                refreshTree();
                                setFields(null);
                            }
                            else
                            {
                                if (newType.Equals(TalkNodeTypes.Trade)
                                    && !canSetNodeToTrade(guiTalkTree.SelectedNode))
                                {
                                    // Already contains a Trade sub-node so reset to previous type
                                    m_replies[childIndex].typeField.SelectedItem = currentType.ToString();
                                    setFields(sender);
                                    return;
                                }

                                if (currentType.Equals(TalkNodeTypes.None))
                                {
                                    // Create a sub-node
                                    if (newType.Equals(TalkNodeTypes.Trade))
                                    {
                                        addNodeTrade(guiTalkTree.SelectedNode, "");
                                    }
                                    else if (newType.Equals(TalkNodeTypes.Branch))
                                    {
                                        addNodeBranch(guiTalkTree.SelectedNode, "", "");
                                    }
                                    else if (newType.Equals(TalkNodeTypes.Flags))
                                    {
                                        addNodeFlag(guiTalkTree.SelectedNode, "", "");
                                    }
                                }
                                else
                                {
                                    // Change the node type
                                    TreeNode modifiedNode = m_replies[childIndex].treeNode;
                                    guiTalkTree.SelectedNode.Nodes.Remove(modifiedNode);
                                    if (newType.Equals(TalkNodeTypes.Trade))
                                    {
                                        addNodeTrade(guiTalkTree.SelectedNode, "");
                                    }
                                    else if (newType.Equals(TalkNodeTypes.Branch))
                                    {
                                        addNodeBranch(guiTalkTree.SelectedNode, "", "");
                                    }
                                    else if (newType.Equals(TalkNodeTypes.Flags))
                                    {
                                        addNodeFlag(guiTalkTree.SelectedNode, "", "");
                                    }
                                }
                                refreshTree();
                                setFields(sender);
                            }
                        }
                    }
                }
            }
        }

        public void refreshTree()
        {
            // Refresh the tree, which will refresh the fields
            TreeNode currentNode = guiTalkTree.SelectedNode;
            if (currentNode != null)
            {
                guiTalkTree.SelectedNode = null;
                guiTalkTree.SelectedNode = currentNode;
                guiTalkTree.SelectedNode.EnsureVisible();
                guiTalkTree.SelectedNode.ExpandAll();
                guiTalkTree.Focus();
            }
        }

        /// <summary>
        /// Display the reply fields based on the type of data
        /// </summary>
        /// <param name="sender"></param>
        private void setFields(object sender)
        {
            for (int childIndex = 0; childIndex < m_replies.Count; ++childIndex)
            {
                if (sender == null // Display every reply type
                    || sender == m_replies[childIndex].typeField) // Display this precise reply type
                {
                    m_replyTrade[childIndex].Visible = false;
                    m_replyBranch[childIndex].Visible = false;
                    m_replyFlag[childIndex].Visible = false;
                    TalkNodeTypes newType = (TalkNodeTypes)Enum.Parse(typeof(TalkNodeTypes), (String)m_replies[childIndex].typeField.SelectedItem);
                    switch (newType)
                    {
                        case TalkNodeTypes.None:
                            { 
                            }
                            break;
                        case TalkNodeTypes.Trade:
                            {
                                Reply.Trade gui = m_replyTrade[childIndex];
                                gui.Focus();
                                gui.Visible = true;
                                gui.setTreeNode(m_replies[childIndex].treeNode);
                            }
                            break;
                        case TalkNodeTypes.Branch:
                            {
                                Reply.Branch gui = m_replyBranch[childIndex];
                                gui.Focus();
                                gui.Visible = true;
                                gui.setTreeNode(m_replies[childIndex].treeNode);
                            }
                            break;
                        case TalkNodeTypes.Flags:
                            {
                                Reply.Flag gui = m_replyFlag[childIndex];
                                gui.Focus();
                                gui.Visible = true;
                                gui.setTreeNode(m_replies[childIndex].treeNode);
                                gui.UpdateData();
                            }
                            break;
                    }

                }
            }
        }

        private void onNodeAdd(object sender, EventArgs e)
        {
            int id = guiTalkTree.Nodes.Count + 1;

            TalkNode talkNode = new TalkNode(null, TalkNodeTypes.None, id.ToString(), "");
            TreeNode newNode = guiTalkTree.Nodes.Add(talkNode.ToString());
            newNode.Tag = talkNode;
            guiTalkTree.SelectedNode = newNode;
            guiTalkTree.Focus();
        }

        private void onNodeRemove(object sender, EventArgs e)
        {
            if (guiTalkTree.SelectedNode != null)
            {
                TalkNode talkNode = (TalkNode)guiTalkTree.SelectedNode.Tag;
                if (!talkNode.id.Equals("1"))
                {
                    // Cannot delete node 1
                    guiTalkTree.Nodes.Remove(guiTalkTree.SelectedNode);
                    if (guiTalkTree.Nodes.Count != 0)
                    {
                        guiTalkTree.SelectedNode = guiTalkTree.Nodes[0];
                        guiTalkTree.Focus();
                    }
                }
            }
        }

        private void onNpcTextChanged(object sender, EventArgs e)
        {
            if (!m_muteFieldEvents)
            {
                TextBox textField = (TextBox)sender;
                TalkNode talkNode = (TalkNode)m_currentTreeNode.Tag;
                talkNode.text = textField.Text;
                m_currentTreeNode.Text = m_currentTreeNode.Tag.ToString();
            }
        }

        private void onIdChanged(object sender, EventArgs e)
        {
            if (!m_muteFieldEvents)
            {
                /*TextBox idField = (TextBox)sender;
                foreach (Replies replyGroup in m_replies)
                {
                    if (replyGroup.idField == idField && replyGroup.treeNode != null)
                    {
                        TalkNode talkNode = (TalkNode)replyGroup.treeNode.Tag;
                        talkNode.id = idField.Text;
                        replyGroup.treeNode.Text = talkNode.ToString();
                        break;
                    }
                }*/
            }
        }

        private void onTextChanged(object sender, EventArgs e)
        {
            if (!m_muteFieldEvents)
            {
                /*TextBox textField = (TextBox)sender;
                foreach (Replies replyGroup in m_replies)
                {
                    if (replyGroup.textField == textField && replyGroup.treeNode != null)
                    {
                        TalkNode talkNode = (TalkNode)replyGroup.treeNode.Tag;
                        talkNode.text = textField.Text;
                        replyGroup.treeNode.Text = talkNode.ToString();
                        break;
                    }
                }*/
            }
        }

        public void onNodeGoto(String gotoNodeId)
        {
            TalkNode talkNode;
            foreach (TreeNode node in guiTalkTree.Nodes)
            {
                talkNode = (TalkNode)node.Tag;
                if (talkNode != null
                    && talkNode.id != null
                    && talkNode.id.Equals(gotoNodeId)
                    && talkNode.type.Equals(TalkNodeTypes.None))
                {
                    if (m_previousNodes.Count == 0)
                    {
                        guiReplyPreviousBtn.Enabled = true;
                    }
                    m_previousNodes.Push(guiTalkTree.SelectedNode);
                    guiTalkTree.SelectedNode = node;
                    guiTalkTree.Focus();
                    break;
                }
            }
        }

        private void onNodePrevious(object sender, EventArgs e)
        {
            if (m_previousNodes.Count != 0)
            {
                TreeNode previousNode = m_previousNodes.Pop();
                if (m_previousNodes.Count == 0)
                {
                    guiReplyPreviousBtn.Enabled = false;
                }
                guiTalkTree.SelectedNode = previousNode;
                refreshTree();
            }
        }

        private void onOk(object sender, EventArgs e)
        {
            String error;
            if (!validate(out error))
            {
                MessageBox.Show(error, "Validation Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            m_madeSelection = true;
            saveConversation();
            Close();
        }

        private void onCancel(object sender, EventArgs e)
        {
            Close();
        }

        class CheckBranch
        {
            public String npcTreeNodeId;
            public String branchToNodeId;
            public CheckBranch(String npc, String id)
            {
                npcTreeNodeId = npc;
                branchToNodeId = id;
            }
        }

        public Boolean validate(out String error)
        {
            // Verify that first node does not contain Trade
            // Check that each text field is non-empty
            // Verify that each node has a unique ID
            // Verify that each branch leads to a valid <Tree> node
            // TODO: Prune unused <Tree> nodes
            TalkNode npcTalkNode;
            TalkNode replyTalkNode;
            int nodeId = 1;
            error = "";
            List<String> idList = new List<string>();
            List<CheckBranch> branchList = new List<CheckBranch>();
            TalkTreeFlag talkTreeFlag = TalkTreeFlag.More;

            if (guiTalkTree.Nodes.Count == 0)
            {
                // Valid to have an empty talk tree since could be removing it
            }

            foreach (TreeNode npcTreeNode in guiTalkTree.Nodes)
            {
                npcTalkNode = (TalkNode)npcTreeNode.Tag;

                if (idList.Contains(npcTalkNode.id))
                {
                    error = "The node id '" + npcTalkNode.id + "' is present more than once";
                    return false;
                }
                else
                {
                    idList.Add(npcTalkNode.id);
                }

                if (nodeId == 1)
                {
                    if (!npcTalkNode.id.Equals("1"))
                    {
                        error = "The first node has an id of '" + npcTalkNode.id + "' but was expected to have the id '1'";
                        return false;
                    }
                    else
                    {
                        foreach (TreeNode replyTreeNode in npcTreeNode.Nodes)
                        {
                            replyTalkNode = (TalkNode)replyTreeNode.Tag;
                            if(replyTalkNode.type.Equals(TalkNodeTypes.Flags))
                            {
                                if (!Enumeration.TryParse<TalkTreeFlag>(replyTalkNode.text, out talkTreeFlag))
                                {
                                    // If cannot parse the value then put it to something different than Trade
                                    // to avoid erronously reporting an error here.  The parse error will be
                                    // reported below.
                                    talkTreeFlag = TalkTreeFlag.More;
                                }
                            }
                            if (replyTalkNode.type.Equals(TalkNodeTypes.Trade)
                                || (replyTalkNode.type.Equals(TalkNodeTypes.Flags) && talkTreeFlag.Equals(TalkTreeFlag.Trade)))
                            {
                                error = "The first node cannot specify a trade action";
                                return false;
                            }
                        }
                    }
                } // First node

                if (npcTalkNode.text.Length == 0)
                {
                    error = "The node '" + npcTalkNode.id + "' does not specify any NPC text";
                    return false;
                }

                if (npcTreeNode.Nodes.Count == 0)
                {
                    error = "The node '" + npcTalkNode.id + "' does not specify any reply";
                    return false;
                }

                foreach (TreeNode childTreeNode in npcTreeNode.Nodes)
                {
                    replyTalkNode = (TalkNode)childTreeNode.Tag;
                    switch(replyTalkNode.type)
                    {
                        case TalkNodeTypes.None:
                            break;
                        case TalkNodeTypes.Branch:
                            if (npcTalkNode.id.Equals(replyTalkNode.id))
                            {
                                error = "Cannot branch to the same/current node (Node: " + npcTalkNode.id + ")";
                                return false;
                            }
                            branchList.Add(new CheckBranch(npcTalkNode.id, replyTalkNode.id));
                            if (replyTalkNode.text.Length == 0)
                            {
                                error = "A branch does not specify any text (Node: " + npcTalkNode.id + ")";
                                return false;
                            }
                            break;
                        case TalkNodeTypes.Trade:
                            break;
                        case TalkNodeTypes.Flags:
                            {
                                if (!Enumeration.TryParse<TalkTreeFlag>(replyTalkNode.text, out talkTreeFlag))
                                {
                                    error = "The flag value '" + replyTalkNode.text + "' cannot be converted to a TalkTreeFlag value (Node: " + npcTalkNode.id + ")";
                                    return false;
                                }
                                else
                                {
                                    switch (talkTreeFlag)
                                    {
                                        case TalkTreeFlag.Mission_Goto_Stage:
                                            if (replyTalkNode.id.Equals("0"))
                                            {
                                                error = "Cannot go back to stage 0 (Node: " + npcTalkNode.id + ")";
                                                return false;
                                            }
                                            else if (replyTalkNode.id.Equals("-2"))
                                            {
                                                // This is not an error but is needed to prevent
                                                // the next validation from reporting an error
                                            }
                                            else
                                            {
                                                error = "The " + TalkTreeFlag.Mission_Goto_Stage.ToString() + " flag points to an invalid stage ID of '" + replyTalkNode.id + "' (Node: " + npcTalkNode.id + ")";
                                                if (m_stages != null)
                                                {
                                                    foreach (CodeValue stage in m_stages)
                                                    {
                                                        if (stage.code.ToString().Equals(replyTalkNode.id))
                                                        {
                                                            error = "";
                                                            break;
                                                        }
                                                    }
                                                }
                                                if (error.Length != 0)
                                                {
                                                    return false;
                                                }
                                            }
                                            break;
                                    }
                                }
                            } // case TalkNodeTypes.Flags
                            break;
                    } // switch(replyTalkNode.type)
                    ++nodeId;
                } // foreach (TreeNode childTreeNode in npcTreeNode.Nodes)

            } // foreach NPC node


            // Validate that each branch has a valid destination
            foreach (CheckBranch checkGoto in branchList)
            {
                if (!idList.Contains(checkGoto.branchToNodeId))
                {
                    error = "The branch directs to an invalid node id '" + checkGoto.branchToNodeId + "' (Node: " + checkGoto.npcTreeNodeId + ")";
                    return false;
                }
            }

            // Validate that each npc node is accessed
            branchList.Add(new CheckBranch("1", "1")); // Node 1 has an implicit branch-to access
            foreach (CheckBranch checkBranch in branchList)
            {
                idList.Remove(checkBranch.branchToNodeId);
            }
            if (idList.Count != 0)
            {
                error = "Node " + idList[0] + " cannot be accessed";
                return false;
            }

            return true;
        } // validate()

        private void onEditXml(object sender, EventArgs e)
        {
            if (m_dlgEditXml == null)
            {
                m_dlgEditXml = new DlgEditXml();
            }
            String conversation = m_conversation; // Keep original conversation intact
            saveConversation(); // Store GUI conversation into m_conversation
            m_dlgEditXml.setXml(m_conversation); // Send GUI conversation to dialog
            setConversation(conversation); // Restore original m_conversation
            m_dlgEditXml.ShowDialog();
            if (m_dlgEditXml.getValues(out conversation))
            {
                // GUI conversation was modified; load it
                loadConversation(conversation);
            }
        } 

    }
}