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
    class Mob
    {
        public Mob(PLayer layer, String name, float x, float y, float sigRadius, float rrRadius)
        {
            float sigDia = (sigRadius * 2) / 100;
            float rrDia = (rrRadius * 2) / 100;

            string dataDirectory = "Images";
            string filePath = Path.Combine("..", "..");
            if (File.Exists(Path.Combine(dataDirectory, "hostileMob.gif")))
            {
                filePath = "";
            }

            Image image = Image.FromFile(Path.Combine(filePath, Path.Combine(dataDirectory, "hostileMob.gif")));
            PImage stationImage = new PImage(image);
            stationImage.X = (x - (image.Width/2)) / 100;
            stationImage.Y = (y - (image.Height/2)) / 100;

            float sigX = (x / 100) - ((sigDia / 2) - (image.Width / 2));
            float sigY = (y / 100) - ((sigDia / 2) - (image.Height / 2));
            float rrX = (x / 100) - ((rrDia / 2) - (image.Width / 2));
            float rrY = (y / 100) - ((rrDia / 2) - (image.Height / 2));

            Pen sigPen = new Pen(Color.Red, 2.0F);
            sigPen.DashStyle = DashStyle.DashDotDot;
            Pen rrPen = new Pen(Color.MistyRose, 1.0F);

            PPath sigCircle = PPath.CreateEllipse(sigX, sigY, sigDia, sigDia);
            sigCircle.Pen = sigPen;
            PPath rrCircle = PPath.CreateEllipse(rrX, rrY, rrDia, rrDia);
            rrCircle.Pen = rrPen;

            PNode sigNode = sigCircle;
            sigNode.Brush = Brushes.Transparent;

            PNode rrNode = rrCircle;
            rrNode.Brush = Brushes.Transparent;

            PText pname = new PText(name);
            pname.TextBrush = Brushes.White;
            pname.TextAlignment = StringAlignment.Center;
            pname.X = (x / 100) - (pname.Width / 2);
            pname.Y = (y / 100) - 20;

            stationImage.AddChild(sigNode);
            stationImage.AddChild(rrNode);
            stationImage.AddChild(pname);

            //Display Object by adding them to its layer
            layer.AddChild(stationImage);
        }
    }
}
