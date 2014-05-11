using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.Collections;

using UMD.HCIL.Piccolo;
using UMD.HCIL.Piccolo.Event;
using UMD.HCIL.Piccolo.Nodes;
using UMD.HCIL.Piccolo.Util;

using N7.Utilities;
using N7.Sql;
using N7.Sprites;
using N7.Props;
using N7.GUI;

namespace N7
{
    public class SectorWindow
    {
        private PCanvas canvas;
        private PLayer boundsLayer;
        private PLayer masterLayer;
        private PLayer mobsLayer;
        private PLayer planetsLayer;
        private PLayer stargatesLayer;
        private PLayer starbasesLayer;
        private PLayer decorationsLayer;
        private PLayer harvestableLayer;
        private PNode pSelectedNode;
        private SectorObjectsSql so;
        private DataRow dr;
        private Hashtable deletedObjectsID = new Hashtable();
        private Hashtable deletedObjectsType = new Hashtable();
        private PropertyGrid _pg;
        private NewSectorObject _nso;
        private SectorProps sp;
        private DataGridView _dgv;

        public SectorWindow(PCanvas pcanvas, DataRow[] sectorRows, PropertyGrid pg, DataGridView dgv)
        {
            canvas = pcanvas;
            dr = sectorRows[0];
            _pg = pg;
            _dgv = dgv;

            //Setup Mouse Wheel Zoom type based on user settings.
            int zoomType = Properties.Settings.Default.zoomSelection;

            switch (zoomType)
            {
                case 0:
                    new MouseWheelZoomController(canvas.Camera);
                    break;
            }

            //Setup Default Background Color
            canvas.BackColor = Color.Black;

            //Create the master Layer
            masterLayer = canvas.Layer;

            //Initialize object Layers
            boundsLayer = new PLayer();
            mobsLayer = new PLayer();
            planetsLayer = new PLayer();
            stargatesLayer = new PLayer();
            starbasesLayer = new PLayer();
            decorationsLayer = new PLayer();
            harvestableLayer = new PLayer();

            //Retrieve Properties from sql row.
            String sectorName = sectorRows[0]["name"].ToString();
            int sectorID = int.Parse(sectorRows[0]["sector_id"].ToString());
            float xmin = float.Parse(sectorRows[0]["x_min"].ToString());
            float xmax = float.Parse(sectorRows[0]["x_max"].ToString());
            float ymin = float.Parse(sectorRows[0]["y_min"].ToString());
            float ymax = float.Parse(sectorRows[0]["y_max"].ToString());
            float zmin = float.Parse(sectorRows[0]["z_min"].ToString());
            float zmax = float.Parse(sectorRows[0]["z_max"].ToString());
            int gridx = int.Parse(sectorRows[0]["grid_x"].ToString());
            int gridy = int.Parse(sectorRows[0]["grid_y"].ToString());
            int gridz = int.Parse(sectorRows[0]["grid_z"].ToString());
            float fognear = float.Parse(sectorRows[0]["fog_near"].ToString());
            float fogfar = float.Parse(sectorRows[0]["fog_far"].ToString());
            int debrismode = int.Parse(sectorRows[0]["debris_mode"].ToString());
            bool lightbackdrop = (Boolean) sectorRows[0]["light_backdrop"];
            bool fogbackdrop = (Boolean) sectorRows[0]["fog_backdrop"];
            bool swapbackdrop = (Boolean) sectorRows[0]["swap_backdrop"];
            float backdropfognear = float.Parse(sectorRows[0]["backdrop_fog_near"].ToString());
            float backdropfogfar = float.Parse(sectorRows[0]["backdrop_fog_far"].ToString());
            float maxtilt = float.Parse(sectorRows[0]["max_tilt"].ToString());
            bool autolevel = (Boolean) sectorRows[0]["auto_level"];
            float impulserate = float.Parse(sectorRows[0]["impulse_rate"].ToString());
            float decayvelocity = float.Parse(sectorRows[0]["decay_velocity"].ToString());
            float decayspin = float.Parse(sectorRows[0]["decay_spin"].ToString());
            int backdropasset = int.Parse(sectorRows[0]["backdrop_asset"].ToString());
            String greetings = sectorRows[0]["greetings"].ToString();
            String notes = sectorRows[0]["notes"].ToString();
            int systemid = int.Parse(sectorRows[0]["system_id"].ToString());
            float galaxyx = float.Parse(sectorRows[0]["galaxy_x"].ToString());
            float galaxyy = float.Parse(sectorRows[0]["galaxy_y"].ToString());
            float galaxyz = float.Parse(sectorRows[0]["galaxy_z"].ToString());
            int sector_type = int.Parse(sectorRows[0]["sector_type"].ToString());

            //Load Sector Object Sql.
            so = new SectorObjectsSql(sectorName);
            DataTable sot = so.getSectorObject();

            float width = xmax - xmin;
            float height = ymax - ymin;
            float depth = zmax - zmin;

            //Populate Properties
            sp = new SectorProps();
            sp.Name = sectorName;
            sp.SectorID = sectorID;
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
                    break; ;
            }

