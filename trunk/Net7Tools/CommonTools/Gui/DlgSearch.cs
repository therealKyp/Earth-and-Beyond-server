using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CommonTools.Database;

namespace CommonTools.Gui
{
    public partial class DlgSearch : Form
    {
        // http://support.microsoft.com/kb/319401
        private ListViewColumnSorter lvwColumnSorter;

        /// <summary>
        /// The ID of the selected item
        /// </summary>
        private String m_selectedId;

        /// <summary>
        /// Search criteria dialog
        /// </summary>
        private DlgSearchCriteria m_dlgSearchCriteria;

        private Net7.Tables m_table;

        private Enum[] m_columns;

        private Boolean m_adjustedColumnWidths;

        public DlgSearch()
        {
            InitializeComponent();
            guiResultLbl.Text = "";
            m_columns = null;
            m_adjustedColumnWidths = false;
            m_dlgSearchCriteria = null;

            // Create an instance of a ListView column sorter and assign it 
            // to the ListView control.
            lvwColumnSorter = new ListViewColumnSorter();
            guiResultTbl.ListViewItemSorter = lvwColumnSorter;
        }

        /// <summary>
        ///   <para>Collapse the tree nodes and (if it exists) select
        ///         the item ID.</para>
        /// </summary>
        protected override void OnShown(EventArgs e)
        {
            // Reset the currently selected item when the form is shown (or reshown)
            base.OnShown(e);
            m_selectedId = "";
        }

        /// <summary>
        ///   <para>Retrieve the selected ID.</para>
        /// </summary>
        /// <returns>The selected item ID</returns>
        public String getSelectedId()
        {
            return m_selectedId;
        }

        private void onAddClick(object sender, EventArgs e)
        {
            m_dlgSearchCriteria.ShowDialog(this);
            SearchCriteria searchCriteria = m_dlgSearchCriteria.getSearchCriteria();
            if (!searchCriteria.IsEmpty())
            {
                int index = guiSearchCriteriaTbl.Items.Add(searchCriteria);
                guiSearchCriteriaTbl.SelectedIndex = index;
            }
        }

        private void onEditClick(object sender, EventArgs e)
        {
            SearchCriteria searchCriteria = (SearchCriteria)guiSearchCriteriaTbl.SelectedItem;
            if (searchCriteria != null)
            {
                m_dlgSearchCriteria.setSearchCriteria(searchCriteria);
                m_dlgSearchCriteria.ShowDialog(this);
                searchCriteria = m_dlgSearchCriteria.getSearchCriteria();
                if (!searchCriteria.IsEmpty())
                {
                    int index = guiSearchCriteriaTbl.SelectedIndex;
                    guiSearchCriteriaTbl.Items[index] = searchCriteria;
                }
            }
        }

        private void onRemoveClick(object sender, EventArgs e)
        {
            SearchCriteria searchCriteria = (SearchCriteria)guiSearchCriteriaTbl.SelectedItem;
            if (searchCriteria != null)
            {
                int index = guiSearchCriteriaTbl.SelectedIndex;
                guiSearchCriteriaTbl.Items.Remove(searchCriteria);
                if (index >= guiResultTbl.Items.Count)
                {
                    index--;
                }
                if (guiSearchCriteriaTbl.Items.Count != 0)
                {
                    guiSearchCriteriaTbl.SelectedIndex = index;
                }
            }
        }

