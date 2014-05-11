using System;
using System.Collections.Generic;
using System.Text;
using Net7_Tools.Database;
using System.Windows.Forms;
using Net7_Tools.Search;
using CommonTools.Database;

namespace Net7_Tools.Editor
{
    public class ItemEditor : EditorCommon, IEditor
    {
        private ISearchDialog m_itemSearchDialog;

        /// <summary>
        ///   <para>Create the appropriate select criteria to perform a search within the DataSet.</para>
        /// </summary>
        /// <param name="currentId">The currently selected id</param>
        /// <example>String selectCommand = "id = " + currentId;</example>
        public String getRecordSelectCriteria(String currentId)
        {
            return "id = " + currentId;
        }

        // TODO: Most of this code is generic for any instance of IEditor

        /// <summary>
        ///   <para>Constructor</para>
        /// </summary>
        public ItemEditor(ISearchDialog itemSearchDialog)
        {
            m_itemSearchDialog = itemSearchDialog;
        }

        /// <summary>
        /// Retrieve the id of the primary table
        /// </summary>
        /// <returns></returns>
        public String getPrimaryId()
        {
            String primaryTableId = "";
            if (listTableHandler.Count != 0)
            {
                System.Data.DataRow primaryTableCurrentRow = primaryTableCurrentRow = listTableHandler[0].getTableIO().getCurrentDataRow();
                if (primaryTableCurrentRow != null)
                {
                    primaryTableId = ColumnData.GetString(primaryTableCurrentRow, Net7.Table_item_base._id);
                }
            }
            return primaryTableId;
        }

        /// <summary>
        ///   <para>Activate this editor</para>
        ///   <para>Tasks include displaying the proper TabPage and displaying the "first" record</para>
        /// </summary>
        /// <returns>The DataRow of the "main" table.</returns>
        public String activate()
        {
            // TODO: This should be one of the first functions called
            // Check whether there are tables and if not then generate an error
            System.Data.DataRow primaryTableCurrentRow = null;
            foreach (TableHandler tableHandler in listTableHandler)
            {
                if (primaryTableCurrentRow == null)
                {
                    // Assumes that the first table is the primary one
                    primaryTableCurrentRow = tableHandler.getTableIO().activate();
                }
                else
                {
                    // Primary table activates every tab
                    tableHandler.getTableIO().activate();
                }
            }

            String primaryTableId = null;
            if (primaryTableCurrentRow != null)
            {
                primaryTableId = ColumnData.GetString(primaryTableCurrentRow, Net7.Table_item_base._id);
            }
            return primaryTableId;
        }

        /// <summary>
        ///   <para>Add a "page" to the editor.  Each page is composed of a TabPage,
        ///         a TableIO, and a IRecordManager</para>
        ///   <para>The first "page" is considered the "main" page, linked with the
        ///         search actions as well as containing the id to be used in the
        ///         other pages.</para>
        /// </summary>
        /// <param name="tabPage">The TabPage to add.</param>
        /// <param name="tableIO">The TableIO to add.</param>
        public void addTableHandler(TabPage tabPage, TableIO tableIO)
        {
            listTableHandler.Add(new TableHandler(tabPage, tableIO));
        }

