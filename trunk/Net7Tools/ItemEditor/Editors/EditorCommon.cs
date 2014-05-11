using System;
using System.Collections.Generic;
using System.Text;
using Net7_Tools.Database;
using System.Windows.Forms;

namespace Net7_Tools.Editor
{
    public class EditorCommon
    {
        protected List<TableHandler> listTableHandler;

        protected EditorCommon()
        {
            listTableHandler = new List<TableHandler>();
        }

        public TableIO getTableIO(TabPage tabPage)
        {
            foreach (TableHandler tableHandler in listTableHandler)
            {
                if (tableHandler.getTabPage().Equals(tabPage))
                {
                    return tableHandler.getTableIO();
                }
            }
            return null;
        }

        public List<TableHandler> getTableHandlers()
        {
            return listTableHandler;
        }

        public void ensureState(TableIO.State state)
        {
            foreach (TableHandler tableHandler in listTableHandler)
            {
                if (tableHandler.getTableIO().getState().Equals(TableIO.State.Unknown))
                {
                    tableHandler.getTableIO().activate();
                }
                if (!tableHandler.getTableIO().getState().Equals(state))
                {
                    tableHandler.getTableIO().setState(state);
                }
            }
        }
    }
}
