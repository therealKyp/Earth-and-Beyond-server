using System;
using System.Collections.Generic;
using System.Text;
using Net7_Tools.Editor;
using Net7_Tools.Database;
using System.Windows.Forms;

namespace Net7_Tools.Editors
{
    public class TableInfo
    {
        public TableIO m_tableIO;
        public TableInfo(TableIO tableIO)
        {
            m_tableIO = tableIO;
        }
    }


    /// <summary>
    /// Handles a single table at a time rather than a group of tables
    /// </summary>
    class TableEditor : EditorCommon, IEditor
    {
        private System.Collections.Generic.Dictionary<Net7_Tools.frmItems.SingleTableEditor, TableInfo> listEditors;

        public TableEditor()
        {
            listEditors = new Dictionary<frmItems.SingleTableEditor, TableInfo>();
        }

        public void add(Net7_Tools.frmItems.SingleTableEditor singleTableEditor, TableIO tableIO)
        {
            TableInfo tableInfo = new TableInfo(tableIO);
            listEditors.Add(singleTableEditor, tableInfo);
        }

        public TableInfo getTableInfo(frmItems.SingleTableEditor singleTableEditor)
        {
            TableInfo tableInfo;
            if (!listEditors.TryGetValue(singleTableEditor, out tableInfo))
            {
                tableInfo = null;
            }
            return tableInfo;
        }

        /// <summary>
        ///   <para>Activate this editor</para>
        ///   <para>Tasks include displaying the proper TabPage and displaying the "first" record</para>
        /// </summary>
        /// <returns>The id of the "main" table.</returns>
        public String activate()
        {
            return "";
        }

        /// <summary>
        /// Retrieve the id of the primary table
        /// </summary>
        /// <returns></returns>
        public String getPrimaryId()
        {
            return "";
        }

        public void addTableHandler(TabPage tabPage, TableIO tableIO)
        {
            throw (new Exception("TableEditor's addTableHandler() should not be used"));
        }

        /// <summary>
        ///   <para>Create the appropriate select criteria to perform a search within the DataSet.</para>
        /// </summary>
        /// <param name="currentId">The currently selected id</param>
        /// <example>String selectCommand = "id = " + currentId;</example>
        public String getRecordSelectCriteria(String currentId)
        {
            return "";
        }

        /// <summary>
        ///   <para>Select the record corresponding to the specified id.</para>
        /// </summary>
        /// <param name="currentId">The currently selected id</param>
        public Boolean selectRecord(String currentId)
        {
            return false;
        }

        /// <summary>
        ///   <para>Use the search dialog to retrieve an id.</para>
        /// </summary>
        /// <param name="currentId">The currently selected id</param>
        /// <returns>
        ///     <para>The newly selected id</para>
        /// </returns>
        /// <remarks>Use the currentId to position the cursor, the initial selection.</remarks>
        public String getIdFromSearchDialog(String currentId)
        {
            return "";
        }

        public void addRecord()
        {
        }

        public void deleteRecord()
        {
        }

        public void saveRecord()
        {
        }

        public void cancelRecord()
        {
        }

        new public void ensureState(TableIO.State state)
        {
        }

    }
}
