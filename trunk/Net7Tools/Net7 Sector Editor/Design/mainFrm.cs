using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using UMD.HCIL.Piccolo;
using N7.Sql;

namespace N7
{
    public partial class mainFrm : Form
    {
        public mainFrm()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            pCanvas1.BackColor = Color.Black;
            Systems systems = new Systems();
            Sectors sectors = new Sectors();
            TreeWindow treeWindow = new TreeWindow(systems.getSystemTable(), sectors.getSectorTable());
            treeView1.Nodes.AddRange(treeWindow.setupInitialTree());
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            SectorTab.Text = e.Node.Text;
            if (e.Node.Level == 0)
            {
                //sectorWindow.createSystem();
            }
            else if (e.Node.Level == 1)
            {
                    pCanvas1.Dispose();

                    this.pCanvas1 = new UMD.HCIL.Piccolo.PCanvas();
                    this.SectorTab.Controls.Add(this.pCanvas1);
                    this.pCanvas1.AllowDrop = true;
                    this.pCanvas1.BackColor = System.Drawing.Color.White;
                    this.pCanvas1.Dock = System.Windows.Forms.DockStyle.Fill;
                    this.pCanvas1.GridFitText = false;
                    this.pCanvas1.Location = new System.Drawing.Point(0, 0);
                    this.pCanvas1.Name = "pCanvas1";
                    this.pCanvas1.RegionManagement = true;
                    this.pCanvas1.Size = new System.Drawing.Size(554, 438);
                    this.pCanvas1.TabIndex = 0;
                    this.pCanvas1.Text = "pCanvas1";

                    SectorWindow sectorWindow = new SectorWindow(pCanvas1, e.Node.Text.ToString());
            }
        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void toolStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }
    }
}