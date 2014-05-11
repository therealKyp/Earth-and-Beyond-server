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
    class SectorBoundsSprite
    {
        public SectorBoundsSprite(PLayer layer, float x_min, float y_min, float x_max, float y_max)
        {
            float width = (x_max-x_min)/100;
            float height = (y_max-y_min)/100;
            float x = -(width / 2);
            float y = -(height / 2);

            Pen boundsPen = new Pen(Color.Red, 10.0F);
            boundsPen.DashStyle = DashStyle.DashDotDot;
            Pen xEdgePen = new Pen(Color.White, 2.5F);
            xEdgePen.DashStyle = DashStyle.Solid;
            Pen yEdgePen = new Pen(Color.White, 2.5F);
            yEdgePen.DashStyle = DashStyle.Solid;

            PPath boundsRectangle = PPath.CreateRectangle(x, y, width, height);
            boundsRectangle.Brush = Brushes.Transparent;
            boundsRectangle.Pen = boundsPen;

            PPath xEdge = new PPath();
            xEdge.AddLine(-50, 0, 50, 0);
            xEdge.Pen = xEdgePen;

            PPath yEdge = new PPath();
            yEdge.AddLine(0, -50, 0, 50);
            yEdge.Pen = xEdgePen;

            PText xy = new PText();
            xy.TextBrush = Brushes.White;
            xy.TextAlignment = StringAlignment.Center;
            xy.Text = "0,0";
            xy.X = 5;
            xy.Y = 5;

            PText posX = new PText();
            posX.TextBrush = Brushes.White;
            posX.TextAlignment = StringAlignment.Center;
            posX.Text = "+X";
            posX.X = 52;
            posX.Y = -9;

            PText posY = new PText();
            posY.TextBrush = Brushes.White;
            posY.TextAlignment = StringAlignment.Center;
            posY.Text = "+Y";
            posY.X = -12;
            posY.Y = 52;

            boundsRectangle.AddChild(xy);
            boundsRectangle.AddChild(posX);
            boundsRectangle.AddChild(posY);
            boundsRectangle.AddChild(xEdge);
            boundsRectangle.AddChild(yEdge);
            boundsRectangle.Pickable = false;

            layer.AddChild(boundsRectangle);
        }
    }
}
