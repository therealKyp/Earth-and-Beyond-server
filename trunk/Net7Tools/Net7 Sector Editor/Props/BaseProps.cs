using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Drawing;

using N7.Utilities;

namespace N7.Props
{
    [DefaultPropertyAttribute("Name")]
    class BaseProps
    {
        //Sector_Nav_Points
        private int sector_id;
        private String nav_type;
        private float signature;
        private bool is_huge;
        private int base_xp;
        private float exploration_range;

        //Sector_Objects
        private int base_asset_id;
        private Color color;
        private String _Type;
        private float scale;
        private float position_x;
        private float position_y;
        private float position_z;
        private double pitch;
        private double yaw;
        private double roll;
        private String name;
        private bool appears_in_rader;
        private float radar_range;
        private int gate_to;
        private int sound_effect;
        private float sound_effect_range;

        public BaseProps()
        {
        }

        [CategoryAttribute("Nav Point Props"), ReadOnlyAttribute(true), DescriptionAttribute("The Sector id in which this object belongs too.")]
        public int SectorID
        {
            get { return sector_id; }
            set { sector_id = value; }
        }

        [CategoryAttribute("Nav Point Props"), DescriptionAttribute("The Nav Type of the Object (0,1,2)")]
        [Browsable(true)]
        [TypeConverter(typeof(NavTypeConverter))]
        public string NavType
        {
            //When first loaded set property with the first item in the list.
            get
            {
                string S = "";
                if (nav_type != null)
                {
                    S = nav_type;
                }
                else
                {
                    if (HE_GlobalVars._ListofNavTypes.Length > 0)
                    {
                        //Sort the list before displaying it
                        Array.Sort(HE_GlobalVars._ListofNavTypes);

                        S = HE_GlobalVars._ListofNavTypes[0];
                    }
                }

                return S;
            }
            set { nav_type = value; }
        }

        [CategoryAttribute("Nav Point Props"), DescriptionAttribute("Base Signature of the Object")]
        public float Signature
        {
            get { return signature; }
            set { signature = value; }
        }

        [CategoryAttribute("Nav Point Props"), DescriptionAttribute("Is the Object Huge")]
        public bool IsHuge
        {
            get { return is_huge; }
            set { is_huge = value; }
        }

        [CategoryAttribute("Nav Point Props"), DescriptionAttribute("The Base Xp you get from exploring this Object")]
        public int BaseXP
        {
            get { return base_xp; }
            set { base_xp = value; }
        }

        [CategoryAttribute("Nav Point Props"), DescriptionAttribute("The range at which the players get xp when intially \n exploring an object.")]
        public float ExplorationRange
        {
            get { return exploration_range; }
            set { exploration_range = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The ID of the graphical Asset for this object")]
        [Editor(typeof(BaseAssetsEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public int BaseAssetID
        {
            get { return base_asset_id; }
            set { base_asset_id = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The Color shade of the object.")]
        public Color Color
        {
            get { return color; }
            set { color = value; }
        }

        [CategoryAttribute("Base Props"), ReadOnlyAttribute(true), DescriptionAttribute("The type of this Object (0,3,11,12,37,38)")]
        [Browsable(true)]
        [TypeConverter(typeof(TypesConverter))]
        public string Type
        {
            //When first loaded set property with the first item in the list.
            get
            {
                string S = "";
                if (_Type != null)
                {
                    S = _Type;
                }
                else
                {
                    if (HE_GlobalVars._ListofTypes.Length > 0)
                    {
                        //Sort the list before displaying it
                        Array.Sort(HE_GlobalVars._ListofTypes);

                        S = HE_GlobalVars._ListofTypes[0];
                    }
                }

                return S;
            }
            set { _Type = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The scale of the object")]
        public float Scale
        {
            get { return scale; }
            set { scale = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The X Coordinate of the object")]
        public float PositionX
        {
            get { return position_x; }
            set { position_x = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The Y Coordinate of the object")]
        public float PositionY
        {
            get { return position_y; }
            set { position_y = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The Z Coordinate of the object")]
        public float PositionZ
        {
            get { return position_z; }
            set { position_z = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The pitch(alttitude) in eular angles (0-360) of this object.")]
        public double Orientation_Pitch
        {
            get { return pitch; }
            set { pitch = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The yaw(heading) in eular angles (0-360) of this object.")]
        public double Orientation_Yaw
        {
            get { return yaw; }
            set { yaw = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The roll(bank) in eular angles (0-360) of this object.")]
        public double Orientation_Roll
        {
            get { return roll; }
            set { roll = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The name of the Object")]
        public String Name
        {
            get { return name; }
            set { name = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Will the object appear on the radar ?")]
        public bool AppearsInRadar
        {
            get { return appears_in_rader; }
            set { appears_in_rader = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The radar range of the object")]
        public float RadarRange
        {
            get { return radar_range; }
            set { radar_range = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The Destination of the object, Currently only used on planets, stargates and stations.")]
        [Editor(typeof(DestinationEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public int Destination
        {
            get { return gate_to; }
            set { gate_to = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The sound effect that is attached to this sector object.")]
        [Editor(typeof(SoundEffectEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public int SoundEffect
        {
            get { return sound_effect; }
            set { sound_effect = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The range (in km) at which players can here this objects sound.")]
        public float SoundEffectRange
        {
            get { return sound_effect_range; }
            set { sound_effect_range = value; }
        }
    }
}
