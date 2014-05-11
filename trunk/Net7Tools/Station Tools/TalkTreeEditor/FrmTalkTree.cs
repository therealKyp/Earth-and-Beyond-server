using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;

namespace TalkTreeEditor
{
    public partial class FrmTalkTree : Form
    {
        private String m_conversation;
        private String[] m_replieTypes = new String[] { TalkNodeTypes.None.ToString(),
                                                        TalkNodeTypes.Branch.ToString(),
                                                        TalkNodeTypes.Trade.ToString() };

        private List<Replies> m_replies;
        private TreeNode m_currentTreeNode;
        private Stack<TreeNode> m_previousNodes;
        private Boolean m_muteFieldEvents = false;
        private Boolean m_displayField = false;

        private const int m_MaxText = 2048;

        public FrmTalkTree()
        {
            m_conversation = "";
            m_currentTreeNode = null;
            m_previousNodes = new Stack<TreeNode>();
            InitializeComponent();
            initReplies();
        }

        private void initReplies()
        {
            m_replies = new List<Replies>();
            addReply(reply0Cbo, replyId0Txt, reply0Txt, reply0Btn);
            addReply(reply1Cbo, replyId1Txt, reply1Txt, reply1Btn);
            addReply(reply2Cbo, replyId2Txt, reply2Txt, reply2Btn);
            addReply(reply3Cbo, replyId3Txt, reply3Txt, reply3Btn);
        }

        private void addReply(ComboBox cbo, TextBox textId, TextBox text, Button btn)
        {
            m_replies.Add(new Replies(null, cbo, textId, text, btn));
            cbo.SelectedValueChanged += new EventHandler(onReplyType);
            cbo.Items.Clear();
            cbo.Items.AddRange(m_replieTypes);
            cbo.SelectedItem = cbo.Items[0];
        }

        public void setConversation(String conversation)
        {
            m_conversation = conversation;
        }

        public String getConversation()
        {
            return m_conversation;
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            loadConversation(m_conversation);
        }

