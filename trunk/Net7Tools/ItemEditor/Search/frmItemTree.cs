using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;
using Net7_Tools.Database;
using Net7_Tools.Search;
using CommonTools.Gui;
using CommonTools.Database;

namespace Net7_Tools
{
    public partial class FrmItemTree : ISearchDialog
    {
        private class ItemNodeInfo
        {
            public String category;
            public String subCategory;
            public String item;
            public Boolean obtainedChildren;
            public ItemNodeInfo(String category, String subCategory, String item, Boolean obtainedChildren)
            {
                this.category = category;
                this.subCategory = subCategory;
                this.item = item;
                this.obtainedChildren = obtainedChildren;
            }
        }

        // http://support.microsoft.com/kb/319401
        private ListViewColumnSorter lvwColumnSorter;

        /// <value>The ID of the selected item.</value>
        private String m_selectedItemId;

        private DlgSearchCriteria m_dlgSearchCriteria = new DlgSearchCriteria();

        private Net7.Tables m_table;
        private String m_primaryId;

        /// <summary>
        ///   <para>Constructor</para>
        ///   <para>Automatically call loadData() to initialize the contents of the tree.</para>
        /// </summary>
        /// <remarks>This form is shown/hidden rather than created/destroyed
        /// in order to retain lists of data.  This is faster than querying
        /// on demand but takes more memory and increases the chances of working
        /// with stale data.</remarks>
        public FrmItemTree()
        {
            InitializeComponent();
            m_selectedItemId = "";
            searchResultCount.Text = "";

            guiTableCbo.Items.Add(Net7.Tables.item_base);
            guiTableCbo.Items.Add(Net7.Tables.item_ammo);
            guiTableCbo.Items.Add(Net7.Tables.item_beam);
            guiTableCbo.Items.Add(Net7.Tables.item_device);
            guiTableCbo.Items.Add(Net7.Tables.item_engine);
            guiTableCbo.Items.Add(Net7.Tables.item_manufacture);
            guiTableCbo.Items.Add(Net7.Tables.item_missile);
            guiTableCbo.Items.Add(Net7.Tables.item_projectile);
            guiTableCbo.Items.Add(Net7.Tables.item_reactor);
            guiTableCbo.Items.Add(Net7.Tables.item_shield);
            guiTableCbo.SelectedIndex = 0;

            // Create an instance of a ListView column sorter and assign it 
            // to the ListView control.
            lvwColumnSorter = new ListViewColumnSorter();
            this.searchResultList.ListViewItemSorter = lvwColumnSorter;
        }

        /// <summary>
        ///   <para>Collapse the tree nodes and (if it exists) select
        ///         the item ID.</para>
        /// </summary>
        protected override void OnShown(EventArgs e)
        {
            // Reset the currently selected item when the form is shown (or reshown)
            base.OnShown(e);
            loadData();
            selectItemId(m_selectedItemId);
            m_selectedItemId = "";
        }

        private TreeNode createDummyNode()
        {
            TreeNode dummyNode = new TreeNode();
            dummyNode.Name = "";
            dummyNode.Text = "";
            dummyNode.Tag = null;
            return dummyNode;
        }

        /// <summary>
        ///   <para>Retrieve the item information from the database and
        ///         display it within the tree, ordered by category, 
        ///         sub category, and item name.</para>
        /// </summary>
        private void loadData()
        {
            Cursor = Cursors.WaitCursor;

            TreeNode categoryNode = null;
            TreeNode subCategoryNode = null;
            Items.ItemCategory itemCategory;
            Items.ItemSubCategory itemSubCategory;
            String categoryId;
            String subCategoryId;

            TreeNode dummyNode = createDummyNode();

            treeItems.SuspendLayout();
            treeItems.Nodes.Clear();

            CodeValue[] categories = Database.Database.getItemCategories();
            CodeValue[] subCategories;

            foreach (CodeValue category in categories)
            {
                categoryId = category.code.ToString();
                categoryNode = new TreeNode();
                itemCategory = (Items.ItemCategory)Enum.Parse(typeof(Items.ItemCategory), categoryId, true);
                categoryNode.Name = categoryId;
                categoryNode.Text = Items.CategoryLookup(itemCategory) + " (" + categoryId + ")";
                categoryNode.Tag = new ItemNodeInfo(categoryId, "", "", false);
                treeItems.Nodes.Add(categoryNode);

                subCategories = Database.Database.getItemSubCategories(category.code);
                foreach (CodeValue subCategory in subCategories)
                {
                    subCategoryId = subCategory.code.ToString();
                    subCategoryNode = new TreeNode();
                    itemSubCategory = (Items.ItemSubCategory)Enum.Parse(typeof(Items.ItemSubCategory), subCategoryId, true);
                    subCategoryNode.Name = subCategoryId;
                    subCategoryNode.Text = Items.SubCategoryLookup(itemSubCategory) + " (" + subCategoryId + ")";
                    subCategoryNode.Tag = new ItemNodeInfo(categoryId, subCategoryId, "", false);
                    categoryNode.Nodes.Add(subCategoryNode);
                    subCategoryNode.Nodes.Add((TreeNode) dummyNode.Clone());
                }
            }

            treeItems.ResumeLayout(false);
            Cursor = Cursors.Default;
    }