        /// <summary>
        ///   <para>Select the record corresponding to the specified id.</para>
        /// </summary>
        /// <param name="selectId">The id to select</param>
        public Boolean selectRecord(String selectId)
        {
            TableHandler primaryTableHandler = listTableHandler[0];
            ItemRecordManager itemRecordManager = (ItemRecordManager)primaryTableHandler.getTableIO().getRecordManager();
            frmItems gui = (frmItems) itemRecordManager.getGui();
            frmItems.EditorType previousEditorType = gui.getCurrentEditorType();
            String selectCommand = primaryTableHandler.getTableIO().getRecordManager().selectDataRowFromId(selectId);
            String currentId = getPrimaryId();
            Boolean foundRecord = primaryTableHandler.getTableIO().recordSearch(selectCommand);
            if (!foundRecord)
            {
                // Remain with this current editor
                selectCommand = primaryTableHandler.getTableIO().getRecordManager().selectDataRowFromId(currentId);
                primaryTableHandler.getTableIO().recordSearch(selectCommand);
                return false;
            }
            primaryTableHandler.getTableIO().viewRecord();

            // A new type of editor may have been activated
            frmItems.EditorType currentEditorType = gui.getCurrentEditorType();
            if (!currentEditorType.Equals(previousEditorType))
            {
                // Let that new editor handle the record
                // Note that this call is recursive since it calls this same
                // function.  An infinite loop is avoided as long as the previous
                // and current editor types are different
                return gui.getCurrentEditor().selectRecord(selectId);
            }


            Boolean skippedFirstTable = false;
            foreach (TableHandler tableHandler in listTableHandler)
            {
                if (!skippedFirstTable)
                {
                    skippedFirstTable = true;
                }
                else
                {
                    selectCommand = tableHandler.getTableIO().getRecordManager().selectDataRowFromId(selectId);
                    tableHandler.getTableIO().recordSearch(selectCommand);
                    tableHandler.getTableIO().viewRecord();
                }
            }
            return foundRecord;
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
            m_itemSearchDialog.setSelectedItemId(currentId);
            m_itemSearchDialog.getForm().ShowDialog();
            if (m_itemSearchDialog.getSelectedItemId().Length == 0)
            {
                return null;
            }
            else
            {
                return m_itemSearchDialog.getSelectedItemId();
            }
        }

        public void addRecord()
        {

            foreach (TableHandler tableHandler in listTableHandler)
            {
                if (!tableHandler.getTableIO().isRecordOptional())
                {
                    tableHandler.getTableIO().recordAdd();
                }
            }
        }

        public void deleteRecord()
        {
            if (TableIO.recordDelete(listTableHandler))
            {
                activate();
            }
        }

        public void saveRecord()
        {
            DataValidation.Result dataValidationResult = new DataValidation.Result();

            // 1. Validate the data
            foreach (TableHandler tableHandler in listTableHandler)
            {
                if (!tableHandler.getTableIO().isRecordOptional())
                {
                    dataValidationResult = tableHandler.getTableIO().recordValid();
                    if (dataValidationResult != null
                        && !dataValidationResult.isValid())
                    {
                        dataValidationResult.showMessage();
                        break;
                    }
                }
            }

            if (dataValidationResult == null || dataValidationResult.isValid())
            {
                // 2. Retrieve the id from the primary table
                String primaryId = null;
                if (listTableHandler.Count != 0)
                {
                    TableHandler primaryTableHandler = listTableHandler[0];
                    if (true /*|| primaryTableHandler.getTableIO().getState().Equals(TableIO.State.Add)*/ )
                    {
                        Control idControl = primaryTableHandler.getTableIO().findControl(Net7.Table_item_base._id);
                        if (idControl != null)
                        {
                            primaryId = idControl.Text;
                        }
                    }
                }
                else
                {
                    // Data is invalid, issue a halt to this process
                }

                // 3. Save each table
                // TODO: This generates multiple calls to TableIO.enableFiels()
                foreach (TableHandler tableHandler in listTableHandler)
                {
                    if (!tableHandler.getTableIO().isRecordOptional())
                    {
                        if (!tableHandler.getTableIO().recordSave(primaryId))
                        {
                            return;
                        }
                    }
                }

                // 4. Set the proper state 
                foreach (TableHandler tableHandler in listTableHandler)
                {
                    tableHandler.getTableIO().setState(TableIO.State.View);
                }
            }
        }

        public void cancelRecord()
        {
            foreach (TableHandler tableHandler in listTableHandler)
            {
                tableHandler.getTableIO().recordCancel();
            }
            String primaryId = getPrimaryId();
            selectRecord(primaryId);
        }

        public ISearchDialog getSearchDialog()
        {
            return m_itemSearchDialog;
        }

    }
}
