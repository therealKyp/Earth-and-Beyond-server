using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Net7_Tools.Database
{
    public class TableHandler
    {
        private TabPage tabPage;
        private TableIO tableIO;

        public TableHandler(TabPage tabPage, TableIO tableIO)
        {
            this.tabPage = tabPage;
            this.tableIO = tableIO;
        }

        public TabPage getTabPage()
        {
            return tabPage;
        }

        public TableIO getTableIO()
        {
            return tableIO;
        }
    }
}
