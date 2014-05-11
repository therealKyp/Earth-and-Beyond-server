using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CommonTools.Gui
{
    public partial class DlgEditXml : Form
    {
        private Boolean m_madeSelection;
        private String m_xml;

        public DlgEditXml()
        {
            InitializeComponent();
            m_xml = "";
        }

        public void setXml(String xml)
        {
            m_xml = xml;
            guiXmlTxt.Text = m_xml;
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);
            m_madeSelection = false;
            guiXmlTxt.Focus();
            guiXmlTxt.Select(0, 0);
            guiXmlTxt.TextChanged += new System.EventHandler(onTextChanged);
        }
        private void onOk(object sender, EventArgs e)
        {
            m_madeSelection = true;
            m_xml = guiXmlTxt.Text;
            this.Close();
        }

        private void onCancel(object sender, EventArgs e)
        {
            this.Close();
        }

        public Boolean getValues(out String xml)
        {
            xml = m_xml;
            return m_madeSelection;
        }

        private void onTextChanged(object sender, EventArgs e)
        {
            // Button only needs to be enabled once so remove the event
            guiOkBtn.Enabled = true;
            guiXmlTxt.TextChanged -= new System.EventHandler(onTextChanged);
        }

        private void onKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Control && e.KeyCode == Keys.A)
            {
                guiXmlTxt.SelectAll();
            }
        }
    }
}
