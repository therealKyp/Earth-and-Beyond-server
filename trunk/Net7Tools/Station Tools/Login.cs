using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;

namespace Station_Tools
{
    public partial class Login : Form
    {
        public bool m_Cancel = false;
        public bool m_Login = false;
        public bool m_HasChanged;

        public Login()
        {
            InitializeComponent();
        }

        private void LoginButton_Click(object sender, EventArgs e)
        {
            MySqlConnection mySqlConnection = null;
            try
            {
                SQLData.User = LoginUsername.Text;
                SQLData.Pass = LoginPassword.Text;
                SQLData.Host = SQLServer.Text;
                SQLData.Port = System.Convert.ToInt32(SQLPort.Text, 10);
                mySqlConnection = new MySqlConnection(SQLData.ConnStr("net7"));
                MySqlCommand m_Command = new MySqlCommand("SELECT `versions`.`Version` FROM `versions` WHERE `versions`.`EName` = 'Station'", mySqlConnection);

                mySqlConnection.Open();

                MySqlDataReader ReadData = m_Command.ExecuteReader();

                if (ReadData.HasRows)
                {
                    ReadData.Read();
                    if (ReadData.GetString(0) != System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString())
                    {
                        MessageBox.Show("You are running an old version.  Please update your editor.", "Old Version", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                        m_Cancel = true;
                    }
                    else
                    {
                        m_Cancel = false;
                        m_Login = true;
                    }
                }
                else
                {
                    m_Cancel = true;
                }
                this.Close();
            }
            catch (Exception connectionException)
            {
                System.Windows.Forms.MessageBox.Show(connectionException.Message, "Connection Error");
            }
            finally
            {
                if (mySqlConnection != null)
                {
                    mySqlConnection.Close();
                }
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

        private void Login_FormClosed(object sender, FormClosedEventArgs e)
        {
            // See if user clicked on the X
            if (m_Login == false)
            {
                // if so the it is cancled
                m_Cancel = true;
            }
        }

    }
}