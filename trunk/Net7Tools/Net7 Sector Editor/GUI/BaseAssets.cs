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
    public partial class BaseAssets : Form
    {
        public IWindowsFormsEditorService _wfes;
        public int selectedID;
        public int pgID;
        private ListViewGroup listViewGroup1;
        private ListViewGroup listViewGroup2;
        private ListViewGroup listViewGroup3;
        private ListViewGroup listViewGroup4;
        private ListViewGroup listViewGroup5;
        private ListViewGroup listViewGroup6;
        private ListViewGroup listViewGroup7;
        private ListViewGroup listViewGroup8;
        private ListViewGroup listViewGroup9;
        private ListViewGroup listViewGroup10;
        private ListViewGroup listViewGroup11;
        private ListViewGroup listViewGroup12;
        private ListViewGroup listViewGroup13;
        private ListViewGroup listViewGroup14;
        private ListViewGroup listViewGroup15;
        private ListViewGroup listViewGroup16;
        private ListViewGroup listViewGroup17;
        private ListViewGroup listViewGroup18;
        private ListViewGroup listViewGroup19;
        private ListViewGroup listViewGroup20;
        private ListViewGroup listViewGroup21;
        private ListViewGroup listViewGroup22;
        private ListViewGroup listViewGroup23;

        public BaseAssets()
        {
            InitializeComponent();
        }

        private void BaseAssets_Load(object sender, EventArgs e)
        {
            comboBox1.Items.Add("Please Make a Selection");
            comboBox1.Items.Add("Asteroids");
            comboBox1.Items.Add("Backgrounds");
            comboBox1.Items.Add("Capital Ships");
            comboBox1.Items.Add("Decorations");
            comboBox1.Items.Add("Drones");
            comboBox1.Items.Add("Destroyed");
            comboBox1.Items.Add("Hulks");
            comboBox1.Items.Add("Jenquai");
            comboBox1.Items.Add("Landmarks");
            comboBox1.Items.Add("Moons");
            comboBox1.Items.Add("Mobs");
            comboBox1.Items.Add("NavBuoy");
            comboBox1.Items.Add("Planets");
            comboBox1.Items.Add("Progen");
            comboBox1.Items.Add("Pvp");
            comboBox1.Items.Add("Resources");
            comboBox1.Items.Add("Shipyards");
            comboBox1.Items.Add("Starbases");
            comboBox1.Items.Add("Stargates");
            comboBox1.Items.Add("Stargates Deco");
            comboBox1.Items.Add("Terran");
            comboBox1.Items.Add("Turrets");
            comboBox1.Items.Add("Under Contruction");

            comboBox1.SelectedItem = "Please Make a Selection";

            //Setup Groups
            listViewGroup2 = new ListViewGroup("Asteroids", HorizontalAlignment.Left);
            listViewGroup2.Header = "Asteroids";
            listViewGroup2.Name = "listViewGroup2";
            listViewGroup23 = new ListViewGroup("Backgrounds", HorizontalAlignment.Left);
            listViewGroup23.Header = "Backgrounds";
            listViewGroup23.Name = "listViewGroup23";
            listViewGroup3 = new ListViewGroup("Capital Ships", HorizontalAlignment.Left);
            listViewGroup3.Header = "Capital Ships";
            listViewGroup3.Name = "listViewGroup3";
            listViewGroup4 = new ListViewGroup("Decorations", HorizontalAlignment.Left);
            listViewGroup4.Header = "Decorations";
            listViewGroup4.Name = "listViewGroup4";
            listViewGroup5 = new ListViewGroup("Drones", HorizontalAlignment.Left);
            listViewGroup5.Header = "Drones";
            listViewGroup5.Name = "listViewGroup5";
            listViewGroup22 = new ListViewGroup("Destroyed", HorizontalAlignment.Left);
            listViewGroup22.Header = "Destroyed";
            listViewGroup22.Name = "listViewGroup22";
            listViewGroup6 = new ListViewGroup("Hulks", HorizontalAlignment.Left);
            listViewGroup6.Header = "Hulks";
            listViewGroup6.Name = "listViewGroup6";
            listViewGroup7 = new ListViewGroup("Jenquai", HorizontalAlignment.Left);
            listViewGroup7.Header = "Jenquai";
            listViewGroup7.Name = "listViewGroup7";
            listViewGroup8 = new ListViewGroup("Landmarks", HorizontalAlignment.Left);
            listViewGroup8.Header = "Landmarks";
            listViewGroup8.Name = "listViewGroup8";
            listViewGroup9 = new ListViewGroup("Moons", HorizontalAlignment.Left);
            listViewGroup9.Header = "Moons";
            listViewGroup9.Name = "listViewGroup9";
            listViewGroup10 = new ListViewGroup("Mobs", HorizontalAlignment.Left);
            listViewGroup10.Header = "Mobs";
            listViewGroup10.Name = "listViewGroup10";
            listViewGroup11 = new ListViewGroup("NavBuoy", HorizontalAlignment.Left);
            listViewGroup11.Header = "NavBuoy";
            listViewGroup11.Name = "listViewGroup11";
            listViewGroup1 = new ListViewGroup("Planets", HorizontalAlignment.Left);
            listViewGroup1.Header = "Planets";
            listViewGroup1.Name = "listViewGroup1";
            listViewGroup12 = new ListViewGroup("Progen", HorizontalAlignment.Left);
            listViewGroup12.Header = "Progen";
            listViewGroup12.Name = "listViewGroup12";
            listViewGroup13 = new ListViewGroup("Pvp", HorizontalAlignment.Left);
            listViewGroup13.Header = "Pvp";
            listViewGroup13.Name = "listViewGroup13";
            listViewGroup14 = new ListViewGroup("Resources", HorizontalAlignment.Left);
            listViewGroup14.Header = "Resources";
            listViewGroup14.Name = "listViewGroup14";
            listViewGroup15 = new ListViewGroup("Shipyards", HorizontalAlignment.Left);
            listViewGroup15.Header = "Shipyards";
            listViewGroup15.Name = "listViewGroup15";
            listViewGroup16 = new ListViewGroup("Starbases", HorizontalAlignment.Left);
            listViewGroup16.Header = "Starbases";
            listViewGroup16.Name = "listViewGroup16";
            listViewGroup17 = new ListViewGroup("Stargates", HorizontalAlignment.Left);
            listViewGroup17.Header = "Stargates";
            listViewGroup17.Name = "listViewGroup17";
            listViewGroup18 = new ListViewGroup("Stargates Deco", HorizontalAlignment.Left);
            listViewGroup18.Header = "Stargates Deco";
            listViewGroup18.Name = "listViewGroup18";
            listViewGroup19 = new ListViewGroup("Terran", HorizontalAlignment.Left);
            listViewGroup19.Header = "Terran";
            listViewGroup19.Name = "listViewGroup19";
            listViewGroup20 = new ListViewGroup("Turrets", HorizontalAlignment.Left);
            listViewGroup20.Header = "Turrets";
            listViewGroup20.Name = "listViewGroup20";
            listViewGroup21 = new ListViewGroup("Under Contruction", HorizontalAlignment.Left);
            listViewGroup21.Header = "Under Contruction";
            listViewGroup21.Name = "listViewGroup21";

            //Add Groups to the list view.
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup2 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup3 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup4 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup5 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup6 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup7 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup8 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup9 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup10 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup11 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup1 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup12 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup13 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup14 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup15 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup16 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup17 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup18 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup19 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup20 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup21 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup22 });
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup23 });
        }

        public void insertItem(ListViewGroup group, String category)
        {
            DataRow[] rows = mainFrm.baseAssets.getRowsbyCategory(category);
            listView1.Items.Clear();

            for (int i = 0; i < rows.Length; i++)
            {
                int id = int.Parse(rows[i]["base_id"].ToString());
                String name = rows[i]["filename"].ToString();
                String cat_name = rows[i]["main_cat"].ToString();

                String newName = "null";

                //Check to see if we have a file name, and substring it to create our new filename.
                if (name.Contains(".w3d") || name.Contains(".W3D"))
                {
                    String cutName = name.Remove(name.Length - 4);
                    newName = cutName + ".jpg";
                }

                //Add the item.
                ListViewItem listViewItem1 = new ListViewItem("ID: " + id, newName);
                listViewItem1.Group = group;
                this.listView1.Items.AddRange(new System.Windows.Forms.ListViewItem[] { listViewItem1 }); 
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            String category = comboBox1.SelectedItem.ToString();

            //Check for the category name
            if (category == "Asteroids")
            {
                insertItem(listViewGroup2, category);
            }
            else if (category == "Backgrounds")
            {
                insertItem(listViewGroup23, category);
            }
            else if (category == "Capital Ships")
            {
                insertItem(listViewGroup3, category);
            }
            else if (category == "Decorations")
            {
                insertItem(listViewGroup4, category);
            }
            else if (category == "Drones")
            {
                insertItem(listViewGroup5, category);
            }
            else if (category == "Destroyed")
            {
                insertItem(listViewGroup22, category);
            }
            else if (category == "Hulks")
            {
                insertItem(listViewGroup6, category);
            }
            else if (category == "Jenquai")
            {
                insertItem(listViewGroup7, category);
            }
            else if (category == "Landmarks")
            {
                insertItem(listViewGroup8, category);
            }
            else if (category == "Moons")
            {
                insertItem(listViewGroup9, category);
            }
            else if (category == "Mobs")
            {
                insertItem(listViewGroup10, category);
            }
            else if (category == "NavBuoy")
            {
                insertItem(listViewGroup11, category);
            }
            else if (category == "Planets")
            {
                insertItem(listViewGroup1, category);
            }
            else if (category == "Progen")
            {
                insertItem(listViewGroup12, category);
            }
            else if (category == "Pvp")
            {
                insertItem(listViewGroup13, category);
            }
            else if (category == "Resources")
            {
                insertItem(listViewGroup14, category);
            }
            else if (category == "Shipyards")
            {
                insertItem(listViewGroup15, category);
            }
            else if (category == "Starbases")
            {
                insertItem(listViewGroup16, category);
            }
            else if (category == "Stargates")
            {
                insertItem(listViewGroup17, category);
            }
            else if (category == "Stargates Deco")
            {
                insertItem(listViewGroup18, category);
            }
            else if (category == "Terran")
            {
                insertItem(listViewGroup19, category);
            }
            else if (category == "Turrets")
            {
                insertItem(listViewGroup20, category);
            }
            else if (category == "Under Contruction")
            {
                insertItem(listViewGroup21, category);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in listView1.SelectedItems)
            {
                String itemText = item.Text;
                String parsedText = itemText.Substring(4);
                selectedID = int.Parse(parsedText);
            }

            this.Close();
        }
    }
}