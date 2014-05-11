using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

using N7.Utilities;

namespace N7.Props
{
    [DefaultPropertyAttribute("Name")]
    class SectorProps
    {
        private String name;
        private int sector_id;
        private float width;
        private float height;
        private float depth;
        private int grid_x;
        private int grid_y;
        private int grid_z;
        private float fog_near;
        private float fog_far;
        private int debris_mode;
        private bool light_backdrop;
        private bool fog_backdrop;
        private bool swap_backdrop;
        private float backdrop_fog_near;
        private float backdrop_fog_far;
        private float max_tilt;
        private bool auto_level;
        private float impulse_rate;
        private float decay_velocity;
        private float decay_spin;
        private int backdrop_asset;
        private String greetings;
        private String notes;
        private int system_id;
        private float galaxy_x;
        private float galaxy_y;
        private float galaxy_z;
        private String sector_type;

        public SectorProps()
        {
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Name of the sector")]
        public string Name
        {
            get{return name;}
            set{name = value;}
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The Sector ID of this System.")]
        public int SectorID
        {
            get { return sector_id; }
            set { sector_id = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The width of the Sector, centered on 0,0,0")]
        public float Width
        {
            get{ return width;}
            set{width = value;}
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The height of the Sector, centered on 0,0,0")]
        public float Height
        {
            get { return height; }
            set { height = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The depth of the Sector, centered on 0,0,0")]
        public float Depth
        {
            get { return depth; }
            set { depth = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("Description and values currently unkown, leave be for the time being.")]
        public int GridX
        {
            get { return grid_x; }
            set { grid_x = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("Description and values currently unkown, leave be for the time being.")]
        public int GridY
        {
            get { return grid_y; }
            set { grid_y = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("Description and values currently unkown, leave be for the time being.")]
        public int GridZ
        {
            get { return grid_z; }
            set { grid_z = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Fog near visibility, distance at which you start to see the sector fog.")]
        public float FogNear
        {
            get { return fog_near; }
            set { fog_near = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Fog far visibility, distance at which you cant see anything anymore.")]
        public float FogFar
        {
            get { return fog_far; }
            set { fog_far = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Sets the background debris mode, values currently unknown.")]
        public int DebrisMode
        {
            get { return debris_mode; }
            set { debris_mode = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Description and values currently unkown, leave be for the time being.")]
        public bool LightBackdrop
        {
            get { return light_backdrop; }
            set { light_backdrop = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Description and values currently unkown, leave be for the time being.")]
        public bool FogBackdrop
        {
            get { return fog_backdrop; }
            set { fog_backdrop = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Description and values currently unkown, leave be for the time being.")]
        public bool SwapBackdrop
        {
            get { return swap_backdrop; }
            set { swap_backdrop = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Background Fog near visibility, same as sector fog but for the backdround.")]
        public float BackdropFogNear
        {
            get { return backdrop_fog_near; }
            set { backdrop_fog_near = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Background Fog far visibility, same as sector fog but for the backdround.")]
        public float BackdropFogFar
        {
            get { return backdrop_fog_far; }
            set { backdrop_fog_far = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The maximum angle at which players ships tilt when moving about the sector.")]
        public float MaxTilt
        {
            get { return max_tilt; }
            set { max_tilt = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Determines whether or not pilots ships can use the autolevel feature in this sector.")]
        public bool AutoLevel
        {
            get { return auto_level; }
            set { auto_level = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The Maximum impulse rate of players in this sector")]
        public float ImpulseRate
        {
            get { return impulse_rate; }
            set { impulse_rate = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Description and values currently unkown, leave be for the time being.")]
        public float DecayVelocity
        {
            get { return decay_velocity; }
            set { decay_velocity = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Description and values currently unkown, leave be for the time being.")]
        public float DecaySpin
        {
            get { return decay_spin; }
            set { decay_spin = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The id of the 3d base asset that represents this sectors background.")]
        [Editor(typeof(BaseAssetsEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public int BackdropAsset
        {
            get { return backdrop_asset; }
            set { backdrop_asset = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Greetings you see upon entering the sector")]
        public string Greetings
        {
            get { return greetings; }
            set { greetings = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("Any Dev Notes")]
        public string Notes
        {
            get { return notes; }
            set { notes = value; }
        }

        [CategoryAttribute("Base Props"), ReadOnlyAttribute(true), DescriptionAttribute("The System Id that this sector is part of.")]
        public int SystemID
        {
            get { return system_id; }
            set { system_id = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("The X Coordinate of this Sector in the Galaxy")]
        public float GalaxyX
        {
            get { return galaxy_x; }
            set { galaxy_x = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("The Y Coordinate of this Sector in the Galaxy")]
        public float GalaxyY
        {
            get { return galaxy_y; }
            set { galaxy_y = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("The Z Coordinate of this Sector in the Galaxy")]
        public float GalaxyZ
        {
            get { return galaxy_z; }
            set { galaxy_z = value; }
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("The Type of Sector.")]
        [Browsable(true)]
        [TypeConverter(typeof(SectorTypeConvertor))]
        public string SectorType
        {
            //When first loaded set property with the first item in the list.
            get
            {
                string S = "";
                if (sector_type != null)
                {
                    S = sector_type;
                }
                else
                {
                    if (HE_GlobalVars._listofSectorTypes.Length > 0)
                    {

                        S = HE_GlobalVars._listofSectorTypes[0];
                    }
                }

                return S;
            }
            set { sector_type = value; }
        }
    }
}
