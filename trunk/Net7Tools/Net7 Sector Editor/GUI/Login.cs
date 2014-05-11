using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;
using System.Xml;

namespace N7
{
    public partial class Login : Form
    {
        public bool m_Cancel = false;
        public bool m_HasChanged;

        public Login()
        {
            InitializeComponent();
        }

        private void LoginButton_Click(object sender, EventArgs e)
        {
            try
            {
                updateData();
                MySqlConnection mySqlConnection = new MySqlConnection(SQLData.ConnStr("net7"));
                mySqlConnection.Open();
                mySqlConnection.Close();
                this.Close();
            }
            catch (MySqlException e1)
            {
                MessageBox.Show("N7 Login Encoutered a problem: \n\nBegin Message:\n" + e1.Message);
            }
        }

        private void ExitLogin_Click(object sender, EventArgs e)
        {
            m_Cancel = true;
            this.Close();
        }

        private void LoginChange(object sender, EventArgs e)
        {
            m_HasChanged = true;
        }

        /// <summary>
        ///   <para>Global form handler to catch the &lt;enter&gt; key and close the dialog</para>
        /// </summary>
        private void onEnterKey(object sender, System.Windows.Forms.KeyPressEventArgs e)
        {
            if (e.KeyChar.Equals('\r'))
            {
                try
                {
                    updateData();
                    MySqlConnection mySqlConnection = new MySqlConnection(SQLData.ConnStr("net7"));
                    mySqlConnection.Open();
                    mySqlConnection.Close();
                    this.Close();
                }
                catch (MySqlException e1)
                {
                    MessageBox.Show("N7 Login Encoutered a problem: \n\nBegin Message:\n" + e1.Message);
                }
            }
        }

        private void Login_Load(object sender, EventArgs e)
        {

        }

        private void updateData()
        {
            SQLData.User = this.LoginUsername.Text;
            SQLData.Pass = this.LoginPassword.Text;
            SQLData.Host = this.SQLServer.Text;
            SQLData.Port = System.Convert.ToInt32(this.SQLPort.Text, 10);

            // Save in config
            XmlTextWriter textWriter = new XmlTextWriter(Application.StartupPath + "\\Config.xml", null);

            textWriter.Formatting = Formatting.Indented;
            // Start writing
            textWriter.WriteStartDocument();
            // Start main element
            textWriter.WriteStartElement("Confg");
            // Write Host
            textWriter.WriteStartElement("Host");
            textWriter.WriteString(SQLData.Host.ToString());
            textWriter.WriteEndElement();
            // Write Port
            textWriter.WriteStartElement("Port");
            textWriter.WriteString(SQLData.Port.ToString());
            textWriter.WriteEndElement();
            // Write User
            textWriter.WriteStartElement("User");
            textWriter.WriteString(SQLData.User.ToString());
            textWriter.WriteEndElement();
            // Write Password
            textWriter.WriteStartElement("Pass");
            textWriter.WriteString(SQLData.Pass.ToString());
            textWriter.WriteEndElement();
            // Write it all to the file
            textWriter.WriteEndElement();
            textWriter.WriteEndDocument();
            // Close it
            textWriter.Close();
        }
    }
}