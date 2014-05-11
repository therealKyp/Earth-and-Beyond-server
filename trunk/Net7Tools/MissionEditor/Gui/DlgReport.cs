using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace MissionEditor.Gui
{
    public partial class DlgReport : Form
    {
        public DlgReport()
        {
            InitializeComponent();
        }

        public void set(String html)
        {
            /*if (guiWebBrowser.Document == null)
            {
                guiWebBrowser.Document = new HtmlDocument();
            }
            guiWebBrowser.Document.OpenNew(true);
            guiWebBrowser.DocumentText = html;*/
            if (guiWebBrowser.Document != null)
            {
                guiWebBrowser.Document.OpenNew(true);
                guiWebBrowser.Document.Write(html);
            }
            else
            {
                guiWebBrowser.DocumentText = html;
            }
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);
        }
    }
}
