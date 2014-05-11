using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

namespace N7.Props
{
    class StarbaseProps : BaseProps
    {
        private bool is_cap_ship;
        private bool is_dockable;

        public StarbaseProps()
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

        [CategoryAttribute("Starbase Object Props"), DescriptionAttribute("Is this starbase object a Capital Ship ?")]
        public bool IsCapShip
        {
            get { return is_cap_ship; }
            set { is_cap_ship = value; }
        }

        [CategoryAttribute("Starbase Object Props"), DescriptionAttribute("Is this starbase dockable by players?")]
        public bool IsDockable
        {
            get { return is_dockable; }
            set { is_dockable = value; }
        }
    }
}
