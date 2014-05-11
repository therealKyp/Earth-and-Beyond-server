using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Drawing;

namespace N7.Props
{
    [DefaultPropertyAttribute("Name")]
    class SystemProps
    {
        private String name;
        private float galaxy_x;
        private float galaxy_y;
        private float galaxy_z;
        private Color color;
        private String notes;

        public SystemProps()
        {
        }

        [CategoryAttribute("Base Props"), DescriptionAttribute("Name of the system")]
        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("X Coordinate of this system in the galaxy.")]
        public float GalaxyX
        {
            get { return galaxy_x ; }
            set { galaxy_x = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("Y Coordinate of this system in the galaxy.")]
        public float GalaxyY
        {
            get { return galaxy_y; }
            set { galaxy_y = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("Z Coordinate of this system in the galaxy.")]
        public float GalaxyZ
        {
            get { return galaxy_z; }
            set { galaxy_z = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("The Color of the System (For future tool purposes only).")]
        public Color Color
        {
            get { return color; }
            set { color = value; }
        }

        [CategoryAttribute("Misc. Properties"), DescriptionAttribute("Any Developer Notes you may have.")]
        public string Notes
        {
            get { return notes; }
            set { notes = value; }
        }
    }
}
