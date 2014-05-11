using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Data;

using UMD.HCIL.Piccolo;
using UMD.HCIL.Piccolo.Event;
using UMD.HCIL.Piccolo.Nodes;
using UMD.HCIL.Piccolo.Util;

using N7.Props;

namespace N7.Sprites
{
    class SectorSprite
    {
        private PropertyGrid _pg;
        private SectorProps sp;
        private PText pname;
        private DataRow dr;

        public SectorSprite(PLayer layer, PropertyGrid pg, DataRow r, String name, float galaxy_x, float galaxy_y, float xmin, float xmax, float ymin, float ymax)
        {
            _pg = pg;
            dr = r;

            setupData(r);

            float width = (xmax - xmin) / 1000;
            float height = (ymax - ymin) / 1000;

            Color penColor = Color.Honeydew;

            Pen sigPen = new Pen(penColor, 2.0F);
            sigPen.DashStyle = DashStyle.Dash;

            PPath sigCircle = PPath.CreateEllipse(galaxy_x, galaxy_y, width, width);
            sigCircle.Pen = sigPen;
            sigCircle.Brush = Brushes.Transparent;

            pname = new PText(name);
            pname.TextBrush = Brushes.White;
            pname.TextAlignment = StringAlignment.Center;
            pname.X = sigCircle.X + ((sigCircle.Width / 2) - (pname.Width / 2));
            pname.Y = sigCircle.Y + (sigCircle.Height / 2);

            sigCircle.Tag = this;

            //Display Object by adding them to its layer
            sigCircle.AddChild(pname);
            layer.AddChild(sigCircle);

            sigCircle.ChildrenPickable = false;

            // Attach event delegates directly to the node.
            sigCircle.MouseDown += new PInputEventHandler(Image_MouseDown);
        }

        private void setupData(DataRow r)
        {
            //Retrieve Properties from sql row.
            String sectorName = r["name"].ToString();
            float xmin = float.Parse(r["x_min"].ToString());
            float xmax = float.Parse(r["x_max"].ToString());
            float ymin = float.Parse(r["y_min"].ToString());
            float ymax = float.Parse(r["y_max"].ToString());
            float zmin = float.Parse(r["z_min"].ToString());
            float zmax = float.Parse(r["z_max"].ToString());
            int gridx = int.Parse(r["grid_x"].ToString());
            int gridy = int.Parse(r["grid_y"].ToString());
            int gridz = int.Parse(r["grid_z"].ToString());
            float fognear = float.Parse(r["fog_near"].ToString());
            float fogfar = float.Parse(r["fog_far"].ToString());
            int debrismode = int.Parse(r["debris_mode"].ToString());
            bool lightbackdrop = (Boolean) r["light_backdrop"];
            bool fogbackdrop = (Boolean) r["fog_backdrop"];
            bool swapbackdrop = (Boolean) r["swap_backdrop"];
            float backdropfognear = float.Parse(r["backdrop_fog_near"].ToString());
            float backdropfogfar = float.Parse(r["backdrop_fog_far"].ToString());
            float maxtilt = float.Parse(r["max_tilt"].ToString());
            bool autolevel = (Boolean)r["auto_level"];
            float impulserate = float.Parse(r["impulse_rate"].ToString());
            float decayvelocity = float.Parse(r["decay_velocity"].ToString());
            float decayspin = float.Parse(r["decay_spin"].ToString());
            int backdropasset = int.Parse(r["backdrop_asset"].ToString());
            String greetings = r["greetings"].ToString();
            String notes = r["notes"].ToString();
            int systemid = int.Parse(r["system_id"].ToString());
            float galaxyx = float.Parse(r["galaxy_x"].ToString());
            float galaxyy = float.Parse(r["galaxy_y"].ToString());
            float galaxyz = float.Parse(r["galaxy_z"].ToString());
            int sector_type = int.Parse(r["sector_type"].ToString());

            float width = xmax - xmin;
            float height = ymax - ymin;
            float depth = zmax - zmin;

            //Populate Properties
            sp = new SectorProps();
            sp.Name = sectorName;
            sp.Width = width;
            sp.Height = height;
            sp.Depth = depth;
            sp.GridX = gridx;
            sp.GridY = gridy;
            sp.GridZ = gridz;
            sp.FogNear = fognear;
            sp.FogFar = fogfar;
            sp.DebrisMode = debrismode;
            sp.LightBackdrop = lightbackdrop;
            sp.FogBackdrop = fogbackdrop;
            sp.SwapBackdrop = swapbackdrop;
            sp.BackdropFogNear = backdropfognear;
            sp.BackdropFogFar = backdropfogfar;
            sp.MaxTilt = maxtilt;
            sp.AutoLevel = autolevel;
            sp.ImpulseRate = impulserate;
            sp.DecayVelocity = decayvelocity;
            sp.DecaySpin = decayspin;
            sp.BackdropAsset = backdropasset;
            sp.Greetings = greetings;
            sp.Notes = notes;
            sp.SystemID = systemid;
            sp.GalaxyX = galaxyx;
            sp.GalaxyY = galaxyy;
            sp.GalaxyZ = galaxyz;

            String oSector = "";
            switch (sector_type)
            {
                case 0:
                    oSector = "Space Sector";
                    break;
                case 1:
                    oSector = "Rocky Planet Surface";
                    break;
                case 2:
                    oSector = "Gas Giant Surface";
                    break;;
            }

            sp.SectorType = oSector;
        }

