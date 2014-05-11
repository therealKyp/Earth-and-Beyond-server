using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using System.IO;

using N7.Sql;

namespace N7.GUI
{
    public partial class MobBaseAssets : Form
    {
        public int selectedID;
        public int pgID;
        private ListViewGroup listViewGroup;
        private TextBox _baText;
        private PictureBox _picBox;

        public MobBaseAssets(TextBox baText, PictureBox picBox)
        {
            _baText = baText;
            _picBox = picBox;
            InitializeComponent();
        }

        private void BaseAssets_Load(object sender, EventArgs e)
        {
            comboBox1.Items.Add("Please Make a Selection");
            comboBox1.Items.Add("Capital Ships");
            comboBox1.Items.Add("Drones");
            comboBox1.Items.Add("Jenquai");
            comboBox1.Items.Add("Mobs");
            comboBox1.Items.Add("NPC Ship Hulls");
            comboBox1.Items.Add("Progen");
            comboBox1.Items.Add("Pvp");
            comboBox1.Items.Add("Terran");
            comboBox1.Items.Add("Turrets");

            comboBox1.SelectedItem = "Please Make a Selection";
        }

        public void insertItem(String category)
        {
            DataRow[] rows = mainFrm.baseAssets.getRowsbyCategory(category);
            listView1.Items.Clear();
            imageList.Images.Clear();

            listViewGroup = new ListViewGroup(category, HorizontalAlignment.Left);
            listViewGroup.Header = category;
            this.listView1.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] { listViewGroup });

            for (int i = 0; i < rows.Length; i++)
            {
                int id = int.Parse(rows[i]["base_id"].ToString());
                String name = rows[i]["filename"].ToString();
                String cat_name = rows[i]["main_cat"].ToString();
                String description = rows[i]["descr"].ToString();

                String newName = "null";

                //Check to see if we have a file name, and substring it to create our new filename.
                if (name.Contains(".w3d") || name.Contains(".W3D") || name.Contains(".tga") || name.Contains(".TGA"))
                {
                    String cutName = name.Remove(name.Length - 4);
                    newName = cutName + ".jpg";
                }

                try
                {
                    Image tmp = Image.FromFile(Application.StartupPath+"\\images\\thumbnails\\"+newName);
                    imageList.Images.Add(newName, tmp);
                }
                catch (Exception)
                {
                    Image tmp = Image.FromFile(Application.StartupPath + "\\images\\naPic.jpg");
                    imageList.Images.Add(newName, tmp);
                }

                //Add the item.
                ListViewItem listViewItem1 = new ListViewItem(description, newName);
                listViewItem1.Tag = id;
                listViewItem1.ToolTipText = "Filename: "+ name +", ID: "+id;
                listViewItem1.Group = listViewGroup;
                this.listView1.Items.AddRange(new System.Windows.Forms.ListViewItem[] { listViewItem1 }); 
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            String category = comboBox1.SelectedItem.ToString();

            //Check for the category name
            if (category == "Capital Ships")
            {
                insertItem(category);
            }
            else if (category == "Drones")
            {
                insertItem(category);
            }
            else if (category == "Jenquai")
            {
                insertItem(category);
            }
            else if (category == "Mobs")
            {
                insertItem(category);
            }
            else if (category == "NPC Ship Hulls")
            {
                insertItem(category);
            }
            else if (category == "Progen")
            {
                insertItem(category);
            }
            else if (category == "Pvp")
            {
                insertItem(category);
            }
            else if (category == "Terran")
            {
                insertItem(category);
            }
            else if (category == "Turrets")
            {
                insertItem(category);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in listView1.SelectedItems)
            {
                selectedID = (int) item.Tag;
                mainFrm.setupMainImage(selectedID);
                mainFrm.baseAssetText.Text = selectedID.ToString();
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