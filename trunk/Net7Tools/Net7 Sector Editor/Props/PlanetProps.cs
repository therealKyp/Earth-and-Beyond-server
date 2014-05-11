using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

namespace N7.Props
{
    class PlanetProps : BaseProps
    {
        private int orbit_id;
        private float orbit_dist;
        private float orbit_angle;
        private float orbit_rate;
        private float rotate_rate;
        private float rotate_angle;
        private float tilt_angle;
        private bool is_landable;

        public PlanetProps()
        {
        }

        public void fillBaseProps(BaseProps bp)
        {
            this.SectorID = bp.SectorID;
            this.NavType = bp.NavType;
            this.Signature = bp.Signature;
            this.IsHuge = bp.IsHuge;
            this.BaseXP = bp.BaseXP;
            this.ExplorationRange = bp.ExplorationRange;
            this.BaseAssetID = bp.BaseAssetID;
            this.Color = bp.Color;
            this.Type = bp.Type;
            this.Scale = bp.Scale;
            this.PositionX = bp.PositionX;
            this.PositionY = bp.PositionY;
            this.PositionZ = bp.PositionZ;
            this.Name = bp.Name;
            this.AppearsInRadar = bp.AppearsInRadar;
            this.RadarRange = bp.RadarRange;
            this.Destination = bp.Destination;
        }

        [CategoryAttribute("Planet Object Props"), DescriptionAttribute("The orbit ID of this object")]
        public int OrbitID
        {
            get { return orbit_id; }
            set { orbit_id = value; }
        }

        [CategoryAttribute("Planet Object Props"), DescriptionAttribute("The orbit distance")]
        public float OrbitDist
        {
            get { return orbit_dist; }
            set { orbit_dist = value; }
        }

        [CategoryAttribute("Planet Object Props"), DescriptionAttribute("The orbit angle")]
        public float OrbitAngle
        {
            get { return orbit_angle; }
            set { orbit_angle = value; }
        }

        [CategoryAttribute("Planet Object Props"), DescriptionAttribute("The orbit rate")]
        public float OrbitRate
        {
            get { return orbit_rate; }
            set { orbit_rate = value; }
        }

        [CategoryAttribute("Planet Object Props"), DescriptionAttribute("The rotational rate")]
        public float RotateRate
        {
            get { return rotate_rate; }
            set { rotate_rate = value; }
        }

        [CategoryAttribute("Planet Object Props"), DescriptionAttribute("The rotation angle")]
        public float RotateAngle
        {
            get { return rotate_angle; }
            set { rotate_angle = value; }
        }

        [CategoryAttribute("Planet Object Props"), DescriptionAttribute("The angle of tilt")]
        public float TiltAngle
        {
            get { return tilt_angle; }
            set { tilt_angle = value; }
        }

        [CategoryAttribute("Planet Object Props"), DescriptionAttribute("Is this planet landable ?")]
        public bool IsLandable
        {
            get { return is_landable; }
            set { is_landable = value; }
        }
    }
}
