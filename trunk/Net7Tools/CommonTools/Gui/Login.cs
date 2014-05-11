using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using MySql.Data.MySqlClient;
using System.Xml;

namespace CommonTools.Gui
{
    public partial class Login : Form
    {
        private bool m_Cancel = false;
        private bool m_HasChanged;
        private Boolean m_updateVersion = false;

        public Login()
        {
            InitializeComponent();
        }

        public Boolean isValid()
        {
            return !m_Cancel;
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            readConfiguration();
            displayConfiguration();
        }

        private void readConfiguration()
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
                LoginData.Host = m_Row.ItemArray[0].ToString();
                LoginData.Port = Int32.Parse(m_Row.ItemArray[1].ToString());
                LoginData.User = m_Row.ItemArray[2].ToString();
                LoginData.Pass = m_Row.ItemArray[3].ToString();
            }
            catch (Exception)
            {
                LoginData.Host = "net-7.org";
                LoginData.Port = 3307;
                LoginData.User = "";
                LoginData.Pass = "";
            }
        }

        private void displayConfiguration()
        {
            // Add in the params
            LoginUsername.Text = LoginData.User;
            LoginPassword.Text = LoginData.Pass;
            SQLServer.Text = LoginData.Host;
            SQLPort.Text = LoginData.Port.ToString();
        }

        private void writeConfiguration()
        {
                LoginData.User = LoginUsername.Text;
                LoginData.Pass = LoginPassword.Text;
                LoginData.Host = SQLServer.Text;
                LoginData.Port = System.Convert.ToInt32(SQLPort.Text, 10);

                // Save in config
                XmlTextWriter textWriter = new XmlTextWriter(Application.StartupPath + "\\Config.xml", null);

                textWriter.Formatting = Formatting.Indented;
                // Start writing
                textWriter.WriteStartDocument();
                // Start main element
                textWriter.WriteStartElement("Confg");
                // Write Host
                textWriter.WriteStartElement("Host");
                textWriter.WriteString(LoginData.Host.ToString());
                textWriter.WriteEndElement();
                // Write Port
                textWriter.WriteStartElement("Port");
                textWriter.WriteString(LoginData.Port.ToString());
                textWriter.WriteEndElement();
                // Write User
                textWriter.WriteStartElement("User");
                textWriter.WriteString(LoginData.User.ToString());
                textWriter.WriteEndElement();
                // Write Password
                textWriter.WriteStartElement("Pass");
                textWriter.WriteString(LoginData.Pass.ToString());
                textWriter.WriteEndElement();
                // Write it all to the file
                textWriter.WriteEndElement();
                textWriter.WriteEndDocument();
                // Close it
                textWriter.Close();
        }

        private void LoginButton_Click(object sender, EventArgs e)
        {
            acceptedLoginInformation();
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
                acceptedLoginInformation();
            }
        }

        public void updateVersion()
        {
            m_updateVersion = true;
        }

        private void acceptedLoginInformation()
        {
            MySqlConnection mySqlConnection = null;
            m_Cancel = true;
            try
            {
                LoginData.User = LoginUsername.Text;
                LoginData.Pass = LoginPassword.Text;
                LoginData.Host = SQLServer.Text;
                LoginData.Port = System.Convert.ToInt32(SQLPort.Text, 10);
                mySqlConnection = new MySqlConnection(LoginData.ConnStr(Database.DB.DATABASE_NAME));
                mySqlConnection.Open();
                mySqlConnection.Close();

                if (m_updateVersion)
                {
                    System.Reflection.AssemblyName assemblyName = System.Reflection.Assembly.GetEntryAssembly().GetName();
                    Database.DB.Instance.setVersion(assemblyName.Name, assemblyName.Version.ToString());
                    MessageBox.Show("Database entry for "
                                   + assemblyName.Name
                                   + " has been updated to "
                                   + LoginData.ApplicationVersion,
                                    "Version Updated",
                                    MessageBoxButtons.OK,
                                    MessageBoxIcon.Exclamation);
                    return;
                }
                else
                {
                    // Verify the version
                    System.Reflection.AssemblyName assemblyName = System.Reflection.Assembly.GetEntryAssembly().GetName();
                    String currentVersion = Database.DB.Instance.getVersion(assemblyName.Name);
                    if (assemblyName.Version.ToString().CompareTo(currentVersion) != 0)
                    {
                        MessageBox.Show(assemblyName.Name + " "
                                       + LoginData.ApplicationVersion
                                       + " is outdated.\nPlease update your editor to "
                                       + LoginData.FormattedVersion(currentVersion),
                                        "Incorrect Version",
                                        MessageBoxButtons.OK,
                                        MessageBoxIcon.Exclamation);
                        return;
                    }
                }

                if (m_HasChanged)
                {
                    writeConfiguration();
                }

                m_Cancel = false;
                this.Close();
            }
            catch (Exception connectionException)
            {
                MessageBox.Show(connectionException.Message + "\n"
                                + connectionException.StackTrace + "\n"
                    /*+ LoginData.ConnStr(Database.DB.DatabaseName.net7.ToString())*/,
                                "Connection Error (" + connectionException.TargetSite.Name + ")");

                foreach (System.Collections.DictionaryEntry de in connectionException.Data)
                {
                    MessageBox.Show(String.Format("    The key is '{0}' and the value is: {1}", de.Key, de.Value));
                }

            }
            finally
            {
                if (mySqlConnection != null)
                {
                    mySqlConnection.Close();
                }
            }
        }
    }

    public static class LoginData
    {
        private static string m_Host;
        private static int m_Port;
        private static string m_User;
        private static string m_Pass;
        private static string m_ApplicationVersion;

        public static string Host
        {
            get { return m_Host; }
            set { m_Host = value; }
        }

        public static int Port
        {
            get { return m_Port; }
            set { m_Port = value; }
        }

        public static string User
        {
            get { return m_User; }
            set { m_User = value; }
        }

        public static string Pass
        {
            get { return m_Pass; }
            set { m_Pass = value; }
        }

        public static string ConnStr(string DB)
        {
            // http://dev.mysql.com/doc/refman/5.0/en/connector-net-examples-mysqlconnection.html#connector-net-examples-mysqlconnection-connectionstring
            return "Connect Timeout=30"
                 + ";Persist Security Info=False"
                 + ";Database=" + DB
                 + ";Host=" + m_Host
                 + ";Port=" + m_Port.ToString()
                 + ";Username=" + m_User
                 + ";Password=" + m_Pass;
        }

        public static string ApplicationVersion
        {
            get
            {
                System.Reflection.AssemblyName assemblyName = System.Reflection.Assembly.GetEntryAssembly().GetName();
                m_ApplicationVersion = FormattedVersion(assemblyName.Version.Major.ToString(),
                                                        assemblyName.Version.Minor.ToString(),
                                                        assemblyName.Version.Build.ToString(),
                                                        assemblyName.Version.Revision.ToString());
                return m_ApplicationVersion;
            }
        }

        public static string FormattedVersion(string version)
        {
            int majorVersion = version.IndexOf('.');
            int minorVersion = -1;
            int buildIndex = -1;
            int revisionIndex = -1;
            if (majorVersion == -1)
            {
                majorVersion = version.Length;
            }
            else
            {
                minorVersion = version.IndexOf('.', majorVersion + 1);
                if (minorVersion == -1)
                {
                    minorVersion = version.Length;
                }
                else
                {
                    buildIndex = version.IndexOf('.', minorVersion + 1);
                    if (buildIndex == -1)
                    {
                        buildIndex = version.Length;
                    }
                    else
                    {
                        revisionIndex = version.IndexOf('.', buildIndex + 1);
                        if (revisionIndex == -1)
                        {
                            revisionIndex = version.Length;
                        }
                    }
                }
            }
            String major = majorVersion == -1 ? "?" : version.Substring(0, majorVersion);
            String minor = majorVersion == -1 || minorVersion == -1 ? "?" : version.Substring(majorVersion + 1, minorVersion - majorVersion - 1);
            String build = minorVersion == -1 || buildIndex == -1 ? "?" : version.Substring(minorVersion + 1, buildIndex - minorVersion - 1);
            String revision = buildIndex == -1 || revisionIndex == -1 ? "?" : version.Substring(buildIndex + 1, revisionIndex - buildIndex - 1);
            return FormattedVersion(major, minor, build, revision);
        }

        public static string FormattedVersion(string major, string minor, string build, string revision)
        {
            String version = "v"
                           + (major.Length == 0 ? "?" : major)
                           + "."
                           + (minor.Length == 0 ? "?" : minor)
                           + " Build "
                           + (build.Length == 0 ? "?" : build)
                           + "."
                           + (revision.Length == 0 ? "?" : revision);
            return version;
        }
    }

}