using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace DataImport
{
    static class Program
    {
        enum ApplicationAction { GenerateDatabaseStructure, UpdateVersion, Run };
        public static DataImport m_dataImport = null;

        [STAThread]
        static void Main()
        {

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            ApplicationAction applicationAction = ApplicationAction.Run;

            CommonTools.Gui.Login login = new CommonTools.Gui.Login();
            if (applicationAction.Equals(ApplicationAction.UpdateVersion))
            {
                login.updateVersion();
            }
            login.ShowDialog();
            if (login.isValid())
            {
                switch (applicationAction)
                {
                    case ApplicationAction.GenerateDatabaseStructure:
                        CommonTools.Database.DB.Instance.makeDatabaseVariables();
                        break;
                    case ApplicationAction.Run:
                        {
                            m_dataImport = new DataImport();
                            m_dataImport.ShowDialog();
                        }
                        break;
                } // switch (applicationAction)
            } // if (login.isValid())

        }
    }
}
