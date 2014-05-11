using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;
using System.Drawing.Drawing2D;

using UMD.HCIL.Piccolo;
using UMD.HCIL.Piccolo.Event;
using UMD.HCIL.Piccolo.Nodes;
using UMD.HCIL.Piccolo.Util;

namespace N7.Sprites
{
    class SectorBounds
    {
        public SectorBounds(PLayer layer, float x_min, float y_min, float x_max, float y_max)
        {
            float width = (x_max-x_min)/100;
            float height = (y_max-y_min)/100;
            float x = x_min / 100;
            float y = (y_min / 100) - (height / 2);

            Console.Out.WriteLine(width + ", " + height);

            Pen boundsPen = new Pen(Color.Red, 10.0F);
            boundsPen.DashStyle = DashStyle.DashDotDot;

            PPath boundsRectangle = PPath.CreateRectangle(x, y, width, height);
            boundsRectangle.Brush = Brushes.Transparent;
            boundsRectangle.Pen = boundsPen;

            layer.AddChild(boundsRectangle);
        }
    }
}