        /// <summary>
        ///   <para>Select the specified item ID if it exists within the tree.</para>
        /// </summary>
        /// <param name="selectedItemId">The item ID to select.</param>
        public void selectItemId(String selectedItemId)
        {
            TreeNodeCollection rootNodes = treeItems.Nodes;

            String category;
            String subCategory;

            if (Database.Database.getItemCategorySubcategory(selectedItemId, out category, out subCategory))
            {
                ItemNodeInfo itemNodeInfo;
                foreach (TreeNode categoryTreeNode in rootNodes)
                {
                    itemNodeInfo = (ItemNodeInfo)categoryTreeNode.Tag;
                    if(itemNodeInfo.category.Equals(category))
                    {
                        categoryTreeNode.Expand();
                        foreach (TreeNode subCategoryTreeNode in categoryTreeNode.Nodes)
                        {
                            itemNodeInfo = (ItemNodeInfo)subCategoryTreeNode.Tag;
                            if (itemNodeInfo.subCategory.Equals(subCategory))
                            {
                                subCategoryTreeNode.Expand();
                                foreach (TreeNode itemTreeNode in subCategoryTreeNode.Nodes)
                                {
                                    itemNodeInfo = (ItemNodeInfo)itemTreeNode.Tag;
                                    if (itemNodeInfo.item.Equals(selectedItemId))
                                    {
                                        // If the tree view does not have the focus then the highlight does not appear
                                        treeItems.Focus();
                                        treeItems.SelectedNode = itemTreeNode;
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }

        /// <summary>
        ///   <para>Set the selected item ID.</para>
        /// </summary>
        /// <remarks>This will be used when the form is shown, to
        /// select the item (if it exists) within the tree.</remarks>
        public void setSelectedItemId(String selectedItemId)
        {
            m_selectedItemId = selectedItemId;
        }

        /// <summary>
        ///   <para>Retrieve the selected item ID.</para>
        /// </summary>
        /// <returns>The selected item ID</returns>
        public String getSelectedItemId()
        {
            return m_selectedItemId;
        }

        /// <summary>
        ///   <para>Close the form and update the selected item ID</para>
        /// </summary>
        private void btnOk_Click(object sender, EventArgs e)
        {
            if (frmItemTreeTabControl.SelectedTab.Equals(frmItemTreeTabBrowse))
            {
                if (treeItems.SelectedNode != null)
                {
                    ItemNodeInfo itemNodeInfo = (ItemNodeInfo)treeItems.SelectedNode.Tag;
                    if (itemNodeInfo.item.Length != 0)
                    {
                        m_selectedItemId = treeItems.SelectedNode.Name;
                        this.Close();
                    }
                }
            }
            else
            {
                if (searchResultList.SelectedItems != null
                    && searchResultList.SelectedItems.Count != 0)
                {
                    ListViewItem listViewItem = searchResultList.SelectedItems[0];
                    m_selectedItemId = listViewItem.SubItems[0].Text;
                    this.Close();
                }
            }
        }

        /// <summary>
        ///   <para>Close the form.</para>
        /// </summary>
        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        public Form getForm()
        {
            return this;
        }

        private void beforeNodeExpand(object sender, TreeViewCancelEventArgs e)
        {
            if (e.Node != null)
            {
                if (e.Node.Nodes.Count == 1
                    && e.Node.Nodes[0].Name.Equals("")
                    && e.Node.Nodes[0].Text.Equals("")
                    && e.Node.Nodes[0].Tag == null)
                {
                    // This is a dummy node; the children of this node have not yet been queried
                    Cursor = Cursors.WaitCursor;

                    e.Node.Nodes.Remove(e.Node.Nodes[0]); // Remove the dummy node
                    List<CodeValue> children;
                    ItemNodeInfo itemNodeInfo = (ItemNodeInfo)e.Node.Tag;
                    {
                        // Obtain the items of the type
                        children = Database.Database.getItems(itemNodeInfo.category, itemNodeInfo.subCategory);
                    }

                    String id;
                    TreeNode node;
                    String itemId = "";
                    TreeNode dummyNode = createDummyNode();
                    foreach (CodeValue child in children)
                    {
                        id = child.code.ToString();
                        node = new TreeNode();
                        node.Name = id;
                        node.Text = child.value + " (" + id + ")";
                        itemId = id;
                        node.Tag = new ItemNodeInfo(itemNodeInfo.category,
                                                    itemNodeInfo.subCategory,
                                                    itemId,
                                                    true);
                        e.Node.Nodes.Add(node);
                        if (itemId.Length == 0)
                        {
                            // This is a type so add a dummy item node
                            node.Nodes.Add((TreeNode)dummyNode.Clone());
                        }
                    }
                    Cursor = Cursors.Default;

                }
            }
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            String query = "";
            String filter = "";
            List<String> sqlParameters = new List<String>();
            List<String> sqlValues = new List<String>();
            int parameterId = 0;

            foreach (SearchCriteria searchCriteria in searchSearchCriteriaLst.Items)
            {
                if (filter.Length != 0)
                {
                    filter += " AND ";
                }
                filter += searchCriteria.getQuery() + parameterId.ToString();
                sqlParameters.Add(searchCriteria.sqlParameter + parameterId.ToString());
                sqlValues.Add(searchCriteria.sqlValue);
                parameterId++;
            }

            if (filter.Length != 0)
            {
                searchResultList.Items.Clear();
                query = "SELECT "
                      + ColumnData.GetName(Net7.Table_item_base._id) + ","
                      + ColumnData.GetName(Net7.Table_item_base._name) + ","
                      + ColumnData.GetName(Net7.Table_item_base._category) + ","
                      + ColumnData.GetName(Net7.Table_item_base._sub_category) + ","
                      + ColumnData.GetName(Net7.Table_item_base._level) + ","
                      + ColumnData.GetName(Net7.Table_item_base._status)
                      + " FROM "
                      + Net7.Tables.item_base;
                if (m_table.Equals(Net7.Tables.item_base))
                {
                    query += DB.WHERE;
                }
                else
                {
                    query += "," + m_table
                           + DB.WHERE
                           + ColumnData.GetName(Net7.Table_item_base._id)
                           + DB.EQUALS
                           + m_primaryId
                           + " AND ";
                }
                query += filter;
                DataTable dataTable = DB.Instance.executeQuery(query, sqlParameters.ToArray(), sqlValues.ToArray());
                searchResultCount.Text = dataTable.Rows.Count.ToString() + " items";
                ListViewItem listViewItem;
                Int32 code;
                Items.ItemCategory categoryValue;
                String category;
                Items.ItemSubCategory subCategoryValue;
                String subCategory;
                foreach(DataRow dataRow in dataTable.Rows)
                {
                    code = ColumnData.GetInt32(dataRow, Net7.Table_item_base._category);
                    categoryValue = (Items.ItemCategory) code;
                    category = Items.CategoryLookup(categoryValue);
                    code = ColumnData.GetInt32(dataRow, Net7.Table_item_base._sub_category);
                    subCategoryValue = (Items.ItemSubCategory) code;
                    subCategory = Items.SubCategoryLookup(subCategoryValue);
                    listViewItem = new ListViewItem(new String[]
                                            {
                                                ColumnData.GetString(dataRow, Net7.Table_item_base._id),
                                                ColumnData.GetString(dataRow, Net7.Table_item_base._name),
                                                category,
                                                subCategory,
                                                ColumnData.GetString(dataRow, Net7.Table_item_base._level),
                                                ColumnData.GetString(dataRow, Net7.Table_item_base._status)
                                            } );
                    searchResultList.Items.Add(listViewItem);
                }
            }
        }

        private void onSearch_KeyEnter(object sender, KeyEventArgs e)
        {
            if (e.KeyCode.Equals(Keys.Return))
            {
                btnSearch_Click(null, null);
            }
        }

        private void onSearchNameTip_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Use the % character to match against 'any'.\n"
                          + "For example:\n"
                          + "\tchavez%\t\t to find names beginning with 'chavez'\n"
                          + "\t%chemical%\t to find names containing 'chemical'\n"
                          + "\t%missile\t\t to find names ending with 'missile'.");
        }

        private void onColumnClick(object sender, ColumnClickEventArgs e)
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
            this.searchResultList.Sort();
        }

        private void onTableChanged(object sender, EventArgs e)
        {
            searchSearchCriteriaLst.Items.Clear();
            m_table = (Net7.Tables)guiTableCbo.SelectedItem;

            ColumnData.ColumnDataInfo[] fields = null;
            switch (m_table)
            {
                case Net7.Tables.item_base:
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_item_base>(true);
                    m_primaryId = ColumnData.GetName(Net7.Table_item_base._id);
                    break;
                case Net7.Tables.item_ammo:
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_item_ammo>(true);
                    m_primaryId = ColumnData.GetName(Net7.Table_item_ammo._item_id);
                    break;
                case Net7.Tables.item_beam:
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_item_beam>(true);
                    m_primaryId = ColumnData.GetName(Net7.Table_item_beam._item_id);
                    break;
                case Net7.Tables.item_device:
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_item_device>(true);
                    m_primaryId = ColumnData.GetName(Net7.Table_item_device._item_id);
                    break;
                case Net7.Tables.item_engine:
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_item_engine>(true);
                    m_primaryId = ColumnData.GetName(Net7.Table_item_engine._item_id);
                    break;
                case Net7.Tables.item_missile:
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_item_missile>(true);
                    m_primaryId = ColumnData.GetName(Net7.Table_item_missile._item_id);
                    break;
                case Net7.Tables.item_manufacture:
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_item_manufacture>(true);
                    m_primaryId = ColumnData.GetName(Net7.Table_item_manufacture._item_id);
                    break;
                case Net7.Tables.item_projectile:
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_item_projectile>(true);
                    m_primaryId = ColumnData.GetName(Net7.Table_item_projectile._item_id);
                    break;
                case Net7.Tables.item_reactor:
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_item_reactor>(true);
                    m_primaryId = ColumnData.GetName(Net7.Table_item_reactor._item_id);
                    break;
                case Net7.Tables.item_shield:
                    fields = CommonTools.Enumeration.ToColumnDataInfo<Net7.Table_item_shield>(true);
                    m_primaryId = ColumnData.GetName(Net7.Table_item_shield._item_id);
                    break;
                default:
                    throw (new Exception("FrmItemTree.onTableChanged() does not handle the table " + m_table.ToString()));
            }
            m_dlgSearchCriteria.configure(fields);

        }

        private void onAddSearchCriteria_Click(object sender, EventArgs e)
        {
            m_dlgSearchCriteria.ShowDialog(this.getForm());
            SearchCriteria searchCriteria = m_dlgSearchCriteria.getSearchCriteria();
            if (!searchCriteria.IsEmpty())
            {
                int index = searchSearchCriteriaLst.Items.Add(searchCriteria);
                searchSearchCriteriaLst.SelectedIndex = index;
            }
        }

        private void onEditSearchCriteria_Click(object sender, EventArgs e)
        {
            SearchCriteria searchCriteria = (SearchCriteria)searchSearchCriteriaLst.SelectedItem;
            if (searchCriteria != null)
            {
                m_dlgSearchCriteria.setSearchCriteria(searchCriteria);
                m_dlgSearchCriteria.ShowDialog(this.getForm());
                searchCriteria = m_dlgSearchCriteria.getSearchCriteria();
                if (!searchCriteria.IsEmpty())
                {
                    int index = searchSearchCriteriaLst.SelectedIndex;
                    searchSearchCriteriaLst.Items[index] = searchCriteria;
                }
            }
        }

        private void onRemoveSearchCriteria_Click(object sender, EventArgs e)
        {
            SearchCriteria searchCriteria = (SearchCriteria)searchSearchCriteriaLst.SelectedItem;
            if (searchCriteria != null)
            {
                int index = searchSearchCriteriaLst.SelectedIndex;
                searchSearchCriteriaLst.Items.Remove(searchCriteria);
                if (index >= searchSearchCriteriaLst.Items.Count)
                {
                    index--;
                }
                if (searchSearchCriteriaLst.Items.Count != 0)
                {
                    searchSearchCriteriaLst.SelectedIndex = index;
                }
            }
        }

        private void onClipboard(object sender, EventArgs e)
        {
            CopyListViewToClipboard(searchResultList);
            MessageBox.Show("You can now paste this information into WordPad or Excel.", "Sent report to clipboard");

        }

        /// <summary>
        /// 
        /// </summary>
        /// <remarks>http://dotnetref.blogspot.com/2007/06/copy-listview-to-clipboard.html</remarks>
        public void CopyListViewToClipboard(ListView lv)
        {
            StringBuilder buffer = new StringBuilder();
            for (int i = 0; i < lv.Columns.Count; i++)
            {
                buffer.Append(lv.Columns[i].Text);
                buffer.Append("\t");
            }
            buffer.Append("\n");

            for (int i = 0; i < lv.Items.Count; i++)
            {
                for (int j = 0; j < lv.Columns.Count; j++)
                {
                    buffer.Append(lv.Items[i].SubItems[j].Text);
                    buffer.Append("\t");
                }
                buffer.Append("\n");
            }

            Clipboard.SetText(buffer.ToString());
        }

    }

    //TODO: Display the description of the selected item at the bottom of the form?
    //TODO: The OK button is only enabled when a node is of type "item"
}