        public void configure(Net7.Tables table)
        {
            if (!m_table.Equals(table))
            {
                // Clear the previous search results
                guiSearchCriteriaTbl.Items.Clear();
                guiResultTbl.Items.Clear();
                guiResultLbl.Text = "";
            }

            m_table = table;
            ColumnData.ColumnDataInfo[] fields = null;
            switch(m_table)
            {
                case Net7.Tables.effects:
                    m_columns = new Enum[] { Net7.Table_effects._effect_id, Net7.Table_effects._description, Net7.Table_effects._base_asset_id };
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_effects>(true);
                    break;
                case Net7.Tables.factions:
                    m_columns = new Enum[] { Net7.Table_factions._faction_id, Net7.Table_factions._name };
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_factions>(true);
                    break;
                case Net7.Tables.item_base:
                    m_columns = new Enum[]{Net7.Table_item_base._id,
                                           Net7.Table_item_base._name,
                                           Net7.Table_item_base._category,
                                           Net7.Table_item_base._sub_category,
                                           Net7.Table_item_base._level,
                                           Net7.Table_item_base._status};
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_item_base>(true);
                    break;
                case Net7.Tables.missions:
                    m_columns = new Enum[]{Net7.Table_missions._mission_id,
                                           Net7.Table_missions._mission_name,
                                           Net7.Table_missions._mission_type,
                                           Net7.Table_missions._mission_key};
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_missions>(true);
                    break;
                case Net7.Tables.mob_base:
                    m_columns = new Enum[] { Net7.Table_mob_base._mob_id,
                                             Net7.Table_mob_base._name };
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_mob_base>(true);
                    break;
                case Net7.Tables.starbase_npcs:
                    m_columns = new Enum[] { Net7.Table_starbase_npcs._npc_Id,
                                             Net7.Table_starbase_npcs._first_name,
                                             Net7.Table_starbase_npcs._last_name };
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_starbase_npcs>(true);
                    break;
                case Net7.Tables.sectors:
                    m_columns = new Enum[] { Net7.Table_sectors._sector_id,
                                             Net7.Table_sectors._name };
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_sectors>(true);
                    break;
                case Net7.Tables.sector_objects:
                    m_columns = new Enum[] { Net7.Table_sector_objects._sector_object_id,
                                             Net7.Table_sector_objects._name,
                                             Net7.Table_sector_objects._base_asset_id,
                                             Net7.Table_sector_objects._sector_id };
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_sector_objects>(true);
                    break;
                case Net7.Tables.skills:
                    m_columns = new Enum[] { Net7.Table_skills._skill_id,
                                             Net7.Table_skills._name };
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_skills>(true);
                    break;
                default:
                    throw (new Exception("DlgSearch.configure() does not handle the table " + table.ToString()));
            }

            guiResultTbl.Columns.Clear();
            m_adjustedColumnWidths = false;

            ColumnHeader columnHeader;
            for(int columnIndex = 0; columnIndex < m_columns.Length; columnIndex++)
            {
                columnHeader = new ColumnHeader();
                columnHeader.DisplayIndex = columnIndex;
                columnHeader.Text = ColumnData.GetName(m_columns[columnIndex]);
                columnHeader.Width = -2;
                guiResultTbl.Columns.Add(columnHeader);
            }

            m_dlgSearchCriteria = new DlgSearchCriteria();
            m_dlgSearchCriteria.configure(fields);
        }

        private void onSearchClick(object sender, EventArgs e)
        {
            String query = "";
            List<String> sqlParameters = new List<String>();
            List<String> sqlValues = new List<String>();
            int parameterId = 0;

            foreach (SearchCriteria searchCriteria in this.guiSearchCriteriaTbl.Items)
            {
                if (query.Length != 0)
                {
                    query += " AND ";
                }
                query += searchCriteria.getQuery() + parameterId.ToString();
                sqlParameters.Add(searchCriteria.sqlParameter + parameterId.ToString());
                sqlValues.Add(searchCriteria.sqlValue);
                parameterId++;
            }

            if (query.Length != 0)
            {
                String select = "";
                foreach (Enum column in m_columns)
                {
                    if (select.Length == 0)
                    {
                        select = DB.SELECT;
                    }
                    else
                    {
                        select += ",";
                    }
                    select += ColumnData.GetName(column);
                }
                query = select
                      + DB.FROM
                      + m_table.ToString()
                      + DB.WHERE
                      + query;

                DataTable dataTable = DB.Instance.executeQuery(query, sqlParameters.ToArray(), sqlValues.ToArray());
                guiResultTbl.Items.Clear();
                guiResultLbl.Text = dataTable.Rows.Count.ToString() + " items";

                ListViewItem listViewItem;
                List<String> rowData = new List<string>();
                foreach (DataRow dataRow in dataTable.Rows)
                {
                    rowData.Clear();
                    for (int columnIndex = 0; columnIndex < dataTable.Columns.Count; columnIndex++)
                    {
                        rowData.Add(dataRow[columnIndex].ToString());
                    }

                    listViewItem = new ListViewItem(rowData.ToArray());
                    guiResultTbl.Items.Add(listViewItem);
                }

                if (m_adjustedColumnWidths == false)
                {
                    List<int> columnWidth = new List<int>();
                    m_adjustedColumnWidths = true;
                    foreach (ColumnHeader columnHeader in guiResultTbl.Columns)
                    {
                        columnHeader.Width = -2;
                        columnWidth.Add(columnHeader.Width);
                    }
                }
            }
        }

        private void onResultColumnClick(object sender, ColumnClickEventArgs e)
        {
            // Determine if clicked column is already the column that is being sorted.
            if (e.Column == lvwColumnSorter.SortColumn)
            {
                // Reverse the current sort direction for this column.
                if (lvwColumnSorter.Order == SortOrder.Ascending)
                {
                    lvwColumnSorter.Order = SortOrder.Descending;
                }
                else
                {
                    lvwColumnSorter.Order = SortOrder.Ascending;
                }
            }
            else
            {
                // Set the column number that is to be sorted; default to ascending.
                lvwColumnSorter.SortColumn = e.Column;
                lvwColumnSorter.Order = SortOrder.Ascending;
            }

            // Perform the sort with these new sort options.
            this.guiResultTbl.Sort();
        }

        private void onOkClick(object sender, EventArgs e)
        {
            if (guiResultTbl.SelectedItems != null
                && guiResultTbl.SelectedItems.Count != 0)
            {
                ListViewItem listViewItem = guiResultTbl.SelectedItems[0];
                m_selectedId = listViewItem.SubItems[0].Text;
                this.Close();
            }
        }

        private void onCancelClick(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
