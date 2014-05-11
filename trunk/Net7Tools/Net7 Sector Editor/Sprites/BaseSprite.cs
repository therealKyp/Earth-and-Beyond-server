using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using SdlDotNet.Graphics;
using SdlDotNet.Graphics.Primitives;
using SdlDotNet.Graphics.Sprites;
using SdlDotNet.Core;
using SdlDotNet.Input;

namespace N7.Sprites
{
    class BaseSprite : Sprite
    {
        private bool FirstClick = true;

        public BaseSprite(Surface surface, Point coord)
            : base(surface, coord)
        {
            if (surface == null)
            {
                throw new ArgumentNullException("surfaces");
            }

            this.AllowDrag = true;
        }


        #region Event Update Methods
        /// <summary>
        /// If the mouse click hits a sprite, 
        /// then the sprite will be marked as 'being dragged'
        /// </summary>
        /// <param name="args"></param>
        public override void Update(MouseButtonEventArgs args)
        {
            if (args == null)
            {
                throw new ArgumentNullException("args");
            }
            if (this.IntersectsWith(new Point(args.X, args.Y)))
            {
                // If we are being held down, pick up the marble
                if (args.ButtonPressed)
                {
                    if (args.Button == MouseButton.PrimaryButton)
                    {
                        if (this.FirstClick)
                        {
                            this.BeingDragged = true;
                            this.FirstClick = false;
                            //TODO: Make Bounding Box controls appear!
                        }
                        else
                        {
                            this.FirstClick = true;
                            this.BeingDragged = false;
                            //TODO: Make Bounding Box controls dis-appear!
                        }
                    }
                    else
                    {
                        this.BeingDragged = false;
                        //TODO: Make Bounding Box controls dis-appear!
                    }
                }
                else
                {
                    this.BeingDragged = false;
                    //TODO: Make Bounding Box controls dis-appear!
                }
            }
        }

        /// <summary>
        /// If the sprite is picked up, this moved the sprite to follow
        /// the mouse.
        /// </summary>
        public override void Update(MouseMotionEventArgs args)
        {
            if (args == null)
            {
                throw new ArgumentNullException("args");
            }
            if (!AllowDrag)
            {
                return;
            }

            // Move the window as appropriate
            if (this.BeingDragged)
            {
                this.X += args.RelativeX;
                this.Y += args.RelativeY;
            }
        }
        #endregion Event Update Methods

        #region IDisposable
        private bool disposed;

        /// <summary>
        /// Destroys the surface object and frees its memory
        /// </summary>
        /// <param name="disposing">If ture, dispose unmanaged resources</param>
        protected override void Dispose(bool disposing)
        {
            try
            {
                if (!this.disposed)
                {
                    if (disposing)
                    {
                    }
                    this.disposed = true;
                }
            }
            finally
            {
                base.Dispose(disposing);
            }
        }
        #endregion IDisposable
    }
}