        protected void Image_MouseDown(object sender, PInputEventArgs e)
        {
            _pg.SelectedObject = sp;
        }

        public PText getText()
        {
            return pname;
        }

        public void updateChangedInfo(String propertyName, String _changedValue)
        {
            String changedValue = _changedValue.Replace("'", "''");
            if (propertyName == "Name")
            {
                dr["name"] = changedValue;
            }
            else if (propertyName == "Width" || propertyName == "Height" || propertyName == "Depth")
            {
                float xmin = 0;
                float xmax = 0;
                float ymin = 0;
                float ymax = 0;
                float zmin = 0;
                float zmax = 0;

                if (sp.Width != 0)
                {
                    xmin = -(sp.Width / 2);
                    xmax = sp.Width / 2;
                }
                if (sp.Height != 0)
                {
                    ymin = -(sp.Height / 2);
                    ymax = sp.Height / 2;
                }
                if (sp.Depth != 0)
                {
                    zmin = -(sp.Depth / 2);
                    zmax = sp.Depth / 2;
                }

                dr["x_min"] = xmin;
                dr["x_max"] = xmax;
                dr["y_min"] = ymin;
                dr["y_max"] = ymax;
                dr["z_min"] = zmin;
                dr["z_max"] = zmax;
            }
            else if (propertyName == "GridX")
            {
                dr["grix_x"] = int.Parse(changedValue);
            }
            else if (propertyName == "GridY")
            {
                dr["grix_y"] = int.Parse(changedValue);
            }
            else if (propertyName == "GridZ")
            {
                dr["grix_z"] = int.Parse(changedValue);
            }
            else if (propertyName == "FogNear")
            {
                dr["fog_near"] = float.Parse(changedValue);
            }
            else if (propertyName == "FogFar")
            {
                dr["fog_far"] = float.Parse(changedValue);
            }
            else if (propertyName == "DebrisMode")
            {
                dr["debris_mode"] = int.Parse(changedValue);
            }
            else if (propertyName == "LightBackdrop")
            {
                dr["light_backdrop"] = bool.Parse(changedValue);
            }
            else if (propertyName == "FogBackdrop")
            {
                dr["fog_backdrop"] = bool.Parse(changedValue);
            }
            else if (propertyName == "SwapBackdrop")
            {
                dr["swap_backdrop"] = bool.Parse(changedValue); ;
            }
            else if (propertyName == "BackdropFogNear")
            {
                dr["backdrop_fog_near"] = float.Parse(changedValue);
            }
            else if (propertyName == "BackdropFogFar")
            {
                dr["backdrop_fog_far"] = float.Parse(changedValue);
            }
            else if (propertyName == "MaxTilt")
            {
                dr["mex_tilt"] = float.Parse(changedValue);
            }
            else if (propertyName == "AutoLevel")
            {
                dr["auto_level"] = bool.Parse(changedValue);
            }
            else if (propertyName == "ImpulseRate")
            {
                dr["impulse_rate"] = float.Parse(changedValue);
            }
            else if (propertyName == "DecayVelocity")
            {
                dr["decay_velocity"] = float.Parse(changedValue);
            }
            else if (propertyName == "DecaySpin")
            {
                dr["decay_spin"] = float.Parse(changedValue);
            }
            else if (propertyName == "BackdropAsset")
            {
                dr["backdrop_asset"] = int.Parse(changedValue);
            }
            else if (propertyName == "Greetings")
            {
                dr["greetings"] = changedValue;
            }
            else if (propertyName == "Notes")
            {
                dr["notes"] = changedValue;
            }
            else if (propertyName == "SystemID")
            {
                dr["system_id"] = int.Parse(changedValue);
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
            else if (propertyName == "SectorType")
            {
                if (changedValue == "Space Sector")
                {
                    dr["sector_type"] = 0;
                }
                else if (changedValue == "Rocky Planet Surface")
                {
                    
                    dr["sector_type"] = 1;
                }
                else if (changedValue == "Gas Giant Surface")
                {
                    dr["sector_type"] = 2;
                }
            }

            if (dr.RowState != DataRowState.Modified)
            {
                dr.SetModified();
            }
        }
    }
}
