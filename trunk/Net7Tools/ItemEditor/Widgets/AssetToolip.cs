using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Net7_Tools.Database;
using System.Drawing;

namespace Net7_Tools.Widgets
{
    class AssetToolip
    {
        private Control m_popupSource;
        private Control m_dataSource;
        private ToolTip m_toolTip;

        public AssetToolip(Control popupSource, Control dataSource)
        {
            m_popupSource = popupSource;
            m_dataSource = dataSource;
            m_toolTip = new ToolTip();
            m_toolTip.OwnerDraw = true;
            //m_toolTip.AutoPopDelay = 1000;
            m_toolTip.InitialDelay = 500;
            m_toolTip.ReshowDelay = 10;
            m_toolTip.Popup += new PopupEventHandler(tooltip_Popup);
            m_toolTip.Draw += new DrawToolTipEventHandler(tooltip_Draw);
            reactivate();
            if (popupSource.GetType() == typeof(Button))
            {
                popupSource.Click += new EventHandler(popupSource_Click);
            }
        }

        void popupSource_Click(object sender, EventArgs e)
        {
            // Crazy stuff:  clicking the button unlinks the tooltip
            reactivate();
        }

        public void reactivate()
        {
            // The text must be non-empty
            m_toolTip.SetToolTip(m_popupSource, "Asset Tooltip");
        }

        public void tooltip_Popup(object sender, PopupEventArgs e)
        {
            String assetId = m_dataSource.Text;
            AssetInformation assetInformation = Database.Database.getAsset(assetId);
            if (assetInformation != null)
            {
                Image image = Search.FrmAsset.getImage(assetInformation.filename,
                                                       assetInformation.base_id.ToString(),
                                                       assetInformation.descr, 
                                                       out assetInformation.filename);
                if (image != null)
                {
                    e.ToolTipSize = new Size(image.Width, image.Height);
                }
            }
        }

        public void tooltip_Draw(object sender, DrawToolTipEventArgs e)
        {
            String assetId = m_dataSource.Text;
            AssetInformation assetInformation = Database.Database.getAsset(assetId);

            if (assetInformation != null)
            {
                // Draw the standard background.
                //e.DrawBackground();

                // Draw the standard background
                //e.DrawBorder();

                Image image = Search.FrmAsset.getImage(assetInformation.filename,
                                                       assetInformation.base_id.ToString(),
                                                       assetInformation.descr,
                                                       out assetInformation.filename);
                if (image != null)
                {
                    e.Graphics.DrawImage(image, 0, 0);
                }

                /*
                // Draw the custom border to appear 3-dimensional.
                e.Graphics.DrawLines(SystemPens.ControlLightLight, new Point[] {
                    new Point (0, e.Bounds.Height - 1), 
                    new Point (0, 0), 
                    new Point (e.Bounds.Width - 1, 0)
                });
                e.Graphics.DrawLines(SystemPens.ControlDarkDark, new Point[] {
                    new Point (0, e.Bounds.Height - 1), 
                    new Point (e.Bounds.Width - 1, e.Bounds.Height - 1), 
                    new Point (e.Bounds.Width - 1, 0)
                });
                */
            }
        }
    }
}
