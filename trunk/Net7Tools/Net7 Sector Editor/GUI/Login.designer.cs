namespace N7
{
    partial class Login
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Login));
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.LoginUsername = new System.Windows.Forms.TextBox();
            this.LoginPassword = new System.Windows.Forms.TextBox();
            this.ExitLogin = new System.Windows.Forms.Button();
            this.LoginButton = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.SQLServer = new System.Windows.Forms.TextBox();
            this.SQLPort = new System.Windows.Forms.TextBox();
            this.lable9 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(37, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Username:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(39, 41);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Password:";
            // 
            // LoginUsername
            // 
            this.LoginUsername.Location = new System.Drawing.Point(101, 12);
            this.LoginUsername.Name = "LoginUsername";
            this.LoginUsername.Size = new System.Drawing.Size(142, 20);
            this.LoginUsername.TabIndex = 3;
            this.LoginUsername.TextChanged += new System.EventHandler(this.LoginChange);
            this.LoginUsername.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.onEnterKey);
            // 
            // LoginPassword
            // 
            this.LoginPassword.Location = new System.Drawing.Point(101, 38);
            this.LoginPassword.Name = "LoginPassword";
            this.LoginPassword.PasswordChar = '*';
            this.LoginPassword.Size = new System.Drawing.Size(142, 20);
            this.LoginPassword.TabIndex = 4;
            this.LoginPassword.TextChanged += new System.EventHandler(this.LoginChange);
            this.LoginPassword.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.onEnterKey);
            // 
            // ExitLogin
            // 
            this.ExitLogin.Location = new System.Drawing.Point(18, 123);
            this.ExitLogin.Name = "ExitLogin";
            this.ExitLogin.Size = new System.Drawing.Size(77, 28);
            this.ExitLogin.TabIndex = 8;
            this.ExitLogin.Text = "Cancel";
            this.ExitLogin.UseVisualStyleBackColor = true;
            this.ExitLogin.Click += new System.EventHandler(this.ExitLogin_Click);
            // 
            // LoginButton
            // 
            this.LoginButton.Location = new System.Drawing.Point(189, 123);
            this.LoginButton.Name = "LoginButton";
            this.LoginButton.Size = new System.Drawing.Size(77, 28);
            this.LoginButton.TabIndex = 7;
            this.LoginButton.Text = "Login";
            this.LoginButton.UseVisualStyleBackColor = true;
            this.LoginButton.Click += new System.EventHandler(this.LoginButton_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(35, 67);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(56, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "SQL Host:";
            // 
            // SQLServer
            // 
            this.SQLServer.Location = new System.Drawing.Point(101, 64);
            this.SQLServer.Name = "SQLServer";
            this.SQLServer.Size = new System.Drawing.Size(142, 20);
            this.SQLServer.TabIndex = 5;
            this.SQLServer.TextChanged += new System.EventHandler(this.LoginChange);
            this.SQLServer.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.onEnterKey);
            // 
            // SQLPort
            // 
            this.SQLPort.Location = new System.Drawing.Point(101, 90);
            this.SQLPort.Name = "SQLPort";
            this.SQLPort.Size = new System.Drawing.Size(61, 20);
            this.SQLPort.TabIndex = 6;
            this.SQLPort.TextChanged += new System.EventHandler(this.LoginChange);
            this.SQLPort.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.onEnterKey);
            // 
            // lable9
            // 
            this.lable9.AutoSize = true;
            this.lable9.Location = new System.Drawing.Point(35, 93);
            this.lable9.Name = "lable9";
            this.lable9.Size = new System.Drawing.Size(53, 13);
            this.lable9.TabIndex = 8;
            this.lable9.Text = "SQL Port:";
            // 
            // Login
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(278, 158);
            this.Controls.Add(this.SQLPort);
            this.Controls.Add(this.lable9);
            this.Controls.Add(this.LoginButton);
            this.Controls.Add(this.ExitLogin);
            this.Controls.Add(this.SQLServer);
            this.Controls.Add(this.LoginPassword);
            this.Controls.Add(this.LoginUsername);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Login";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "N7 Sector Editor Login";
            this.Load += new System.EventHandler(this.Login_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        public System.Windows.Forms.TextBox LoginUsername;
        public System.Windows.Forms.TextBox LoginPassword;
        private System.Windows.Forms.Button ExitLogin;
        private System.Windows.Forms.Button LoginButton;
        private System.Windows.Forms.Label label3;
        public System.Windows.Forms.TextBox SQLServer;
        public System.Windows.Forms.TextBox SQLPort;
        private System.Windows.Forms.Label lable9;
    }
}