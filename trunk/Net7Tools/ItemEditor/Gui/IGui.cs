using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Net7_Tools
{
    /// <summary>
    ///   <para>Callbacks to the main form</para>
    /// </summary>
    public interface IGui
    {
        void setGuiEnabled(Boolean add, Boolean delete, Boolean saveCancel);

        ToolStripTextBox getSearchField();

        TabControl getTabControl();

        void setTabPages(TabPage[] listTabPages);

        void setEditor(frmItems.EditorType editorType, Boolean activate);

        Net7_Tools.Editor.IEditor getCurrentEditor();

        Net7_Tools.Editors.TableInfo getTableInfo(frmItems.SingleTableEditor singleTableEditor);

        Net7_Tools.Search.ISearchDialog getSearchDialog(Net7_Tools.frmItems.SearchDialog searchDialog);
    }
}