            sp.SectorType = oSector;

            pg.SelectedObject = sp;

            //Create Sector Bounds
            new SectorBoundsSprite(boundsLayer, xmin, ymin, xmax, ymax);

            //Create All Sector Object sprites
            foreach (DataRow r in sot.Rows)
            {
                int type = int.Parse(r["type"].ToString());

                switch (type)
                {
                    case 0:
                        new MobSprite(mobsLayer, r, pg, dgv);
                        break;
                    case 3:
                        new PlanetSprite(planetsLayer, r, pg, dgv);
                        break;
                    case 11:
                        new StargateSprite(stargatesLayer, r, pg, dgv);
                        break;
                    case 12:
                        new StarbaseSprite(starbasesLayer, r, pg, dgv);
                        break;
                    case 37:
                        new DecorationSprite(decorationsLayer, r, pg, dgv);
                        break;
                    case 38:
                        new HarvestableSprite(harvestableLayer, r, pg, dgv);
                        break;

                }
            }

            //Attach all layers to their master
            masterLayer.AddChild(boundsLayer);
            masterLayer.AddChild(mobsLayer);
            masterLayer.AddChild(planetsLayer);
            masterLayer.AddChild(stargatesLayer);
            masterLayer.AddChild(starbasesLayer);
            masterLayer.AddChild(decorationsLayer);
            masterLayer.AddChild(harvestableLayer);

            //create events
            masterLayer.MouseDown += new PInputEventHandler(MasterLayer_OnMouseDown);
            
            canvas.Camera.MouseDown += new PInputEventHandler(canvasCamera_MouseDown);

            //Zoom all the way out.
            canvas.Camera.ViewScale = .375f;
        }

        public SectorObjectsSql getSectorObjectsSQL()
        {
            return so;
        }

