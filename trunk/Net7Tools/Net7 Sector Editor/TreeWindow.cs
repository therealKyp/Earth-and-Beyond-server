using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data;

namespace N7
{
    class TreeWindow
    {
        private TreeNode[] parent;
        public TreeWindow(DataTable systems, DataTable sectors)
        {
            parent = new TreeNode[systems.Rows.Count];

            int i = 0;
            foreach (DataRow r in systems.Rows)
            {
                String name = r["name"].ToString();
                String id =  r["system_id"].ToString();
                TreeNode tn1 = new TreeNode(name);

                foreach (DataRow r2 in sectors.Rows)
                {
                    String id2 = r2["system_id"].ToString();
                    if (id == id2)
                    {
                        String name2 = r2["name"].ToString();
                        tn1.Nodes.Add(name2);
                    }
                }

                parent[i] = tn1;
                i++;
            }
        }

        public TreeNode[] setupInitialTree()
        {
            return parent;
        }
    }
}
