using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace N7.GUI
{
	/// <summary>
	/// Summary description for frmContrast.
	/// </summary>

	public class frmContrast : System.Windows.Forms.Form
	{
		public int BarValue;
		public IWindowsFormsEditorService _wfes;
		public System.Windows.Forms.TrackBar trackBar1;

		private System.Windows.Forms.Button btnTrack;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public frmContrast()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			TopLevel = false;
			btnTrack.Text = BarValue.ToString();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}


		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.btnTrack = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.SuspendLayout();
            // 
            // trackBar1
            // 
            this.trackBar1.LargeChange = 10;
            this.trackBar1.Location = new System.Drawing.Point(0, 8);
            this.trackBar1.Maximum = 100;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(152, 45);
            this.trackBar1.TabIndex = 0;
            this.trackBar1.TickFrequency = 10;
            this.trackBar1.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBar1.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            // 
            // btnTrack
            // 
            this.btnTrack.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnTrack.Location = new System.Drawing.Point(150, 16);
            this.btnTrack.Name = "btnTrack";
            this.btnTrack.Size = new System.Drawing.Size(35, 23);
            this.btnTrack.TabIndex = 2;
            this.btnTrack.Text = "45";
            this.btnTrack.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnTrack.Click += new System.EventHandler(this.btnTrack_Click);
            // 
            // frmContrast
            // 
            this.AcceptButton = this.btnTrack;
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(192, 70);
            this.ControlBox = false;
            this.Controls.Add(this.btnTrack);
            this.Controls.Add(this.trackBar1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmContrast";
            this.ShowInTaskbar = false;
            this.Closed += new System.EventHandler(this.frmContrast_Closed);
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		private void trackBar1_ValueChanged(object sender, System.EventArgs e)
		{
			BarValue = trackBar1.Value;
			btnTrack.Text = BarValue.ToString();
			
		}

		private void btnTrack_Click(object sender, System.EventArgs e)
		{
			Close();
						
		}

		private void frmContrast_Closed(object sender, System.EventArgs e)
		{
			_wfes.CloseDropDown();
			
		}
	}
}
