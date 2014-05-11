using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace TalkTreeEditor
{
    enum TalkNodeTypes { None, Branch, Trade, Flags };
    class TalkNode
    {
        public TreeNode parentNode;
        public TalkNodeTypes type;
        public String id;
        public String text;

        public TalkNode()
        {
            parentNode = null;
            type = TalkNodeTypes.None;
            id = "";
            text = "";
        }

        public TalkNode(TreeNode parentNode, TalkNodeTypes type, String id, String text)
        {
            this.parentNode = parentNode;
            this.type = type;
            this.id = id;
            this.text = text;
        }

        public override string ToString()
        {
            if (parentNode == null)
            {
                return id
                     + ": "
                     + ((text.Length > 80) ? text.Substring(0, 80) : text);
            }
            else
            {
                switch (type)
                {
                    case TalkNodeTypes.Branch:
                        return id + ") " + ((text.Length > 80) ? text.Substring(0, 80) : text);
                    case TalkNodeTypes.Trade:
                        return "*Old Trade, automatically removed upon save*";
                    case TalkNodeTypes.Flags:
                        return CommonTools.Enumeration.GetString<CommonTools.TalkTreeFlag>(text)
                             + ((id.Length == 0) ? "" : ": " + id);
                }
            }
            return "";
        }
    }
}
