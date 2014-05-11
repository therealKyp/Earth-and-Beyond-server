using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CommonTools.Gui
{
    public class TableButtonHandler
    {
        private ListView table;
        private Button add;
        private Button delete;
        private Button edit;
        private Button up;
        private Button down;

        public TableButtonHandler(ListView table, Button add, Button delete, Button edit, Button up, Button down)
        {
            this.table = table;
            this.add = add;
            this.delete = delete;
            this.edit = edit;
            this.up = up;
            this.down = down;

            table.SelectedIndexChanged += new EventHandler(onTableRowSelected);
            up.Click += new EventHandler(onMoveUp);
            down.Click += new EventHandler(onMoveDown);
        }

        private void onTableRowSelected(object sender, EventArgs e)
        {
            delete.Enabled = table.SelectedItems.Count != 0;
            if (edit != null)
            {
                edit.Enabled = false; // table.SelectedItems.Count == 1;
            }
            up.Enabled = table.SelectedItems.Count == 1
                            && table.SelectedIndices[0] != 0;
            down.Enabled = table.SelectedItems.Count == 1
                            && table.SelectedIndices[0] != table.Items.Count - 1;
        }

        private void onMoveUp(object sender, EventArgs e)
        {
            table.BeginUpdate();
            int index = table.SelectedItems[0].Index;
            ListViewItem lvItem = table.Items[index];
            table.Items.Remove(lvItem);
            table.Items.Insert(index - 1, lvItem);
            /*int ilevel = 0;
            if (table.SelectedItems.Count > 0)
            {
                for (int itmp = 0; itmp <= table.Items.Count - 1; itmp++)
                {
                    if (table.Items[itmp].Selected == true)
                    {
                        ilevel = itmp;
                        if (ilevel - 1 >= 0)
                        {
                            ListViewItem lvitem = table.Items[itmp];
                            table.Items.Remove(lvitem);
                            table.Items.Insert(ilevel - 1, lvitem);
                        }

                    }
                }
            }*/
            table.EndUpdate();
        }

        private void onMoveDown(object sender, EventArgs e)
        {
            table.BeginUpdate();
            int index = table.SelectedItems[0].Index;
            ListViewItem lvItem = table.Items[index];
            table.Items.Remove(lvItem);
            table.Items.Insert(index + 1, lvItem);
            /*int iLevel = 0;
            if (table.SelectedItems.Count > 0)
            {
                for (int iTmp = 0; iTmp <= table.Items.Count - 1; iTmp++)
                {
                    if (table.Items[iTmp].Selected == true)
                    {
                        iLevel = iTmp;
                        if (iLevel + 1 <= table.Items.Count - 1)
                        {
                            ListViewItem lvItem = table.Items[iTmp];
                            table.Items.Remove(lvItem);
                            table.Items.Insert(iLevel + 1, lvItem);
                        }
                    }
                }
            }*/
            table.EndUpdate();
        }

    }

}
