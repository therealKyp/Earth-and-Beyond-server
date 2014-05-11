using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

using N7.Utilities;

namespace N7.Props
{
    class MobProps : BaseProps
    {
        private int count;
        private String spawn_group_id;
        private float spawn_radius;
        private float respawn_time;
        private Boolean delayed_spawn;

        public MobProps()
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

        [CategoryAttribute("Mob Object Props"), DescriptionAttribute("The number of maximum mobs spawned at a given time.")]
        public int Count
        {
            get { return count; }
            set { count = value; }
        }

        [CategoryAttribute("Mob Object Props"), DescriptionAttribute("The mobs that are contained within this spawn.")]
        [Editor(typeof(MobGroupEditor), typeof(System.Drawing.Design.UITypeEditor))]
        public String SpawnGroup
        {
            get { return spawn_group_id; }
            set { spawn_group_id = value; }
        }

        [CategoryAttribute("Mob Object Props"), DescriptionAttribute("The radius the mobs spead out to.")]
        public float SpawnRadius
        {
            get { return spawn_radius; }
            set { spawn_radius = value; }
        }

        [CategoryAttribute("Mob Object Props"), DescriptionAttribute("The time from when all mobs in the spawn "+            "were killed to the time when they start spawning again, value should be in seconds.")]
        public float RespawnTime
        {
            get { return respawn_time; }
            set { respawn_time = value; }
        }

        [CategoryAttribute("Mob Object Props"), DescriptionAttribute("Is the mob spawn available at server "+                "startup or is there a delay ? Intial delay would be equal to the spawn timer.")]
        public Boolean DelayedSpawn
        {
            get { return delayed_spawn; }
            set { delayed_spawn = value; }
        }
    }
}
