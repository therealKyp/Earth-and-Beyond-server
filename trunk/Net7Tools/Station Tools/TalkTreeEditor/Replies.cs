using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace TalkTreeEditor
{
    class Replies
    {
        public TreeNode treeNode;
        public ComboBox typeField;
        public TextBox idField;
        public TextBox textField;
        public Button selectField;

        public Replies(TreeNode treeNode, ComboBox type, TextBox id, TextBox text, Button select)
        {
            this.treeNode = treeNode;
            this.typeField = type;
            this.idField = id;
            this.textField = text;
            this.selectField = select;
        }
    }
}
