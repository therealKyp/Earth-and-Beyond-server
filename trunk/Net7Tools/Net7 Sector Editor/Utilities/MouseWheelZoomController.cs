using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

using UMD.HCIL.Piccolo;
using UMD.HCIL.Piccolo.Event;
using UMD.HCIL.Piccolo.Nodes;
using UMD.HCIL.Piccolo.Util;

namespace N7.Utilities
{
    public class MouseWheelZoomController
    {
        public static float MIN_SCALE = .0001f;
        public static float MAX_SCALE = 2500;
        PCamera camera;

        public MouseWheelZoomController(PCamera camera)
        {
            this.camera = camera;
            camera.Canvas.ZoomEventHandler = null;
            camera.MouseWheel += OnMouseWheel;
        }

        public void OnMouseWheel(object o, PInputEventArgs ea)
        {
            float currentScale = camera.ViewScale;
            float scaleDelta = (1.0f + (0.001f * ea.WheelDelta));
            float newScale = currentScale * scaleDelta;
            if (newScale < MIN_SCALE)
            {
                camera.ViewScale = MIN_SCALE;
                return;
            }
            if ((MAX_SCALE > 0) && (newScale > MAX_SCALE))
            {
                camera.ViewScale = MAX_SCALE;
                return;
            }
            PointF pos = ea.Position;
            camera.ScaleViewBy(scaleDelta, pos.X, pos.Y);
        }
    }
}
