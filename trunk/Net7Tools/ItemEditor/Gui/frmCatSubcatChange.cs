using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Net7_Tools.Database;
using CommonTools.Database;

namespace Net7_Tools.Gui
{
    public partial class frmCatSubcatChange : Form
    {
        private Boolean m_madeSelection;

        public frmCatSubcatChange()
        {
            InitializeComponent();
            cboCategory.SelectionChangeCommitted += new EventHandler(categorySelectionChanged);
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            // Initialize the categories
            cboCategory.Items.Clear();
            cboCategory.Items.AddRange(Database.Database.getItemCategories());
            m_madeSelection = false;
        }

        public void categorySelectionChanged(object sender, EventArgs e)
        {
            ComboBox category = (ComboBox)sender;
            CodeValue codeValue = (CodeValue)category.SelectedItem;
            CodeValue[] listSubCategory = Database.Database.getItemSubCategories(codeValue.code);
            cboSubCategory.Items.Clear();
            if (listSubCategory != null && listSubCategory.Length != 0)
            {
                cboSubCategory.Items.AddRange(listSubCategory);
                cboSubCategory.SelectedItem = listSubCategory.GetValue(0);
            }
            else
            {
                cboSubCategory.Text = "";
            }
        }

        public Boolean getSelections(out CodeValue category, out CodeValue subCategory)
        {
            if (m_madeSelection)
            {
                category = (CodeValue)cboCategory.SelectedItem;
                subCategory = (CodeValue)cboSubCategory.SelectedItem;
            }
            else
            {
                category = null;
                subCategory = null;
            }
            return m_madeSelection;
        }

        private void onOk(object sender, EventArgs e)
        {
            m_madeSelection = true;
            this.Close();
        }

        private void OnCancel(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}