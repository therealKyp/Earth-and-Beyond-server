using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace TalkTreeEditor
{
    class Replies
    {
        public ComboBox typeField;
        public TreeNode treeNode;

        public Replies(TreeNode treeNode, ComboBox type)
        {
            this.treeNode = treeNode;
            this.typeField = type;
        }
    }
}
