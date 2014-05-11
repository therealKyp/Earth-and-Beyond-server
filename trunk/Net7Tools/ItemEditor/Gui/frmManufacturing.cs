using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Net7_Tools.Editors;
using Net7_Tools.Database;

namespace Net7_Tools.Gui
{
    public partial class frmManufacturing : Form
    {
        private String m_itemId = "";
        private TableIO m_tableIO;

        public frmManufacturing()
        {
            InitializeComponent();
        }

        protected override void OnShown(EventArgs e)
        {
            // Reset the currently selected item when the form is shown (or reshown)
            base.OnShown(e);
            String selectCommand = m_tableIO.getRecordManager().selectDataRowFromId(m_itemId);
            if (m_tableIO.recordSearch(selectCommand))
            {
                // Record exists
                m_tableIO.setState(TableIO.State.View);
            }
            else
            {
                // This record does not exist
                m_tableIO.setGuiChangeSuspended(true);
                m_tableIO.recordAdd();
                m_tableIO.setGuiChangeSuspended(false);
            }
        }

        public void setTableIO(TableIO tableIO)
        {
            m_tableIO = tableIO;
        }

        public void setItemId(String itemId)
        {
            m_itemId = itemId;
        }

        private void manufacturingOk_Click(object sender, EventArgs e)
        {
            if (manufacturingTxtComponent1.Text.Equals(ItemManufacturingRecordManager.UnspecifiedComponentId))
            {
                // When the first component is unspecified we assume the user intends
                // to delete the manufacturing recipe
                if (m_tableIO.getState().Equals(TableIO.State.View))
                {
                    // Only delete on View since Add only creates the record during save
                    m_tableIO.recordDelete();
                }
                this.Close();
            }
            else
            {
                DataValidation.Result dataValidationResult = m_tableIO.recordValid();
                if (dataValidationResult == null || dataValidationResult.isValid())
                {
                    if (m_tableIO.recordSave(m_itemId))
                    {
                        this.Close();
                    }
                }
                else
                {
                    dataValidationResult.showMessage();
                }
            }
            // Not every path leads to this.Close() and that is intentional
        }
    }
}