        /// <summary>
        /// Load an XML conversation from a String
        /// </summary>
        /// <param name="conversation">The string containing the conversation</param>
        public void loadConversation(String conversation)
        {
            try
            {
                XmlDocument xmlDocument = new XmlDocument();
                m_conversation = conversation;
                if (conversation != "Enter Talk tree Handler")
                {
                    xmlDocument.Load(new StringReader("<Chat>" + conversation + "</Chat>"));

                    talkTree.Nodes.Clear();
                    AddNodes(xmlDocument.DocumentElement);
                    if (xmlDocument.HasChildNodes)
                    {
                        talkTree.SelectedNode = talkTree.Nodes[0];
                        talkTree.ExpandAll();
                    }
                }
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
                                if (text.Length > m_MaxText)
                                {
                                    text = text.Substring(0, m_MaxText);
                                }
                                textNode = addNodeText(id, text);
                            }
                            else if (treeChild.Name.Equals("Trade"))
                            {
                                id = treeChild.FirstChild.Value;
                                addNodeTrade(textNode, id);
                            }
                            else if (treeChild.Name.Equals("Branch"))
                            {
                                id = treeChild.Attributes[0].Value;
                                text = treeChild.FirstChild.Value;
                                addNodeBranch(textNode, id, text);
                            }
                            else
                            {
                                MessageBox.Show("Unexpected child node of Tree: " + treeChild.Name);
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
            TreeNode textNode = talkTree.Nodes.Add(talkNode.ToString());
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
            if (text.Length > m_MaxText)
            {
                text = text.Substring(0, m_MaxText);
            }
            TalkNode talkNode = new TalkNode(textNode, TalkNodeTypes.Branch, id, text);
            int childNode = textNode.Nodes.Add(new TreeNode(talkNode.ToString()));
            textNode.Nodes[childNode].Tag = talkNode;
        }

        /// <summary>
        /// Convert the information from the TreeView into an XML representation.
        /// This XML data is saved to a String rather than written to a file.
        /// </summary>
        private void saveConversation()
        {
            
            // TODO: Check that each text field is non-empty
            // TODO: Verify that each branch leads to a valid <Tree> node
            // TODO: Prune unused <Tree> nodes
            StringWriter stringWriter = new StringWriter();
            TalkNode talkNode;

            int []TreeID = new int[64];

            // 0 out all data
            for (int x = 0; x < 64; x++)
                TreeID[x] = 0;

            foreach (TreeNode node in talkTree.Nodes)
            {
                talkNode = (TalkNode)node.Tag;

                // See if our ID's are unique
                if (TreeID[Int32.Parse(talkNode.id)] == 1)
                {
                    MessageBox.Show("Error: Duplocate TreeID! Adding 1 to this!");
                    int TID = Int32.Parse(talkNode.id) + 1;
                    talkNode.id = TID.ToString();
                }
                TreeID[Int32.Parse(talkNode.id)] = 1;

                // Give warning about node 1 and trade
                if (talkNode.id == "1")
                {
                    // Recurse the nodes
                    foreach (TreeNode childTreeNode in node.Nodes)
                    {
                        TalkNode tcNode = (TalkNode)childTreeNode.Tag;
                        if (tcNode.type.Equals(TalkNodeTypes.Trade))
                        {
                            MessageBox.Show("Warning! Trade should not be in Node 1!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        }
                    }
                }

                stringWriter.WriteLine("<Tree Node=\"" + talkNode.id + "\">");
                stringWriter.WriteLine("\t<Text>" + talkNode.text + "</Text>");
                saveConversation(stringWriter, node);
                stringWriter.WriteLine("</Tree>");
            
            }

            if (stringWriter.ToString().Length > 0)
            {
                m_conversation = stringWriter.ToString();
            }
        }

        private void saveConversation(StringWriter stringWriter, TreeNode treeNode)
        {
            TalkNode talkNode = (TalkNode) treeNode.Tag;
            if (!talkNode.type.Equals(TalkNodeTypes.None))
            {
                stringWriter.Write("\t<" + talkNode.type.ToString());
                if (talkNode.type.Equals(TalkNodeTypes.Branch))
                {
                    stringWriter.Write(" Node=\"" + talkNode.id + "\"");
                }
                if (talkNode.type.Equals(TalkNodeTypes.Trade))
                {
                    stringWriter.WriteLine(">" + talkNode.id + "</" + talkNode.type.ToString() + ">");
                }
                else
                {
                    stringWriter.WriteLine(">" + talkNode.text + "</" + talkNode.type.ToString() + ">");
                }
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
                    talkTree.SelectedNode = talkNode.parentNode;
                }
                else
                {
                    // A text node is selected, display the contents of these nodes
                    m_displayField = true;
                    m_currentTreeNode = e.Node;
                    npcText.Text = talkNode.text;
                    TreeNode children = e.Node.FirstNode;
                    TreeNode currentNode;
                    String id;
                    TalkNodeTypes type;
                    String text;
                    for (int childIndex = 0; childIndex < m_replies.Count; ++childIndex)
                    {
                        currentNode = children;
                        if (children == null)
                        {
                            id = "";
                            type = TalkNodeTypes.None;
                            text = "";
                        }
                        else
                        {
                            talkNode = (TalkNode)children.Tag;
                            id = talkNode.id;
                            type = talkNode.type;
                            text = talkNode.text;
                            children = children.NextNode;
                        }
                        m_replies[childIndex].treeNode = currentNode;
                        m_replies[childIndex].typeField.SelectedItem = type.ToString();
                        m_replies[childIndex].idField.Text = id;
                        m_replies[childIndex].textField.Text = text;
                    }
                    m_displayField = false;
                }
            }
            m_muteFieldEvents = false;
        }

        /// <summary>
        /// The type has been changed.  Find the appropriate reply and enable/disable its fields.
        /// </summary>
        private void onReplyType(object sender, EventArgs e)
        {
            if (m_displayField)
            {
                setFields(null);
            }
            else
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
                                talkTree.SelectedNode.Nodes.Remove(deletedNode);
                                refreshTree();
                                setFields(null);
                            }
                            else
                            {
                                if (newType.Equals(TalkNodeTypes.Trade)
                                    && !canSetNodeToTrade(talkTree.SelectedNode))
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
                                        addNodeTrade(talkTree.SelectedNode, "1");
                                    }
                                    else
                                    {
                                        addNodeBranch(talkTree.SelectedNode, "", "");
                                    }
                                    setFields(sender);
                                    refreshTree();
                                }
                                else
                                {
                                    TreeNode modifiedNode = m_replies[childIndex].treeNode;
                                    TalkNode talkNode = (TalkNode)modifiedNode.Tag;
                                    talkNode.type = newType;
                                    if (newType.Equals(TalkNodeTypes.Trade))
                                    {
                                        talkTree.SelectedNode.Nodes.Remove(modifiedNode);
                                        addNodeTrade(talkTree.SelectedNode, "");
                                    }
                                    refreshTree();
                                    setFields(sender);
                                    modifiedNode.Text = talkNode.ToString();
                                }
                            }
                        }
                    }
                }
            }
        }

        private void refreshTree()
        {
            // Refresh the tree, which will refresh the fields
            TreeNode currentNode = talkTree.SelectedNode;
            talkTree.SelectedNode = null;
            talkTree.SelectedNode = currentNode;
            talkTree.SelectedNode.EnsureVisible();
            talkTree.SelectedNode.ExpandAll();
            talkTree.Focus();
        }

        private void setFields(object sender)
        {
            for (int childIndex = 0; childIndex < m_replies.Count; ++childIndex)
            {
                if (sender == null || sender == m_replies[childIndex].typeField)
                {
                    TalkNodeTypes newType = (TalkNodeTypes)Enum.Parse(typeof(TalkNodeTypes), (String)m_replies[childIndex].typeField.SelectedItem);
                    switch (newType)
                    {
                        case TalkNodeTypes.None:
                            m_replies[childIndex].idField.Enabled = false;
                            m_replies[childIndex].idField.Text = "";
                            m_replies[childIndex].textField.Enabled = false;
                            m_replies[childIndex].textField.Text = "";
                            m_replies[childIndex].selectField.Enabled = false;
                            break;
                        case TalkNodeTypes.Trade:
                            m_replies[childIndex].idField.Enabled = true;
                            m_replies[childIndex].textField.Enabled = false;
                            m_replies[childIndex].textField.Text = "";
                            m_replies[childIndex].selectField.Enabled = false;
                            break;
                        case TalkNodeTypes.Branch:
                            m_replies[childIndex].idField.Enabled = true;
                            m_replies[childIndex].textField.Enabled = true;
                            m_replies[childIndex].selectField.Enabled = true;
                            break;
                    }
                }
            }
        }

        private void okBtn_Click(object sender, EventArgs e)
        {
            saveConversation();
            Close();
        }

        private void cancelBtn_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void onIdChange(object sender, EventArgs e)
        {
            if (!m_muteFieldEvents)
            {
                TextBox idField = (TextBox)sender;
                foreach (Replies replyGroup in m_replies)
                {
                    if (replyGroup.idField == idField && replyGroup.treeNode != null)
                    {
                        TalkNode talkNode = (TalkNode)replyGroup.treeNode.Tag;
                        talkNode.id = idField.Text;
                        replyGroup.treeNode.Text = talkNode.ToString();
                        break;
                    }
                }
            }
        }

        private void onTextChanged(object sender, EventArgs e)
        {
            if (!m_muteFieldEvents)
            {
                TextBox textField = (TextBox)sender;
                foreach (Replies replyGroup in m_replies)
                {
                    if (replyGroup.textField == textField && replyGroup.treeNode != null)
                    {
                        TalkNode talkNode = (TalkNode)replyGroup.treeNode.Tag;
                        talkNode.text = textField.Text;
                        replyGroup.treeNode.Text = talkNode.ToString();
                        break;
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

        private void onAddNode(object sender, EventArgs e)
        {
            int id = talkTree.Nodes.Count + 1;

            TalkNode talkNode = new TalkNode(null, TalkNodeTypes.None, id.ToString(), "");
            TreeNode newNode = talkTree.Nodes.Add(talkNode.ToString());
            newNode.Tag = talkNode;
            talkTree.SelectedNode = newNode;
            talkTree.Focus();
            setFields(null);
            refreshTree();
        }

        private void onRemoveNode(object sender, EventArgs e)
        {
            talkTree.Nodes.Remove(talkTree.SelectedNode);
            if (talkTree.Nodes.Count != 0)
            {
                talkTree.SelectedNode = talkTree.Nodes[0];
                talkTree.Focus();
            }
        }

        private void onGotoNode(object sender, EventArgs e)
        {
            for (int childIndex = 0; childIndex < m_replies.Count; ++childIndex)
            {
                if (sender == m_replies[childIndex].selectField)
                {
                    TalkNode talkNode;
                    String nodeId = m_replies[childIndex].idField.Text;
                    foreach (TreeNode node in talkTree.Nodes)
                    {
                        talkNode = (TalkNode)node.Tag;
                        if (talkNode != null
                            && talkNode.id != null
                            && talkNode.id.Equals(nodeId)
                            && talkNode.type.Equals(TalkNodeTypes.None))
                        {
                            if (m_previousNodes.Count == 0)
                            {
                                replyPrevious.Enabled = true;
                            }
                            m_previousNodes.Push(talkTree.SelectedNode);
                            talkTree.SelectedNode = node;
                            talkTree.Focus();
                            break;
                        }
                    }
                }
            }
        }

        private void onPasteXml(object sender, EventArgs e)
        {
            if (Clipboard.ContainsText())
            {
                String xmlText = Clipboard.GetText();
                loadConversation(xmlText);
            }
            else
            {
                MessageBox.Show("The contents of the Clipboard is inappropriate");
            }
        }

        private void onCopyXml(object sender, EventArgs e)
        {
            String previousXml = m_conversation;
            saveConversation();
            String currentXml = m_conversation;
            m_conversation = previousXml;
            Clipboard.SetText(currentXml);
        }

        private void onViewXml(object sender, EventArgs e)
        {
            saveConversation();

            // Bring up the manual editor
            EditTalkTree MEdit = new EditTalkTree();

            MEdit.SetTalkTreeData(m_conversation);

            // Show the box
            MEdit.ShowDialog();

            // Save the string
            String xmlText = MEdit.GetTalkTreeData();

            // See if anything has changed
            if (xmlText != m_conversation)
            {
                // Reload data
                loadConversation(xmlText);
            }
        }

        private void onPreviousNode(object sender, EventArgs e)
        {
            if (m_previousNodes.Count != 0)
            {
                TreeNode previousNode = m_previousNodes.Pop();
                if (m_previousNodes.Count == 0)
                {
                    replyPrevious.Enabled = false;
                }
                talkTree.SelectedNode = previousNode;
                refreshTree();
            }
        }


    }
}