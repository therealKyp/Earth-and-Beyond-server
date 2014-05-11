using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace N7.GUI
{
    public partial class HarvestableResTypes : Form
    {
        public IWindowsFormsEditorService _wfes;
        private int _id;
        private DataTable loadTypes;

        public HarvestableResTypes()
        {
            
            DataTable tmp = Database.executeQuery(Database.DatabaseName.net7, "SELECT Auto_increment FROM information_schema.tables WHERE table_name='sector_objects' and table_schema='net7';");
            int autoID = 0;
            foreach (DataRow z in tmp.Rows)
            {
                autoID = int.Parse(z["Auto_increment"].ToString());
            }

            DataTable tmp2 = Database.executeQuery(Database.DatabaseName.net7, "SELECT sector_object_id FROM sector_objects where sector_object_id='" + mainFrm.selectedObjectID + "';");
            int id = 0;
            foreach (DataRow z in tmp2.Rows)
            {
                id = int.Parse(z["sector_object_id"].ToString());
            }

            if (id != 0)
            {
                _id = mainFrm.selectedObjectID;
            }
            else
            {
                _id = autoID;
            }

            InitializeComponent();
        }

        private void HarvestableResTypes_Load(object sender, EventArgs e)
        {
            String query = "SELECT * FROM sector_objects_harvestable_restypes where group_id='"+_id+"';";
            loadTypes = Database.executeQuery(Database.DatabaseName.net7, query);

            foreach (DataRow i in loadTypes.Rows)
            {
                int type = int.Parse(i["type"].ToString());
                switch (type)
                {
                    case 0x71E:
                        listBox1.Items.Remove("Glowing Asteroid 1");
                        listBox2.Items.Add("Glowing Asteroid 1");
                        break;
                    case 0x71F:
                        listBox1.Items.Remove("Glowing Asteroid 2");
                        listBox2.Items.Add("Glowing Asteroid 2");
                        break;
                    case 0x720:
                        listBox1.Items.Remove("Glowing Asteroid 3");
                        listBox2.Items.Add("Glowing Asteroid 3");
                        break;
                    case 0x721:
                        listBox1.Items.Remove("Asteroid 1");
                        listBox2.Items.Add("Asteroid 1");
                        break;
                    case 0x722:
                        listBox1.Items.Remove("Asteroid 2");
                        listBox2.Items.Add("Asteroid 2");
                        break;
                    case 0x723:
                        listBox1.Items.Remove("Asteroid 3");
                        listBox2.Items.Add("Asteroid 3");
                        break;
                    case 0x724:
                        listBox1.Items.Remove("Hydrocarbon Deposit 1");
                        listBox2.Items.Add("Hydrocarbon Deposit 1");
                        break;
                    case 0x725:
                        listBox1.Items.Remove("Hydrocarbon Deposit 2");
                        listBox2.Items.Add("Hydrocarbon Deposit 2");
                        break;
                    case 0x726:
                        listBox1.Items.Remove("Hydrocarbon Deposit 3");
                        listBox2.Items.Add("Hydrocarbon Deposit 3");
                        break;
                    case 0x727:
                        listBox1.Items.Remove("Crystalline Asteroid 1");
                        listBox2.Items.Add("Crystalline Asteroid 1");
                        break;
                    case 0x728:
                        listBox1.Items.Remove("Crystalline Asteroid 2");
                        listBox2.Items.Add("Crystalline Asteroid 2");
                        break;
                    case 0x729:
                        listBox1.Items.Remove("Crystalline Asteroid 3");
                        listBox2.Items.Add("Crystalline Asteroid 3");
                        break;
                    case 0x72a:
                        listBox1.Items.Remove("Gas Cloud");
                        listBox2.Items.Add("Gas Cloud");
                        break;
                    case 1131:
                        listBox1.Items.Remove("Inorganic Hulk 01");
                        listBox2.Items.Add("Inorganic Hulk 01");
                        break;
                    case 1132:
                        listBox1.Items.Remove("Organic Hulk 01");
                        listBox2.Items.Add("Organic Hulk 01");
                        break;
                }
            } 
        }

        private void button2_Click(object sender, EventArgs e)
        {
            String item = listBox1.SelectedItem.ToString();

            int type = 1823;
            if (item == "Glowing Asteroid 1")
            {
                type = 0x71E;
            }
            else if (item == "Glowing Asteroid 2")
            {
                type = 1823;
            }
            else if (item == "Glowing Asteroid 3")
            {
                type = 0x720;
            }
            else if (item == "Asteroid 1")
            {
                type = 0x721;
            }
            else if (item == "Asteroid 2")
            {
                type = 0x722;
            }
            else if (item == "Asteroid 3")
            {
                type = 0x723;
            }
            else if (item == "Hydrocarbon Deposit 1")
            {
                type = 0x724;
            }
            else if (item == "Hydrocarbon Deposit 2")
            {
                type = 0x725;
            }
            else if (item == "Hydrocarbon Deposit 3")
            {
                type = 0x726;
            }
            else if (item == "Crystalline Asteroid 1")
            {
                type = 0x727;
            }
            else if (item == "Crystalline Asteroid 2")
            {
                type = 0x728;
            }
            else if (item == "Crystalline Asteroid 3")
            {
                type = 0x729;
            }
            else if (item == "Gas Cloud")
            {
                type = 0x72a;
            }
            else if (item == "Inorganic Hulk 01")
            {
                type = 1131;
            }
            else if (item == "Organic Hulk 01")
            {
                type = 1132;
            }

            listBox2.Items.Add(item);
            listBox1.Items.Remove(item);
            String query = "INSERT INTO sector_objects_harvestable_restypes SET group_id='" + _id + "', type='" + type + "';";
            DataTable resTypes = Database.executeQuery(Database.DatabaseName.net7, query);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            String item = listBox2.SelectedItem.ToString();

            int type = 1823;
            if (item == "Glowing Asteroid 1")
            {
                type = 0x71E;
            }
            else if (item == "Glowing Asteroid 2")
            {
                type = 1823;
            }
            else if (item == "Glowing Asteroid 3")
            {
                type = 0x720;
            }
            else if (item == "Asteroid 1")
            {
                type = 0x721;
            }
            else if (item == "Asteroid 2")
            {
                type = 0x722;
            }
            else if (item == "Asteroid 3")
            {
                type = 0x723;
            }
            else if (item == "Hydrocarbon Deposit 1")
            {
                type = 0x724;
            }
            else if (item == "Hydrocarbon Deposit 2")
            {
                type = 0x725;
            }
            else if (item == "Hydrocarbon Deposit 3")
            {
                type = 0x726;
            }
            else if (item == "Crystalline Asteroid 1")
            {
                type = 0x727;
            }
            else if (item == "Crystalline Asteroid 2")
            {
                type = 0x728;
            }
            else if (item == "Crystalline Asteroid 3")
            {
                type = 0x729;
            }
            else if (item == "Gas Cloud")
            {
                type = 0x72a;
            }
            else if (item == "Inorganic Hulk 01")
            {
                type = 1131;
            }
            else if (item == "Organic Hulk 01")
            {
                type = 1132;
            }

            listBox1.Items.Add(item);
            listBox2.Items.Remove(item);
            String query = "DELETE FROM sector_objects_harvestable_restypes where group_id='"+_id+"' and type='"+type+"';";
            DataTable resTypes = Database.executeQuery(Database.DatabaseName.net7, query);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}