        public void updateChangedInfo(String propertyName, String _changedValue)
        {
            String changedValue = _changedValue.Replace("'","''");
            if (propertyName == "Name")
            {
                dr["name"] = changedValue;
            }
            else if (propertyName == "SectorID")
            {
                dr["sector_id"] = int.Parse(changedValue);
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

                float x = -(sp.Width / 2) / 100;
                float y = -(sp.Height / 2) / 100;

                boundsLayer.GetChild(0).X = x;
                boundsLayer.GetChild(0).Y = y;
                boundsLayer.GetChild(0).Width = sp.Width / 100;
                boundsLayer.GetChild(0).Height = sp.Height / 100;
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
                dr["swap_backdrop"] = bool.Parse(changedValue);;
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

        #region Events
        public void PropertyGrid1_PropertyValueChanged(Object sender, PropertyValueChangedEventArgs e)
        {
            String propertyName = e.ChangedItem.PropertyDescriptor.Name;

            try
            {
                if (pSelectedNode.Tag.GetType().ToString().Contains("MobSprite"))
                {
                    MobSprite tmp = (MobSprite)pSelectedNode.Tag;
                    tmp.updateChangedInfo(propertyName, e.ChangedItem.Value);
                }
                else if (pSelectedNode.Tag.GetType().ToString().Contains("PlanetSprite"))
                {
                    PlanetSprite tmp = (PlanetSprite)pSelectedNode.Tag;
                    tmp.updateChangedInfo(propertyName, e.ChangedItem.Value);
                }
                else if (pSelectedNode.Tag.GetType().ToString().Contains("StargateSprite"))
                {
                    StargateSprite tmp = (StargateSprite)pSelectedNode.Tag;
                    tmp.updateChangedInfo(propertyName, e.ChangedItem.Value);
                }
                else if (pSelectedNode.Tag.GetType().ToString().Contains("StarbaseSprite"))
                {
                    StarbaseSprite tmp = (StarbaseSprite)pSelectedNode.Tag;
                    tmp.updateChangedInfo(propertyName, e.ChangedItem.Value);
                }
                else if (pSelectedNode.Tag.GetType().ToString().Contains("HarvestableSprite"))
                {
                    HarvestableSprite tmp = (HarvestableSprite)pSelectedNode.Tag;
                    tmp.updateChangedInfo(propertyName, e.ChangedItem.Value);
                }
                else if (pSelectedNode.Tag.GetType().ToString().Contains("DecorationSprite"))
                {
                    DecorationSprite tmp = (DecorationSprite)pSelectedNode.Tag;
                    tmp.updateChangedInfo(propertyName, e.ChangedItem.Value);
                }
            }
            catch (Exception)
            {
                updateChangedInfo(propertyName, e.ChangedItem.Value.ToString());
            }
        }


        public void canvasCamera_MouseDown(object sender, PInputEventArgs e)
        {
            if (_nso != null)
            {
                _nso.setPosition(e.Position);
                _nso.Show();
                _nso = null;
            }
        }

        public void MasterLayer_OnMouseDown(object sender, PInputEventArgs e)
        {
            int id = 0;
            if (e.Button == MouseButtons.Right)
            {
            }
            else
            {
                if (e.PickedNode.ChildrenCount >= 3)
                {
                    if (e.PickedNode.Tag.GetType().ToString().Contains("MobSprite"))
                    {
                        setOriginalText(pSelectedNode);

                        MobSprite tmp2 = (MobSprite)e.PickedNode.Tag;
                        PText pnameNew = tmp2.getText();
                        pnameNew.TextBrush = Brushes.Red;

                        DataRow dr = tmp2.getRow();
                        id = int.Parse(dr["sector_object_id"].ToString());
                    }
                    if (e.PickedNode.Tag.GetType().ToString().Contains("PlanetSprite"))
                    {
                        setOriginalText(pSelectedNode);

                        PlanetSprite tmp2 = (PlanetSprite)e.PickedNode.Tag;
                        PText pnameNew = tmp2.getText();
                        pnameNew.TextBrush = Brushes.Red;

                        DataRow dr = tmp2.getRow();
                        id = int.Parse(dr["sector_object_id"].ToString());
                    }
                    if (e.PickedNode.Tag.GetType().ToString().Contains("StargateSprite"))
                    {
                        setOriginalText(pSelectedNode);

                        StargateSprite tmp2 = (StargateSprite)e.PickedNode.Tag;
                        PText pnameNew = tmp2.getText();
                        pnameNew.TextBrush = Brushes.Red;

                        DataRow dr = tmp2.getRow();
                        id = int.Parse(dr["sector_object_id"].ToString());
                    }
                    if (e.PickedNode.Tag.GetType().ToString().Contains("StarbaseSprite"))
                    {
                        setOriginalText(pSelectedNode);

                        StarbaseSprite tmp2 = (StarbaseSprite)e.PickedNode.Tag;
                        PText pnameNew = tmp2.getText();
                        pnameNew.TextBrush = Brushes.Red;

                        DataRow dr = tmp2.getRow();
                        id = int.Parse(dr["sector_object_id"].ToString());
                    }
                    if (e.PickedNode.Tag.GetType().ToString().Contains("HarvestableSprite"))
                    {
                        setOriginalText(pSelectedNode);

                        HarvestableSprite tmp2 = (HarvestableSprite)e.PickedNode.Tag;
                        PText pnameNew = tmp2.getText();
                        pnameNew.TextBrush = Brushes.Red;

                        DataRow dr = tmp2.getRow();
                        id = int.Parse(dr["sector_object_id"].ToString());
                    }
                    if (e.PickedNode.Tag.GetType().ToString().Contains("DecorationSprite"))
                    {
                        setOriginalText(pSelectedNode);

                        DecorationSprite tmp2 = (DecorationSprite)e.PickedNode.Tag;
                        PText pnameNew = tmp2.getText();
                        pnameNew.TextBrush = Brushes.Red;

                        DataRow dr = tmp2.getRow();
                        id = int.Parse(dr["sector_object_id"].ToString());
                    }

                    pSelectedNode = e.PickedNode;

                    foreach (DataGridViewRow r in _dgv.Rows)
                    {
                        int rowid = int.Parse(_dgv.Rows[r.Index].Cells["id"].Value.ToString());
                        if(rowid == id)
                        {
                            _dgv.Rows[r.Index].Selected = true;
                            _dgv.CurrentCell = _dgv.Rows[r.Index].Cells[0];
                        }
                    }
                } 
            }
        }

        public void setOriginalText(PNode pickedNode)
        {
                if (pSelectedNode != null)
                {
                    if (pickedNode.Tag.GetType().ToString().Contains("MobSprite"))
                    {
                        MobSprite tmp = (MobSprite)pSelectedNode.Tag;
                        PText pname = tmp.getText();
                        pname.TextBrush = Brushes.White;
                    }
                    if (pickedNode.Tag.GetType().ToString().Contains("PlanetSprite"))
                    {
                        PlanetSprite tmp = (PlanetSprite)pSelectedNode.Tag;
                        PText pname = tmp.getText();
                        pname.TextBrush = Brushes.White;
                    }
                    if (pickedNode.Tag.GetType().ToString().Contains("StargateSprite"))
                    {
                        StargateSprite tmp = (StargateSprite)pSelectedNode.Tag;
                        PText pname = tmp.getText();
                        pname.TextBrush = Brushes.White;
                    }
                    if (pickedNode.Tag.GetType().ToString().Contains("StarbaseSprite"))
                    {
                        StarbaseSprite tmp = (StarbaseSprite)pSelectedNode.Tag;
                        PText pname = tmp.getText();
                        pname.TextBrush = Brushes.White;
                    }
                    if (pickedNode.Tag.GetType().ToString().Contains("HarvestableSprite"))
                    {
                        HarvestableSprite tmp = (HarvestableSprite)pSelectedNode.Tag;
                        PText pname = tmp.getText();
                        pname.TextBrush = Brushes.White;
                    }
                    if (pickedNode.Tag.GetType().ToString().Contains("DecorationSprite"))
                    {
                        DecorationSprite tmp = (DecorationSprite)pSelectedNode.Tag;
                        PText pname = tmp.getText();
                        pname.TextBrush = Brushes.White;
                    }
                }
            }

        public void newSectorObject(NewSectorObject nso)
        {
            _nso = nso;
        }

        public void setSelected(int _id)
        {
            PNode node = null;
            for (int i = 1; i < masterLayer.ChildrenCount; i++)
			{
                for (int i2 = 0; i2 < masterLayer.GetChild(i).ChildrenCount; i2++)
                {
                    node = masterLayer.GetChild(i).GetChild(i2);

                    if (node.Tag.GetType().ToString().Contains("MobSprite"))
                    {
                        MobSprite tmp2 = (MobSprite)node.Tag;

                        DataRow dr = tmp2.getRow();
                        int id = int.Parse(dr["sector_object_id"].ToString());


                        if (_id == id)
                        {
                            setOriginalText(pSelectedNode);
                            PText pnameNew = tmp2.getText();
                            pnameNew.TextBrush = Brushes.Red;
                            pSelectedNode = node;
                            tmp2.setPropGrid();
                            return;
                        }
                    }
                    if (node.Tag.GetType().ToString().Contains("PlanetSprite"))
                    {
                        PlanetSprite tmp2 = (PlanetSprite)node.Tag;

                        DataRow dr = tmp2.getRow();
                        int id = int.Parse(dr["sector_object_id"].ToString());

                        if (_id == id)
                        {
                            setOriginalText(pSelectedNode);
                            PText pnameNew = tmp2.getText();
                            pnameNew.TextBrush = Brushes.Red;
                            pSelectedNode = node;
                            tmp2.setPropGrid();
                            return;
                        }
                    }
                    if (node.Tag.GetType().ToString().Contains("StargateSprite"))
                    {
                        StargateSprite tmp2 = (StargateSprite)node.Tag;

                        DataRow dr = tmp2.getRow();
                        int id = int.Parse(dr["sector_object_id"].ToString());

                        if (_id == id)
                        {
                            setOriginalText(pSelectedNode);
                            PText pnameNew = tmp2.getText();
                            pnameNew.TextBrush = Brushes.Red;
                            pSelectedNode = node;
                            tmp2.setPropGrid();
                            return;
                        }
                    }
                    if (node.Tag.GetType().ToString().Contains("StarbaseSprite"))
                    {
                        StarbaseSprite tmp2 = (StarbaseSprite)node.Tag;

                        DataRow dr = tmp2.getRow();
                        int id = int.Parse(dr["sector_object_id"].ToString());

                        if (_id == id)
                        {
                            setOriginalText(pSelectedNode);
                            PText pnameNew = tmp2.getText();
                            pnameNew.TextBrush = Brushes.Red;
                            pSelectedNode = node;
                            tmp2.setPropGrid();
                            return;
                        }
                    }
                    if (node.Tag.GetType().ToString().Contains("HarvestableSprite"))
                    {
                        HarvestableSprite tmp2 = (HarvestableSprite)node.Tag;

                        DataRow dr = tmp2.getRow();
                        int id = int.Parse(dr["sector_object_id"].ToString());

                        if (_id == id)
                        {
                            setOriginalText(pSelectedNode);
                            PText pnameNew = tmp2.getText();
                            pnameNew.TextBrush = Brushes.Red;
                            pSelectedNode = node;
                            tmp2.setPropGrid();
                            return;
                        }
                    }
                    if (node.Tag.GetType().ToString().Contains("DecorationSprite"))
                    {
                        DecorationSprite tmp2 = (DecorationSprite)node.Tag;

                        DataRow dr = tmp2.getRow();
                        int id = int.Parse(dr["sector_object_id"].ToString());

                        if (_id == id)
                        {
                            setOriginalText(pSelectedNode);
                            PText pnameNew = tmp2.getText();
                            pnameNew.TextBrush = Brushes.Red;
                            pSelectedNode = node;
                            tmp2.setPropGrid();
                            return;
                        }
                    }
                }
			}
        }
        #endregion

        #region Sector Options Methods
        public void hideLayer(int type)
        {
            switch (type)
            {
                case 0:
                    mobsLayer.Visible = false;
                    break;
                case 3:
                    planetsLayer.Visible = false;
                    break;
                case 11:
                    stargatesLayer.Visible = false;
                    break;
                case 12:
                    starbasesLayer.Visible = false;
                    break;
                case 37:
                    decorationsLayer.Visible = false;
                    break;
                case 38:
                    harvestableLayer.Visible = false;
                    break;
            }
        }

        public void showLayer(int type)
        {
            switch (type)
            {
                case 0:
                    mobsLayer.Visible = true;
                    break;
                case 3:
                    planetsLayer.Visible = true;
                    break;
                case 11:
                    stargatesLayer.Visible = true;
                    break;
                case 12:
                    starbasesLayer.Visible = true;
                    break;
                case 37:
                    decorationsLayer.Visible = true;
                    break;
                case 38:
                    harvestableLayer.Visible = true;
                    break;
            }
        }

        public void turnOffText(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        tmp.GetChild(3).Visible = false;
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        tmp.GetChild(3).Visible = false;
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        tmp.GetChild(3).Visible = false;
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        tmp.GetChild(3).Visible = false;
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        tmp.GetChild(3).Visible = false;
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        tmp.GetChild(3).Visible = false;
                    }
                    break;
            }
        }

