using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Net7_Tools.Editor;
using Net7_Tools.Database;
using System.Threading;
using CommonTools.Database;

namespace Net7_Tools.Gui
{
    public partial class FrmItemValidationReport : Form
    {
        private ItemRecordManager m_itemRecordManager;
        private frmItems m_gui;

        // Multi-threaded form with delegate
        // http://msdn.microsoft.com/en-us/library/ms171728.aspx

        public FrmItemValidationReport(ItemRecordManager itemRecordManager, frmItems gui)
        {
            InitializeComponent();

            m_itemRecordManager = itemRecordManager;
            m_gui = gui;
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);
            validate(null);
        }

        private void onClose(object sender, EventArgs e)
        {
            Close();
        }

        public int validate(BackgroundWorker backgroundWorker)
        {
            Cursor = Cursors.WaitCursor;
            progressBar.Show();

            int displayCount = 10;
            Int32 category;
            String categoryId;
            Int32 subCategory;
            String subCategoryId;
            TabPage[] tabPages;
            IEditor itemEditor;
            DataValidation.Result dataValidationResult;

            List<String> items = Database.Database.getEveryItem();
            progressBar.Maximum = items.Count;
            List<ListViewItem> itemsToAdd = new List<ListViewItem>();
            ListViewItem listViewItem;
            Boolean validCategoryAndSubCategory;

            report.Items.Clear();
            foreach (String id in items)
            {
                if (backgroundWorker != null && backgroundWorker.CancellationPending == true)
                {
                    break;
                }

                if (Database.Database.getItemCategorySubcategory(id, out categoryId, out subCategoryId))
                {
                    // Category is a number
                    if (!Int32.TryParse(categoryId, out category))
                    {
                        listViewItem = new ListViewItem(new String[] { id,
                                                                       "Category",
                                                                       "Value '" + categoryId + "' is not a number"});
                        itemsToAdd.Add(listViewItem);
                        continue;
                    }

                    // Sub-category is a number
                    if (!Int32.TryParse(subCategoryId, out subCategory))
                    {
                        listViewItem = new ListViewItem(new String[] { id,
                                                                       "Category",
                                                                       "Value '" + subCategoryId + "' is not a number"});
                        itemsToAdd.Add(listViewItem);
                        continue;
                    }

                    // Sub-category belongs to category
                    validCategoryAndSubCategory = false;
                    CodeValue[] listCodeValue = Database.Database.getItemSubCategories(Int32.Parse(categoryId));
                    for (int codeValueIndex = 0; codeValueIndex < listCodeValue.Length; codeValueIndex++)
                    {
                        if (listCodeValue[codeValueIndex].code.Equals(subCategory))
                        {
                            validCategoryAndSubCategory = true;
                            break;
                        }
                    }
                    if (!validCategoryAndSubCategory)
                    {
                        listViewItem = new ListViewItem(new String[] { id,
                                                                       "SubCategory",
                                                                       "Value '" + subCategoryId + "' is inappropriate for the sub-category"});
                        itemsToAdd.Add(listViewItem);
                        continue;
                    }

                    // Find the tab pages associated with this category and sub-category
                    tabPages = m_itemRecordManager.getTabPages((Items.ItemCategory)category);
                    if (tabPages == null)
                    {
                        tabPages = m_itemRecordManager.getTabPages((Items.ItemSubCategory)subCategory);
                    }
                    if (tabPages == null)
                    {
                        listViewItem = new ListViewItem(new String[] { id,
                                                                       "TabPages",
                                                                       "The category and sub-category are not associated with GUI pages"});
                        itemsToAdd.Add(listViewItem);
                        continue;
                    }

                    foreach (TabPage tabPage in tabPages)
                    {
                        itemEditor = m_gui.getEditor(tabPage);
                        m_gui.setEditor(Items.getEditorType((Items.ItemSubCategory)subCategory), false);
                        if (itemEditor != null)
                        {
                            dataValidationResult = itemEditor.getTableIO(tabPage).recordValid();
                            if (dataValidationResult != null && !dataValidationResult.isValid())
                            {
                                listViewItem = new ListViewItem(new String[] { id,
                                                                               dataValidationResult.getControlName(),
                                                                               dataValidationResult.getMessage() });
                                itemsToAdd.Add(listViewItem);
                            }
                        }
                    }
                }

                if (itemsToAdd.Count >= displayCount)
                {
                    displayCount += displayCount;
                    report.Items.AddRange(itemsToAdd.ToArray());
                    itemsToAdd.Clear();
                }
if (displayCount > 150) break;
                progressBar.PerformStep();
                System.Windows.Forms.Application.DoEvents();
            }

            if (itemsToAdd.Count != 0)
            {
                report.Items.AddRange(itemsToAdd.ToArray());
                itemsToAdd.Clear();
            }
            progressBar.Hide();

            Cursor = Cursors.Default;
            return 0;
        }

        private void onClipboard(object sender, EventArgs e)
        {
            CopyListViewToClipboard(report);
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
}