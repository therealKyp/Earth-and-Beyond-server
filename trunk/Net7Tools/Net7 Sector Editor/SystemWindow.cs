using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

using UMD.HCIL.Piccolo;
using UMD.HCIL.Piccolo.Event;
using UMD.HCIL.Piccolo.Nodes;
using UMD.HCIL.Piccolo.Util;

using N7.Utilities;
using N7.Sql;
using N7.Sprites;

namespace N7
{
    public class SystemWindow
    {
        private PCanvas canvas;
        private PLayer masterLayer;
        private PLayer sectorLayer;
        private PNode pSelectedNode;
        private DataRow dr;
        private PropertyGrid _pg;

        public SystemWindow(PCanvas pcanvas, String systemName, DataRow[] sectorTable, PropertyGrid pg, DataRow selectedRow)
        {
            canvas = pcanvas;
            dr = selectedRow;
            _pg = pg;

            sectorLayer = new PLayer();

            //Setup Mouse Wheel Zoom instead of regular piccollo zoom.
            new MouseWheelZoomController(canvas.Camera);

            //Setup Default Background Color
            canvas.BackColor = Color.Black;

            //Create the master Layer
            masterLayer = canvas.Layer;

            for (int i = 0; i < sectorTable.Length; i++)
			{
                String sectorName = sectorTable[i]["name"].ToString();
                float xmin = float.Parse(sectorTable[i]["x_min"].ToString());
                float xmax = float.Parse(sectorTable[i]["x_max"].ToString());
                float ymin = float.Parse(sectorTable[i]["y_min"].ToString());
                float ymax = float.Parse(sectorTable[i]["y_max"].ToString());
                float x = float.Parse(sectorTable[i]["galaxy_x"].ToString());
                float y = float.Parse(sectorTable[i]["galaxy_y"].ToString());

                DataRow r = sectorTable[i];
                new SectorSprite(sectorLayer, pg, r, sectorName, x, y, xmin, xmax, ymin, ymax);
			}

            masterLayer.AddChild(sectorLayer);

            //create events
            masterLayer.MouseDown += new PInputEventHandler(MasterLayer_OnMouseDown);
        }

        public void pg_PropertyValueChanged(Object sender, PropertyValueChangedEventArgs e)
        {
            String propertyName = e.ChangedItem.PropertyDescriptor.Name;

            try
            {
                if (pSelectedNode.Tag.GetType().ToString().Contains("SectorSprite"))
                {
                    SectorSprite tmp = (SectorSprite)pSelectedNode.Tag;
                    tmp.updateChangedInfo(propertyName, e.ChangedItem.Value.ToString());
                }
            }
            catch (Exception)
            {
              updateChangedInfo(propertyName, e.ChangedItem.Value);
            }
        }

        public void MasterLayer_OnMouseDown(object sender, PInputEventArgs e)
        {
                if (e.PickedNode.Tag.GetType().ToString().Contains("SectorSprite"))
                {
                    setOriginalText(pSelectedNode);

                    SectorSprite tmp2 = (SectorSprite)e.PickedNode.Tag;
                    PText pnameNew = tmp2.getText();
                    pnameNew.TextBrush = Brushes.Red;
                }
                pSelectedNode = e.PickedNode;
        }

        public void setOriginalText(PNode pickedNode)
        {
            if (pSelectedNode != null)
            {
                if (pickedNode.Tag.GetType().ToString().Contains("SectorSprite"))
                {
                    SectorSprite tmp = (SectorSprite)pSelectedNode.Tag;
                    PText pname = tmp.getText();
                    pname.TextBrush = Brushes.White;
                }
            }
        }

        public void updateChangedInfo(String propertyName, Object _changedValue)
        {
            String value = _changedValue.ToString();
            String changedValue = value.Replace("'", "''");

            if (propertyName == "Name")
            {
                dr["name"] = changedValue;
            }
            else if (propertyName == "GalaxyX")
            {
                dr["galaxy_x"] = float.Parse(changedValue);
            }
            else if (propertyName == "GalaxyY")
            {
                dr["galaxy_y"] = float.Parse(changedValue);
            }
            else if (propertyName == "GalaxyZ")
            {
                dr["galaxy_z"] = float.Parse(changedValue);
            }
            else if (propertyName == "Color")
            {
                Color color = (Color) _changedValue;

                dr["color_r"] = color.R;
                dr["color_g"] = color.G;
                dr["color_b"] = color.B;
            }
            else if (propertyName == "Notes")
            {
                dr["notes"] = changedValue;
            }

            if (dr.RowState != DataRowState.Modified)
            {
                dr.SetModified();
            }
        }

        public void newSector(DataRow ndr)
        {
            String sectorName = ndr["name"].ToString();
            float xmin = float.Parse(ndr["x_min"].ToString());
            float xmax = float.Parse(ndr["x_max"].ToString());
            float ymin = float.Parse(ndr["y_min"].ToString());
            float ymax = float.Parse(ndr["y_max"].ToString());
            float x = float.Parse(ndr["galaxy_x"].ToString());
            float y = float.Parse(ndr["galaxy_y"].ToString());

            new SectorSprite(sectorLayer, _pg, ndr, sectorName, x, y, xmin, xmax, ymin, ymax);
        }
    }
}
