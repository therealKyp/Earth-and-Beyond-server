using System;
using System.Collections.Generic;
using System.Linq;
using System.Data;
using System.Xml;
using System.Windows.Forms;

namespace SQLBind
{
    static class Program
    {
        private static String Host, Port, User, Pass;
        private static String DataBase = "net7";

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
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
                Host = m_Row.ItemArray[0].ToString();
                Port = m_Row.ItemArray[1].ToString();
                User = m_Row.ItemArray[2].ToString();
                Pass = m_Row.ItemArray[3].ToString();
            }
            catch
            {
                Host = "net-7.org";
                Port = "3307";
                User = "";
                Pass = "";
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Login LoginBox = new Login();

            // Add in the params
            LoginBox.LoginUsername.Text = User;
            LoginBox.LoginPassword.Text = Pass;
            LoginBox.SQLServer.Text = Host;
            LoginBox.SQLPort.Text = Port;

            // Show the box
            LoginBox.ShowDialog();

            DB.SetLogin(Host, Port, DataBase, User, Pass);

            // Read the values back
            if (!LoginBox.m_Cancel && LoginBox.m_HasChanged)
            {
                DB.SetLogin(LoginBox.SQLServer.Text, LoginBox.SQLPort.Text,
                    DataBase, LoginBox.LoginUsername.Text, LoginBox.LoginPassword.Text);

                // Save in config
                XmlTextWriter textWriter = new XmlTextWriter(Application.StartupPath + "\\Config.xml", null);

                textWriter.Formatting = Formatting.Indented;
                // Start writing
                textWriter.WriteStartDocument();
                // Start main element
                textWriter.WriteStartElement("Confg");
                // Write Host
                textWriter.WriteStartElement("Host");
                textWriter.WriteString(LoginBox.SQLServer.Text);
                textWriter.WriteEndElement();
                // Write Port
                textWriter.WriteStartElement("Port");
                textWriter.WriteString(LoginBox.SQLPort.Text);
                textWriter.WriteEndElement();
                // Write User
                textWriter.WriteStartElement("User");
                textWriter.WriteString(LoginBox.LoginUsername.Text);
                textWriter.WriteEndElement();
                // Write Password
                textWriter.WriteStartElement("Pass");
                textWriter.WriteString(LoginBox.LoginPassword.Text);
                textWriter.WriteEndElement();
                // Write it all to the file
                textWriter.WriteEndElement();
                textWriter.WriteEndDocument();
                // Close it
                textWriter.Close();
            }            
            LoginBox.Close();

            // Load if not cancled
            if (!LoginBox.m_Cancel)
                Application.Run(new Form1());
        }
    }
}