        public void turnOnText(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        tmp.GetChild(3).Visible = true;
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        tmp.GetChild(3).Visible = true;
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        tmp.GetChild(3).Visible = true;
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        tmp.GetChild(3).Visible = true;
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        tmp.GetChild(3).Visible = true;
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        tmp.GetChild(3).Visible = true;
                    }
                    break;
            }
        }

        public void explorationRangeOn(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        tmp.GetChild(2).Visible = true;
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        tmp.GetChild(2).Visible = true;
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        tmp.GetChild(2).Visible = true;
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        tmp.GetChild(2).Visible = true;
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        tmp.GetChild(2).Visible = true;
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        tmp.GetChild(2).Visible = true;
                    }
                    break;
            }
        }

        public void explorationRangeOff(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        tmp.GetChild(2).Visible = false;
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        tmp.GetChild(2).Visible = false;
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        tmp.GetChild(2).Visible = false;
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        tmp.GetChild(2).Visible = false;
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        tmp.GetChild(2).Visible = false;
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        tmp.GetChild(2).Visible = false;
                    }
                    break;
            }
        }

        public void radarRangeOn(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        tmp.GetChild(1).Visible = true;
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        tmp.GetChild(1).Visible = true;
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        tmp.GetChild(1).Visible = true;
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        tmp.GetChild(1).Visible = true;
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        tmp.GetChild(1).Visible = true;
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        tmp.GetChild(1).Visible = true;
                    }
                    break;
            }
        }

        public void radarRangeOff(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        tmp.GetChild(1).Visible = false;
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        tmp.GetChild(1).Visible = false;
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        tmp.GetChild(1).Visible = false;
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        tmp.GetChild(1).Visible = false;
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        tmp.GetChild(1).Visible = false;
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        tmp.GetChild(1).Visible = false;
                    }
                    break;
            }
        }

        public void SignatureOn(int type)
        {

            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        tmp.GetChild(0).Visible = true;
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        tmp.GetChild(0).Visible = true;
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        tmp.GetChild(0).Visible = true;
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        tmp.GetChild(0).Visible = true;
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        tmp.GetChild(0).Visible = true;
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        tmp.GetChild(0).Visible = true;
                    }
                    break;
            }
        }

        public void SignatureOff(int type)
        {

            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        tmp.GetChild(0).Visible = false;
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        tmp.GetChild(0).Visible = false;
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        tmp.GetChild(0).Visible = false;
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        tmp.GetChild(0).Visible = false;
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        tmp.GetChild(0).Visible = false;
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        tmp.GetChild(0).Visible = false;
                    }
                    break;
            }
        }

        public void navTypeZeroOn(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
            }
        }

        public void navTypeOneOn(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
            }
        }

        public void navTypeTwoOn(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
            }
        }

        public void navTypeZeroOff(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        if (tmp.ChildrenCount == 4)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
            }
        }

        public void navTypeOneOff(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        if (tmp.ChildrenCount == 5)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
            }
        }

        public void navTypeTwoOff(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        if (tmp.ChildrenCount == 6)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
            }
        }

        public void appearsInRadarOn(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        MobSprite dstmp = (MobSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        PlanetSprite dstmp = (PlanetSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        StargateSprite dstmp = (StargateSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        StarbaseSprite dstmp = (StarbaseSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        DecorationSprite dstmp = (DecorationSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        HarvestableSprite dstmp = (HarvestableSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = true;
                        }
                    }
                    break;
            }
        }

        public void appearsInRadarOff(int type)
        {
            PNode tmp = null;
            switch (type)
            {
                case 0:
                    for (int i = 0; i < mobsLayer.ChildrenCount; i++)
                    {
                        tmp = mobsLayer.GetChild(i);
                        MobSprite dstmp = (MobSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 3:
                    for (int i = 0; i < planetsLayer.ChildrenCount; i++)
                    {
                        tmp = planetsLayer.GetChild(i);
                        PlanetSprite dstmp = (PlanetSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 11:
                    for (int i = 0; i < stargatesLayer.ChildrenCount; i++)
                    {
                        tmp = stargatesLayer.GetChild(i);
                        StargateSprite dstmp = (StargateSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 12:
                    for (int i = 0; i < starbasesLayer.ChildrenCount; i++)
                    {
                        tmp = starbasesLayer.GetChild(i);
                        StarbaseSprite dstmp = (StarbaseSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 37:
                    for (int i = 0; i < decorationsLayer.ChildrenCount; i++)
                    {
                        tmp = decorationsLayer.GetChild(i);
                        DecorationSprite dstmp = (DecorationSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
                case 38:
                    for (int i = 0; i < harvestableLayer.ChildrenCount; i++)
                    {
                        tmp = harvestableLayer.GetChild(i);
                        HarvestableSprite dstmp = (HarvestableSprite)tmp.Tag;
                        if (dstmp.getAppearsInRader() == true)
                        {
                            tmp.Visible = false;
                        }
                    }
                    break;
            }
        }

        public void deleteSelectedObject()
        {
            if (pSelectedNode != null)
            {
                //Remove graphic from screen
                pSelectedNode.RemoveFromParent();

                DataRow dr2 = null;

                if (pSelectedNode.Tag.GetType().ToString().Contains("MobSprite"))
                {
                    MobSprite tmp = (MobSprite)pSelectedNode.Tag;
                    dr2 = tmp.getRow();
                }
                else if (pSelectedNode.Tag.GetType().ToString().Contains("PlanetSprite"))
                {
                    PlanetSprite tmp = (PlanetSprite)pSelectedNode.Tag;
                    dr2 = tmp.getRow();
                }
                else if (pSelectedNode.Tag.GetType().ToString().Contains("StargateSprite"))
                {
                    StargateSprite tmp = (StargateSprite)pSelectedNode.Tag;
                    dr2 = tmp.getRow();
                }
                else if (pSelectedNode.Tag.GetType().ToString().Contains("StarbaseSprite"))
                {
                    StarbaseSprite tmp = (StarbaseSprite)pSelectedNode.Tag;
                    dr2 = tmp.getRow();
                }
                else if (pSelectedNode.Tag.GetType().ToString().Contains("HarvestableSprite"))
                {
                    HarvestableSprite tmp = (HarvestableSprite)pSelectedNode.Tag;
                    dr2 = tmp.getRow();
                }
                else if (pSelectedNode.Tag.GetType().ToString().Contains("DecorationSprite"))
                {
                    DecorationSprite tmp = (DecorationSprite)pSelectedNode.Tag;
                    dr2 = tmp.getRow();
                }

                deletedObjectsID.Add(deletedObjectsID.Count, int.Parse(dr2["sector_object_id"].ToString()));
                deletedObjectsType.Add(deletedObjectsType.Count, int.Parse(dr2["type"].ToString()));
                dr2.Delete();
                dr2.AcceptChanges();

                foreach (DataGridViewRow row in _dgv.SelectedRows)
                {
                    _dgv.Rows.RemoveAt(row.Index);

                    _dgv.CurrentRow.Selected = false;
                    _dgv.CurrentCell.Selected = false;

                    _dgv.Update();
                    _dgv.Refresh();
                }

                _pg.SelectedObject = null;
            }
            else
            {
                MessageBox.Show("Sorry there are no Sector objects Selected. \n Please Try Again.");
            }
        }

        public void clearDeletedHashTables()
        {
            deletedObjectsID.Clear();
            deletedObjectsType.Clear();
        }

        public Hashtable getDeletedObjectsID()
        {
            return deletedObjectsID;
        }

        public Hashtable getDeletedObjectsType()
        {
            return deletedObjectsType;
        }

        public void addNewObject(int type, DataRow ndr)
        {
            switch (type)
            {
                case 0:
                    new MobSprite(mobsLayer, ndr, _pg, _dgv);
                    break;
                case 3:
                    new PlanetSprite(planetsLayer, ndr, _pg, _dgv);
                    break;
                case 11:
                    new StargateSprite(stargatesLayer, ndr, _pg, _dgv);
                    break;
                case 12:
                    new StarbaseSprite(starbasesLayer, ndr, _pg, _dgv);
                    break;
                case 37:
                    new DecorationSprite(decorationsLayer, ndr, _pg, _dgv);
                    break;
                case 38:
                    new HarvestableSprite(harvestableLayer, ndr, _pg, _dgv);
                    break;
            }

            _dgv.Rows.Add(ndr["sector_object_id"].ToString(), ndr["name"].ToString(), 
                ndr["base_asset_id"].ToString(), ndr["type"].ToString());

            _dgv.Rows[_dgv.Rows.Count - 1].Selected = true;
            _dgv.CurrentCell = _dgv.Rows[_dgv.Rows.Count - 1].Cells[0];

            _dgv.Update();
            _dgv.Refresh();

            setSelected(int.Parse(ndr["sector_object_id"].ToString()));
        }
        #endregion
    }
}
