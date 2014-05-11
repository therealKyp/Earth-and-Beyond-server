using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using N7.Props;
using N7.Sql;
using N7.Utilities;

namespace N7.GUI
{
    public partial class NewSectorObject : Form
    {
        String type;
        private SectorObjectsSql sectorObjectsSQL;
        private TreeView tree;
        private MobProps mp;
        private PlanetProps pp;
        private StarbaseProps sbp;
        private StargateProps sgp;
        private BaseProps bp;
        private HarvestableProps hp;
        private int type2;
        private PointF position;
        private SectorWindow _s1;
        private int lastInsertID = 0;
        private DataRow newSectorObjectsRow;

        public NewSectorObject(String typeName, TreeView t1, SectorWindow s1)
        {
            tree = t1;
            sectorObjectsSQL = mainFrm.sectorObjects;
            type = typeName;
            _s1 = s1;
            mainFrm.selectedObjectID = 0;
            InitializeComponent();
            
        }

        private void NewSectorObject_Load(object sender, EventArgs e)
        {
            String sectorName = tree.SelectedNode.Text;
            int sectorID = mainFrm.sectors.getIDFromName(sectorName);

            if(type.Contains("Mobs"))
            {
                mp = new MobProps();
                mp.SectorID = sectorID;
                mp.Color = Color.Black;
                mp.Type = "Mobs";
                mp.Signature = 7000;
                mp.RadarRange = 5000;
                mp.ExplorationRange = 3000;
                mp.SoundEffect = -1;
                mp.SoundEffectRange = 0;
                mp.SpawnGroup = "<Collection...>";
                mp.SpawnRadius = 4000;
                mp.Name = "<New Sector Object>";
                propertyGrid1.SelectedObject = mp;
                type2 = 0;
            }
            else if (type.Contains("Planets"))
            {
                pp = new PlanetProps();
                pp.SectorID = sectorID;
                pp.Color = Color.Black;
                pp.Type = "Planets";
                pp.Signature = 7000;
                pp.RadarRange = 5000;
                pp.ExplorationRange = 3000;
                pp.SoundEffect = -1;
                pp.SoundEffectRange = 0;
                pp.Name = "<New Sector Object>";
                propertyGrid1.SelectedObject = pp;
                type2 = 3;
            }
            else if (type.Contains("Stargates"))
            {
                sgp = new StargateProps();
                sgp.SectorID = sectorID;
                sgp.Color = Color.Black;
                sgp.Type = "Stargates";
                sgp.Signature = 7000;
                sgp.RadarRange = 5000;
                sgp.ExplorationRange = 3000;
                sgp.SoundEffect = -1;
                sgp.SoundEffectRange = 0;
                sgp.Name = "<New Sector Object>";
                sgp.PositionX = position.X * 100;
                sgp.PositionY = position.Y * 100;
                propertyGrid1.SelectedObject = sgp;
                type2 = 11;
            }
            else if (type.Contains("Starbases"))
            {
                sbp = new StarbaseProps();
                sbp.SectorID = sectorID;
                sbp.Color = Color.Black;
                sbp.Type = "Starbases";
                sbp.Signature = 7000;
                sbp.RadarRange = 5000;
                sbp.ExplorationRange = 3000;
                sbp.SoundEffect = -1;
                sbp.SoundEffectRange = 0;
                sbp.Name = "<New Sector Object>";
                propertyGrid1.SelectedObject = sbp;
                type2 = 12;
            }
            else if (type.Contains("Decorations"))
            {
                bp = new BaseProps();
                bp.SectorID = sectorID;
                bp.Color = Color.Black;
                bp.Type = "Decorations";
                bp.Signature = 7000;
                bp.RadarRange = 5000;
                bp.ExplorationRange = 3000;
                bp.SoundEffect = -1;
                bp.SoundEffectRange = 0;
                bp.Name = "<New Sector Object>";
                propertyGrid1.SelectedObject = bp;
                type2 = 37;
            }
            else if (type.Contains("Harvestable"))
            {
                hp = new HarvestableProps();
                hp.SectorID = sectorID;
                hp.Color = Color.Black;
                hp.Type = "Harvestables";
                hp.Signature = 7000;
                hp.RadarRange = 5000;
                hp.Field = "Random";
                hp.ExplorationRange = 3000;
                hp.SoundEffect = -1;
                hp.SoundEffectRange = 0;
                hp.SpawnGroup = "<Collection...>";
                hp.MaxFieldRadius = 3000;
                hp.MobSpawnRadius = 0;
                hp.Name = "<New Sector Object>";
                hp.ResType = "<Collection...>";
                propertyGrid1.SelectedObject = hp;
                type2 = 38;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (lastInsertID == 0)
            {
                DataTable tmp = sectorObjectsSQL.getSectorObject();
                newSectorObjectsRow = tmp.NewRow();

                QuaternionCalc qtmp = new QuaternionCalc();
                double[] q1 = null;

                switch (type2)
                {
                    case 0:
                        newSectorObjectsRow["sector_id"] = mp.SectorID;
                        newSectorObjectsRow["nav_type"] = mp.NavType;
                        newSectorObjectsRow["signature"] = mp.Signature;
                        newSectorObjectsRow["is_huge"] = mp.IsHuge;
                        newSectorObjectsRow["base_xp"] = mp.BaseXP;
                        newSectorObjectsRow["exploration_range"] = mp.ExplorationRange;
                        newSectorObjectsRow["base_asset_id"] = mp.BaseAssetID;
                        newSectorObjectsRow["h"] = mp.Color.GetHue();
                        newSectorObjectsRow["s"] = mp.Color.GetSaturation();
                        newSectorObjectsRow["v"] = mp.Color.GetBrightness();
                        newSectorObjectsRow["type"] = 0;
                        newSectorObjectsRow["scale"] = mp.Scale;
                        newSectorObjectsRow["position_x"] = mp.PositionX;
                        newSectorObjectsRow["position_y"] = mp.PositionY;
                        newSectorObjectsRow["position_z"] = mp.PositionZ;

                        q1 = qtmp.AngleToQuat(mp.Orientation_Yaw, mp.Orientation_Pitch, mp.Orientation_Roll);

                        newSectorObjectsRow["orientation_z"] = q1[0];
                        newSectorObjectsRow["orientation_u"] = q1[1];
                        newSectorObjectsRow["orientation_v"] = q1[2];
                        newSectorObjectsRow["orientation_w"] = q1[3];

                        newSectorObjectsRow["name"] = mp.Name.Replace("'", "''");
                        newSectorObjectsRow["appears_in_radar"] = mp.AppearsInRadar;
                        newSectorObjectsRow["radar_range"] = mp.RadarRange;
                        newSectorObjectsRow["gate_to"] = mp.Destination;
                        newSectorObjectsRow["sound_effect_id"] = mp.SoundEffect;
                        newSectorObjectsRow["sound_effect_range"] = mp.SoundEffectRange;

                        newSectorObjectsRow["mob_spawn_radius"] = mp.SpawnRadius;
                        newSectorObjectsRow["mob_count"] = mp.Count;

                        newSectorObjectsRow["respawn_time"] = mp.RespawnTime;
                        newSectorObjectsRow["delayed_spawn"] = mp.DelayedSpawn;
                        break;
                    case 3:
                        newSectorObjectsRow["sector_id"] = pp.SectorID;
                        newSectorObjectsRow["nav_type"] = pp.NavType;
                        newSectorObjectsRow["signature"] = pp.Signature;
                        newSectorObjectsRow["is_huge"] = pp.IsHuge;
                        newSectorObjectsRow["base_xp"] = pp.BaseXP;
                        newSectorObjectsRow["exploration_range"] = pp.ExplorationRange;
                        newSectorObjectsRow["base_asset_id"] = pp.BaseAssetID;
                        newSectorObjectsRow["h"] = pp.Color.GetHue();
                        newSectorObjectsRow["s"] = pp.Color.GetSaturation();
                        newSectorObjectsRow["v"] = pp.Color.GetBrightness();
                        newSectorObjectsRow["type"] = 3;
                        newSectorObjectsRow["scale"] = pp.Scale;
                        newSectorObjectsRow["position_x"] = pp.PositionX;
                        newSectorObjectsRow["position_y"] = pp.PositionY;
                        newSectorObjectsRow["position_z"] = pp.PositionZ;

                        q1 = qtmp.AngleToQuat(pp.Orientation_Yaw, pp.Orientation_Pitch, pp.Orientation_Roll);

                        newSectorObjectsRow["orientation_z"] = q1[0];
                        newSectorObjectsRow["orientation_u"] = q1[1];
                        newSectorObjectsRow["orientation_v"] = q1[2];
                        newSectorObjectsRow["orientation_w"] = q1[3];

                        newSectorObjectsRow["name"] = pp.Name.Replace("'", "''");
                        newSectorObjectsRow["appears_in_radar"] = pp.AppearsInRadar;
                        newSectorObjectsRow["radar_range"] = pp.RadarRange;
                        newSectorObjectsRow["gate_to"] = pp.Destination;
                        newSectorObjectsRow["sound_effect_id"] = pp.SoundEffect;
                        newSectorObjectsRow["sound_effect_range"] = pp.SoundEffectRange;

                        newSectorObjectsRow["orbit_id"] = pp.OrbitID;
                        newSectorObjectsRow["orbit_dist"] = pp.OrbitDist;
                        newSectorObjectsRow["orbit_angle"] = pp.OrbitAngle;
                        newSectorObjectsRow["orbit_rate"] = pp.OrbitRate;
                        newSectorObjectsRow["rotate_rate"] = pp.RotateRate;
                        newSectorObjectsRow["rotate_angle"] = pp.RotateAngle;
                        newSectorObjectsRow["tilt_angle"] = pp.TiltAngle;
                        newSectorObjectsRow["is_landable"] = pp.IsLandable;
                        break;
                    case 11:
                        newSectorObjectsRow["sector_id"] = sgp.SectorID;
                        newSectorObjectsRow["nav_type"] = sgp.NavType;
                        newSectorObjectsRow["signature"] = sgp.Signature;
                        newSectorObjectsRow["is_huge"] = sgp.IsHuge;
                        newSectorObjectsRow["base_xp"] = sgp.BaseXP;
                        newSectorObjectsRow["exploration_range"] = sgp.ExplorationRange;
                        newSectorObjectsRow["base_asset_id"] = sgp.BaseAssetID;
                        newSectorObjectsRow["h"] = sgp.Color.GetHue();
                        newSectorObjectsRow["s"] = sgp.Color.GetSaturation();
                        newSectorObjectsRow["v"] = sgp.Color.GetBrightness();
                        newSectorObjectsRow["type"] = 11;
                        newSectorObjectsRow["scale"] = sgp.Scale;
                        newSectorObjectsRow["position_x"] = sgp.PositionX;
                        newSectorObjectsRow["position_y"] = sgp.PositionY;
                        newSectorObjectsRow["position_z"] = sgp.PositionZ;

                        q1 = qtmp.AngleToQuat(sgp.Orientation_Yaw, sgp.Orientation_Pitch, sgp.Orientation_Roll);

                        newSectorObjectsRow["orientation_z"] = q1[0];
                        newSectorObjectsRow["orientation_u"] = q1[1];
                        newSectorObjectsRow["orientation_v"] = q1[2];
                        newSectorObjectsRow["orientation_w"] = q1[3];

                        newSectorObjectsRow["name"] = sgp.Name.Replace("'", "''");
                        newSectorObjectsRow["appears_in_radar"] = sgp.AppearsInRadar;
                        newSectorObjectsRow["radar_range"] = sgp.RadarRange;
                        newSectorObjectsRow["gate_to"] = sgp.Destination;
                        newSectorObjectsRow["classSpecific"] = sgp.IsClassSpecific;
                        newSectorObjectsRow["sound_effect_id"] = sgp.SoundEffect;
                        newSectorObjectsRow["sound_effect_range"] = sgp.SoundEffectRange;

                        int factionID = mainFrm.factions.findIDbyName(sgp.FactionID);
                        newSectorObjectsRow["faction_id"] = factionID;
                        break;
                    case 12:
                        newSectorObjectsRow["sector_id"] = sbp.SectorID;
                        newSectorObjectsRow["nav_type"] = sbp.NavType;
                        newSectorObjectsRow["signature"] = sbp.Signature;
                        newSectorObjectsRow["is_huge"] = sbp.IsHuge;
                        newSectorObjectsRow["base_xp"] = sbp.BaseXP;
                        newSectorObjectsRow["exploration_range"] = sbp.ExplorationRange;
                        newSectorObjectsRow["base_asset_id"] = sbp.BaseAssetID;
                        newSectorObjectsRow["h"] = sbp.Color.GetHue();
                        newSectorObjectsRow["s"] = sbp.Color.GetSaturation();
                        newSectorObjectsRow["v"] = sbp.Color.GetBrightness();
                        newSectorObjectsRow["type"] = 12;
                        newSectorObjectsRow["scale"] = sbp.Scale;
                        newSectorObjectsRow["position_x"] = sbp.PositionX;
                        newSectorObjectsRow["position_y"] = sbp.PositionY;
                        newSectorObjectsRow["position_z"] = sbp.PositionZ;

                        q1 = qtmp.AngleToQuat(sbp.Orientation_Yaw, sbp.Orientation_Pitch, sbp.Orientation_Roll);

                        newSectorObjectsRow["orientation_z"] = q1[0];
                        newSectorObjectsRow["orientation_u"] = q1[1];
                        newSectorObjectsRow["orientation_v"] = q1[2];
                        newSectorObjectsRow["orientation_w"] = q1[3];

                        newSectorObjectsRow["name"] = sbp.Name.Replace("'", "''");
                        newSectorObjectsRow["appears_in_radar"] = sbp.AppearsInRadar;
                        newSectorObjectsRow["radar_range"] = sbp.RadarRange;
                        newSectorObjectsRow["gate_to"] = sbp.Destination;
                        newSectorObjectsRow["sound_effect_id"] = sbp.SoundEffect;
                        newSectorObjectsRow["sound_effect_range"] = sbp.SoundEffectRange;

                        newSectorObjectsRow["capShip"] = sbp.IsCapShip;
                        newSectorObjectsRow["dockable"] = sbp.IsDockable;
                        break;
                    case 37:
                        newSectorObjectsRow["sector_id"] = bp.SectorID;
                        newSectorObjectsRow["nav_type"] = bp.NavType;
                        newSectorObjectsRow["signature"] = bp.Signature;
                        newSectorObjectsRow["is_huge"] = bp.IsHuge;
                        newSectorObjectsRow["base_xp"] = bp.BaseXP;
                        newSectorObjectsRow["exploration_range"] = bp.ExplorationRange;
                        newSectorObjectsRow["base_asset_id"] = bp.BaseAssetID;
                        newSectorObjectsRow["h"] = bp.Color.GetHue();
                        newSectorObjectsRow["s"] = bp.Color.GetSaturation();
                        newSectorObjectsRow["v"] = bp.Color.GetBrightness();
                        newSectorObjectsRow["type"] = 37;
                        newSectorObjectsRow["scale"] = bp.Scale;
                        newSectorObjectsRow["position_x"] = bp.PositionX;
                        newSectorObjectsRow["position_y"] = bp.PositionY;
                        newSectorObjectsRow["position_z"] = bp.PositionZ;

                        q1 = qtmp.AngleToQuat(bp.Orientation_Yaw, bp.Orientation_Pitch, bp.Orientation_Roll);

                        newSectorObjectsRow["orientation_z"] = q1[0];
                        newSectorObjectsRow["orientation_u"] = q1[1];
                        newSectorObjectsRow["orientation_v"] = q1[2];
                        newSectorObjectsRow["orientation_w"] = q1[3];

                        newSectorObjectsRow["name"] = bp.Name.Replace("'", "''");
                        newSectorObjectsRow["appears_in_radar"] = bp.AppearsInRadar;
                        newSectorObjectsRow["radar_range"] = bp.RadarRange;
                        newSectorObjectsRow["gate_to"] = bp.Destination;
                        newSectorObjectsRow["sound_effect_id"] = bp.SoundEffect;
                        newSectorObjectsRow["sound_effect_range"] = bp.SoundEffectRange;
                        break;
                    case 38:
                        newSectorObjectsRow["sector_id"] = hp.SectorID;
                        newSectorObjectsRow["nav_type"] = hp.NavType;
                        newSectorObjectsRow["signature"] = hp.Signature;
                        newSectorObjectsRow["is_huge"] = hp.IsHuge;
                        newSectorObjectsRow["base_xp"] = hp.BaseXP;
                        newSectorObjectsRow["exploration_range"] = hp.ExplorationRange;
                        newSectorObjectsRow["base_asset_id"] = hp.BaseAssetID;
                        newSectorObjectsRow["h"] = hp.Color.GetHue();
                        newSectorObjectsRow["s"] = hp.Color.GetSaturation();
                        newSectorObjectsRow["v"] = hp.Color.GetBrightness();
                        newSectorObjectsRow["type"] = 38;
                        newSectorObjectsRow["scale"] = hp.Scale;
                        newSectorObjectsRow["position_x"] = hp.PositionX;
                        newSectorObjectsRow["position_y"] = hp.PositionY;
                        newSectorObjectsRow["position_z"] = hp.PositionZ;

                        q1 = qtmp.AngleToQuat(hp.Orientation_Yaw, hp.Orientation_Pitch, hp.Orientation_Roll);

                        newSectorObjectsRow["orientation_z"] = q1[0];
                        newSectorObjectsRow["orientation_u"] = q1[1];
                        newSectorObjectsRow["orientation_v"] = q1[2];
                        newSectorObjectsRow["orientation_w"] = q1[3];

                        newSectorObjectsRow["name"] = hp.Name.Replace("'", "''");
                        newSectorObjectsRow["appears_in_radar"] = hp.AppearsInRadar;
                        newSectorObjectsRow["radar_range"] = hp.RadarRange;
                        newSectorObjectsRow["gate_to"] = hp.Destination;
                        newSectorObjectsRow["sound_effect_id"] = hp.SoundEffect;
                        newSectorObjectsRow["sound_effect_range"] = hp.SoundEffectRange;

                        newSectorObjectsRow["level"] = hp.Level;
                        newSectorObjectsRow["res_count"] = hp.ResCount;
                        newSectorObjectsRow["spawn_radius"] = hp.MobSpawnRadius;
                        newSectorObjectsRow["pop_rock_chance"] = hp.PopRockChance;
                        newSectorObjectsRow["max_field_radius"] = hp.MaxFieldRadius;

                        if (hp.Field == "Random")
                        {
                            newSectorObjectsRow["field"] = 0;
                        }
                        else if (hp.Field == "Ring")
                        {
                            newSectorObjectsRow["field"] = 1;
                        }
                        else if (hp.Field == "Donut")
                        {
                            newSectorObjectsRow["field"] = 2;
                        }
                        else if (hp.Field == "Cylinder")
                        {
                            newSectorObjectsRow["field"] = 3;
                        }
                        else if (hp.Field == "Sphere")
                        {
                            newSectorObjectsRow["field"] = 4;
                        }
                        else if (hp.Field == "Gas Cloud Clump")
                        {
                            newSectorObjectsRow["field"] = 5;
                        }
                        break;
                }

                //Check if their is a sound_effect and set its default range if it has not been set.
                int id = int.Parse(newSectorObjectsRow["sound_effect_id"].ToString());
                Console.Out.WriteLine(id);
                float range = int.Parse(newSectorObjectsRow["sound_effect_range"].ToString());
                if (id != -1)
                {
                    if (range == 0)
                    {
                        newSectorObjectsRow["sound_effect_range"] = 30000;
                    }
                }

                //Add To dataTable
                tmp.Rows.Add(newSectorObjectsRow); ;

                //Add to Database
                sectorObjectsSQL.newRow(newSectorObjectsRow);

                switch(type2)
                {
                    case 0:
                        Boolean b1 = validateMobs(0);
                        if (b1 == true)
                        {
                            //Add to Graphic window
                            mainFrm.sectorWindow.addNewObject(type2, newSectorObjectsRow);
                            this.Close();
                        }
                        break;
                    case 3:
                        mainFrm.sectorWindow.addNewObject(type2, newSectorObjectsRow);
                        this.Close();
                        break;
                    case 11:
                        mainFrm.sectorWindow.addNewObject(type2, newSectorObjectsRow);
                        this.Close();
                        break;
                    case 12:
                        mainFrm.sectorWindow.addNewObject(type2, newSectorObjectsRow);
                        this.Close();
                        break;
                    case 37:
                        mainFrm.sectorWindow.addNewObject(type2, newSectorObjectsRow);
                        this.Close();
                        break;
                    case 38:
                        Boolean b2 = validateHarvestables(0);
                        if (b2 == true)
                        {
                            //Add to Graphic window
                            mainFrm.sectorWindow.addNewObject(type2, newSectorObjectsRow);
                            this.Close();
                        }
                        break;
                }
            }
            else
            {
                if (type2 == 0)
                {
                    Boolean b1 = validateMobs(lastInsertID);
                    if (b1 == true)
                    {
                        //Add to Graphic window
                        mainFrm.sectorWindow.addNewObject(type2, newSectorObjectsRow);
                        this.Close();
                    }
                }
                else if (type2 == 38)
                {
                    Boolean b2 = validateHarvestables(lastInsertID);
                    if (b2 == true)
                    {
                        //Add to Graphic window
                        mainFrm.sectorWindow.addNewObject(type2, newSectorObjectsRow);
                        this.Close();
                    }
                }
            }
        }

        private Boolean validateMobs(int id)
        {
            lastInsertID = id;
            if (lastInsertID == 0)
            {
                DataTable tmp = Database.executeQuery(Database.DatabaseName.net7, "SELECT LAST_INSERT_ID()");
                foreach (DataRow z in tmp.Rows)
                {
                    lastInsertID = int.Parse(z["LAST_INSERT_ID()"].ToString());
                    mainFrm.selectedObjectID = lastInsertID;
                }
            }

            //Check and see if a mob has been added.
            String query = "SELECT * FROM mob_spawn_group where spawn_group_id='" + lastInsertID + "';";
            DataTable groupMobsTable = Database.executeQuery(Database.DatabaseName.net7, query);

            if (groupMobsTable.Rows.Count == 0)
            {
                DialogResult dr = MessageBox.Show("You have no mobs in your spawn group, to save this record, add some mobs.",
                "Mob Verification", MessageBoxButtons.OK, MessageBoxIcon.Stop);

                return false;
            }
            else
            {
                switch (type2)
                {
                    case 0:
                        if (mp.Count == 0)
                        {
                            DialogResult dr = MessageBox.Show("Your Maximum spawn count must be > 0 when you have mobs in your group ",
                                    "Mob Count Verification", MessageBoxButtons.OK, MessageBoxIcon.Stop);

                            return false;
                        }
                        break;
                }

                return true;
            }
        }

        private Boolean validateHarvestables(int id)
        {
            lastInsertID = id;
            if (lastInsertID == 0)
            {
                DataTable tmp = Database.executeQuery(Database.DatabaseName.net7, "SELECT LAST_INSERT_ID()");
                foreach (DataRow z in tmp.Rows)
                {
                    lastInsertID = int.Parse(z["LAST_INSERT_ID()"].ToString());
                    mainFrm.selectedObjectID = lastInsertID;
                }
            }

            //check and see if a harvestable restype has been added.
            String query2 = "SELECT * FROM sector_objects_harvestable_restypes where group_id='" + lastInsertID + "';";
            DataTable loadTypes = Database.executeQuery(Database.DatabaseName.net7, query2);

            if (loadTypes.Rows.Count == 0)
            {
                DialogResult dr = MessageBox.Show("You have no resource types in your field, to save this record, add at least 1 restype.",
                "Res Type Verification", MessageBoxButtons.OK, MessageBoxIcon.Stop);

                return false;
            }
            else
            {
                if (hp.MaxFieldRadius == 0)
                {
                    DialogResult dr = MessageBox.Show("Your Maximum field radius must be > 0.",
                            "Mob Count Verification", MessageBoxButtons.OK, MessageBoxIcon.Stop);

                    return false;
                }

                if (hp.MobSpawnRadius > 0)
                {
                    Boolean mgb = validateMobs(lastInsertID);
                    if (mgb == true)
                    {
                        if (hp.MobSpawnRadius == 0)
                        {
                            DialogResult dr = MessageBox.Show("Since you have mobs guardians your Maximum spawn radius must be > 0.",
                                    "Mob Count Verification", MessageBoxButtons.OK, MessageBoxIcon.Stop);

                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }

                return true;
            }
        }

        public void setPosition(PointF position)
        {
            switch (type2)
            {
                case 0:
                    mp.PositionX = position.X * 100;
                    mp.PositionY = -(position.Y * 100);
                    break;
                case 3:
                    pp.PositionX = position.X * 100;
                    pp.PositionY = -(position.Y * 100);
                    break;
                case 11:
                    sgp.PositionX = position.X * 100;
                    sgp.PositionY = -(position.Y * 100);
                    break;
                case 12:
                    sbp.PositionX = position.X * 100;
                    sbp.PositionY = -(position.Y * 100);
                    break;
                case 37:
                    bp.PositionX = position.X * 100;
                    bp.PositionY = -(position.Y * 100);
                    break;
                case 38:
                    hp.PositionX = position.X * 100;
                    hp.PositionY = -(position.Y * 100);
                    break;
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.Hide();
            _s1.newSectorObject(this);
        }
    }
}