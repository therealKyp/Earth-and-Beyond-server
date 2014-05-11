using System;
using System.Data;
using System.Windows.Forms;
using System.Xml;

using N7.Utilities;
using N7.SQL;

namespace N7
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            //Attempt to load in the login information from the config file
            try
            {
                DataSet m_Data = new DataSet();
                DataTable m_Table;
                DataRow m_Row;

                m_Data.ReadXml(Application.StartupPath + "\\Config.xml");
                m_Table = m_Data.Tables[0];
                m_Row = m_Table.Rows[0];
                SQLData.Host = m_Row.ItemArray[0].ToString();
                SQLData.Port = Int32.Parse(m_Row.ItemArray[1].ToString());
                SQLData.User = m_Row.ItemArray[2].ToString();
                SQLData.Pass = m_Row.ItemArray[3].ToString();
            }
            catch (Exception)
            {
                SQLData.Host = "net-7.org";
                SQLData.Port = 3307;
                SQLData.User = "";
                SQLData.Pass = "";

                //MessageBox.Show("Failure loading in SQL configuration!\n" + e.ToString());
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Login LoginBox = new Login();

            // Add in the params
            LoginBox.LoginUsername.Text = SQLData.User;
            LoginBox.LoginPassword.Text = SQLData.Pass;
            LoginBox.SQLServer.Text = SQLData.Host;
            LoginBox.SQLPort.Text = SQLData.Port.ToString();
            // Show the box
            LoginBox.ShowDialog();

            // Load if not cancelled
            if (!LoginBox.m_Cancel)
            {
                Database.initiliazeDB(Database.DatabaseName.net7);

                String query = "SELECT * FROM versions where Ename='Faction';";
                DataTable mobEditor = Database.executeQuery(query);

                DataRow tmp = mobEditor.Rows[0];
                if (tmp["Version"].ToString() != "1.0.0.0")
                {
                    MessageBox.Show("Sorry, you have an old version, please update! \n Program will now exit!", "Version Check");
                    Database.closeDB();
                    Application.Exit();
                }
                else
                {
                    Application.Run(new N7.GUI.mainFrm());
                }
            }  
        }
    }
}