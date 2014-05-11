using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace MissionEditor
{
    enum TalkNodeTypes { None, Branch, Trade };
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
                return id + ": " + text;
            }
            else
            {
                switch (type)
                {
                    case TalkNodeTypes.Branch:
                        return id + ") " + text;
                    case TalkNodeTypes.Trade:
                        return "=> " + id;
                }
            }
            return "";
        }
    }
}
