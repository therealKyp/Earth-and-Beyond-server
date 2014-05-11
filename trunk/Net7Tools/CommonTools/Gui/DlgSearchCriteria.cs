using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using CommonTools.Database;

namespace CommonTools.Gui
{
    public partial class DlgSearchCriteria : Form
    {
        private SearchCriteria m_searchCriteria = new SearchCriteria();

        public DlgSearchCriteria()
        {
            InitializeComponent();

            guiComparisonCbo.Items.Clear();
            guiComparisonCbo.Items.Add(SearchCriteria.getComparatorSymbol(0) + "   (lesser than)");
            guiComparisonCbo.Items.Add(SearchCriteria.getComparatorSymbol(1) + "  (lesser or equal)");
            guiComparisonCbo.Items.Add(SearchCriteria.getComparatorSymbol(2) + "   (equal)");
            guiComparisonCbo.Items.Add(SearchCriteria.getComparatorSymbol(3) + "  (greater or equal)");
            guiComparisonCbo.Items.Add(SearchCriteria.getComparatorSymbol(4) + "   (greater than)");
            guiComparisonCbo.SelectedItem = guiComparisonCbo.Items[0];

        }

        /// <summary>
        ///   <para>Reset the sub-query when displaying the form</para>
        /// </summary>
        protected override void OnShown(EventArgs e)
        {
            // Reset the currently selected item when the form is shown (or reshown)
            base.OnShown(e);
            m_searchCriteria = new SearchCriteria();
        }

        public void configure(ColumnData.ColumnDataInfo[] fieldNames)
        {
            guiFieldCbo.Items.Clear();
            foreach (ColumnData.ColumnDataInfo field in fieldNames)
            {
                guiFieldCbo.Items.Add(field);
            }
            if (guiFieldCbo.Items.Count != 0)
            {
                guiFieldCbo.SelectedItem = guiFieldCbo.Items[0];
                onFieldSelected(null, null);
            }
        }

        /// <summary>
        /// Return the sub-query
        /// </summary>
        /// <returns></returns>
        public SearchCriteria getSearchCriteria()
        {
            return m_searchCriteria;
        }

        public void setSearchCriteria(SearchCriteria searchCriteria)
        {
            m_searchCriteria = searchCriteria;
            for (int index = 0; index < guiFieldCbo.Items.Count; index++)
            {
                if (((ColumnData.ColumnDataInfo)guiFieldCbo.Items[index]).Name.Equals(m_searchCriteria.column))
                {
                    guiFieldCbo.SelectedIndex = index;
                    break;
                }
            }
            onFieldSelected(null, null);
        }

        private void onPatternTip_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Use the % character to match against 'any'.\n"
                          + "For example:\n"
                          + "\tchavez%\t\t to find names beginning with 'chavez'\n"
                          + "\t%chemical%\t to find names containing 'chemical'\n"
                          + "\t%missile\t\t to find names ending with 'missile'.");
        }

        private void onFieldSelected(object sender, EventArgs e)
        {
            ColumnData.ColumnDataInfo column = (ColumnData.ColumnDataInfo)guiFieldCbo.SelectedItem;
            Boolean isString = column.TypeIsString;

            guiPatternTxt.Enabled = isString;
            guiPatternTxt.Text = guiPatternTxt.Enabled ? m_searchCriteria.criteria : "";
            guiPatternTipBtn.Enabled = isString;
            guiComparisonCbo.Enabled = !isString;
            if (guiComparisonCbo.Enabled)
            {
                guiComparisonCbo.SelectedItem = guiComparisonCbo.Items[m_searchCriteria.comparisonSymbol];
            }
            else
            {
                guiComparisonCbo.Text = "";
            }
            guiComparisonTxt.Enabled = guiComparisonCbo.Enabled;
            guiComparisonTxt.Text = guiComparisonTxt.Enabled ? m_searchCriteria.criteria : "";
        }

        private void onOk(object sender, EventArgs e)
        {
            ColumnData.ColumnDataInfo column = (ColumnData.ColumnDataInfo)guiFieldCbo.SelectedItem;
            Boolean isString = column.TypeIsString;
            if (isString)
            {
                m_searchCriteria.comparisonSymbol = (guiPatternTxt.Text.Length == 0)
                                                  ? 2  // =
                                                  : 5; // Like
                m_searchCriteria.criteria = guiPatternTxt.Text;
                m_searchCriteria.subQuery = column.Name
                                          + " "
                                          + m_searchCriteria.getComparatorSymbol()
                                          + " "
                                          + DB.QueryParameterCharacter + column.Name;
                m_searchCriteria.sqlParameter = column.Name;
                m_searchCriteria.sqlValue = guiPatternTxt.Text;
            }
            else
            {
                if(guiComparisonTxt.Text.Length == 0)
                {
                    MessageBox.Show("You must enter a comparison value");
                    guiComparisonTxt.Focus();
                    return;
                }
                m_searchCriteria.comparisonSymbol = guiComparisonCbo.SelectedIndex;
                m_searchCriteria.criteria = guiComparisonTxt.Text;
                m_searchCriteria.subQuery = column.Name
                                          + " "
                                          + m_searchCriteria.getComparatorSymbol()
                                          + " "
                                          + DB.QueryParameterCharacter + column.Name;
                m_searchCriteria.sqlParameter = column.Name;
                m_searchCriteria.sqlValue = guiComparisonTxt.Text;
            }
            m_searchCriteria.column = column.Name;
            this.Close();
        }

        private void onCancel(object sender, EventArgs e)
        {
            this.Close();
        }

        private void onKeyDown(object sender, KeyEventArgs e)
        {
            if(    (sender.Equals(guiFieldCbo) && guiFieldCbo.DroppedDown)
                || (sender.Equals(guiComparisonCbo) && guiComparisonCbo.DroppedDown) )
            {
                // When the combobox list is opened let the combobox handle the return key 
                return;
            }

            if (e.KeyCode.Equals(Keys.Return))
            {
                onOk(null, null);
            }
            else if (e.KeyCode.Equals(Keys.Escape))
            {
                onCancel(null, null);
            }
        }

    }
}