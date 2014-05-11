using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.Design;

using N7.Sql;

namespace N7.GUI
{
    public partial class ItemBaseAssets : Form
    {
        private ListViewGroup level_one;
        private ListViewGroup level_two;
        private ListViewGroup level_three;
        private ListViewGroup level_four;
        private ListViewGroup level_five;
        private ListViewGroup level_six;
        private ListViewGroup level_seven;
        private ListViewGroup level_eight;
        private ListViewGroup level_nine;
        private DataRow newMobItem;
        private int _type;
        private int _mob_id;

        public ItemBaseAssets(int type, int mob_id)
        {
            _type = type;
            _mob_id = mob_id;
            InitializeComponent();
        }

        private void BaseAssets_Load(object sender, EventArgs e)
        {
            //Setup Groups
            level_one = new ListViewGroup("Level One", HorizontalAlignment.Left);
            level_two = new ListViewGroup("Level Two", HorizontalAlignment.Left);
            level_three = new ListViewGroup("Level Three", HorizontalAlignment.Left);
            level_four = new ListViewGroup("Level Four", HorizontalAlignment.Left);
            level_five = new ListViewGroup("Level Five", HorizontalAlignment.Left);
            level_six = new ListViewGroup("Level Six", HorizontalAlignment.Left);
            level_seven = new ListViewGroup("Level Seven", HorizontalAlignment.Left);
            level_eight = new ListViewGroup("Level Eight", HorizontalAlignment.Left);
            level_nine = new ListViewGroup("Level Nine", HorizontalAlignment.Left);

            listView1.Groups.AddRange(new ListViewGroup[] { level_one, level_two, level_three, level_four, level_five,
                                                            level_six, level_seven, level_eight, level_nine } );
        }

        public void insertListViewItem(int category)
        {
            listView1.Items.Clear();
            listView1.Clear();
            DataRow[] ibt = mainFrm.itemBase.getRowsByCategory(category);

            for (int i = 0; i < ibt.Length; i++)
            {
                DataRow r = ibt[i];
                String itemName = r["name"].ToString();
                int itemID = int.Parse(r["id"].ToString());
                int itemCat = int.Parse(r["sub_category"].ToString());
                int asset_2d = int.Parse(r["2d_asset"].ToString());
                int level = int.Parse(r["level"].ToString());

                String imageFileName = mainFrm.baseAssets.getFileNameByID(asset_2d);

                ListViewItem lvi2 = new ListViewItem(itemName, imageFileName);
                lvi2.Tag = itemID;
                lvi2.ToolTipText = itemName;

                switch (level)
                {
                    case 1:
                        lvi2.Group = level_one;
                        break;
                    case 2:
                        lvi2.Group = level_two;
                        break;
                    case 3:
                        lvi2.Group = level_three;
                        break;
                    case 4:
                        lvi2.Group = level_four;
                        break;
                    case 5:
                        lvi2.Group = level_five;
                        break;
                    case 6:
                        lvi2.Group = level_six;
                        break;
                    case 7:
                        lvi2.Group = level_seven;
                        break;
                    case 8:
                        lvi2.Group = level_eight;
                        break;
                    case 9:
                        lvi2.Group = level_nine;
                        break;
                }

                listView1.Items.Add(lvi2);
                comboBox2.SelectedIndex = 0;
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if( comboBox1.SelectedItem.ToString() == "Ammo")
            {
                insertListViewItem(103);
            }
            else if( comboBox1.SelectedItem.ToString() == "Devices")
            {
                insertListViewItem(110);
            }
            else if( comboBox1.SelectedItem.ToString() == "Engines")
            {
                insertListViewItem(121);
            }
            else if( comboBox1.SelectedItem.ToString() == "Reactors")
            {
                insertListViewItem(120);
            }
            else if( comboBox1.SelectedItem.ToString() == "Shields")
            {
                insertListViewItem(122);
            }
            else if( comboBox1.SelectedItem.ToString() == "Missile Weapon")
            {
                insertListViewItem(102);
            }
            else if( comboBox1.SelectedItem.ToString() == "Projectile Weapon")
            {
                insertListViewItem(101);
            }
            else if( comboBox1.SelectedItem.ToString() == "Beam Weapon")
            {
                insertListViewItem(100);
            }
            else if( comboBox1.SelectedItem.ToString() == "Components")
            {
                insertListViewItem(500);
            }
            else if( comboBox1.SelectedItem.ToString() == "Ore & Resources")
            {
                insertListViewItem(800);
            }
            else if( comboBox1.SelectedItem.ToString() == "Misc. Loot")
            {
                insertListViewItem(-1);
            }
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            String level = comboBox2.SelectedItem.ToString();
            listView1.Groups.Clear();

            switch(level)
            {
                case "All":
                    listView1.Groups.AddRange(new ListViewGroup[] { level_one, level_two, level_three, level_four, level_five,
                                                            level_six, level_seven, level_eight, level_nine });
                    break;
                case "1":
                    listView1.Groups.Add(level_one);
                    break;
                case "2":
                    listView1.Groups.Add(level_two);
                    break;
                case "3":
                    listView1.Groups.Add(level_three);
                    break;
                case "4":
                    listView1.Groups.Add(level_four);
                    break;
                case "5":
                    listView1.Groups.Add(level_five);
                    break;
                case "6":
                    listView1.Groups.Add(level_six);
                    break;
                case "7":
                    listView1.Groups.Add(level_seven);
                    break;
                case "8":
                    listView1.Groups.Add(level_eight);
                    break;
                case "9":
                    listView1.Groups.Add(level_nine);
                    break;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in listView1.SelectedItems)
            {
                int selectedID = int.Parse(item.Tag.ToString());

                newMobItem = mainFrm.mobItemsSql.getMobItemsTable().NewRow();
                newMobItem["mob_id"] = _mob_id;
                newMobItem["item_base_id"] = selectedID;
                newMobItem["usage_chance"] = 0;
                newMobItem["drop_chance"] = 0;
                newMobItem["type"] = _type;
                newMobItem["qty"] = 1;

                mainFrm.mobItemsSql.getMobItemsTable().Rows.Add(newMobItem);
                mainFrm.mobItemsSql.getMobItemsTable().AcceptChanges();
                mainFrm.mobItemsSql.insertRecord(newMobItem);

                mainFrm.newMobItem = newMobItem;
            }
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            mainFrm.newMobItem = null;
            this.Close();
        }
    }
}