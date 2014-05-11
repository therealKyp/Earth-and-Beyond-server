using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;
using MissionEditor.Gui;
using CommonTools.Database;

namespace MissionEditor
{
    static class Program
    {
        enum ApplicationAction { GenerateDatabaseStructure, UpdateVersion, Run, Search };

        public static FrmMission m_frmMission = null;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(String[] args)
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
                switch(applicationAction)
                {
                    case ApplicationAction.GenerateDatabaseStructure:
                        CommonTools.Database.DB.Instance.makeDatabaseVariables();
                        break;
                    case ApplicationAction.Run:
                        {
                            m_frmMission = new FrmMission();
                            m_frmMission.ShowDialog();
                            //System.Console.WriteLine("\n\n" + frmMission.getMissionXml() + "\n\n");
                            //Clipboard.SetText(frmMission.getMissionXml());
                            /*String missionReport = m_frmMission.getMission().getReport();
                            System.IO.File.Delete("Mission.htm");
                            StreamWriter streamWriter = new StreamWriter("Mission.htm");
                            streamWriter.Write(missionReport);
                            streamWriter.Close();*/
                            //FileStream fileStream = System.IO.File.Open("Mission.hml", FileMode.Truncate);
                            //fileStream.Write((byte[])missionReport.ToCharArray(), 0, missionReport.Length);
                            //System.Console.WriteLine("\n\n" + m_frmMission.getMission().getReport() + "\n\n");
                        }
                        break;
                    case ApplicationAction.Search:
                        {
                            CommonTools.Gui.DlgSearch m_dlgSearch = new CommonTools.Gui.DlgSearch();
                            Boolean tryItems = false;
                            if (tryItems)
                                m_dlgSearch.configure(Net7.Tables.item_base);
                            else
                                m_dlgSearch.configure(Net7.Tables.missions);
                            m_dlgSearch.ShowDialog();
                        }
                        break;
                }

            }
        }
    }
}
