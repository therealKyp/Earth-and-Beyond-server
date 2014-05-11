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
    class Sector
    {
        public Sector(PLayer layer, String name)
        {
            Random rnd = new Random((int)DateTime.Now.Ticks); // seeded with ticks
            Color penColor = Color.FromArgb((rnd.Next(0, 255)), (rnd.Next(0, 255)), (rnd.Next(0, 255)));

            Pen sigPen = new Pen(penColor, 2.0F);
            sigPen.DashStyle = DashStyle.DashDotDot;

            PPath sigCircle = PPath.CreateEllipse(100, 500, 100, 100);
            sigCircle.Pen = sigPen;
            sigCircle.Brush = Brushes.Transparent;

            PText pname = new PText(name);
            pname.TextBrush = Brushes.White;
            pname.TextAlignment = StringAlignment.Center;
            pname.X = sigCircle.X;
            pname.Y = sigCircle.Y;

            //Display Object by adding them to its layer
            layer.AddChild(pname);
            layer.AddChild(sigCircle);
        }
    }
}
